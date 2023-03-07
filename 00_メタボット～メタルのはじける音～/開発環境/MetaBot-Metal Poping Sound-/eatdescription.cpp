//--------------------------------------------------------------------------------------
//
//食べられるお品書き
//Author;takano
//
//--------------------------------------------------------------------------------------

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include "application.h"
#include "eatdescription.h"
#include "game.h"
#include "effect.h"
#include "player.h"
#include "sound.h"
#include <time.h>

//----------------------------------------
//コンストラクタ
//----------------------------------------
CEatDescription::CEatDescription()
{
	m_HitCount = 0;
}

//----------------------------------------
//デストラクタ
//----------------------------------------
CEatDescription::~CEatDescription()
{
}

//----------------------------------------
//初期化
//----------------------------------------
HRESULT CEatDescription::Init()
{
	CObject3D::Init();

	m_HitCount = 0;									//カウンターを初期化

	srand((unsigned int)time(nullptr));				//ランダムの初期化

	return S_OK;
}

//----------------------------------------
//終了
//----------------------------------------
void CEatDescription::Uninit()
{
	CObject3D::Uninit();
}

//----------------------------------------
//更新
//----------------------------------------
void CEatDescription::Update()
{
	CObject3D::Update();
}

//----------------------------------------
//描画
//----------------------------------------
void CEatDescription::Draw()
{
	CObject3D::Draw();
}

//----------------------------------------
//お品書きが食べられた時
//----------------------------------------
void CEatDescription::HitDescription()
{
	m_HitCount++;	//ヒットカウンタを加算

	CPlayer* pPlayer = CGame::GetPlayer();	//プレイヤー情報の取得

											//出現するエフェクトの設定
	CEffect::Spawn(D3DXVECTOR3(pPlayer->GetPos().x,
		pPlayer->GetPos().y,
		pPlayer->GetPos().z),		//位置の設定
		10,														//エフェクトの最大数
		20.f,													//ランダムの最大数
		-20.f,													//ランダムの最小数
		D3DXCOLOR(1.0f, 1.f, 0.f, 1.0f),						//色の設定
		30.0f * pPlayer->GetScale().y,							//エフェクトサイズの設定
		30.0f * pPlayer->GetScale().y);							//エフェクトサイズの設定

	if (m_HitCount == 20)
	{//カウンターが60フレームを超えたとき
		CApplication::GetSound()->Play(CSound::SOUND_SE_EAT);		//食したときのSEを再生
	}

	if (m_HitCount > 60)
	{//カウンターが60フレームを超えたとき
		ChangeSE();					//CVを変える
		m_HitCount = 0;				//カウンターを初期化する
	}
}

//----------------------------------------
//お品書きが食べられた時
//----------------------------------------
void CEatDescription::HitEatDescription()
{
	CPlayer* pPlayer = CGame::GetPlayer();

	CEffect::Spawn(D3DXVECTOR3(pPlayer->GetPos().x,
		pPlayer->GetPos().y,
		pPlayer->GetPos().z),		//位置の設定
		10,														//エフェクトの最大数
		20.f,													//ランダムの最大数
		-20.f,													//ランダムの最小数
		D3DXCOLOR(1.0f, 1.f, 0.f, 1.0f),						//色の設定
		30.0f * pPlayer->GetScale().y,
		30.0f * pPlayer->GetScale().y);

	CApplication::GetSound()->Play(CSound::SOUND_SE_EAT);		//食したSEを再生

	ChangeSE();					//CVを変える

	//オブジェクトの開放
	Release();
}

//----------------------------------------
//食べられるお品書きの生成
//----------------------------------------
CEatDescription * CEatDescription::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fHi, float fWd,ObjType type,CTexture::TEXTURE tex)
{
	CEatDescription* pEatDescription = new CEatDescription;

	if (pEatDescription != nullptr)
	{
		pEatDescription->Init();					//初期化
		pEatDescription->SetSize(fHi, fWd);			//サイズの設定
		pEatDescription->SetPos(pos);				//位置の設定
		pEatDescription->SetRot(rot);				//角度の設定
		pEatDescription->SetTexture(tex);			//テクスチャの設定
		pEatDescription->SetObjectType(type);		//オブジェクトタイプを設定
	}

	//値を返還
	return pEatDescription;
}

//----------------------------------------
//食べられるお品書きのSE設定
//----------------------------------------
void CEatDescription::ChangeSE()
{
	m_nCHangeSound = rand() % 3;

	switch (m_nCHangeSound)
	{
	case 0:

		CApplication::GetSound()->Play(CSound::SOUND_SE_EAT_D_ONE);

		break;

	case 1:

		CApplication::GetSound()->Play(CSound::SOUND_SE_EAT_D_TWO);

		break;

	case 2:

		CApplication::GetSound()->Play(CSound::SOUND_SE_EAT_D_THREE);

		break;

	default:
		break;
	}
}
