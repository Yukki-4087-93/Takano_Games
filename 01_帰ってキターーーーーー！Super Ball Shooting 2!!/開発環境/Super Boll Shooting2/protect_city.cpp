//--------------------------------------------------------------------------------------
//
// プレイヤー
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "protect_city.h"
#include "application.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "result.h"
#include "ranking.h"
#include "score.h"
#include <assert.h>
#include <stdlib.h>

int Cprotect_City::m_nLife;

//--------------------------------------
//コンストラクタ
//--------------------------------------
Cprotect_City::Cprotect_City()
{
	m_nLife = 0;
	m_Damage = 0;
	m_nState = STARE_NOMAL;
	m_nColorCount = 0;
}

//--------------------------------------
//デストラクタ
//--------------------------------------
Cprotect_City::~Cprotect_City()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT Cprotect_City::Init()
{
	CObject2D::Init();											//オブジェクト2Dの初期化
	CObject2D::SetTexture(CTexture::TEXTURE_PROTECT);			//テクスチャの配置
	m_nLife = 20;

	return S_OK;
}

//--------------------------------------
//クリエイト
//--------------------------------------
Cprotect_City *Cprotect_City::Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd)
{
	Cprotect_City *m_pCity = new Cprotect_City;		//プレイヤーの動的確保

	if (m_pCity != nullptr)
	{//プレイヤーのポインターに値が入っていなかったら
		m_pCity->Init();								//初期化
		m_pCity->SetObjectType(OBJTYPE_PROTECT);		//オブジェクトの設定
		m_pCity->Setpos(pos);						//位置設定
		m_pCity->SetSize(ObfHi, ObfWd);				//サイズの設定
	}
	else
	{
		assert(false);
	}

	return m_pCity;
}

//--------------------------------------
//更新
//--------------------------------------
void Cprotect_City::Update()
{
	if (m_nLife <= 0)
	{
		CExplosion::Create(m_pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		Uninit();
		CResult::Set(1);
		CRanking::Set(CScore::Get());
		CApplication::SetMode(CApplication::MODE::MODE_RESULT);
		return;				//これ以上更新に入らないように返す
	}


	CObject2D::Update();
	State();
}

//--------------------------------------
//描画
//--------------------------------------
void Cprotect_City::Draw()
{
	CObject2D::Draw();
}

//--------------------------------------
//終了
//--------------------------------------
void Cprotect_City::Uninit()
{
	CObject2D::Uninit();
}

//--------------------------------------
//ダメージ
//--------------------------------------
void Cprotect_City::Damage(int nDamage)
{
	m_nLife -= nDamage;

	CScore::HitDamage(nDamage);

	if (m_nLife < 20 && m_nLife >= 15)
	{
		SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		CObject2D::SetTexture(CTexture::TEXTURE_PROTECT_ALERT);			//テクスチャの配置
	}

	if (m_nLife < 15 && m_nLife >= 5)
	{
		SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		CObject2D::SetTexture(CTexture::TEXTURE_PROTECT_DEEPALERT);			//テクスチャの配置
	}

	if (m_nLife < 5 && m_nLife >= 0)
	{
		SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		CObject2D::SetTexture(CTexture::TEXTURE_PROTECT_DEATHALERT);			//テクスチャの配置
	}
}

//--------------------------------------
//壁の状態管理
//--------------------------------------
void Cprotect_City::State()
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
//壁の状態設定
//--------------------------------------
void Cprotect_City::SetState(Cprotect_City::CITY_STATE state)
{
	m_nState = state;
}

//--------------------------------------
//壁の寿命取得
//--------------------------------------
int Cprotect_City::GetLife()
{
	return m_nLife;
}
