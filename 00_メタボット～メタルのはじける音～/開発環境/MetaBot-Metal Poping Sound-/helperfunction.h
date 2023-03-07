//--------------------------------------------------------------------------------------
//
// ヘルパー関数
// Author::TAKANO
//
//--------------------------------------------------------------------------------------
#ifndef _HELPERFUNCTION_H_
#define _HELPERFUNCTION_H_

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include <d3dx9.h>

//----------------------------------------
//クラス定義
//ヘルパー関数
//----------------------------------------
class CHelperFunction
{
public:
	CHelperFunction();									//コンストラクタ
	~CHelperFunction();									//デストラクタ
	static bool CircleCollision(D3DXVECTOR3 pos1, float fRadius1, D3DXVECTOR3 pos2, float fRadius2);
	static bool SphereCollision(D3DXVECTOR3 pos1,float fRadius1, D3DXVECTOR3 pos2, float fRadius2);
	static bool BoxCollision(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2, D3DXVECTOR3 posOld);
	static float FloatRandom(float fMax, float fMin);

protected:

};

#endif // !_HELPERFUNCTION_H_
