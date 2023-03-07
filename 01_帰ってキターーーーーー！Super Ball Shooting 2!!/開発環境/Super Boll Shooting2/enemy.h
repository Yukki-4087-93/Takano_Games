//--------------------------------------------------------------------------------------
//
// オブジェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _CENEMY_H_
#define _CENEMY_H_

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
class CEnemy : public CObject2D
{
public:
	/*エネミーのタイプ*/
	enum ENEMY_TYPE
	{
		TYPE_NOME = 0,					//エネミーを使用しない状態
		TYPE_TUTORIAL,					//チュートリアルエネミー
		TYPE_TUTORIAL_BACK,
		TYPE_STRAIGHT,					//真っ直ぐ進むエネミー
		TYPE_COLLISION_DOWN,			//下壁に反射するエネミー	
		TYPE_COLLISION_UP,				//上壁に反射するエネミー
		TYPE_STRANGER_DOWN,				//変な動きするエネミーの初動
		TYPE_STRANGER_UP,				//変な動きするエネミーの初動
		TYPE_STRENGER_GO,				//変な動きするエネミーの次手
		TYPE_STRENGER_CHANGE,			//変な動きするエネミーの次手
		TYPE_MIDBOSS,					//中ボスクラスのエネミー
		TYPE_BOSS,
		TYPE_BOSS_DOWN,
		TYPE_BOSS_UP,
		TYPE_BOSS_TWO,
		TYPE_BOSS_TWO_DOWN,
		TYPE_BOSS_TWO_UP,
		TYPE_BOSS_TREE,
		TYPE_BOSS_TREE_DOWN,
		TYPE_BOSS_TREE_UP,
		TYPE_MAX,						//最大エネミー
	};

	/*エネミーの状態管理*/
	enum ENEMY_STATE
	{
		STARE_NOMAL = 0,
		STARE_DAMAGE,
		STATE_MAX
	};

	CEnemy();																								//コンストラクタ
	~CEnemy();																								//デストラクタ
	HRESULT Init()override;																					//初期化
	static CEnemy * Create(D3DXVECTOR3 pos,int nlife,float ObfHi, float ObfWd,CTexture::TEXTURE tex, CObject::ObjType typ, CEnemy::ENEMY_TYPE type, float sin, float cos);				//クリエイト
	void Update()override;																					//更新
	void Draw()override;																					//描画
	void Uninit()override;																					//終了
	void NumTypeEnemy(CEnemy::ENEMY_TYPE type,float fsin,float fcos);																//エネミー情報の設定
	void HitLife(int nDamage);
	void SetColor(D3DXCOLOR col);
	void SetState(CEnemy::ENEMY_STATE state);

private:
	void ShotBullet();
	void Updatepos();
	void State();
	void Relesepos();

private:
	D3DXVECTOR3 m_move;						//エネミーの移動量
	int m_nECount;							//カウント用変数
	int m_nELife;							//エネミー用体力
	float m_fSpeed_sin;				
	float m_fSpeed_cos;
	int m_nSwitch;
	int m_nOpen_Fire;
	int m_nColorCount;
	D3DXCOLOR m_col;
	CEnemy::ENEMY_TYPE m_nType;				
	CEnemy::ENEMY_STATE m_nState;
};

#endif // !_CENEMY_H_
