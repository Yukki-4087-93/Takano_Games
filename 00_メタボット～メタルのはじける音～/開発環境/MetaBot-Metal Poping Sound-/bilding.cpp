//--------------------------------------------------------------------------------------
//
// 建物
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "bilding.h"
#include "model.h"
#include "helperfunction.h"
#include "player.h"
#include "enemy.h"

//----------------------------------------
//コンストラクタ
//----------------------------------------
CBilding::CBilding()
{
}

//----------------------------------------
//デストラクタ
//----------------------------------------
CBilding::~CBilding()
{
}

//----------------------------------------
//生成
//----------------------------------------
CBilding * CBilding::Create(const char* pfile, D3DXVECTOR3 pos, D3DXVECTOR3 rot, ObjType type)
{
	//動的確保
	CBilding *pBilding = new CBilding;

	if (pBilding != nullptr)
	{//ビルディングポインタに値が入っていたら
		pBilding->Init();										//初期化
		pBilding->SetModel(pfile);								//モデルの設定
		pBilding->SetPos(pos);									//位置の設定
		pBilding->SetRot(rot);									//角度の設定
		pBilding->SetObjectType(type);							//オブジェクトタイプの設定
	}

	//値を返す
	return pBilding;
}

//----------------------------------------
//初期化
//----------------------------------------
HRESULT CBilding::Init()
{
	m_pos = D3DXVECTOR3(0.f, 0.f, 0.f);

	return S_OK;
}

//----------------------------------------
//終了
//----------------------------------------
void CBilding::Uninit()
{
	//オブジェクトの開放
	CObject::Release();
}

//----------------------------------------
//更新
//----------------------------------------
void CBilding::Update()
{
	m_posOld = m_pos;

	Collision();
}

//----------------------------------------
//描画
//----------------------------------------
void CBilding::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();		//デバイスのポインタ

	D3DXMATRIX mtxRot, mtxTrans;												//計算用マトリックス

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

	//配置した分だけ描画する
	m_pModel->Draw();
}

//----------------------------------------
//当たり判定
//----------------------------------------
void CBilding::Collision()
{
	if (CGame::GetPlayer == nullptr)
	{//プレイヤーが使用されていないとき
		return;						//これ以上通さない
	}

	if (CHelperFunction::CircleCollision(m_pos, 20.0f, CGame::GetPlayer()->GetPos(), 20.0f))
	{//当たり判定の返り値が真なら
		m_pos = m_posOld;			//位置を過去の位置に設定
	}
}

//----------------------------------------
//モデルの設定
//----------------------------------------
void CBilding::SetModel(const char * pfile)
{
	//モデルの生成
	m_pModel = CModel::Create(pfile, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CModel::TYPE_BILDING);
}

//----------------------------------------
//モデルの設定
//----------------------------------------
LPD3DXMESH CBilding::GetMesh()
{
	return m_pModel->GetMedh();
}

//----------------------------------------
//モデルのマトリックスを取得
//----------------------------------------
D3DXMATRIX CBilding::GetmtxWorldModel()
{
	return m_pModel->GetMtxWorld();
}

//----------------------------------------
//カラーの設定
//----------------------------------------
void CBilding::SetColor(bool bcolor)
{
	m_pModel->SetChangeColor(bcolor);
}

//----------------------------------------
//カラーの設定
//----------------------------------------
D3DXVECTOR3 CBilding::GetModelpos()
{
	return m_pModel->GetPos();
}
