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
#include "score.h"
#include "life.h"
#include "result.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ場所
#define TEXTURE_GAME_CLEAR		_T("data/TEXTURE/game_clear.png")
#define TEXTURE_GAME_OVER		_T("data/TEXTURE/gameover.png")

// リザルトのサイズ
#define RESULT_SIZE_X			(300.0f)
#define RESULT_SIZE_Y			(50.0f)	

// リザルトの座標
#define RESULT_POS_X			(362.0f)
#define RESULT_POS_Y			(100.0f)
#define RESULT_SCORE_POS		(D3DXVECTOR3(590.0f, 170.0f, 0.0f))


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(GAMERESULT *result);	// 頂点の作成
void SetVertexResult(GAMERESULT *result);		// 頂点座標の設定
void SetColorResult(GAMERESULT *result);		// 頂点カラーの設定
void SetTextureResult(GAMERESULT *result);		// テクスチャ座標の設定


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMERESULT	result;
BOOL		isSoundPlay;		// 音再生するか

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
	result.pos = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y, 0.0f);
	result.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	result.use = TRUE;
	result.cntBlink = 0;
	MakeVertexResult(&result);

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
	gameData->isGameClear = TRUE;

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
			GAMEBG *bg = GetBg();
			SetColorBg(bg, D3DCOLOR_RGBA(150, 150, 150, 255));
		}
		isSoundPlay = FALSE;
	}

	// 点滅
	result.cntBlink++;
	if (result.cntBlink == SECOND(0.7))
	{
		result.use = FALSE;
	}
	if (result.cntBlink == SECOND(1.2))
	{
		result.use = TRUE;
		result.cntBlink = 0;
	}

	// スコア
	GAMESCORE *score = GetScore();
	score->pos = RESULT_SCORE_POS;
	PlaceDigit(score);
	SetVertexScore(score->digit);

	// ステージ遷移
	if (IsButtonTriggered(0, BUTTON_A) || GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE))
	{
		StopAllSound(INIT_SOUND);
		gameData->isGameClear = FALSE;
		gameData->isCombo = FALSE;
		gameData->numCombo = 0;
		InitPlayer(REINIT);
		InitEnemy(REINIT);
		InitBg(REINIT);
		InitRoad(REINIT);
		InitTimer(REINIT);
		InitScore(REINIT);
		InitLife(REINIT);
		InitResult(REINIT);
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
			pDevice->SetTexture(0, result.pTexture[GAME_CLEAR]);

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
	if (result.use)
	{
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, result.vertexWk, sizeof(VERTEX_2D));
	}
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(GAMERESULT *result)
{
	// 頂点座標の設定	
	SetVertexResult(result);

	// 頂点カラーの設定
	SetColorResult(result);

	// テクスチャ座標の設定
	SetTextureResult(result);

	// rhwの設定
	result->vertexWk[0].rhw =
		result->vertexWk[1].rhw =
		result->vertexWk[2].rhw =
		result->vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexResult(GAMERESULT *result)
{
	result->vertexWk[0].vtx = D3DXVECTOR3(result->pos.x, result->pos.y, result->pos.z);
	result->vertexWk[1].vtx = D3DXVECTOR3(result->pos.x + RESULT_SIZE_X, result->pos.y, result->pos.z);
	result->vertexWk[2].vtx = D3DXVECTOR3(result->pos.x, result->pos.y + RESULT_SIZE_Y, result->pos.z);
	result->vertexWk[3].vtx = D3DXVECTOR3(result->pos.x + RESULT_SIZE_X, result->pos.y + RESULT_SIZE_Y, result->pos.z);
}


//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorResult(GAMERESULT *result)
{
	result->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureResult(GAMERESULT *result)
{
	result->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	result->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	result->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	result->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
