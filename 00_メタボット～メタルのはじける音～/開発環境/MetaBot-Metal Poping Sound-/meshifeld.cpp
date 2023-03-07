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
#include "meshifild.h"
#include "input.h"
#include "player.h"
#include <d3dx9.h>

//--------------------------------------
//定数定義
//--------------------------------------
const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);	//頂点フォーマット

const float CENTER_POINT = 20000.0f;														//メッシュフィールドの中心点
const float MESH_SIZE = 100.0f;																//1メッシュのサイズ
const int  MAX_MENX = 200;																	//Xの辺の最大数
const int  MAX_MENZ = 200;																	//Zの辺の最大数
const int VEX = ((MAX_MENX + 1) * (MAX_MENZ + 1));											//最大頂点数
const int INDEX = ((((MAX_MENX + 1) * 2) * MAX_MENZ) + ((MAX_MENZ - 1) * 2));				//インデックスの計算
const int POLYGON = ((MAX_MENX * MAX_MENZ * 2) + ((MAX_MENZ - 1) * 4));						//ポリゴンの総数

//--------------------------------------
//コンストラクタ
//--------------------------------------
CMeshField::CMeshField()
{
	m_pIdxBuff = nullptr;				//インデックスバッファのクリア
	m_pVtxBuff = nullptr;				//頂点バッファのクリア
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CMeshField::~CMeshField()
{
}

//--------------------------------------
//生成
//--------------------------------------
CMeshField * CMeshField::Create()
{
	CMeshField *pMeshfild = new CMeshField;

	if (pMeshfild != nullptr)
	{
		pMeshfild->Init();
	}

	return pMeshfild;
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CMeshField::Init()
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
	VERTEX_3D*pVtx;			

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ <= MAX_MENZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= MAX_MENX; nCntX++)
		{
			pVtx[0].pos = D3DXVECTOR3(MESH_SIZE * nCntX - CENTER_POINT / 2,
									  0.0f + 2.0f, 
									  -MESH_SIZE * nCntZ + CENTER_POINT / 2);

			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[0].col = D3DXCOLOR(0.2f, 0.5f, 0.1f, 1.0f);

			pVtx += 1;
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

	for (int nCtZ = 0; nCtZ < MAX_MENZ; nCtZ++)
	{
		for (int nCtX = 0; nCtX <= MAX_MENX; nCtX++)
		{
			pIdx[0] = (nCtX)+(MAX_MENX + 1) + (nCtZ * (MAX_MENX + 1));		//頂点の上段の番号
			pIdx[1] = (nCtX)+(nCtZ * (MAX_MENX + 1));						//頂点の下段の番号		　
			pIdx += 2;														//頂点の番号をずらす
		}

		if (nCtZ < MAX_MENZ - 1)
		{
			pIdx[0] = pIdx[-1];				//縮退ポリゴン上段
			pIdx[1] = ((MAX_MENX + 1) * 2) + (nCtZ * (MAX_MENX + 1));	//縮退ポリゴン下段

			//次から始まるスタート地点をずらす(このままだと値が次のfor文で上書きされてしまう)
			pIdx += 2;										//頂点の番号をずらす
		}
		else
		{

		}
	}

	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CMeshField::Uninit()
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
void CMeshField::Update()
{

}

//--------------------------------------
//描画
//--------------------------------------
void CMeshField::Draw()
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

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	//メッシュの描画
	pDevice->DrawIndexedPrimitive
	(D3DPT_TRIANGLESTRIP,
		0,
		0,
		VEX,
		0,
		POLYGON);
}

//--------------------------------------------------
// 2Dベクトルの外積
//--------------------------------------------------
float CMeshField::Vec2Cross(D3DXVECTOR3 * v1, D3DXVECTOR3 * v2)
{
	return (v1->x * v2->z) - (v1->z * v2->x);
}

//--------------------------------------------------
// 2Dベクトルの外積
//--------------------------------------------------
float CMeshField::Vec2Dot(D3DXVECTOR3 * v1, D3DXVECTOR3 * v2)
{
	return (v1->x * v2->x) + (v1->z * v2->z);
}

//--------------------------------------
//当たり判定
//--------------------------------------
void CMeshField::Collision(D3DXVECTOR3 *pos)
{
	VERTEX_3D*pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//インデックスバッファへのポインタ
	WORD *pIdx;
	//インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	for (int i = 0; i < POLYGON; i++)
	{
		if (pIdx[i] == pIdx[i + 1] ||
			pIdx[i] == pIdx[i + 2] ||
			pIdx[i + 1] == pIdx[i + 2])
		{
			continue;
		}

		D3DXVECTOR3 Vecpos[3] = {};

		Vecpos[0] = pVtx[pIdx[i]].pos;
		Vecpos[1] = pVtx[pIdx[i] + 1].pos;
		Vecpos[2] = pVtx[pIdx[i] + 2].pos;

		for (int j = 0; j < 3; j++)
		{
			D3DXVec3TransformCoord(&Vecpos[j], &Vecpos[j], &mtxWorld);
		}

		int nLine = 0;

		for (int j = 0; j < 3; j++)
		{
			D3DXVECTOR3 Vec = Vecpos[(j + 1) % 3] - Vecpos[j];			//ポリゴンの線上の計算

			D3DXVECTOR3 DiffVec = *pos - Vecpos[j];						//現在の位置と線上の減算

			float fVecLine = Vec2Cross(&DiffVec, &Vec);

			if ((i % 2 == 0 && fVecLine > 0.0f) ||
				(i % 2 != 0 && fVecLine < 0.0f))
			{
				nLine++;
			}
			else
			{
				break;
			}

		}

		if (nLine == 3)
		{
			//ベクトルを計算
			D3DXVECTOR3 Vec1 = Vecpos[1] - Vecpos[0];
			D3DXVECTOR3 Vec2 = Vecpos[2] - Vecpos[0];
			D3DXVECTOR3 Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//正規化の初期化

																	//外積の計算
			D3DXVec3Cross(&Normal, &Vec1, &Vec2);

			//正規化
			D3DXVec3Normalize(&Normal, &Normal);

			D3DXVECTOR3 VecA = *pos - Vecpos[0];

			//メッシュフィールドのプレイヤーの位置設定
			pos->y = (Vecpos[0].y - (Normal.x * (pos->x - Vecpos[0].x) + Normal.z * (pos->z - Vecpos[0].z)) / Normal.y);
		}
	}

	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
