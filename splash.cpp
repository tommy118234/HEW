//=============================================================================
// スプラッシュ [splash.cpp]
//
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
// 作成日 : 2018/1/16
//=============================================================================


//*****************************************************************************
// インクルード
//*****************************************************************************
#include "sound.h"
#include "input.h"
#include "quadrangle.h"
#include "splash.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ
#define TEXTURE_TEAM_LOGO			("data/TEXTURE/logo.png")

// テクスチャのサイズ
#define TEAM_LOGO_SIZE_X			(480)						// サイズ（X）
#define TEAM_LOGO_SIZE_Y			(200)						// サイズ（Y）

// タイムテーブル（1/1000秒）
#define 	SEQUENCE_1				(0			+ 1000)			// ウェイト
#define		SEQUENCE_2				(SEQUENCE_1	+ 1000)			// フェードイン
#define		SEQUENCE_3				(SEQUENCE_2	+ 2000)			// 表示中
#define		WAIT_PRE_TITLE			(SEQUENCE_3	+ 1000)			// フェードアウト
#define		END						(WAIT_PRE_TITLE + 500)		// ウェイト	


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
QUADRANGLE			splash;										// チームロゴ
QUADRANGLE			bg;											// 背景
LPDIRECT3DTEXTURE9	texSplash = NULL;							// テクスチャへのポインタ


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSplash(BOOL type)
{
	LPDIRECT3DDEVICE9 device = GetDevice();

	// ロゴ
	InitQuad(&splash, TEAM_LOGO_SIZE_X, TEAM_LOGO_SIZE_Y);
	splash.transform.pos = D3DXVECTOR3((float)SCREEN_CENTER_X, (float)SCREEN_CENTER_Y, 0.0f);
	SetColorQuad(&bg, D3DCOLOR_RGBA(255, 255, 255, 0));
	SetVertexQuad(&splash);

	// 背景
	InitQuad(&bg, SCREEN_WIDTH, SCREEN_HEIGHT);
	bg.transform.pos = D3DXVECTOR3((float)SCREEN_CENTER_X, (float)SCREEN_CENTER_Y, 0.0f);
	SetColorQuad(&bg, D3DCOLOR_RGBA(255, 255, 255, 255));
	SetVertexQuad(&bg);

	// 1回目の初期化時のみ実行
	if (type != REINIT)
	{// シーン遷移などによる再初期化時は行わない処理を↓に書く

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(device,		// デバイスのポインタ
			TEXTURE_TEAM_LOGO,					// ファイルの名前
			&texSplash);						// 読み込むメモリのポインタ
	}

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitSplash(void)
{
		SAFE_RELEASE(texSplash)
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateSplash(void)
{
	// @下準備
	DWORD currentTime = GetTime();			// 現在時刻を取得する
	static	DWORD lastTime = currentTime;	// 時間計測用
	static	DWORD time = 0;					// 時間計測用
	static  int valueFade = 0;

	// @時間計測
	time += currentTime - lastTime;			// 経過時間（ミリ秒）を記録
	lastTime = currentTime;					// 現フレームでの時刻を記録

	// @スキップ
	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE)
		|| IsButtonTriggered(0, BUTTON_A) || IsButtonTriggered(0, BUTTON_X))
	{
		time = END;
	}

	// @スプラッシュ
	if (0 < time && time < SEQUENCE_1)
	{// 表示までのウェイト

	}
	if (SEQUENCE_1 < time && time < SEQUENCE_2)
	{// フェードイン
		valueFade += 255 / (int)MSECOND(SEQUENCE_2 - SEQUENCE_1);
		if (valueFade > 255) { valueFade = 255; }
	}
	if (SEQUENCE_2 < time && time < SEQUENCE_3)
	{// 表示中
		valueFade = 255;
	}
	if (SEQUENCE_3 < time && time < WAIT_PRE_TITLE)
	{// フェードアウト
		valueFade -= 255 / (int)MSECOND(WAIT_PRE_TITLE - SEQUENCE_3);
		if (valueFade < 0) { valueFade = 0; }
	}
	if (WAIT_PRE_TITLE < time && time < END)
	{// ウェイト
		valueFade = 0;
	}
	if (END < time)
	{// 終了処理
		time = 0;
		valueFade = 0;
		SetStage(TITLE);
	}

	// α値の反映
	SetColorQuad(&splash, D3DCOLOR_RGBA(255, 255, 255, valueFade));
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawSplash(void)
{
	LPDIRECT3DDEVICE9 device = GetDevice();

	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_2D);

	// @背景
	// テクスチャの設定
	device->SetTexture(0, NULL);

	// ポリゴンの描画
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, bg.vertexWk, sizeof(VERTEX_2D));
	
	// @ロゴ
	// テクスチャの設定
	device->SetTexture(0, texSplash);

	// ポリゴンの描画
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, splash.vertexWk, sizeof(VERTEX_2D));
}



