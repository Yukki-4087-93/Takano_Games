//---------------------------------------------------------
//
//リザルト
//Author::TAKANO
//
//---------------------------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_

#include <d3dx9.h>

//----------------------------------------
//前方宣言
//----------------------------------------
class CBg;
class CTexture;
class CSoundManager;

//----------------------------------------
//クラス定義
//マネージャー
//----------------------------------------
class CResult
{
public:
	CResult();															//コンストラクタ
	~CResult();															//デストラクタ
	HRESULT Init();														//初期化
	void Uninit();														//終了
	void Update();														//更新
	void Draw();														//描画
	static void Set(int nCount);
	static CBg* GetBg();												//背景の情報取得
	//static CTexture* GetTexture();										//テクスチャ情報の取得

private:
	static int m_nResult;												//リザルトの結果を受け取る変数
	static CBg* m_pBg;													//背景のポインター
	//static CTexture* m_pTexture;										//テクスチャのポインター
};

#endif // !_RESULT_H_
