//--------------------------------------------------------------------------------------
//
// オブジェクト
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "object2d.h"
#include "renderer.h"
#include "application.h"
#include "texture.h"
#include "main.h"
#include <assert.h>

//--------------------------------------
//マクロ定義
//--------------------------------------
#define HIGHT		(30.0f)
#define WIDTH		(30.0f)

//--------------------------------------
//定数定義
//--------------------------------------
// 頂点フォーマット
const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

//--------------------------------------
//コンストラクタ
//--------------------------------------
CObject2D::CObject2D()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_texture = CTexture::TEXTURE_NONE;
	m_Change_Height = 0.0f;
	m_Change_Width = 0.0f;
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CObject2D::~CObject2D()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CObject2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_Vertex, NULL);

	VERTEX_2D * pVtx;

	m_Change_Height = HIGHT;			//高さの設定
	m_Change_Width = WIDTH;				//幅の設定

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_Vertex->Lock(0, 0, (void**)&pVtx, 0);

	//対角線の長さを算出する
	m_fLength = sqrtf(m_Change_Height * m_Change_Height + m_Change_Width * m_Change_Width) / 2.0f;

	//対角線の角度を算出する
	m_fAngle = atan2f(m_Change_Height, m_Change_Width);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + (m_fAngle * -1.0f)) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + (m_fAngle * -1.0f)) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + (m_fAngle * 1.0f)) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + (m_fAngle * 1.0f)) * m_fLength;
	pVtx[3].pos.z = 0.0f;
	
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//テクスチャを使用していない状態にする
	m_texture = CTexture::TEXTURE_NONE;

	//頂点バッファをアンロックする
	m_Vertex->Unlock();

	return S_OK;
}

//--------------------------------------
//クリエイト
//--------------------------------------
CObject2D *CObject2D::Create(D3DXVECTOR3 pos, float ObfHi, float ObfWd)
{
	CObject2D* pObject2D = new CObject2D;		//オブジェクトの動的確保

	if (pObject2D != nullptr)
	{
		pObject2D->Init();						//初期化
		pObject2D->SetPos(pos);					//位置の設定
		pObject2D->SetSize(ObfHi, ObfWd);		//サイズの設定
	}
	else
	{//それ以外
		assert(false);							//プログラムを止めます
	}

	//値を返還する
	return pObject2D;
}

//--------------------------------------
//更新
//--------------------------------------
void CObject2D::Update()
{
	VERTEX_2D* pVtx;			//頂点情報へのポインタ

	//頂点バッファをコックし、頂点情報へのポインタを取得
	m_Vertex->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + (m_fAngle * -1.0f)) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + (m_fAngle * -1.0f)) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + (m_fAngle * 1.0f)) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + (m_fAngle * 1.0f)) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_Vertex->Unlock();
}

//--------------------------------------
//描画
//--------------------------------------
void CObject2D::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//テクスチャの取得
	CTexture* pTexture = CApplication::GetTexture();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_Vertex, 0, sizeof(VERTEX_2D));

	//アルファテストの有効化
	//映らない白ポリゴンを透明化できる。
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 80);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	//ポリゴンの描画
	pDevice->DrawPrimitive
	(D3DPT_TRIANGLESTRIP,
		0,
		2);

	// アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------
//終了
//--------------------------------------
void CObject2D::Uninit()
{
	if (m_Vertex != nullptr)
	{//頂点情報に値が入っていたら
		m_Vertex->Release();			//頂点情報の開放
		m_Vertex = nullptr;				//ヌルポを代入
	}

	//オブジェクトの開放
	CObject::Release();
}

//--------------------------------------
//ポスのセット
//--------------------------------------
void CObject2D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;			//位置に取得した位置を代入

	VERTEX_2D * pVtx;		//頂点情報のポインターを取得

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_Vertex->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos.x = pos.x + sinf(m_rot.z + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.y = pos.y + cosf(m_rot.z + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.y = pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(m_rot.z + (m_fAngle * -1.0f)) * m_fLength;
	pVtx[2].pos.y = pos.y + cosf(m_rot.z + (m_fAngle * -1.0f)) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(m_rot.z + (m_fAngle * 1.0f)) * m_fLength;
	pVtx[3].pos.y = pos.y + cosf(m_rot.z + (m_fAngle * 1.0f)) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_Vertex->Unlock();
}

//--------------------------------------
//テクスチャアニメーションの設定
//--------------------------------------
void CObject2D::PattarnTexture(int nPattarn, int nSplit)
{
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_Vertex->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャパターンを制御する処理
	nPattarn = (nPattarn) % nSplit;

	//テキスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(nPattarn * 1.0f / nSplit, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nPattarn * 1.0f / nSplit + 1.0f / nSplit, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nPattarn * 1.0f / nSplit, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nPattarn * 1.0f / nSplit + 1.0f / nSplit, 1.0f);

	//頂点バッファをアンロックする
	m_Vertex->Unlock();
}

//--------------------------------------
//色の設定
//--------------------------------------
void CObject2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_Vertex->Lock(0, 0, (void**)&pVtx, 0);

	//テキスチャの座標設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_Vertex->Unlock();
}

//--------------------------------------
//サイズの設定
//--------------------------------------
void CObject2D::SetSize(float fHi, float fWd)
{
	m_Change_Height = fHi;		//高さを制御する変数に受け取った高さを代入する
	m_Change_Width = fWd;		//幅を制御する変数に受け取った幅を代入する

	//対角線の長さを算出する
	m_fLength = sqrtf(fHi * fHi + fWd * fWd) / 2.0f;

	//対角線の角度を算出する
	m_fAngle = atan2f(fHi, fWd);
}