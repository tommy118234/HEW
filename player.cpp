//=============================================================================
//
// ポリゴン処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer(void);
void SetVertexPlayer(void);
void SetTexturePlayer( int cntPattern );	

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePlayer = NULL;		// テクスチャへのポリゴン
VERTEX_2D				g_vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク  
PLAYER					player[PLAYER_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);	// プレイヤー０番のアドレスを取得する	
	//player->g_posPlayer = D3DXVECTOR3(SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X/2, SCREEN_CENTER_Y - TEXTURE_SAMPLE00_SIZE_Y/2, 0.0f);
	player->g_posPlayer = D3DXVECTOR3(SCREEN_CENTER_X - TEXTURE_SAMPLE00_SIZE_X / 2, SCREEN_CENTER_Y - TEXTURE_SAMPLE00_SIZE_Y / 2, 0.0f);

	player->g_posPlayer = D3DXVECTOR3(SCREEN_CENTER_X + TEXTURE_SAMPLE00_SIZE_X / 2, SCREEN_CENTER_Y + TEXTURE_SAMPLE00_SIZE_Y / 2, 0.0f);
	player->g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->g_nCountAnim = 0;
	player->g_nPatternAnim = 0;
	// 頂点情報の作成
	MakeVertexPlayer();
	// テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice,		// デバイスのポインタ
		TEXTURE_GAME_SAMPLE00,				// ファイルの名前
		&g_pD3DTexturePlayer );			// 読み込むメモリのポインタ	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if( g_pD3DTexturePlayer != NULL )	//
	{	// テクスチャの開放
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{			
	//player->bullet_num = 0;
	
	// アニメーション	
	player->g_nCountAnim++;
	if ((player->g_nCountAnim % FPS) != 0)
	{
		player->g_nPatternAnim = (player->g_nPatternAnim + 1) % ANIM_PATTERN_NUM;
	}

	if (player->g_posPlayer.x < 0) 
	{
		player->g_posPlayer.x = SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X / 2;
	}
	if (player->g_posPlayer.x > SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X / 2)
	{
		player->g_posPlayer.x = 0;
	}
	if (player->g_posPlayer.y < 0)
	{
		player->g_posPlayer.y = SCREEN_HEIGHT - TEXTURE_SAMPLE00_SIZE_Y;
	}
	if (player->g_posPlayer.y > SCREEN_HEIGHT - TEXTURE_SAMPLE00_SIZE_Y)
	{
		player->g_posPlayer.y = 0;
	}

	// 入力対応
	if (GetKeyboardPress(DIK_DOWN)) {
		player->g_posPlayer.y += 5;
	}
	if (GetKeyboardPress(DIK_UP)) {
		player->g_posPlayer.y -= 5;
	}
	if (GetKeyboardPress(DIK_LEFT)) {
		player->g_posPlayer.x -= 5;
	}
	if (GetKeyboardPress(DIK_RIGHT)) {		
		player->g_posPlayer.x += 5;
	}
	if (GetKeyboardPress(DIK_SPACE) ) {	
		//BULLET *bullet = GetBullet(player->bullet_num % BULLET_MAX);
		//while (!bullet->use) {
			//OutputDebugStringA("\nSPACE\n");			
		BULLET *bullet = GetBullet(player->bullet_num );
		//Sleep(20);
		bullet->use = TRUE;
		player->bullet_num = (++player->bullet_num % (BULLET_MAX));
			
	}
  	MakeVertexPlayer();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture( 0, g_pD3DTexturePlayer );
	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWk, sizeof(VERTEX_2D));	
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexPlayer();
	// rhwの設定
	g_vertexWk[0].rhw =
	g_vertexWk[1].rhw =
	g_vertexWk[2].rhw =
	g_vertexWk[3].rhw = 1.0f;
	// 反射光の設定
	g_vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// テクスチャ座標の設定
	SetTexturePlayer(player->g_nPatternAnim);
	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPlayer(void)
{
	// 頂点座標の設定
	g_vertexWk[0].vtx = D3DXVECTOR3(player->g_posPlayer.x, player->g_posPlayer.y, player->g_posPlayer.z);
	g_vertexWk[1].vtx = D3DXVECTOR3(player->g_posPlayer.x + TEXTURE_SAMPLE00_SIZE_X, player->g_posPlayer.y, player->g_posPlayer.z);
	g_vertexWk[2].vtx = D3DXVECTOR3(player->g_posPlayer.x, player->g_posPlayer.y + TEXTURE_SAMPLE00_SIZE_Y, player->g_posPlayer.z);
	g_vertexWk[3].vtx = D3DXVECTOR3(player->g_posPlayer.x + TEXTURE_SAMPLE00_SIZE_X, player->g_posPlayer.y + TEXTURE_SAMPLE00_SIZE_Y, player->g_posPlayer.z);
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePlayer( int cntPattern )
{
	// テクスチャ座標の設定
	g_vertexWk[0].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_X, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_Y);
	g_vertexWk[1].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X) + 1)/ (float)TEXTURE_PATTERN_DIVIDE_X, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_Y);
	g_vertexWk[2].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_X, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y);
	g_vertexWk[3].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X) + 1)/ (float)TEXTURE_PATTERN_DIVIDE_Y);
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