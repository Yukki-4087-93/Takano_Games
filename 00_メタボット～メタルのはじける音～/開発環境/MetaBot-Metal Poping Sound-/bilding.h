//--------------------------------------------------------------------------------------
//
// 建物
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _BILDING_H_
#define _BILDING_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "object.h"
//#include "model.h"

class CModel;

//----------------------------------------
//クラス定義
//ゲーム内の建物に関するクラス
//----------------------------------------
class CBilding : public CObject
{
public:
	CBilding();																		//コンストラクタ
	~CBilding();																	//デストラクタ
	static CBilding* Create(const char* pfile, D3DXVECTOR3 pos, D3DXVECTOR3 rot, ObjType type);	//生成
	HRESULT Init() override;														//初期化
	void Uninit() override;															//終了
	void Update() override;															//更新
	void Draw() override;															//描画
	D3DXVECTOR3 GetPosOld() { return m_posOld; }									//建物の古い位置情報の取得
	void Collision();																//建物の当たり判定を制御する関数
	void SetModel(const char* pfile);												//建物のモデルを変更する関数
	CModel* GetModel() { return m_pModel; }
	LPD3DXMESH GetMesh();										//メッシュ情報の取得
	D3DXMATRIX GetmtxWorld() { return m_mtxWorld; }				//マトリックスワールドの取得
	D3DXMATRIX GetmtxWorldModel();									//マトリックスワールドの取得
	void SetColor(bool bcolor);
	D3DXVECTOR3 GetModelpos(); 								//位置の取得

private:
	CModel* m_pModel;											//モデルの配置量
	D3DXVECTOR3 m_posOld;										//視点
	D3DXMATRIX m_mtxWorld;										//マトリックスワールド
	//DWORD m_nNumMat;											//マテリアル情報の数
	//LPD3DXBUFFER m_BuffMat;									//マテリアル情報
	//LPD3DXMESH m_Mesh;										//メッシュ情報
};

#endif // !_BILDING_H_