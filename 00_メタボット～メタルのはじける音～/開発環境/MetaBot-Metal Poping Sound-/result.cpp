//--------------------------------------------------------------------------------------
//
// リザルトマネージャー
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
CBg *CResult::m_pBg;
//CTexture *CResult::m_pTexture;
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
	//m_pTexture = new CTexture;

	Set(m_nResult);

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CResult::Uninit()
{
	CApplication::GetSound()->Stop();
}

//--------------------------------------
//更新
//--------------------------------------
void CResult::Update()
{

	if (m_pBg != nullptr)
	{//背景のポインターに値が入っていたとき
		m_pBg->Update();							//背景の更新
	}

	//決定キー(ENTER)が押されたかどうか
	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_RETURN))
	{
		CApplication::SetMode(CApplication::MODE::MODE_RANKING);
	}

	//決定キー(ENTER)が押されたかどうか
	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_SPACE))
	{
		CApplication::SetMode(CApplication::MODE::MODE_GAME);
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
	//リザルト画面の結果を変える変数に代入
	m_nResult = nCount;

	//サウンド情報の取得
	CSound* pSound = CApplication::GetSound();


	if (m_nResult == 0)
	{//結果が0だったら
		
		//背景の生成
		m_pBg = CBg::Create(D3DXVECTOR3(1280.0f / 2, 720.0f / 2, 0.0f), 
							1280.0f, 
							720.0f, 
							CTexture::TEXTURE::TEXTURE_RESULT);

		//ロゴの生成
		m_pBg = CBg::Create(D3DXVECTOR3(1280.0f / 2, 630.0f, 0.0f),
			600.0f,
			300.0f,
			CTexture::TEXTURE::TEXTURE_RESULT_LOGO);

		//BGMを再生
		pSound->Play(CSound::SOUND_BGM_RESULT);
	}

	else
	{//結果が0以外だったら
		
		//背景の生成
		m_pBg = CBg::Create(D3DXVECTOR3(1280.0f / 2, 720.0f / 2, 0.0f),
							1280.0f,
							720.0f,
							CTexture::TEXTURE::TEXTURE_RESULT_DEATH);

		//ロゴの生成
		m_pBg = CBg::Create(D3DXVECTOR3(1280.0f / 2, 630.0f, 0.0f),
			600.0f,
			300.0f,
			CTexture::TEXTURE::TEXTURE_RESULT_LOGO);
		
		pSound->Play(CSound::SOUND_BGM_RESULT_DETH);		//BGMを再生
		pSound->Play(CSound::SOUND_SE_EAT);					//SEを再生
		pSound->Play(CSound::SOUND_SE_GAMEOVER);			//永遠に流れるSEを再生
		pSound->SetVolume(CSound::SOUND_SE_GAMEOVER,0.8f);	//永遠に流れるSEの音量を設定
	}
}

//---------------------------------------
//背景情報取得
//---------------------------------------
CBg * CResult::GetBg()
{
	return m_pBg;
}

////---------------------------------------
//// テクスチャの情報取得
////---------------------------------------
//CTexture* CResult::GetTexture()
//{
//	return m_pTexture;
//}
