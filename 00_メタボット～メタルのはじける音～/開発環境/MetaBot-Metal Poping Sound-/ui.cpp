//--------------------------------------------------------------------------------------
//
// ユーザーインターフェース。
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "ui.h"
#include "application.h"
#include "input.h"
#include "ranking.h"
#include "score.h"
#include "game.h"
#include "player.h"
#include <assert.h>
#include <stdlib.h>


//--------------------------------------
//コンストラクタ
//--------------------------------------
CUi::CUi()
{
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CUi::~CUi()
{
}

//--------------------------------------
//クリエイト
//--------------------------------------
CUi *CUi::Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd, CTexture::TEXTURE tex)
{
	CUi *m_pUi = new CUi;		//UIの動的確保

	if (m_pUi != nullptr)
	{//UIのポインターに値が入っていなかったら
		m_pUi->Init();								//初期化
		m_pUi->SetObjectType(OBJTYPE_UI);			//オブジェクトの設定
		m_pUi->SetPos(pos);							//位置設定
		m_pUi->SetSize(ObfHi, ObfWd);				//サイズの設定
		m_pUi->SetTexture(tex);
	}
	else
	{
		assert(false);
	}

	return m_pUi;
}

//--------------------------------------
//終了
//--------------------------------------
void CUi::Uninit()
{
	CObject2D::Uninit();
}
