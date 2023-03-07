//-----------------------------------------------------------------
//
// カメラ
// Author::TAKANO
//
//-----------------------------------------------------------------
#ifndef _CAMERA_H_									//このマクロ定義がなされなっかたら
#define _CAMERA_H_									//二重インクルード防止のマクロ定義

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include <d3dx9.h>

//----------------------------------------
//マクロ定義
//----------------------------------------
#define MAX_SPEED		(0.3f)						// 移動に使用する最大速度

//----------------------------------------
//クラス定義
//----------------------------------------
class CCamera
{
public:
	CCamera();										// コンストラクタ
	~CCamera();										// デストラクタ
	static CCamera* Create(D3DXVECTOR3 pos);		// 生成
	void Init();									// 初期化
	void Uninit();									// 終了
	void Update();									// 更新
	void Set();										// カメラの配置
	D3DXVECTOR3 GetRot() { return m_rot; }			// 角度
	D3DXVECTOR3 GetPosV() { return m_WorldPosV; }			// 角度
	D3DXVECTOR3 GetPosR() { return m_WorldPosR; }			// 角度
	D3DXMATRIX GetWorldmtx() { return m_mtxWorld; }			// 角度
	void Sequence(int nFlame, float fMagnitude);		// カメラを揺らす関数

private:
	int m_nCameraFlame;								// カメラを揺らす際に使用するカウント変数
	float m_fCameraMagnitude;						// カメラを揺らす際に使用する震度を制御する変数
	D3DXVECTOR3 m_InitPosV;							// 視点
	D3DXVECTOR3 m_InitPosR;							// 注視点
	D3DXVECTOR3 m_VecU;							// 距離
	D3DXVECTOR3 m_WorldPosV;						// ローカル座標系
	D3DXVECTOR3 m_WorldPosR;						// ローカル座標系
	D3DXVECTOR3 m_rot;							// 向き
	D3DXVECTOR3 m_rotDest;						// 角度のフォフォフォ
	D3DXMATRIX m_mtxprojection;					// マトリックスの投影
	D3DXMATRIX m_mtxWorld;						// マトリックスワールド
	D3DXMATRIX m_mtxView;							// マトリックスの視点
	float m_fDistance;							// 距離
};

#endif // !CAMERA_H_
