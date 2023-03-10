//--------------------------------------------------------------------------------------
//
// ヘルパー関数
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "helperfunction.h"

//--------------------------------------
//コンストラクタ
//--------------------------------------
CHelperFunction::CHelperFunction()
{
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CHelperFunction::~CHelperFunction()
{
}

//--------------------------------------
//円同士の当たり判定
//--------------------------------------
bool CHelperFunction::CircleCollision(D3DXVECTOR3 pos1, float fRadius1, D3DXVECTOR3 pos2, float fRadius2)
{
	//計算結果を代入
	float fAnser = fRadius1 + fRadius2;			//半径同士の加算結果を代入	

	//差分を保存する変数
	float fDifference_X, fDifference_Z;

	//距離を保存する変数
	float fRange;

	//Xの差分
	fDifference_X = pos1.x - pos2.x;

	//Zの差分
	fDifference_Z = pos1.z - pos2.z;

	//2点の距離の計算
	fRange = sqrtf(fDifference_X * fDifference_X + fDifference_Z * fDifference_Z);

	if (fAnser > fRange)
	{//当たっていたら
		return true;
	}

	//当たっていなかったら
	return false;
}

//--------------------------------------
//球同士の当たり判定
//--------------------------------------
bool CHelperFunction::SphereCollision(D3DXVECTOR3 pos1, float fRadius1, D3DXVECTOR3 pos2, float fRadius2)
{
	//計算結果を代入
	float fAnser = fRadius1 + fRadius2;			//半径同士の加算結果を代入

	//距離を出す
	D3DXVECTOR3 Vec = pos1 - pos2;

	//距離を保存する変数
	float fRange;

	//2点の距離の計算
	fRange = sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z);

	if (fAnser > fRange)
	{//当たっていたら
		return true;
	}

	//当たっていなかったら
	return false;
}

//--------------------------------------
//箱型の当たり判定(く形)
//--------------------------------------
bool CHelperFunction::BoxCollision(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2, D3DXVECTOR3 posOld)
{
	float fRight = pos2.x + size2.x / 2;
	float fLeft = pos2.x - size2.x / 2;
	float fBack = pos2.z + size2.z / 2;
	float fFlont = pos2.z - size2.z / 2;

	if (pos1.x + size1.x / 2 > fLeft &&
		pos1.x - size1.x / 2 < fRight)
	{
		if (posOld.z + size1.z / 2 <= fFlont &&
			pos1.z + size1.z / 2 > fFlont)
		{//前から後ろの判定
			return true;
		}

		if (posOld.z - size1.z / 2 >= fBack &&
			pos1.z - size1.z / 2 < fBack)
		{
			return true;
		}
	}

	if (pos1.z + size1.z / 2 > fFlont &&
		pos1.z - size1.z / 2 < fBack)
	{
		if (posOld.x + size1.x / 2 <= fLeft &&
			pos1.x + size1.x / 2 > fLeft)
		{//前から後ろの判定
			return true;
		}

		if (posOld.x - size1.x / 2 >= fRight &&
			pos1.x - size1.x / 2 < fRight)
		{//前から後ろの判定
			return true;
		}
	}

	return false;
}

//--------------------------------------
//ランダム関数
//--------------------------------------
float CHelperFunction::FloatRandom(float fMax, float fMin)
{
	return ((rand() / (float)RAND_MAX) * (fMax - fMin)) + fMin;
}
