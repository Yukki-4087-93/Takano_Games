//--------------------------------------------------------------------------------------
//
// ゲームマネージャー
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "application.h"
#include "game.h"
#include "renderer.h"
#include "object.h"
#include "object2d.h"
#include "player.h"
#include "input.h"
#include "bg.h"
#include "enemymanager.h"
#include "protect_city.h"
#include "score.h"
#include "ui.h"
#include "sound.h"
#include "ranking.h"

//--------------------------------------
//静的メンバ変数の宣言
//--------------------------------------
CPlayer *CGame::m_pPlayer;
CEnemyManager *CGame::m_pEnemyManager;
Cprotect_City *CGame::m_pProtect_City;
CUi *CGame::m_pUi;
CScore *CGame::m_pScore;

//--------------------------------------
//コンストラクタ
//--------------------------------------
CGame::CGame()
{

}

//--------------------------------------
//デストラクタ
//--------------------------------------
CGame::~CGame()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CGame::Init()
{
	//エネミーマネージャーの動的確保
	m_pEnemyManager = new CEnemyManager;

	//エネミーマネージャーの初期化
	m_pEnemyManager->Init();

/*ゲーム内のオブジェクト生成*/

	//背景の生成
	CBg* pBg = CApplication::GetBg()->Create(D3DXVECTOR3(1280.0f / 2, 720.0f / 2, 0.0f),
											1280.0f,
											720.0f,
											CTexture::TEXTURE::TEXTERE_BG);

	//壁の生成
	m_pProtect_City = Cprotect_City::Create(D3DXVECTOR3(0.0f, 720.0f / 2, 0.0f), 100.0f, 720.0f);

	//ユーザーインターフェースの生成
	m_pUi = CUi::Create(D3DXVECTOR3(480.0f, 60.0f, 0.0f), 780.0f, 100.0f, CTexture::TEXTURE::TEXTURE_UI);
	
	//スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(423.0f,68.0f,0.0f),0);

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(100.0f, 720.0f / 2, 0.0f), 30.0f, 30.0f);

	//エネミーマネージャーを起動
	m_pEnemyManager->CreateEnemy();

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CGame::Uninit()
{
	//サウンドの停止
	CApplication::GetSound()->Stop();
	
	if (m_pProtect_City != nullptr)
	{//壁のポインタに値に何かしらの値が入っていたら
		m_pProtect_City = nullptr;		//ヌルを代入
	}
	
	if (m_pUi != nullptr)
	{//ユーザーインターフェースのポインタに何かしらの値が入っていたら
		m_pUi->Uninit();				//ユーザーインターフェースの終了
		m_pUi = nullptr;				//ヌルを代入
	}
	
	if (m_pScore != nullptr)
	{//スコアのポインタに値が何かしらの値が入っていたら
		m_pScore = nullptr;				//ヌルを代入
	}
	
	if (m_pPlayer != nullptr)
	{//プレイヤーのポインタに何かしらの値が入っていたら
		m_pPlayer = nullptr;			//ヌルを代入
	}

	if (m_pEnemyManager != nullptr)
	{//レンダラーのポインタに何かしらの値が入っていたら
		m_pEnemyManager->Uninit();		//終了処理
		delete m_pEnemyManager;			//レンダラーポインタを解放
		m_pEnemyManager = nullptr;		//ヌルを代入
	}
}

//--------------------------------------
//更新
//--------------------------------------
void CGame::Update()
{
	if (m_pEnemyManager != nullptr)
	{//エネミーマネージャーのポインタに値が入っていたとき
		m_pEnemyManager->PopEnenmy();				//エネミーを呼び出す
	}
}

//--------------------------------------
//描画
//--------------------------------------
void CGame::Draw()
{
	
}

//---------------------------------------
//プレイヤーの情報取得
//---------------------------------------
CPlayer * CGame::GetPlayer()
{
	return m_pPlayer;
}

//---------------------------------------
// 街の情報取得
//---------------------------------------
Cprotect_City * CGame::GetProtect_City()
{
	return m_pProtect_City;
}

//---------------------------------------
// UIの情報取得
//---------------------------------------
CUi * CGame::GetUi()
{
	return m_pUi;
}

//---------------------------------------
// スコアの情報取得
//---------------------------------------
CScore * CGame::GetScore()
{
	return m_pScore;
}
