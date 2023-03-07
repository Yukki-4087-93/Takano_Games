//--------------------------------------------------------------------------------------
//
// タイトルマネージャー
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
CBg *CTitle::m_pBg;

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
	//ゲーム内のオブジェクト生成
	m_pBg = CBg::Create(D3DXVECTOR3(1280.0f / 2, 720.0f / 2, 0.0f), 
		1280.0f, 
		720.0f, 
		CTexture::TEXTURE::TEXTURE_TITLE);

	m_pBg = CBg::Create(D3DXVECTOR3(540.0f, 630.0f, 0.0f),
		600.0f,
		300.0f,
		CTexture::TEXTURE::TEXTURE_TITLE_LOGO);

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
	if (m_pBg != nullptr)
	{//背景のポインターに値が入っていたとき
		m_pBg->Update();							//背景の更新
	}

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
//背景情報取得
//---------------------------------------
CBg * CTitle::GetBg()
{
	return m_pBg;
}
