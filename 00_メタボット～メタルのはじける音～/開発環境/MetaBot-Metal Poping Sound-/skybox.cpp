//-----------------------------------------------------------------------------------------------------------
//
//メッシュ
//Author;takano
//
//-----------------------------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "application.h"
#include "renderer.h"
#include "skybox.h"
#include "input.h"
#include "player.h"
#include <d3dx9.h>

//--------------------------------------
//定数定義
//--------------------------------------
const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);		//頂点フォーマット

const float CENTER_POINT = 20000.0f;															//メッシュフィールドの中心点
const float MESH_SIZE = 100.0f;																	//1メッシュのサイズ
const float WIDTH_MESH = 20000.0f;																//球の幅
const int MESH_DIVISION = 20;																	//球の分割数
const int  MAX_MENX = 200;																		//Xの辺の最大数
const int  MAX_MENZ = 200;																		//Yの辺の最大数
const int VEX = ((MESH_DIVISION + 1) * (MESH_DIVISION + 1));									//最大頂点数
const int INDEX = ((((MESH_DIVISION + 1) * 2) * MESH_DIVISION) + ((MESH_DIVISION - 1) * 2));	//インデックスの計算
const int POLYGON = ((MESH_DIVISION * MESH_DIVISION * 2) + ((MESH_DIVISION - 1) * 4));			//ポリゴンの総数

//--------------------------------------
//コンストラクタ
//--------------------------------------
CSkybox::CSkybox()
{
	m_pIdxBuff = nullptr;				//インデックスバッファのクリア
	m_pVtxBuff = nullptr;				//頂点バッファのクリア
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CSkybox::~CSkybox()
{
}

//--------------------------------------
//生成
//--------------------------------------
CSkybox * CSkybox::Create(D3DXVECTOR3 pos)
{
	CSkybox *pMeshfild = new CSkybox;

	if (pMeshfild != nullptr)
	{
		pMeshfild->Init();
		pMeshfild->SetPos(pos);
	}

	return pMeshfild;
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CSkybox::Init()
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//円一週分を球の分割数で割って角度を求める(Z軸)
	float fDivisionNumberZ = (D3DX_PI * 2.0f) / (MESH_DIVISION - 1);

	//円一週分を球の分割数で割って角度を求める(X軸)
	float fDivisionNumberX = (D3DX_PI) / (MESH_DIVISION - 1);

	for (int nCntZ = 0; nCntZ < MESH_DIVISION; nCntZ++)
	{
		//現在のZ軸の角度を求める
		float fRotZ = fDivisionNumberZ * nCntZ;

		for (int nCntX = 0; nCntX < MESH_DIVISION; nCntX++)
		{
			//現在のX軸の角度を求める
			float fRotX = fDivisionNumberX * nCntX;

			//頂点座標の設定(極座標を使用)
			pVtx[0].pos.x = sinf(fRotX) * sinf(fRotZ) * WIDTH_MESH;
			pVtx[0].pos.y = cosf(fRotX) * WIDTH_MESH;
			pVtx[0].pos.z = sinf(fRotX) * cosf(fRotZ) * WIDTH_MESH;

			//法線の設定
			pVtx[0].nor = D3DXVECTOR3(sinf(fRotX), 0.0f, cosf(fRotZ));

			//色の設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャの設定
			float fTexZ = 1.0f / MESH_DIVISION * fRotZ;
			float fTexX = 1.0f / MESH_DIVISION * fRotX;
			pVtx[0].tex = D3DXVECTOR2(fTexZ, -(fTexX * 2.f));

			//頂点番号をずらす
			pVtx++;
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	//インデックスバッファへのポインタ
	WORD *pIdx;

	//インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCount = 0;

	for (int nCtZ = 0; nCtZ < MESH_DIVISION -1; nCtZ++)
	{
		for (int nCtX = 0; nCtX <= MESH_DIVISION; nCtX++)
		{
			pIdx[nCount] = nCtX + nCtZ * MESH_DIVISION + MESH_DIVISION;		//頂点の上段の番号
			pIdx[nCount + 1] = nCtX + nCtZ * MESH_DIVISION;						//頂点の下段の番号		　
			nCount += 2;														//頂点の番号をずらす

			if (nCtX == MESH_DIVISION - 1)
			{
				pIdx[nCount] = nCtX + nCtZ * MESH_DIVISION;				//縮退ポリゴン上段
				pIdx[nCount + 1] = (nCtZ + 2) * MESH_DIVISION;			//縮退ポリゴン下段

				//次から始まるスタート地点をずらす(このままだと値が次のfor文で上書きされてしまう)
				nCount += 2;				//頂点の番号をずらす
			}
		}
	}

	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CSkybox::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの解放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	//オブジェクトの開放
	CObject::Release();
}

//--------------------------------------
//更新
//--------------------------------------
void CSkybox::Update()
{
	
}

//--------------------------------------
//描画
//--------------------------------------
void CSkybox::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();		//デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;												//計算用メイトリックス

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

	//ライトの無効化
	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの取得
	CTexture *pTexture = CApplication::GetTexture();

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(CTexture::TEXTURE_SKY));

	//メッシュの描画
	pDevice->DrawIndexedPrimitive
	(D3DPT_TRIANGLESTRIP,
		0,
		0,
		VEX,
		0,
		POLYGON);

	//テクスチャのリセット
	pDevice->SetTexture(0, NULL);

	//ライトの有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
