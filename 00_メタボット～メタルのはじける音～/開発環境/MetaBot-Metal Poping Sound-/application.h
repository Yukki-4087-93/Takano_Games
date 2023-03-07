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
class CCamera;
class CTexture;
class CInput;
class CSound;
class CObject;
class CObject2D;
class CObject3D;
class CObjectX;
class CTitle;
class CGame;
class CResult;
class CRanking;

//----------------------------------------
//クラス定義
//マネージャー
//----------------------------------------
class CApplication
{
public:
/*モードを制御する列挙*/
	enum MODE
	{
		MODE_NONE = -1,													//モードを使用していない状態
		MODE_TITLE = 0,													//タイトルモード
		MODE_TUTORE,													//チュートリアル
		MODE_GAME,														//ゲームモード
		MODE_RESULT,													//リザルトモード
		MODE_RANKING,													//ランキングモード
		MODE_MAX
	};

/*公開メンバ関数*/
	CApplication();														//コンストラクタ
	~CApplication();													//デストラクタ
	HRESULT Init(HINSTANCE hInctance, HWND hWnd, bool bWindow);			//初期化
	void Uninit();														//終了
	void Update();														//更新
	void Draw();														//描画

/*非公開メンバ関数*/
private:
	void ReleseDustCheck();												//解放漏れをしていないか確認用関数

/*公開静的メンバ関数*/
public:
	static CRenderer* GetRenderer() { return m_aRenderer; }				//描画の情報取得
	static CInput* GetKyeboard() { return m_pInputKeyboard; }			//キーボードの情報取得
	static CTexture* GetTexture() { return m_pTexture; }				//テクスチャの情報取得
	static CSound* GetSound() { return m_pSound; }						//サウンドの情報取得
	static void SetMode(MODE mode);	
	static MODE GetMode() { return m_mode; }
	static void UpdateMode(MODE mode);
	
	static CTitle* GetTitle() { return m_pTitle; }						//タイトルの情報取得
	static CGame* GetGame() { return m_pGame; }							//ゲームの情報取得
	static CResult* GetResult() { return m_pResult; }					//リザルトの情報取得
	static CRanking* GetRanking() { return m_pRanking; }				//ランキングの情報取得

/*一般メンバ変数*/
private:
	CObject3D* m_pObj3D;												//オブジェクト3Dのポインター
	CObject2D* m_pObj2D;												//オブジェクト2Dのポインター

/*静的メンバ変数*/
private:
	static MODE m_mode;
	static CRenderer* m_aRenderer;										//レンダラーのポインター
	static CInput* m_pInputKeyboard;									//キーボードのポインター
	static CTexture* m_pTexture;										//テクスチャのポインター																		//static MODE m_mode;
	static CSound* m_pSound;											//サウンドのポインター

	static CTitle* m_pTitle;											//タイトルのポインター
	static CGame* m_pGame;												//ゲームのポインター
	static CResult* m_pResult;											//リザルトのポインター
	static CRanking* m_pRanking;										//ランキングのポインター
};

#endif // !_APPLICATION_H_
