//-----------------------------------------------------------------------------------------------------------
//
//プレイヤー
//Author::takano()
//
//-----------------------------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "enemymanager.h"
#include "bilding.h"
#include "bildingmanager.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "helperfunction.h"
#include "score.h"
#include "result.h"
#include "ranking.h"
#include "effect.h"
#include "ui.h"
#include "eatdescription.h"
#include "sound.h"
#include <assert.h>

//--------------------------------------
//定数定義
//--------------------------------------
const float DEFAULT_MOTION_FRAME = 60.0f;				//モーションキーフレームのデフォルト値
const float PLAYER_LIFE = 10;							//プレイヤーの初期体力
const float PLAYER_INERTIA = 10.5f;						//プレイヤーの慣性力
const float DEFAULT_SIZE = 1.0f;						//初期サイズ
const float MAX_SCALE = 40.f;							//最大サイズ
const float SIZE_PLUS = 30.0f;							//当たり判定に掛けるサイズ

//--------------------------------------
//コンストラクタ
//--------------------------------------
CPlayer::CPlayer()
{
	for (int i = 0; i < 4; i++)
	{
		m_pParts[i] = nullptr;	//パーツのクリアリング
	}
	m_motionType = TYPE_NONE;	//モーションタイプのクリアリング

	m_nComboCounter = 0;		//コンボキルカウンタの初期化

	m_bDenger = false;
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CPlayer::~CPlayer()
{
}

//--------------------------------------
//クリエイト
//--------------------------------------
CPlayer * CPlayer::Create(D3DXVECTOR3 pos)
{
	//動的確保
	CPlayer* pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{//ポインタに値が入っていたら
		pPlayer->Init();									//プレイヤーの初期化
		pPlayer->SetPos(pos);								//位置を設定
		pPlayer->SetObjectType(CObject::OBJTYPE_PLAYER);	//オブジェクトタイプの設定
	}

	//値を返却
	return pPlayer;
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CPlayer::Init()
{
	//変数の初期化
	m_fMotionCounter = 0.0f;												//モーションカウンターの初期化
	m_nNumKey = 0;															//キー番号の初期化
	m_nComboCounter = 0;													//コンボカウンターを初期化
	m_motionType = TYPE_IDLE;												//モーションタイプの初期化
	m_nLife = PLAYER_LIFE;													//プレイヤーのライフの初期化
	m_fInertia = PLAYER_INERTIA;											//慣性力の初期化
	m_fAttack = 1.f;														//火力の初期化
	m_Scale = D3DXVECTOR3(DEFAULT_SIZE, DEFAULT_SIZE, DEFAULT_SIZE);		//スケールを初期化

	//モデルの生成
	m_pParts[0] = CModel::Create("Data/Model/PlayerModel/Down_Head.x",D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CModel::TYPE_PLAYER);
	m_pParts[1] = CModel::Create("Data/Model/PlayerModel/Up_Head.x", D3DXVECTOR3(0.0f, 19.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CModel::TYPE_PLAYER);

	for (int nPartsCount = 0; nPartsCount < PARTS_MAX; nPartsCount++)
	{//	パーツの初期化
		if (nPartsCount == 0)
		{//パーツが0番目だったとき(親だったとき)
			continue;		//forを戻す
		}

		//親子付けにする
		m_pParts[nPartsCount]->SetParent(m_pParts[0]);
	}

	for (int nKeyCount = 0; nKeyCount < KEY_MAX; nKeyCount++)
	{// フレームの初期化
		m_MotionInfo.aKeyInfo[nKeyCount].fFrame = DEFAULT_MOTION_FRAME;
	}
	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CPlayer::Uninit()
{
	//オブジェクトの開放
	CObject::Release();
}

//--------------------------------------
//更新
//--------------------------------------
void CPlayer::Update()
{
	m_posOld = m_pos;					//古い位置に現在の位置を設定

	m_motionType = TYPE_IDLE;			//モードの更新

	Command();							//プレイヤーの操作系統

	PlayMotion();						//モーションを動かす

	MotionManager(m_motionType);		//モーションの設定

	CollisionManager();					//当たり判定を呼び出す

	DethPlayer(m_nLife);				//プレイヤーの死亡時の処理

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_U))
	{//Uキーが押されたとき
		UpScale(0.1f);	//スケールUP！
	}

#ifdef _DEBUG
	
	
	
#endif // _DEBUG

}

//--------------------------------------
//描画
//--------------------------------------
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();		//デバイスのポインタ

	D3DXMATRIX mtxRot, mtxTrans,mtxScale;										//計算用マトリックス

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

	for (int i = 0; i < 2; i++)
	{//パーツ分描画する
		m_pParts[i]->Draw();
	}
}

//--------------------------------------
//再生モーション
//--------------------------------------
void CPlayer::PlayMotion()
{
	D3DXVECTOR3 DestPos[PARTS_MAX];			//モーションの位置の差分を入れる変数
	D3DXVECTOR3 DestRot[PARTS_MAX];			//モーションの角度の差分を入れる変数
	D3DXVECTOR3 AuthorPos[PARTS_MAX];		//モーションの位置の計算結果を入れる変数
	D3DXVECTOR3 AuthorRot[PARTS_MAX];		//モーションの角度の計算結果を入れる変数

	if (m_nNumKey + 1 >= KEY_MAX)
	{//キーの最大値をこえたら
		m_nNumKey = 0;						//キーの初期化
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
	{//モーションカウンターが想定のフレーム値以上になったら
		m_fMotionCounter = 0.0f;					//モーションカウンターを初期化する
		m_nNumKey++;								//キー番号を進める
	}

	if (m_nNumKey >= KEY_MAX)
	{//キー番号の値が最大値をこえたとき

		/*最後のキーから最後のキーの差分を求める*/

		//位置の差分を求める
		DestPos[KEY_MAX].x = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[KEY_MAX].fPosX - m_MotionInfo.aKeyInfo[0].aKeyData[0].fPosX;
		DestPos[KEY_MAX].y = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[KEY_MAX].fPosY - m_MotionInfo.aKeyInfo[0].aKeyData[0].fPosY;
		DestPos[KEY_MAX].z = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[KEY_MAX].fPosZ - m_MotionInfo.aKeyInfo[0].aKeyData[0].fPosZ;

		//角度の差分を求める
		DestRot[KEY_MAX].x = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[KEY_MAX].fRotX - m_MotionInfo.aKeyInfo[0].aKeyData[0].fRotX;
		DestRot[KEY_MAX].y = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[KEY_MAX].fRotY - m_MotionInfo.aKeyInfo[0].aKeyData[0].fRotY;
		DestRot[KEY_MAX].z = m_MotionInfo.aKeyInfo[m_nNumKey].aKeyData[KEY_MAX].fRotZ - m_MotionInfo.aKeyInfo[0].aKeyData[0].fRotZ;

		//キーの加算された値を初期化する
		m_nNumKey = 0;
	}
}

//--------------------------------------
//ライフの加算
//--------------------------------------
void CPlayer::AddLife(int nAdd)
{
	if (m_nLife <= 99)
	{//ライフが99以下なら
		m_nLife += nAdd;		//ライフを加算する
	}
	
}

//--------------------------------------
//ライフの減算
//--------------------------------------
void CPlayer::SubLife(int nSub)
{
	if (m_nLife >= 0)
	{//ライフが0以上だったら
		m_nLife -= nSub;		//ライフを減算する
	}
}

//--------------------------------------
//スケールアップ
//--------------------------------------
void CPlayer::UpScale(float fScale)
{
	if (m_Scale.x <= MAX_SCALE &&
		m_Scale.y <= MAX_SCALE &&
		m_Scale.z <= MAX_SCALE)
	{//上記のスケール以下なら各スケールに加算
		m_Scale.x += fScale;
		m_Scale.y += fScale;
		m_Scale.z += fScale;
	}
}

//--------------------------------------
//スケールの設定
//--------------------------------------
void CPlayer::SetScale(float fScale)
{//各値に設定
	m_Scale.x = fScale;
	m_Scale.y = fScale;
	m_Scale.z = fScale;
}

//--------------------------------------
//コンボをリセットする関数
//--------------------------------------
void CPlayer::ResetCombo()
{
	if (m_nComboCounter > 0)
	{
		m_nComboCounter = 0;								//エネミーのスコアに関連するコンボカウンターをリセット
		CGame::GetScore()->AddCombo(m_nComboCounter);	//UI上のコンボカウンターをリセット
	}
}

//--------------------------------------
//スケールの設定
//--------------------------------------
void CPlayer::DethPlayer(int nLifeChacker)
{
	if (!m_bDenger &&
		nLifeChacker == 3)
	{
		CApplication::GetSound()->Play(CSound::SOUND_SE_LIFE_WARNING);
		m_bDenger = true;
	}

	if (nLifeChacker <= 0)
	{//ライフが0より大きかったら
		Uninit();													//終了処理
		CRanking::Set(CScore::Get());								//ランキングを設定
		CResult::Set(1);											//リザルト画面の設定(0が通常、それ以外は死亡した状態に設定する。)
		CApplication::SetMode(CApplication::MODE::MODE_RESULT);		//モードを設定する
		return;														//更新にこれ以上入らないように返す
	}
}

//--------------------------------------
//更新の関数分け
//プレイヤーの操作系統
//--------------------------------------
void CPlayer::Command()
{
	//カメラ情報取得
	CCamera* pCam = CGame::GetCamera();

	//カメラの角度情報を代入
	D3DXVECTOR3 camrot = pCam->GetRot();

/*移動系統*/
		if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_W))
		{//該当するキーが押されたとき

			m_move.x = sinf(camrot.y - D3DX_PI * 0.0f) * m_fInertia;		//プレイヤーの位置をX方向に動かす
			m_move.z = cosf(camrot.y - D3DX_PI * 0.0f) * m_fInertia;		//プレイヤーの位置をZ方向に動かす

			m_rotDest.y = camrot.y - D3DX_PI * 1.0f;						//プレイヤーの移動中の向きを設定

			m_motionType = TYPE_WARK;										//使用するモーションの設定

		}

		if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_S))
		{//該当するキーが押されたとき

			m_move.x = sinf(camrot.y - D3DX_PI * 1.0f) * m_fInertia;		//プレイヤーの位置をX方向に動かす
			m_move.z = cosf(camrot.y - D3DX_PI * 1.0f) * m_fInertia;		//プレイヤーの位置をZ方向に動かす

			m_rotDest.y = camrot.y + D3DX_PI * 0.0f;						//プレイヤーの移動中の向きを設定

			m_motionType = TYPE_WARK;										//使用するモーションの設定
		}

		if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_A))
		{//該当するキーが押されたとき

			m_move.x = -sinf(camrot.y + D3DX_PI * 0.5f) * m_fInertia;		//プレイヤーの位置をX方向に動かす
			m_move.z = -cosf(camrot.y + D3DX_PI * 0.5f) * m_fInertia;		//プレイヤーの位置をZ方向に動かす

			m_rotDest.y = camrot.y + D3DX_PI * 0.5f;						//プレイヤーの移動中の向きを設定

			m_motionType = TYPE_WARK;										//使用するモーションの設定
		}
		
		if (CApplication::GetKyeboard()->GetkeyboardPress(DIK_D))
		{//該当するキーが押されたとき

			m_move.x = sinf(camrot.y + D3DX_PI * 0.5f) * m_fInertia;		//プレイヤーの位置をX方向に動かす
			m_move.z = cosf(camrot.y + D3DX_PI * 0.5f) * m_fInertia;		//プレイヤーの位置をZ方向に動かす

			m_rotDest.y = camrot.y - D3DX_PI * 0.5f;						//プレイヤーの移動中の向きを設定

			m_motionType = TYPE_WARK;										//使用するモーションの設定
		}

/*移動位置の代入と移動量の更新*/
		MoveCalculation();				//移動量の計算用関数を呼び出す

/*角度の計算と正規化*/
		NormalizRotation();				//角度の正規化用関数を呼び出す
}

//--------------------------------------
//移動量の計算
//--------------------------------------
void CPlayer::MoveCalculation()
{
	//移動量を位置に代入
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	//移動量を更新
	m_move.x += (0.0f - m_move.x) * 0.6f;
	m_move.z += (0.0f - m_move.z) * 0.6f;
}

//--------------------------------------
//角度の正規化
//--------------------------------------
void CPlayer::NormalizRotation()
{
	//元の角度の正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{//元の回転角から目標の回転角との差が180度より大きくなった時
		m_rotDest.y -= D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{//元の回転角から目標の回転角との差が-180度よりも小さくなった時
		m_rotDest.y += D3DX_PI * 2.0f;
	}

	//現在の角度の正規化
	if (m_rot.y > D3DX_PI)
	{//現在の回転角が180度より大きくなった時
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{//現在の回転角が-180度よりも小さくなった時
		m_rot.y += D3DX_PI * 2.0f;
	}

	//各目標の角度に元の角度を加算する
	m_rot.x += (m_rotDest.x - m_rot.x) *  0.1f;
	m_rot.y += (m_rotDest.y - m_rot.y) *  0.1f;
	m_rot.z += (m_rotDest.z - m_rot.z) *  0.1f;
}

//--------------------------------------
//モーションタイプの設定
//--------------------------------------
void CPlayer::MotionManager(MOTION_TYPE type)
{
	m_motionType = type;		//モーションのタイプを代入

	switch (m_motionType)
	{
	case CPlayer::TYPE_IDLE:

		//0番目のモーション設定
		m_MotionInfo.aKeyInfo[0].aKeyData[0].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[0].aKeyData[1].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[0].fFrame = DEFAULT_MOTION_FRAME;

		//1番目のモーション設定
		m_MotionInfo.aKeyInfo[1].aKeyData[0].fPosX = 0.0f;
		m_MotionInfo.aKeyInfo[1].aKeyData[0].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[1].aKeyData[1].fRotX = D3DXToRadian(10);
		m_MotionInfo.aKeyInfo[1].fFrame = DEFAULT_MOTION_FRAME;

		//2番目のモーション設定
		m_MotionInfo.aKeyInfo[2].aKeyData[0].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[2].aKeyData[2].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[2].fFrame = DEFAULT_MOTION_FRAME;
		
		break;

	case CPlayer::TYPE_WARK:

		//0番目のモーション設定
		m_MotionInfo.aKeyInfo[0].aKeyData[0].fPosX = 0.0f;
		m_MotionInfo.aKeyInfo[0].aKeyData[1].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[0].fFrame = 30.0f;

		//1番目のモーション設定
		m_MotionInfo.aKeyInfo[1].aKeyData[0].fPosX = 0.0f;
		m_MotionInfo.aKeyInfo[1].aKeyData[0].fRotX = -0.70f;
		m_MotionInfo.aKeyInfo[1].aKeyData[1].fRotX = D3DXToRadian(92);
		m_MotionInfo.aKeyInfo[1].fFrame = 10.0f;

		//2番目のモーション設定
		m_MotionInfo.aKeyInfo[2].aKeyData[0].fPosX = 0.0f;
		m_MotionInfo.aKeyInfo[2].aKeyData[2].fRotX = 0.0f;
		m_MotionInfo.aKeyInfo[2].fFrame = 20.0f;

		break;

	default:		//それ以外
		assert(false);					//プログラムを止めまする
		break;
	}
}

//--------------------------------------
//各所当たり判定
//--------------------------------------
void CPlayer::CollisionManager()
{
	for (int i = 0; i < MAX_OBJ; i++)
	{//オブジェクトの総量分カウントする
		CObject *pObj = CObject::Getobject(i);		//オブジェクトの取得
		CObject::ObjType objType;					//オブジェクトタイプを宣言

		if (pObj == nullptr)
		{//オブジェクトが使用されていなかったとき
			continue;						//forを最初に返す
		}

		//オブジェクトタイプ変数にオブジェクトタイプを入れる
		objType = pObj->GetObjectType();

		/*オブジェクトタイプがエネミーで当たり判定の返り値が真だったら*/
		if (objType == OBJTYPE_ENEMY_NORMAL)
		{
			CEnemy* pEnemy = (CEnemy*)pObj;			//pObjectをCEnemy型にダウンキャスト

			if (CHelperFunction::CircleCollision(m_pos, m_Scale.y * 20.0f, pEnemy->GetPos(), 20.0f) &&
				m_Scale.y >= pEnemy->GetScale().y)
			{//当たり判定の返り値が真かつプレイヤーのスケールがエネミーのスケール以上なら
				m_pos = m_posOld;								//位置を過去の位置に設定
				m_nComboCounter++;								//エネミーのスコアに関連するコンボカウンターを加算
				pEnemy->HitNormal(m_nComboCounter);					//エネミーの当たり判定処理を召喚する
				CGame::GetScore()->AddCombo(m_nComboCounter);	//UI上のコンボカウンターを加算
			}
		}

		if (objType == OBJTYPE_ENEMY_ATTACKER)
		{
			CEnemy* pEnemy = (CEnemy*)pObj;			//pObjectをCEnemy型にダウンキャスト

			if (CHelperFunction::CircleCollision(m_pos, m_Scale.y * 20.0f, pEnemy->GetPos(), 20.0f))
			{//当たり判定の返り値が真
				m_pos = m_posOld;						//位置を過去の位置に設定
				m_nComboCounter++;						//エネミーのスコアに関連するコンボカウンターを加算
				pEnemy->HitAttacker(m_nComboCounter);					//アタッカーの当たり判定処理を召喚する
				CGame::GetScore()->AddCombo(m_nComboCounter);	//UI上のコンボカウンターを加算
			}
		}

		if (objType == OBJTYPE_ENEMY_SPEEDER)
		{
			CEnemy* pEnemy = (CEnemy*)pObj;			//pObjectをCEnemy型にダウンキャスト

			if (CHelperFunction::CircleCollision(m_pos, m_Scale.y * 20.0f, pEnemy->GetPos(), 20.0f) &&
				m_Scale.y >= pEnemy->GetScale().y)
			{//当たり判定の返り値が真かつプレイヤーのスケールがエネミーのスケール以上なら
				m_pos = m_posOld;						//位置を過去の位置に設定
				m_nComboCounter++;						//エネミーのスコアに関連するコンボカウンターを加算
				pEnemy->HitSpeeder(m_nComboCounter);					//スピーダーの当たり判定処理を召喚する
				CGame::GetScore()->AddCombo(m_nComboCounter);	//UI上のコンボカウンターを加算
			}
		}

		if (objType == OBJTYPE_ENEMY_DOCTOR)
		{
			CEnemy* pEnemy = (CEnemy*)pObj;			//pObjectをCEnemy型にダウンキャスト

			if (CHelperFunction::CircleCollision(m_pos, m_Scale.y * 20.0f, pEnemy->GetPos(), 20.0f) &&
				m_Scale.y >= pEnemy->GetScale().y)
			{//当たり判定の返り値が真かつプレイヤーのスケールがエネミーのスケール以上なら
				m_pos = m_posOld;						//位置を過去の位置に設定
				m_nComboCounter++;								//エネミーのスコアに関連するコンボカウンターを加算
				pEnemy->HitCure(m_nComboCounter);					//ドクターの当たり判定処理を召喚する
				CGame::GetScore()->AddCombo(m_nComboCounter);	//UI上のコンボカウンターを加算
			}
		}

		if (objType == OBJTYPE_ENEMY_MULTI)
		{
			CEnemy* pEnemy = (CEnemy*)pObj;			//pObjectをCEnemy型にダウンキャスト

			if (CHelperFunction::CircleCollision(m_pos, m_Scale.y * 20.0f, pEnemy->GetPos(), 20.0f) &&
				m_Scale.y >= pEnemy->GetScale().y)
			{//当たり判定の返り値が真かつプレイヤーのスケールがエネミーのスケール以上なら
				m_pos = m_posOld;						//位置を過去の位置に設定
				m_nComboCounter++;								//エネミーのスコアに関連するコンボカウンターを加算
				pEnemy->HitMulti(m_nComboCounter);						//複合型の当たり判定処理を召喚する
				CGame::GetScore()->AddCombo(m_nComboCounter);	//UI上のコンボカウンターを加算
			}
		}

		if (objType == OBJTYPE_ENEMY_BOSS)
		{
			CEnemy* pEnemy = (CEnemy*)pObj;			//pObjectをCEnemy型にダウンキャスト

			if (CHelperFunction::CircleCollision(m_pos, m_Scale.y * 20.0f, pEnemy->GetPos(), 20.0f) &&
				m_Scale.y >= pEnemy->GetScale().y)
			{//当たり判定の返り値が真かつプレイヤーのスケールがエネミーのスケール以上なら
				m_pos = m_posOld;						//位置を過去の位置に設定
				m_nComboCounter++;								//エネミーのスコアに関連するコンボカウンターを加算
				pEnemy->HitBoss(m_nComboCounter);				//ボスの当たり判定処理を召喚する
				CGame::GetScore()->AddCombo(m_nComboCounter);	//UI上のコンボカウンターを加算
			}
		}

		/*オブジェクトタイプが建物で当たり判定の返り値が真だったら*/
		if ((objType == OBJTYPE_BILDING_HELLOWARK ||							
			objType == OBJTYPE_BILDING_LANDMARKTOWER ||						
			objType == OBJTYPE_BILDING_THUMBSUP ||							
			objType == OBJTYPE_BILDING_DTHUMBSUP ||							
			objType == OBJTYPE_BILDING_TEMPLE ||								
			objType == OBJTYPE_BILDING_APOSTLE) &&								
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(SIZE_PLUS, 0.0f, SIZE_PLUS), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定
		}

		//if (objType == OBJTYPE_SPOWN_BILDING &&
		//	CHelperFunction::CircleCollision(m_pos, m_Scale.y * SIZE_PLUS, pObj->Getpos(), SIZE_PLUS))
		//{//オブジェクトタイプが出現する建物かつ当たり判定処理が真なら
		//	m_pos = m_posOld;						//位置を過去の位置に設定
		//}

		if (objType == OBJTYPE_WALL_OUTSIDE_WIDE &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(10060.0f, 0.0f, 50.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定
		}

		if (objType == OBJTYPE_WALL_OUTSIDE_HIGHT &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(52.0f, 0.0f, 10060.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定
		}

		if (objType == OBJTYPE_WALL_WIDE &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(800.0f, 0.0f, 60.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定

		}

		if (objType == OBJTYPE_WALL_HIGHT &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(60.0f, 0.0f, 800.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定

		}

		if (objType == OBJTYPE_WALL_LONGLONG_WILD &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(2600.0f, 0.0f, 60.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定

		}

		if (objType == OBJTYPE_WALL_LONGLONG_HIGHT &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(60.0f, 0.0f, 2600.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定
		}

		if (objType == OBJTYPE_FENCE_WIDE &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(460.0f, 0.0f, 30.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定
		}

		if (objType == OBJTYPE_FENCE_HIGHT &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(30.0f, 0.0f, 460.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定
		}

		if (objType == OBJTYPE_EAT_DESCRIPTION &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(2000.0f, 0.0f, 30.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			//既存のサウンドを停止
			CApplication::GetSound()->Stop();
			
			m_pos = m_posOld;						//位置を過去の位置に設定
			CGame::GetEatDescription()->HitEatDescription();
			CGame::GetScore()->SetStartTimer(true);
			
			//サウンドの再生
			CApplication::GetSound()->Play(CSound::SOUND_BGM_GAME);
		}

		if (objType == OBJTYPE_DESCRIPTION &&
			CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(460.0f, 0.0f, 30.0f), m_posOld))
		{//オブジェクトタイプが建物かつ当たり判定処理が真なら
			m_pos = m_posOld;						//位置を過去の位置に設定
			CGame::GetEatDescription()->HitDescription();
		}

	}
}
