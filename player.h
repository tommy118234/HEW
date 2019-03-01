//=============================================================================
//
// ポリゴン処理 [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_




#include "bullet.h"
// マクロ定義
//#define TEXTURE_GAME_SAMPLE00	_T("data/TEXTURE/runningman000.png")	// サンプル用画像
//#define TEXTURE_SAMPLE00_SIZE_X	(200) // テクスチャサイズ
//#define TEXTURE_SAMPLE00_SIZE_Y	(200) // 同上
//
//
//#define TEXTURE_PATTERN_DIVIDE_X	(8)	// アニメパターンのテクスチャ内分割数（X)
//#define TEXTURE_PATTERN_DIVIDE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)
//#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
//#define TIME_ANIMATION				(100)	// アニメーションの切り替わるカウント


#define TEXTURE_GAME_SAMPLE00	_T("data/TEXTURE/runningman001.png")	// サンプル用画像
#define TEXTURE_SAMPLE00_SIZE_X	(140) // テクスチャサイズ
#define TEXTURE_SAMPLE00_SIZE_Y	(200) // 同上

#define TEXTURE_PATTERN_DIVIDE_X	(5)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

#define PLAYER_MAX	(1)			// プレイヤーの数
/*******************************************************************************
* 構造体定義
********************************************************************************/
struct	 PLAYER				// キャラクターの基本構造体
{
	CHARACTER	status;			// 共通パラメーター
	
	//LPDIRECT3DTEXTURE9		g_pD3DTexturePlayer = NULL;		// テクスチャへのポリゴン
	//
	//VERTEX_2D				g_vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク
	D3DXVECTOR3				g_posPlayer;						// ポリゴンの移動量
	D3DXVECTOR3				g_rotPlayer;						// ポリゴンの回転量
	int						g_nCountAnim;						// アニメーションカウント
	int						g_nPatternAnim;						// アニメーションパターンナンバー

	float					g_fRadiusPlayer;					// ポリゴンの半径
	float					g_fBaseAnglePlayer;					// ポリゴンの角度

	/*
	g_fBaseAnglePolygon = Atan2f(120,80)
	D3DXVECTOR2 temp= D3DXVECTOR2()
	*/

	int						direction = -1;
	int						bullet_num = 0;
	int						score = 0;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(int pno);	// プレイヤーの(アドレス)を取得


#endif
