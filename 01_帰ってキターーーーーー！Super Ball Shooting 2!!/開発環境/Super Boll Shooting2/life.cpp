//------------------------------------------------------------------------------
//
//ライフ
//Author::TAKANO
//
//------------------------------------------------------------------------------
#include"life.h"

//---------------
//マクロ定義
//---------------
#define MAX_LIFE			(256)								//最大ライフ

//---------------------
//スタティック変数
//---------------------
static bool bUseLife[MAX_LIFE];
static int nCountNumber;

//--------------------------------------
//コンストラクタ
//--------------------------------------
CLife::CLife()
{

}

//--------------------------------------
//デストラクタ
//--------------------------------------
CLife::~CLife()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CLife::Init(void)
{
	//初期化
	CObject2D::Init();
	//テクスチャの設定
	CObject2D::SetTexture(CTexture::TEXTURE_PLAYER);

	return S_OK;
}


//--------------------------------------
//クリエイト
//--------------------------------------
CLife* CLife::Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd)
{
	CLife* pbg;

	pbg = new CLife;
	if (pbg != nullptr)
	{
		pbg->Init();
		pbg->SetObjectType(OBJTYPE_BG);
		pbg->Setpos(pos);
		pbg->SetSize(ObfHi, ObfWd);
	}

	return pbg;
}

//--------------------------------------
//終了
//--------------------------------------
void CLife::Uninit(void)
{
	CObject2D::Uninit();
}

//--------------------------------------
//更新
//--------------------------------------
void CLife::Update(void)
{
	CObject2D::Update();
}

//--------------------------------------
//描画
//--------------------------------------
void CLife::Draw(void)
{
	CObject2D::Draw();
}

//-----------------------------------
//ライフの配置
//-----------------------------------
void CLife::SetLife()
{
	if (nCountNumber < MAX_LIFE)
	{//MAXLIFE以上入るのを防ぐ

		bUseLife[nCountNumber] = true;		//使用する

		nCountNumber++;						//ライフのカウンターを増やす
	}
}

//-----------------------------------
//ライフのヒット処理
//-----------------------------------
void CLife::HitLife()
{
	if (nCountNumber > 0)
	{//0以下が入るのを防ぐ

		nCountNumber--;						//ライフのカウンターを減らす

		bUseLife[nCountNumber] = false;		//使用しない状態にする
	}

}