//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include "enemy.h"
#include "bg.h"
#include "road.h"
#include "timer.h"
#include "result.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ場所
#define TEXTURE_GAME_CLEAR		_T("data/TEXTURE/game_clear.png")
#define TEXTURE_GAME_OVER		_T("data/TEXTURE/game_over.png")

// リザルトのサイズ
#define RESULT_SIZE_X			(SCREEN_WIDTH)
#define RESULT_SIZE_Y			(SCREEN_HEIGHT)	

// リザルトの座標
#define INIT_POS_X				(0.0f)
#define MOVE_SPEED_X			(1.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(void);	// 頂点の作成
void SetVertexResult(void);		// 頂点座標の設定
void SetColorResult(void);		// 頂点カラーの設定
void SetTextureResult(void);	// テクスチャ座標の設定


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMERESULT	result;
BOOL		isSoundPlay;	// 音再生するか

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_GAME_CLEAR,
			&result.pTexture[GAME_CLEAR]);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_GAME_OVER,
			&result.pTexture[GAME_OVER]);
	}

	isSoundPlay = TRUE;
	result.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	result.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MakeVertexResult();

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	SAFE_RELEASE(result.pTexture[GAME_CLEAR])
	SAFE_RELEASE(result.pTexture[GAME_OVER])
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	GAMEDATA *gameData = GetGameData();

	// サウンド
	if (isSoundPlay)
	{
		if (gameData->isGameClear)
		{
			PlayGameSound(SE_GAME_CLEAR, INIT_SOUND, NONE);
		}
		else
		{
			PlayGameSound(SE_GAME_OVER, INIT_SOUND, NONE);
		}
		isSoundPlay = FALSE;
	}

	// ステージ遷移
	if (IsButtonTriggered(0, BUTTON_A) || GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE))
	{
		GetGameData()->isGameClear = FALSE;
		InitPlayer(REINIT);
		InitEnemy(REINIT);
		InitBg(REINIT);
		InitRoad(REINIT);
		InitTimer(REINIT);
		SetStage(TITLE);
	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	if (GetGameData()->isGameClear)
	{
		pDevice->SetTexture(0, result.pTexture[GAME_CLEAR]);
	}
	else
	{
		pDevice->SetTexture(0, result.pTexture[GAME_OVER]);
	}

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, result.vertexWk, sizeof(VERTEX_2D));
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(void)
{
	// 頂点座標の設定	
	SetVertexResult();

	// 頂点カラーの設定
	SetColorResult();

	// テクスチャ座標の設定
	SetTextureResult();

	// rhwの設定
	result.vertexWk[0].rhw =
		result.vertexWk[1].rhw =
		result.vertexWk[2].rhw =
		result.vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexResult(void)
{
	result.vertexWk[0].vtx = D3DXVECTOR3(result.pos.x, result.pos.y, result.pos.z);
	result.vertexWk[1].vtx = D3DXVECTOR3(result.pos.x + RESULT_SIZE_X, result.pos.y, result.pos.z);
	result.vertexWk[2].vtx = D3DXVECTOR3(result.pos.x, result.pos.y + RESULT_SIZE_Y, result.pos.z);
	result.vertexWk[3].vtx = D3DXVECTOR3(result.pos.x + RESULT_SIZE_X, result.pos.y + RESULT_SIZE_Y, result.pos.z);
}


//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorResult(void)
{
	result.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureResult(void)
{
	result.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	result.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	result.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	result.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
