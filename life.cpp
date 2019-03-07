//=============================================================================
//
// ライフ処理 [life.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include "life.h"
#include "font.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ場所
#define TEXTURE_HEART		_T("data/TEXTURE/heart.png")
#define TEXTURE_HALF_HEART	_T("data/TEXTURE/half_heart.png")
#define TEXTURE_BLANK_HEART	_T("data/TEXTURE/blank_heart.png")

// ライフのサイズ
#define HEART_SIZE			(50)
#define HEART_SPAN			(5)

// ライフの座標
#define INIT_POS_X			(100.0f)
#define INIT_POS_Y			(50.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexHeart(HEART* heart);			// 頂点の作成
void SetVertexHeart(HEART* heart);				// 頂点座標の設定
void SetColorHeart(HEART* heart);				// 頂点カラーの設定
void SetTextureHeart(HEART* heart);				// テクスチャ座標の設定


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMELIFE life;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLife(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// @テクスチャの読み込み
		// ライフ３つ
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_HEART,
			&life.pTexHeart);

		// 半分のハート
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_HALF_HEART,
			&life.pTexHalfHeart);

		// 空ハート
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_BLANK_HEART,
			&life.pTexBlankHeart);
	}

	// 全体の表示場所
	life.pos = D3DXVECTOR3(INIT_POS_X, INIT_POS_Y, 0.0f);
	life.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ハートの表示場所
	for (int i = 0; i < HEART_MAX; i++)
	{
		life.heart[i].pos = D3DXVECTOR3(life.pos.x + i * (HEART_SIZE + HEART_SPAN), life.pos.y, 0.0f);
		life.heart[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		life.heart[i].drawTex = FULL_HEART;
		MakeVertexHeart(&life.heart[i]);
	}

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitLife(void)
{
	SAFE_RELEASE(life.pTexHeart)
	SAFE_RELEASE(life.pTexHalfHeart)
	SAFE_RELEASE(life.pTexBlankHeart)
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateLife(void)
{
#ifdef _DEBUG
	if(GetKeyboardTrigger(DIK_P))
	{
		GetPlayer(0)->status.HP--;
	}
#endif

	int hp = GetPlayer(0)->status.HP;
	
	// ハートの表示場所
	for (int i = 0; i < HEART_MAX; i++)
	{
		// 半分ハート
		if (hp % 2 && (hp == (i + 1) * 2 - 1))
		{
			life.heart[i].drawTex = HALF_HEART;
			continue;
		}

		// 空のハート
		if (hp < (i + 1) * 2)
		{
			life.heart[i].drawTex = BLANK_HEART;
		}
	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < HEART_MAX; i++)
	{
		// テクスチャの設定
		if (life.heart[i].drawTex == FULL_HEART)
		{
			pDevice->SetTexture(0, life.pTexHeart);
		}
		else if (life.heart[i].drawTex == HALF_HEART)
		{
			pDevice->SetTexture(0, life.pTexHalfHeart);
		}
		else if (life.heart[i].drawTex == BLANK_HEART)
		{
			pDevice->SetTexture(0, life.pTexBlankHeart);
		}

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, life.heart[i].vertexWk, sizeof(VERTEX_2D));
	}
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexHeart(HEART *heart)
{
	// 頂点座標の設定	
	SetVertexHeart(heart);

	// 頂点カラーの設定
	SetColorHeart(heart);

	// テクスチャ座標の設定
	SetTextureHeart(heart);

	// rhwの設定
	heart->vertexWk[0].rhw =
		heart->vertexWk[1].rhw =
		heart->vertexWk[2].rhw =
		heart->vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexHeart(HEART *heart)
{
	heart->vertexWk[0].vtx = D3DXVECTOR3(heart->pos.x, heart->pos.y, heart->pos.z);
	heart->vertexWk[1].vtx = D3DXVECTOR3(heart->pos.x + HEART_SIZE, heart->pos.y, heart->pos.z);
	heart->vertexWk[2].vtx = D3DXVECTOR3(heart->pos.x, heart->pos.y + HEART_SIZE, heart->pos.z);
	heart->vertexWk[3].vtx = D3DXVECTOR3(heart->pos.x + HEART_SIZE, heart->pos.y + HEART_SIZE, heart->pos.z);
}


//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorHeart(HEART *heart)
{
	heart->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	heart->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	heart->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	heart->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureHeart(HEART *heart)
{
	heart->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	heart->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	heart->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	heart->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

