//-----------------------------------------------------------------------------------------------------------
//
//メッシュ
//Author;takano
//
//-----------------------------------------------------------------------------------------------------------
#ifndef _MESHIFILD_H_													//このマクロ定義がなされなっかたら
#define _MESHIFILD_H_													//二重インクルード防止のマクロ定義

//-----------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------
#include"object3d.h"

//-----------------------------------------------------------
//クラス定義
//メッシュフィールド、オブジェクト3Dを継承
//-----------------------------------------------------------
class CMeshField : public CObject3D
{
public:
	CMeshField();												//コンストラクタ
	~CMeshField();												//デストラクタ
	static CMeshField* Create();								//生成
	HRESULT Init();												//初期化
	void Uninit();												//終了
	void Update();												//更新
	void Draw();												//描画
	float Vec2Cross(D3DXVECTOR3* v1, D3DXVECTOR3* v2);			//掛け算
	float Vec2Dot(D3DXVECTOR3* v1, D3DXVECTOR3* v2);			//角度
	void Collision(D3DXVECTOR3 *pos);							//当たり判定

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff ;							//インデックスバッファへのポインタ
	D3DXMATRIX m_mtxWorld;										//メッシュフィールドのワールドマトリックス

};

#endif // !MESHIFIL_H_
