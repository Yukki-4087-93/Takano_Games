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
#include "camera.h"
#include "texture.h"
#include "input.h"
#include "sound.h"
#include "object.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include <assert.h>

//--------------------------------------
//静的メンバ変数の宣言
//--------------------------------------
CApplication::MODE CApplication::m_mode;
CRenderer *CApplication::m_aRenderer;
CTexture *CApplication::m_pTexture;
CInput *CApplication::m_pInputKeyboard;
CTitle *CApplication::m_pTitle;
CGame *CApplication::m_pGame;
CResult *CApplication::m_pResult;
CRanking *CApplication::m_pRanking;
CSound *CApplication::m_pSound;

//--------------------------------------
//コンストラクタ
//--------------------------------------
CApplication::CApplication()
{
	//モードの初期値を設定(クリア)
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
	//ポインタにゴミが入っていないかチェック
	ReleseDustCheck();

	//動的確保
	m_pInputKeyboard = new CInput;				//キーボードインプットの動的確保
	m_aRenderer = new CRenderer;				//レンダラー（描画関係）の動的確保
	m_pTexture = new CTexture;					//テクスチャの動的確保
	m_pSound = new CSound;						//サウンドの動的確保
	m_pTitle = new CTitle;						//タイトルの動的確保
	m_pGame = new CGame;						//ゲームの動的確保
	m_pResult = new CResult;					//リザルトの動的確保
	m_pRanking = new CRanking;					//ランキングの動的確保

	//レンダラー(描画関係)の初期化
	m_aRenderer->Init(hWnd, bWindow);

	//サウンドの初期化
	m_pSound->Init();

	//キーボードの初期化
	m_pInputKeyboard->Initkeyboard(hInctance, hWnd);

	//モードの設定
	SetMode(m_mode);

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CApplication::Uninit()
{
	//ゴミが入っていないか確認
	ReleseDustCheck();
}

//--------------------------------------
//更新
//--------------------------------------
void CApplication::Update()
{
	if (m_aRenderer != nullptr)
	{//レンダラーのポインタに値が入っていたとき
		m_aRenderer->Update();							//レンダラーの更新
	}

	if (m_pInputKeyboard != nullptr)
	{//キーボードのポインタに値が入っていたとき
		m_pInputKeyboard->Updatekeyboard();				//キーボードの更新
	}

	if (m_mode != MODE_NONE)
	{//モードのポインタに値が入っていたとき
		UpdateMode(m_mode);								//モードの更新
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
	if (m_pInputKeyboard != nullptr)
	{//キーボードのポインタに何かしらの値が入っていたら
		m_pInputKeyboard->Uninitkeyboard();		//終了処理
		delete m_pInputKeyboard;				//キーボードのポインタを解放
		m_pInputKeyboard = nullptr;				//ヌルを代入
	}

	if (m_aRenderer != nullptr)
	{//レンダラーのポインタに何かしらの値が入っていたら
		m_aRenderer->Uninit();					//終了処理
		delete m_aRenderer;						//レンダラーポインタを解放
		m_aRenderer = nullptr;					//ヌルを代入
	}

	if (m_pTitle != nullptr)
	{//タイトルのポインタに何かしらの値が入っていたら
		m_pTitle->Uninit();						//タイトルの終了処理
		delete m_pTitle;						//タイトルのポインタを解放
		m_pTitle = nullptr;						//ヌルを代入
	}

	if (m_pGame != nullptr)
	{//ゲームのポインタに何かしらの値が入っていたら
		m_pGame->Uninit();						//ゲームの終了処理
		delete m_pGame;							//ゲームのポインタを解放
		m_pGame = nullptr;						//ヌルを代入
	}

	if (m_pResult != nullptr)
	{//リザルトのポインタに何かしらの値が入っていたら
		m_pResult->Uninit();					//リザルトの終了処理
		delete m_pResult;						//リザルトのポインタを解放
		m_pResult = nullptr;					//ヌルを代入
	}

	if (m_pRanking != nullptr)
	{//ランキングのポインタに何かしらの値が入っていたら
		m_pRanking->Uninit();					//ランキングの終了処理
		delete m_pRanking;						//ランキングのポインタを解放
		m_pRanking = nullptr;					//ヌルを代入
	}

	if (m_pTexture != nullptr)
	{//テクスチャのポインタに何かしらの値が入っていたら
		m_pTexture->ReleaseAll();				//テクスチャ全体の終了処理
		delete m_pTexture;						//テクスチャのポインタを解放
		m_pTexture = nullptr;					//ヌルを代入
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
		
		m_pTitle->Uninit();			//タイトルの終了

		break;

	case CApplication::MODE_GAME:
		
		m_pGame->Uninit();			//ゲームの終了

		break;

	case CApplication::MODE_RESULT:
		
		m_pResult->Uninit();		//リザルトの終了

		break;

	case CApplication::MODE_RANKING:
		
		m_pRanking->Uninit();		//ランキングの終了

		break;

	default:	//それ以外

		assert(false);				//プログラムを止めます(デバックの時のみ)

		break;
	}

	//生成したオブジェクトの全開放(オブジェクトが残っていたら困るので)
	CObject::ReleaseAll();

	//モードを再設定する
	m_mode = mode;

	switch (m_mode)
	{
	case CApplication::MODE_TITLE:
		
		m_pTitle->Init();			//タイトルの初期化

		break;

	case CApplication::MODE_GAME:

		m_pGame->Init();			//ゲームの初期化

		break;

	case CApplication::MODE_RESULT:

		m_pResult->Init();			//リザルトの初期化

		break;

	case CApplication::MODE_RANKING:

		m_pRanking->Init();			//ランキングの初期化

		break;

	default:	//それ以外

		assert(false);				//プログラムを止める(デバックの時のみ)

		break;
	}

}

//---------------------------------------
//モードの設定
//関数分けです。
//---------------------------------------
void CApplication::UpdateMode(MODE mode)
{
	switch (mode)
	{
	case CApplication::MODE_TITLE:

		m_pTitle->Update();			//タイトル更新

		break;

	case CApplication::MODE_GAME:

		m_pGame->Update();			//ゲームの更新

		break;

	case CApplication::MODE_RESULT:

		m_pResult->Update();		//リザルトの更新

		break;

	case CApplication::MODE_RANKING:

		m_pRanking->Update();		//ランキングの更新

		break;

	default:	//それ以外のタイプ(例外)が入って来たら

		assert(false);					//プログラムを止めます。

		break;
	}
}
