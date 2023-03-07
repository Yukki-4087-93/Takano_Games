//--------------------------------------------------------------------------------------
//
// オブジェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _OBJECT_H_
#define _OBJECT_H_

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include <d3dx9.h>

//--------------------------------------
//定数定義
//--------------------------------------
const int MAX_OBJ = 264;

//----------------------------------------
//クラス定義
//オブジェクト
//----------------------------------------
class CObject
{
public:
	enum ObjType
	{//オブジェクトタイプ
		OBJTYPE_NONE = -1,												//使用していない状態
		OBJTYPE_PLAYER = 0,												//プレイヤー
	/*エネミーズ*/
		OBJTYPE_ENEMY_NORMAL,											//通常のエネミー
		OBJTYPE_ENEMY_ATTACKER,											//火力マシマシエネミー
		OBJTYPE_ENEMY_SPEEDER,											//速度マシマシエネミー
		OBJTYPE_ENEMY_DOCTOR,											//ドクターエネミー
		OBJTYPE_ENEMY_MULTI,											//複合持ちエネミー
		OBJTYPE_ENEMY_BOSS,												//ボスエネミー
	/*ビルダーズ*/
		OBJTYPE_BILDING_HELLOWARK,										//ハローワーク
		OBJTYPE_BILDING_LANDMARKTOWER,									//ランドマークタワー
		OBJTYPE_BILDING_THUMBSUP,										//サムズアップ
		OBJTYPE_BILDING_DTHUMBSUP,										//ダブルサムズアップ
		OBJTYPE_BILDING_TEMPLE,											//寺
		OBJTYPE_BILDING_APOSTLE,										//使徒
		OBJTYPE_SPOWN_BILDING,											//出現位置
	/*ウォールズ*/
		OBJTYPE_WALL_WIDE,												//壁
		OBJTYPE_WALL_HIGHT,												//壁
		OBJTYPE_WALL_LONGLONG_WILD,										//壁(LongVer.)
		OBJTYPE_WALL_LONGLONG_HIGHT,									//壁(LongVer.)
		OBJTYPE_WALL_OUTSIDE_WIDE,										//外壁
		OBJTYPE_WALL_OUTSIDE_HIGHT,										//外壁
	/*フェンスオブガイア*/
		OBJTYPE_FENCE_WIDE,												//フェンス
		OBJTYPE_FENCE_HIGHT,											//フェンス
	/*アウターズ*/
		OBJTYPE_MESHFIELD,												//メッシュフィールド
		OBJTYPE_SCORE,													//スコア
		OBJTYPE_EFFECT,													//エフェクト
		OBJTYPE_BG,														//背景
		OBJTYPE_UI,														//ユーザーインターフェース
		OBJTYPE_DESCRIPTION,											//食べられない説明
		OBJTYPE_EAT_DESCRIPTION,										//食べられる説明
		OBJTYPE_EAT_DESCRIPTION_DENGER,									//食べられる僕
		OBJTYPE_MAX														//最大
	};

	CObject();															//コンストラクタ
	virtual ~CObject();													//デストラクタ
	virtual HRESULT Init() = 0;											//初期化
	virtual void Uninit() = 0;											//終了
	virtual void Update() = 0;											//更新
	virtual void Draw() = 0;											//描画
	virtual void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }				//位置の設定
	D3DXVECTOR3 Getpos() { return m_pos; }								//位置の取得
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }						//角度の設定
	D3DXVECTOR3 GetRot() { return m_rot; }								//角度の位置情報の取得
	static void ReleaseAll();											//全開放
	static void UpdateAll();											//全更新
	static void DrawAll();												//全描画
	void Release();														//一部開放
	static CObject* Getobject(int nID) { return m_apObject[nID]; }		//オブジェクトの情報取得
	void SetObjectType(ObjType objtype) { m_objType = objtype; }		//オブジェクトの設定
	ObjType GetObjectType() { return m_objType; }						//オブジェクトタイプの取得

protected:
	static CObject *m_apObject[MAX_OBJ];								//オブジェクトクラスのポインタ
	D3DXVECTOR3 m_pos;													//位置
	D3DXVECTOR3 m_rot;													//角度
	int m_nID;															//オブジェクト切替用のID
	ObjType m_objType;													//オブジェクトタイプの列挙を使用するための変数
};

#endif // !_OBJECT_H_
