//--------------------------------------------------------------------------------------
//
// オブジェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "object2d.h"
#include "renderer.h"

//----------------------------------------
//クラス定義
//オブジェクト
//----------------------------------------
class CExplosion : public CObject2D
{
public:
	CExplosion();														//コンストラクタ
	~CExplosion();														//デストラクタ
	HRESULT Init()override;												//初期化
	static CExplosion * Create(D3DXVECTOR3 pos, D3DXCOLOR col);			//クリエイト
	void Update()override;												//更新
	void Draw()override;												//描画
	void Uninit()override;												//終了

private:
};

#endif // !_EXPLOSION_H_
