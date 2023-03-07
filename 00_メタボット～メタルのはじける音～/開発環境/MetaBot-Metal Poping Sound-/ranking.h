//--------------------------------------------------------------------------------------
//
//ランキング処理
//Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _RANKING_H_
#define _RANKING_H_

#include <d3dx9.h>

//----------------------------------------
//前方宣言
//----------------------------------------
class CBg;
class CScore;
class CObject2D;
class CTexture;
class CUi;

//---------------------------------------------------
//マクロ定義
//---------------------------------------------------
#define MAX_RANKING			(5)
#define COMPARE_RANKING		(4)
#define FILE_NAME			"Data/Text/Ranking.txt"			//読み込むファイル

//----------------------------------------
//クラス宣言
//ランキング
//----------------------------------------
class CRanking
{
public:
	CRanking();												//コンストラクタ
	~CRanking();											//デストラクタ
	HRESULT Init();											//初期化
	void Uninit();											//終了
	void Update();											//更新
	void Draw();											//描画
	static void Set(int nScore);							//ランキングの設定
	int Get();												//ランキングの取得
	void BubbleSort(int nSort);								//ソート処理
	void Save();											//ファイル保存
	void Load();											//ファイル呼び出し
	static CBg* GetBg();									//背景の情報取得
	static CScore* GetScore();								//スコアの情報取得
	static CUi* GetUi();									//ユーザーインターフェースの取得

private:
	static int m_nBox;										//ランキングの値を受け取る変数
	int m_nRankUpdate;										//更新ランクNo.
	int m_nCountState;										//ランキングスコアの状態カウンター
	int m_nScoreState;										//ランキングスコアの状態情報
	int m_SortData[MAX_RANKING];							//ソートしたものを受け取る変数
	static CBg* m_pBg;										//背景のポインター
	static CScore* m_pScore;								//スコアのポインター
	static CUi* m_pUi;										//ユーザーインターフェースのポインター
};

#endif // !_RANKING_H_
