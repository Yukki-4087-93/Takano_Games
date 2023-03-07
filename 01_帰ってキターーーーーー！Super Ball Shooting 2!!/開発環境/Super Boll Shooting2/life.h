//-----------------------------------------------------------------------------------------------------------
//
//ライフ
//Author::TAKANO
//
//-----------------------------------------------------------------------------------------------------------
#ifndef _LIFE_H_
#define _LIFE_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "object2d.h"
#include "renderer.h"
#include "texture.h"

//----------------------------------------
//クラス定義
//キーボード系統の入力処理
//----------------------------------------
class CLife : public CObject2D
{
public:
	CLife();
	~CLife();
	HRESULT Init()override;													//初期化
	static CLife * Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd);		//クリエイト
	void Update()override;													//更新
	void Draw()override;													//描画
	void Uninit()override;													//終了
	void HitDamage(int nDamage);											//ダメージ処理
	void SetLife();
	void HitLife();

private:
	int m_nCountNumber;

};


#endif // !_LIFE_H_