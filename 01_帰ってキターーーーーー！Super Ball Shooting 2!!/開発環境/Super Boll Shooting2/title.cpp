//--------------------------------------------------------------------------------------
//
// タイトル
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "application.h"
#include "title.h"
#include "object2d.h"
#include "input.h"
#include "bg.h"
#include "sound.h"

//--------------------------------------
//グローバル変数
//--------------------------------------
//CBg *CTitle::m_pBg;
CTexture *CTitle::m_pTexture;

//--------------------------------------
//コンストラクタ
//--------------------------------------
CTitle::CTitle()
{

}

//--------------------------------------
//デストラクタ
//--------------------------------------
CTitle::~CTitle()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CTitle::Init()
{
	m_pTexture = new CTexture;

	//ゲーム内のオブジェクト生成
	CBg* pBg = CApplication::GetBg()->Create(D3DXVECTOR3(1280.0f / 2, 
											720.0f / 2, 0.0f), 
											1280.0f, 
											720.0f, 
											CTexture::TEXTURE::TEXTURE_TITLE);
	//m_pBg = CBg::Create(D3DXVECTOR3(1280.0f / 2, 720.0f / 2, 0.0f), 1280.0f, 720.0f, CTexture::TEXTURE::TEXTURE_TITLE);

	CApplication::GetSound()->Play(CSound::SOUND_BGM_TITLE);

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CTitle::Uninit()
{
	CApplication::GetSound()->Stop(CSound::SOUND_BGM_TITLE);

	//オブジェクトの全破棄
	CObject::ReleaseAll();
}

//--------------------------------------
//更新
//--------------------------------------
void CTitle::Update()
{

	//if (m_pBg != nullptr)
	//{//背景のポインターに値が入っていたとき
	//	m_pBg->Update();							//背景の更新
	//}

	//決定キー(ENTER)が押されたかどうか
	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_RETURN))
	{
		CApplication::SetMode(CApplication::MODE::MODE_GAME);
	}
}

//--------------------------------------
//描画
//--------------------------------------
void CTitle::Draw()
{

}

//---------------------------------------
// テクスチャの情報取得
//---------------------------------------
CTexture* CTitle::GetTexture()
{
	return m_pTexture;
}
