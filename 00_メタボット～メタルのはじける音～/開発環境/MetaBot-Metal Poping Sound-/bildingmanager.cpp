//--------------------------------------------------------------------------------------
//
// 建物の管理
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "application.h"
#include "bildingmanager.h"
#include "bilding.h"
#include "helperfunction.h"
#include "game.h"
#include "player.h"
#include "input.h"
#include "object.h"
#include "camera.h"
#include <stdio.h>
#include <assert.h>

//--------------------------------------
//定数定義
//--------------------------------------
const float SMALLEST_SQUARESIZE = 100.f;
const float HIGHT_POSITION = 0.0f;
const float WALL_HIGHT = 100.f;

//--------------------------------------
//コンストラクタ
//--------------------------------------
CBildingManager::CBildingManager()
{

}

//--------------------------------------
//デストラクタ
//--------------------------------------
CBildingManager::~CBildingManager()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CBildingManager::Init()
{
	//モデルの読み込み
	Load();

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CBildingManager::Uninit()
{
	if (m_pBilding != nullptr)
	{
		m_pBilding->Uninit();
		m_pBilding = nullptr;
	}
}

//--------------------------------------
//更新
//--------------------------------------
void CBildingManager::Update()
{
	for (int i = 0; i < MAX_OBJ; i++)
	{//オブジェクトの総量分カウントする
		CObject *pObj = CObject::Getobject(i);		//オブジェクトの取得
		CObject::ObjType objType;					//オブジェクトタイプを宣言

		if (pObj == nullptr)
		{//オブジェクトが使用されていなかったとき
			continue;						//forを最初に返す
		}

		//オブジェクトタイプ変数にオブジェクトタイプを入れる
		objType = pObj->GetObjectType();

		///*オブジェクトタイプが建物で当たり判定の返り値が真だったら*/
		//if ((objType == CObject::OBJTYPE_BILDING_HELLOWARK ||
		//	objType == CObject::OBJTYPE_BILDING_LANDMARKTOWER ||
		//	objType == CObject::OBJTYPE_BILDING_THUMBSUP ||
		//	objType == CObject::OBJTYPE_BILDING_DTHUMBSUP ||
		//	objType == CObject::OBJTYPE_BILDING_TEMPLE ||
		//	objType == CObject::OBJTYPE_BILDING_APOSTLE) &&
		//	CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(SIZE_PLUS, 0.0f, SIZE_PLUS), m_posOld))
		//{//オブジェクトタイプが建物かつ当たり判定処理が真なら
		//	m_pos = m_posOld;						//位置を過去の位置に設定
		//}

		if (objType == CObject::OBJTYPE_BILDING_HELLOWARK ||
			objType == CObject::OBJTYPE_BILDING_LANDMARKTOWER ||
			objType == CObject::OBJTYPE_BILDING_THUMBSUP ||
			objType == CObject::OBJTYPE_BILDING_DTHUMBSUP ||
			objType == CObject::OBJTYPE_BILDING_TEMPLE ||
			objType == CObject::OBJTYPE_BILDING_APOSTLE ||
			objType == CObject::OBJTYPE_SPOWN_BILDING ||
			objType == CObject::OBJTYPE_WALL_LONGLONG_WILD ||
			objType == CObject::OBJTYPE_WALL_LONGLONG_HIGHT ||
			objType == CObject::OBJTYPE_WALL_OUTSIDE_WIDE ||
			objType == CObject::OBJTYPE_WALL_OUTSIDE_HIGHT)
		{
			CBilding* pBuilding = (CBilding*)pObj;
			D3DXVECTOR3 posV, posR, vec;
			D3DXMATRIX mtxWorld;
			posV = CGame::GetCamera()->GetPosV() - pBuilding->GetModelpos();
			posR = CGame::GetCamera()->GetPosR() - pBuilding->GetModelpos();
			mtxWorld = pBuilding->GetmtxWorldModel();

			//逆行列に変換
			D3DXMatrixInverse(&mtxWorld, nullptr, &mtxWorld);

			//
			D3DXVec3TransformCoord(&posR, &posR, &mtxWorld);

			//
			D3DXVec3TransformCoord(&posV, &posV, &mtxWorld);

			//posVからposRの差をベクトルに代入
			vec = posV - posR;

			//ベクトルを１にする 第一引数に結果を保存するもの、第二引数に変換したいベクトルを入れる
			D3DXVec3Normalize(&vec, &vec);
			BOOL bHit = true;
			FLOAT pfDistance = 0.0f;

			//壁とあったときの判定
			D3DXIntersect(pBuilding->GetMesh(), &posR, &vec, &bHit, nullptr, nullptr, nullptr, &pfDistance, nullptr, nullptr);

			if (pfDistance >= 0.0f)
			{
				//カラーの変更
				pBuilding->SetColor(bHit);
			}

		}

		//if (objType == OBJTYPE_SPOWN_BILDING &&
		//	CHelperFunction::CircleCollision(m_pos, m_Scale.y * SIZE_PLUS, pObj->Getpos(), SIZE_PLUS))
		//{//オブジェクトタイプが出現する建物かつ当たり判定処理が真なら
		//	m_pos = m_posOld;						//位置を過去の位置に設定
		//}

		//if (objType == CObject::OBJTYPE_WALL_OUTSIDE_WIDE &&
		//	CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(10060.0f, 0.0f, 50.0f), m_posOld))
		//{//オブジェクトタイプが建物かつ当たり判定処理が真なら
		//	m_pos = m_posOld;						//位置を過去の位置に設定
		//}

		//if (objType == CObject::OBJTYPE_WALL_OUTSIDE_HIGHT &&
		//	CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(52.0f, 0.0f, 10060.0f), m_posOld))
		//{//オブジェクトタイプが建物かつ当たり判定処理が真なら
		//	m_pos = m_posOld;						//位置を過去の位置に設定
		//}

		//if (objType == CObject::OBJTYPE_WALL_WIDE &&
		//	CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(800.0f, 0.0f, 60.0f), m_posOld))
		//{//オブジェクトタイプが建物かつ当たり判定処理が真なら
		//	m_pos = m_posOld;						//位置を過去の位置に設定

		//}

		//if (objType == CObject::OBJTYPE_WALL_HIGHT &&
		//	CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(60.0f, 0.0f, 800.0f), m_posOld))
		//{//オブジェクトタイプが建物かつ当たり判定処理が真なら
		//	m_pos = m_posOld;						//位置を過去の位置に設定

		//}

		//if (objType == CObject::OBJTYPE_WALL_LONGLONG_WILD &&
		//	CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(2600.0f, 0.0f, 60.0f), m_posOld))
		//{//オブジェクトタイプが建物かつ当たり判定処理が真なら
		//	m_pos = m_posOld;						//位置を過去の位置に設定

		//}

		//if (objType == CObject::OBJTYPE_WALL_LONGLONG_HIGHT &&
		//	CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(60.0f, 0.0f, 2600.0f), m_posOld))
		//{//オブジェクトタイプが建物かつ当たり判定処理が真なら
		//	m_pos = m_posOld;						//位置を過去の位置に設定
		//}

		//if (objType == CObject::OBJTYPE_FENCE_WIDE &&
		//	CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(460.0f, 0.0f, 30.0f), m_posOld))
		//{//オブジェクトタイプが建物かつ当たり判定処理が真なら
		//	m_pos = m_posOld;						//位置を過去の位置に設定
		//}

		//if (objType == CObject::OBJTYPE_FENCE_HIGHT &&
		//	CHelperFunction::BoxCollision(m_pos, D3DXVECTOR3(m_Scale.x * SIZE_PLUS, 0.0f, m_Scale.z * SIZE_PLUS), pObj->Getpos(), D3DXVECTOR3(30.0f, 0.0f, 460.0f), m_posOld))
		//{//オブジェクトタイプが建物かつ当たり判定処理が真なら
		//	m_pos = m_posOld;						//位置を過去の位置に設定
		//}
	}

#ifdef _DEBUG

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_F1))
	{
		if (m_pBilding != nullptr)
		{
			m_pBilding->Uninit();
			m_pBilding = nullptr;
		}
	}

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_F2))
	{
		m_pBilding = CBilding::Create("Data/Model/Fence/Fence.x",
			CGame::GetPlayer()->GetPos(),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			CObject::OBJTYPE_FENCE_WIDE);
	}

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_F3))
	{
		m_pBilding = CBilding::Create("Data/Model/Fence/Fence.x",
			CGame::GetPlayer()->GetPos(),
			D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),
			CObject::OBJTYPE_FENCE_HIGHT);
	}

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_F5))
	{
		m_pBilding = CBilding::Create("Data/Model/Building/Factory.x",
			CGame::GetPlayer()->GetPos(),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			CObject::OBJTYPE_SPOWN_BILDING);
	}

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_F6))
	{
		m_pBilding = CBilding::Create("Data/Model/Wall/WALL_LONGBELONG.x",
			CGame::GetPlayer()->GetPos(),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			CObject::OBJTYPE_WALL_LONGLONG_HIGHT);
	}

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_F7))
	{
		m_pBilding = CBilding::Create("Data/Model/Wall/WALL_LONGBELONG.x",
			CGame::GetPlayer()->GetPos(),
			D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),
			CObject::OBJTYPE_WALL_LONGLONG_WILD);
	}

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_F8))
	{
		m_pBilding = CBilding::Create("Data/Model/Wall/WALL_NOMAL.x",
			CGame::GetPlayer()->GetPos(),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			CObject::OBJTYPE_WALL_HIGHT);
	}

	if (CApplication::GetKyeboard()->GetkeyboardTrigger(DIK_SPACE))
	{
		Save();
	}

#endif // _DEBUG
}

//--------------------------------------
//描画
//--------------------------------------
void CBildingManager::Draw()
{
}

//--------------------------------------
//モデルの読み込み
//--------------------------------------
void CBildingManager::Load()
{
	//ファイルに書き込む
	FILE *pFile;        //ファイルポインタを宣言

						//ファイルを開く
	pFile = fopen(OPEN_FILE, "r");

	if (pFile != nullptr)
	{//ファイルが開けたとき
	 //ファイルに数値を書き出す

		for (int i = 0; i < MAX_OBJ; i++)
		{
			int nNumber = 0;
			D3DXVECTOR3 pos, rot;
			fscanf(pFile, "%d", &nNumber);
			fscanf(pFile, "%f,%f,%f", &pos.x, &pos.y, &pos.z);
			fscanf(pFile, "%f,%f,%f", &rot.x, &rot.y, &rot.z);

			switch (nNumber)
			{
			case 7:

				CBilding::Create("Data/Model/Building/Hello_Work_C.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_BILDING_HELLOWARK);

				break;

			case 8:

				CBilding::Create("Data/Model/Building/Landtower.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_BILDING_LANDMARKTOWER);

				break;

			case 9:

				CBilding::Create("Data/Model/Building/Thumbs_Up.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_BILDING_THUMBSUP);

				break;

			case 10:

				CBilding::Create("Data/Model/Building/DThumbs_Up.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_BILDING_DTHUMBSUP);

				break;

			case 11:

				CBilding::Create("Data/Model/Building/teratera.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_BILDING_TEMPLE);

				break;

			case 12:

				CBilding::Create("Data/Model/Building/shito.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_BILDING_APOSTLE);

				break;

			case 13:

				CBilding::Create("Data/Model/Building/Factory.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					CObject::OBJTYPE_SPOWN_BILDING);

				break;

			case 14:

				CBilding::Create("Data/Model/Wall/WALL_NOMAL.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_WALL_WIDE);

				break;

			case 15:

				CBilding::Create("Data/Model/Wall/WALL_NOMAL.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_WALL_HIGHT);

				break;

			case 16:

				CBilding::Create("Data/Model/Wall/WALL_LONGBELONG.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_WALL_LONGLONG_WILD);

				break;

			case 17:

				CBilding::Create("Data/Model/Wall/WALL_LONGBELONG.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_WALL_LONGLONG_HIGHT);

				break;

			case 18:

				CBilding::Create("Data/Model/Wall/WALL_OUTSIDER.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_WALL_OUTSIDE_WIDE);

				break;

			case 19:

				CBilding::Create("Data/Model/Wall/WALL_OUTSIDER.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_WALL_OUTSIDE_HIGHT);

				break;

			case 20:

				CBilding::Create("Data/Model/Fence/Fence.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_FENCE_WIDE);

				break;

			case 21:

				CBilding::Create("Data/Model/Fence/Fence.x",
					D3DXVECTOR3(pos.x, pos.y, pos.z),
					D3DXVECTOR3(rot.x, rot.y, rot.z),
					CObject::OBJTYPE_FENCE_HIGHT);

				break;

			default:
				break;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}

	else
	{//ファイルが開けなかったとき
		assert(false);				//プログラムを止める
	}
}

//--------------------------------------
//保存
//--------------------------------------
void CBildingManager::Save()
{
	//ファイルに書き込む
	FILE *pFile;        //ファイルポインタを宣言

						//ファイルを開く
	pFile = fopen(OPEN_FILE, "w");

	if (pFile != nullptr)
	{//ファイルが開けたとき
	 //ファイルに数値を書き出す

		for (int i = 0; i < MAX_OBJ; i++)
		{
			CObject* pObj = CObject::Getobject(i);
			CObject::ObjType objType;					//オブジェクトタイプを宣言

			if (pObj == nullptr)
			{//オブジェクトが使用されていなかったとき
				continue;						//forを最初に返す
			}

			//オブジェクトタイプ変数にオブジェクトタイプを入れる
			objType = pObj->GetObjectType();

			if (objType == CObject::OBJTYPE_BILDING_HELLOWARK ||
				objType == CObject::OBJTYPE_BILDING_LANDMARKTOWER ||
				objType == CObject::OBJTYPE_BILDING_THUMBSUP ||
				objType == CObject::OBJTYPE_BILDING_DTHUMBSUP ||
				objType == CObject::OBJTYPE_BILDING_TEMPLE ||
				objType == CObject::OBJTYPE_BILDING_APOSTLE ||
				objType == CObject::OBJTYPE_SPOWN_BILDING ||
				objType == CObject::OBJTYPE_WALL_WIDE ||
				objType == CObject::OBJTYPE_WALL_HIGHT ||
				objType == CObject::OBJTYPE_WALL_LONGLONG_WILD ||
				objType == CObject::OBJTYPE_WALL_LONGLONG_HIGHT ||
				objType == CObject::OBJTYPE_WALL_OUTSIDE_WIDE ||
				objType == CObject::OBJTYPE_WALL_OUTSIDE_HIGHT ||
				objType == CObject::OBJTYPE_FENCE_WIDE ||
				objType == CObject::OBJTYPE_FENCE_HIGHT)
			{
				fprintf(pFile, "%d\n", objType);
				fprintf(pFile, "%f,%f,%f \n", pObj->Getpos().x, pObj->Getpos().y, pObj->Getpos().z);
				fprintf(pFile, "%f,%f,%f \n\n", pObj->GetRot().x, pObj->GetRot().y, pObj->GetRot().z);
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}

	else
	{//ファイルが開けなかったとき
		assert(false);				//プログラムを止める
	}
}
