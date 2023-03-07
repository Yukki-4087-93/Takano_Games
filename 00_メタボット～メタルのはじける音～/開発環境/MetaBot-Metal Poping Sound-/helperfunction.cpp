//--------------------------------------------------------------------------------------
//
// ƒwƒ‹ƒp[ŠÖ”
// Author::TAKANO
//
//--------------------------------------------------------------------------------------

//--------------------------------------
//ƒCƒ“ƒNƒ‹[ƒhƒtƒ@ƒCƒ‹
//--------------------------------------
#include "helperfunction.h"

//--------------------------------------
//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
//--------------------------------------
CHelperFunction::CHelperFunction()
{
}

//--------------------------------------
//ƒfƒXƒgƒ‰ƒNƒ^
//--------------------------------------
CHelperFunction::~CHelperFunction()
{
}

//--------------------------------------
//‰~“¯Žm‚Ì“–‚½‚è”»’è
//--------------------------------------
bool CHelperFunction::CircleCollision(D3DXVECTOR3 pos1, float fRadius1, D3DXVECTOR3 pos2, float fRadius2)
{
	//ŒvŽZŒ‹‰Ê‚ð‘ã“ü
	float fAnser = fRadius1 + fRadius2;			//”¼Œa“¯Žm‚Ì‰ÁŽZŒ‹‰Ê‚ð‘ã“ü	

	//·•ª‚ð•Û‘¶‚·‚é•Ï”
	float fDifference_X, fDifference_Z;

	//‹——£‚ð•Û‘¶‚·‚é•Ï”
	float fRange;

	//X‚Ì·•ª
	fDifference_X = pos1.x - pos2.x;

	//Z‚Ì·•ª
	fDifference_Z = pos1.z - pos2.z;

	//2“_‚Ì‹——£‚ÌŒvŽZ
	fRange = sqrtf(fDifference_X * fDifference_X + fDifference_Z * fDifference_Z);

	if (fAnser > fRange)
	{//“–‚½‚Á‚Ä‚¢‚½‚ç
		return true;
	}

	//“–‚½‚Á‚Ä‚¢‚È‚©‚Á‚½‚ç
	return false;
}

//--------------------------------------
//‹…“¯Žm‚Ì“–‚½‚è”»’è
//--------------------------------------
bool CHelperFunction::SphereCollision(D3DXVECTOR3 pos1, float fRadius1, D3DXVECTOR3 pos2, float fRadius2)
{
	//ŒvŽZŒ‹‰Ê‚ð‘ã“ü
	float fAnser = fRadius1 + fRadius2;			//”¼Œa“¯Žm‚Ì‰ÁŽZŒ‹‰Ê‚ð‘ã“ü

	//‹——£‚ðo‚·
	D3DXVECTOR3 Vec = pos1 - pos2;

	//‹——£‚ð•Û‘¶‚·‚é•Ï”
	float fRange;

	//2“_‚Ì‹——£‚ÌŒvŽZ
	fRange = sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z);

	if (fAnser > fRange)
	{//“–‚½‚Á‚Ä‚¢‚½‚ç
		return true;
	}

	//“–‚½‚Á‚Ä‚¢‚È‚©‚Á‚½‚ç
	return false;
}

//--------------------------------------
//” Œ^‚Ì“–‚½‚è”»’è(‚­Œ`)
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
		{//‘O‚©‚çŒã‚ë‚Ì”»’è
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
		{//‘O‚©‚çŒã‚ë‚Ì”»’è
			return true;
		}

		if (posOld.x - size1.x / 2 >= fRight &&
			pos1.x - size1.x / 2 < fRight)
		{//‘O‚©‚çŒã‚ë‚Ì”»’è
			return true;
		}
	}

	return false;
}

//--------------------------------------
//ƒ‰ƒ“ƒ_ƒ€ŠÖ”
//--------------------------------------
float CHelperFunction::FloatRandom(float fMax, float fMin)
{
	return ((rand() / (float)RAND_MAX) * (fMax - fMin)) + fMin;
}
