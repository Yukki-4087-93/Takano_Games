//--------------------------------------------------------------------------------------
//
// オブジェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "object.h"
#include "texture.h"

//----------------------------------------
//クラス定義
//オブジェクト
//----------------------------------------
class CObject3D : public CObject
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
	CObject3D();																//コンストラクタ
	~CObject3D();																//デストラクタ
	HRESULT Init()override;														//初期化
	static CObject3D * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float ObfHi, float ObfWd, CTexture::TEXTURE tex,bool bBillboard);		//クリエイト
	void Update()override;														//更新
	void Draw()override;														//描画
	void Uninit()override;														//終了
	const D3DXVECTOR3& Getpos() const;											//位置の取得
	void Addmove(D3DXVECTOR3 move);												//移動量の加算
	void SetTexture(CTexture::TEXTURE texture);									//テクスチャの設定
	void PattarnTexture(int nPattarn, int nSplit);								//テクスチャアニメーションの設定
	void SetColor(D3DXCOLOR col);												//オブジェクトのカラー設定
	void SetSize(float fHi, float fWd);											//サイズ設定
	void SetBillboard(bool bswitch) { m_bBillboard = bswitch; }					//ビルボードの設定

protected:
	LPDIRECT3DVERTEXBUFFER9 m_Vertex;											//頂点情報取得の取得
	CTexture::TEXTURE m_texture;												//テクスチャの列挙型
	D3DXMATRIX m_mtxWorld;														//ワールドマトリックス
	float m_fAngle;																//アングル
	float m_fLength;															//レングス
	float m_Change_Height;														//変更する高さ
	float m_Change_Width;														//変更する幅
	float m_fPatternAnim;														//アニメーションカウンター
	int m_nCounterAnim;															//アニメーションカウンター
	bool m_bBillboard;															//ビルボードか否か

};

#endif // !_OBJECT3D_H_
