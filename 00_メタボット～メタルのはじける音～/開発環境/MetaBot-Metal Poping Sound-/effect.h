//--------------------------------------------------------------------------------------
//
//エフェクト
//Author;takano
//
//--------------------------------------------------------------------------------------
#ifndef _EFFECT_H_													//このマクロ定義がなされなっかたら
#define _EFFECT_H_													//二重インクルード防止のマクロ定義

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include <d3dx9.h>
#include "object3d.h"

//----------------------------------------
//クラス定義
//エフェクト関連
//----------------------------------------
class CEffect : public CObject3D
{
public:
	CEffect();										//コンストラクタ
	~CEffect();										//デストラクタ
	HRESULT Init();									//初期化
	void Uninit();									//終了
	void Update();									//更新
	void Draw();									//描画
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 move);	//配置

public:
	static CEffect* Create(D3DXVECTOR3 pos,D3DXVECTOR3 move,D3DXCOLOR col,float fHi,float fWd);		//エフェクトの生成
	static void Spawn(D3DXVECTOR3 pos, int nMaxSpawn, float fRandMax, float fRandMin, D3DXCOLOR col, float fHi, float fWd);				//エフェクトの出現を管理する関数

private:
	D3DXVECTOR3 m_move;								//移動量
	D3DXCOLOR m_color;
};

#endif // !EFFECT_H_
