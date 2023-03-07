//--------------------------------------------------------------------------------------
//
// エフェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "effect.h"
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "application.h"
#include <assert.h>
#include <time.h>

//--------------------------------------
//コンストラクタ
//--------------------------------------
CEffect::CEffect()
{
	m_nRun = 0;
	m_nLife = 0;
	m_nMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CEffect::~CEffect()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CEffect::Init()
{
	CObject2D::Init();
	CObject2D::SetTexture(CTexture::TEXTURE_BULLET_E);
	m_nLife = 50;
	
	return S_OK;
}

//--------------------------------------
//クリエイト
//--------------------------------------
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	CEffect* pEffect;

	pEffect = new CEffect;

	if (pEffect != nullptr)
	{
		pEffect->Init();
		pEffect->SetObjectType(OBJTYPE_EFFECT);
		pEffect->Setpos(pos);
		pEffect->SetEffect(col, fRadius, nLife);
	}
	else
	{
		assert(false);
	}

	return pEffect;
}

//--------------------------------------
//更新
//--------------------------------------
void CEffect::Update()
{
	CObject2D::Update();

	CObject2D::PattarnTexture(m_nRun, 8);
	
	m_nLife--;

	//半径の減らす量
	m_fRadius -= 0.7891f;

	//色の変更
	m_Col += D3DXCOLOR(0.005f, 0.005f, 0.005f, 0.0f);

	if (m_nLife <= 0)
	{
		Uninit();		//エフェクトの解放
	}
}

//--------------------------------------
//描画
//--------------------------------------
void CEffect::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//Aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2D::Draw();

	//Aブレンディングを元に戻すze☆
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//--------------------------------------
//終了
//--------------------------------------
void CEffect::Uninit()
{
	CObject2D::Uninit();
}

//--------------------------------------
//エフェクトの設定
//--------------------------------------
void CEffect::SetEffect(D3DXCOLOR col, float fRadius, int nLife)
{
	col = m_Col;
	fRadius = m_fRadius;
	nLife = m_nLife;
}
