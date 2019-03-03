//=============================================================================
//
// 道（背景）処理 [road.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "road.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ場所
#define TEXTURE_ROAD		_T("data/TEXTURE/road.png")

// 道（背景）のサイズ
#define ROAD_SIZE_X			(SCREEN_WIDTH * 2)
#define ROAD_SIZE_Y			(SCREEN_HEIGHT / 5)	

// 道（背景）の座標
#define INIT_POS_X			(0.0f)
#define INIT_POS_Y			(SCREEN_HEIGHT * 0.8f)
#define MOVE_SPEED_X		(2.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexRoad(void);					// 頂点の作成
void SetVertexRoad(void);						// 頂点座標の設定
void SetColorRoad(void);						// 頂点カラーの設定
void SetTextureRoad(void);						// テクスチャ座標の設定


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMEROAD road;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRoad(int type)
{
	if (type == 0)
	{
		// テクスチャの読み込み
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_ROAD,
			&road.pTexture);
	}

	road.pos = D3DXVECTOR3(INIT_POS_X, INIT_POS_Y, 0.0f);
	road.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MakeVertexRoad();

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitRoad(void)
{
	SAFE_RELEASE(road.pTexture)
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateRoad(void)
{
	if (IsButtonPressed(0, BUTTON_RIGHT) || GetKeyboardPress(DIK_D))
	{
		road.pos.x -= MOVE_SPEED_X;
	}
	else if (IsButtonPressed(0, BUTTON_LEFT) || GetKeyboardPress(DIK_A))
	{
		road.pos.x += MOVE_SPEED_X;
	}

	if (road.pos.x < -ROAD_SIZE_X / 2)
	{
		road.pos.x = INIT_POS_X;
	}
	if (road.pos.x > INIT_POS_X)
	{
		road.pos.x = -ROAD_SIZE_X / 2;
	}

	SetVertexRoad();
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawRoad(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture( 0, road.pTexture );

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, road.vertexWk, sizeof(VERTEX_2D));
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexRoad(void)
{
	// 頂点座標の設定	
	SetVertexRoad();

	// 頂点カラーの設定
	SetColorRoad();

	// テクスチャ座標の設定
	SetTextureRoad();

	// rhwの設定
	road.vertexWk[0].rhw =
		road.vertexWk[1].rhw =
		road.vertexWk[2].rhw =
		road.vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexRoad(void)
{
	road.vertexWk[0].vtx = D3DXVECTOR3(road.pos.x, road.pos.y, road.pos.z);
	road.vertexWk[1].vtx = D3DXVECTOR3(road.pos.x + ROAD_SIZE_X, road.pos.y, road.pos.z);
	road.vertexWk[2].vtx = D3DXVECTOR3(road.pos.x, road.pos.y + ROAD_SIZE_Y, road.pos.z);
	road.vertexWk[3].vtx = D3DXVECTOR3(road.pos.x + ROAD_SIZE_X, road.pos.y + ROAD_SIZE_Y, road.pos.z);
}


//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorRoad(void)
{
	road.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	road.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	road.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	road.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureRoad(void)
{
	road.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	road.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	road.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	road.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
