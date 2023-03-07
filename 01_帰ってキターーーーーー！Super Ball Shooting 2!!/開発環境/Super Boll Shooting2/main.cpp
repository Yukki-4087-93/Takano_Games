//=============================================================================
//
// メイン
// Author::TAKANO
//
//=============================================================================

//*****************************************************************************
// ライブラリーリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include <tchar.h> // _T
#include <d3dx9.h>
#include "main.h"
#include "application.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
namespace
{
	// ウインドウのクラス名
	LPCTSTR CLASS_NAME = _T("SUPERBOLLSHOOTING2");
	// ウインドウのキャプション名
	LPCTSTR WINDOW_NAME = _T("帰ってｷﾀ━(ﾟ∀ﾟ)━! SuperBollShooting 2 ! ～Return to Hero～");

	// スクリーンの幅
	const int SCREEN_WIDTH = 1280;
	// スクリーンの高さ
	const int SCREEN_HEIGHT = 720;
}

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hinstace,HWND hWnd, bool bWindow);
void Uninit();
void Update();
void Draw();
int GetCount();

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// FPSカウンタ
int g_nCountFPS;

CApplication* pApplecation;

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウの作成
	HWND hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	pApplecation = new CApplication;

	// 初期化処理
	if (FAILED(Init(hInstance,hWnd, TRUE)))
	{//初期化が失敗した場合
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	DWORD dwCurrentTime = 0;
	DWORD dwExecLastTime = timeGetTime();
#ifdef _DEBUG
	DWORD dwFrameCount = 0;
	DWORD dwFPSLastTime = dwExecLastTime;
#endif // _DEBUG

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	// メッセージループ
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{ // PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();	// 現在の時間を取得
#ifdef _DEBUG
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒ごとに実行
				// FPSを算出
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
				dwFrameCount = 0;
			}
#endif // _DEBUG

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{ // 1/60秒経過
				// 現在の時間を保存
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

#ifdef _DEBUG
				dwFrameCount++;
#endif // _DEBUG
			}
		}
	}

	// 終了処理
	Uninit();

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE: // [ESC]キーが押された
			// ウィンドウを破棄
			Uninit();
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT Init(HINSTANCE hinstace, HWND hWnd, bool bWindow)
{
	pApplecation->Init(hinstace, hWnd, bWindow);
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void Uninit()
{
	if (pApplecation != nullptr)
	{
		pApplecation->Uninit();
		delete pApplecation;
		pApplecation = nullptr;
	}
}

//=============================================================================
// 更新
//=============================================================================
void Update()
{
	pApplecation->Update();
}

//=============================================================================
// 描画
//=============================================================================
void Draw()
{
	pApplecation->Draw();
}

int GetCount()
{
	return g_nCountFPS;
}

/*
レンダリング…計算などを駆使して描画を行うこと

メインで生成してレンダリング処理で実体を持つ

//キャストやらを使った複数化

//変数の宣言
CObject *g_pObj = nullptr;

//ポリゴン生成
if (!g_pObj)
{
g_pObj = new CObject2D;
}

if (g_pObj)
{
g_pObj->Init();
//キャストする
CObject2D *pObj2D = (CObject2D*)g_pObj;
pObj2D->Setpos
}

if(g_pObj)
{//終了
g_pObj->Uninit();
g_pObj = nullptr;
}

//ポインタのずらし方

 CObject **ppObject = GetObject();

//ポインタ自体を＋＋してずらす方法
for(int nCnt = 0; nCnt < 最大数; nCnt++,ppObject++;)
{
	if((*(ppObject) != nullptr )
	{
		(*ppObject)-> Updata();
	}
}

//配列の数字を足しつつ番号をずらす方法
for(int nCnt = 0; nCnt < 最大数; nCnt++;)
{
	if((*(ppObject + nCnt)) != nullptr )
	{
		(*(ppObject + nCnt))-> Updata();
	}
}

g_pObject[nCnt]->Setpos({ 100.0f*nCnt,50.0f,0.0f });

// 今日のトレビア //
NULLチェック　思ったよりも　大事です
*/