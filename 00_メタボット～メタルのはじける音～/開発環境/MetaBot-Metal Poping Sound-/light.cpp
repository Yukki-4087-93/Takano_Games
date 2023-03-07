//-----------------------------------------------------------------
//
//ライト
//Author:takano
//
//----------------------------------------------------------------

//----------------------------
//インクルードファイル
//----------------------------
#include <stdio.h>
#include"light.h"
#include "application.h"
#include "renderer.h"

//----------------------------
//コンストラクタ
//----------------------------
CLight::CLight()
{
}

//----------------------------
//デストラクタ
//----------------------------
CLight::~CLight()
{
}

//----------------------------
//ライトの生成
//----------------------------
CLight * CLight::Create(D3DXVECTOR3 pos)
{
	CLight* pLight = new CLight;

	if (pLight != nullptr)
	{
		pLight->Init();
	}

	return pLight;
}

//----------------------------
//ライト初期化
//----------------------------
void CLight::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();						//デバイスのポインタ
	D3DXVECTOR3 vecDir;												//ライトの方向ベクトル

	//ライトをクリアする
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));

	//ライトの種類
	m_Light[0].Type = D3DLIGHT_DIRECTIONAL;
	//ライトの種類
	m_Light[1].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光の設定
	m_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	//ライトの拡散光の設定
	m_Light[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 0.65f);

	//ライトの方向を設定
	m_vecDir[0] = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
	//ライトの方向を設定
	m_vecDir[1] = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

	//正規化する（大きさを”１”ベクトルにする）
	D3DXVec3Normalize(&m_vecDir[0],&m_vecDir[0]);
	m_Light[0].Direction = m_vecDir[0];

	//正規化する（大きさを”１”ベクトルにする）
	D3DXVec3Normalize(&m_vecDir[1], &m_vecDir[1]);
	m_Light[1].Direction = m_vecDir[1];

	//ライトを設定する
	pDevice->SetLight(0, &m_Light[0]);			//最初の数字0はライトの番号を示している

	//ライトを設定する
	pDevice->SetLight(1, &m_Light[1]);			//最初の数字1はライトの番号を示しているぜ

	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);
	//ライトを有効にする
	pDevice->LightEnable(1, TRUE);

	pDevice->SetRenderState(D3DRS_AMBIENT, 0xff403333);
}

//----------------------------
//ライト終了
//----------------------------
void CLight::Uninit()
{

}

//----------------------------
//ライト更新
//----------------------------
void CLight::Update()
{

}