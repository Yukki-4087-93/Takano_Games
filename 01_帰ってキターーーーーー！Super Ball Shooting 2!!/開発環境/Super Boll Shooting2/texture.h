//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>

//==================================================
// 定義
//==================================================
class CTexture
{
public: /* 定義 */
	enum TEXTURE
	{
		TEXTURE_PLAYER = 0,								// プレイヤー
		TEXTURE_PLAYER_VER2,							// ダメージを負ったプレイヤー
		TEXTURE_PLAYER_VER3,							// よりダメージを負ったプレイヤー
		TEXTURE_PLAYER_VER4,							// さらにダメージを負ったプレイヤー
		TEXTURE_BULLET,									// 弾１
		TEXTURE_BULLET_E,								// 弾２
		TEXTURE_EXPLOSION,								// 爆発
		TEXTERE_ENEMETYPE,								// エネミー１
		TEXTERE_TUTORIAL_ZERO,							// チュートリアルエネミー0
		TEXTERE_TUTORIAL_ONE,							// チュートリアルエネミー1
		TEXTERE_TUTORIAL_TWO,							// チュートリアルエネミー2
		TEXTURE_AGE,									// ボス
		TEXTERE_BG,										// 背景
		TEXTURE_RANKING_BG,								// ランキングの背景
		TEXTURE_PROTECT,								// 街のテクスチャ
		TEXTURE_PROTECT_ALERT,							// 街のテクスチャダメージ
		TEXTURE_PROTECT_DEEPALERT,						// 街のテクスチャダメージ2
		TEXTURE_PROTECT_DEATHALERT,						// 街のテクスチャダメージ3
		TEXTURE_TITLE,									// タイトルのテクスチャ
		TEXTURE_RESULT,									// リザルトのテクスチャ
		TEXTURE_RESULT_DEATH,							// リザルトのテクスチャ デスバージョン
		TEXTURE_SCORE,									// スコアのテクスチャ
		TEXTURE_UI,										// UIのテクスチャ
		TEXTURE_RANKING,								// ランキングロゴ
		TEXTURE_MAX,									// 最大
		TEXTURE_NONE,									// 使用しない
	};

	static const char* s_FileName[];					// ファイルパス

public:
	CTexture();											// デフォルトコンストラクタ
	~CTexture();										// デストラクタ

public: /* メンバ関数 */
	void LoadAll();										// 全ての読み込み
	void Load(TEXTURE inTexture);						// 指定の読み込み
	void ReleaseAll();									// 全ての破棄
	void Release(TEXTURE inTexture);					// 指定の破棄
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// 情報の取得

private: /* メンバ変数 */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];			// テクスチャの情報
};

#endif // !_TEXTURE_H_
