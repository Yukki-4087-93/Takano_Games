//-----------------------------------------------------------------------------------------------------------
//
//メッシュ
//Author;takano
//
//-----------------------------------------------------------------------------------------------------------
#ifndef _MODEL_H_													//このマクロ定義がなされなっかたら
#define _MODEL_H_													//二重インクルード防止のマクロ定義

//-----------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------
//#include <d3dx9.h>
#include "object.h"

//-----------------------------------------------------------
//クラス定義
//モデル関係
//-----------------------------------------------------------
class CModel
{
public:

	enum MODEL_TYPE
	{
		TYPE_NONE = -1,
		TYPE_PLAYER = 0,
		TYPE_ENEMY,
		TYPE_BILDING,
		TYPE_MAX
	};

	CModel();															//コンストラクタ
	~CModel();															//デストラクタ
	static CModel *Create(const char* pfile, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, MODEL_TYPE type);			//生成
	HRESULT Init();										//初期化
	void Uninit();										//終了
	void Update();										//更新
	void Draw();										//描画
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		//位置の設定
	D3DXVECTOR3 GetPos() { return m_pos; }								//位置の取得
	D3DXVECTOR3 GetRot() { return m_rot; }								//角度の取得
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }						//角度の設定
	void SetParent(CModel* pmodel) { m_pParent = pmodel; }				//親の情報を取得する
	D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }						//マトリックスワールドを取得する
	D3DXVECTOR3 GetStartPoint() { return m_StartPoint; }				//初期位置を取得する関数
	D3DXVECTOR3 GetStartRotation() { return m_StartRotation; }			//初期位置を取得する関数
	void SetModelType(MODEL_TYPE type) { m_ModelType = type; }			//モデルタイプの設定
	MODEL_TYPE GetModelType() { return m_ModelType; }					//モデルタイプの情報を取得する
	D3DXVECTOR3 GetVtxMax() { return m_VtxMax; }
	D3DXVECTOR3 GetVtxMin() { return m_VtxMin; }
	LPD3DXMESH GetMedh() { return m_Mesh; }								//メッシュ情報の取得
	void SetModel(const char* pfile);									//モデルの配置
	void SetChangeColor(bool bcolor) { m_bChangeColor = bcolor; }		//カラーを設定する関数

private:
	MODEL_TYPE m_ModelType;
	LPD3DXMESH m_Mesh;													//メッシュ
	LPD3DXBUFFER m_BuffMat;												//バーテックス
	DWORD m_nNumMat;													//マテリアル情報の数
	D3DXMATRIX m_mtxWorld;												//ワールドマトリックス
	D3DXVECTOR3 m_pos;													//視点
	D3DXVECTOR3 m_rot;													//向き
	D3DXVECTOR3 m_VtxMax;												//最大
	D3DXVECTOR3 m_VtxMin;												//最小
	int m_nIdxModelParent;												//親モデルの番号
	int m_nNubParts;													//モデルの番号
	CModel *m_pParent;													//親モデルへのポインタ
	D3DXMATRIX m_mtxParent;												//親のマトリックス
	D3DXVECTOR3 m_StartPoint;											//初期位置
	D3DXVECTOR3 m_StartRotation;										//初期角度
	bool m_bChangeColor;												//カラーを変更するか変える変数
};

#endif // !_MODEL_H_
