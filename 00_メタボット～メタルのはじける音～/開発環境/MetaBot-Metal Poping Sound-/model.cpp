//--------------------------------------------------------------------------------------
//
// モデル
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "application.h"
#include "renderer.h"
#include "model.h"
#include "object.h"

//--------------------------------------
//コンストラクタ
//--------------------------------------
CModel::CModel()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//位置のクリア
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向きのクリア
	m_bChangeColor = false;
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CModel::~CModel()
{
}

//--------------------------------------
//生成
//--------------------------------------
CModel * CModel::Create(const char* pfile, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, MODEL_TYPE type)
{
	CModel * pModel = new CModel;

	if (pModel != nullptr)
	{
		pModel->SetModel(pfile);
		pModel->Init();
		pModel->m_pos = pos;
		pModel->m_rot = rot;
		pModel->m_StartPoint = pos;
		pModel->m_StartRotation = rot;
		pModel->SetModelType(type);
	}

	return pModel;
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CModel::Init()
{
	//モデルの初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//位置の初期化
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向きの初期化

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CModel::Uninit()
{
}

//--------------------------------------
//更新
//--------------------------------------
void CModel::Update()
{
}

//--------------------------------------
//描画
//--------------------------------------
void CModel::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent;					//計算用マトリックス
	D3DMATERIAL9 matDef;									//現在のマトリックス保存用
	D3DXMATERIAL *pMat;										//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != nullptr)
	{//親モデルのポインタに値が入っていたとき
		mtxParent = m_pParent->GetMtxWorld();				//親モデルのワールド座標を取得
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);		//最新のマトリックスを取得
	}

	//親のマトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//ワールドマトリックスに設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアル保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)(m_BuffMat->GetBufferPointer());

	for (int i = 0; i < (int)m_nNumMat; i++)
	{
		D3DXCOLOR col = pMat[i].MatD3D.Diffuse;
		if (m_bChangeColor)
		{
			//マテリアルの色変更
			col.a = 0.3f;

		}

		else
		{
			//マテリアルの色変更
			col.a = 1.f;
		}

		//設定したカラーを代入
		pMat[i].MatD3D.Diffuse = col;

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, nullptr);

		//モデルパーツの描画
		m_Mesh->DrawSubset(i);

	}

	//保持していたマテリアルを返却
	pDevice->SetMaterial(&matDef);

	/*ワールドマトリックスの初期化、向き・位置の反映
	　親のマトリックスと掛け合わせる
	 ワールドマトリックスの設定
	 　Xファイルの描画*/
}

//--------------------------------------
//モデルの設定
//--------------------------------------
void CModel::SetModel(const char* pfile)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX(pfile,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_BuffMat,
		NULL,
		&m_nNumMat,
		&m_Mesh);

	//モデルのサイズの値を初期化する
	m_VtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	m_VtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

	//頂点数の保存する変数を宣言
	int nNumVertex = 0;

	//頂点数を代入する
	nNumVertex = m_Mesh->GetNumVertices();

	//頂点フォーマットを保存する変数を宣言
	DWORD SizeFVF;

	//頂点フォーマットの取得
	SizeFVF = D3DXGetFVFVertexSize(m_Mesh->GetFVF());

	//頂点バッファへのポインタ
	BYTE* pVtxBuffer;

	//頂点をロックする
	m_Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuffer);

	//全ての頂点の位置を取得する
	for (int nCounter = 0; nCounter < nNumVertex; nCounter++)
	{
		//頂点座標を代入
		D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuffer;

		if (m_VtxMax.x < Vtx.x)
		{//X
			m_VtxMax.x = Vtx.x;
		}
		if (m_VtxMin.x > Vtx.x)
		{
			m_VtxMin.x = Vtx.x;
		}

		if (m_VtxMax.y < Vtx.y)
		{//Y
			m_VtxMax.y = Vtx.y;
		}
		if (m_VtxMin.y > Vtx.y)
		{
			m_VtxMin.y = Vtx.y;
		}

		if (m_VtxMax.z < Vtx.z)
		{//Z
			m_VtxMax.z = Vtx.z;
		}
		if (m_VtxMin.z > Vtx.z)
		{
			m_VtxMin.z = Vtx.z;
		}

		//頂点フォーマットのサイズの分ポインタを進める
		pVtxBuffer += SizeFVF;
	}

	//頂点をアンロックする
	m_Mesh->UnlockVertexBuffer();
}
