////--------------------------------------------------------------------------------------
////
////影
////Author;takano
////
////--------------------------------------------------------------------------------------
//#ifndef _SHADOW_H_													//このマクロ定義がなされなっかたら
//#define _SHADOW_H_													//二重インクルード防止のマクロ定義
//
////----------------------------------------
////インクルードファイル
////----------------------------------------
//#include <d3dx9.h>
//#include "object3d.h"
//
////----------------------------------------
////クラス定義
////影関連
////----------------------------------------
//class CShadow : public CObject3D
//{
//public:
//	CShadow();
//	~CShadow();
//	HRESULT Init();
//	void Uninit();
//	void Update();
//	void Draw();
//	int Set(D3DXVECTOR3 pos, float fRadius);
//	//void SetPos(int nIdxShadow, D3DXVECTOR3 pos);
//
//
//private:
//	D3DXVECTOR3 m_move;						//移動量
//	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス(？)
//	float m_fRadius;						//影の半径
//};
//
//#endif // !SHADOW_H_
