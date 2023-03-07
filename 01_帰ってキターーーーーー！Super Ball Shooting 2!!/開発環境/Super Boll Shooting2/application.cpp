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
#include "renderer.h"
#include "title.h"
#include "texture.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "input.h"
#include "sound.h"
#include "object.h"
#include "bg.h"
#include <assert.h>

//--------------------------------------
//グローバル変数
//--------------------------------------
CRenderer *CApplication::m_aRenderer;
CTitle *CApplication::m_pTitle;
CTexture *CApplication::m_pTexture;
CGame *CApplication::m_pGame;
CResult *CApplication::m_pResult;
CRanking *CApplication::m_pRanking;
CInput *CApplication::m_pInputKyeboard;
CApplication::MODE CApplication::m_mode;
CSound *CApplication::m_pSound;
CBg *CApplication::m_pBg;

//--------------------------------------
//コンストラクタ
//--------------------------------------
CApplication::CApplication()
{
	m_mode = CApplication::MODE_TITLE;
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CApplication::~CApplication()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CApplication::Init(HINSTANCE hInctance, HWND hWnd, bool bWindow)
{
	//ポインタゴミが入っていないかチェック
	CApplication::ReleseDustCheck();
	
	//動的確保
	m_aRenderer = new CRenderer;
	m_pInputKyeboard = new CInput;
	m_pSound = new CSound;
	m_pTitle = new CTitle;
	m_pTexture = new CTexture;
	m_pGame = new CGame;
	m_pResult = new CResult;
	m_pRanking = new CRanking;

	//サウンドの初期化
	m_pSound->Init();

	//初期化
	m_aRenderer->Init(hWnd, bWindow);

	//キーボードの初期化
	m_pInputKyeboard->Initkeyboard(hInctance, hWnd);
	
	SetMode(m_mode);

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CApplication::Uninit()
{
	//ゴミが入っていないか確認
	CApplication::ReleseDustCheck();
}

//--------------------------------------
//更新
//--------------------------------------
void CApplication::Update()
{
	if (m_pInputKyeboard != nullptr)
	{//キーボードのポインタに値が入っていたとき
		m_pInputKyeboard->Updatekeyboard();			//キーボードの更新
	}

	if (m_aRenderer != nullptr)
	{//レンダラーのポインタに値が入っていたとき
		m_aRenderer->Update();						//レンダラーの更新
	}

	switch (m_mode)
	{
	case CApplication::MODE_TITLE:
		//タイトルの更新
		m_pTitle->Update();

		break;

	case CApplication::MODE_GAME:
		//ゲームの更新
		m_pGame->Update();

		break;

	case CApplication::MODE_RESULT:
		//リザルトの更新
		m_pResult->Update();

		break;

	case CApplication::MODE_RANKING:
		//ランキングの更新
		m_pRanking->Update();

		break;

	default:	//それ以外
		assert(false);
		break;
	}
}

//--------------------------------------
//描画
//--------------------------------------
void CApplication::Draw()
{
	if (m_aRenderer != nullptr)
	{
		//レンダラーの描画
		m_aRenderer->Draw();
	}
}

//--------------------------------------
//終了処理とダストチェック用関数
//--------------------------------------
void CApplication::ReleseDustCheck()
{
	if (m_aRenderer != nullptr)
	{//レンダラーのポインタに何かしらの値が入っていたら
		m_aRenderer->Uninit();					//終了処理
		delete m_aRenderer;						//レンダラーポインタを解放
		m_aRenderer = nullptr;					//ヌルを代入
	}

	if (m_pTexture != nullptr)
	{//テクスチャのポインタに何かしらの値が入っていたら
		m_pTexture->ReleaseAll();				//テクスチャ全体の終了処理
		delete m_pTexture;						//テクスチャのポインタを解放
		m_pTexture = nullptr;					//ヌルを代入
	}

	if (m_pGame != nullptr)
	{//ゲームのポインタに何かしらの値が入っていたら
		m_pGame->Uninit();						//ゲーム全体の終了処理
		delete m_pGame;							//ゲームのポインタを解放
		m_pGame = nullptr;						//ヌルを代入
	}

	if (m_pInputKyeboard != nullptr)
	{//キーボードのポインタに何かしらの値が入っていたら
		m_pInputKyeboard->Uninitkeyboard();		//終了処理
		delete m_pInputKyeboard;				//キーボードのポインタを解放
		m_pInputKyeboard = nullptr;				//ヌルを代入
	}

	if (m_pSound != nullptr)
	{//サウンドのポインタに何かしらの値が入っていたら
		m_pSound->Uninit();						//終了処理
		delete m_pSound;						//サウンドのポインタを開放
		m_pSound = nullptr;						//ヌルを代入
	}

	if (m_pBg != nullptr)
	{//背景のポインタに何かしらの値が入っていたら
		m_pBg->Uninit();						//終了処理
	}
}

//---------------------------------------
//モードの設定
//---------------------------------------
void CApplication::SetMode(MODE mode)
{
	switch (m_mode)
	{
	case CApplication::MODE_TITLE:
		//タイトルの終了
		m_pTitle->Uninit();

		break;

	case CApplication::MODE_GAME:
		//ゲームの終了
		m_pGame->Uninit();

		break;

	case CApplication::MODE_RESULT:
		//リザルトの終了
		m_pResult->Uninit();

		break;

	case CApplication::MODE_RANKING:
		//ランキングの終了
		m_pRanking->Uninit();

		break;

	default:	//それ以外
		assert(false);
		break;
	}

	//生成したオブジェクトの全破棄(オブジェクトが残っていたら困るので)
	CObject::ReleaseAll();

	//モードを設定する
	m_mode = mode;

	switch (m_mode)
	{
	case CApplication::MODE_TITLE:
		//タイトルの初期化
		m_pTitle->Init();

		break;

	case CApplication::MODE_GAME:
		//ゲームの初期化
		m_pGame->Init();

		break;

	case CApplication::MODE_RESULT:
		//リザルトの初期化
		m_pResult->Init();
		
		break;

	case CApplication::MODE_RANKING:
		//ランキングの初期化
		m_pRanking->Init();
		
		break;
	
	default:	//それ以外
		assert(false);
		break;
	}

}
