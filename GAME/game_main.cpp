//=============================================================================
// ステージ処理 [GameMain.cpp]
//
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
// 作成日 : 2018/11/1
//=============================================================================


//*****************************************************************************
// インクルード
//*****************************************************************************
#include "input.h"
#include "sound.h"
#include "debugproc.h"
//#include "game_config.h"
//#include "shadow.h"
//#include "collision.h"
//#include "gameobject.h"
//#include "field.h"
//#include "field_effect.h"
//#include "wall.h"
//#include "model.h"
//#include "character.h"
//#include "orb.h"
//#include "direction_cursor.h"
//#include "effect_locus.h"
//#include "effect_explosion.h"
//#include "effect_wind.h"
//#include "effect_fire.h"
//#include "effect_orb_beacon.h"
//#include "gauge.h"
#include <time.h> 
//#include "score.h"
//#include "number_hp.h"
//#include "face_display.h"
//#include "timer.h"
//#include "item.h"
//#include "map.h" 
//#include "game_bg.h"
//#include "letter_signal.h"
//#include "box.h"
//#include "start_manager.h"
//#include "particle.h"
//#include "score.h"
//#include "title.h"
//#include "result.h" 
#include "game_main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ゲーム進行上の各フェイズの名称
enum PHASE
{
	START,						// 開始
	FINISH,						// 終了
	PLAY_TIME					// ゲーム中			
};

// アイテム出現頻度調整の最大秒数
#define ADJUST_ITEM_INTERVAL	(SECOND(10))

// フォグの座標
#define START_FOG_POS			(500.0f)
#define END_FOG_POS				(1200.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void StartWork(void);			// START処理
BOOL IsVictoryOrDefeat(void);	// 終了と勝敗の判定
void PreFinishWork(void);		// FINISH遷移前の準備		
void FinishWork(void);			// FINISH処理


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMEDATA	gameData;			// ゲームデータの作成


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(int type)
{	
	// ゲームデータの初期化
	gameData.phase = START;
	gameData.winner = -1;
	gameData.loser = -1;
	gameData.winnerUser = -1;
	gameData.winnerModel = -1;
	gameData.isCountTime = TRUE;
	gameData.time = 0;
	gameData.lastTime = 0;
	gameData.remainingTime = GAME_TIME;
	gameData.isTimeUp = FALSE;
	gameData.isSignal = FALSE;
	gameData.startTime = 0;
	gameData.isPrepareFinish = FALSE;
	gameData.finishTime = 0;
	gameData.cntItemLoop = 0;
	gameData.isIce = FALSE;
	gameData.iceTime = 0;
	gameData.isWind = FALSE;
	gameData.windTime = 0;
	gameData.dirWind = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gameData.mapX = MAP_X;
	gameData.mapY = MAP_Y;

#ifdef _DEBUG
	BOOL isCollisionDraw = FALSE;
	BOOL isMapNodeDraw = FALSE;
#endif

	// ゲームモジュールの初期化
	//InitCollisionList(type);	// 衝突判定リストの初期化処理
	//InitHitList(type);			// 当たりリストの初期化処理
	//InitGameObject(type);		// ゲームオブジェクト初期化処理
	//InitBullet(type);			// バレットの初期化処理
	//InitShadow(type);			// 影の初期化処理
	//InitGameBg(type);			// ゲーム背景の初期化処理
	//InitField(type);			// フィールドの初期化処理
	//InitFieldEffect(type);		// フィールドエフェクトの初期化処理
	//InitBox(type);				// 箱の初期化処理
	//InitModel(type);			// モデルの初期化処理
	//InitCharacter(type);		// キャラクターの初期化処理
	//InitOrb(type);				// オーブの初期化処理
	//InitItem(type);				// アイテムの初期化処理
	//InitDirectionCursor(type);	// 方角カーソルの初期化処理
	//InitWall(type);				// 壁の初期化処理
	//InitLocus(type);			// 軌跡エフェクトの初期化処理
	//InitExplosion(type);		// 爆発エフェクトの初期化処理
	//InitFire(type);				// 炎エフェクトの初期化処理
	//InitWind(type);				// 風エフェクトの初期化処理
	//InitBeacon(type);			// オーブビーコンの初期化処理
	//InitGauge(type);			// ゲージの初期化処理
	//InitScore(type);			// スコアの初期化
	//InitNumberHp(type);			// HP数字表示の初期化処理
	//InitFaceDisplay(type);		// 顔表示の初期化処理
	//InitTimer(type);			// タイマー表示の初期化処理
	//InitLetterSignal(type);		// シグナルの初期化処理
	//InitMap(type);				// マップの初期化処理

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	//UninitShadow();				// 影の終了処理
	//UninitGameBg();				// ゲーム背景の終了処理
	//UninitField();				// フィールドの終了処理
	//UninitFieldEffect();		// フィールドエフェクトの終了処理
	//UninitWall();				// 壁の終了処理
	//UninitModel();				// モデルの終了処理
	//UninitCharacter();			// キャラクターの終了処理
	//UninitOrb();				// オーブの終了処理
	//UninitLocus();				// 軌跡エフェクトの終了処理
	//UninitExplosion();			// 爆発エフェクトの終了処理
	//UninitFire();				// 炎エフェクトの終了処理
	//UninitWind();				// 風エフェクトの終了処理
	//UninitBeacon();				// オーブビーコンの終了処理
	//UninitDirectionCursor();	// 方角カーソルの終了処理
	//UninitGauge();				// ゲージの終了処理
	//UninitScore();				// スコアの終了処理
	//UninitTimer();				// タイマー表示の終了処理
	//UninitLetterSignal();		// シグナルの終了処理
	//UninitItem();				// アイテムの終了処理
	//UninitMap();				// マップの終了処理
	//UninitBullet();				// バレットの終了処理
	//UninitGameObject();			// ゲームオブジェクト終了処理
	//UninitCollisionList();		// 衝突判定リストの終了処理
	// ●アンイニットが少し足りない

	//UninitScore();
	//UninitNumberHp();			// HP数字表示の終了処理
	//UninitFaceDisplay();		// 顔表示の終了処理
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	DWORD currentTime = GetTime();			// 現在時刻を取得する
	srand((unsigned)time(NULL));			// 乱数シード初期化

	// バトルBGMをループ再生
	PlayGameSound(BGM_BATTLE_1, CONTINUE, LOOP);

	// ステージ時間計測
	if (gameData.isCountTime == TRUE)
	{
		// 1/10秒単位で経過時刻を記録
		if ((currentTime - gameData.lastTime) >= DECI_SECOND)
		{
			gameData.time++;				// 0.「1」秒インクリメント
			gameData.remainingTime--;		// 0.「1」秒デクリメント
			gameData.lastTime = currentTime;
		}
	}

	// スタート＆フィニッシュ
	if (gameData.phase == START)
	{
		StartWork();		// ゲーム開始処理
	}
	if(gameData.phase == FINISH)
	{
		FinishWork();		// ゲーム終了処理
	}

	// 更新処理
	UpdateGameBg();			// ゲーム背景の更新処理
	UpdateField();			// フィールドの更新処理
	UpdateFieldEffect();	// フィールドエフェクトの更新処理
	UpdateShadow();			// 影の更新処理
	UpdateCharacter();		// キャラクターの更新処理
	UpdateItem();			// アイテムの更新処理
	UpdateOrb();			// オーブの更新処理
	UpdateLocus();			// 軌跡エフェクトの更新処理
	UpdateExplosion();		// 爆発エフェクトの更新処理
	UpdateFire();			// 炎エフェクトの更新処理
	UpdateWind();			// 風エフェクトの更新処理
	UpdateBeacon();			// オーブビーコンの更新処理
	UpdateDirectionCursor();// 方角カーソルの更新処理
	UpdateBullet();			// バレットの更新処理
	UpdateGameObject();		// ゲームオブジェクトの更新処理
	UpdateGauge();			// ゲージの更新処理
	UpdateScore();			// スコアの更新処理
	UpdateNumberHp();		// HP数字表示の更新処理
	UpdateFaceDisplay();	// 顔表示の更新処理
	UpdateTimer();			// タイマー表示の更新処理
	UpdateLetterSignal();	// シグナルの更新処理
	CheckCollision();		// 当たり判定

	// アイテム生成
	gameData.cntItemLoop++;
	if (gameData.cntItemLoop >SECOND(10) + rand() % ADJUST_ITEM_INTERVAL)
	{
		MAP map = GetMap();
		int itemMapPosX, itemMapPosY;

		do {// 箱の上じゃない座標が出るまで繰り返す
			itemMapPosX = rand() % (gameData.mapX);
			itemMapPosY = rand() % (gameData.mapY);

		} while (map[itemMapPosY][itemMapPosX].terrain == OBSTACLE);

		CreateItem(// アイテムがマップ座標上空にに発生
			D3DXVECTOR3(map[itemMapPosY][itemMapPosX].worldPos.x, 300.0f, map[itemMapPosY][itemMapPosX].worldPos.z),
			ITEM_WIDTH,
			ITEM_HEIGHT);
		gameData.cntItemLoop = 0;
	}

	// アイテム効果
	if (gameData.isIce)
	{
		if (currentTime > gameData.iceTime
			&&currentTime - gameData.iceTime > ICE_TIME)
		{// 指定時間経過したらフラグをオフ
			gameData.isIce = FALSE;
		}
	}
	if (gameData.isWind)
	{
		if (currentTime > gameData.windTime
			&&currentTime - gameData.windTime > WIND_TIME)
		{// 指定時間経過したらフラグをオフ
			gameData.isWind = FALSE;
			StopSound(SE_ITEM_WIND, 0);
		}
	}

	// 勝敗判定
	if(gameData.isPrepareFinish)
	{
		PreFinishWork();
	}
	else if(gameData.isPrepareFinish == FALSE)
	{
		gameData.isPrepareFinish = IsVictoryOrDefeat();
	}

	// ポーズ画面の呼び出し
	if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_START))
	{
		StopAllSound(CONTINUE);
		PlayGameSound(SE_STAGE_PAUSE, FROM_START, NONE);
		SetStage(PAUSE);
	}

	// マップノードを描画
	if (IsButtonTriggered(0, BUTTON_M) || GetKeyboardTrigger(DIK_M))
	{
		gameData.isMapNodeDraw = gameData.isMapNodeDraw ? FALSE : TRUE;
	}


#ifdef _DEBUG
	// リセット
	if (GetKeyboardTrigger(DIK_DELETE))
	{
		StopAllSound(FROM_START);
		SetStage(TITLE);
	}

	// リザルトに進む
	if (GetKeyboardTrigger(DIK_R))
	{
		SetStage(RESULT);
	}

	// 当たり判定を描画
	if (GetKeyboardTrigger(DIK_T))
	{
		gameData.isCollisionDraw = gameData.isCollisionDraw ? FALSE : TRUE;
	}

	// 壁の描画を消す
	if (GetKeyboardTrigger(DIK_U))
	{
		WALL *wall = GetWall(0);
		for (int i = 0; i < WALL_MAX; i++, wall++)
		{
			if (wall->stdParam->state & DRAW_PROCESS)
			{
				SetState(&wall->stdParam->state, FALSE, DRAW_PROCESS);
			}
			else
			{
				SetState(&wall->stdParam->state, TRUE, DRAW_PROCESS);
			}
		}
	}

#endif
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 device = GetDevice();

	//フォグの設定
	FLOAT StartPos = START_FOG_POS;
	FLOAT EndPos = END_FOG_POS;
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);									// フォグ：ON
	device->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(0xAf, 0xff, 0xff, 0xff));	// 色
	device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);						// 頂点モード
	device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);						// テーブルモード
	device->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos));					// 開始位置
	device->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));						// 終了位置

	DrawGameBg();			// ゲーム背景の描画処理	
	DrawField();			// フィールドの描画処理
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);									//フォグ：OFF
	DrawFieldEffect();		// フィールドエフェクトの描画処理
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);									//フォグ：ON
	DrawMap();				// マップタイルの描画

	DrawWall();				// 壁の描画処理
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);									//フォグ：OFF
	DrawShadow();			// 影の描画処理
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);									//フォグ：ON

	DrawBox();				// 箱の描画処理
	DrawOrb();				// オーブの描画処理
	DrawItem();				// アイテムの描画処理
	DrawDirectionCursor();	// 方角カーソルの描画処理
	DrawCharacter();		// キャラクターの描画処理

	// パーティクル
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);									//フォグ：OFF
	DrawLocus();			// 軌跡エフェクトの描画処理
	DrawBeacon();			// オーブビーコンの描画処理
	DrawExplosion();		// 爆発エフェクトの描画処理
	DrawFire();				// 炎エフェクトの描画処理
	DrawWind();				// 風エフェクトの描画処理
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);									//フォグ：ON

	// UI
	DrawGauge();			// ゲージの描画処理
	DrawScore();			// スコアの描画処理
	DrawNumberHp();			// HP数字表示の描画処理
	DrawFaceDisplay();		// 顔表示の描画処理
	DrawTimer();			// タイマー表示の描画処理
	DrawLetterSignal();		// シグナルの描画処理
}


//=============================================================================
// ゲームデータ取得
//=============================================================================
GAMEDATA *GetGameData(void)
{
	return	&gameData;
}


//=============================================================================
// START処理
//=============================================================================
void StartWork(void)
{
	// スタート処理時間をカウント
	gameData.startTime++;

	PlayGameSound(SE_BATTLE_START, CONTINUE, NONE);

	if (SECOND(0) < gameData.startTime && gameData.startTime <= SECOND(1))
	{
		if (gameData.isSignal == FALSE)
		{
			SetSignal(SIGNAL_START, TRUE);
			gameData.isSignal = TRUE;
		}
	}
	else if (gameData.startTime > SECOND(3))
	{
		if (gameData.isSignal)
		{
			SetSignal(SIGNAL_START, FALSE);
			gameData.isSignal = FALSE;
		}
		StopSound(SE_BATTLE_START, FROM_START);
		gameData.phase = PLAY_TIME;
	}
}


//=============================================================================
// 勝敗の判定
//=============================================================================
BOOL IsVictoryOrDefeat(void)
{
	CHARACTER *character0 = GetCharacter(0);
	CHARACTER *character1 = GetCharacter(1);
	BOOL flag = -1;

	// HP切れによる勝敗
	if (character0->hp == 0 && character1->hp == 0)
	{// 同時にHP0の場合はランダムで勝敗決定
		flag = rand() % 2;
	}
	else if (character1->hp == 0)
	{
		flag = 0;
	}
	else if (character0->hp == 0)
	{
		flag = 1;
	}

	// タイムアップによる勝敗
	if (gameData.remainingTime <= 0)
	{// 同じHPでタイプアップの場合はランダムで勝敗決定
		gameData.isTimeUp = TRUE;	// 時間切れフラグをオン

		if (character0->hp == character1->hp)
		{// 
			flag = rand() % 2;
		}
		else if (character0->hp > character1->hp)
		{
			flag = 0;
		}
		else if (character0->hp < character1->hp)
		{
			flag = 1;
		}
	}

	// 勝敗の設定
	if (flag == 0)
	{
		gameData.winner = character0->typeNo;
		gameData.loser = character1->typeNo;
		return TRUE;
	}
	else if (flag == 1)
	{
		gameData.winner = character1->typeNo;
		gameData.loser = character0->typeNo;
		return TRUE;
	}

	return FALSE;
}


//=============================================================================
// FINISH遷移前の準備
//=============================================================================
void PreFinishWork(void)
{
	if (GetGauge(gameData.loser)->displayLength != 0.0f && gameData.remainingTime > 0) return;

	// データ上だけでなくゲージの見た目としても完全にHPが切れたら
	// もしくはタイムアップしていたら

	// 勝利ユーザー勝利モデルを特定
	CONFIGTABLE config = GetConfigTable();
	for (int i = 0; i < USER_MAX; i++)
	{
		if (config.table[i][CHARACTER_SELECT] == gameData.winner)
		{
			gameData.winnerModel = config.table[i][MODEL_SELECT];
			gameData.winnerUser = i;
		}
	}

	// 敗者の操作切断
	CHARACTER *character = GetCharacter(gameData.loser);
	if ((character->stdParam->state & AIR) == FALSE)
	{
		SetState(&character->stdParam->state, FALSE, UPDATE_PROCESS);
	}
	GetDirectionCursor(gameData.loser)->use = FALSE;

	gameData.isCountTime = FALSE;	// 時間計測終了
	gameData.phase = FINISH;		// 遷移
}


//=============================================================================
// FINISH処理
//=============================================================================
void FinishWork(void)
{

	// シグナルの設定
	if (gameData.isTimeUp)
	{
		SetSignal(SIGNAL_TIMEUP, TRUE);
	}
	else
	{
		SetSignal(SIGNAL_FINISH, TRUE);
	}
	gameData.isSignal = TRUE;

	PlayGameSound(SE_BATTLE_FINISH, CONTINUE, NONE);

	// フィニッシュ処理時間をカウント
	gameData.finishTime++;

	if (SECOND(3) < gameData.finishTime && gameData.finishTime <= SECOND(5))
	{
		StopSound(SE_BATTLE_FINISH, FROM_START);
	}
	if (gameData.finishTime > SECOND(5))
	{
		StopAllSound(FROM_START);
		SetStage(RESULT);
	}
}


//=============================================================================
// 移動処理
//=============================================================================
void Move(D3DXVECTOR3 *pos, D3DXVECTOR3 *speed, D3DXVECTOR3 *accelerator, float friction)
{
	// 加速度の影響を速度に与える
	D3DXVec3Add(speed, speed, accelerator);

	// 速度に摩擦の影響を与える
	D3DXVec3Scale(speed, speed, friction);

	// 速度の影響を座標に与える
	D3DXVec3Add(pos, pos, speed);

	return;
}


//=============================================================================
// ベクトルの長さを設定
//=============================================================================
D3DXVECTOR3 *SetVecLength(D3DXVECTOR3 *vec, float length)
{
	// ベクトルを正規化
	D3DXVec3Normalize(vec, vec);

	// 単位ベクトルに長さを掛けてスケール
	D3DXVec3Scale(vec, vec, length);

	return vec;
}


//=============================================================================
// 壁ずりベクトル処理
// out : 壁ずりベクトル（戻り値）
// front : 進行ベクトル
// normal: 衝突点での法線ベクトル
//=============================================================================
D3DXVECTOR3 *WallScratchVector(D3DXVECTOR3 *out, D3DXVECTOR3 front, D3DXVECTOR3 nor)
{
	float len = D3DXVec3Length(&front);		// ベクトルの長さを保管 
	D3DXVECTOR3 normal;
	D3DXVec3Normalize(&normal, &nor);
	D3DXVec3Normalize(out, &(front - D3DXVec3Dot(&front, &normal) * normal));
	*out *= len;							// 求めた反射単位ベクトルに長さを掛ける
	return out;
}


//=============================================================================
// 反射ベクトル処理
// out : 反射ベクトル（戻り値）
// front : 進行ベクトル
// normal: 衝突点での法線ベクトル
//=============================================================================
D3DXVECTOR3 *ReflectVector(D3DXVECTOR3 *out, D3DXVECTOR3 front, D3DXVECTOR3 nor)
{
	float len = D3DXVec3Length(&front);		// ベクトルの長さを保管 
	D3DXVECTOR3 normal;
	D3DXVec3Normalize(&normal, &nor);
	D3DXVec3Normalize(out, &(front - 2.0f * D3DXVec3Dot(&front, &normal) * normal));
	*out *= len;							// 求めた反射単位ベクトルに長さを掛ける
	return out;
}


//=============================================================================
// ダメージ処理
//=============================================================================
int Damage(int *hp, int damage)
{
	// HPからダメージを引く
	*hp -= damage;

	// 0以下のHPは0に戻す
	if (*hp < 0)
	{
		*hp = 0;
	}

	return *hp;
}


//=============================================================================
// リカバリ処理
//=============================================================================
int Recovery(int hpMax, int *hp, int recovery)
{
	// HPを足す
	*hp += recovery;

	// 最大値を越えたHPは最大値に
	if (*hp > hpMax)
	{
		*hp = hpMax;
	}

	return *hp;
}
