//--------------------------------------------------------------------------------------
//
// エネミーの管理
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _CENEMYMANAGER_H_
#define _CENEMYMANAGER_H_

//----------------------------------------
//前方宣言
//----------------------------------------
class CEnemy;

//----------------------------------------
//クラス定義
//オブジェクト
//----------------------------------------
class CEnemyManager
{
public:
	enum ENEMY_PART
	{
		PART_NONE = 0,
		PART_WAVETYPE_ONE,
		PART_WAVETYPE_TWO,
		PART_WAVETYPE_TREE,
		PART_BOSS,
		PART_BOSS_SECOND,
		PART_BOSS_THIRD,
		PART_MAX
	};

	CEnemyManager();						//コンストラクタ
	~CEnemyManager();						//デストラクタ
	void Init();							//初期化
	void Uninit();							//終了	
	void CreateEnemy();						//エネミーの生成
	void PopEnenmy();						//エネミーの出現
	void SwitchPart(ENEMY_PART part);		//パート切替
	void DestroyEnemy();					//エネミーの削除
	static CEnemy* GetEnemy();				//エネミーの情報の取得

private:
	static CEnemy* m_pEnemy;				//エネミーのポインタ
	int m_nPopCount;						//エネミーの出現カウンター
	int m_CollPop;							//エネミーがどれだけ呼ばれたかのカウンター
	int m_BossCount;
	int m_nCount;
	bool m_UseSound;
	bool m_BossColl;
	CEnemyManager::ENEMY_PART m_Part;
};

#endif // !_CENEMYMANAGER_H_