//=============================================================================
//
// プレーヤー処理 [player.cpp]
// Author : 徐　ワイ延
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "debugproc.h"
#include "road.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT	 MakeVertexPlayer(void);
void	 SetVertexPlayer(void);
void	 SetTexturePlayer(int dir, int cntPattern);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
PLAYER	 player[PLAYER_MAX];


int		 tEXTURE_PLAYER_SIZE_X = 140;
int		 tEXTURE_PLAYER_SIZE_Y = 200;
int      tEXTURE_PATTERN_DIVIDE_X = 5;
int      aNIM_PATTERN_NUM = 10;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);	// プレイヤー０番のアドレスを取得する	
	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイスのポインタ
			TEXTURE_GAME_PLAYER,	// ファイルの名前
			&player->texture[0]);

		D3DXCreateTextureFromFile(pDevice,				// デバイスのポインタ
			TEXTURE_GAME_PLAYER2,	// ファイルの名前
			&player->texture[1]);
	}
	// プレイヤーの初期化処理	   
	player->pos = D3DXVECTOR3(tEXTURE_PLAYER_SIZE_X / 2, SCREEN_HEIGHT - tEXTURE_PLAYER_SIZE_Y, 0.0f);
	player->countAnim = 0;
	player->patternAnim = 0;
	player->direction = 1;
	player->moving_cooldown = 0;
	player->speed = 3.0f;
	player->status.HP = 6;
	player->status.LUCK = 0;
	D3DXVECTOR2 temp = D3DXVECTOR2(tEXTURE_PLAYER_SIZE_X, tEXTURE_PLAYER_SIZE_Y);
	player->radius = D3DXVec2Length(&temp);
	player->baseAngle = atan2f(tEXTURE_PLAYER_SIZE_Y, tEXTURE_PLAYER_SIZE_X);	// プレイヤーの角度を初期化
	// 頂点情報の作成
	MakeVertexPlayer();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	PLAYER *player = GetPlayer(0);
	SAFE_RELEASE(player->texture[0]);
	SAFE_RELEASE(player->texture[1]);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	PLAYER *player = GetPlayer(0);
	player->speed_boost = 1;
	if (GetKeyboardPress(DIK_LCONTROL) || GetKeyboardPress(DIK_RCONTROL) || IsButtonPressed(0, BUTTON_Y))
		player->speed_boost = 2.0f;
	if (player->status.LUCK >0)
		player->speed_boost = 3.0f;
	// アニメーション
	if (!GetBullet(0)->use)
		player->countAnim += player->speed * player->speed_boost * 0.1f;
	else
		player->countAnim += player->speed * player->speed_boost * 0.2f;
	if (player->moving_cooldown > 0)
	{
		player->patternAnim = (int)(player->countAnim) % aNIM_PATTERN_NUM;
		// テクスチャ座標を設定
		SetTexturePlayer(player->direction, player->patternAnim);
		if (player->patternAnim == 6)
			player->moving_cooldown--;
	}
	// 入力対応
	if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, BUTTON_DOWN))
	{
		player->moving_cooldown = 1;
		player->pos.y += player->speed * player->speed_boost;
	}
	if (GetKeyboardPress(DIK_UP) || IsButtonPressed(0, BUTTON_UP))
	{
		player->moving_cooldown = 1;
		player->pos.y -= player->speed * player->speed_boost;
	}
	if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT))
	{
		player->moving_cooldown = 1;
		player->direction = -1;
		player->pos.x -= player->speed * player->speed_boost;
	}
	if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT))
	{
		player->moving_cooldown = 1;
		player->direction = 1;
		player->pos.x += player->speed * player->speed_boost;
	}
	if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_A))
	{
		player->moving_cooldown = 1;
		D3DXVECTOR3 player_centre;
		player_centre.x = player->pos.x + player->direction * tEXTURE_PLAYER_SIZE_X / 2;
		player_centre.y = player->pos.y + tEXTURE_PLAYER_SIZE_Y;
		SetBullet(player_centre, player->status.ATK, player->direction);
		//tEXTURE_PLAYER_SIZE_X = 194;
		//tEXTURE_PLAYER_SIZE_Y = 209;
		//tEXTURE_PATTERN_DIVIDE_X = 3;
		//aNIM_PATTERN_NUM = 6;
	}
	//if (!GetBullet(0)->use)
	//{
	//	tEXTURE_PLAYER_SIZE_X = 140;
	//	tEXTURE_PLAYER_SIZE_Y = 200;
	//	tEXTURE_PATTERN_DIVIDE_X = 5;
	//	aNIM_PATTERN_NUM = 10;
	//}
	if (player->pos.x < 0)
	{
		player->pos.x = 0;
	}
	else if (player->pos.x > SCREEN_WIDTH - tEXTURE_PLAYER_SIZE_X)
	{
		player->pos.x = SCREEN_WIDTH - tEXTURE_PLAYER_SIZE_X;
	}
	else if (player->pos.y < SCREEN_HEIGHT * 0.85f - tEXTURE_PLAYER_SIZE_Y)
	{
		player->pos.y = SCREEN_HEIGHT * 0.85f - tEXTURE_PLAYER_SIZE_Y;
	}
	else if (player->pos.y > SCREEN_HEIGHT - tEXTURE_PLAYER_SIZE_Y)
	{
		player->pos.y = SCREEN_HEIGHT - tEXTURE_PLAYER_SIZE_Y;
	}

	// ●中込追加部分 ゴールが見えてないときはこの処理
	// ゴールが見えているときは画面端まで移動できるらしい
	if (player->pos.x > RIGHT_SCROLL_LINE_X)
	{
		player->pos.x = RIGHT_SCROLL_LINE_X;
	}
	else if (player->pos.x < LEFT_SCROLL_LINE_X)
	{
		player->pos.x = LEFT_SCROLL_LINE_X;
	}

	// Yの座標によって、Z(偽Depth)を変わる

	player->pos.z = 0;
	if (player->pos.y > SCREEN_HEIGHT * 0.8f - tEXTURE_PLAYER_SIZE_Y / 2)
	{
		player->pos.z = 3;
	}
	else if (player->pos.y > SCREEN_HEIGHT * 0.75f - tEXTURE_PLAYER_SIZE_Y / 2)
	{
		player->pos.z = 2;
	}

	else if (player->pos.y > SCREEN_HEIGHT * 0.7f - tEXTURE_PLAYER_SIZE_Y / 2)
	{
		player->pos.z = 1;
	}

	if (player->status.LUCK > 0)
	{
		player->status.LUCK--;
	}

	// 移動後の座標で頂点を設定
	SetVertexPlayer();

	PrintDebugProc(1, "Player_Y:%f\n", player->pos.y);
	PrintDebugProc(1, "Player_Z:%f\n", player->pos.z);

	PrintDebugProc(1, "Player_LUCK:%d\n", player->status.LUCK);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(int pno)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(pno);
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	BULLET *bullet = GetBullet(0);
	if (bullet->use)
		pDevice->SetTexture(0, player->texture[1]);
	else
		pDevice->SetTexture(0, player->texture[0]);
	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, player->vtx, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);
	// 頂点座標の設定
	SetVertexPlayer();
	// rhwの設定
	player->vtx[0].rhw =
	player->vtx[1].rhw =
	player->vtx[2].rhw =
	player->vtx[3].rhw = 1.0f;
	// 反射光の設定	
	player->vtx[0].diffuse =
	player->vtx[1].diffuse =
	player->vtx[2].diffuse =
	player->vtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// テクスチャ座標の設定
	player->vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	player->vtx[1].tex = D3DXVECTOR2(1.0f / tEXTURE_PATTERN_DIVIDE_X, 0.0f);
	player->vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	player->vtx[3].tex = D3DXVECTOR2(1.0f / tEXTURE_PATTERN_DIVIDE_X, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	// テクスチャの設定
	SetTexturePlayer(player->direction, player->patternAnim);
	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPlayer(void)
{
	PLAYER *player = GetPlayer(0);
	// 頂点座標の設定
	player->vtx[0].vtx = D3DXVECTOR3(player->pos.x, player->pos.y, 0);
	player->vtx[1].vtx = D3DXVECTOR3(player->pos.x + tEXTURE_PLAYER_SIZE_X, player->pos.y, 0);
	player->vtx[2].vtx = D3DXVECTOR3(player->pos.x, player->pos.y + tEXTURE_PLAYER_SIZE_Y, 0);
	player->vtx[3].vtx = D3DXVECTOR3(player->pos.x + tEXTURE_PLAYER_SIZE_X, player->pos.y + tEXTURE_PLAYER_SIZE_Y, 0);
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePlayer(int dir, int cntPattern)
{
	PLAYER *player = GetPlayer(0);
	// テクスチャ座標の設定
	int x = cntPattern % tEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / tEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / tEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;

	if (dir == -1)
	{
		player->vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		player->vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		player->vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		player->vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
	else
	{
		player->vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		player->vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		player->vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		player->vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
	// 反射光の設定
	if (player->status.LUCK == 0)
	{
		player->vtx[0].diffuse =
			player->vtx[1].diffuse =
			player->vtx[2].diffuse =
			player->vtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	else
	{
		player->vtx[0].diffuse =
			player->vtx[1].diffuse =
			player->vtx[2].diffuse =
			player->vtx[3].diffuse = D3DCOLOR_RGBA(255, 128, 255, 255);
	}
}
/*******************************************************************************
関数名:	PLAYER *GetMapAdr( int pno )
引数:	int pno : プレイヤー番号
戻り値:	PLAYER *
説明:	プレイヤーのアドレスを取得する
*******************************************************************************/
PLAYER *GetPlayer(int pno)
{
	return &player[pno];
}