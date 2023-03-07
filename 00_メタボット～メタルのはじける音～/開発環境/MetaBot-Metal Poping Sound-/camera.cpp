//--------------------------------------------------------------
//
//カメラ
//Author:takano
//
//--------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include"camera.h"
#include "input.h"
#include "renderer.h"
#include "application.h"
#include "game.h"
#include "player.h"
#include<time.h>

//------------------------
//コンストラクタ
//------------------------
CCamera::CCamera()
{
	srand((unsigned int)time(nullptr));						//ランダムの初期化
}

//------------------------
//デストラクタ
//------------------------
CCamera::~CCamera()
{
}

//------------------------
//クリエイト
//------------------------
CCamera * CCamera::Create(D3DXVECTOR3 pos)
{
	CCamera* pCamera = new CCamera;

	if (pCamera != nullptr)
	{
		pCamera->Init();
	}

	return pCamera;
}

//------------------------
//カメラの初期化
//------------------------
void CCamera::Init()
{
	//視点・注視点・上方向を設定する
	m_InitPosV = D3DXVECTOR3(0.0f, 0.0f, -400.0f);								//視点 見る位置
	m_InitPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//見てる位置
	m_VecU = D3DXVECTOR3(0.0f, 10.0f, 0.0f);								//
	m_rot = D3DXVECTOR3(D3DXToRadian(10.f), 0.0f, 0.0f);					//角度
	float fLength1 = (m_InitPosV.x - m_InitPosR.x);							//視点から注視点までのX軸距離
	float fLength2 = (m_InitPosV.z - m_InitPosR.z);							//視点から注視点までのY軸距離
	m_fDistance = sqrtf((fLength1 * fLength1) + (fLength2 * fLength2));		//視点から注視点の距離
	m_nCameraFlame = -1;
}

//------------------------
//カメラの終了
//------------------------
void CCamera::Uninit()
{
}

//------------------------
//カメラの更新
//------------------------
void CCamera::Update()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//プレイヤー情報の取得
	CPlayer *pPlayer = CGame::GetPlayer();

	//プレイヤーのスケールを取得
	D3DXVECTOR3 Scale = CGame::GetPlayer()->GetScale();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pPlayer->GetPos().x, pPlayer->GetPos().y, pPlayer->GetPos().z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスに設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//視点の調整
	D3DXVECTOR3 VecPosV = m_InitPosV;
	VecPosV.z -= ((Scale.z - 1.0f) * 120.0f);

	//注視点の調整
	D3DXVECTOR3 VecPosR = m_InitPosR;
	VecPosR.y = (Scale.y - 1.0f) * 40.f + 100.f;

	//ワールド座標行列を使って座標を求める
	D3DXVec3TransformCoord(&m_WorldPosR, &VecPosR, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_WorldPosV, &VecPosV, &m_mtxWorld);

	//視点の旋回
	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_C) ||
		CApplication::GetKyeboard()->GetkeyboardPress(DIK_E) ||
		CApplication::GetKyeboard()->GetkeyboardPress(DIK_RIGHT))
	{//左旋回
		m_rot.y += 0.05f;
	}

	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_Z) ||
		CApplication::GetKyeboard()->GetkeyboardPress(DIK_Q) ||
		CApplication::GetKyeboard()->GetkeyboardPress(DIK_LEFT))
	{//右旋回
		m_rot.y -= 0.05f;
	}

	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_UP))
	{
		if (m_rot.x < D3DXToRadian(45.f))
		{
			m_rot.x += 0.02f;
		}

	}

	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_DOWN))
	{
		if (m_rot.x > D3DXToRadian(10.f))
		{
			m_rot.x -= 0.02f;
		}
	}


	//角度の正規化
	if (m_rot.y > D3DX_PI)
	{//回転角が180度より大きくなった時
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{//回転角が-180度よりも小さくなった時
		m_rot.y += D3DX_PI * 2;
	}
}

//------------------------
//カメラのセット
//------------------------
void CCamera::Set()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//ビューメイトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//カメラ震度の設定
	D3DXVECTOR3 magnitude = D3DXVECTOR3(0, 0, 0);

	if (m_nCameraFlame >= 0)
	{
		m_nCameraFlame--;

		magnitude.x = rand() % (int)m_fCameraMagnitude - m_fCameraMagnitude / 2;					//ランダム値の初期値を設定

		magnitude.y = rand() % (int)m_fCameraMagnitude - m_fCameraMagnitude / 2;				//ランダム値の初期値を設定

		magnitude.z = rand() % (int)m_fCameraMagnitude - m_fCameraMagnitude / 2;					//ランダム値の初期値を設定

	}

	//ビューマトリックスの生成
	D3DXMatrixLookAtLH(&m_mtxView,
		&(m_WorldPosV + magnitude),
		&(m_WorldPosR + magnitude),
		&m_VecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxprojection);

	//プロジェクションマトリックス生成
	D3DXMatrixPerspectiveFovLH(&m_mtxprojection,
		D3DXToRadian(60.0f),					//視野比
		(float)1280.0f / (float)720.0f,			//アスペクト比
		100.0f,									//カメラの近視距離
		40000.0f);								//カメラの遠視距離

												//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxprojection);
}

//------------------------
//カメラの震度設定
//------------------------
void CCamera::Sequence(int nFlame, float fMagnitude)
{
	m_nCameraFlame = nFlame;
	m_fCameraMagnitude = fMagnitude;
}
