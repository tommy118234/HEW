//=============================================================================
// サウンド処理 [sound.cpp]
//
// Author : GP11B341 24　中込和輝
// 作成日 : 2018/11/1
//=============================================================================


//*****************************************************************************
// インクルード
//*****************************************************************************
#include "sound.h"


//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )


//*****************************************************************************
// サウンドファイルのパス
//*****************************************************************************
// サウンドファイルのパス（hの通しナンバーと合わせること）
const TCHAR* c_soundFilename[] = {
	// SE
	_T("data/SOUND/SE/Attack.wav"),
	_T("data/SOUND/SE/gameclear.wav"),
	_T("data/SOUND/SE/gameover.wav"),
	_T("data/SOUND/SE/item.wav"),
	_T("data/SOUND/SE/PlayerDamage.wav"),
	_T("data/SOUND/SE/TargetDamage.wav"),
	_T("data/SOUND/SE/walk.wav"),
	_T("data/SOUND/SE/se_menu_decision.wav"),
	_T("data/SOUND/SE/se_menu_cursor.wav"),


	// BGM
	_T("data/SOUND/BGM/title.wav"),
	_T("data/SOUND/BGM/gamegamen.wav"),
	_T("data/SOUND/BGM/muteki.wav"),
};


//*****************************************************************************
// グローバル変数
//*****************************************************************************
IDirectSound8 *directSound = NULL;			// サウンドインターフェース
LPDIRECTSOUNDBUFFER8 sound[SOUND_MAX];		// サウンド用バッファ
//LPDIRECTSOUNDBUFFER8 dualSound[DUPLICATED_SOUND_MAX];



//=============================================================================
// 初期化処理
//=============================================================================
// hWnd:ウィンドウハンドル
HRESULT InitSound( HWND hWnd )
{
	// DirectSoundオブジェクトの作成
	if (FAILED(DirectSoundCreate8(NULL, &directSound, NULL)))
		return E_FAIL;
	
	// 協調レベル設定 
	if (FAILED(directSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		return E_FAIL;

	// ロード
	int i;
	for (i = 0; i < SOUND_MAX; i++)
	{
		sound[i] = LoadSound(i);
	}

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{
	// サウンド用バッファの解放
	int i;
	for (i = 0; i < SOUND_MAX; i++)
	{
		SAFE_RELEASE(sound[i])
	}

	// ※サウンド用バッファ解放後に行う
	if( directSound != NULL )
	{
		directSound->Release();
		directSound = NULL;
	}
}


//=============================================================================
// サウンドのロード
//=============================================================================
// no:サウンドナンバー（ヘッダに定義された列挙型定数）
LPDIRECTSOUNDBUFFER8 LoadSound(int no)
{
	// MMIO = マルチメディア入出力、の略。
	LPDIRECTSOUNDBUFFER  BaseBuffer = NULL;	// 曲データの総合バッファ
	LPDIRECTSOUNDBUFFER8 buffer = NULL;		// 曲データのバッファ
	DSBUFFERDESC buff;							// バッファ設定構造体

	HMMIO hMmio = NULL;							// MMIOハンドル
	MMIOINFO mmioInfo;							// マルチメディアデータ構造体

	MMRESULT mmRes;								// MM処理結果格納変数
	MMCKINFO riffChunk, formatChunk, dataChunk;	// チャンクデータ構造体
	DWORD size;									// データサイズ格納用
	WAVEFORMATEX pcm;							// 曲データフォーマット構造体

	LPVOID Block;								// セカンダリバッファのデータ書き込み先アドレス
	DWORD  dwSize;								// セカンダリバッファのサイズ

	// 1.ハンドルをもとにファイルを開く
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)c_soundFilename[no], &mmioInfo, MMIO_READ);
	if(!hMmio)
		return NULL;

	// 2.ファイル解析① RIFFチャンク検索
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');						// 検索キーワード
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);			// キーワードをもとに検索
	
	if(mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 3.ファイル解析② フォーマットチャンク検索
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// 検索キーワード
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// キーワードをもとに検索
	
	if(mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);				// 検索情報をもとに読み込み
	
	if(size != formatChunk.cksize) 		// 正しく読み込めなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0); 

	// 4.ファイル解析③ データチャンク検索
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');						// 検索キーワード
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);		// キーワードをもとに検索
	
	if(mmRes != MMSYSERR_NOERROR) 		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 5.データ読み込み		
	char *data = new char[dataChunk.cksize];								// 必要な大きさの領域を確保して
	size = mmioRead(hMmio, (HPSTR)data, dataChunk.cksize);					// データを読み込む
	
	if(size != dataChunk.cksize)  		// 正しく読み込めなかったら異常終了
	{
		delete[] data;
		return NULL;
	}

	// 6.曲を読み込む「セカンダリバッファ」を用意
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));								// まず初期化
	buff.dwSize = sizeof(DSBUFFERDESC);										// そこから各種設定
	buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// 総合バッファを作る
	if (FAILED(directSound->CreateSoundBuffer(&buff, &BaseBuffer, NULL)))
		return NULL;

	// サウンドバッファを取り出す
	if (FAILED(BaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&buffer)))
		return NULL;

	// 取り出したら総合バッファを捨てる
	BaseBuffer->Release();

	// 7.作られたセカンダリバッファにデータを転送
	// まずは書き込みできるようバッファをロック
	if (FAILED(buffer->Lock(0, 0, &Block, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
		return NULL;

	memcpy(Block, data, dwSize);	// データ転送
	delete[] data;					// 元の読み込み領域を消す

	// バッファロック解除
	buffer->Unlock(Block, dwSize, 0, 0);

	// セカンダリバッファを返してようやく完了...
	return buffer;
}


//=============================================================================
// 音を鳴らす
//-----------------------------------------------------------------------------
// 戻り値：void
// 引数  ：SOUND_NAMME     no    再生するサウンドの番号
//         SOUND_PLAY_TYPE type  INIT_SOUND……最初から鳴らす　CONTINUE_SOUND……継続して鳴らす
//	       SOUND_PLAY_MODE flag  NONE……１回再生して終了　LOOP……ループ再生
//=============================================================================
void PlayGameSound(SOUND_NAME no, SOUND_PLAY_TYPE type, SOUND_PLAY_MODE mode)
{	
	// typeをINITIALにすると関数を呼ぶごとに再生位置が先頭にもどるので
	// BGMや連続で発生する効果音はCONTINUEにする
	if (type == INIT_SOUND)
	{
		// 再生位置を先頭に戻す
		// 同じサウンドを無音時間なく連続再生するため
		sound[no]->SetCurrentPosition(0);
	}

	sound[no]->Play(0, 0, mode);
}



//=============================================================================
// 音を止める
//-----------------------------------------------------------------------------
// 戻り値：void
// 引数  ：SOUND_NAMME     no    停止するサウンドの番号
//         SOUND_PLAY_TYPE type  INIT_SOUND……停止後再生位置をリセット　CONTINUE_SOUND……停止後再生位置はそのまま
//=============================================================================
void StopSound(SOUND_NAME no, SOUND_PLAY_TYPE type)
{
	DWORD status;

	sound[no]->GetStatus(&status);
	if( status & DSBSTATUS_PLAYING )	// 鳴っていたら
	{
		sound[no]->Stop();				// 意味的にはPauseになる。
	}

	if (type == INIT_SOUND)
	{
		// 再生位置を先頭に戻す
		sound[no]->SetCurrentPosition(0);
	}
}


//=============================================================================
// 音を全て止める
//-----------------------------------------------------------------------------
// 戻り値：void
// 引数  ：SOUND_PLAY_TYPE type  INIT_SOUND……停止後再生位置をリセット　CONTINUE_SOUND……停止後再生位置はそのまま         
//=============================================================================
void StopAllSound(SOUND_PLAY_TYPE type)
{
	DWORD status;
	
	for (int i = 0; i < SOUND_MAX; i++)
	{
		sound[i]->GetStatus(&status);

		if (status & DSBSTATUS_PLAYING)	// 鳴っていたら
		{
			sound[i]->Stop();			// 意味的にはPauseになる。
		}

		if (type == INIT_SOUND)
		{
			// ↑の後にこれを実行しないと再生位置が初期化されない
			// 再生位置を先頭に戻す
			sound[i]->SetCurrentPosition(0);
		}
	}
}


//=============================================================================
// 再生中かどうか調べる
//=============================================================================
// buffer:音を鳴らしたいサウンドデータのセカンダリバッファ
bool IsPlaying(LPDIRECTSOUNDBUFFER8 buffer)
{
	DWORD status;

	buffer->GetStatus(&status);
	if( status & DSBSTATUS_PLAYING )
	{
		return true;
	}
	return false;
}


//=============================================================================
// サウンドをリセット
//=============================================================================
void ResetSound(SOUND_NAME sound_name)
{
	LPDIRECTSOUNDBUFFER8 buffer = GetSound(sound_name);
	// 再生位置を先頭に戻す
	// 同じサウンドを無音時間なく連続再生するため
	buffer->SetCurrentPosition(0);
}


//=============================================================================
// サウンドを取得
//-----------------------------------------------------------------------------
// 戻り値：LPDIRECTSOUNDBUFFER8　 　サウンドバッファへのポインタ
// 引数  ：SOUND_NAMME  soundName 　取得するサウンドの番号
//=============================================================================
LPDIRECTSOUNDBUFFER8 GetSound(SOUND_NAME soundName)
{
	return	sound[soundName];
}


