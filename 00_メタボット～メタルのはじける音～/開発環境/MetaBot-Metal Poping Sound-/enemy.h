//-----------------------------------------------------------------------------------------------------------
//
//エネミー
//Author;takano
//
//-----------------------------------------------------------------------------------------------------------
#ifndef _ENEMY_H_												//このマクロ定義がなされなっかたら
#define _ENEMY_H_												//二重インクルード防止のマクロ定義

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "object.h"

//----------------------------------------
//前方宣言
//----------------------------------------
class CModel;

//----------------------------------------
//マクロ定義
//----------------------------------------
#define KEY_MAX			(3)										//キーの最大数
#define PARTS_MAX		(2)										//キーの最大数
#define MOTION_KYE		(10)									//モーションキーの最大値

//----------------------------------------
//クラス定義
//プレイヤー関連
//----------------------------------------
class CEnemy : public CObject
{
public:
/*構造体*/

	//モーションの種類
	enum MOTION_TYPE
	{//モーションの設定
		TYPE_NONE = -1,						//モーションを使用していない
		TYPE_IDLE = 0,						//アイドルモーション(何も行動していないときに使用)
		TYPE_WARK,							//歩くモーション(移動のときに使用)
		TYPE_MAX							//タイプの最大値
	};

	//キー構造体
	struct KEY
	{
		float fPosX;						//位置X
		float fPosY;						//位置Y
		float fPosZ;						//位置Z
		float fRotX;						//角度X
		float fRotY;						//角度Y
		float fRotZ;						//角度Z
	};

	//キーの情報
	struct KEY_INFO
	{
		float fFrame;						//フレーム数
		KEY aKeyData[KEY_MAX];				//キーの最大数
	};

	//モーション構造体
	struct MOTION_INFO
	{
		bool bMotion;						//モーションが再生中かどうか
		int nKeymax;						//キーの最大数
		KEY_INFO aKeyInfo[MOTION_KYE];		//モーションキーの最大数
	};

/*公開メンバ関数*/
public:
	CEnemy();																	//コンストラクタ
	~CEnemy();																	//デストラクタ
	static CEnemy * Create(const char* pfileParent,const char* pfileSon,D3DXVECTOR3 pos ,ObjType type,int nLife);		//クリエイト
	HRESULT Init() override;													//初期化
	void Uninit() override;														//終了
	void Update() override;														//更新
	void Draw() override;														//描画
	D3DXVECTOR3 GetPos() { return m_pos; }										//エネミーの情報取得
	void PlayMotion();															//モーション
	void HitNormal(int nCombo);													//エネミーの当たり判定
	void HitAttacker(int nCombo);												//火力の高いエネミーへの当たり判定
	void HitSpeeder(int nCombo);												//速度の高いエネミーへの当たり判定
	void HitCure(int nCombo);													//回復させるエネミーへの当たり判定
	void HitMulti(int nCombo);													//複合能力持ちエネミーへの当たり判定
	void HitBoss(int nCombo);													//ボスクラスへの当たり判定
	void SetLife(int nLife) { m_nLife = nLife; }								//ライフの設定
	int GetLife() { return m_nLife; }											//ライフの取得
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }							//速度の設定
	float GetSpeed() { return m_fSpeed; }										//速度の取得
	void SubLife(int nSub);														//ライフの減算
	void SetScale(float fScale);												//スケールの設定
	D3DXVECTOR3 GetScale() { return m_Scale; }									//スケールの情報取得
	void SetNumber(int nNum) { m_nNumber = nNum; }								//番号の設定
	int GetNumber() { return m_nNumber; }										//番号を取得	

/*非公開メンバ関数*/
private:
	void Command();												//エネミーの操作に関連するもの用関数
	void NormalizationRotation();								//角度の正規化用関数
	void MoveCalculation();										//移動の減衰量などを管理する関数
	void Collision();											//エネミーの当たり判定に関連するものをまとめた関数
	void MotionManager(MOTION_TYPE type);						//モーションを管理する関数君
	void Chase();												//追従の設定
	void Run();													//逃げる設定
	void SelectAction();										//各種エネミーの判断設定

/*一般メンバ変数*/
private:
	CModel* m_pParts[PARTS_MAX] = {};							//エネミーのパーツのポインタ
	MOTION_TYPE m_motionType;									//モーションタイプを扱う用変数
	D3DXVECTOR3 m_posOld;										//視点
	D3DXVECTOR3 m_move;											//移動量
	D3DXVECTOR3 m_Scale;										//サイズアップ用変数
	D3DXVECTOR3 m_rotDest;										//目的の角度

	D3DXVECTOR3 m_VecPos;										//追従するための位置を求める変数
	D3DXVECTOR3 m_VecRot;										//追従するための向きを求める変数

	D3DXMATRIX m_mtxWorld;										//マトリックスワールド
	DWORD m_nNumMat;											//マテリアル情報の数
	LPD3DXBUFFER m_BuffMat;										//マテリアル情報
	LPD3DXMESH m_Mesh;											//メッシュ情報
	MOTION_INFO m_MotionInfo;									//モーションの情報
	MOTION_TYPE m_Type;											//モーションタイプ情報

	float m_fMotionCounter;										//モーションカウンター変数
	float m_fScaleSeter;										//スケールの設定変数
	float m_fSpeed;												//スピードを扱う変数
	int m_nNumModel;											//モデル数
	int m_nNumKey;												//現在のキーの番号
	int m_nRand;												//エネミーの行動に使用する変数
	int m_nRandCount;											//移動のランダム化に使用する変数
	int m_nLife;												//エネミーの生命力
	int m_HitCount;												//当たり判定で敵に当たっている時間を計る変数
	int m_nNumber;												//エネミーの各ナンバーを設定する変数
	int m_nRand_Up_Multi;										//ATTRIBUTE_MULTIで使用する変数
};

#endif // !_ENEMY_H_
