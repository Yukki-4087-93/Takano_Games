//--------------------------------------------------------------------------------------
//
// マネージャーアプリケーション
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include <d3dx9.h>

//----------------------------------------
//前方宣言
//----------------------------------------
class CRenderer;
class CTexture;
class CTitle;
class CGame;
class CResult;
class CRanking;
class CInput;
class CSound;
class CBg;

//----------------------------------------
//クラス定義
//マネージャー
//----------------------------------------
class CApplication
{
public:
	//モード情報
	enum MODE
	{
		MODE_TITLE = 0,
		MODE_GAME,
		MODE_RESULT,
		MODE_RANKING,
		MODE_MAX
	};

/*メンバ関数*/
	CApplication();														//コンストラクタ
	~CApplication();													//デストラクタ
	HRESULT Init(HINSTANCE hInctance, HWND hWnd, bool bWindow);			//初期化
	void Uninit();														//終了
	void Update();														//更新
	void Draw();														//描画

/*公開しないメンバ関数*/
private:
	void ReleseDustCheck();												//解放漏れをしていないか確認用関数

/*静的メンバ関数*/
public:
	static CRenderer* GetRenderer() { return m_aRenderer; }				//描画情報の取得
	static CInput* GetKyeboard() { return m_pInputKyeboard; }			//キーボード情報の取得
	static CTexture* GetTexture() { return m_pTexture; }				//テクスチャ情報の取得
	static CTitle* GetTitle() { return m_pTitle; }						//タイトル情報の取得
	static CGame* GetGame() { return m_pGame; }							//ゲーム情報の取得
	static CResult* GetResult() { return m_pResult; }					//リザルト情報の取得
	static CRanking* GetRanking() { return m_pRanking; }				//ランキング情報の取得
	static void SetMode(MODE mode);										//モードの設定
	static CApplication::MODE GetMode() { return m_mode; }				//モード情報の取得
	static CSound* GetSound() { return m_pSound; }						//サウンド情報の取得
	static CBg* GetBg() { return m_pBg; }								//背景情報の取得

/*静的メンバ変数*/
private:
	static CRenderer* m_aRenderer;										//レンダラーのポインター
	static CInput* m_pInputKyeboard;									//キーボードのポインター
	static CTexture* m_pTexture;										//テクスチャのポインター
	static CTitle* m_pTitle;											//タイトルのポインター
	static CGame* m_pGame;												//ゲームのポインター
	static CResult* m_pResult;											//リザルトのポインター
	static CRanking* m_pRanking;										//ランキングのポインター
	static MODE m_mode;													//モードのポインター
	static CSound* m_pSound;											//サウンドのポインター
	static CBg* m_pBg;													//背景のポインター
};

#endif // !_APPLICATION_H_
