//--------------------------------------------------------------------------------------
//
// ゲーム管理
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "application.h"
#include "game.h"
#include "ranking.h"
#include "renderer.h"
#include "object.h"
#include "object2d.h"
#include "object3d.h"
#include "objectx.h"
#include "player.h"
#include "input.h"
#include "score.h"
#include "ui.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "meshifild.h"
#include "ranking.h"
#include "result.h"
#include "enemymanager.h"
#include "bildingmanager.h"
#include "skybox.h"
#include "effect.h"
#include "eatdescription.h"
#include <time.h>

//--------------------------------------
//静的メンバ変数の宣言
//--------------------------------------
CCamera *CGame::m_pCamera;
CPlayer *CGame::m_pPlayer;
CEnemyManager *CGame::m_pEnemyManager;
CUi *CGame::m_pUi;
CScore *CGame::m_pScore;
CLight *CGame::m_pLight;
CMeshField *CGame::m_pMeshifild;
CSkybox *CGame::m_pSkybox;
CBildingManager *CGame::m_pBildingManager;
CEatDescription *CGame::m_pEatDescription;

const float TEST_PLAYER = -300.0f;

//--------------------------------------
//コンストラクタ
//--------------------------------------
CGame::CGame()
{
	m_pEnemyManager = nullptr;
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
/*ゲーム内のオブジェクト生成*/

	//スカイボックスの生成
	m_pSkybox = CSkybox::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//メッシュフィールドの生成
	m_pMeshifild = CMeshField::Create();

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, TEST_PLAYER));

	//エネミーマネージャーの動的確保
	m_pEnemyManager = new CEnemyManager;

	//エネミーマネージャーの初期化
	m_pEnemyManager->Init();

	m_pEatDescription = CEatDescription::Create(D3DXVECTOR3(0.0f, 200.0f, 369.0f),
		D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI / 2),
		400.0f,
		800.0f,
		CObject::OBJTYPE_DESCRIPTION,
		CTexture::TEXTURE_TUTORIAL);

	m_pEatDescription = CEatDescription::Create(D3DXVECTOR3(600.0f, 100.0f, -3069.0f),
		D3DXVECTOR3(0.0f, D3DX_PI, -D3DX_PI / 2),
		200.0f,
		400.0f,
		CObject::OBJTYPE_DESCRIPTION,
		CTexture::TEXTURE_DISCRIPTION);

	m_pEatDescription = CEatDescription::Create(D3DXVECTOR3(1600.0f, 100.0f, -3069.0f),
		D3DXVECTOR3(0.0f, D3DX_PI, -D3DX_PI / 2),
		200.0f,
		400.0f,
		CObject::OBJTYPE_DESCRIPTION,
		CTexture::TEXTURE_ARROW);

	m_pEatDescription = CEatDescription::Create(D3DXVECTOR3(5400.0f, 100.0f, -3069.0f),
		D3DXVECTOR3(0.0f, D3DX_PI /2, -D3DX_PI / 2),
		200.0f,
		400.0f,
		CObject::OBJTYPE_DESCRIPTION,
		CTexture::TEXTURE_ARROW);

	m_pEatDescription = CEatDescription::Create(D3DXVECTOR3(5400.0f, 100.0f, -500.0f),
		D3DXVECTOR3(0.0f, D3DX_PI / 2, -D3DX_PI / 2),
		200.0f,
		400.0f,
		CObject::OBJTYPE_DESCRIPTION,
		CTexture::TEXTURE_DISCRIPTION2);

	m_pEatDescription = CEatDescription::Create(D3DXVECTOR3(5400.0f, 100.0f, -1000.0f),
		D3DXVECTOR3(0.0f, D3DX_PI / 2, -D3DX_PI / 2),
		200.0f,
		400.0f,
		CObject::OBJTYPE_DESCRIPTION,
		CTexture::TEXTURE_ARROW);

	m_pEatDescription = CEatDescription::Create(D3DXVECTOR3(4385.0f, 250.0f, 4300.0f),
		D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI / 2),
		1200.0f,
		1600.0f,
		CObject::OBJTYPE_EAT_DESCRIPTION,
		CTexture::TEXTURE_EAT_ME);

	//ビルディングマネージャーの動的確保
	m_pBildingManager = new CBildingManager;

	//建物の生成
	m_pBildingManager->Init();

	//スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(393.0f, 68.0f, 0.0f), 0);

	//ユーザーインターフェースの生成
	m_pUi = CUi::Create(D3DXVECTOR3(480.0f, 60.0f, 0.0f), 780.0f, 100.0f, CTexture::TEXTURE::TEXTURE_UI);

	//ユーザーインターフェースの生成
	m_pUi = CUi::Create(D3DXVECTOR3(635.0f, 605.0f, 0.0f), 200.0f, 100.0f, CTexture::TEXTURE::TEXTURE_UI_PLAYERDATA);

	//ライトの生成
	m_pLight = CLight::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//カメラの生成
	m_pCamera = CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//サウンドの再生
	CApplication::GetSound()->Play(CSound::SOUND_BGM_TUTORIAL);

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CGame::Uninit()
{
	//サウンドを全停止する
	CApplication::GetSound()->Stop();
	
	if (m_pBildingManager != nullptr)
	{//建物のポインタに何かしらの値が入っていたら
		m_pBildingManager->Uninit();				//終了処理
		delete m_pBildingManager;					//建物マネージャーのポインタを解放
		m_pBildingManager = nullptr;				//ヌルを代入
	}

	if (m_pUi != nullptr)
	{//ユーザーインターフェースのポインタに何かしらの値が入っていたら
		m_pUi = nullptr;						//ヌルを代入
	}

	if (m_pScore != nullptr)
	{//スコアのポインタに何かしらの値が入っていたら
		m_pScore = nullptr;						//ヌルを代入
	}
	
	if (m_pPlayer != nullptr)
	{//プレイヤーのポインタに何かしらの値が入っていたら
		m_pPlayer = nullptr;					//ヌルを代入
	}

	if (m_pEnemyManager != nullptr)
	{//エネミーのポインタに何かしらの値が入っていたら
		m_pEnemyManager->Uninit();				//終了処理
		delete m_pEnemyManager;					//エネミーマネージャーのポインタを解放
		m_pEnemyManager = nullptr;				//ヌルを代入
	}

	if (m_pCamera != nullptr)
	{//カメラのポインタに何かしらの値が入っていたら
		m_pCamera = nullptr;					//ヌルを代入
	}

	if (m_pMeshifild != nullptr)
	{//メッシュフィールドのポインタに何かしらの値が入っていたら
		m_pMeshifild = nullptr;					//ヌルを代入
	}

	if (m_pEatDescription != nullptr)
	{//メッシュフィールドのポインタに何かしらの値が入っていたら
		m_pEatDescription = nullptr;					//ヌルを代入
	}
}

//--------------------------------------
//更新
//--------------------------------------
void CGame::Update()
{
	if (m_pCamera != nullptr)
	{//カメラのポインタに値が入っていたら
		m_pCamera->Update();						//カメラの更新
	}

	if (m_pEnemyManager != nullptr)
	{//カメラのポインタに値が入っていたら
		m_pEnemyManager->Update();					//エネミーマネージャーの更新
	}

	if (m_pBildingManager != nullptr)
	{
		m_pBildingManager->Update();
	}

#ifdef _DEBUG
	// リザルトに移行

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_RSHIFT))
	{//Enterキーが押されたら
		CResult::Set(0);
		CRanking::Set(CScore::Get());
		CApplication::SetMode(CApplication::MODE::MODE_RESULT);
	}

	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_4))
	{
		m_pPlayer->AddLife(1);
		m_pScore->AddLife(1);
	}

	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_3))
	{
		m_pPlayer->SubLife(1);
		m_pScore->HitDamage(1);
		m_pScore->Sub(10);
	}

	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_9))
	{
		m_pCamera->Sequence(10.0f,20.0f);
	}

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_M))
	{
		//エフェクトの設定
		CEffect::Spawn(D3DXVECTOR3(0.0f,100.0f,0.0f),					//位置の設定
								   10,									//エフェクトの最大数
								   5.f,									//ランダムの最大数
								   -5.f,								//ランダムの最小数
								   D3DXCOLOR(1.0f,0.f,0.f,1.0f),			//色の設定
								   30.0f * m_pPlayer->GetScale().y,
								   30.0f * m_pPlayer->GetScale().y);

		m_pPlayer->AddInertia(1.0f);
	}

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_B))
	{
		m_pScore->SetStartTimer(true);
	}

#endif // _DEBUG

}

//--------------------------------------
//描画
//--------------------------------------
void CGame::Draw()
{
}
