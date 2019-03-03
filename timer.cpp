//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "timer.h"
#include "font.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ場所
//#define TEXTURE_TIMER		_T("data/TEXTURE/timer.png")

// タイマーのサイズ
#define TIMER_SIZE_X		(SCREEN_WIDTH * 2)
#define TIMER_SIZE_Y		(SCREEN_HEIGHT / 5)	

// タイマーの座標
#define INIT_POS_X			(0.0f)
#define INIT_POS_Y			(SCREEN_HEIGHT * 0.8f)
#define MOVE_SPEED_X		(2.0f)

// 秒
#define INIT_REMAIN_TIME	(60)				// 残り時間
#define SECOND_TIME			(1000)				// システム時刻上の１秒


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTimer(void);					// 頂点の作成
void SetVertexTimer(void);						// 頂点座標の設定
void SetColorTimer(void);						// 頂点カラーの設定
void SetTextureTimer(void);						// テクスチャ座標の設定


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMETIMER timer;
LPSTR text;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTimer(int type)
{
	if (type == 0)
	{
		//// テクスチャの読み込み
		//LPDIRECT3DDEVICE9 pDevice = GetDevice();
		//D3DXCreateTextureFromFile(pDevice,
		//	TEXTURE_TIMER,
		//	&timer.pTexture);
	}

	timer.pos = D3DXVECTOR3(INIT_POS_X, INIT_POS_Y, 0.0f);
	timer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	timer.currentTime = 0;
	timer.lastTime = 0;
	timer.displayTime = INIT_REMAIN_TIME;
	MakeVertexTimer();

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitTimer(void)
{
	SAFE_RELEASE(timer.pTexture)
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimer(void)
{
	timer.currentTime = GetTime();

	// システム時刻で1秒経過ごとに表示タイムをカウントダウン
	if ((timer.currentTime - timer.lastTime) >= SECOND_TIME)
	{
		timer.lastTime = timer.currentTime;
		timer.displayTime--;
	}

	wsprintf(text, TEXT("%d\n"), &timer.displayTime);

	SetVertexTimer();
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPD3DXFONT font = GetFont(DEFAULT_FONT);
	RECT rect = { 0, 150, SCREEN_WIDTH, SCREEN_HEIGHT };
	font->DrawText(NULL, text, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));

	//// 頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_2D);

	//// テクスチャの設定
	//pDevice->SetTexture( 0, timer.pTexture );

	//// ポリゴンの描画
	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, timer.vertexWk, sizeof(VERTEX_2D));
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTimer(void)
{
	// 頂点座標の設定	
	SetVertexTimer();

	// 頂点カラーの設定
	SetColorTimer();

	// テクスチャ座標の設定
	SetTextureTimer();

	// rhwの設定
	timer.vertexWk[0].rhw =
		timer.vertexWk[1].rhw =
		timer.vertexWk[2].rhw =
		timer.vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexTimer(void)
{
	timer.vertexWk[0].vtx = D3DXVECTOR3(timer.pos.x, timer.pos.y, timer.pos.z);
	timer.vertexWk[1].vtx = D3DXVECTOR3(timer.pos.x + TIMER_SIZE_X, timer.pos.y, timer.pos.z);
	timer.vertexWk[2].vtx = D3DXVECTOR3(timer.pos.x, timer.pos.y + TIMER_SIZE_Y, timer.pos.z);
	timer.vertexWk[3].vtx = D3DXVECTOR3(timer.pos.x + TIMER_SIZE_X, timer.pos.y + TIMER_SIZE_Y, timer.pos.z);
}


//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorTimer(void)
{
	timer.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timer.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timer.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timer.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureTimer(void)
{
	timer.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	timer.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	timer.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	timer.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
