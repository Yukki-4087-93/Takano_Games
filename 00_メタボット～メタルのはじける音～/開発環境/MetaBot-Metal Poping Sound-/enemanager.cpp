//--------------------------------------------------------------------------------------
//
// エネミーの管理
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "application.h"
#include "input.h"
#include "enemymanager.h"
#include "enemy.h"
#include "sound.h"
#include <time.h>

//--------------------------------------
//定数定義
//--------------------------------------
const int DEFAULT_LIFE = 1;
const int DEFAULT_BOSSLIFE = 5;
const float HIGHT_POSITION = 0.0f;
const float SPEEDER_INTEGER = 7.2f;
const float TUTORIAL_POSISION_X_ONE = 2516.0f;
const float TUTORIAL_POSISION_Z_ONE = -2838.0f;
const float TUTORIAL_POSISION_X_TWO = 4786.0f;
const float TUTORIAL_POSISION_Z_TWO = -3641.0f;
const float TUTORIAL_POSISION_X_THREE = 4973.0f;
const float TUTORIAL_POSISION_Z_THREE = -2096.0f;
const float TUTORIAL_POSISION_X_FOUR = 4000.0f;
const float TUTORIAL_POSISION_Z_FOUR = -3157.0f;
const float TUTORIAL_POSISION_X_FIVE = 3691.0f;
const float TUTORIAL_POSISION_Z_FIVE = -2057.0f;

const float FLONT_SPOWN_POSISION_X_ONE = 6021.0f;
const float FLONT_SPOWN_POSISION_Z_ONE = 5152.0f;
const float FLONT_SPOWN_POSISION_X_TWO = 7284.0f;
const float FLONT_SPOWN_POSISION_Z_TWO = 6232.0f;
const float FLONT_SPOWN_POSISION_X_THREE = 7315.0f;
const float FLONT_SPOWN_POSISION_Z_THREE = 6897.0f;
const float FLONT_SPOWN_POSISION_X_FOUR = 8629.0f;
const float FLONT_SPOWN_POSISION_Z_FOUR = 7695.0f;
const float FLONT_SPOWN_POSISION_X_FIVE = 5724.0f;
const float FLONT_SPOWN_POSISION_Z_FIVE = 9421.0f;

const float SPOWN_POSISION_X_ONE = 651.0f;
const float SPOWN_POSISION_Z_ONE = 4540.0f;
const float SPOWN_POSISION_X_TWO = -1025.0f;
const float SPOWN_POSISION_Z_TWO = 5071.0f;
const float SPOWN_POSISION_X_THREE = -2999.0f;
const float SPOWN_POSISION_Z_THREE = 8543.0f;
const float SPOWN_POSISION_X_FOUR = -7323.0f;
const float SPOWN_POSISION_Z_FOUR = 7603.0f;
const float SPOWN_POSISION_X_FIVE = -8742.0f;
const float SPOWN_POSISION_Z_FIVE = 6139.0f;
const float LAST_BOSS_POSION_X = -1837.f;
const float LAST_BOSS_POSION_Z = 2788.f;
const float LAST_SIZE = 40.0f;
const float DEFAULT_BOSS_SIZE = 5.0f;

//--------------------------------------
//静的メンバ変数の宣言
//--------------------------------------
CEnemy *CEnemyManager::m_pEnemy;

//--------------------------------------
//コンストラクタ
//--------------------------------------
CEnemyManager::CEnemyManager()
{
	m_pEnemy = nullptr;
	m_nPopCounter = 0;
	m_nRand_Spawn_Point = 0;
	m_nScaleCounter = 0;
	m_nNumber = 0;
	m_nSpawnType = 0;
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CEnemyManager::~CEnemyManager()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CEnemyManager::Init()
{
	srand((unsigned int)time(nullptr));				//ランダムの初期化

	SpawnTutoral();									//チュートリアル用エネミーを出現させる

	m_nRand_Spawn_Point = rand() % 4;				//ランダム値の初期値を設定
	m_nChangeSpawn = rand() % 2;
	m_nSpawnType = rand() % 4;

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CEnemyManager::Uninit()
{
	if (m_pEnemy != nullptr)
	{//エネミーのポインタに何かしらの値が入っていたら
		m_pEnemy = nullptr;					//ヌルを代入
		m_nPopCounter = 0;					//カウンターをリセット
	}
}

//--------------------------------------
//更新
//--------------------------------------
void CEnemyManager::Update()
{
	m_nPopCounter++;

	if (m_nPopCounter % 80 == 0)
	{
		m_nRand_Spawn_Point = rand() % 4;									//ランダム値の初期値を設定

		SetChangeSpawn();													//エネミーの出現位置を設定する
	}

#ifdef _DEBUG
	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_5))
	{
		m_pEnemy = CEnemy::Create("Data/Model/EnemyModel/enemy_down.x", 
			"Data/Model/EnemyModel/enemy_up.x",
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			CObject::OBJTYPE_ENEMY_NORMAL,
			DEFAULT_LIFE);
	}
#endif // _DEBUG
}

//--------------------------------------
//描画
//--------------------------------------
void CEnemyManager::Draw()
{
}

//--------------------------------------
//チュートリアル用のエネミーを
//出現させる
//--------------------------------------
void CEnemyManager::SpawnTutoral()
{
	SpawnNormal(TUTORIAL_POSISION_X_ONE, TUTORIAL_POSISION_Z_ONE);

	SpawnAttacker(TUTORIAL_POSISION_X_TWO, TUTORIAL_POSISION_Z_TWO);

	SpawnSpeeder(TUTORIAL_POSISION_X_THREE, TUTORIAL_POSISION_Z_THREE);

	SpawnDoctor(TUTORIAL_POSISION_X_FOUR, TUTORIAL_POSISION_Z_FOUR);

	SpawnMulti(TUTORIAL_POSISION_X_FIVE, TUTORIAL_POSISION_Z_FIVE);

	SpawnBoss(LAST_BOSS_POSION_X, LAST_BOSS_POSION_Z, LAST_SIZE);
}

//--------------------------------------
//出現位置の設定
//--------------------------------------
void CEnemyManager::SetSpawnPoint()
{
	switch (m_nRand_Spawn_Point)
	{
	case 0:

		SpawnNormal(FLONT_SPOWN_POSISION_X_ONE, FLONT_SPOWN_POSISION_Z_ONE);

		if (m_nPopCounter > 360)
		{
			SpawnNormal(SPOWN_POSISION_X_ONE, SPOWN_POSISION_Z_ONE);
		}

		m_nScaleCounter++;

		break;

	case 1:

		SpawnNormal(FLONT_SPOWN_POSISION_X_TWO, FLONT_SPOWN_POSISION_Z_TWO);

		if (m_nPopCounter > 360)
		{
			SpawnNormal(SPOWN_POSISION_X_TWO, SPOWN_POSISION_Z_TWO);
		}

		m_nScaleCounter++;

		break;

	case 2:

		SpawnNormal(FLONT_SPOWN_POSISION_X_THREE, FLONT_SPOWN_POSISION_Z_THREE);

		if (m_nPopCounter > 360)
		{
			SpawnNormal(SPOWN_POSISION_X_THREE, SPOWN_POSISION_Z_THREE);
		}

		m_nScaleCounter++;

		break;

	case 3:

		SpawnNormal(FLONT_SPOWN_POSISION_X_FOUR, FLONT_SPOWN_POSISION_Z_FOUR);

		if (m_nPopCounter > 360)
		{
			SpawnNormal(SPOWN_POSISION_X_FOUR, SPOWN_POSISION_Z_FOUR);
		}

		m_nScaleCounter++;

		break;

	default:
		break;
	}

	if(m_nScaleCounter >= 10)
	{//出現カウンターが10以上なら
		SetBigSpawnPoint();					//出現位置を設定する
	}
}

//--------------------------------------
//出現位置の設定
//--------------------------------------
void CEnemyManager::SetAnotherPoint()
{
	m_nSpawnType = rand() % 4;

	switch (m_nRand_Spawn_Point)
	{
	case 0:

		switch (m_nSpawnType)
		{
		case 0: 
			SpawnAttacker(SPOWN_POSISION_X_ONE, SPOWN_POSISION_Z_ONE);
			m_nScaleCounter++;
			break;
		case 1:
			SpawnSpeeder(SPOWN_POSISION_X_ONE, SPOWN_POSISION_Z_ONE);
			m_nScaleCounter++;
			break;
		case 2:
			SpawnDoctor(SPOWN_POSISION_X_ONE, SPOWN_POSISION_Z_ONE);
			m_nScaleCounter++;
			break;
		case 3:
			SpawnMulti(SPOWN_POSISION_X_ONE, SPOWN_POSISION_Z_ONE);
			m_nScaleCounter++;
			break;
		default:
			break;
		}

		break;

	case 1:

		switch (m_nSpawnType)
		{
		case 0:
			SpawnAttacker(SPOWN_POSISION_X_TWO, SPOWN_POSISION_Z_TWO);
			m_nScaleCounter++;
			break;
		case 1:
			SpawnSpeeder(SPOWN_POSISION_X_TWO, SPOWN_POSISION_Z_TWO);
			m_nScaleCounter++;
			break;
		case 2:
			SpawnDoctor(SPOWN_POSISION_X_TWO, SPOWN_POSISION_Z_TWO);
			m_nScaleCounter++;
			break;
		case 3:
			SpawnMulti(SPOWN_POSISION_X_TWO, SPOWN_POSISION_Z_TWO);
			m_nScaleCounter++;
			break;
		default:
			break;
		}

		break;

	case 2:

		switch (m_nSpawnType)
		{
		case 0:
			SpawnAttacker(SPOWN_POSISION_X_THREE, SPOWN_POSISION_Z_THREE);
			m_nScaleCounter++;
			break;
		case 1:
			SpawnSpeeder(SPOWN_POSISION_X_THREE, SPOWN_POSISION_Z_THREE);
			m_nScaleCounter++;
			break;
		case 2:
			SpawnDoctor(SPOWN_POSISION_X_THREE, SPOWN_POSISION_Z_THREE);
			m_nScaleCounter++;
			break;
		case 3:
			SpawnMulti(SPOWN_POSISION_X_THREE, SPOWN_POSISION_Z_THREE);
			m_nScaleCounter++;
			break;
		default:
			break;
		}

		break;

	case 3:

		switch (m_nSpawnType)
		{
		case 0:
			SpawnAttacker(SPOWN_POSISION_X_FOUR, SPOWN_POSISION_Z_FOUR);
			m_nScaleCounter++;
			break;
		case 1:
			SpawnSpeeder(SPOWN_POSISION_X_FOUR, SPOWN_POSISION_Z_FOUR);
			m_nScaleCounter++;
			break;
		case 2:
			SpawnDoctor(SPOWN_POSISION_X_FOUR, SPOWN_POSISION_Z_FOUR);
			m_nScaleCounter++;
			break;
		case 3:
			SpawnMulti(SPOWN_POSISION_X_FOUR, SPOWN_POSISION_Z_FOUR);
			m_nScaleCounter++;
			break;
		default:
			break;
		}

		break;

	default:
		break;
	}
}

//--------------------------------------
//巨体の出現位置の設定
//--------------------------------------
void CEnemyManager::SetBigSpawnPoint()
{
	m_nBig_Spawn_Point = rand() % 4;				//ランダム値の初期値を設定

	switch (m_nRand_Spawn_Point)
	{
	case 0:

		SpawnBoss(SPOWN_POSISION_X_FIVE, SPOWN_POSISION_Z_FIVE, DEFAULT_BOSS_SIZE);

		break;

	case 1:

		SpawnBoss(SPOWN_POSISION_X_FIVE, SPOWN_POSISION_Z_FIVE, DEFAULT_BOSS_SIZE + 5.0f);

		break;

	case 2:

		SpawnBoss(SPOWN_POSISION_X_FIVE, SPOWN_POSISION_Z_FIVE, DEFAULT_BOSS_SIZE + 10.0f);

		break;

	case 3:

		SpawnBoss(SPOWN_POSISION_X_FIVE, SPOWN_POSISION_Z_FIVE, DEFAULT_BOSS_SIZE + 15.0f);

		break;

	default:
		break;
	}
}

//--------------------------------------
//エネミーの出現する種類を設定
//--------------------------------------
void CEnemyManager::SetChangeSpawn()
{
	m_nChangeSpawn = rand() % 2;
	
	switch (m_nChangeSpawn)
	{
	case 0:
		SetSpawnPoint();
		break;

	case 1:
		SetAnotherPoint();
		break;

	default:
		break;
	}
}

//--------------------------------------
//通常の種類の出現を設定
//--------------------------------------
void CEnemyManager::SpawnNormal(float fPosX, float fPosZ)
{
	m_pEnemy = CEnemy::Create("Data/Model/EnemyModel/Nomal_Down.x",
		"Data/Model/EnemyModel/Nomal_Up.x",
		D3DXVECTOR3(fPosX,
			HIGHT_POSITION,
			fPosZ),
		CObject::OBJTYPE_ENEMY_NORMAL,
		DEFAULT_LIFE);

	m_pEnemy->SetNumber(m_nNumber);

	m_nNumber++;
}

//--------------------------------------
//火力マシマシの種類の出現を設定
//--------------------------------------
void CEnemyManager::SpawnAttacker(float fPosX, float fPosZ)
{
	m_pEnemy = CEnemy::Create("Data/Model/EnemyModel/enemy_down.x",
		"Data/Model/EnemyModel/enemy_up.x",
		D3DXVECTOR3(fPosX,
			HIGHT_POSITION, 
			fPosZ),
		CObject::OBJTYPE_ENEMY_ATTACKER,
		DEFAULT_LIFE);

	m_pEnemy->SetNumber(m_nNumber);

	m_nNumber++;
}

//--------------------------------------
//速度マシマシの種類の出現を設定
//--------------------------------------
void CEnemyManager::SpawnSpeeder(float fPosX, float fPosZ)
{
	m_pEnemy = CEnemy::Create("Data/Model/EnemyModel/Speeder_Under.x",
		"Data/Model/EnemyModel/Speeder_Up.x",
		D3DXVECTOR3(fPosX,
			HIGHT_POSITION,
			fPosZ),
		CObject::OBJTYPE_ENEMY_SPEEDER,
		DEFAULT_LIFE);

	m_pEnemy->SetSpeed(SPEEDER_INTEGER);

	m_pEnemy->SetNumber(m_nNumber);

	m_nNumber++;
}

//--------------------------------------
//回復する出現を設定
//--------------------------------------
void CEnemyManager::SpawnDoctor(float fPosX, float fPosZ)
{
	m_pEnemy = CEnemy::Create("Data/Model/EnemyModel/Doctor_Down.x",
		"Data/Model/EnemyModel/Doctor_Up.x",
		D3DXVECTOR3(fPosX,
			HIGHT_POSITION,
			fPosZ),
		CObject::OBJTYPE_ENEMY_DOCTOR,
		DEFAULT_LIFE);

	m_pEnemy->SetNumber(m_nNumber);

	m_nNumber++;
}

//--------------------------------------
//複合能力持ちの出現を設定
//--------------------------------------
void CEnemyManager::SpawnMulti(float fPosX, float fPosZ)
{
	m_pEnemy = CEnemy::Create("Data/Model/EnemyModel/Multi_Down.x",
		"Data/Model/EnemyModel/Multi_Up.x",
		D3DXVECTOR3(fPosX,
			HIGHT_POSITION,
			fPosZ),
		CObject::OBJTYPE_ENEMY_MULTI,
		DEFAULT_LIFE);

	m_pEnemy->SetNumber(m_nNumber);

	m_nNumber++;
}

//--------------------------------------
//ボスの出現を設定
//--------------------------------------
void CEnemyManager::SpawnBoss(float fPosX, float fPosZ, float fsize)
{
	m_pEnemy = CEnemy::Create("Data/Model/EnemyModel/enemy_down.x",
		"Data/Model/EnemyModel/enemy_up.x",
		D3DXVECTOR3(fPosX,
			HIGHT_POSITION,
			fPosZ),
		CObject::OBJTYPE_ENEMY_BOSS,
		DEFAULT_LIFE);

	m_pEnemy->SetScale(fsize);			//スケールの設定
	m_pEnemy->SetSpeed(7.0f);
	m_pEnemy->SetLife(DEFAULT_BOSSLIFE);

	m_pEnemy->SetNumber(m_nNumber);

	m_nNumber++;
}
