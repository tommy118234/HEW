//=============================================================================
//
// パンティ処理 [panty.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "panty.h"
#include "font.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ場所
#define TEXTURE_PANTY		_T("data/TEXTURE/pant.png")

// パンティのサイズ
#define PANTY_SIZE_X		(50)
#define PANTY_SIZE_Y		(50)	

// パンティの座標
#define INIT_POS_X			(300)
#define INIT_POS_Y			(50.0f)
#define INTARVAL_X			(10.0f)
#define	AJUST_Y				(5.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPanty(void);						// 頂点の作成
void SetVertexPanty(void);							// 頂点座標の設定
void SetColorPanty(void);							// 頂点カラーの設定
void SetTexturePanty(void);							// テクスチャ座標の設定


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMEPANTY panty;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPanty(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_PANTY,
			&panty.pTexture);
	}

	panty.pos = D3DXVECTOR3(INIT_POS_X, INIT_POS_Y, 0.0f);
	panty.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ZeroMemory(panty.text, sizeof(TCHAR) * TEXT_MAX);
	panty.numPanty = 0;
	MakeVertexPanty();
	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitPanty(void)
{
	SAFE_RELEASE(panty.pTexture)
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdatePanty(void)
{
	// 数字の更新
	wsprintf(panty.text, _T("× %d\n"), panty.numPanty);

	// 頂点座標の設定
	SetVertexPanty();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_P))
	{
		AddNumPanty();
	}
#endif
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawPanty(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// パンティ
	pDevice->SetTexture(0, panty.pTexture);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, panty.vertexWk, sizeof(VERTEX_2D));

	// × 何個
	LPD3DXFONT font = GetFont(IMPACT);
	RECT rect = { panty.pos.x + PANTY_SIZE_X + INTARVAL_X,  panty.pos.y +  AJUST_Y, SCREEN_WIDTH, SCREEN_HEIGHT };
	font->DrawText(NULL, panty.text, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPanty(void)
{
	// 頂点座標の設定	
	SetVertexPanty();

	// 頂点カラーの設定
	SetColorPanty();

	// テクスチャ座標の設定
	SetTexturePanty();

	// rhwの設定
	panty.vertexWk[0].rhw =
		panty.vertexWk[1].rhw =
		panty.vertexWk[2].rhw =
		panty.vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPanty(void)
{
	panty.vertexWk[0].vtx = D3DXVECTOR3(panty.pos.x, panty.pos.y, panty.pos.z);
	panty.vertexWk[1].vtx = D3DXVECTOR3(panty.pos.x + PANTY_SIZE_X, panty.pos.y, panty.pos.z);
	panty.vertexWk[2].vtx = D3DXVECTOR3(panty.pos.x, panty.pos.y + PANTY_SIZE_Y, panty.pos.z);
	panty.vertexWk[3].vtx = D3DXVECTOR3(panty.pos.x + PANTY_SIZE_X, panty.pos.y + PANTY_SIZE_Y, panty.pos.z);
}


//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorPanty(void)
{
	panty.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	panty.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	panty.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	panty.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePanty(void)
{
	panty.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	panty.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	panty.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	panty.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


//=============================================================================
// パンティの数を追加
//=============================================================================
void AddNumPanty(void)
{
	panty.numPanty++;
}