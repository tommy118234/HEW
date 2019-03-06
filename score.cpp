//=============================================================================
//
// スコア処理 [score.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "score.h"
#include "font.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ場所
//#define TEXTURE_SCORE		_T("data/TEXTURE/score.png")

// スコアのサイズ
#define SCORE_SIZE_X		(SCREEN_WIDTH * 2)
#define SCORE_SIZE_Y		(SCREEN_HEIGHT / 5)	

// スコア座標
#define SCORE_POS_X			(800)
#define SCORE_POS_Y			(100)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexScore(void);					// 頂点の作成
void SetVertexScore(void);						// 頂点座標の設定
void SetColorScore(void);						// 頂点カラーの設定
void SetTextureScore(void);						// テクスチャ座標の設定


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMESCORE score;
static TCHAR text[256];


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(int type)
{
	if (type == 0)
	{
		//// テクスチャの読み込み
		//LPDIRECT3DDEVICE9 pDevice = GetDevice();
		//D3DXCreateTextureFromFile(pDevice,
		//	TEXTURE_SCORE,
		//	&score.pTexture);
	}

	score.pos = D3DXVECTOR3(SCORE_POS_X, SCORE_POS_Y, 0.0f);
	score.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MakeVertexScore();

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	SAFE_RELEASE(score.pTexture)
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{


	// ●仮の表示
	wsprintf(text, _T("%d\n"), score.value);

	SetVertexScore();
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ●仮の表示
	LPD3DXFONT font = GetFont(IMPACT);
	RECT rect = { SCORE_POS_X, SCORE_POS_Y, SCREEN_WIDTH, SCREEN_HEIGHT };
	font->DrawText(NULL, text, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	//// 頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_2D);

	//// テクスチャの設定
	//pDevice->SetTexture( 0, score.pTexture );

	//// ポリゴンの描画
	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, score.vertexWk, sizeof(VERTEX_2D));
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexScore(void)
{
	// 頂点座標の設定	
	SetVertexScore();

	// 頂点カラーの設定
	SetColorScore();

	// テクスチャ座標の設定
	SetTextureScore();

	// rhwの設定
	score.vertexWk[0].rhw =
		score.vertexWk[1].rhw =
		score.vertexWk[2].rhw =
		score.vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexScore(void)
{
	score.vertexWk[0].vtx = D3DXVECTOR3(score.pos.x, score.pos.y, score.pos.z);
	score.vertexWk[1].vtx = D3DXVECTOR3(score.pos.x + SCORE_SIZE_X, score.pos.y, score.pos.z);
	score.vertexWk[2].vtx = D3DXVECTOR3(score.pos.x, score.pos.y + SCORE_SIZE_Y, score.pos.z);
	score.vertexWk[3].vtx = D3DXVECTOR3(score.pos.x + SCORE_SIZE_X, score.pos.y + SCORE_SIZE_Y, score.pos.z);
}


//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorScore(void)
{
	score.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	score.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	score.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	score.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureScore(void)
{
	score.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	score.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	score.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	score.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


//=============================================================================
// スコアを加算
//=============================================================================
void AddScore(int addValue)
{
	score.value += addValue;
}
