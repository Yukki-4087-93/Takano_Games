//---------------------------------------------------------
//
//タイトル
//Author::TAKANO
//
//---------------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_

#include <d3dx9.h>

//----------------------------------------
//前方宣言
//----------------------------------------
//class CBg;
class CTexture;
class CSoundManager;

//----------------------------------------
//クラス定義
//マネージャー
//----------------------------------------
class CTitle
{
public:
	CTitle();															//コンストラクタ
	~CTitle();															//デストラクタ
	HRESULT Init();														//初期化
	void Uninit();														//終了
	void Update();														//更新
	void Draw();														//描画
	//static CBg* GetBg();												//背景の情報取得
	static CTexture* GetTexture();										//テクスチャ情報の取得

private:
	//static CBg* m_pBg;													//背景のポインター
	static CTexture* m_pTexture;										//テクスチャのポインター
};

#endif // !_TITLE_H_
