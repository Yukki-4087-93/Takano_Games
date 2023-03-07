//-----------------------------------------------------------------------------------------------------------
//
//メッシュ
//Author;takano
//
//-----------------------------------------------------------------------------------------------------------
#ifndef _SKYBOX_H_													//このマクロ定義がなされなっかたら
#define _SKYBOX_H_													//二重インクルード防止のマクロ定義

//-----------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------
#include"object3d.h"

//-----------------------------------------------------------
//クラス定義
//-----------------------------------------------------------
class CSkybox : public CObject3D
{
public:
	CSkybox();													//コンストラクタ
	~CSkybox();													//デストラクタ
	static CSkybox* Create(D3DXVECTOR3 pos);					//生成
	HRESULT Init() override;									//初期化
	void Uninit() override;										//終了
	void Update() override;										//更新
	void Draw() override;										//描画
	void SetPos(D3DXVECTOR3 pos) override { m_pos = pos; }		//位置の設定

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;							//インデックスバッファへのポインタ
	D3DXMATRIX m_mtxWorld;										//メッシュフィールドのワールドマトリックス

};

#endif // !MESHIFIL_H_
