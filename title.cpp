//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include "title.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ場所
#define TEXTURE_TITLE			_T("data/TEXTURE/title.png")

// タイトルのサイズ
#define TITLE_SIZE_X			(SCREEN_WIDTH)
#define TITLE_SIZE_Y			(SCREEN_HEIGHT)	

// タイトルの座標
#define INIT_POS_X				(0.0f)
#define MOVE_SPEED_X			(1.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(void);				// 頂点の作成
void SetVertexTitle(void);					// 頂点座標の設定
void SetColorTitle(void);					// 頂点カラーの設定
void SetTextureTitle(void);					// テクスチャ座標の設定


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMETITLE title;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_TITLE,
			&title.pTexture);
	}

	title.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	title.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MakeVertexTitle();

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	SAFE_RELEASE(title.pTexture)
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	// BGM再生
	PlayGameSound(BGM_TITLE, CONTINUE_SOUND, LOOP);
	if (IsButtonTriggered(0, BUTTON_A) || IsButtonTriggered(0, BUTTON_X)
		|| GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE))
	{
		StopAllSound(INIT_SOUND);	// 音を全て止める
		SetStage(GAME);				// ステージ遷移
	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, title.pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, title.vertexWk, sizeof(VERTEX_2D));
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(void)
{
	// 頂点座標の設定	
	SetVertexTitle();

	// 頂点カラーの設定
	SetColorTitle();

	// テクスチャ座標の設定
	SetTextureTitle();

	// rhwの設定
	title.vertexWk[0].rhw =
		title.vertexWk[1].rhw =
		title.vertexWk[2].rhw =
		title.vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexTitle(void)
{
	title.vertexWk[0].vtx = D3DXVECTOR3(title.pos.x, title.pos.y, title.pos.z);
	title.vertexWk[1].vtx = D3DXVECTOR3(title.pos.x + TITLE_SIZE_X, title.pos.y, title.pos.z);
	title.vertexWk[2].vtx = D3DXVECTOR3(title.pos.x, title.pos.y + TITLE_SIZE_Y, title.pos.z);
	title.vertexWk[3].vtx = D3DXVECTOR3(title.pos.x + TITLE_SIZE_X, title.pos.y + TITLE_SIZE_Y, title.pos.z);
}


//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorTitle(void)
{
	title.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	title.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	title.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	title.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureTitle(void)
{
	title.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	title.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	title.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	title.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
