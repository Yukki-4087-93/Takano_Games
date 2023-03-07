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
{// テクスチャのパス
	"Data/TEXTURE/Title_103.png",				// タイトル
	"Data/TEXTURE/number001.png",				// ナンバー
	"Data/TEXTURE/UI.png",						// ユーザーインターフェース
	"Data/TEXTURE/UI2.png",						// ユーザーインターフェース
	"Data/TEXTURE/reseut0099.png",				// リザルト
	"Data/TEXTURE/reseut0999.png",				// リザルト（死亡時）
	"Data/TEXTURE/Ranking_BG.png",				// ランキングの背景
	"Data/TEXTURE/ranking_rank.png",			// ランキング
	"Data/TEXTURE/sky.jpg",						// 空
	"Data/TEXTURE/NewType.png",					// エフェクト用
	"Data/TEXTURE/damage_insignificant.png",	// 軽微ダメージ用
	"Data/TEXTURE/damage_medium_wave.png",		// 中破ダメージ用
	"Data/TEXTURE/damage_wave.png",				// 大破寸前ダメージ用
	"Data/TEXTURE/tutorial.jpg",				// tutorial用
	"Data/TEXTURE/EatMe.jpg",					// 食べられる運命の私
	"Data/TEXTURE/tutorial2.jpg",					// 説明
	"Data/TEXTURE/arrow.jpg",					// やじるし
	"Data/TEXTURE/tutorial3.jpg",					// 説明
	"Data/TEXTURE/TitleLogo.jpg",					// 説明
	"Data/TEXTURE/ResultLogo.png",					// 説明
};

//どちらかの呼込数が一致していない状態ならエラーを出す(デバックだけ有効)
static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "Texture Check Error");

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
			continue;			//最初に戻す
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
