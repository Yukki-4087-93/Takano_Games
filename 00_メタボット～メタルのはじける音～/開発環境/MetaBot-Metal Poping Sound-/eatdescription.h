//--------------------------------------------------------------------------------------
//
//お品書き
//Author;takano
//
//--------------------------------------------------------------------------------------
#ifndef _EATDESCRIPTION_H_													//このマクロ定義がなされなっかたら
#define _EATDESCRIPTION_H_													//二重インクルード防止のマクロ定義

//----------------------------------------
//インクルードファイル
//----------------------------------------
#include <d3dx9.h>
#include "object3d.h"

//----------------------------------------
//クラス定義
//お品書き関連
//----------------------------------------
class CEatDescription : public CObject3D
{
public:
	CEatDescription();								//コンストラクタ
	~CEatDescription();								//デストラクタ
	HRESULT Init();									//初期化
	void Uninit();									//終了
	void Update();									//更新
	void Draw();									//描画
	void HitDescription();							//当たり判定
	void HitEatDescription();						//当たり判定


public:
	static CEatDescription* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHi, float fWd, ObjType type, CTexture::TEXTURE tex);		//お品書きの生成
	void ChangeSE();

private:
	int m_nCHangeSound;				//どのSEを使用するか決めるための変数
	int m_HitCount;					//当たったときの当たり判定を加算するための変数
};

#endif // !EATDESCRIPTION_H_

