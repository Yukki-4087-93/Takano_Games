//--------------------------------------------------------------------------------------
//
// オブジェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "object.h"
#include "object2d.h"
#include "renderer.h"
#include "main.h"

//--------------------------------------
//静的メンバ変数の宣言
//--------------------------------------
CObject *CObject::m_apObject[MAX_OBJ] = {};

//--------------------------------------
//コンストラクタ
//--------------------------------------
CObject::CObject()
{
	for (int nCnt = 0; nCnt < MAX_OBJ; nCnt++)
	{
		if (m_apObject[nCnt] == nullptr)
		{
			m_apObject[nCnt] = this;
			m_nID = nCnt;
			break;
		}
	}
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CObject::~CObject()
{
}

//--------------------------------------
//ポス
//--------------------------------------
D3DXVECTOR3 CObject::Getpos()
{
	return m_pos;
}

//--------------------------------------
//オブジェクトの解放
//--------------------------------------
void CObject::ReleaseAll()
{
	for (int i = 0; i < MAX_OBJ; i++)
	{
		if (m_apObject[i] != nullptr)
		{
			m_apObject[i]->Release();
		}
	}
}

//--------------------------------------
//オブジェクトの更新
//--------------------------------------
void CObject::UpdateAll()
{
	for (int nCnt = 0; nCnt < MAX_OBJ; nCnt++)
	{
		if (m_apObject[nCnt] != nullptr)
		{
			m_apObject[nCnt]->Update();
		}
	}
}

//--------------------------------------
//オブジェクトの描画
//--------------------------------------
void CObject::DrawAll()
{
	for (int nCnt = 0; nCnt < MAX_OBJ; nCnt++)
	{
		if (m_apObject[nCnt] != nullptr)
		{
			m_apObject[nCnt]->Draw();
		}
	}
}

//--------------------------------------
//オブジェクトの部分解放
//--------------------------------------
void CObject::Release()
{
	if (m_apObject[m_nID] != nullptr)
	{
		const int nId = m_nID;
		delete m_apObject[nId];
		m_apObject[nId] = nullptr;
	}
}

//--------------------------------------
//オブジェクトの情報取得
//--------------------------------------
CObject * CObject::Getobject(int nID)
{
	return m_apObject[nID];
}

//--------------------------------------
//オブジェクトタイプの設定
//--------------------------------------
void CObject::SetObjectType(ObjType objtype)
{
	m_objType = objtype;
}

//--------------------------------------
//オブジェクトタイプの情報取得
//--------------------------------------
CObject::ObjType CObject::GetObjectType()
{
	return m_objType;
}
