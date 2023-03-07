//---------------------------------------------------------
//
//背景
//Author::TAKANO
//
//---------------------------------------------------------
#ifndef _BG_H_
#define _BG_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "object2d.h"

//----------------------------------------
//クラス定義
//背景(オブジェクトから派生)
//----------------------------------------
class CBg : public CObject2D
{
public:
	CBg();																//コンストラクタ
	~CBg() override;													//デストラクタ

	void Uninit() override;												//終了
	void Update() override;												//更新
	void Draw() override;												//描画

public:
	static CBg * Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd,CTexture::TEXTURE tex);		//クリエイト
};

#endif // !_BG_H_
