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
//クラス宣言
//スコア、オブジェクトの継承
//----------------------------------------
class CScore : public CObject
{
public:
	CScore();													//コンストラクタ
	~CScore()override;											//デストラクタ
	HRESULT Init()override;										//初期化
	static CScore * Create(D3DXVECTOR3 pos, int nScore);		//クリエイト
	void Uninit()override;										//終了
	void Update()override;										//更新
	void Draw()override;										//描画

/*スコアを計算する関数まとめ*/
	void Set(int nScore);										//スコアの配置
	void Add(int nValue);										//スコアの加算
	void Sub(int nSub);											//スコアの減算
	static int Get() { return m_nScore; }						//スコアの取得

/*プレイヤーのライフを計算する関数まとめ*/
	void SetPlayerLife(int nLife);								//ライフのセット（目に見える値として）
	void AddLife(int nAdd);										//ライフの加算
	static void HitDamage(int nDamage);							//ライフの減算

/*タイマーを制御する関数まとめ*/
	void SetTime(int nTime);									//タイマーの配置
	void MinusTime(int nMinus);									//タイマーの減算
	void SetStartTimer(bool bTime) { m_bTimerStart = bTime; }

/*コンボを制御する関数まとめ*/
	void SetCombo(int nCombo);									//コンボの配置
	void AddCombo(int nAddCombo);								//コンボの加算
	void SubCombo(int nSubCombo);								//コンボの減算

private:
	static CScore* m_pScore;									//スコアのポインター
	static int m_nScore;										//スコアの値
	int m_nTime;												//タイマーの目に見えるテクスチャ制御するための変数
	int m_nTimeCounter;											//タイマーを進める変数
	int m_nCountstartTime;										//タイマーが動くまでの時間を数える変数
	static int m_nLife;											//ライフ制御変数
	D3DXVECTOR3 m_ScorePos;										//スコアの位置
	CObject2D* m_pNumber[7];									//スコアナンバーのポインター
	static CObject2D* m_pLife[2];								//ライフナンバーのポインター
	CObject2D* m_pTime[3];										//タイムナンバーのポインター
	static CObject2D* m_pCombo[4];								//コンボナンバーのポインター
	static int m_nCombo;										//コンボを数える変数
	bool m_bTimerStart;											//タイマーを開始するか否か
	bool m_bSount;
};

#endif // !_SCORE_H_
