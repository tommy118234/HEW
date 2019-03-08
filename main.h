//=============================================================================
//
// メイン処理 [main.h]
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"

// 警告対策
#define DIRECTINPUT_VERSION (0x0800)

#include "dinput.h"
#include <tchar.h>
#include "mmsystem.h"


//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#endif


//*****************************************************************************
// マクロ・列挙型定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	NUM_VERTEX				(4)					// 頂点数
#define	NUM_POLYGON				(2)					// ポリゴン数

// 画面サイズ
#define SCREEN_WIDTH			(1024)				// ウインドウの幅
#define SCREEN_HEIGHT			(576)				// ウインドウの高さ
#define SCREEN_CENTER_X			(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y			(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

// プログラム全般で使用する表記
#define FIRSTINIT				(0)					// 初回初期化
#define REINIT					(1)					// 再初期化
#define	FPS						(60)				// FPS
#define SCREEN_OUT				(-10000.0f)			// 画面外


//-----------------------------------------------------------------------------
// メモリ解放マクロ
//-----------------------------------------------------------------------------
#define SAFE_RELEASE(p) if (p) { p->Release(); p = NULL; }
#define SAFE_DELETE_ARRAY(a) { delete [] (a); (a) = NULL; }
#define SAFE_DELETE(p) if(p){ delete[] p; p=NULL; }
#define SAFE_DESTROY(x) { if(x) { (x)->Destroy(); (x) = NULL; } }
//-----------------------------------------------------------------------------
// 物理計算
//-----------------------------------------------------------------------------
// 「秒」「分」「時」→ フレーム数
#define		MSECOND(_msec)		((_msec) * 0.001f * FPS)
#define		SECOND(_sec)		((_sec) * FPS)
#define		MINUTE(_min)		(SECOND((_min) * 60))
#define		HOUR(_hour)			(MINUTE((_hour) * 60))

// 「ミリ」「センチ」「キロ」→ メートル 
#define		MM(_mm)				(CM((_mm) * 0.1f))
#define		CM(_cm)				(M((_cm) * 0.01f))
#define		M(_m)				(_m)
#define		KM(_km)				(M((_km) * 1000.0f))

// 速度
#define		M_S(_speed)			(M(_speed) / SECOND(1.0f))
#define		KM_H(_speed)		(KM(_speed) / HOUR(1.0f))

// 加速度
#define		M_S2(_acc)			(M(_acc) / (SECOND(1.0f) * SECOND(1.0f)))

// 標準重力加速度 (↓方向なので－)
#define		GRAVITY				(M_S2(-9.8f))


//-----------------------------------------------------------------------------
// ステージ遷移
//-----------------------------------------------------------------------------
enum STAGE
{
	SPLASH,						// スプラッシュ
	TITLE,						// タイトル
	TUTORIAL,					// チュートリアル
	GAME,						// ゲーム
	PAUSE,						// ポーズ画面
	RESULT,						// リザルト
	EXIT						// 終了
};


//*****************************************************************************
// 構造体定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
struct  VERTEX_2D
{
	D3DXVECTOR3		vtx;		// 頂点座標
	float			rhw;		// テクスチャのパースペクティブコレクト用
	D3DCOLOR		diffuse;	// 反射光
	D3DXVECTOR2		tex;		// テクスチャ座標
};


// TRANSFORM構造体を定義
struct  TRANSFORM
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 回転
	D3DXVECTOR3		scl;		// 大きさ
};


// ゲーム進行データセット
struct GAMEDATA
{
	BOOL			isGameClear;	// ゲームのリザルト// TRUE:GameClear FALSE:GameOver
};


struct CHARACTER 
{
	char name[32];
	int HP;
	int MP;
	int ATK;
	int DEF;
	int LUCK;
	int EXP;
	int LV;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// デバイス取得
LPDIRECT3DDEVICE9 GetDevice(void);

// ステージをセット
void SetStage(STAGE set);

// FPS取得
int GetFPS(void);

// 現在時刻取得
DWORD GetTime(void);

// ゲーム進行データセットを取得
GAMEDATA *GetGameData(void);


#endif