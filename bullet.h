//=============================================================================
//
// ポリゴン処理 [bullet.h]
// Author : 
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_


// マクロ定義
#define TEXTURE_GAME_BULLET _T("data/TEXTURE/bullet.png")	// サンプル用画像
#define TEXTURE_SAMPLE00_SIZE_X3	(22) // テクスチャサイズ
#define TEXTURE_SAMPLE00_SIZE_Y3	(60) // 同上

#define TEXTURE_PATTERN_DIVIDE_X3	(1)														// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y3	(1)														// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM3			(TEXTURE_PATTERN_DIVIDE_X3*TEXTURE_PATTERN_DIVIDE_Y3)	// アニメーションパターン数

#define BULLET_MAX	(1)			// プレイヤーの数
/*******************************************************************************
* 構造体定義
********************************************************************************/
typedef struct					// キャラクターの基本構造体
{
	//CHARACTER	status;			// 共通パラメーター

	//LPDIRECT3DTEXTURE9	g_pD3DTexturePlayer = NULL;			// テクスチャへのポリゴン
	//
	//VERTEX_2D				g_vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク
	D3DXVECTOR3				g_posBullet;						// ポリゴンの移動量
	bool					use = FALSE;
	int						g_nCountAnim;						// アニメーションカウント
	int						g_nPatternAnim;						// アニメーションパターンナンバー



	D3DXVECTOR3				g_rotBullet;						// ポリゴンの回転量
	float					g_fRadiusBullet;					// ポリゴンの半径
	float					g_fBaseAngleBullet;					// ポリゴンの角度

	LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;			// テクスチャへのポリゴン
	VERTEX_2D				g_vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク
	
	//int						direction = -1;
} BULLET;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
BULLET *GetBullet(int pno);	// プレイヤーの(アドレス)を取得


#endif
