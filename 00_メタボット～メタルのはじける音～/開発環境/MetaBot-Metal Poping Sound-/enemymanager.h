//---------------------------------------------------------
//
// エネミー管理
// Author::TAKANO
//
//---------------------------------------------------------
#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include <d3dx9.h>

//----------------------------------------
//前方宣言
//----------------------------------------
class CEnemy;

//----------------------------------------
//クラス定義
//敵出現を総括するクラス
//----------------------------------------
class CEnemyManager
{
public:
/*一般関数*/
	CEnemyManager();													//コンストラクタ
	~CEnemyManager();													//デストラクタ
	HRESULT Init();														//初期化
	void Uninit();														//終了
	void Update();														//更新
	void Draw();														//描画

/*エネミーを管理する関数*/
public:
	static CEnemy* GetEnemy() { return m_pEnemy; }						//エネミーの情報取得

/*マネージャークラスで非公開に持っている関数*/
private:
	void SpawnTutoral();												//チュートリアル用のエネミーを出現させる
	void SetSpawnPoint();												//エネミーの出現位置を設定する関数
	void SetAnotherPoint();												//エネミーの出現位置を設定する関数
	void SetBigSpawnPoint();											//巨躯を駆るエネミーの出現位置を設定する関数
	void SetChangeSpawn();												//エネミーの出現するときのタイプを設定する関数
	void SpawnNormal(float fPosX, float fPosZ);							//通常のエネミーの出現を設定する関数
	void SpawnAttacker(float fPosX, float fPosZ);						//火力マシマシのエネミーの出現を設定する関数
	void SpawnSpeeder(float fPosX, float fPosZ);						//速度マシマシのエネミーの出現を設定する関数
	void SpawnDoctor(float fPosX, float fPosZ);							//回復マンの出現を設定する関数
	void SpawnMulti(float fPosX, float fPosZ);							//複合能力持ちの出現を設定する関数
	void SpawnBoss(float fPosX, float fPosZ,float fsize);							//ボスの出現を設定する関数

/*エネミージャーマネで管理するメンバ変数*/
private:
	static CEnemy* m_pEnemy;											//エネミーのポインター
	int m_nPopCounter;													//エネミーの出現を管理する変数
	int m_nRand_Spawn_Point;											//エネミーの出現位置を決定する変数
	int m_nScaleCounter;												//スケールのサイズを設定するカウンター変数
	int m_nBig_Spawn_Point;												//サイズの大きいものが出現する位置を設定する変数
	int m_nNumber;														//エネミー番号を設定する変数
	int m_nSpawnType;
	int m_nChangeSpawn;
};

#endif // !_ENEMYMANAGER_H_
