//=============================================================================
//
// デバッグ表示処理 [debugproc.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS		// WARNINGS防止

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <stdio.h>
#include "input.h"
#include "quadrangle.h"
#include "debugproc.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DEBUG_BG_WIDTH		(200)
#define DEBUG_BG_HEIGHT		(450)

//*****************************************************************************
// プロトタイプ宣言
//****************************************************************************

#ifdef _DEBUG

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXFONT	D3DXFont = NULL;				// フォントへのポインタ
char		aStrDebug1[1024] = { "\0" };	// デバッグ情報
char		aStrDebug2[1024] = { "\0" };	// デバッグ情報
char		aStrDebug3[1024] = { "\0" };	// デバッグ情報
char		aStrDebug4[1024] = { "\0" };	// デバッグ情報
char		aStrDebug5[1024] = { "\0" };	// デバッグ情報
char		aStrDebug6[1024] = { "\0" };	// デバッグ情報
char		aStrDebug7[1024] = { "\0" };	// デバッグ情報
char		aStrDebug8[1024] = { "\0" };	// デバッグ情報
char		aStrDebug9[1024] = { "\0" };	// デバッグ情報
int			selectData;						// 選択番号
BOOL		isdisplay;						// 表示するかフラグ
QUADRANGLE	debugBG;						// 背景


//=============================================================================
// デバッグ表示処理の初期化
//=============================================================================
HRESULT InitDebugProc(void)
{
	LPDIRECT3DDEVICE9 device = GetDevice();
	HRESULT hr = NULL;

	// 背景の初期化
	InitQuad(&debugBG, DEBUG_BG_WIDTH, DEBUG_BG_HEIGHT);

	// 初期座標
	debugBG.transform.pos.x = SCREEN_WIDTH - 120.0f;
	debugBG.transform.pos.y = SCREEN_HEIGHT / 2;

	// 情報表示用フォントを設定
	hr = D3DXCreateFont(device, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &D3DXFont);

	// 情報クリア
	memset(aStrDebug1, 0, sizeof aStrDebug1);
	memset(aStrDebug2, 0, sizeof aStrDebug2);
	memset(aStrDebug3, 0, sizeof aStrDebug3);
	memset(aStrDebug4, 0, sizeof aStrDebug4);
	memset(aStrDebug5, 0, sizeof aStrDebug5);
	memset(aStrDebug6, 0, sizeof aStrDebug6);
	memset(aStrDebug7, 0, sizeof aStrDebug7);
	memset(aStrDebug8, 0, sizeof aStrDebug8);
	memset(aStrDebug9, 0, sizeof aStrDebug9);

	// 初期セレクト
	selectData = 1;
	SetColorQuad(&debugBG, D3DCOLOR_RGBA(255, 0, 0, 50));

	// 初期表示フラグ
	isdisplay = TRUE;

	return hr;
}


//=============================================================================
// デバッグ表示処理の終了処理
//=============================================================================
void UninitDebugProc(void)
{
	if(D3DXFont != NULL)
	{// 情報表示用フォントの開放
		D3DXFont->Release();
		D3DXFont = NULL;
	}
}


//=============================================================================
// デバッグ表示処理の更新処理
//=============================================================================
void UpdateDebugProc(void)
{
	// FPS
	int fps = GetFPS();


//----------------------------------------------------------------------------1
	PrintDebugProc(1, "******  D E B U G  ******\n");
	PrintDebugProc(1, "FPS:%d\n", fps);

//----------------------------------------------------------------------------2
	PrintDebugProc(2, "******  D E B U G  ******\n");

//----------------------------------------------------------------------------3
	PrintDebugProc(3, "******  D E B U G  ******\n");

//----------------------------------------------------------------------------4
	PrintDebugProc(4, "******  D E B U G  ******\n");

//----------------------------------------------------------------------------5
	PrintDebugProc(5, "******  D E B U G  ******\n");

//----------------------------------------------------------------------------6
	PrintDebugProc(6, "******  D E B U G  ******\n");

//----------------------------------------------------------------------------7
	PrintDebugProc(7, "******  D E B U G  ******\n");

//----------------------------------------------------------------------------8
	PrintDebugProc(8, "******  D E B U G  ******\n");

//----------------------------------------------------------------------------9
	PrintDebugProc(9, "******  D E B U G  ******\n");


//-------------------------------------------------------------------------操作
	// 表示⇔非表示
	if (GetKeyboardTrigger(DIK_0))
	{
		isdisplay ? isdisplay = FALSE : isdisplay = TRUE;
	}

	// データ選択
	if (GetKeyboardTrigger(DIK_1))
	{
		selectData = 1;

		// 色の設定
		debugBG.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 50);
		debugBG.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 50);
		debugBG.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 50);
		debugBG.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 50);

	}
	if (GetKeyboardTrigger(DIK_2))
	{
		selectData = 2;

		// 色の設定
		debugBG.vertexWk[0].diffuse = D3DCOLOR_RGBA(0, 0, 255, 50);
		debugBG.vertexWk[1].diffuse = D3DCOLOR_RGBA(0, 0, 255, 50);
		debugBG.vertexWk[2].diffuse = D3DCOLOR_RGBA(0, 0, 255, 50);
		debugBG.vertexWk[3].diffuse = D3DCOLOR_RGBA(0, 0, 255, 50);
	}
	if (GetKeyboardTrigger(DIK_3))
	{
		selectData = 3;

		// 色の設定
		debugBG.vertexWk[0].diffuse = D3DCOLOR_RGBA(0, 255, 0, 50);
		debugBG.vertexWk[1].diffuse = D3DCOLOR_RGBA(0, 255, 0, 50);
		debugBG.vertexWk[2].diffuse = D3DCOLOR_RGBA(0, 255, 0, 50);
		debugBG.vertexWk[3].diffuse = D3DCOLOR_RGBA(0, 255, 0, 50);
	}
	if (GetKeyboardTrigger(DIK_4))
	{
		selectData = 4;

		// 色の設定
		debugBG.vertexWk[0].diffuse = D3DCOLOR_RGBA(127, 127, 0, 50);
		debugBG.vertexWk[1].diffuse = D3DCOLOR_RGBA(127, 127, 0, 50);
		debugBG.vertexWk[2].diffuse = D3DCOLOR_RGBA(127, 127, 0, 50);
		debugBG.vertexWk[3].diffuse = D3DCOLOR_RGBA(127, 127, 0, 50);
	}
	if (GetKeyboardTrigger(DIK_5))
	{
		selectData = 5;

		// 色の設定
		debugBG.vertexWk[0].diffuse = D3DCOLOR_RGBA(127, 0, 127, 50);
		debugBG.vertexWk[1].diffuse = D3DCOLOR_RGBA(127, 0, 127, 50);
		debugBG.vertexWk[2].diffuse = D3DCOLOR_RGBA(127, 0, 127, 50);
		debugBG.vertexWk[3].diffuse = D3DCOLOR_RGBA(127, 0, 127, 50);
	}
	if (GetKeyboardTrigger(DIK_6))
	{
		selectData = 6;

		// 色の設定
		debugBG.vertexWk[0].diffuse = D3DCOLOR_RGBA(0, 127, 127, 50);
		debugBG.vertexWk[1].diffuse = D3DCOLOR_RGBA(0, 127, 127, 50);
		debugBG.vertexWk[2].diffuse = D3DCOLOR_RGBA(0, 127, 127, 50);
		debugBG.vertexWk[3].diffuse = D3DCOLOR_RGBA(0, 127, 127, 50);
	}
	if (GetKeyboardTrigger(DIK_7))
	{
		selectData = 7;

		// 色の設定
		debugBG.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 50);
		debugBG.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 50);
		debugBG.vertexWk[2].diffuse = D3DCOLOR_RGBA(0, 0, 255, 50);
		debugBG.vertexWk[3].diffuse = D3DCOLOR_RGBA(0, 0, 255, 50);
	}
	if (GetKeyboardTrigger(DIK_8))
	{
		selectData = 8;

		// 色の設定
		debugBG.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 50);
		debugBG.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 50);
		debugBG.vertexWk[2].diffuse = D3DCOLOR_RGBA(0, 255, 0, 50);
		debugBG.vertexWk[3].diffuse = D3DCOLOR_RGBA(0, 255, 0, 50);
	}
	if (GetKeyboardTrigger(DIK_9))
	{
		selectData = 9;

		// 色の設定
		debugBG.vertexWk[0].diffuse = D3DCOLOR_RGBA(0, 255, 0, 50);
		debugBG.vertexWk[1].diffuse = D3DCOLOR_RGBA(0, 255, 0, 50);
		debugBG.vertexWk[2].diffuse = D3DCOLOR_RGBA(0, 0, 255, 50);
		debugBG.vertexWk[3].diffuse = D3DCOLOR_RGBA(0, 0, 255, 50);
	}


	// 移動
	if (GetKeyboardPress(DIK_J))
	{
		debugBG.transform.pos.x -= 3.0f;
	}
	if (GetKeyboardPress(DIK_L))
	{
		debugBG.transform.pos.x += 3.0f;
	}
	if (GetKeyboardPress(DIK_I))
	{
		debugBG.transform.pos.y -= 3.0f;
	}
	if (GetKeyboardPress(DIK_K))
	{
		debugBG.transform.pos.y += 3.0f;
	}

	// 移動の反映
	SetVertexQuad(&debugBG);
}

//=============================================================================
// デバッグ表示処理の描画処理
//=============================================================================
void DrawDebugProc(void)
{
	if (isdisplay)
	{
		LPDIRECT3DDEVICE9 device = GetDevice();

		// 頂点フォーマットの設定
		device->SetFVF(FVF_VERTEX_2D);

		device->SetTexture(0, NULL);

		// 背景の描画
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &debugBG.vertexWk, sizeof(VERTEX_2D));

		RECT rect = { (LONG)debugBG.transform.pos.x - DEBUG_BG_WIDTH / 2, (LONG)debugBG.transform.pos.y - DEBUG_BG_HEIGHT / 2,
			(LONG)debugBG.transform.pos.x + DEBUG_BG_WIDTH / 2, (LONG)debugBG.transform.pos.y + DEBUG_BG_HEIGHT / 2 };

		switch (selectData)
		{
		case 1:
			// 情報表示
			D3DXFont->DrawText(NULL, aStrDebug1, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
			// 情報クリア
			memset(aStrDebug1, 0, sizeof aStrDebug1);
			break;

		case 2:
			// 情報表示
			D3DXFont->DrawText(NULL, aStrDebug2, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
			// 情報クリア
			memset(aStrDebug2, 0, sizeof aStrDebug2);
			break;

		case 3:
			// 情報表示
			D3DXFont->DrawText(NULL, aStrDebug3, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
			// 情報クリア
			memset(aStrDebug3, 0, sizeof aStrDebug3);
			break;

		case 4:
			// 情報表示
			D3DXFont->DrawText(NULL, aStrDebug4, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
			// 情報クリア
			memset(aStrDebug4, 0, sizeof aStrDebug4);
			break;

		case 5:
			// 情報表示
			D3DXFont->DrawText(NULL, aStrDebug5, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
			// 情報クリア
			memset(aStrDebug5, 0, sizeof aStrDebug5);
			break;

		case 6:
			// 情報表示
			D3DXFont->DrawText(NULL, aStrDebug6, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
			// 情報クリア
			memset(aStrDebug6, 0, sizeof aStrDebug6);
			break;

		case 7:
			// 情報表示
			D3DXFont->DrawText(NULL, aStrDebug7, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
			// 情報クリア
			memset(aStrDebug7, 0, sizeof aStrDebug7);
			break;

		case 8:
			// 情報表示
			D3DXFont->DrawText(NULL, aStrDebug8, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
			// 情報クリア
			memset(aStrDebug8, 0, sizeof aStrDebug8);
			break;

		case 9:
			// 情報表示
			D3DXFont->DrawText(NULL, aStrDebug9, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
			// 情報クリア
			memset(aStrDebug9, 0, sizeof aStrDebug9);
			break;
		}
	}
}

//=============================================================================
// デバッグ表示の登録
//=============================================================================
void PrintDebugProc(int no, const char *fmt,...)
{
// noはデバッグ情報の番号
#if 0
	long *pParam;
	static char aBuf[256];

	pParam = (long*)&fmt;
	sprintf(aBuf, fmt, pParam[1], pParam[2], pParam[3], pParam[4],
									pParam[5], pParam[6], pParam[7], pParam[8],
									pParam[9], pParam[10], pParam[11], pParam[12]);
#else
	va_list list;			// 可変引数を処理する為に使用する変数
	char *pCur;
	char aBuf[256]={"\0"};
	char aWk[32];

	// 可変引数にアクセスする前の初期処理
	va_start(list, fmt);

	pCur = (char *)fmt;
	for( ; *pCur; ++pCur)
	{
		if(*pCur != '%')
		{
			sprintf(aWk, "%c", *pCur);
		}
		else
		{
			pCur++;

			switch(*pCur)
			{
			case 'd':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf(aWk, "%d", va_arg(list, int));
				break;

			case 'f':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf(aWk, "%.2f", va_arg(list, double));		// double型で指定
				break;

			case 's':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf(aWk, "%s", va_arg(list, char*));
				break;

			case 'c':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf(aWk, "%c", va_arg(list, char));
				break;

			case 'L':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf(aWk, "%lx", va_arg(list, unsigned long));
				break;



			default:
				sprintf(aWk, "%c", *pCur);
				break;
			}
		}
		strcat(aBuf, aWk);
	}

	// 可変引数にアクセスした後の終了処理
	va_end(list);

	// 連結
	switch (no)
	{
	case 1:
		if ((strlen(aStrDebug1) + strlen(aBuf)) < ((sizeof aStrDebug1) - 1))
		{
			strcat(aStrDebug1, aBuf);
		}
		break;

	case 2:
		if ((strlen(aStrDebug2) + strlen(aBuf)) < ((sizeof aStrDebug2) - 1))
		{
			strcat(aStrDebug2, aBuf);
		}
		break;

	case 3:
		if ((strlen(aStrDebug3) + strlen(aBuf)) < ((sizeof aStrDebug3) - 1))
		{
			strcat(aStrDebug3, aBuf);
		}
		break;

	case 4:
		if ((strlen(aStrDebug4) + strlen(aBuf)) < ((sizeof aStrDebug4) - 1))
		{
			strcat(aStrDebug4, aBuf);
		}
		break;

	case 5:
		if ((strlen(aStrDebug5) + strlen(aBuf)) < ((sizeof aStrDebug5) - 1))
		{
			strcat(aStrDebug5, aBuf);
		}
		break;

	case 6:
		if ((strlen(aStrDebug6) + strlen(aBuf)) < ((sizeof aStrDebug6) - 1))
		{
			strcat(aStrDebug6, aBuf);
		}
		break;

	case 7:
		if ((strlen(aStrDebug7) + strlen(aBuf)) < ((sizeof aStrDebug7) - 1))
		{
			strcat(aStrDebug7, aBuf);
		}
		break;

	case 8:
		if ((strlen(aStrDebug8) + strlen(aBuf)) < ((sizeof aStrDebug8) - 1))
		{
			strcat(aStrDebug8, aBuf);
		}
		break;

	case 9:
		if ((strlen(aStrDebug9) + strlen(aBuf)) < ((sizeof aStrDebug9) - 1))
		{
			strcat(aStrDebug9, aBuf);
		}
		break;

	}
#endif
}


////=============================================================================
//// 頂点の作成：背景の頂点情報を作成
////=============================================================================
//HRESULT MakeVertexDebugBG(BG *bg)
//{
//	// 頂点座標の設定
//	SetVertexBG(bg);
//
//	// rhwの設定
//	bg->vertexWk[0].rhw =
//	bg->vertexWk[1].rhw =
//	bg->vertexWk[2].rhw =
//	bg->vertexWk[3].rhw = 3.0f;
//
//	// 反射光の設定
//	bg->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 50);
//	bg->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 50);
//	bg->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 50);
//	bg->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 50);
//
//	// テクスチャ座標の設定
//	bg->vertexWk[0].tex = D3DXVECTOR2(0, 0);
//	bg->vertexWk[1].tex = D3DXVECTOR2(1, 0);
//	bg->vertexWk[2].tex = D3DXVECTOR2(0, 1);
//	bg->vertexWk[3].tex = D3DXVECTOR2(1, 1);
//
//	return S_OK;
//}


////=============================================================================
//// 頂点座標の設定：背景の頂点座標を設定
////=============================================================================
//void SetVertexBG(BG *bg)
//{
//	bg->vertexWk[0].vtx.x = debugBG.pos.x;
//	bg->vertexWk[0].vtx.y = debugBG.pos.y;
//	bg->vertexWk[1].vtx.x = debugBG.pos.x + DEBUG_BG_WIDTH;
//	bg->vertexWk[1].vtx.y = debugBG.pos.y;
//	bg->vertexWk[2].vtx.x = debugBG.pos.x;
//	bg->vertexWk[2].vtx.y = debugBG.pos.y + DEBUG_BG_HEIGHT;
//	bg->vertexWk[3].vtx.x = debugBG.pos.x + DEBUG_BG_WIDTH;
//	bg->vertexWk[3].vtx.y = debugBG.pos.y + DEBUG_BG_HEIGHT;
//}


#endif