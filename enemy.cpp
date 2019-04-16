//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : HAL東京 2年制ゲーム学科 GP11B341 16 徐　ワイ延
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "bullet.h"
#include "debugproc.h"
#include "math.h"
#include "time.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT	 MakeVertexEnemy(int no);
void	 SetVertexEnemy(int no);
void	 SetTextureEnemy(int no, int cntPattern);
void	 SetEnemy(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
ENEMY			   enemy[ENEMY_MAX];			// エネミー構造体
LPDIRECT3DTEXTURE9 enemyTexture[2];				// 二種類のエネミー
int				   enemy_count,enemy_rate;		// エネミーの出現頻度



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(int type)
{
	srand(time(NULL));
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);		// エネミーのポインターを初期化	
	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイスのポインタ
			TEXTURE_GAME_ENEMY1,	// ファイルの名前
			&enemyTexture[0]);
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイスのポインタ
			TEXTURE_GAME_ENEMY2,	// ファイルの名前
			&enemyTexture[1]);
	}
	for (int i = 0; i < ENEMY_MAX; i++,enemy++) 
	{
		// エネミーの初期化処理	   
		enemy->pos = D3DXVECTOR3(SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X,
								 SCREEN_CENTER_Y/4 + rand() % SCREEN_CENTER_Y,
								 0.0f);
		enemy->use = false;
		enemy->countAnim = 0;
		enemy->patternAnim = 0;
		enemy->moving_cooldown = 0;
		enemy->speed = 1.0f;
		enemy->status.ATK = 5;
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y);
		enemy->radius = D3DXVec2Length(&temp);
		enemy->baseAngle = atan2f(TEXTURE_ENEMY_SIZE_Y, TEXTURE_ENEMY_SIZE_X);	// プレイヤーの角度を初期化
		enemy->type = rand()%2;		
		MakeVertexEnemy(i);			// 頂点情報の作成
	}
	enemy_count = 0;
	enemy_rate = 300;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	SAFE_RELEASE(enemyTexture[0]);
	SAFE_RELEASE(enemyTexture[1]);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	ENEMY *enemy = GetEnemy(0);
	// アニメーション	
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == true)
		{
			enemy->countAnim += enemy->speed * 0.2f;
			enemy->patternAnim = (int)(enemy->countAnim) % ANIM_PATTERN_NUM;
			// テクスチャ座標を設定
			SetTextureEnemy(i, enemy->patternAnim);	
			//// 移動後の座標で頂点を設定
			enemy->pos.x += enemy->direction*enemy->speed * 1.5f;
			if (enemy->pos.x < 0 || enemy->pos.x > SCREEN_WIDTH)
				enemy->use = false;
			SetVertexEnemy(i);
		}
		else
		{
			enemy->pos = D3DXVECTOR3(SCREEN_OUT, SCREEN_OUT, -1.0f);
			SetVertexEnemy(i);
			enemy_count++;
			if (enemy_count>enemy_rate)			
			SetEnemy();
		}
	}
	//PrintDebugProc(1, "Enemy0_XYZ:%f\n", GetEnemy(0)->pos.x, GetEnemy(0)->pos.y, GetEnemy(0)->pos.z);
	//PrintDebugProc(1, "Enemy1_XYZ:%f\n", GetEnemy(1)->pos.x, GetEnemy(1)->pos.y, GetEnemy(1)->pos.z);
	//PrintDebugProc(1, "Enemy2_XYZ:%f\n", GetEnemy(2)->pos.x, GetEnemy(2)->pos.y, GetEnemy(2)->pos.z);
	//PrintDebugProc(1, "Enemy3_XYZ:%f\n", GetEnemy(3)->pos.x, GetEnemy(3)->pos.y, GetEnemy(3)->pos.z);

	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(int pno)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(pno);
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	if (enemy->use)					// 使用している状態なら更新する
	{
		// テクスチャの設定
		pDevice->SetTexture(0, enemyTexture[enemy->type]);
		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_BULLET, enemy->vtx, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEnemy(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(no);
	// 頂点座標の設定
	SetVertexEnemy(no);
	// rhwの設定
	enemy->vtx[0].rhw =
	enemy->vtx[1].rhw =
	enemy->vtx[2].rhw =
	enemy->vtx[3].rhw = 1.0f;
	// 反射光の設定
	enemy->vtx[0].diffuse = 
	enemy->vtx[1].diffuse = 
	enemy->vtx[2].diffuse = 
	enemy->vtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// テクスチャ座標の設定
	enemy->vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	enemy->vtx[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	enemy->vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	enemy->vtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	// テクスチャの設定
	SetTextureEnemy(no, enemy->patternAnim);
	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEnemy(int no)
{
	ENEMY *enemy = GetEnemy(no);
	// 頂点座標の設定
	enemy->vtx[0].vtx = D3DXVECTOR3(enemy->pos.x, enemy->pos.y, 0);
	enemy->vtx[1].vtx = D3DXVECTOR3(enemy->pos.x + TEXTURE_ENEMY_SIZE_X, enemy->pos.y, 0);
	enemy->vtx[2].vtx = D3DXVECTOR3(enemy->pos.x, enemy->pos.y + TEXTURE_ENEMY_SIZE_Y, 0);
	enemy->vtx[3].vtx = D3DXVECTOR3(enemy->pos.x + TEXTURE_ENEMY_SIZE_X, enemy->pos.y + TEXTURE_ENEMY_SIZE_Y, 0);
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEnemy(int no, int cntPattern)
{
	ENEMY *enemy = GetEnemy(no);
	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;
		
	if (enemy->direction == 1)
	{
		enemy->vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		enemy->vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		enemy->vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		enemy->vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
	else
	{
		enemy->vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		enemy->vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		enemy->vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		enemy->vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
}
//=============================================================================
// 敵の発射設定
//=============================================================================
void SetEnemy(void)
{
	ENEMY *enemy = GetEnemy(0);			  
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == false)			// 未使用状態のバレットを見つける
		{	
			enemy->use = true;				// 使用状態へ変更する
			enemy->pos.z = (1+rand() % (ENEMY_MAX-1));
			enemy->pos = D3DXVECTOR3(SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X,
									 SCREEN_HEIGHT * 0.85f - TEXTURE_ENEMY_SIZE_Y + enemy->pos.z*(SCREEN_HEIGHT*0.05f),
									 enemy->pos.z);	// 座標をセット
			enemy->patternAnim = 0;			
			enemy->direction = -1;
			enemy->type = rand() % 2;
			enemy_count = 0;
			enemy_rate = 400;
			if (enemy->type == 1)
				enemy->speed = 1.0f;
			else
				enemy->speed = 1.5f;
			return;							// 1発セットしたので終了する
		}
	}
}

/************************thsths*******************************************************
関数名:	ENEMY *GetMapAdr( int pno )
引数:	int pno : プレイヤー番号
戻り値:	ENEMY *
説明:	プレイヤーのアドレスを取得する
*******************************************************************************/
ENEMY *GetEnemy(int pno)
{
	return &enemy[pno];
}