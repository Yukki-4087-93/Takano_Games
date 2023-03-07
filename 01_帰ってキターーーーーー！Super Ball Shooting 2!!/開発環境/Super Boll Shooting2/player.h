//--------------------------------------------------------------------------------------
//
// オブジェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _CPLAYER_H_
#define _CPLAYER_H_

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
class CPlayer : public CObject2D
{
public:

	/*プレイヤーの状態管理*/
	enum PLAYER_STATE
	{
		STARE_NOMAL = 0,
		STARE_DAMAGE,
		STATE_MAX
	};

	CPlayer();																//コンストラクタ
	~CPlayer();																//デストラクタ
	HRESULT Init()override;													//初期化
	static CPlayer * Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd);		//クリエイト
	void Update()override;													//更新
	void Draw()override;													//描画
	void Uninit()override;													//終了
	void HitDamage(int nDamage);											//ダメージ処理
	void SetState(CPlayer::PLAYER_STATE state);

private:
	void Command();							//更新で行うプレイヤーの動き(移動や弾の発射など)処理
	void CommandBullet();							//更新で行う弾関連の処理
	void Move();								//移動量などの設定
	void Collision();							//画面端などの当たり判定処理
	void State();

private:
	D3DXVECTOR3 m_move;								//プレイヤーの移動量
	int m_nCount;									//弾の発射カウント用の変数
	int m_nPi;										//弾の角度変更用の変数
	int m_nRand;									//ランダム用の変数
	int m_Damage;									//ダメージを取得する変数
	int m_nLife;									//寿命
	int m_nPressCount;								//ボタンを押したタイミングで使用するカウント用変数
	int m_nColorCount;
	bool m_bSetTexture;	
	D3DXCOLOR m_col;
	CPlayer::PLAYER_STATE m_State;

};

#endif // !_CPLAYER_H_
