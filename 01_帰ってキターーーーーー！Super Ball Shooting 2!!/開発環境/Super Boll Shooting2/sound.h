//---------------------------------------------------------
//
//サウンド
//Author::TAKANO
//
//---------------------------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include"d3dx9.h"
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include"xaudio2.h"
#include <xaudio2fx.h>

//--------------------------------------
//クラス定義
//サウンド関連
//--------------------------------------
class CSound
{
public:
	struct PARAM
	{//サウンドパラメーターの構造体
		char *m_pFilename;									// ファイル名
		int m_nCntLoop;										// ループカウント
	};

	enum SOUND_TYPE
	{//サウンドタイプ
		SOUND_BGM_TITLE = 0,								// タイトル
		SOUND_BGM_GAME,										// ゲーム
		SOUND_BGM_GAME_BOSS,								// ゲーム中のボスに使用
		SOUND_BGM_GAME_LAST,								// ボス出現から時間が数秒経ったときに使用
		SOUND_BGM_RESULT,									// リザルト
		SOUND_BGM_RANKING,									// ランキング
		SOUND_BGM_TUTORAL,									// チュートリアル
		SOUND_SE_SOOT,										// 弾発射SE
		SOUND_SE_HIT,										// ダメージSE
		SOUND_SE_EXPLOSION,									// 爆発SE
		SOUND_MAX,
	};

	static const PARAM param[];								// パラメータの情報

	CSound();												// コンストラクタ
	~CSound();												// デストラクタ
	HRESULT Init();											// 初期化
	void Uninit();											// 終了
	HRESULT Play(SOUND_TYPE type);							// 再生
	void SetVolume(SOUND_TYPE type, float fVolume);			// 音量の設定、初期化のときに使ってほしいです。最悪なくてもいい。ちなみに、SEにはいらないよ
	void SetRate(SOUND_TYPE type, float rate);				// ピッチ操作、初期化以下略。最悪なくてもいい。
	void Stop(SOUND_TYPE type);								// 一部停止
	void Stop();											// 全停止

public:
	void UpVolume(SOUND_TYPE type, float volume);			// 音量を上げたいときに使う
	void DownVolume(SOUND_TYPE type, float volume);			// 音量を下げたいときに使う
	void UpPitch(SOUND_TYPE type, float pitch);				// 音程を上げたいときに使う
	void DownPitch(SOUND_TYPE type, float pitch);			// 音程を下げたいときに使う

private:
	HRESULT CheckChunk(HANDLE hFile,						// 一塊(チャンク)の確認
					   DWORD format,
					   DWORD *pChunkSize, 
					   DWORD *pChunkDataPosition);			


	HRESULT ReadChunkData(HANDLE hFile,						// 一塊(チャンク)の読み込み
						  void *pBuffer, 
						  DWORD dwBuffersize, 
						  DWORD dwBufferoffset);			

private:
	static CSound * m_pSound;
	IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_MAX];		// ソースボイス
	BYTE *m_apDataAudio[SOUND_MAX];							// オーディオデータ
	DWORD m_aSizeAudio[SOUND_MAX];							// オーディオデータサイズ
	int m_nCountFade;										// フェード用カウンター
	float m_fVol;											// サウンドの音量を変化させる変数
	float m_fPitch;											// サウンドの音程を変化させる変数
};

#endif // !_SOUND_H_
