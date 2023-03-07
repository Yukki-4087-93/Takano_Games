//---------------------------------------------------------
//
//スコア表示
//Author::TAKANO
//
//---------------------------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "object.h"
#include "object2d.h"
#include "renderer.h"
#include "texture.h"

//----------------------------------------
//定数定義
//----------------------------------------
const int SCORE_NUMBER = 5;
const int LIFE_NUMBER = 2;
const int TIME_NUMBER = 3;

//----------------------------------------
//クラス宣言
//スコア
//----------------------------------------
class CScore : public CObject
{
public:
	CScore();
	~CScore()override;
	HRESULT Init()override;
	static CScore * Create(D3DXVECTOR3 pos, int nScore);					//クリエイト
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void Setpos(D3DXVECTOR3 pos) override {}			//位置の設定

	void Set(int nScore);
	void Add(int nValue);

	void SetWallLife(int nLife);
	static void HitDamage(int nDamage);

	void SetTime(int nTime);
	void MinusTime(int nMinus);

	static int Get();

private:
	static int m_nScore;														//スコアの値
	int m_nTime;
	int m_nTimeCounter;
	int m_nTimeStartCounter;
	static int m_nWallLife;
	D3DXVECTOR3 m_ScorePos;
	CObject2D* m_pNumber[SCORE_NUMBER];
	static CObject2D* m_pWoll_Life[LIFE_NUMBER];
	CObject2D* m_pTime[TIME_NUMBER];
};

#endif // !_SCORE_H_
