//-----------------------------------------------------------------------------------------------------------
//
//プレイヤー	
//Author;takano
//
//-----------------------------------------------------------------------------------------------------------
#ifndef _PLAYER_H_													//このマクロ定義がなされなっかたら
#define _PLAYER_H_													//二重インクルード防止のマクロ定義

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
class CPlayer : public CObject
{
public:
/*構造体*/
	//モーションの種類
	enum MOTION_TYPE
	{
		TYPE_NONE = -1,											//モーションを使用していない
		TYPE_IDLE = 0,											//アイドルモーション(何も行動していないときに使用)
		TYPE_WARK,												//歩くモーション(移動のときに使用)
		TYPE_HIGH_MEGA_CANNON,									//ハイ・メガ・キャノン!!!!
		TYPE_MAX												//タイプの最大値
	};

	//キー構造体
	struct KEY
	{
		float fPosX;											//位置X
		float fPosY;											//位置Y
		float fPosZ;											//位置Z
		float fRotX;											//角度X
		float fRotY;											//角度Y
		float fRotZ;											//角度Z
	};

	//キーの情報
	struct KEY_INFO
	{
		float fFrame;											//フレーム数
		KEY aKeyData[KEY_MAX];									//キーの最大数
	};

	//モーション構造体
	struct MOTION_INFO
	{
		bool bMotion;											//モーションが再生中かどうか
		int nKeymax;											//キーの最大数
		KEY_INFO aKeyInfo[MOTION_KYE];							//モーションキーの最大数
	};

/*メンバ関数*/
public:
	CPlayer();													//コンストラクタ
	~CPlayer();													//デストラクタ
	static CPlayer * Create(D3DXVECTOR3 pos);					//クリエイト
	HRESULT Init() override;									//初期化
	void Uninit() override;										//終了
	void Update() override;										//更新
	void Draw() override;										//描画
	D3DXVECTOR3 GetPos() { return m_pos; }						//プレイヤーの情報取得
	D3DXVECTOR3 GetPosOld() { return m_posOld; }				//プレイヤーの古い情報取得
	D3DXVECTOR3 GetMove() { return m_move; };					//プレイヤーの移動量の取得
	void PlayMotion();											//モーション
	int GetPlayerLife() { return m_nLife; }						//ライフ情報取得
	void AddLife(int nAdd);										//ライフの加算
	void SubLife(int nSub);										//ライフの減算
	void AddAttack(float nAddAtc) { m_fAttack += nAddAtc; }		//火力の加算
	void SubAttack(float nSubAtc) { m_fAttack -= nSubAtc; }		//火力の加算
	void AddInertia(float fAddIr) { m_fInertia += fAddIr; }		//速度の加算
	void SubInertia(float fSubIr) { m_fInertia += fSubIr; }		//速度の減算
	void UpScale(float fScale);									//スケールアップ関数
	void SetScale(float fScale);
	D3DXVECTOR3 GetScale() { return m_Scale; }
	int GetCombo() { return m_nComboCounter; }
	void ResetCombo();

private:
	void DethPlayer(int nLifeChacker);							//プレイヤーの死亡時の処理
	void Command();												//プレイヤーの操作に関連するもの用関数
	void MoveCalculation();										//移動量の計算
	void NormalizRotation();									//角度の正規化
	void MotionManager(MOTION_TYPE type);						//モーションを管理する関数君
	void CollisionManager();									//当たり判定に関係するものをまとめる関数

/*メンバ変数*/
private:
	CModel* m_pParts[PARTS_MAX] = {};							//プレイヤーのパーツのポインタ
	MOTION_TYPE m_motionType;									//モーションタイプを扱う用変数
	D3DXVECTOR3 m_posOld;										//過去の位置
	D3DXVECTOR3 m_rotDest;										//目的の角度
	D3DXVECTOR3 m_move;											//移動量
	D3DXVECTOR3 m_Scale;										//サイズアップ用変数
	D3DXMATRIX m_mtxWorld;										//マトリックスワールド
	DWORD m_nNumMat;											//マテリアル情報の数
	LPD3DXBUFFER m_BuffMat;										//マテリアル情報
	LPD3DXMESH m_Mesh;											//メッシュ情報
	MOTION_INFO m_MotionInfo;									//モーションの情報
	MOTION_TYPE m_Type;											//モーションタイプ情報
	float m_fMotionCounter;										//モーションカウンター変数
	int m_nNumModel;											//モデル数
	int m_nNumKey;												//現在のキーの番号

private:
	int m_nLife;												//プレイヤーの生命力
	float m_fAttack;											//プレイヤーの火力
	float m_fInertia;											//プレイヤーの完成
	int m_nComboCounter;										//撃破されてスコアを加算するときのコンボに使用する変数
	int m_nCountCombo;
	bool m_bDenger;
};

#endif // !PLAYER_H_
