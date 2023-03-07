//--------------------------------------------------------------------------------------
//
//エフェクト
//Author;takano
//
//--------------------------------------------------------------------------------------

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "application.h"
#include "renderer.h"
#include "helperfunction.h"
#include "effect.h"

//----------------------------------------
//コンストラクタ
//----------------------------------------
CEffect::CEffect()
{
}

//----------------------------------------
//デストラクタ
//----------------------------------------
CEffect::~CEffect()
{
}

//----------------------------------------
//初期化
//----------------------------------------
HRESULT CEffect::Init()
{
	CObject3D::Init();

	//各メンバ変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color = (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

	CObject3D::SetTexture(CTexture::TRXTURE_EFFECT);

	m_bBillboard = true;

	return S_OK;
}

//----------------------------------------
//終了
//----------------------------------------
void CEffect::Uninit()
{
	CObject3D::Uninit();
}

//----------------------------------------
//更新
//----------------------------------------
void CEffect::Update()
{
	CObject3D::Update();

	m_pos += m_move;

	m_color.a -= 0.05f;

	SetColor(m_color);

	if (m_color.a <= 0.0f)
	{
		Uninit();
	}
}

//----------------------------------------
//描画
//----------------------------------------
void CEffect::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//ライトを無効化する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject3D::Draw();

	//Aブレンディングを元に戻すze☆
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	//ライトを元に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//----------------------------------------
//エフェクトの配置
//----------------------------------------
void CEffect::Set(D3DXVECTOR3 pos,D3DXVECTOR3 move)
{
	//位置のを反映
	m_pos = pos;

	//位置の設定/*処理順で初期化されたオブジェクト3D(ポリゴン)が描画されることを防ぐため*/
	CObject3D::SetPos(m_pos);
	
	//移動量の反映
	m_move = move;
}

//----------------------------------------
//エフェクトの生成
//----------------------------------------
CEffect * CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fHi, float fWd)
{
	CEffect* pEffect = new CEffect;

	if (pEffect != nullptr)
	{
		pEffect->Init();				//初期化
		pEffect->Set(pos, move);
		pEffect->m_color = col;
		pEffect->SetColor(col);
		pEffect->SetSize(fHi, fWd);
		pEffect->SetObjectType(CObject::OBJTYPE_EFFECT);
		pEffect->SetTexture(CTexture::TRXTURE_EFFECT);
	}

	return pEffect;
}

//----------------------------------------
//エフェクトの出現
//----------------------------------------
void CEffect::Spawn(D3DXVECTOR3 pos,int nMaxSpawn,float fRandMax, float fRandMin, D3DXCOLOR col,float fHi,float fWd)
{
	D3DXVECTOR3 randomPos,move;

	for (int i = 0; i < nMaxSpawn; i++)
	{
		move.x = (CHelperFunction::FloatRandom(fRandMax, fRandMin));
		move.y = (CHelperFunction::FloatRandom(fRandMax, fRandMin));
		move.z = (CHelperFunction::FloatRandom(fRandMax, fRandMin));

		//エフェクトの生成
		Create(pos,move,col, fHi, fWd);
	}
}
