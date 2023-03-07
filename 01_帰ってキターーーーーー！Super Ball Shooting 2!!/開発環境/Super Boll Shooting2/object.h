//--------------------------------------------------------------------------------------
//
// オブジェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _OBJECT_H_
#define _OBJECT_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "renderer.h"
//カットandペーストでクラス指定

//--------------------------------------
//マクロ定義
//--------------------------------------
#define HI_SIZE		(100.0f)
#define WD_SIZE		(400.0f)
#define MAX_OBJ		(3280)

//----------------------------------------
//クラス定義
//オブジェクト
//----------------------------------------
class CObject
{
public:
	enum ObjType
	{//オブジェクトタイプ
		OBJTYPE_NONE = 0,		//使用していない状態
		OBJTYPE_PLAYER,			//プレイヤー
		OBJTYPE_ENEMY,				//エネミー
		OBJTYPE_BOSS,
		OBJTYPE_BULLET,				//弾
		OBJTYPE_EFFECT,				//エフェクト
		OBJTYPE_EXPLOSION,			//爆発
		OBJTYPE_BG,					//背景
		OBJTYPE_PROTECT,			//街
		OBJTYPE_LIFE,				//ライフ
		OBJTYPE_SCORE,
		OBJTYPE_UI,
		OBJTYPE_MAX				//最大
	};

	CObject();											//コンストラクタ
	virtual ~CObject();									//デストラクタ
	virtual HRESULT Init() = 0;							//初期化
	virtual void Uninit() = 0;							//終了
	virtual void Update() = 0;							//更新
	virtual void Draw() = 0;							//描画
	virtual void Setpos(D3DXVECTOR3 pos) = 0;			//位置の設定
	D3DXVECTOR3 Getpos();								//位置の取得
	static void ReleaseAll();							//全開放
	static void UpdateAll();							//全更新
	static void DrawAll();								//全描画
	void Release();										//一部開放
	CObject* Getobject(int nID);						//オブジェクトの情報取得
	void SetObjectType(ObjType objtype);				//オブジェクトの設定
	ObjType GetObjectType();							//オブジェクトタイプの取得

protected:
	static CObject *m_apObject[MAX_OBJ];				//オブジェクトクラスのポインタ
	D3DXVECTOR3 m_pos;									//位置
	int m_nID;											//オブジェクト切替用のID
	ObjType m_objType;									//オブジェクトタイプの列挙を使用するための変数
};

#endif // !_OBJECT_H_
