//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef ___SOUND_H___
#define ___SOUND_H___


//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include <windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>


//*****************************************************************************
// マクロ・列挙型定義
//*****************************************************************************
enum SOUND_NAME	//サウンドテーブル --------------------------------------------
{	// SE
	SE_ATTACK,
	SE_GAME_CLEAR,
	SE_GAME_OVER,
	SE_ITEM_GET,
	SE_DAMAGE_PLAYER,
	SE_DAMAGE_TARGET,
	SE_WALK,

	// BGM
	BGM_TITLE,
	BGM_GAME,
	BGM_MUTEKI,
//-----------------------------------------------------------------------------
	SOUND_MAX			// サウンドの数
};


enum SOUND_PLAY_TYPE
{	// 再生タイプ
	INIT_SOUND,			// 最初から再生
	CONTINUE_SOUND,		// 継続して再生
	TYPE_MAX
};

enum SOUND_PLAY_MODE
{	// 再生用フラグ
	NONE,				// 一度のみ再生
	LOOP,				// ループ再生
	FLAG_MAX
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT					InitSound( HWND hWnd );		// サウンドの初期化処理
void					UninitSound();				// サウンドの終了処理
LPDIRECTSOUNDBUFFER8	LoadSound(int no);			// サウンドのロード

// 音ごとに再生する
void PlayGameSound(SOUND_NAME no, SOUND_PLAY_TYPE type, SOUND_PLAY_MODE mode);

// 音を止める
void StopSound(SOUND_NAME no, SOUND_PLAY_TYPE type);

// 音を全て止める
void StopAllSound(SOUND_PLAY_TYPE type);

// 再生中かどうか調べる
bool IsPlaying( LPDIRECTSOUNDBUFFER8 pBuffer );

// サウンドを取得
LPDIRECTSOUNDBUFFER8 GetSound(SOUND_NAME sound_name);

// サウンドをリセット
void ResetSound(SOUND_NAME sound_name);

#endif