//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "item.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ場所
#define TEXTURE_ITEM		_T("data/TEXTURE/pop_corn.png")

// アイテムの座標
#define INIT_POS_X			(100.0f)
#define MOVE_SPEED_X		(2.0f)

/* アイテムセット関係 */
#define IEMSET_RAND_RANGE	(1)					// アイテム出現頻度の調整値
#define ITEM_SET_NUMBER		(0)					// アイテム出現
#define LANE_MAX			(4)					// 仮想レーンの数
// リポップ時
#define SET_POS_Y(_lane)	(SCREEN_HEIGHT * 0.5f + _lane * (SCREEN_HEIGHT * 0.1f))	// 座標
#define SET_SIZE_X(_lane)	(ITEM_SIZE_X + _lane * ITEM_SIZE_X * 0.5)				// サイズX
#define SET_SIZE_Y(_lane)	(ITEM_SIZE_Y + _lane * ITEM_SIZE_Y * 0.5)				// サイズY
// サイン波
#define WAVE_CNT_UNIT		(0.02f)				// カウント単位radian
#define WAVE_HEIGHT			(2.0f)				// 高さ



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexItem(ITEM *item);				// 頂点の作成
void SetVertexItem(ITEM *item);					// 頂点座標の設定
void SetTextureItem(ITEM *item);				// テクスチャ座標の設定
void SetItemGameStage(void);					// アイテムをセット


//*****************************************************************************
// グローバル変数
//*****************************************************************************
ITEM item;
float waveAngleCounter;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitItem(int type)
{
	if (type == 0)
	{
		// テクスチャの読み込み
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_ITEM,
			&item.pTexture);
	}

	item.pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	item.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	item.use = FALSE;
	item.sizeX = ITEM_SIZE_X;
	item.sizeY = ITEM_SIZE_Y;
	MakeVertexItem(&item);

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	SAFE_RELEASE(item.pTexture)
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{
	if (item.use == FALSE)
	{
		item.cntNonUseTime++;
		if (item.cntNonUseTime > SECOND(1))
		{
			SetItemGameStage();
		}
		return;
	}

	waveAngleCounter += WAVE_CNT_UNIT;
	  
	// アイテムの移動
	item.pos.x -= MOVE_SPEED_X;
	item.pos.y += WAVE_HEIGHT * sin(waveAngleCounter);


	// 画面端で消去
	if (item.pos.x + ITEM_SIZE_X < 0.0f)
	{
		item.use = FALSE;
	}

	SetVertexItem(&item);
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(int itemNo)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture( 0, item.pTexture );

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, item.vertexWk, sizeof(VERTEX_2D));
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexItem(ITEM *item)
{
	// 頂点座標の設定	
	SetVertexItem(item);

	// 頂点カラーの設定
	SetColorItem(item, D3DCOLOR_RGBA(255, 255, 255, 255));

	// テクスチャ座標の設定
	SetTextureItem(item);

	// rhwの設定
	item->vertexWk[0].rhw =
		item->vertexWk[1].rhw =
		item->vertexWk[2].rhw =
		item->vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexItem(ITEM *item)
{
	item->vertexWk[0].vtx = D3DXVECTOR3(item->pos.x, item->pos.y, item->pos.z);
	item->vertexWk[1].vtx = D3DXVECTOR3(item->pos.x + item->sizeX, item->pos.y, item->pos.z);
	item->vertexWk[2].vtx = D3DXVECTOR3(item->pos.x, item->pos.y + item->sizeY, item->pos.z);
	item->vertexWk[3].vtx = D3DXVECTOR3(item->pos.x + item->sizeX, item->pos.y + item->sizeY, item->pos.z);
}


//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorItem(ITEM *item, D3DCOLOR setCol)
{
	item->vertexWk[0].diffuse = setCol;
	item->vertexWk[1].diffuse = setCol;
	item->vertexWk[2].diffuse = setCol;
	item->vertexWk[3].diffuse = setCol;
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureItem(ITEM *item)
{
	item->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	item->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	item->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	item->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


//=============================================================================
// ITEMの取得
//=============================================================================
ITEM *GetItem(void)
{
	return &item;
}


//=============================================================================
// アイテムをセット
//=============================================================================
void SetItemGameStage(void)
{
	// 出現頻度調整
	int number = rand() % IEMSET_RAND_RANGE;
	if (number != ITEM_SET_NUMBER) { return; }
	
	// レーンをランダムに決定
	int lane = -1;
	lane = rand() % LANE_MAX;

	// 座標と大きさを設定しリポップの準備
	item.use = TRUE;
	item.pos = D3DXVECTOR3((float)SCREEN_WIDTH, (float)SET_POS_Y(lane), (float)lane);
	item.sizeX = SET_SIZE_X(lane);
	item.sizeY = SET_SIZE_Y(lane);
}
