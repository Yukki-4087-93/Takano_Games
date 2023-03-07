////-----------------------------------------------------------------------------------------------------------
////
////影
////Author;takano
////
////-----------------------------------------------------------------------------------------------------------
//
////--------------------------
////インクルードファイル
////--------------------------
//#include "application.h"
//#include "renderer.h"
//#include "shadow.h"
//
////--------------------------
////定数定義
////--------------------------
//const int MAX_SHADOW = 128;								//影の最大数
//
//CShadow::CShadow()
//{
//}
//
//CShadow::~CShadow()
//{
//}
//
////-------------------------
////影の初期化処理
////-------------------------
//HRESULT CShadow::Init()
//{
//	CObject3D::Init();
//
//	return S_OK;
//}
//
////---------------------
////影の終了処理
////---------------------
//void CShadow::Uninit()
//{
//	CObject3D::Uninit();
//}
//
////----------------
////影の更新処理
////----------------
//void CShadow::Update()
//{
//	
//}
//
////--------------------------------
////影の描画処理
////--------------------------------
//void CShadow::Draw()
//{
//	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得
//	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;				//計算用メイトリックス
//
//	//Aブレンディングを減算合成に設定
//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
//
//	//テクスチャの設定
//	pDevice->SetTexture(0, nullptr);
//
//	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
//	{
//		//ワールドマトリックスの初期化
//		D3DXMatrixIdentity(&mtxWorld);
//
//		//向きを反映
//		D3DXMatrixRotationYawPitchRoll(&mtxRot, s_aShadow[nCnt].rot.y, s_aShadow[nCnt].rot.x, s_aShadow[nCnt].rot.z);
//
//		D3DXMatrixMultiply(&s_aShadow[nCnt].mtxWorld, &s_aShadow[nCnt].mtxWorld, &mtxRot);
//
//		//位置を反映
//		D3DXMatrixTranslation(&mtxTrans, s_aShadow[nCnt].pos.x, s_aShadow[nCnt].pos.y, s_aShadow[nCnt].pos.z);
//
//		D3DXMatrixMultiply(&s_aShadow[nCnt].mtxWorld, &s_aShadow[nCnt].mtxWorld, &mtxTrans);
//
//
//		//ワールドマトリックスに設定
//		pDevice->SetTransform(D3DTS_WORLD, &s_aShadow[nCnt].mtxWorld);
//
//		//頂点バッファをデータストリームに設定
//		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
//
//		//頂点フォーマット
//		pDevice->SetFVF(VERTEX_3D);
//
//		//ポリゴンの描画
//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
//			nCnt * 4,							//描画する最初の頂点インデックス
//			2);										//ポリゴン数
//	}
//
//	//Aブレンディングを元に戻す
//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//	pDevice->SetTexture(0, NULL);
//}
//
////-------------------
////影の設定処理
////-------------------
//int CShadow::Set(D3DXVECTOR3 pos, float fRadius)
//{
//
//}