//----------------------------------------------------------------------------------------
//
//ランキング
//Author::TAKANO
//
//----------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "application.h"
#include "ranking.h"
#include "bg.h"
#include "score.h"
#include "input.h"
#include "sound.h"
#include "ui.h"
#include <stdio.h>

//--------------------------------------
//マクロ定義
//--------------------------------------
#define RANK_H				(80)					//縦幅
#define RANK_WD				(50)					//横幅

//--------------------------------------
//グローバル変数
//--------------------------------------
//CBg *CRanking::m_pBg;
CScore *CRanking::m_pScore;
CTexture *CRanking::m_pTexture;
CUi *CRanking::m_pUi;
int CRanking::m_nBox;

//--------------------------------------
//コンストラクタ
//--------------------------------------
CRanking::CRanking()
{
	for (int i = 0; i < MAX_RANKING; i++)
	{
		m_SortData[i] = {};
	}
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CRanking::~CRanking()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CRanking::Init()
{
	Load();
	BubbleSort(m_nBox);
	Save();

	CBg* pBg = CApplication::GetBg()->Create(D3DXVECTOR3(1280.0f / 2, 720.0f / 2, 0.0f),
											1280.0f,
											720.0f,
											CTexture::TEXTURE::TEXTURE_RANKING_BG);

	m_pUi = CUi::Create(D3DXVECTOR3(360.0f, 383.0f, 0.0f), 80.0f, 600.0f, CTexture::TEXTURE::TEXTURE_RANKING);

	for (int i = 0; i < MAX_RANKING; i++)
	{
		m_pScore = CScore::Create(D3DXVECTOR3(620.0f + 40.0f * i, 137.0f + 123.0f * i, 0.0f), m_SortData[i]);
	}
	
	//サウンドの再生
	CApplication::GetSound()->Play(CSound::SOUND_BGM_RANKING);

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CRanking::Uninit()
{
	//サウンドの停止
	CApplication::GetSound()->Stop(CSound::SOUND_BGM_RANKING);
}

//--------------------------------------
//更新
//--------------------------------------
void CRanking::Update()
{
	//決定キー(ENTER)が押されたかどうか
	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_RETURN))
	{
		CApplication::SetMode(CApplication::MODE::MODE_TITLE);
	}
}

//--------------------------------------
//描画
//--------------------------------------
void CRanking::Draw()
{
	
}

//--------------------------------------
//ランキングの値設定
//--------------------------------------
void CRanking::Set(int nScore)
{
	m_nBox = nScore;
}

//--------------------------------------
//ランキングの情報取得
//--------------------------------------
int CRanking::Get()
{
	return m_nBox;
}

//--------------------------------------
//バブルソート(値入れ替え)
//--------------------------------------
void CRanking::BubbleSort(int nSort)
{
	if (nSort >= m_SortData[COMPARE_RANKING])
	{//受け取った値がランキングの値の一番下より大きかったら
		m_SortData[COMPARE_RANKING] = nSort;										//一番下の値に代入する
	}
	for (int nInData = 0; nInData < COMPARE_RANKING; nInData++)						//比較する方
	{
		for (int nChange = nInData + 1; nChange < MAX_RANKING; nChange++)			//比較される方
		{
			if (m_SortData[nInData] < m_SortData[nChange])
			{
				int nBox = m_SortData[nInData];
				m_SortData[nInData] = m_SortData[nChange];
				m_SortData[nChange] = nBox;
			}
		}
	}
}

//--------------------------------------
//外部ファイルに保存
//--------------------------------------
void CRanking::Save()
{
	//ファイルに書き込む
	FILE *pFile;        //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(FILE_NAME, "w");

	if (pFile != nullptr)
	{//ファイルが開けたとき
	 //ファイルに数値を書き出す
		for (int i = 0; i < MAX_RANKING; i++)
		{
			fprintf(pFile, "%d\n", CRanking::m_SortData[i]);
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	//else
	//{//ファイルが開けなかったとき
	//	printf("~~~ファイルが開けなかった！~~~");
	//}
}

//--------------------------------------
//外部ファイルを開く
//--------------------------------------
void CRanking::Load()
{
	FILE *pFile;        //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(FILE_NAME, "r");

	if (pFile != nullptr)
	{//ファイルが開けたとき
	 //ファイルに数値を書き出す
		for (int i = 0; i < MAX_RANKING; i++)
		{
			fscanf(pFile, "%d", &CRanking::m_SortData[i]);
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	//else
	//{//ファイルが開けなかったとき
	//	printf("~~~ファイルが開けなかった！~~~");
	//}
}

//--------------------------------------
//背景の情報取得
//--------------------------------------
CScore * CRanking::GetScore()
{
	return m_pScore;
}

CUi * CRanking::GetUi()
{
	return m_pUi;
}

//--------------------------------------
//テクスチャの情報取得
//--------------------------------------
CTexture * CRanking::GetTexture()
{
	return m_pTexture;
}
