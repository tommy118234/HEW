//=============================================================================
//
// ポリゴン処理 [enemy.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

#define TEXTURE_GAME_ENEMY1		_T("data/TEXTURE/enemy1.png")
#define TEXTURE_GAME_ENEMY2		_T("data/TEXTURE/enemy2.png")
#define TEXTURE_ENEMY_SIZE_X	(140) // テクスチャサイズ
#define TEXTURE_ENEMY_SIZE_Y	(200) // 同上

#define TEXTURE_PATTERN_DIVIDE_X	(5)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

#define ENEMY_MAX	(10)			// プレイヤーの数
/*******************************************************************************
* 構造体定義
********************************************************************************/
struct	ENEMY				// キャラクターの基本構造体
{
	CHARACTER				status;			// 共通パラメーター
	bool					use;			// true:使用  false:未使用
	D3DXVECTOR3				pos;			// ポリゴンの移動量
	int						patternAnim;	// アニメーションパターンナンバー
	float					countAnim;		// アニメーションカウント	
	int						type;
	VERTEX_2D				vtx[NUM_VERTEX];// 頂点情報格納ワーク
	float					radius;			// ポリゴンの角度	
	float					baseAngle;
	int						direction = -1;
	int						moving_cooldown = -1;
	int						speed;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(int type);
void	UninitEnemy(void);
void	UpdateEnemy(void);
void	DrawEnemy(void);
ENEMY   *GetEnemy(int pno);	// プレイヤーの(アドレス)を取得
#endif