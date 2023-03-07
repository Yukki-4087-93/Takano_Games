//--------------------------------------------------------------------------------------
//
// オブジェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "object.h"
#include "texture.h"

//----------------------------------------
//クラス定義
//オブジェクト
//----------------------------------------
class CObject2D : public CObject
{
public:
	// 頂点データ
	struct VERTEX_2D
	{
		D3DXVECTOR3 pos;														//位置
		float rhw;																//RHW
		D3DCOLOR col;															//色
		D3DXVECTOR2 tex;														//テクスチャ
	};

public:
	CObject2D();																//コンストラクタ
	~CObject2D();																//デストラクタ
	HRESULT Init()override;														//初期化
	static CObject2D * Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd);		//クリエイト
	void Update()override;														//更新
	void Draw()override;														//描画
	void Uninit()override;														//終了
	void SetPos(D3DXVECTOR3 pos) override;
	const D3DXVECTOR3& Getpos() const { return m_pos; }							//位置の取得
	void Addmove(D3DXVECTOR3 move) { m_pos += move; }							//移動量の加算
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }			//テクスチャの設定
	void PattarnTexture(int nPattarn,int nSplit);								//テクスチャアニメーションの設定
	void SetColor(D3DXCOLOR col);												//オブジェクトのカラー設定
	void SetSize(float fHi,float fWd);											//サイズ設定

protected:
	LPDIRECT3DVERTEXBUFFER9 m_Vertex;											//頂点情報取得の取得
	float m_fAngle;																//アングル
	float m_fLength;															//レングス
	float m_Change_Height;														//変更する高さ
	float m_Change_Width;														//変更する幅
	int m_nCounterAnim;															//アニメーションカウンター
	float m_fPatternAnim;														//アニメーションカウンター
	CTexture::TEXTURE m_texture;												//テクスチャの列挙型
};

#endif // !_OBJECT2D_H_
