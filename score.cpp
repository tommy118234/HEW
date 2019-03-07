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
#define TEXTURE_SCORE				_T("data/TEXTURE/number.png")
#define TEXTURE_PATTERN_DIVIDE_X	(10)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)

// 桁のサイズ
#define DIGIT_WIDTH					(35)
#define DIGIT_HEIGHT				(50)	

// スコア座標
#define SCORE_POS_X					(950)		// 1桁目（右端）の桁の座標となる
#define SCORE_POS_Y					(50.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexScore(DIGIT *digit);					// 頂点の作成
void SetVertexScore(DIGIT *digit);						// 頂点座標の設定
void SetColorScore(DIGIT *digit);						// 頂点カラーの設定
void SetTextureScore(DIGIT *digit, int cntPattern);		// テクスチャ座標の設定


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMESCORE score;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(int type)
{
	if (type == 0)
	{
		// テクスチャの読み込み
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_SCORE,
			&score.pTexture);
	}

	// スコア初期化
	score.pos = D3DXVECTOR3(SCORE_POS_X, SCORE_POS_Y, 0.0f);
	score.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	score.value = 0;

	// 各桁初期化
	for (int i = 0; i < SCORE_DIGIT_MAX; i++)
	{
		score.digit[i].number = 0;
		// 右から並べている
		score.digit[i].pos.x = score.pos.x - i * DIGIT_WIDTH;
		score.digit[i].pos.y = score.pos.y = score.pos.y;
		MakeVertexScore(&score.digit[i]);
	}

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
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_O))
	{
		AddScore(100);
	}
#endif

	// スコア各桁の数字を算出
	int wk = score.value;
	for (int i = 0; i < SCORE_DIGIT_MAX; i++)
	{
		score.digit[i].number = wk % 10;
		wk /= 10;

		//テクスチャ座標の設定
		SetTextureScore(&score.digit[i], score.digit[i].number);
	}

}


//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture( 0, score.pTexture );

	// ポリゴンの描画
	for (int i = 0; i < SCORE_DIGIT_MAX; i++)
	{
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, score.digit[i].vertexWk, sizeof(VERTEX_2D));
	}
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexScore(DIGIT *digit)
{
	// 頂点座標の設定	
	SetVertexScore(digit);

	// 頂点カラーの設定
	SetColorScore(digit);

	// テクスチャ座標の設定
	SetTextureScore(digit, digit->number);

	// rhwの設定
	digit->vertexWk[0].rhw =
		digit->vertexWk[1].rhw =
		digit->vertexWk[2].rhw =
		digit->vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexScore(DIGIT *digit)
{
	digit->vertexWk[0].vtx = D3DXVECTOR3(digit->pos.x, digit->pos.y, digit->pos.z);
	digit->vertexWk[1].vtx = D3DXVECTOR3(digit->pos.x + DIGIT_WIDTH, digit->pos.y, digit->pos.z);
	digit->vertexWk[2].vtx = D3DXVECTOR3(digit->pos.x, digit->pos.y + DIGIT_HEIGHT, digit->pos.z);
	digit->vertexWk[3].vtx = D3DXVECTOR3(digit->pos.x + DIGIT_WIDTH, digit->pos.y + DIGIT_HEIGHT, digit->pos.z);
}


//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorScore(DIGIT *digit)
{
	digit->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	digit->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	digit->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	digit->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureScore(DIGIT *digit, int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X;		//テクスチャパターンがX軸(U軸)方向に何枚目かカウント
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X;		//テクスチャパターンがY軸(V軸)方向に何枚目かカウント
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X;		//１パターンのU値
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;		//１パターンのV値

	digit->vertexWk[0].tex = D3DXVECTOR2((float)(x) * sizeX, (float)(y) * sizeY);
	digit->vertexWk[1].tex = D3DXVECTOR2((float)(x) * sizeX + sizeX, (float)(y) * sizeY);
	digit->vertexWk[2].tex = D3DXVECTOR2((float)(x) * sizeX, (float)(y) * sizeY + sizeY);
	digit->vertexWk[3].tex = D3DXVECTOR2((float)(x) * sizeX + sizeX, (float)(y) * sizeY + sizeY);
}


//=============================================================================
// スコアを加算
//=============================================================================
void AddScore(int addValue)
{
	score.value += addValue;
}
