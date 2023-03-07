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
#include"object.h"
#include "object2d.h"
#include "protect_city.h"
#include "application.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "enemy.h"
#include "input.h"
#include "renderer.h"

//--------------------------------------
//静的メンバ変数の定義
//--------------------------------------
int CScore::m_nScore;
int CScore::m_nWallLife;
CObject2D *CScore::m_pWoll_Life[LIFE_NUMBER];

//--------------------------------------
//コンストラクタ(デフォ)
//--------------------------------------
CScore::CScore()
{
	m_ScorePos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_nScore = 0;
	m_nTime = 0;
	m_nTimeCounter = 0;
	m_nTimeStartCounter = 0;
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
	CScore *m_pScore = new CScore;

	m_pScore->m_ScorePos = pos;
	m_pScore->Init();
	m_pScore->SetObjectType(OBJTYPE_SCORE);
	m_pScore->Set(nScore);
	
	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		m_pScore->m_nWallLife = Cprotect_City::GetLife();
		m_pScore->SetWallLife(m_nWallLife);
		m_pScore->SetTime(90);
	}	

	return m_pScore;
}

//--------------------------------------
//スコアの初期化処理
//--------------------------------------
HRESULT CScore::Init()
{
	m_nScore = 0;
	
	for (int i = 0; i < SCORE_NUMBER; i++)
	{
		m_pNumber[i] = CObject2D::Create(D3DXVECTOR3(m_ScorePos.x + (30 * i), m_ScorePos.y,0.0f),30.0f,60.0f);
		m_pNumber[i]->SetTexture(CTexture::TEXTURE::TEXTURE_SCORE);
		m_pNumber[i]->SetObjectType(OBJTYPE_SCORE);
	}

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		for (int i = 0; i < LIFE_NUMBER; i++)
		{
			m_pWoll_Life[i] = CObject2D::Create(D3DXVECTOR3(205.0f + (30 * i), m_ScorePos.y, 0.0f), 30.0f, 60.0f);
			m_pWoll_Life[i]->SetTexture(CTexture::TEXTURE::TEXTURE_SCORE);
			m_pWoll_Life[i]->SetObjectType(OBJTYPE_SCORE);
		}

		for (int k = 0; k < TIME_NUMBER; k++)
		{
			m_pTime[k] = CObject2D::Create(D3DXVECTOR3(697.0f + (30 * k), m_ScorePos.y, 0.0f), 30.0f, 60.0f);
			m_pTime[k]->SetTexture(CTexture::TEXTURE::TEXTURE_SCORE);
			m_pTime[k]->SetObjectType(OBJTYPE_SCORE);
		}
	}

	return S_OK;
}

//--------------------------------------
//スコアの終了処理
//--------------------------------------
void CScore::Uninit()
{
	
}

//--------------------------------------
//スコアのの更新処理
//--------------------------------------
void CScore::Update()
{
	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		m_nTimeStartCounter++;

		if (m_nTimeStartCounter >= 1800)
		{
			m_nTimeCounter++;

			if (m_nTimeCounter % 60 == 0)
			{
				MinusTime(1);
			}
		}
	}

#ifdef _DEBUG
	// リザルトに移行

	if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_U))
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
	int aPosTexU[SCORE_NUMBER];

	m_nScore = nScore;

	aPosTexU[0] = m_nScore % 100000 / 10000;
	aPosTexU[1] = m_nScore % 10000 / 1000;
	aPosTexU[2] = m_nScore % 1000 / 100;
	aPosTexU[3] = m_nScore % 100 / 10;
	aPosTexU[4] = m_nScore % 10;

	for (int i = 0; i < SCORE_NUMBER; i++)
	{
		m_pNumber[i]->PattarnTexture(aPosTexU[i],10);
	}

}

//--------------------------------------
//スコアの加算処理
//--------------------------------------
void CScore::Add(int nValue)
{
	int aPosTexU[SCORE_NUMBER];

	m_nScore += nValue;

	aPosTexU[0] = m_nScore % 100000 / 10000;
	aPosTexU[1] = m_nScore % 10000 / 1000;
	aPosTexU[2] = m_nScore % 1000 / 100;
	aPosTexU[3] = m_nScore % 100 / 10;
	aPosTexU[4] = m_nScore % 10;

	for (int i = 0; i < SCORE_NUMBER; i++)
	{
		m_pNumber[i]->PattarnTexture(aPosTexU[i], 10);
	}
}

//--------------------------------------
//壁のライフの設定処理
//--------------------------------------
void CScore::SetWallLife(int nLife)
{
	int aPosTexU[LIFE_NUMBER];

	m_nWallLife = nLife;

	aPosTexU[0] = m_nWallLife % 100 / 10;
	aPosTexU[1] = m_nWallLife % 10;

	for (int i = 0; i < LIFE_NUMBER; i++)
	{
		m_pWoll_Life[i]->PattarnTexture(aPosTexU[i], 10);
	}
}

//--------------------------------------
//スコアの設定処理
//--------------------------------------
void CScore::HitDamage(int nDamage)
{
	int aPosTexU[LIFE_NUMBER];

	m_nWallLife -= nDamage;

	aPosTexU[0] = m_nWallLife % 100 / 10;
	aPosTexU[1] = m_nWallLife % 10;

	for (int i = 0; i < LIFE_NUMBER; i++)
	{
		m_pWoll_Life[i]->PattarnTexture(aPosTexU[i], 10);
	}
}

//--------------------------------------
//時間の設定処理
//--------------------------------------
void CScore::SetTime(int nTime)
{
	int aPosTexU[TIME_NUMBER];

	m_nTime = nTime;

	aPosTexU[0] = m_nTime % 1000 / 100;
	aPosTexU[1] = m_nTime % 100 / 10;
	aPosTexU[2] = m_nTime % 10;

	for (int i = 0; i < TIME_NUMBER; i++)
	{
		m_pTime[i]->PattarnTexture(aPosTexU[i], 10);
	}
}

//--------------------------------------
//時間の減算処理
//--------------------------------------
void CScore::MinusTime(int nMinus)
{
	int aPosTexU[TIME_NUMBER];

	m_nTime -= nMinus;

	aPosTexU[0] = m_nTime % 1000 / 100;
	aPosTexU[1] = m_nTime % 100 / 10;
	aPosTexU[2] = m_nTime % 10;

	for (int i = 0; i < TIME_NUMBER; i++)
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
//スコアの情報取得
//--------------------------------------
int CScore::Get()
{
	return m_nScore;
}
