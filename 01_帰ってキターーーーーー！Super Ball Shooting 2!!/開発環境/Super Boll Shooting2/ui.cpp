//--------------------------------------------------------------------------------------
//
// User Interface
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "ui.h"
#include "application.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "ranking.h"
#include "score.h"
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
	CUi *m_pUi = new CUi;		//プレイヤーの動的確保

	if (m_pUi != nullptr)
	{//プレイヤーのポインターに値が入っていなかったら
		m_pUi->Init();								//初期化
		m_pUi->SetObjectType(OBJTYPE_UI);			//オブジェクトの設定
		m_pUi->Setpos(pos);							//位置設定
		m_pUi->SetSize(ObfHi, ObfWd);				//サイズの設定
		m_pUi->SetTexture(tex);
	}
	else
	{
		assert(false);
	}

	return m_pUi;
}

void CUi::Uninit()
{
	CObject2D::Uninit();
}
