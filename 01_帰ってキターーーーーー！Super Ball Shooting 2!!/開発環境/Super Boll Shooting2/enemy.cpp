//--------------------------------------------------------------------------------------
//
// エネミー
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "enemy.h"
#include "application.h"
#include "game.h"
#include "score.h"
#include "explosion.h"
#include "bullet.h"
#include "sound.h"
#include "protect_city.h"
#include <assert.h>
#include <time.h>

//--------------------------------------
//コンストラクタ
//--------------------------------------
CEnemy::CEnemy()
{
	m_nECount = 0;
	m_nELife = 0;
	m_fSpeed_sin = 0.0f;
	m_fSpeed_cos = 0.0f;
	m_nSwitch = 0;
	m_nOpen_Fire = 0;
	m_nColorCount = 0;
	m_col = (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CEnemy::~CEnemy()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CEnemy::Init()
{
	CObject2D::Init();
	srand((unsigned int)time(nullptr));						//ランダムの初期化

	return S_OK;
}

//--------------------------------------
//クリエイト
//--------------------------------------
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, int nlife, float ObfHi, float ObfWd,CTexture::TEXTURE tex,CObject::ObjType typ,CEnemy::ENEMY_TYPE type,float sin, float cos)
{
	CEnemy *m_pEnemy = new CEnemy;

	if (m_pEnemy != nullptr)
	{
		m_pEnemy->m_nELife = nlife;
		m_pEnemy->Init();
		m_pEnemy->SetObjectType(typ);
		m_pEnemy->Setpos(pos);
		m_pEnemy->SetTexture(tex);
		m_pEnemy->SetSize(ObfHi, ObfWd);
		m_pEnemy->NumTypeEnemy(type, sin, cos);
	}
	else
	{
		assert(false);
	}

	return m_pEnemy;
}

//--------------------------------------
//更新
//--------------------------------------
void CEnemy::Update()
{

	CObject2D::Update();

	CObject2D::SetColor(m_col);

	m_nOpen_Fire++;					//弾を発射するまでのカウントを加算

	m_nECount++;					//発射カウントの加算

								
	NumTypeEnemy(m_nType,			//エネミーの種別を設定
				 m_fSpeed_sin,
				 m_fSpeed_cos);	

	ShotBullet();					//撃つ弾の設定

	Updatepos();					//位置更新

	State();						//エネミーの状態管理

	Relesepos();					//エネミーの開放位置
}

//--------------------------------------
//描画
//--------------------------------------
void CEnemy::Draw()
{
	CObject2D::Draw();
}

//--------------------------------------
//終了
//--------------------------------------
void CEnemy::Uninit()
{
	CObject2D::Uninit();
}

//--------------------------------------
//エネミーの種類の設定
//--------------------------------------
void CEnemy::NumTypeEnemy(CEnemy::ENEMY_TYPE type, float fsin, float fcos)
{
	m_nType = type;

	m_fSpeed_sin = fsin;

	m_fSpeed_cos = fcos;

	switch (type)
	{
/*エネミーが使用されていない状態*/

	case CEnemy::TYPE_NOME:

		/*変更なし*/

		break;

/*チュートリアルエネミーの機動*/

	case CEnemy::TYPE_TUTORIAL:

		m_move.x = sinf(D3DX_PI * 0.5f) * fsin/*-0.8f*/;
		m_move.y = cosf(D3DX_PI) * fcos/*0.0f*/;

		if (m_pos.x <= 1080.0f)
		{
			m_move.x = sinf(D3DX_PI * 0.5f) * 0.0f;
			m_move.y = cosf(D3DX_PI) * 0.0f;
		}
		
		if (m_nOpen_Fire >= 600)
		{//弾発射カウントが1分を超えたら

			m_nType = TYPE_TUTORIAL_BACK;
		}

		break;

	case CEnemy::TYPE_TUTORIAL_BACK:

		m_move.x = sinf(D3DX_PI * 0.5f) * 1.8f;
		m_move.y = cosf(D3DX_PI) * 0.0f;

		break;

/*真っ直ぐ進むエネミー*/

	case CEnemy::TYPE_STRAIGHT:

		m_move.x = sinf(D3DX_PI * 0.5f) * fsin /*-1.12f*/;
		m_move.y = cosf(D3DX_PI) * fcos/*0.0f*/;

		break;

/*壁に反射するエネミー*/

	case CEnemy::TYPE_COLLISION_DOWN:

		m_move.x = -0.9f;
		m_move.y = 1.24f;

		if (m_pos.y >= 720.0f - 30.0f)
		{//下端ラインに当たったとき
			m_move.y *= -1.0f;
			m_nType = TYPE_COLLISION_UP;
		}
		break;

	case CEnemy::TYPE_COLLISION_UP:

		m_move.x = -0.9f;
		m_move.y = -1.24f;

		if (m_pos.y <= 0 + 20.0f)
		{//上端ラインに当たったとき
			m_move.y *= -1.0f;
			m_nType = TYPE_COLLISION_DOWN;
		}
		break;

/*変な動きをするエネミー*/

	case CEnemy::TYPE_STRANGER_DOWN:

		m_move.x = -0.9f;
		m_move.y = 1.24f;

		if ((int)m_pos.y <= (int)(720.0f / 2))
		{
			m_nType = TYPE_STRENGER_GO;
		}

		break;

	case CEnemy::TYPE_STRANGER_UP:

		m_move.x = -0.9f;
		m_move.y = -1.24f;

		if ((int)m_pos.y >= (int)(720.0f / 2))
		{
			m_nType = TYPE_STRENGER_GO;
		}

		break;

	case CEnemy::TYPE_STRENGER_GO:

		m_move.x = sinf(D3DX_PI * 0.5f) * -3.0f;
		m_move.y = cosf(D3DX_PI) * 0.0f;

		if (m_pos.x <= 840.0f)
		{
			m_nType = TYPE_STRENGER_CHANGE;
		}

		break;

	case CEnemy::TYPE_STRENGER_CHANGE:

		switch (rand() % 3)
		{
		case 0:

			m_nType = TYPE_COLLISION_DOWN;

			break;

		case 1:

			m_nType = TYPE_STRAIGHT;

			break;

		case 2:

			m_nType = TYPE_COLLISION_UP;

			break;

		default:
			break;
		}

		break;

/*ボスクラスのエネミー*/

	case CEnemy::TYPE_MIDBOSS:

		m_move.x = sinf(-D3DX_PI * 0.5f) * fsin/*0.4f*/;
		m_move.y = cosf(-D3DX_PI * 0.5f) * fcos/*0.4f*/;

		break;

/*ボスのエネミー*/

	case CEnemy::TYPE_BOSS:
	case CEnemy::TYPE_BOSS_TWO:
	case CEnemy::TYPE_BOSS_TREE:

		m_move.x = sinf(-D3DX_PI * 0.5f) * fsin;
		m_move.y = cosf(-D3DX_PI * 0.5f) * fcos;

		if (m_pos.x <= 1080.0f)
		{
			m_move.x = 0.0f;
			m_move.y = 1.24f;
			if (m_nType == TYPE_BOSS)
			{
				m_nType = TYPE_BOSS_DOWN;
			}
			if (m_nType == TYPE_BOSS_TWO)
			{
				m_nType = TYPE_BOSS_TWO_DOWN;
			}
			if (m_nType == TYPE_BOSS_TREE)
			{
				m_nType = TYPE_BOSS_TREE_DOWN;
			}
		}

		break;

	case CEnemy::TYPE_BOSS_DOWN:
	case CEnemy::TYPE_BOSS_TWO_DOWN:
	case CEnemy::TYPE_BOSS_TREE_DOWN:

		//m_move.y = cosf(D3DX_PI * 0.5f) * 1.24f;

		if (m_pos.y >= 720.0f - 30.0f)
		{//下端ラインに当たったとき
			m_move.y *= -1.0f;
			if (m_nType == TYPE_BOSS_DOWN)
			{
				m_nType = TYPE_BOSS_UP;
			}
			if (m_nType == TYPE_BOSS_TWO_DOWN)
			{
				m_nType = TYPE_BOSS_TWO_UP;
			}
			if (m_nType == TYPE_BOSS_TREE_DOWN)
			{
				m_nType = TYPE_BOSS_TREE_UP;
			}
		}

		break;

	case CEnemy::TYPE_BOSS_UP:
	case CEnemy::TYPE_BOSS_TWO_UP:
	case CEnemy::TYPE_BOSS_TREE_UP:

		if (m_pos.y <= 0 + 20.0f)
		{//上端ラインに当たったとき
			m_move.y *= -1.0f;
			if (m_nType == TYPE_BOSS_UP)
			{
				m_nType = TYPE_BOSS_DOWN;
			}
			if (m_nType == TYPE_BOSS_TWO_UP)
			{
				m_nType = TYPE_BOSS_TWO_DOWN;
			}
			if (m_nType == TYPE_BOSS_TREE_UP)
			{
				m_nType = TYPE_BOSS_TREE_DOWN;
			}
		}

		break;

	default:
		break;
	}
}

//--------------------------------------
//エネミーに当たったとき
//--------------------------------------
void CEnemy::HitLife(int nDamage)
{
	m_nELife -= nDamage;

	if (m_nELife <= 0)
	{
		switch (m_nType)
		{
		case CEnemy::TYPE_STRAIGHT:
			
			CGame::GetScore()->Add(100);

			break;

		case CEnemy::TYPE_STRANGER_DOWN:
		case CEnemy::TYPE_STRANGER_UP:
		case CEnemy::TYPE_STRENGER_GO:
		case CEnemy::TYPE_COLLISION_DOWN:
		case CEnemy::TYPE_COLLISION_UP:

			CGame::GetScore()->Add(250);

			break;

		case CEnemy::TYPE_STRENGER_CHANGE:

			CGame::GetScore()->Add(400);

			break;

		case CEnemy::TYPE_MIDBOSS:

			CGame::GetScore()->Add(600);

			break;

		case CEnemy::TYPE_BOSS:
		case CEnemy::TYPE_BOSS_DOWN:
		case CEnemy::TYPE_BOSS_UP:

			CGame::GetScore()->Add(1000);

		case CEnemy::TYPE_BOSS_TWO:
		case CEnemy::TYPE_BOSS_TWO_DOWN:
		case CEnemy::TYPE_BOSS_TWO_UP:

			CGame::GetScore()->Add(2000);

			break;

		case CEnemy::TYPE_BOSS_TREE:
		case CEnemy::TYPE_BOSS_TREE_DOWN:
		case CEnemy::TYPE_BOSS_TREE_UP:

			CGame::GetScore()->Add(7000);

			break;

		default:
			break;
		}
		CApplication::GetSound()->Play(CSound::SOUND_SE_EXPLOSION);
		Uninit();
		return;				//これ以上更新に入らないように返す
	}
}

//--------------------------------------
//エネミーの弾発射設定
//--------------------------------------
void CEnemy::ShotBullet()
{
	switch (m_nType)
	{
	case CEnemy::TYPE_NOME:
	case CEnemy::TYPE_TUTORIAL:
	case CEnemy::TYPE_TUTORIAL_BACK:

		/*チュートリアル・タイプ未使用じゃ弾は撃てん…*/

		break;

	case CEnemy::TYPE_STRAIGHT:
	case CEnemy::TYPE_COLLISION_DOWN:
	case CEnemy::TYPE_COLLISION_UP:
	case CEnemy::TYPE_STRANGER_DOWN:
	case CEnemy::TYPE_STRANGER_UP:
	case CEnemy::TYPE_STRENGER_GO:
	case CEnemy::TYPE_STRENGER_CHANGE:
	case CEnemy::TYPE_MIDBOSS:
	case CEnemy::TYPE_BOSS:
	case CEnemy::TYPE_BOSS_DOWN:
	case CEnemy::TYPE_BOSS_UP:

		if (m_nECount <= 1 &&
			m_pos.x <= 1280.0f)
		{
			CBullet::Create(m_pos,
				D3DXVECTOR3(sinf(D3DX_PI * -0.5f),
					cosf(D3DX_PI * -0.5f), 0.0f) * 25.0f,
				CTexture::TEXTURE_BULLET_E,
				CBullet::BULLET_USER::USER_ENEMY);
		}

		if (m_nECount >= 120)
		{
			m_nECount = 0;
		}

		break;

	case CEnemy::TYPE_BOSS_TWO:
	case CEnemy::TYPE_BOSS_TWO_DOWN:
	case CEnemy::TYPE_BOSS_TWO_UP:

		if (m_nECount <= 1)
		{
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CBullet::Create(m_pos,
					D3DXVECTOR3(sinf(D3DX_PI * -0.5f),
						cosf(D3DX_PI * -0.25f + (-0.78f * nCnt)),
						0.0f) * 24.0f,
					CTexture::TEXTURE_BULLET_E,
					CBullet::BULLET_USER::USER_ENEMY);
			}
		}

		if (m_nECount >= 100)
		{
			m_nECount = 0;
		}

		break;

	case CEnemy::TYPE_BOSS_TREE:
	case CEnemy::TYPE_BOSS_TREE_DOWN:
	case CEnemy::TYPE_BOSS_TREE_UP:
		
		if (m_nECount <= 1)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				CBullet::Create(m_pos,
					D3DXVECTOR3(sinf(D3DX_PI * - 0.5f),
						cosf(D3DX_PI * - 0.25f - 0.25f * nCnt),
						0.0f) * 26.0f,
					CTexture::TEXTURE_BULLET_E,
					CBullet::BULLET_USER::USER_ENEMY);
			}
		}

		if (m_nECount >= 70)
		{
			m_nECount = 0;
		}

		break;

	default:
		break;
	}
}

//--------------------------------------
//エネミーの位置更新
//--------------------------------------
void CEnemy::Updatepos()
{
	//位置を更新
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
}

//--------------------------------------
//エネミーの状態管理
//--------------------------------------
void CEnemy::State()
{
	if (m_nState == STARE_DAMAGE)
	{
		m_nColorCount++;
		if (m_nColorCount >= 10)
		{
			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_nColorCount = 0;
			//敵の状態を戻す
			m_nState = STARE_NOMAL;
		}
	}
}

//--------------------------------------
//エネミーの開放設定
//--------------------------------------
void CEnemy::Relesepos()
{
	if (m_pos.x >= 1500.0f)
	{
		CObject2D::Release();
		return;
	}
	
	if (m_pos.x <= -30.0f && m_pos.y >= 0.0f && m_pos.y <= 720.0f)
	{//左橋(画面左兼画面端)
		if (CGame::GetProtect_City()->GetLife() > 0)
		{
			CGame::GetProtect_City()->SetState(Cprotect_City::CITY_STATE::STARE_DAMAGE);
			CGame::GetProtect_City()->Damage(1);
		}
		
		CObject2D::Release();
		return;
	}
}

//--------------------------------------
//エネミーのカラー設定
//--------------------------------------
void CEnemy::SetColor(D3DXCOLOR col)
{
	m_col = col;

	CObject2D::SetColor(m_col);
}

//--------------------------------------
//エネミーの状態設定
//--------------------------------------
void CEnemy::SetState(CEnemy::ENEMY_STATE state)
{
	m_nState = state;
}
