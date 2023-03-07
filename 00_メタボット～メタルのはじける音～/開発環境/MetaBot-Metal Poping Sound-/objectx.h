//--------------------------------------------------------------------------------------
//
// オブジェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "object.h"
#include "texture.h"

//----------------------------------------
//クラス定義
//オブジェクト
//----------------------------------------
class CObjectX : public CObject
{
public:
	// 頂点データ
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;														//頂点座標
		D3DXVECTOR3 nor;														//法線ベクトル
		D3DCOLOR col;															//頂点カラー
		D3DXVECTOR2 tex;														//テクスチャの座標
	};

public:
	CObjectX();																	//コンストラクタ
	~CObjectX();																//デストラクタ
	HRESULT Init()override;														//初期化
	static CObjectX * Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd);		//クリエイト
	void Update()override;														//更新
	void Draw()override;														//描画
	void Uninit()override;														//終了
	const D3DXVECTOR3& Getpos() const;											//位置の取得

protected:
	LPD3DXMESH m_Mesh;															//メッシュ情報
	LPD3DXBUFFER m_BuffMat;														//マテリアル情報
	DWORD m_nNumMat;															//マテリアル情報の数
	D3DXVECTOR3 m_rotDest;														//元の角度
	D3DXMATRIX m_mtxWorld;														//ワールドマトリックス
};

#endif // !_OBJECTX_H_
