//=============================================================================
//
// ポリゴン処理 [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

#define TEXTURE_GAME_PLAYER		_T("data/TEXTURE/player.png")	// サンプル用画像
#define TEXTURE_GAME_PLAYER2	_T("data/TEXTURE/player_attack.png")	// サンプル用画像
#define TEXTURE_PLAYER_SIZE_X	(140)							// テクスチャサイズ
#define TEXTURE_PLAYER_SIZE_Y	(200)							// 同上

#define TEXTURE_PATTERN_DIVIDE_X	(5)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

#define PLAYER_MAX	(1)			// プレイヤーの数

// スクロール開始座標
#define LEFT_SCROLL_LINE_X	(SCREEN_WIDTH * 0.05f)
#define RIGHT_SCROLL_LINE_X	(SCREEN_WIDTH * 0.5f)

/*******************************************************************************
* 構造体定義
********************************************************************************/
struct	PLAYER				// キャラクターの基本構造体
{
	CHARACTER	status;		// 共通パラメーター

	D3DXVECTOR3				pos;					// ポリゴンの移動量
	int						patternAnim;			// アニメーションパターンナンバー
	float					countAnim;				// アニメーションカウント	
	LPDIRECT3DTEXTURE9		texture[MOVE_MAX];			// テクスチャへのポリゴン
	VERTEX_2D				vtx[NUM_VERTEX];		// 頂点情報格納ワーク
	float					radius;					// ポリゴンの角度	
	float					baseAngle;
	int						direction = -1;
	int						moving_cooldown = -1;	
	int						speed;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int type);
void	UninitPlayer(void);
void	UpdatePlayer(void);
void	DrawPlayer(void);
PLAYER  *GetPlayer(int pno);	// プレイヤーの(アドレス)を取得
#endif