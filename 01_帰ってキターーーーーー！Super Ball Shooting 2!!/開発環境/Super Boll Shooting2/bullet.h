//--------------------------------------------------------------------------------------
//
// 弾
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "object2d.h"

//----------------------------------------
//クラス定義
//弾関係一式
//----------------------------------------
class CBullet : public CObject2D
{
public:
	/*弾の種類*/
	enum BULLET_TYPE
	{
		TYPE_NOMAL = 0,					//ノーマルの弾
		TYPE_BURST,						//周囲に広がる弾
		TYPE_DIFFUSION,					//3方向散弾
		TYPE_GIJIMASUPAA,				//疑似マスパー
		TYPE_MAX,						//最大
		TYPE_NOME,						//弾の種類を使用しない状態
	};
	/*弾のユーザー*/
	enum BULLET_USER
	{
		USER_PLAYER = 0,				//プレイヤーユーザー		
		USER_ENEMY,						//エネミーユーザー
		USER_MAX,						//ユーザーの最大
		USER_NOME,						//ユーザーが使用されていない状態
	};

	CBullet();																									//コンストラクタ
	~CBullet() ;																								//デストラクタ
	HRESULT Init()override;																						//初期化
	static CBullet * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, CTexture::TEXTURE tex,CBullet::BULLET_USER user);		//クリエイト
	void Update()override;																						//更新
	void Draw()override;																						//描画
	void Uninit()override;																						//終了
	void Release();																								//弾の開放
	void SetMove(D3DXVECTOR3 move);																				//移動量の設定
	D3DXVECTOR3 GetMove();																						//移動量の情報取得
	CBullet::BULLET_USER GetUser();																				//ユーザー情報取得
	void SetTypeBullet(CBullet::BULLET_USER user);																//ユーザー情報の設定
	
private:
	void SubstructLife();											//ライフ減少処理
	void Collisison();												//更新で使用している当たり判定

private:
	D3DXVECTOR3 m_nMove;											//移動量
	CBullet::BULLET_USER m_TypeUser;								//ユーザーのポインタ
	int m_nBLife;
	int m_nColorCount;
	int m_nPattarnCount;
};

#endif // !_BULLET_H_

/*2022/06/22	フィールドノート

弾のテクスチャは１つにしたい。

//バレッド
ststic LPDIRECT3DTEXTURE9 m_Texture;	//共有テクスチャのポインタ

HRESULT CBullet::Load()
{
	テクスチャの生成
}

void CBullet::Unload
{
	テクスチャの破棄or解放
}

object2dh

void BindTexture(LPDIRECT3DTEXTURE9 pTexture)

LPDIRECT3DTEXTURE9 m_Texture;
*/