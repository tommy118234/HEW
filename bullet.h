/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	敵処理 [bullet.h]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#ifndef _BULLET_H_
#define _BULLET_H_
// マクロ定義
#define	NUM_BULLET						(2)			// ポリゴン数
#define TEXTURE_GAME_BULLET				_T("data/TEXTURE/bullet2.png")	// サンプル用画像
#define TEXTURE_BULLET_SIZE_X			(50)		// テクスチャサイズ
#define TEXTURE_BULLET_SIZE_Y			(10)		// 同上
#define TEXTURE_PATTERN_DIVIDE_X_BULLET	(1)			// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_BULLET	(1)			// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_BULLET			(TEXTURE_PATTERN_DIVIDE_X_BULLET*TEXTURE_PATTERN_DIVIDE_Y_BULLET)	// アニメーションパターン数
#define TIME_ANIMATION_BULLET			(50)		// アニメーションの切り替わるカウント
#define BULLET_MAX						(1)		// バレットの最大数
#define BULLET_SPEED					(30.0f)		// バレットの移動スピード
/*******************************************************************************
* 構造体定義
********************************************************************************/
struct  BULLET											// バレットの構造体
{	
	bool					use;						// true:使用  false:未使用
	D3DXVECTOR3				pos;						// ポリゴンの移動量
	LPDIRECT3DTEXTURE9		texture;					// テクスチャへのポリゴン
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク	
	float					radius;						// ポリゴンの半径
	float					baseAngle = 0.0;			// ポリゴンの角度	
	int						atk;
	int						direction;
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(int type);
void	UninitBullet(void);
void	UpdateBullet(void);
void	DrawBullet(void);
void	SetBullet(D3DXVECTOR3 pos,int atk, int dir);
BULLET  *GetBullet(int pno);							//バレットの(アドレス)を取得
#endif