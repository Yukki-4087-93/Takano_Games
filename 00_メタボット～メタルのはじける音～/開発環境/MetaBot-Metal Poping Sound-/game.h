//---------------------------------------------------------
//
// ゲーム管理
// Author::TAKANO
//
//---------------------------------------------------------
#ifndef _GAME_H_
#define _GAME_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include <d3dx9.h>

//----------------------------------------
//前方宣言
//----------------------------------------
class CCamera;
class CLight;
class CPlayer;
class CMeshField;
class CSkybox;
class CScore;
class CUi;
class CEnemyManager;
class CBildingManager;
class CEatDescription;

//----------------------------------------
//クラス定義
//ゲームシーンを総括するクラス
//----------------------------------------
class CGame
{
public:
/*関数群*/
/*一般関数*/
	CGame();																//コンストラクタ
	~CGame();																//デストラクタ
	HRESULT Init();															//初期化
	void Uninit();															//終了
	void Update();															//更新
	void Draw();															//描画

/*ゲームで関数する関数*/
public:
	static CCamera* GetCamera()	{ return m_pCamera; }							//カメラの情報取得
	static CLight* GetLight() { return m_pLight; }								//ライトの情報取得
	static CPlayer* GetPlayer() { return m_pPlayer; }							//プレイヤーの情報の取得
	static CMeshField* GetMeshifild() { return m_pMeshifild; }					//メッシュフィールドの情報取得
	static CSkybox* GetSkybox() { return m_pSkybox; }							//スカイボックスの情報取得
	static CScore* GetScore() { return m_pScore; }								//スコアの情報取得
	static CUi* GetUi() { return m_pUi; }										//ユーザーインターフェースの情報取得
	static CEnemyManager* GetEnemyManager() { return m_pEnemyManager; }			//エネミーマネージャーの情報取得
	static CBildingManager* GetBildingManager() { return m_pBildingManager; }	//建物マネージャーの情報取得
	static CEatDescription* GetEatDescription() { return m_pEatDescription; }	//お品書きの情報取得

/*メンバ変数群*/
/*ゲームで管理する変数*/
private:
	static CCamera* m_pCamera;											//カメラのポインター
	static CLight* m_pLight;											//ライトのポインター
	static CPlayer* m_pPlayer;											//プレイヤーのポインター
	static CMeshField* m_pMeshifild;									//メッシュフィールドのポインター
	static CSkybox* m_pSkybox;											//スカイボックスのポインター
	static CScore* m_pScore;											//スコアのポインター
	static CUi* m_pUi;													//ユーザーインターフェースのポインター
	static CEnemyManager* m_pEnemyManager;								//エネミージャーマネのポインター
	static CBildingManager* m_pBildingManager;							//建物マーネジャのポインター
	static CEatDescription* m_pEatDescription;							//お品書きのポインター
};

#endif // !_GAME_H_
