//--------------------------------------------------------------------------------------
//
// プレイヤー
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "player.h"
#include "game.h"
#include "application.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "sound.h"
#include "result.h"
#include "ranking.h"
#include "score.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>

//--------------------------------------
//定数定義
//--------------------------------------
static const float SCREEN_WIDH = 1280.0f;			//画面の当たり判定で使用する横幅
static const float SCREEN_HIGHT = 720.0f;			//画面の当たり判定で使用する縦幅
static const float COLLISION_SIZE = 14.0f;			//画面の当たり判定で使用するプレイヤーのサイズ
static const float PLAYER_MOVEMENT = 0.62f;			//プレイヤーの移動量
static const int MAX_COUNT = 80;					//弾発射カウントの最大数

//--------------------------------------
//コンストラクタ
//--------------------------------------
CPlayer::CPlayer()
{
	m_nCount = 0;
	m_nPi = 0;
	m_nRand = 0;
	m_nPressCount = 0;
	m_nLife = 0;
	m_Damage = 0;
	m_nColorCount = 0;
	m_col = (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_bSetTexture = false;
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CPlayer::~CPlayer()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CPlayer::Init()
{
	CObject2D::Init();										//オブジェクト2Dの初期化
	CObject2D::SetTexture(CTexture::TEXTURE_PLAYER);		//テクスチャの配置
	srand((unsigned int)time(nullptr));						//ランダムの初期化
	m_nLife = 99;

	return S_OK;
}

//--------------------------------------
//クリエイト
//--------------------------------------
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd)
{
	CPlayer *m_pPlayer = new CPlayer;		//プレイヤーの動的確保

	if (m_pPlayer != nullptr)
	{//プレイヤーのポインターに値が入っていなかったら
		m_pPlayer->Init();
		m_pPlayer->SetObjectType(OBJTYPE_PLAYER);
		m_pPlayer->Setpos(pos);
		m_pPlayer->SetSize(ObfHi, ObfWd);
	}
	else
	{
		assert(false);
	}

	return m_pPlayer;
}

//--------------------------------------
//更新
//--------------------------------------
void CPlayer::Update()
{
	if (m_nLife <= 0)
	{
		Uninit();
		CRanking::Set(CScore::Get());
		CResult::Set(1);
		CApplication::SetMode(CApplication::MODE::MODE_RESULT);
		return;				//更新にこれ以上入らないように返す
	}

	CObject2D::Update();

	CObject2D::SetColor(m_col);

	//操作の処理の呼び出し
	Command();

	//弾操作の処理の呼び出し
	CommandBullet();

	//移動系統の処理の呼び出し
	Move();

	//当たり判定系統の処理の呼び出し
	Collision();

	//状態管理
	State();

#ifdef _DEBUG
	// リザルトに移行
	
	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_R))
	{//Rキーが押されたら
		CResult::Set(1);
		CRanking::Set(CScore::Get());
		CApplication::SetMode(CApplication::MODE::MODE_RESULT);
	}

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_H))
	{
		HitDamage(19);
	}

#endif // _DEBUG
}

//--------------------------------------
//描画
//--------------------------------------
void CPlayer::Draw()
{
	CObject2D::Draw();
}

//--------------------------------------
//終了
//--------------------------------------
void CPlayer::Uninit()
{
	CObject2D::Uninit();
}

//--------------------------------------
//プレイヤーの操作処理
//--------------------------------------
void CPlayer::Command()
{
/* プレイヤーの移動 */

	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_W))
	{//Wキーが押されたら
		m_move.x += sinf(D3DX_PI) * PLAYER_MOVEMENT;
		m_move.y += cosf(D3DX_PI) * PLAYER_MOVEMENT;
	}
	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_S))
	{//Sキーが押されたら
		m_move.x += sinf(D3DX_PI * 0.0f) * PLAYER_MOVEMENT;
		m_move.y += cosf(D3DX_PI * 0.0f) * PLAYER_MOVEMENT;
	}
	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_A))
	{
		//移動量の更新
		m_move.x += sinf(-D3DX_PI * 0.5f) * PLAYER_MOVEMENT;
		m_move.y += cosf(-D3DX_PI * 0.5f) * PLAYER_MOVEMENT;
	}
	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_D))
	{
		m_move.x += sinf(D3DX_PI * 0.5f) * PLAYER_MOVEMENT;
		m_move.y += cosf(D3DX_PI * 0.5f) * PLAYER_MOVEMENT;
	}
}

//--------------------------------------
//プレイヤーの弾処理
//--------------------------------------
void CPlayer::CommandBullet()
{
/*変数群*/
	m_nCount++;																	//弾発射用カウントの加算
	bool bSpace = CApplication::GetKyeboard()->GetkeyboardPress(DIK_SPACE);		//スペースキー押込みの代行
	bool bBpush = CApplication::GetKyeboard()->GetkeyboardPress(DIK_B);			//Bキー押込みの代行
	bool bNpush = CApplication::GetKyeboard()->GetkeyboardPress(DIK_N);			//Nキー押込みの代行
	bool bMarisa = CApplication::GetKyeboard()->GetkeyboardPress(DIK_M);		//Mキー押込みの代行(1度しか使われていないが、弾の操作系統の統一のため変数化)
	CBullet::BULLET_USER CUser = CBullet::BULLET_USER::USER_PLAYER;				//ユーザー情報の代行

/* 通常弾の処理 */

	if (bSpace)
	{//スペースキーが押されたら通常弾を生成
		if (m_nCount <= 10)
		{
			if (m_nCount % 3 == 0)
			{
				CApplication::GetSound()->Play(CSound::SOUND_SE_SOOT);
				CBullet::Create(m_pos,
					D3DXVECTOR3(sinf(D3DX_PI * 0.5f),
						cosf(D3DX_PI * 0.5f), 0.0f) * 25.0f,
					CTexture::TEXTURE_BULLET,
					CUser);
			}
		}
		if (m_nCount >= MAX_COUNT)
		{
			m_nCount = 0;
		}
	}

/* ランダム方向のバースト弾の処理 */

	if (bSpace == false)
	{//スペースキーが押されなかったら
		if (bBpush)
		{//ランダムバースト弾の発射処理
			if (m_nCount <= 12)
			{
				CApplication::GetSound()->Play(CSound::SOUND_SE_SOOT);
				m_nPi = D3DX_PI * 200.0f;									//角度計算
				m_nRand = rand() % m_nPi;								//ランダム化
				CBullet::Create(m_pos,
					D3DXVECTOR3(sinf(m_nRand * 0.01f),
						cosf(m_nRand * 0.01f), 0.0f) * 5.0f,
					CTexture::TEXTURE_BULLET,
					CUser);
			}
			if (m_nCount >= MAX_COUNT)
			{
				m_nCount = 0;
			}
		}
	}

/* 3方向の弾の処理 */

	if (bSpace == false &&
		bBpush == false)
	{//スペースキーとBキーが押されなかったら
		if (bNpush)
		{//3方向の弾発射処理
			if (m_nCount <= 12)
			{
				if (m_nCount % 4 == 0)
				{
					CApplication::GetSound()->Play(CSound::SOUND_SE_SOOT);
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						CBullet::Create(m_pos,
							D3DXVECTOR3(sinf(D3DX_PI * 0.5f),
								cosf(D3DX_PI * 0.25f + 0.78f * nCnt),
								0.0f) * 12.0f,
							CTexture::TEXTURE_BULLET,
							CUser);
					}
				}
			}
			
			if (m_nCount >= MAX_COUNT)
			{
				m_nCount = 0;
			}
		}
	}
	
/* 疑似マスパの処理 */
	if (bSpace == false &&
		bBpush == false &&
		bNpush == false)
	{//スペースキーとBキーとNキーが押されなかったら
		if (bMarisa)
		{//疑似マスパーの発射処理
			if (m_nCount <= 1)
			{
				CApplication::GetSound()->Play(CSound::SOUND_SE_SOOT);
				for (int nCnt = 0; nCnt < 8; nCnt++)
				{
					CBullet::Create(m_pos,
						D3DXVECTOR3(sinf(D3DX_PI * 0.5f),
							cosf(D3DX_PI * 0.25f + 0.25f * nCnt),
							0.0f) * 14.0f,
						CTexture::TEXTURE_BULLET,
						CUser);
				}
			}
			if (m_nCount >= MAX_COUNT)
			{
				m_nCount = 0;
			}
		}
	}
}

//--------------------------------------
//プレイヤーの移動系統処理
//--------------------------------------
void CPlayer::Move()
{
	//プレイヤーの位置更新
	//位置を更新
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//移動量を更新
	m_move.x += (0.0f - m_move.x) * 0.08f;
	m_move.y += (0.0f - m_move.y) * 0.08f;
}

//--------------------------------------
//プレイヤーの当たり判定系統処理
//--------------------------------------
void CPlayer::Collision()
{
	if (m_pos.y >= SCREEN_HIGHT - COLLISION_SIZE)
	{//下橋(画面下兼画面端)
		m_pos.y = SCREEN_HIGHT - COLLISION_SIZE;
	}
	else if (m_pos.y <= 0 + COLLISION_SIZE)
	{//上橋(画面上兼画面端)
		m_pos.y = 0 + COLLISION_SIZE;
	}
	if (m_pos.x >= SCREEN_WIDH - COLLISION_SIZE)
	{//ミギー橋(画面右兼画面端)
		m_pos.x = SCREEN_WIDH - COLLISION_SIZE;
	}
	else if (m_pos.x <= 0 + COLLISION_SIZE)
	{//左橋(画面左兼画面端)
		m_pos.x = 0 + COLLISION_SIZE;
	}
}

//--------------------------------------
//プレイヤーの状態管理処理
//--------------------------------------
void CPlayer::State()
{
	if (m_State == STARE_DAMAGE)
	{
		m_nColorCount++;
		if (m_nColorCount >= 240)
		{
			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_nColorCount = 0;
			//敵の状態を戻す
			m_State = STARE_NOMAL;
		}
	}
}

//--------------------------------------
//プレイヤーのダメージ処理
//--------------------------------------
void CPlayer::HitDamage(int nDamage)
{
	m_nLife -= nDamage;

	SetState(STARE_DAMAGE);

	if (m_nLife <= 99 && m_nLife >= 80)
	{
		SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}

	if (m_nLife < 80 && m_nLife >= 60)
	{
		if (m_bSetTexture != true)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER_VER2);		//テクスチャの配置
			m_bSetTexture = true;
		}
		SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}

	if (m_nLife < 60 && m_nLife >= 40)
	{
		if (m_bSetTexture != false)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER_VER3);		//テクスチャの配置
			m_bSetTexture = false;
		}
		SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	if (m_nLife < 40 && m_nLife >= 0)
	{
		if (m_bSetTexture != true)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER_VER4);		//テクスチャの配置
			m_bSetTexture = true;
		}
		SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

//--------------------------------------
//プレイヤーの状態設定
//--------------------------------------
void CPlayer::SetState(CPlayer::PLAYER_STATE state)
{
	m_State = state;
}
