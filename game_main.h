//=============================================================================
//
// ステージ処理 [GameMain.h]
//
//=============================================================================
#ifndef _GAME_MAIN_H_
#define _GAME_MAIN_H_


//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"


//*****************************************************************************
// マクロ・列挙型定義
//*****************************************************************************

// 操作方法のビルドスイッチ
// OPERATIN_1, OPERTION_2どちらか
#define OPERATION_1

// オブジェクトカテゴリ
enum OBJECT_CATERGORY {
	// 大分類
	STD_GAMEOBJECT = 1,
	STD_UI,

	// 中分類
	BULLET_KIND,					// STD_GAMEOBJECT

	// 小分類	
	ORB_TYPE,						// BULLET_KIND
	JIN_TYPE,						// BULLET_KIND
	ITEM_TYPE,						// BULLET_KIND
	CHARACTER_TYPE,
	BOX_TYPE,
	DIRECTIONCURSOR_TYPE,
	WALL_TYPE,
	RAY_TYPE
};

// ゲーム基本設定
#define GAME_TIME					(1800)					// 制限時間
#define MAP_X						(18)					// マップX座標の数
#define MAP_Y						(10)					// マップY座標の数

// 物理
#define GROUND_POS_Y				(1.0f)					// 地上Y座標
#define GROUND_HITPOS_Y				(0.0f)					// 着地判定Y座標
#define VALUE_GROUND_FRICTION		(0.8f)					// 地上摩擦係数
#define VALUE_LANDING_FRICTION		(0.2f)					// 着地時摩擦係数
#define ORBLANDING_FRICTION			(0.7f)					// 着地時摩擦係数
#define ITEM_LANDING_FRICTION		(0.7f)					// アイテム着地時摩擦係数
#define ICE_FRICTION				(0.995f)				// アイス効果時の摩擦係数
#define MAKE_ICE_ACCELERATOR		(0.05f)					// 掛けてアイス効果時の加速度		

// ダメージ
#define EXPLOSION_DAMAGE			(20)


//*****************************************************************************
// 列挙型定義　※当たり判定関連以外
//*****************************************************************************
/* GAMEDATA構造体のメンバ変数に使用する */
// ステージ番号
enum STAGE_NO
{
	STAGE_1,				// ステージ 1
	STAGE_2,				// ステージ 2
	STAGE_3,				// ステージ 3
	STAGE_MAX				// ステージの数
};


//*****************************************************************************
// 構造体定義
//*****************************************************************************
// GAMEDATA構造体
typedef struct {

	// ゲーム進行フェイズ
	int	phase;

	// 勝敗
	int winner;							// バトルの勝者キャラクター
	int loser;							// バトルの敗者キャラクター
	int winnerUser;						// バトルの勝者ユーザー
	int winnerModel;					// バトルの勝者モデル

	// 時間
	int time;							// 経過時間を格納
	DWORD lastTime;						// 時間計算に使用（1/10秒前の時刻を格納する）
	int remainingTime;					// 残り時間を格納
	BOOL isCountTime;					// TRUE→時間計測する FALSE→時間計測しない
	BOOL isTimeUp;						// 時間切れか
	BOOL isSignal;						// シグナル表示するか
	DWORD startTime;					// 時間計算に使用
	BOOL isPrepareFinish;				// ステージ終了準備に入ったか
	DWORD finishTime;				// 時間計算に使用

	// アイテム
	int	cntItemLoop = 0;				// アイテムが登場するまでの経過フレーム数
	BOOL isIce;							// アイス効果中か
	DWORD iceTime;						// アイス開始時間
	BOOL isWind;						// ウインド効果中か
	DWORD windTime;						// ウインド開始時間
	D3DXVECTOR3 dirWind;				// ウインドベクトル

	// マップ
	int mapX;							// マップX座標の数
	int mapY;							// マップY座標の数
	BOOL isCollisionDraw;				// 当たり判定を描画するか
	BOOL isMapNodeDraw;					// マップノードを描画するか

}GAMEDATA;


//*****************************************************************************
// 描画ソート関連定義
//*****************************************************************************


//*****************************************************************************
// 当たり判定関連定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGame(int type);				// ゲーム部分の初期化処理
void UninitGame(void);					// ゲーム部分の終了処理
void UpdateGame(void);					// ゲーム部分の更新処理
void DrawGame(void);					// ゲーム部分の描画処理

// ゲームデータを取得
GAMEDATA *GetGameData(void);

// ベクトルの長さを設定
D3DXVECTOR3 *SetVecLength(D3DXVECTOR3 *vec, float length);

// 移動処理
void Move(D3DXVECTOR3 *pos, D3DXVECTOR3 *speed, D3DXVECTOR3 *accel, float friction);

// 壁ずりベクトル処理
D3DXVECTOR3 *WallScratchVector(D3DXVECTOR3 *out, D3DXVECTOR3 front, D3DXVECTOR3 normal);

// 反射ベクトル処理
D3DXVECTOR3 *ReflectVector(D3DXVECTOR3 *out, D3DXVECTOR3 front, D3DXVECTOR3 normal);

// ダメージ処理
int Damage(int *hp, int damage);

// 回復処理
int Recovery(int hpMax, int *hp, int recovery);

#endif
