//---------------------------------------------------------
//
//ゲーム
//Author::TAKANO
//
//---------------------------------------------------------
#ifndef _GAME_H_
#define _GAME_H_

#include <d3dx9.h>

//----------------------------------------
//前方宣言
//----------------------------------------
class CRenderer;
//class CInput;
class CBg;
class CPlayer;
class CTexture;
class CEnemyManager;
class Cprotect_City;
class CSoundManager;
class CScore;
class CUi;

//----------------------------------------
//クラス定義
//マネージャー
//----------------------------------------
class CGame
{
public:
	CGame();												//コンストラクタ
	~CGame();												//デストラクタ
	HRESULT Init();											//初期化
	void Uninit();											//終了
	void Update();											//更新
	void Draw();											//描画
	static CPlayer* GetPlayer();							//プレイヤー情報の取得
	static Cprotect_City* GetProtect_City();				//街の情報の取得
	static CUi* GetUi();
	static CScore* GetScore();

private:
	static CPlayer* m_pPlayer;											//プレイヤーのポインター
	static CEnemyManager* m_pEnemyManager;								//エネミーのポインター
	static Cprotect_City* m_pProtect_City;								//街のポインター
	static CUi* m_pUi;
	static CScore* m_pScore;
};

#endif // !_GAME_H_
