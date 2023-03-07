//------------------------------------------------------------
//
//スコア表示
//Author::TAKANO
//
//------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include"score.h"
#include "game.h"
#include"object.h"
#include "object2d.h"
#include "application.h"
#include "result.h"
#include "ranking.h"
#include "input.h"
#include "renderer.h"
#include "player.h"
#include "sound.h"

//--------------------------------------
//定数定義
//--------------------------------------
const int MAX_SCORE = 7;

//--------------------------------------
//静的メンバ変数の宣言
//--------------------------------------
CScore* CScore::m_pScore;
CObject2D *CScore::m_pLife[2];
int CScore::m_nScore;
int CScore::m_nLife;
CObject2D *CScore::m_pCombo[4];
int CScore::m_nCombo;

//--------------------------------------
//コンストラクタ
//--------------------------------------
CScore::CScore()
{
	m_ScorePos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_nScore = 0;
	m_nTime = 0;
	m_nTimeCounter = 0;
	m_nCountstartTime = 0;
	m_bTimerStart = false;
	m_bSount = true;
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CScore::~CScore()
{
}

//--------------------------------------
//クリエイト
//--------------------------------------
CScore *CScore::Create(D3DXVECTOR3 pos,int nScore)
{
	m_pScore = new CScore;

	m_pScore->m_ScorePos = pos;
	m_pScore->Init();
	m_pScore->SetObjectType(OBJTYPE_SCORE);
	m_pScore->Set(nScore);
	
	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		m_pScore->m_nLife = CGame::GetPlayer()->GetPlayerLife();
		m_pScore->SetPlayerLife(m_nLife);
		m_pScore->SetTime(90);
		m_pScore->m_nCombo = CGame::GetPlayer()->GetCombo();
		m_pScore->SetCombo(m_nCombo);
	}	

	return m_pScore;
}

//--------------------------------------
//スコアの初期化処理
//--------------------------------------
HRESULT CScore::Init()
{
	m_nScore = 0;

	m_bTimerStart = false;
	
	for (int i = 0; i < MAX_SCORE; i++)
	{
		m_pNumber[i] = CObject2D::Create(D3DXVECTOR3(m_ScorePos.x + (30 * i), m_ScorePos.y,0.0f),30.0f,60.0f);
		m_pNumber[i]->SetTexture(CTexture::TEXTURE::TEXTURE_SCORE);
		m_pNumber[i]->SetObjectType(OBJTYPE_SCORE);
	}

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		for (int i = 0; i < 2; i++)
		{
			m_pLife[i] = CObject2D::Create(D3DXVECTOR3(205.0f + (30 * i), m_ScorePos.y, 0.0f), 30.0f, 60.0f);
			m_pLife[i]->SetTexture(CTexture::TEXTURE::TEXTURE_SCORE);
			m_pLife[i]->SetObjectType(OBJTYPE_SCORE);
		}

		for (int j = 0; j < 3; j++)
		{
			m_pTime[j] = CObject2D::Create(D3DXVECTOR3(697.0f + (30 * j), m_ScorePos.y, 0.0f), 30.0f, 60.0f);
			m_pTime[j]->SetTexture(CTexture::TEXTURE::TEXTURE_SCORE);
			m_pTime[j]->SetObjectType(OBJTYPE_SCORE);
		}

		for (int k = 0; k < 4; k++)
		{
			m_pCombo[k] = CObject2D::Create(D3DXVECTOR3(590.0f + (30 * k), 620.0f, 0.0f), 30.0f, 60.0f);
			m_pCombo[k]->SetTexture(CTexture::TEXTURE::TEXTURE_SCORE);
			m_pCombo[k]->SetObjectType(OBJTYPE_SCORE);
		}
	}

	return S_OK;
}

//--------------------------------------
//スコアの終了処理
//--------------------------------------
void CScore::Uninit()
{
	if (m_pScore != nullptr)
	{
		delete m_pScore;
		m_pScore = nullptr;
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_pCombo[i] != nullptr)
		{
			delete[] m_pCombo;
			m_pCombo[i] = nullptr;
		}
	}
}

//--------------------------------------
//スコアのの更新処理
//--------------------------------------
void CScore::Update()
{
	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		if (m_bTimerStart)
		{
			m_nTimeCounter++;

			if (m_nTimeCounter % 60 == 0)
			{
				MinusTime(1);
			}
		}

		if (m_nTime == 15 &&
			m_bSount)
		{
			CApplication::GetSound()->Play(CSound::SOUND_SE_WARNING);
			m_bSount = false;
		}
	}

#ifdef _DEBUG
	// リザルトに移行

	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_7))
	{//Uキーが押されたら
		Add(100);
	}

#endif // _DEBUG
}

//--------------------------------------
//スコアの描画処理
//--------------------------------------
void CScore::Draw()
{
	
}

//--------------------------------------
//スコアの設定処理
//--------------------------------------
void CScore::Set(int nScore)
{
	int aPosTexU[MAX_SCORE];

	m_nScore = nScore;

	aPosTexU[0] = m_nScore % 10000000 / 1000000;
	aPosTexU[1] = m_nScore % 1000000 / 100000;
	aPosTexU[2] = m_nScore % 100000 / 10000;
	aPosTexU[3] = m_nScore % 10000 / 1000;
	aPosTexU[4] = m_nScore % 1000 / 100;
	aPosTexU[5] = m_nScore % 100 / 10;
	aPosTexU[6] = m_nScore % 10;

	for (int i = 0; i < MAX_SCORE; i++)
	{
		m_pNumber[i]->PattarnTexture(aPosTexU[i],10);
	}

}

//--------------------------------------
//スコアの加算処理
//--------------------------------------
void CScore::Add(int nValue)
{
	int aPosTexU[MAX_SCORE];

	m_nScore += nValue;

	aPosTexU[0] = m_nScore % 10000000 / 1000000;
	aPosTexU[1] = m_nScore % 1000000 / 100000;
	aPosTexU[2] = m_nScore % 100000 / 10000;
	aPosTexU[3] = m_nScore % 10000 / 1000;
	aPosTexU[4] = m_nScore % 1000 / 100;
	aPosTexU[5] = m_nScore % 100 / 10;
	aPosTexU[6] = m_nScore % 10;

	for (int i = 0; i < MAX_SCORE; i++)
	{
		m_pNumber[i]->PattarnTexture(aPosTexU[i], 10);
	}
}

//--------------------------------------
//スコアの減算処理
//--------------------------------------
void CScore::Sub(int nSub)
{
	int aPosTexU[MAX_SCORE];

	m_nScore -= nSub;

	aPosTexU[0] = m_nScore % 10000000 / 1000000;
	aPosTexU[1] = m_nScore % 1000000 / 100000;
	aPosTexU[2] = m_nScore % 100000 / 10000;
	aPosTexU[3] = m_nScore % 10000 / 1000;
	aPosTexU[4] = m_nScore % 1000 / 100;
	aPosTexU[5] = m_nScore % 100 / 10;
	aPosTexU[6] = m_nScore % 10;

	for (int i = 0; i < MAX_SCORE; i++)
	{
		m_pNumber[i]->PattarnTexture(aPosTexU[i], 10);
	}
}

//--------------------------------------
//プレイヤーのライフの設定処理
//--------------------------------------
void CScore::SetPlayerLife(int nLife)
{
	int aPosTexU[2];

	m_nLife = nLife;

	aPosTexU[0] = m_nLife % 100 / 10;
	aPosTexU[1] = m_nLife % 10;

	for (int i = 0; i < 2; i++)
	{
		m_pLife[i]->PattarnTexture(aPosTexU[i], 10);
	}
}

//--------------------------------------
//プレイヤーのライフの設定処理
//--------------------------------------
void CScore::AddLife(int nAdd)
{
	if (m_nLife < 99)
	{
		int aPosTexU[2];
		m_nLife += nAdd;

		aPosTexU[0] = m_nLife % 100 / 10;
		aPosTexU[1] = m_nLife % 10;

		for (int i = 0; i < 2; i++)
		{
			m_pLife[i]->PattarnTexture(aPosTexU[i], 10);
		}
	}
}

//--------------------------------------
//プレイヤーのダメージ処理
//--------------------------------------
void CScore::HitDamage(int nDamage)
{
	if (m_nLife > 0)
	{
		int aPosTexU[2];

		m_nLife -= nDamage;

		aPosTexU[0] = m_nLife % 100 / 10;
		aPosTexU[1] = m_nLife % 10;

		for (int i = 0; i < 2; i++)
		{
			m_pLife[i]->PattarnTexture(aPosTexU[i], 10);
		}
	}
}

//--------------------------------------
//時間の設定処理
//--------------------------------------
void CScore::SetTime(int nTime)
{
	int aPosTexU[3];

	m_nTime = nTime;

	aPosTexU[0] = m_nTime % 1000 / 100;
	aPosTexU[1] = m_nTime % 100 / 10;
	aPosTexU[2] = m_nTime % 10;

	for (int i = 0; i < 3; i++)
	{
		m_pTime[i]->PattarnTexture(aPosTexU[i], 10);
	}
}

//--------------------------------------
//時間の減算処理
//--------------------------------------
void CScore::MinusTime(int nMinus)
{
	int aPosTexU[3];

	m_nTime -= nMinus;

	aPosTexU[0] = m_nTime % 1000 / 100;
	aPosTexU[1] = m_nTime % 100 / 10;
	aPosTexU[2] = m_nTime % 10;

	for (int i = 0; i < 3; i++)
	{
		m_pTime[i]->PattarnTexture(aPosTexU[i], 10);
	}

	if (m_nTime <= 0)
	{
		CResult::Set(0);
		CRanking::Set(CScore::Get());
		CApplication::SetMode(CApplication::MODE::MODE_RESULT);
	}
}

//--------------------------------------
//コンボの設定
//--------------------------------------
void CScore::SetCombo(int nCombo)
{
	int aPosTexU[4];

	m_nCombo = nCombo;

	aPosTexU[0] = m_nScore % 10000 / 1000;
	aPosTexU[1] = m_nScore % 1000 / 100;
	aPosTexU[2] = m_nScore % 100 / 10;
	aPosTexU[3] = m_nScore % 10;

	for (int i = 0; i < 4; i++)
	{
		m_pCombo[i]->PattarnTexture(aPosTexU[i], 10);
	}
}

//--------------------------------------
//コンボの加算
//--------------------------------------
void CScore::AddCombo(int nAddCombo)
{
	int aPosTexU[4];

	m_nCombo = nAddCombo;

	aPosTexU[0] = m_nCombo % 10000 / 1000;
	aPosTexU[1] = m_nCombo % 1000 / 100;
	aPosTexU[2] = m_nCombo % 100 / 10;
	aPosTexU[3] = m_nCombo % 10;

	for (int i = 0; i < 4; i++)
	{
		m_pCombo[i]->PattarnTexture(aPosTexU[i], 10);
	}
}

//--------------------------------------
//コンボの減算
//--------------------------------------
void CScore::SubCombo(int nSubCombo)
{
	int aPosTexU[4];

	m_nCombo = nSubCombo;

	aPosTexU[0] = m_nCombo % 10000 / 1000;
	aPosTexU[1] = m_nCombo % 1000 / 100;
	aPosTexU[2] = m_nCombo % 100 / 10;
	aPosTexU[3] = m_nCombo % 10;

	for (int i = 0; i < 4; i++)
	{
		m_pCombo[i]->PattarnTexture(aPosTexU[i], 10);
	}
}
