//--------------------------------------------------------------------------------------
//
// 街
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _PROTECT_CITY_H_
#define _PROTECT_CITY_H_

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
class Cprotect_City : public CObject2D
{
public:
	/*状態管理*/
	enum CITY_STATE
	{
		STARE_NOMAL = 0,
		STARE_DAMAGE,
		STATE_MAX
	};

	Cprotect_City();																//コンストラクタ
	~Cprotect_City();																//デストラクタ
	HRESULT Init()override;													//初期化
	static Cprotect_City * Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd);		//クリエイト
	void Update()override;													//更新
	void Draw()override;													//描画
	void Uninit()override;													//終了
	void Damage(int nDamage);
	void State();
	void SetState(Cprotect_City::CITY_STATE state);
	static int GetLife();

private:
	int m_Damage;									//ダメージを取得する変数
	static int m_nLife;									//寿命
	int m_nColorCount;
	Cprotect_City::CITY_STATE m_nState;
};

#endif // !_PROTECT_CITY_H_
