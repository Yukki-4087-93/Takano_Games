//--------------------------------------------------------------------------------------
//
// User Interface
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _UI_H_
#define _UI_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "object2d.h"
#include "renderer.h"
#include "texture.h"

//----------------------------------------
//クラス定義
//オブジェクト
//----------------------------------------
class CUi : public CObject2D
{
public:
	CUi();																//コンストラクタ
	~CUi();																//デストラクタ
	static CUi * Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd, CTexture::TEXTURE tex);		//クリエイト
	void Uninit()override;												//終了

private:
};

#endif // !_UI_H_
