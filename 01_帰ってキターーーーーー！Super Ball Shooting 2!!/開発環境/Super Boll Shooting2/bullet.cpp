//--------------------------------------------------------------------------------------
//
// 弾
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "bullet.h"
#include "application.h"
#include "explosion.h"
#include "effect.h"
#include "enemy.h"
#include "enemymanager.h"
#include "player.h"
#include <assert.h>
#include <time.h>

//----------------------------------------
//定数定義
//----------------------------------------
const int MAX_BULLET_OBJECT = 1280;
const float HitSizeEnemy = 40.0f;
const float HitSizePlayer = 30.0f;

//----------------------------------------
//コンストラクタ
//----------------------------------------
CBullet::CBullet()
{//値をクリア(綺麗に)する
	m_nBLife = 0;
	m_nMove = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_TypeUser = USER_NOME;
	m_nColorCount = 0;
	m_nPattarnCount = 0;
}

//----------------------------------------
//デストラクタ
//----------------------------------------
CBullet::~CBullet()
{
}

//----------------------------------------
//初期化
//----------------------------------------
HRESULT CBullet::Init()
{
	m_fPatternAnim = 0;																			//アニメーションパターンの初期化
	CObject2D::Init();																			//初期化
	m_nBLife = 50;																				//ライフの初期設定
	m_nMove = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 20.0f, cosf(D3DX_PI * 0.5f) * 20.0f, 0.0f);	//移動量の初期化
	m_TypeUser = USER_NOME;																		//ユーザー状態を未使用に設定
	return S_OK;
}

//----------------------------------------
//クリエイト
//----------------------------------------
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, CTexture::TEXTURE tex, CBullet::BULLET_USER user)
{
	//動的確保
	CBullet* pBullet = new CBullet;

	if (pBullet != nullptr)
	{//弾のポインタに値が入っていなかったとき
		pBullet->Init();														//初期化
		pBullet->SetObjectType(OBJTYPE_BULLET);									//オブジェクトタイプの設定
		pBullet->SetTexture(tex);												//テクスチャの設定
		pBullet->Setpos(pos);													//位置の設定
		pBullet->SetMove(move);													//移動量の設定
		pBullet->SetTypeBullet(user);											//弾のユーザー設定
	}
	else
	{//それ以外(弾のポインタに値が入っていたとき)
		assert(false);
	}

	return pBullet;
}

//----------------------------------------
//更新
//----------------------------------------
void CBullet::Update()
{
	//ライフを減らす
	m_nBLife--;

	//エフェクトの位置の更新
	CObject2D::Addmove(m_nMove);

	//オブジェクト自体の更新
	CObject2D::Update();

	m_nPattarnCount++;

	//テクスチャパターンの更新
	CObject2D::PattarnTexture(m_nPattarnCount,8);
	
	//弾ライフの減少
	SubstructLife();

	//当たり判定
	Collisison();
}

//----------------------------------------
//描画
//----------------------------------------
void CBullet::Draw()
{
	CObject2D::Draw();
}

//----------------------------------------
//終了
//----------------------------------------
void CBullet::Uninit()
{
	CObject2D::Uninit();
}

//----------------------------------------
//弾の開放
//----------------------------------------
void CBullet::Release()
{
	CObject2D::Release();
}

//----------------------------------------
//ムーブをセットする
//----------------------------------------
void CBullet::SetMove(D3DXVECTOR3 move)
{
	m_nMove = move;
}

//----------------------------------------
//移動量の情報取得
//----------------------------------------
D3DXVECTOR3 CBullet::GetMove()
{
	return m_nMove;
}

//----------------------------------------
//ユーザー情報取得
//----------------------------------------
CBullet::BULLET_USER CBullet::GetUser()
{
	return m_TypeUser;
}

//----------------------------------------
//弾のユーザーの設定
//----------------------------------------
void CBullet::SetTypeBullet(CBullet::BULLET_USER user)
{
	m_TypeUser = user;
}

//----------------------------------------
//弾のライフ減少
//----------------------------------------
void CBullet::SubstructLife()
{
	if (m_nBLife <= 0)
	{//ライフが0以下になったら
		CExplosion::Create(m_pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		Uninit();		//弾の解放

		return;
	}
}

//----------------------------------------
//弾の当たり判定
//----------------------------------------
void CBullet::Collisison()
{
	for (int i = 0; i < MAX_BULLET_OBJECT; i++)
	{
		CObject *pObj;
		CObject::ObjType objType;
		pObj = CObject::Getobject(i);

		if (pObj == nullptr)
		{//オブジェクトが使用されていなかったとき
			continue;
		}
		if (m_TypeUser == USER_NOME)
		{//ユーザータイプが使用されていないとき
			continue;
		}
		objType = pObj->GetObjectType();
		D3DXVECTOR3 ObjPos = pObj->Getpos();

/*弾使用ユーザーがプレイヤーでオブジェクトタイプがエネミーだったとき*/

		if (m_TypeUser == USER_PLAYER &&
			objType == OBJTYPE_ENEMY &&
			m_pos.x >= ObjPos.x - HitSizeEnemy &&
			m_pos.x <= ObjPos.x + HitSizeEnemy &&
			m_pos.y <= ObjPos.y + HitSizeEnemy &&
			m_pos.y >= ObjPos.y - HitSizeEnemy)
		{//位置の中心に弾の半分のサイズを+-した範囲に当たったとき
			CExplosion::Create(m_pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CEnemy* pEnemy = (CEnemy*)pObj;						//pObjectをCEnemy型にダウンキャスト
			pEnemy->SetState(CEnemy::ENEMY_STATE::STARE_DAMAGE);
			pEnemy->SetColor(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));
			pEnemy->HitLife(1);

			Release();						//弾自身を開放する

			//これ以上処理に入らないようにする
			return;
		}

/*弾使用ユーザーがプレイヤーでオブジェクトタイプがボスだったとき*/

		if (m_TypeUser == USER_PLAYER &&
			objType == OBJTYPE_BOSS &&
			m_pos.x >= ObjPos.x - 200.0f &&
			m_pos.x <= ObjPos.x + 200.0f &&
			m_pos.y <= ObjPos.y + 200.0f &&
			m_pos.y >= ObjPos.y - 200.0f)
		{//位置の中心に弾の半分のサイズを+-した範囲に当たったとき
			CExplosion::Create(m_pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CEnemy* pEnemy = (CEnemy*)pObj;						//pObjectをCEnemy型にダウンキャスト
			pEnemy->SetState(CEnemy::ENEMY_STATE::STARE_DAMAGE);
			pEnemy->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			pEnemy->HitLife(1);

			Release();						//弾自身を開放する

			//これ以上処理に入らないようにする
			return;
		}

/*弾使用ユーザーがエネミーでオブジェクトタイプがプレイヤーだったとき*/

		if (m_TypeUser == USER_ENEMY &&
			objType == OBJTYPE_PLAYER &&
			m_pos.x >= ObjPos.x - HitSizePlayer &&
			m_pos.x <= ObjPos.x + HitSizePlayer &&
			m_pos.y <= ObjPos.y + HitSizePlayer &&
			m_pos.y >= ObjPos.y - HitSizePlayer)
		{//位置の中心に弾の半分のサイズを+-した範囲に当たったとき
			CExplosion::Create(m_pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CPlayer* pPlayer = (CPlayer*)pObj;						//pObjectをCPlayer型にダウンキャスト
			pPlayer->SetState(CPlayer::PLAYER_STATE::STARE_DAMAGE);
			pPlayer->HitDamage(1);
			Release();						//弾自身を開放する

			//これ以上処理に入らないようにする
			return;
		}
	}
}
