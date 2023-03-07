//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "application.h"
#include "renderer.h"
#include "texture.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
const char* CTexture::s_FileName[] =
{// テクスチャのパスrunningman000
	"Data/TEXTURE/NewType.png",					// プレイヤー
	"Data/TEXTURE/Brokon_S.png",				// プレイヤーVer2
	"Data/TEXTURE/Brokon_T.png",				// プレイヤーVer3
	"Data/TEXTURE/Brokon_F.png",				// プレイヤーVer4
	"Data/TEXTURE/Bullet.png",					// 弾１
	"Data/TEXTURE/Bullet_E.png",				// 弾２
	"Data/TEXTURE/explosion000.png",			// 爆発
	"Data/TEXTURE/enemy000.png",				// エネミー
	"Data/TEXTURE/TutorialEnemy0.png",			// チュートリアルエネミー1
	"Data/TEXTURE/TutorialEnemy1.png",			// チュートリアルエネミー2
	"Data/TEXTURE/TutorialEnemy2.png",			// チュートリアルエネミー3
	"Data/TEXTURE/open_my_hart.png",			// ボス
	"Data/TEXTURE/RGBA2.png",					// 背景
	"Data/TEXTURE/Ranking_BG.png",				// 背景
	"Data/TEXTURE/joheki_2.png",				// 街
	"Data/TEXTURE/RED1.png",					// 街、ダメージ状態
	"Data/TEXTURE/RED2.png",					// 街、よりダメージ状態
	"Data/TEXTURE/RED3.png",					// 街、さらにダメージ状態
	"Data/TEXTURE/Taitoru_102.png",				// タイトル
	"Data/TEXTURE/reseut0099.png",				// リザルト
	"Data/TEXTURE/reseut0999.png",				// リザルト（死亡時）
	"Data/TEXTURE/number000.png",				// ナンバー
	"Data/TEXTURE/UI.png",						// ユーザーインターフェース
	"Data/TEXTURE/ranking_rank.png",			// ランキング
};

//どちらかの呼込数が一致していない状態ならエラーを出す(デバックだけ有効)
static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "数があっていません。");

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CTexture::CTexture()
{
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CTexture::~CTexture()
{
}

//--------------------------------------------------
// 全ての読み込み
//--------------------------------------------------
void CTexture::LoadAll()
{
	for (int nCountTex = 0; nCountTex < TEXTURE_MAX; nCountTex++)
	{
		if (s_pTexture[nCountTex] != nullptr)
		{// テクスチャの読み込みがされている
			continue;			//forの最初に戻す
		}

		Load((TEXTURE)nCountTex);
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void CTexture::Load(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// テクスチャの読み込みがされている
		return;
	}

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		s_FileName[inTexture],
		&s_pTexture[inTexture]);
}

//--------------------------------------------------
// 全ての解放
//--------------------------------------------------
void CTexture::ReleaseAll(void)
{
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (s_pTexture[nCntTex] != nullptr)
		{// テクスチャの解放
			s_pTexture[nCntTex]->Release();
			s_pTexture[nCntTex] = nullptr;
		}
	}
}

//--------------------------------------------------
// 解放
//--------------------------------------------------
void CTexture::Release(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// テクスチャの解放
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = nullptr;
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE inTexture)
{
	if (inTexture == TEXTURE_NONE)
	{// テクスチャを使用しない
		return nullptr;
	}

	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	// 読み込み
	Load(inTexture);

	return s_pTexture[inTexture];
}
