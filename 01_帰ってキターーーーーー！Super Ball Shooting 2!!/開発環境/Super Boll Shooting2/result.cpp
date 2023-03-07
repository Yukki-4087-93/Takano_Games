//--------------------------------------------------------------------------------------
//
// マネージャーアプリケーション
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "application.h"
#include "result.h"
#include "object2d.h"
#include "input.h"
#include "bg.h"
#include "sound.h"

//--------------------------------------
//グローバル変数
//--------------------------------------
//CBg *CResult::m_pBg;
CTexture *CResult::m_pTexture;
int CResult::m_nResult;

//--------------------------------------
//コンストラクタ
//--------------------------------------
CResult::CResult()
{

}

//--------------------------------------
//デストラクタ
//--------------------------------------
CResult::~CResult()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CResult::Init()
{
	m_pTexture = new CTexture;

	Set(m_nResult);

	CApplication::GetSound()->Play(CSound::SOUND_BGM_RESULT);

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CResult::Uninit()
{
	CApplication::GetSound()->Stop(CSound::SOUND_BGM_RESULT);
}

//--------------------------------------
//更新
//--------------------------------------
void CResult::Update()
{

	//if (m_pBg != nullptr)
	//{//背景のポインターに値が入っていたとき
	//	m_pBg->Update();							//背景の更新
	//}

	//決定キー(ENTER)が押されたかどうか
	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_RETURN))
	{
		CApplication::SetMode(CApplication::MODE::MODE_RANKING);
	}
}

//--------------------------------------
//描画
//--------------------------------------
void CResult::Draw()
{

}

//--------------------------------------
//リザルトの設定
//--------------------------------------
void CResult::Set(int nCount)
{
	m_nResult = nCount;

	if (nCount == 0)
	{
		//ゲーム内のオブジェクト生成
		CBg* pBg = CApplication::GetBg()->Create(D3DXVECTOR3(1280.0f / 2, 720.0f / 2, 0.0f), 
												1280.0f, 
												720.0f, 
												CTexture::TEXTURE::TEXTURE_RESULT);
	}
	else
	{
		//ゲーム内のオブジェクト生成
		CBg* pBg = CApplication::GetBg()->Create(D3DXVECTOR3(1280.0f / 2, 720.0f / 2, 0.0f),
												1280.0f,
												720.0f,
												CTexture::TEXTURE::TEXTURE_RESULT_DEATH);
	}
}

//---------------------------------------
// テクスチャの情報取得
//---------------------------------------
CTexture* CResult::GetTexture()
{
	return m_pTexture;
}
