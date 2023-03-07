//--------------------------------------------------------------------------------------
//
// エフェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "object2d.h"

//----------------------------------------
//クラス定義
//エフェクト
//----------------------------------------
class CEffect : public CObject2D
{
public:
	CEffect();																				//コンストラクタ
	~CEffect();																				//デストラクタ
	HRESULT Init()override;																	//初期化
	static CEffect * Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife);		//クリエイト
	void Update()override;																	//更新
	void Draw()override;																	//描画
	void Uninit()override;																	//終了
	void SetEffect(D3DXCOLOR col, float fRadius, int nLife);

private:
	float m_fRadius;												//半径
	int m_nRun;														//走るカウント
	D3DXCOLOR m_Col;												//色
	int m_nLife;													//寿命
	D3DXVECTOR3 m_nMove;											//移動量
};

#endif // !_EFFECT_H_
