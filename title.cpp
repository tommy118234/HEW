//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "quadrangle.h"
#include "player.h"
#include "title.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ場所
#define TEXTURE_TITLE			_T("data/TEXTURE/title.png")
#define TEXTURE_MENU_START		_T("data/TEXTURE/start.png")
#define TEXTURE_MENU_EXIT		_T("data/TEXTURE/exit.png")
#define TEXTURE_CURSOR			_T("data/TEXTURE/cursor.png")

// タイトルのサイズ
#define TITLE_SIZE_X			(SCREEN_WIDTH)
#define TITLE_SIZE_Y			(SCREEN_HEIGHT)	

// タイトルの座標
#define INIT_POS_X				(0.0f)
#define MOVE_SPEED_X			(1.0f)

// メニュ―の種類
enum MENU_TYPE
{
	MENU_START,					// ゲームスタート
	MENU_EXIT,					// ゲーム終了
	MENU_TYPE_MAX				// メニューの数
};

// メニュー
#define MENU_SIZE_X				(250.0f)
#define MENU_SIZE_Y				(150.0f)
#define START_POS_X				(200.0f)
#define EXIT_POS_X				(500.0f)
#define MENU_POS_Y				(450.0f)

// カーソル
#define CURSOR_SIZE_X			(45.0f)
#define CURSOR_SIZE_Y			(80.0f)
#define CURSOR_POS_X			(500.0f)
#define CURSOR_POS_Y			(530.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(void);					// 頂点の作成
void SetVertexTitle(void);						// 頂点座標の設定
void SetColorTitle(void);						// 頂点カラーの設定
void SetTextureTitle(void);						// テクスチャ座標の設定


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMETITLE title;
int	selectNo;	

// @QUADRANGLE構造体で手抜き実装
QUADRANGLE menu[MENU_TYPE_MAX];					// メニュー
LPDIRECT3DTEXTURE9 menuTex[MENU_TYPE_MAX];		// メニュ―テクスチャ
QUADRANGLE cursor;								// カーソル
LPDIRECT3DTEXTURE9 cursorTex;					// カーソルテクスチャ


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

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_MENU_START,
			&menuTex[MENU_START]);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_MENU_EXIT,
			&menuTex[MENU_EXIT]);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_CURSOR,
			&cursorTex);
	}

	// BG初期化
	title.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	title.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MakeVertexTitle();

	// メニュー
	InitQuad(&menu[MENU_START], MENU_SIZE_X, MENU_SIZE_Y);
	InitQuad(&menu[MENU_EXIT], MENU_SIZE_X, MENU_SIZE_Y);
	menu[MENU_START].transform.pos = D3DXVECTOR3(START_POS_X, MENU_POS_Y, 0.0f);
	menu[MENU_EXIT].transform.pos = D3DXVECTOR3(EXIT_POS_X, MENU_POS_Y, 0.0f);
	SetVertexQuad(&menu[MENU_START]);
	SetVertexQuad(&menu[MENU_EXIT]);

	// カーソル
	InitQuad(&cursor, CURSOR_SIZE_X, CURSOR_SIZE_Y);
	cursor.transform.pos = D3DXVECTOR3(CURSOR_POS_X, CURSOR_POS_Y, 0.0f);
	SetVertexQuad(&cursor);

	selectNo = MENU_START;

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

	// 選択
	if (IsButtonTriggered(0, BUTTON_RIGHT) || GetKeyboardTrigger(DIK_RIGHT)
		|| IsButtonTriggered(0, BUTTON_LEFT) || GetKeyboardTrigger(DIK_LEFT))
	{
		selectNo = selectNo ? MENU_START : MENU_EXIT;
		PlayGameSound(SE_MENU_CURSOR, INIT_SOUND, NONE);	// カーソル音
	}

	// 決定
	if (IsButtonTriggered(0, BUTTON_A) || IsButtonTriggered(0, BUTTON_X)
		|| GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE))
	{
		StopAllSound(INIT_SOUND);							// 音を全て止める
		PlayGameSound(SE_MENU_DECISION, INIT_SOUND, NONE);	// 決定音
		if (selectNo == MENU_START) SetStage(GAME);			// ゲームに遷移
		if (selectNo == MENU_EXIT)	SetStage(EXIT);			// ゲーム終了
	}

	// カーソルの場所
	selectNo ? cursor.transform.pos.x = EXIT_POS_X : cursor.transform.pos.x = START_POS_X;
	SetVertexQuad(&cursor);

	// メニュー
	for (int i = 0; i < MENU_TYPE_MAX; i++)
	{
		menu[i].radius = menu[i].originalRadius;
	}
	menu[selectNo].radius = menu[selectNo].radius * 1.2f;
	SetVertexQuad(&menu[MENU_START]);
	SetVertexQuad(&menu[MENU_EXIT]);
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// タイトルBG
	pDevice->SetTexture(0, title.pTexture);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, title.vertexWk, sizeof(VERTEX_2D));

	// メニュー
	for (int i = 0; i < MENU_TYPE_MAX; i++)
	{
		pDevice->SetTexture(0, menuTex[i]);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, menu[i].vertexWk, sizeof(VERTEX_2D));
	}

	// カーソル
	pDevice->SetTexture(0, cursorTex);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, cursor.vertexWk, sizeof(VERTEX_2D));
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
