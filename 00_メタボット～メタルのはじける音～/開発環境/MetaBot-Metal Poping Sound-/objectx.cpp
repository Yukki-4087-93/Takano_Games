//--------------------------------------------------------------------------------------
//
// オブジェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "objectx.h"
#include "renderer.h"
#include "application.h"
#include "texture.h"
#include "main.h"
#include <assert.h>

//--------------------------------------
//コンストラクタ
//--------------------------------------
CObjectX::CObjectX()
{
	m_pos = { 0.0f,0.0f,0.0f };
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CObjectX::~CObjectX()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CObjectX::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX("Data/Model/H.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_BuffMat,
		NULL,
		&m_nNumMat,
		&m_Mesh);

	
	//モデルの初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//位置の初期化
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向きの初期化

	return S_OK;
}

//--------------------------------------
//クリエイト
//--------------------------------------
CObjectX *CObjectX::Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd)
{
	CObjectX* pObject3D;

	pObject3D = new CObjectX;

	if (pObject3D != nullptr)
	{
		pObject3D->Init();
		pObject3D->SetPos(pos);
	}
	else
	{
		assert(false);
	}

	return pObject3D;
}

//--------------------------------------
//更新
//--------------------------------------
void CObjectX::Update()
{
	//VERTEX_3D* pVtx;			//頂点情報へのポインタ

	////頂点バッファをコックし、頂点情報へのポインタを取得
	//m_Vertex->Lock(0, 0, (void**)&pVtx, 0);

	////頂点座標の設定
	//pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + (-D3DX_PI + m_fAngle)) * m_fLength;
	//pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + (-D3DX_PI + m_fAngle)) * m_fLength;
	//pVtx[0].pos.z = 150.0f;

	//pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[1].pos.z = 150.0f;

	//pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + (m_fAngle * -1.0f)) * m_fLength;
	//pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + (m_fAngle * -1.0f)) * m_fLength;
	//pVtx[2].pos.z = 150.0f;

	//pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + (m_fAngle * 1.0f)) * m_fLength;
	//pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + (m_fAngle * 1.0f)) * m_fLength;
	//pVtx[3].pos.z = 150.0f;

	////頂点バッファをアンロックする
	//m_Vertex->Unlock();
}

//--------------------------------------
//描画
//--------------------------------------
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;					//計算用マトリックス
	D3DMATERIAL9 matDef;							//現在のマトリックス保存用
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ

													//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスに設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアル保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_BuffMat->GetBufferPointer();

	for (int i = 0; i < (int)m_nNumMat; i++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		//モデルパーツの描画
		m_Mesh->DrawSubset(i);
	}

	//保持していたマテリアルを返却
	pDevice->SetMaterial(&matDef);
}

//--------------------------------------
//終了
//--------------------------------------
void CObjectX::Uninit()
{
	//メッシュの破壊
	if (m_Mesh != NULL)
	{
		m_Mesh->Release();
		m_Mesh = NULL;
	}

	//マテリアルの破棄
	if (m_BuffMat != NULL)
	{
		m_BuffMat->Release();
		m_BuffMat = NULL;
	}

	CObject::Release();
}

//--------------------------------------
//位置のゲッター
//--------------------------------------
const D3DXVECTOR3 & CObjectX::Getpos() const
{
	return m_pos;
}
