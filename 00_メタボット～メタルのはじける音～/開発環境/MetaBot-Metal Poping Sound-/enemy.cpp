//-----------------------------------------------------------------------------------------------------------
//
//エネミー
//Author;takano
//
//-----------------------------------------------------------------------------------------------------------

//--------------------------
//インクルードファイル
//--------------------------
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "enemymanager.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "score.h"
#include <assert.h>
#include <time.h>
#include "helperfunction.h"
#include "sound.h"
#include "effect.h"
#include "bildingmanager.h"
#include "bilding.h"

//-------------------------
//定数定義
//-------------------------
const D3DXCOLOR BLOOD_COLOR = D3DXCOLOR(1.f, 0.f, 0.f, 1.0f);	//出現させるエフェクトの色
const D3DXCOLOR SCALE_UP_COLOR = D3DXCOLOR(0.f,0.f,1.f,1.0f);	//出現させるエフェクトの色
const float MOVE_SPEED = 4.0f;								//移動用
const float CHASE_SPEED = 4.6f;								//追従の際に使用する移動速度
const float CHASE_SPEEDER = 8.0f;							//速度マシマシの追従速度
const float RUN_SPEED = 4.0f;								//逃げる速度
const float RUN_SPEEDER = 8.0f;								//速度マシマシが逃げる速度
const float DEFAULT_MOTION_FRAME = 60.0f;					//モーションで使用するフレームのデフォルト値
const float PLAYER_COLLISION_AREA = 200.0f;					//プレイヤーに反応する判定の範囲
const float ENEMY_REACTION_AREA = 200.0f;					//エネミーがプレイヤーに反応する範囲
const float COUNTER_ATTACK_AREA = 100.0f;					//エネミーがカウンターをする範囲
const float MAX_SCALE = 40.0f;								//スケールの最大値
const float EFFECT_SIZE = 30.0f;							//エフェクトサイズの基本値
const float DEFAULT_EFFECT_SIZE = 20.0f;					//エフェクトの基本値
const int NUM_EFFECT = 10;									//一度に出現させるエフェクトの最大値
const int DEFAULT_SCORE = 100;								//プレイヤーに与えるスコアの基本値
const int DEFAULT_BOSS_SCORE = 1000;						//プレイヤーに与えるボススコアの基本値

//-------------------------
//コンストラクタ
//-------------------------
CEnemy::CEnemy()
{
	for (int i = 0; i < 4; i++)
	{
		m_pParts[i] = nullptr;				//パーツ分変数をクリアリングする
	}

	m_nNumber = 0;

	m_HitCount = 0;

	m_motionType = TYPE_NONE;				//モーションタイプをクリアリングする
}

//-------------------------
//デストラクタ
//-------------------------
CEnemy::~CEnemy()
{
}

//-------------------------
//クリエイト
//-------------------------
CEnemy * CEnemy::Create(const char* pfileParent, const char* pfileSon, D3DXVECTOR3 pos, ObjType type, int nLife)
{
	//動的確保
	CEnemy* pEnemy = new CEnemy;

	if (pEnemy != nullptr)
	{//エネミーのポインタに何かしらの値が入っていたら
		pEnemy->Init();										//エネミーの初期化
		pEnemy->m_pParts[0]->SetModel(pfileParent);
		pEnemy->m_pParts[1]->SetModel(pfileSon);
		pEnemy->SetPos(pos);								//位置を設定
		pEnemy->SetLife(nLife);								//ライフの設定
		pEnemy->SetObjectType(type);						//オブジェクトタイプの設定"Data/Model/enemy_up.x"
	}

	//値を返す
	return pEnemy;
}

//-------------------------
//初期化
//-------------------------
HRESULT CEnemy::Init()
{
	//初期化
	m_fMotionCounter = 0.0f;					//モーションカウンターの初期化
	m_nNumKey = 0;								//キーのカウンターを初期化
	m_motionType = TYPE_IDLE;					//初期モーションの設定
	m_fSpeed = MOVE_SPEED;
	m_Scale = D3DXVECTOR3(1.0f,1.0f,1.0f);		//スケールの初期化

	//モデルの生成
	m_pParts[0] = CModel::Create("Data/Model/EnemyModel/enemy_down.x", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),CModel::TYPE_ENEMY);
	m_pParts[1] = CModel::Create("Data/Model/EnemyModel/enemy_up.x", D3DXVECTOR3(0.0f, 19.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CModel::TYPE_ENEMY);

	for (int nPartsCount = 0; nPartsCount < PARTS_MAX; nPartsCount++)
	{//	パーツの初期化
		if (nPartsCount == 0)
		{//パーツが0番目だったとき(親だったとき)
			continue;		//forを飛ばす。(戻す)
		}

		//親子付けかなぁ？
		m_pParts[nPartsCount]->SetParent(m_pParts[0]);
	}

	for (int nKeyCount = 0; nKeyCount < KEY_MAX; nKeyCount++)
	{// フレームの初期化
		m_MotionInfo.aKeyInfo[nKeyCount].fFrame = DEFAULT_MOTION_FRAME;
	}

	srand((unsigned int)time(nullptr));						//ランダムの初期化

	m_nRand = rand() % 5;									//ランダム値の初期値を設定

	m_nRand_Up_Multi = rand() % 2;							//multiで使用する変数の初期化

	return S_OK;
}

//------------------------
//終了
//------------------------
void CEnemy::Uninit()
{
	//オブジェクトの開放
	CObject::Release();
}

//---------------------
//更新
//---------------------
void CEnemy::Update()
{
	m_posOld = m_pos;					//位置の設定

	m_motionType = TYPE_WARK;			//モードの更新

	Command();							//行動規範を設定

	PlayMotion();						//モーションを動かす

	MotionManager(m_motionType);		//モーションタイプの設定

	Collision();						//当たり判定
}

//----------------------
//描画
//----------------------
void CEnemy::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();		//デバイスのポインタ

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;										//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//スケールを反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスに設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int k = 0; k < 2; k++)
	{
		m_pParts[k]->Draw();		//パーツの描画
	}
}

//---------------------------------
//再生モーション
//---------------------------------
void CEnemy::PlayMotion()
{
	D3DXVECTOR3 DestPos[PARTS_MAX];			//モーションの位置の差分を入れる変数
	D3DXVECTOR3 DestRot[PARTS_MAX];			//モーションの角度の差分を入れる変数
	D3DXVECTOR3 AuthorPos[PARTS_MAX];		//モーションの位置の計算結果を入れる変数
	D3DXVECTOR3 AuthorRot[PARTS_MAX];		//モーションの角度の計算結果を入れる変数

	if (m_nNumKey + 1 >= KEY_MAX)
	{//キーが最大数を超えた時
		m_nNumKey = 0;						//キーを最初に設定する
	}

	for (int nCountParts = 0; nCountParts < PARTS_MAX; nCountParts++)
	{
		//位置の差分を求める
		DestPos[nCountParts].x = m_MotionInfo.aKeyInfo[m_nNumKey + 1].aKeyData[nCountParts].fPosX - m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[nCountParts].fPosX;
		DestPos[nCountParts].y = m_MotionInfo.aKeyInfo[m_nNumKey + 1].aKeyData[nCountParts].fPosY - m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[nCountParts].fPosY;
		DestPos[nCountParts].z = m_MotionInfo.aKeyInfo[m_nNumKey + 1].aKeyData[nCountParts].fPosZ - m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[nCountParts].fPosZ;

		//角度の差分を求める
		DestRot[nCountParts].x = m_MotionInfo.aKeyInfo[m_nNumKey + 1].aKeyData[nCountParts].fRotX - m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[nCountParts].fRotX;
		DestRot[nCountParts].y = m_MotionInfo.aKeyInfo[m_nNumKey + 1].aKeyData[nCountParts].fRotY - m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[nCountParts].fRotY;
		DestRot[nCountParts].z = m_MotionInfo.aKeyInfo[m_nNumKey + 1].aKeyData[nCountParts].fRotZ - m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[nCountParts].fRotZ;

		//保管する位置の値を求める
		AuthorPos[nCountParts].x = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[nCountParts].fPosX + DestPos[nCountParts].x * (m_fMotionCounter / m_MotionInfo.aKeyInfo[m_nNumKey].fFrame);
		AuthorPos[nCountParts].y = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[nCountParts].fPosY + DestPos[nCountParts].y * (m_fMotionCounter / m_MotionInfo.aKeyInfo[m_nNumKey].fFrame);
		AuthorPos[nCountParts].z = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[nCountParts].fPosZ + DestPos[nCountParts].z * (m_fMotionCounter / m_MotionInfo.aKeyInfo[m_nNumKey].fFrame);

		//保管する角度の値を求める
		AuthorRot[nCountParts].x = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[nCountParts].fRotX + DestRot[nCountParts].x * (m_fMotionCounter / m_MotionInfo.aKeyInfo[m_nNumKey].fFrame);
		AuthorRot[nCountParts].y = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[nCountParts].fRotY + DestRot[nCountParts].y * (m_fMotionCounter / m_MotionInfo.aKeyInfo[m_nNumKey].fFrame);
		AuthorRot[nCountParts].z = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[nCountParts].fRotZ + DestRot[nCountParts].z * (m_fMotionCounter / m_MotionInfo.aKeyInfo[m_nNumKey].fFrame);

		//求めた値を使用して位置を動かす
		m_pParts[nCountParts]->SetPos(D3DXVECTOR3(AuthorPos[nCountParts].x + m_pParts[nCountParts]->GetStartPoint().x,
			AuthorPos[nCountParts].y + m_pParts[nCountParts]->GetStartPoint().y,
			AuthorPos[nCountParts].z + m_pParts[nCountParts]->GetStartPoint().z));

		//求めた値を使用して角度を動かす
		m_pParts[nCountParts]->SetRot(D3DXVECTOR3(AuthorRot[nCountParts].x + m_pParts[nCountParts]->GetStartRotation().x,
			AuthorRot[nCountParts].y + m_pParts[nCountParts]->GetStartRotation().y,
			AuthorRot[nCountParts].z + m_pParts[nCountParts]->GetStartRotation().z));
	}

	//モーションカウンターの加算
	m_fMotionCounter++;

	if (m_fMotionCounter >= m_MotionInfo.aKeyInfo[m_nNumKey].fFrame)
	{
		m_fMotionCounter = 0.0f;
		m_nNumKey++;
	}

	if (m_nNumKey >= KEY_MAX)
	{
		//位置の差分を求める
		DestPos[KEY_MAX].x = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[KEY_MAX].fPosX - m_MotionInfo.aKeyInfo[0].aKeyData[0].fPosX;
		DestPos[KEY_MAX].y = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[KEY_MAX].fPosY - m_MotionInfo.aKeyInfo[0].aKeyData[0].fPosY;
		DestPos[KEY_MAX].z = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[KEY_MAX].fPosZ - m_MotionInfo.aKeyInfo[0].aKeyData[0].fPosZ;

		//角度の差分を求める
		DestRot[KEY_MAX].x = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[KEY_MAX].fRotX - m_MotionInfo.aKeyInfo[0].aKeyData[0].fRotX;
		DestRot[KEY_MAX].y = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[KEY_MAX].fRotY - m_MotionInfo.aKeyInfo[0].aKeyData[0].fRotY;
		DestRot[KEY_MAX].z = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[KEY_MAX].fRotZ - m_MotionInfo.aKeyInfo[0].aKeyData[0].fRotZ;

		m_nNumKey = 0;
	}
}

//---------------------------------
//エネミーに当たったとき
//---------------------------------
void CEnemy::HitNormal(int nCombo)
{
	SubLife(1);

	if (m_nLife <= 0)
	{
		CPlayer* pPlayer = CGame::GetPlayer();

		//スコアを加算
		CGame::GetScore()->Add(DEFAULT_SCORE + ((nCombo -1) * 10));

		CEffect::Spawn(D3DXVECTOR3(m_pos.x,m_pos.y,m_pos.z),		//位置の設定
			NUM_EFFECT,												//エフェクトの最大数
			DEFAULT_EFFECT_SIZE,			//ランダムの最大数
			-DEFAULT_EFFECT_SIZE,			//ランダムの最小数
			BLOOD_COLOR,											//色の設定
			EFFECT_SIZE,							//エフェクトサイズ
			EFFECT_SIZE);							//エフェクトサイズ

		CApplication::GetSound()->Play(CSound::SOUND_SE_BREAK);

		if (pPlayer->GetScale().y < MAX_SCALE)
		{
			CEffect::Spawn(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z),		//位置の設定
				NUM_EFFECT,														//エフェクトの最大数
				DEFAULT_EFFECT_SIZE,							//ランダムの最大数
				-DEFAULT_EFFECT_SIZE,							//ランダムの最小数
				SCALE_UP_COLOR,									//色の設定
				EFFECT_SIZE* pPlayer->GetScale().y,
				EFFECT_SIZE* pPlayer->GetScale().y);

			CApplication::GetSound()->Play(CSound::SOUND_SE_SIZE_UP);
			pPlayer->UpScale(0.5f);

			pPlayer->AddInertia(0.4f);
		}

		//オブジェクトの開放
		Release();
	}
}

//---------------------------------
//アタッカーに当たったとき
//---------------------------------
void CEnemy::HitAttacker(int nCombo)
{
	SubLife(1);

	CPlayer* pPlayer = CGame::GetPlayer();

	if (m_nLife <= 0)
	{
		//スコアを加算
		CGame::GetScore()->Add(DEFAULT_SCORE + 20);

		CEffect::Spawn(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z),		//位置の設定
			NUM_EFFECT,												//エフェクトの最大数
			DEFAULT_EFFECT_SIZE,							//ランダムの最大数
			-DEFAULT_EFFECT_SIZE,							//ランダムの最小数
			BLOOD_COLOR,						//色の設定
			EFFECT_SIZE,
			EFFECT_SIZE);

		CApplication::GetSound()->Play(CSound::SOUND_SE_BREAK);

		if (pPlayer->GetScale().y < MAX_SCALE)
		{
			CEffect::Spawn(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z),		//位置の設定
				NUM_EFFECT,														//エフェクトの最大数
				DEFAULT_EFFECT_SIZE,							//ランダムの最大数
				-DEFAULT_EFFECT_SIZE,							//ランダムの最小数
				SCALE_UP_COLOR,									//色の設定
				EFFECT_SIZE* pPlayer->GetScale().y,
				EFFECT_SIZE* pPlayer->GetScale().y);

			CApplication::GetSound()->Play(CSound::SOUND_SE_SIZE_UP);
			pPlayer->UpScale(0.2f);
		}

		pPlayer->AddInertia(0.1f);

		//オブジェクトの開放
		Release();
	}
}

//---------------------------------
//スピーダーに当たったとき
//---------------------------------
void CEnemy::HitSpeeder(int nCombo)
{
	SubLife(1);

	CPlayer* pPlayer = CGame::GetPlayer();

	if (m_nLife <= 0)
	{
		//スコアを加算
		CGame::GetScore()->Add(DEFAULT_SCORE);

		CEffect::Spawn(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z),		//位置の設定
			NUM_EFFECT,												//エフェクトの最大数
			DEFAULT_EFFECT_SIZE,						//ランダムの最大数
			-DEFAULT_EFFECT_SIZE,						//ランダムの最小数
			BLOOD_COLOR,											//色の設定
			EFFECT_SIZE,
			EFFECT_SIZE);

		CApplication::GetSound()->Play(CSound::SOUND_SE_BREAK);

		if (pPlayer->GetScale().y < MAX_SCALE)
		{
			CEffect::Spawn(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z),		//位置の設定
				NUM_EFFECT,														//エフェクトの最大数
				DEFAULT_EFFECT_SIZE,							//ランダムの最大数
				-DEFAULT_EFFECT_SIZE,							//ランダムの最小数
				SCALE_UP_COLOR,									//色の設定
				EFFECT_SIZE* pPlayer->GetScale().y,
				EFFECT_SIZE* pPlayer->GetScale().y);

			CApplication::GetSound()->Play(CSound::SOUND_SE_SIZE_UP);
			pPlayer->UpScale(0.2f);

		}

		pPlayer->AddInertia(4.0f);

		//オブジェクトの開放
		Release();
	}
}

//---------------------------------
//ドクターに当たったとき
//---------------------------------
void CEnemy::HitCure(int nCombo)
{
	SubLife(1);

	CPlayer* pPlayer = CGame::GetPlayer();

	if (m_nLife <= 0)
	{
		CEffect::Spawn(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z),		//位置の設定
			NUM_EFFECT,														//エフェクトの最大数
			DEFAULT_EFFECT_SIZE,							//ランダムの最大数
			-DEFAULT_EFFECT_SIZE,							//ランダムの最小数
			BLOOD_COLOR,						//色の設定
			EFFECT_SIZE,
			EFFECT_SIZE);

		CApplication::GetSound()->Play(CSound::SOUND_SE_BREAK);

		if (pPlayer->GetScale().y < MAX_SCALE)
		{
			CEffect::Spawn(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z),		//位置の設定
				NUM_EFFECT,														//エフェクトの最大数
				DEFAULT_EFFECT_SIZE,							//ランダムの最大数
				-DEFAULT_EFFECT_SIZE,							//ランダムの最小数
				SCALE_UP_COLOR,									//色の設定
				EFFECT_SIZE* pPlayer->GetScale().y,
				EFFECT_SIZE* pPlayer->GetScale().y);

			CApplication::GetSound()->Play(CSound::SOUND_SE_SIZE_UP);
			pPlayer->UpScale(0.2f);
		}

		pPlayer->AddLife(1);

		//スコアを加算
		CGame::GetScore()->AddLife(1);

		//オブジェクトの開放
		Release();
	}
}

//---------------------------------
//複合に当たったとき
//---------------------------------
void CEnemy::HitMulti(int nCombo)
{
	SubLife(1);

	CPlayer* pPlayer = CGame::GetPlayer();

	if (m_nLife <= 0)
	{
		//スコアを加算
		CGame::GetScore()->Add(DEFAULT_SCORE);

		CEffect::Spawn(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z),		//位置の設定
			NUM_EFFECT,														//エフェクトの最大数
			DEFAULT_EFFECT_SIZE,							//ランダムの最大数
			-DEFAULT_EFFECT_SIZE,							//ランダムの最小数
			BLOOD_COLOR,						//色の設定
			EFFECT_SIZE,
			EFFECT_SIZE);

		CApplication::GetSound()->Play(CSound::SOUND_SE_BREAK);

		if (pPlayer->GetScale().y < MAX_SCALE)
		{
			CEffect::Spawn(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z),		//位置の設定
				NUM_EFFECT,														//エフェクトの最大数
				DEFAULT_EFFECT_SIZE,							//ランダムの最大数
				-DEFAULT_EFFECT_SIZE,							//ランダムの最小数
				SCALE_UP_COLOR,									//色の設定
				EFFECT_SIZE* pPlayer->GetScale().y,
				EFFECT_SIZE* pPlayer->GetScale().y);

			CApplication::GetSound()->Play(CSound::SOUND_SE_SIZE_UP);
			pPlayer->UpScale(0.2f);

		}

		m_nRand_Up_Multi = rand() % 2;								//能力UP用の変数に値を設定

		switch (m_nRand_Up_Multi)
		{
		case 0:

			pPlayer->AddInertia(0.5f);		//速度(慣性)の増加

			break;

		case 1:

			pPlayer->AddLife(1);			//体力の増加

			CGame::GetScore()->AddLife(1);	//ライフカウンターを加算

			break;

		default:
			break;
		}

		//オブジェクトの開放
		Release();
	}
}

//---------------------------------
//ボスに当たったとき
//---------------------------------
void CEnemy::HitBoss(int nCombo)
{
	SubLife(1);

	CPlayer* pPlayer = CGame::GetPlayer();

	if (m_nLife < 0)
	{
		//スコアを加算
		CGame::GetScore()->Add(DEFAULT_BOSS_SCORE * (int)m_Scale.y);

		CEffect::Spawn(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z),		//位置の設定
			NUM_EFFECT,														//エフェクトの最大数
			DEFAULT_EFFECT_SIZE,							//ランダムの最大数
			-DEFAULT_EFFECT_SIZE,							//ランダムの最小数
			BLOOD_COLOR,						//色の設定
			EFFECT_SIZE * pPlayer->GetScale().y,
			EFFECT_SIZE * pPlayer->GetScale().y);

		CApplication::GetSound()->Play(CSound::SOUND_SE_BREAK);

		if (pPlayer->GetScale().y < MAX_SCALE)
		{
			CEffect::Spawn(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z),		//位置の設定
				NUM_EFFECT,														//エフェクトの最大数
				DEFAULT_EFFECT_SIZE,							//ランダムの最大数
				-DEFAULT_EFFECT_SIZE,							//ランダムの最小数
				SCALE_UP_COLOR,									//色の設定
				EFFECT_SIZE* pPlayer->GetScale().y,
				EFFECT_SIZE* pPlayer->GetScale().y);

			CApplication::GetSound()->Play(CSound::SOUND_SE_SIZE_UP);
			pPlayer->UpScale(2.0f);

		}

		//オブジェクトの開放
		Release();
	}
}

//---------------------------------
//エネミーのライフ減算処理
//---------------------------------
void CEnemy::SubLife(int nSub)
{ 
	m_nLife -= nSub; 
}

//---------------------------------
//スケールの設定
//---------------------------------
void CEnemy::SetScale(float fScale)
{
	m_Scale.x = fScale;
	m_Scale.y = fScale;
	m_Scale.z = fScale;
}

//---------------------------------
//更新の関数分け
//---------------------------------
void CEnemy::Command()
{
	//プレイヤーの位置を取得
	D3DXVECTOR3 PLpos = CGame::GetPlayer()->GetPos();
	float fScale = CGame::GetPlayer()->GetScale().y;

	if (m_objType == CObject::OBJTYPE_ENEMY_DOCTOR &&
		CHelperFunction::CircleCollision(PLpos, PLAYER_COLLISION_AREA + ((fScale - 1.0f) * 50.0f ), m_pos, ENEMY_REACTION_AREA))
	{// 円の当たり判定で当たり判定が反応したら
		Run();		//プレイヤーから逃げる関数を呼び出す
		return;		//ここで返す
	}

	else if (m_objType == CObject::OBJTYPE_ENEMY_ATTACKER &&
		CHelperFunction::CircleCollision(PLpos, PLAYER_COLLISION_AREA + ((fScale - 1.0f) * 50.0f), m_pos, ENEMY_REACTION_AREA))
	{// 円の当たり判定で当たり判定が反応したら
		Chase();		//プレイヤーを追撃する関数を呼び出す
		return;			//ここで反旗を翻す
	}

	else
	{
		if (CHelperFunction::CircleCollision(PLpos, PLAYER_COLLISION_AREA + ((fScale - 1.0f) * 50.0f), m_pos, ENEMY_REACTION_AREA))
		{// 円の当たり判定で当たり判定が反応したら
			SelectAction();			//行動を選択する関数を呼び出す。
			return;					//ここで返す
		}
	}

	//ランダム化変数を加算
	m_nRandCount++;

	if (m_nRandCount % 120 == 0)
	{//ランダム変数にランダムの値を入れる
		m_nRand = rand() % 5;
	}
	
	switch (m_nRand)
	{
	case 0:

		m_motionType = TYPE_IDLE;									//使用するモーションの設定

		NormalizationRotation();									//角度の正規化

		MoveCalculation();											//移動量の計算

		break;

	case 1:

		m_move.x = sinf(D3DX_PI * 0.0f) * m_fSpeed;				//エネミーの位置をX方向に動かす
		m_move.z = cosf(D3DX_PI * 0.0f) * m_fSpeed;				//エネミーの位置をZ方向に動かす

		m_rotDest.y = m_move.y - D3DX_PI * 1.0f;					//エネミーの移動中の向きを設定

		m_motionType = TYPE_WARK;									//使用するモーションの設定

		NormalizationRotation();									//角度の正規化

		MoveCalculation();											//移動量の計算

		break;

	case 2:

		m_move.x = sinf(D3DX_PI * 1.0f) * m_fSpeed;				//エネミーの位置をX方向に動かす
		m_move.z = cosf(D3DX_PI * 1.0f) * m_fSpeed;				//エネミーの位置をZ方向に動かす

		m_rotDest.y = m_move.y + D3DX_PI * 0.0f;					//エネミーの移動中の向きを設定

		m_motionType = TYPE_WARK;									//使用するモーションの設定

		NormalizationRotation();									//角度の正規化

		MoveCalculation();											//移動量の計算

		break;

	case 3:

		m_move.x = -sinf(D3DX_PI * 0.5f) * m_fSpeed;				//エネミーの位置をX方向に動かす
		m_move.z = -cosf(D3DX_PI * 0.5f) * m_fSpeed;				//エネミーの位置をZ方向に動かす

		m_rotDest.y = m_move.y + D3DX_PI * 0.5f;					//エネミーの移動中の向きを設定

		m_motionType = TYPE_WARK;									//使用するモーションの設定

		NormalizationRotation();									//角度の正規化

		MoveCalculation();											//移動量の計算

		break;

	case 4:

		m_move.x = sinf(D3DX_PI * 0.5f) * m_fSpeed;				//エネミーの位置をX方向に動かす
		m_move.z = cosf(D3DX_PI * 0.5f) * m_fSpeed;				//エネミーの位置をZ方向に動かす

		m_rotDest.y = m_move.y - D3DX_PI * 0.5f;					//エネミーの移動中の向きを設定

		m_motionType = TYPE_WARK;									//使用するモーションの設定

		NormalizationRotation();									//角度の正規化

		MoveCalculation();											//移動量の計算

		break;

	default:		//上記以外だった場合
		assert(false);			//プログラムを止めます。デバックの時だけ
		break;
	}
}

//---------------------------------
//角度の正規化
//---------------------------------
void CEnemy::NormalizationRotation()
{
	//角度の正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{//元の回転角から目標の回転角との差が180度より大きくなった時
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{//元の回転角から目標の回転角との差が-180度よりも小さくなった時
		m_rotDest.y += D3DX_PI * 2;
	}

	//各目標の角度に元の角度を加算する
	m_rot.x += (m_rotDest.x - m_rot.x) *  0.08f;
	m_rot.y += (m_rotDest.y - m_rot.y) *  0.08f;
	m_rot.z += (m_rotDest.z - m_rot.z) *  0.08f;
}

//---------------------------------
//移動量の計算
//---------------------------------
void CEnemy::MoveCalculation()
{
	//移動量を位置に代入
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	//移動量を更新
	m_move.x += (0.0f - m_move.x) * 0.08f;
	m_move.z += (0.0f - m_move.z) * 0.08f;
}

//---------------------------------
//当たり判定まとめ
//---------------------------------
void CEnemy::Collision()
{
	for (int i = 0; i < MAX_OBJ; i++)
	{
		CObject *pObj = CObject::Getobject(i);		//オブジェクトの取得
		CObject::ObjType objType;					//オブジェクトタイプを宣言

		if (pObj == nullptr)
		{//オブジェクトが使用されていなかったとき
			continue;						//forを最初に返す
		}

		//オブジェクトタイプ変数にオブジェクトタイプを入れる
		objType = pObj->GetObjectType();

		if (objType == OBJTYPE_ENEMY_NORMAL ||
			objType == OBJTYPE_ENEMY_ATTACKER ||
			objType == OBJTYPE_ENEMY_SPEEDER ||
			objType == OBJTYPE_ENEMY_DOCTOR ||
			objType == OBJTYPE_ENEMY_MULTI ||
			objType == OBJTYPE_ENEMY_BOSS)
		{
			CEnemy* pEnemy = CGame::GetEnemyManager()->GetEnemy();

			if (m_nNumber == pEnemy->m_nNumber)
			{
				continue;
			}

			if (CHelperFunction::CircleCollision(m_pos, GetScale().y, pEnemy->m_pos, GetScale().y * 20.0f))
			{//当たり判定の返り値が真なら
				m_pos = m_posOld;						//位置を過去の位置に設定
			}
		}

		if (objType == OBJTYPE_PLAYER)
		{
			CPlayer* pPlayer = CGame::GetPlayer();

			if (CHelperFunction::CircleCollision(m_pos, m_Scale.y, pPlayer->GetPos(), pPlayer->GetScale().y * 20.0f))
			{//当たり判定の返り値が真なら

				m_pos = m_posOld;				//位置を過去の位置に設定

				m_HitCount++;					//カウンターを加算

				if (m_HitCount % 20 == 0)
				{//ヒットカウンターが割り切れたとき
					CApplication::GetSound()->Play(CSound::SOUND_SE_HIT);	//HitSEを再生
					pPlayer->SubLife(1);									//プレイヤーのライフを減らす
					pPlayer->ResetCombo();									//コンボをリセットする
					CGame::GetCamera()->Sequence(10.0f, 20.0f);				//カメラを揺らす
					CScore::HitDamage(1);									//操作者から見えるライフカウンターを減算
				}
			}
		}


		if ((objType == OBJTYPE_BILDING_HELLOWARK ||
			objType == OBJTYPE_BILDING_LANDMARKTOWER ||
			objType == OBJTYPE_BILDING_THUMBSUP ||
			objType == OBJTYPE_BILDING_DTHUMBSUP ||
			objType == OBJTYPE_BILDING_TEMPLE ||
			objType == OBJTYPE_BILDING_APOSTLE) &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * 30.0f, 0.0f, m_Scale.z * 30.0f), pObj->Getpos(), D3DXVECTOR3(60.0f, 0.0f, 60.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定
		}

		if (objType == OBJTYPE_WALL_OUTSIDE_WIDE &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * 30.0f, 0.0f, m_Scale.z * 30.0f), pObj->Getpos(), D3DXVECTOR3(10060.0f, 0.0f, 50.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定
		}

		if (objType == OBJTYPE_WALL_OUTSIDE_HIGHT &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * 30.0f, 0.0f, m_Scale.z * 30.0f), pObj->Getpos(), D3DXVECTOR3(52.0f, 0.0f, 10060.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定
		}

		if (objType == OBJTYPE_WALL_WIDE &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * 30.0f, 0.0f, m_Scale.z * 30.0f), pObj->Getpos(), D3DXVECTOR3(800.0f, 0.0f, 60.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定

		}

		if (objType == OBJTYPE_WALL_HIGHT &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * 30.0f, 0.0f, m_Scale.z * 30.0f), pObj->Getpos(), D3DXVECTOR3(60.0f, 0.0f, 800.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定

		}

		if (objType == OBJTYPE_WALL_LONGLONG_WILD &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * 30.0f, 0.0f, m_Scale.z * 30.0f), pObj->Getpos(), D3DXVECTOR3(2600.0f, 0.0f, 60.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定

		}

		if (objType == OBJTYPE_WALL_LONGLONG_HIGHT &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * 30.0f, 0.0f, m_Scale.z * 30.0f), pObj->Getpos(), D3DXVECTOR3(60.0f, 0.0f, 2600.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定

		}

		if (objType == OBJTYPE_FENCE_WIDE &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * 30.0f, 0.0f, m_Scale.z * 30.0f), pObj->Getpos(), D3DXVECTOR3(460.0f, 0.0f, 30.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定

		}

		if (objType == OBJTYPE_FENCE_HIGHT &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * 30.0f, 0.0f, m_Scale.z * 30.0f), pObj->Getpos(), D3DXVECTOR3(30.0f, 0.0f, 460.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定
		}

		if (objType == OBJTYPE_EAT_DESCRIPTION &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * 30.0f, 0.0f, m_Scale.z * 30.0f), pObj->Getpos(), D3DXVECTOR3(2460.0f, 0.0f, 30.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定
		}
	}
}

//---------------------------------
//モーションタイプの設定
//---------------------------------
void CEnemy::MotionManager(MOTION_TYPE type)
{
	m_motionType = type;

	switch (m_motionType)
	{
	case CEnemy::TYPE_IDLE:

		m_MotionInfo.aKeyInfo[0].aKeyData[0].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[0].aKeyData[1].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[0].fFrame = DEFAULT_MOTION_FRAME;

		m_MotionInfo.aKeyInfo[1].aKeyData[0].fPosX = 0.0f;
		m_MotionInfo.aKeyInfo[1].aKeyData[0].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[1].aKeyData[1].fRotX = D3DXToRadian(10.f);
		m_MotionInfo.aKeyInfo[1].fFrame = DEFAULT_MOTION_FRAME;

		m_MotionInfo.aKeyInfo[2].aKeyData[0].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[2].aKeyData[2].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[2].fFrame = DEFAULT_MOTION_FRAME;

		break;

	case CEnemy::TYPE_WARK:

		m_MotionInfo.aKeyInfo[0].aKeyData[0].fPosX = 0.0f;
		m_MotionInfo.aKeyInfo[0].aKeyData[1].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[0].fFrame = 30.0f;

		m_MotionInfo.aKeyInfo[1].aKeyData[0].fPosX = 0.0f;
		m_MotionInfo.aKeyInfo[1].aKeyData[0].fRotX = -0.70f;
		m_MotionInfo.aKeyInfo[1].aKeyData[1].fRotX = D3DXToRadian(92.f);
		m_MotionInfo.aKeyInfo[1].fFrame = 10.0f;

		m_MotionInfo.aKeyInfo[2].aKeyData[0].fPosX = 0.0f;
		m_MotionInfo.aKeyInfo[2].aKeyData[2].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[2].fFrame = 20.0f;

		break;

	default:
		assert(false);
		break;
	}
}

//---------------------------------
//エネミーの追従設定
//---------------------------------
void CEnemy::Chase()
{
	//プレイヤーの位置を取得
	D3DXVECTOR3 PLpos = CGame::GetPlayer()->GetPos();

	//ベクトルを扱う変数を宣言
	D3DXVECTOR3 Vec;

	//向きを扱う変数の宣言
	float fangle;

	//プレイヤーの位置からエネミーの位置を引く
	Vec = PLpos - m_pos;

	//ベクトルの大きさを出す
	D3DXVec3Length(&Vec);

	//ベクトルを１にする 第一引数に結果を保存するもの、第二引数に変換したいベクトルを入れる
	D3DXVec3Normalize(&Vec, &Vec);

	//角度の計算
	fangle = atan2f(Vec.x, Vec.z);

	if (m_objType == OBJTYPE_ENEMY_NORMAL ||
		m_objType == OBJTYPE_ENEMY_ATTACKER ||
		m_objType == OBJTYPE_ENEMY_MULTI ||
		m_objType == OBJTYPE_ENEMY_BOSS)
	{
		//エネミーの位置にベクトルを加算
		m_move = Vec * CHASE_SPEED;
	}

	else
	{
		//エネミーの位置にベクトルを加算
		m_move = Vec * CHASE_SPEEDER;
	}

	//移動量を計算
	MoveCalculation();

	//角度を対象の方向に設定
	m_rot.y = fangle + D3DX_PI;
}

//---------------------------------
//エネミーの逃亡設定
//---------------------------------
void CEnemy::Run()
{
	//プレイヤーの位置を取得
	D3DXVECTOR3 PLpos = CGame::GetPlayer()->GetPos();

	//ベクトルを扱う変数を宣言
	D3DXVECTOR3 Vec;

	//向きを扱う変数の宣言
	float fangle;

	//プレイヤーの位置からエネミーの位置を引く
	Vec = PLpos - m_pos;

	//ベクトルの大きさを出す
	D3DXVec3Length(&Vec);

	//ベクトルを１にする 第一引数に結果を保存するもの、第二引数に変換したいベクトルを入れる
	D3DXVec3Normalize(&Vec, &Vec);

	//角度の計算
	fangle = atan2f(Vec.x, Vec.z);

	if (m_objType == OBJTYPE_ENEMY_NORMAL ||
		m_objType == OBJTYPE_ENEMY_ATTACKER ||
		m_objType == OBJTYPE_ENEMY_DOCTOR ||
		m_objType == OBJTYPE_ENEMY_MULTI ||
		m_objType == OBJTYPE_ENEMY_BOSS)
	{
		//エネミーの位置にベクトルを加算
		m_move = -Vec * RUN_SPEED;
	}

	else
	{
		//エネミーの位置にベクトルを加算
		m_move = -Vec * RUN_SPEEDER;
	}

	//移動量を計算
	MoveCalculation();

	//角度を対象の方向に設定
	m_rot.y = fangle + D3DX_PI;
}
//---------------------------------
//エネミーの追撃or逃走の設定
//---------------------------------
void CEnemy::SelectAction()
{
	D3DXVECTOR3 pScale = CGame::GetPlayer()->GetScale();

	if (m_objType == OBJTYPE_ENEMY_BOSS)
	{
		if (m_Scale.y >= pScale.y)
		{
			m_motionType = TYPE_IDLE;									//使用するモーションの設定
			
			NormalizationRotation();									//角度の正規化

			MoveCalculation();											//移動量の計算
		}

		else
		{
			Chase();		//追撃設定
		}
	}

	else
	{
		if (m_Scale.y >= pScale.y ||
			m_Scale.y == pScale.y ||
			pScale.y - m_Scale.y < 1.0f)
		{//エネミーのスケールがプレイヤーのスケールより大きいか丁度同じとき
			Chase();		//追撃設定
		}

		else
		{//以上の条件以外
			Run();			//Run away.
		}
	}
}
