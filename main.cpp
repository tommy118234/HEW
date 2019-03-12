//=============================================================================
// メイン処理 [main.cpp]
//
// Author : GP11B341 24 中込和輝
// 作成日 : 2019/2/25
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "input.h"
#include "sound.h"
#include <stdio.h> 
#include "font.h"
#include "debugproc.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "bg.h"
#include "road.h"
#include "timer.h"
#include "splash.h"
#include "title.h"
#include "result.h"
#include "score.h"
#include "life.h"
#include "item.h"
#include "panty.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"Team Adult Game"	// ウインドウのクラス名
#define WINDOW_NAME		"Team Adult Game"	// ウインドウのキャプション名
#define OBJECT_MAX		(PLAYER_MAX + ENEMY_MAX + ITEM_MAX)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void CheckHit(void);
void SortDraw(int num);



//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			direct3D  = NULL;		// direct3D  オブジェクト
LPDIRECT3DDEVICE9	pD3DDevice = NULL;		// Deviceオブジェクト(描画に必要)
STAGE				stage;					// 現在のステージ
DWORD				currentTime;			// 現在のシステム時刻
GAMEDATA			gameData;				// ゲーム進行データセット
#ifdef _DEBUG
int					cntFPS;					// FPSカウンタ
#endif
int					player_cnt;
SORTOBJECT			sortArray[OBJECT_MAX];	// 描画ソート用構造体配列


//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測
	DWORD execLastTime;
	DWORD fpsLastTime;
	DWORD frameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		LoadIcon(hInstance, MAKEINTRESOURCE(NULL)),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	execLastTime = 
	fpsLastTime = timeGetTime();
	currentTime = 0;
	frameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);

				DispatchMessage(&msg);
			}
        }
		else
		{
			currentTime = timeGetTime();
			if((currentTime - fpsLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				cntFPS =frameCount * 1000 / (currentTime - fpsLastTime);
#endif
				fpsLastTime = currentTime;
				frameCount = 0;
			}

			if((currentTime - execLastTime) >= (1000 / FPS))
			{
				execLastTime = currentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				frameCount++;
			}
		}
	}
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}


//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	//  direct3D オブジェクトの作成
	 direct3D  =  Direct3DCreate9(D3D_SDK_VERSION);
	if(direct3D  == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED( direct3D ->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う

	if(bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if(FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if(FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if(FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}


	// レンダーステートパラメータの設定
    pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// テクスチャアドレッシング方法(U値)を設定
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// テクスチャアドレッシング方法(V値)を設定
	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// テクスチャ縮小フィルタモードを設定
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	// ゲームシステムの初期化
	InitInput(hInstance, hWnd);	// 入力処理の初期化
	InitSound(hWnd);			// サウンドの初期化
	InitFont();					// フォントの初期化
	InitPlayer(0);				// プレイヤーの初期化
	InitBullet(0);				// バレットの初期化
	InitEnemy(0);				// ENEMYの初期化
	InitBg(0);					// BGの初期化
	InitRoad(0);				// 道の初期化
	InitItem(0);				// アイテムの初期化
	InitPanty(0);				// パンティの初期化
	InitTimer(0);				// タイマーの初期化
	InitScore(0);				// スコア初期化
	InitLife(0);				// ライフの初期化
	InitSplash(0);				// スプラッシュの初期化
	InitTitle(0);				// タイトルの初期化
	InitResult(0);				// リザルトの初期化
#ifdef _DEBUG
	InitDebugProc();			// デバッグ表示の初期化
#endif

	// gameData初期化
	gameData.isGameClear = FALSE;
	gameData.isCombo = FALSE;
	gameData.numCombo = 0;
	
	// 描画ソート用構造体配列の初期化
	sortArray[0].drawFunc = DrawPlayer;
	sortArray[0].objectNo = 0;
	sortArray[0].pos = &GetPlayer(0)->pos;
	sortArray[0].adjustCenterY = TEXTURE_PLAYER_SIZE_Y / 2;
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		sortArray[1 + i].drawFunc = DrawEnemy;
		sortArray[1 + i].objectNo = i;
		sortArray[1 + i].pos = &GetEnemy(0)->pos;
		sortArray[1 + i].adjustCenterY = TEXTURE_ENEMY_SIZE_Y / 2;
	}
	sortArray[5].drawFunc = DrawItem;
	sortArray[5].objectNo = 0;
	sortArray[5].pos = &GetItem()->pos;
	sortArray[5].adjustCenterY = ITEM_SIZE_Y / 2;


	// 音量調節
	//GetSound(BGM_BATTLE_1)->SetVolume(-200);

	// 最初のステージを設定
	stage = SPLASH;

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	UninitDebugProc();			// デバッグ表示の終了
#endif

	//UninitTitle();			// タイトルの終了処理
	UninitInput();				// 入力処理の終了処理
	UninitSound();				// サウンドの終了処理
	UninitFont();				// フォントの終了処理
	UninitPlayer();				// プレイヤーの終了処理
	UninitBullet();				// バレットの終了処理
	UninitEnemy();				// ENEMYの終了処理
	UninitBg();					// BGの終了処理
	UninitRoad();				// 道の終了処理
	UninitItem();				// アイテムの終了処理
	UninitPanty();				// パンティの終了処理
	UninitTimer();				// タイマーの終了処理
	UninitScore();				// スコアの終了処理
	UninitLife();				// ライフの終了処理
	UninitSplash();				// スプラッシュの終了処理
	UninitTitle();				// タイトルの終了処理
	UninitResult();				// リザルトの終了処理

	// デバイスの開放
	SAFE_RELEASE(pD3DDevice);

	//  direct3D オブジェクトの開放
	SAFE_RELEASE(direct3D);

}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	UpdateInput();					// 入力更新		
	switch (stage)
	{
	case SPLASH:
		UpdateSplash();				// スプラッシュの更新処理
		break;

	case TITLE:
		UpdateTitle();				// タイトルの更新処理
		break;

	case TUTORIAL:
		break;

	case GAME:

		PlayGameSound(BGM_GAME, CONTINUE_SOUND, LOOP);
		//PlayGameSound(BGM_MUTEKI, CONTINUE_SOUND, LOOP);
		UpdatePlayer();				// プレイヤーの更新
		UpdateBullet();				// バレットの更新
		UpdateEnemy();				// ENEMYの更新
		UpdateBg();					// BGの更新
		UpdateRoad();				// 道の更新
		UpdateItem();				// アイテムの更新
		UpdatePanty();				// パンティの更新
		UpdateTimer();				// タイマーの更新
		UpdateScore();				// スコアの更新
		UpdateLife();				// ライフの更新
		CheckHit();					// 当たり判定
		break;

	case PAUSE:
		break;

	case RESULT:
		UpdateResult();				// リザルトの更新
		break;

	case EXIT:
		PostQuitMessage(0);
		break;
	}

#ifdef _DEBUG
	UpdateDebugProc();				// デバッグ表示の更新


#endif
}


//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファのクリア
	pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	PLAYER *player = GetPlayer(0);
	ENEMY  *enemy = GetEnemy(0);
	BULLET *bullet = GetBullet(0);
	ITEM   *item = GetItem();

	int i;
	D3DXVECTOR3 player_center, enemy_center;
	//  direct3D による描画の開始
	if(SUCCEEDED(pD3DDevice->BeginScene()))
	{
		switch (stage)
		{
		case SPLASH:
			DrawSplash();				// スプラッシュの描画
			break;

		case TITLE:
			DrawTitle();				// タイトルの描画
			break;

		case TUTORIAL:
			break;

		case GAME:
			DrawBg();					// BGの描画
			//DrawRoad();					// 道の描画

			// プレイヤー, エネミー, アイテムをY座標でソートして描画
			SortDraw(OBJECT_MAX);
			for (int i = 0; i < OBJECT_MAX; i++)
			{
				(*sortArray[i].drawFunc)(sortArray[i].objectNo);
			}

			PrintDebugProc(2, "num : %d\n", gameData.numCombo);
			PrintDebugProc(2, "score : %d\n", GetScore()->value);


			//player_center = player->pos + D3DXVECTOR3(TEXTURE_PLAYER_SIZE_X / 2, TEXTURE_PLAYER_SIZE_Y / 2, 0);
			//PrintDebugProc(1, "P : %f\n", player_center.y);
			//for (i = 0; i < ENEMY_MAX; i++ ,enemy++)
			//{
			//	if (enemy->use == true) 
			//	{
			//		enemy_center = enemy->pos + D3DXVECTOR3(TEXTURE_ENEMY_SIZE_X / 2, TEXTURE_ENEMY_SIZE_Y / 2, 0);
			//		PrintDebugProc(1, "%d  %f\n",i+1, enemy_center.y);
			//		if (player->pos.y > enemy->pos.y)
			//		{
			//			DrawEnemy(i);	 // ENEMYの描画	
			//		}
			//		else
			//		{
			//			DrawPlayer();
			//			break;
			//		}
			//	}
			//}
			//if (i == ENEMY_MAX)
			//	DrawPlayer();
			//while (i < ENEMY_MAX)
			//{
			//	DrawEnemy(i);
			//	i++;
			//	enemy++;
			//}
			//DrawBullet();


			// UI
			DrawPanty();				// パンティの描画
			DrawTimer();				// タイマーの描画
			DrawScore();				// スコアの描画
			DrawLife();					// ライフの描画
			break;

		case PAUSE:
			break;

		case RESULT:
			DrawBg();					// BGの描画
			DrawPlayer(0);				// プレイヤーの描画
			DrawResult();				// リザルトの描画
			DrawScore();				// スコアの描画
			break;

		case EXIT:
			break;
		}

#ifdef _DEBUG
		DrawDebugProc();				// デバッグ表示の描画
#endif

		//  direct3D による描画の終了
		pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


//=============================================================================
// デバイス取得関数
//-----------------------------------------------------------------------------
// 戻り値：LPDIRECT3DDEVICE9 デバイスへのポインタ
// 引数  ：void
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return(pD3DDevice);
}


//=============================================================================
// ステージを設定
//-----------------------------------------------------------------------------
// 戻り値：void
// 引数  ：STAGE　設定するステージ
//=============================================================================
void SetStage(STAGE set)
{
	stage = set;
}


//=============================================================================
// 現在時刻取得取得
//-----------------------------------------------------------------------------
// 戻り値：DWORD　現在のシステム時刻
// 引数  ：void
//=============================================================================
DWORD GetTime(void)
{
	return currentTime;
}


//=============================================================================
// ゲーム進行データセットを取得
//-----------------------------------------------------------------------------
// 戻り値：GAMEDATA*　ゲーム進行データセットのアドレス
// 引数  ：void
//=============================================================================
GAMEDATA *GetGameData(void)
{
	return &gameData;
}
//=============================================================================
// BBによる当たり判定処理
// 回転は考慮しない 
// 戻り値：当たってたらtrue
//=============================================================================
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2)
{	
	size1 /= 2.0f;	// 半サイズにする
	size2 /= 2.0f;	// 同上
	if (pos1.x + size1.x > pos2.x - size2.x && pos2.x + size2.x > pos1.x - size1.x &&
		pos1.y + size1.y > pos2.y - size2.y && pos2.y + size2.y > pos1.y - size1.y)
	{
		return true;
	}
	return false;
}
//=============================================================================
// BCによる当たり判定処理
// posは円の中心、radiusは半径
// 戻り値：当たってたらtrue
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2)
{
	if ((radius1 + radius2) * (radius1 + radius2) >=
		((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y)) + (pos2.z - pos1.z)*(pos2.z - pos1.z))
	{
		return true;
	}
	return false;
}

//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	PLAYER *player = GetPlayer(0);			// エネミーのポインターを初期化
	ENEMY  *enemy  = GetEnemy(0);			// エネミーのポインターを初期化
	BULLET *bullet = GetBullet(0);			// バレットのポインターを初期化
	ITEM	*item = GetItem();				// アイテムのポインターを初期化

	D3DXVECTOR3 player_center, enemy_center, bullet_center, item_center;
	D3DXVECTOR2 player_size = D3DXVECTOR2(TEXTURE_PLAYER_SIZE_X/2, TEXTURE_PLAYER_SIZE_Y/2);
	D3DXVECTOR2 enemy_size =  D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y);
	D3DXVECTOR2 bullet_size = D3DXVECTOR2(TEXTURE_BULLET_SIZE_X, TEXTURE_BULLET_SIZE_Y);
	D3DXVECTOR2 item_size = D3DXVECTOR2(item->sizeX, item->sizeY);

	player_center = player->pos + D3DXVECTOR3(TEXTURE_PLAYER_SIZE_X / 2, TEXTURE_PLAYER_SIZE_Y / 2, 0);

	// アイテムと操作キャラ(BB)
	item_center = item->pos + D3DXVECTOR3(item->sizeX / 2, item->sizeY / 2, 0.0f);
	if (CheckHitBB(player_center, item_center, player_size, item_size) &&
		player->pos.z == item->pos.z)
	{
		item->use = FALSE;
		/* 無敵状態になる処理をかく */
		player->status.LUCK += 5 * 60;
	}


	// 敵と操作キャラ(BB)
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == false)	continue;
		enemy_center = enemy->pos + D3DXVECTOR3(TEXTURE_ENEMY_SIZE_X / 2, TEXTURE_ENEMY_SIZE_Y / 2, 0);
		if (CheckHitBB(player_center, enemy_center, player_size/2 ,enemy_size) &&
			player->pos.z == enemy->pos.z &&
			enemy->direction != player->direction)
		{
			enemy->use = false;

			/* 無敵だったらHP減らずにコンボも継続 */
			if (player->status.LUCK == 0)
			{
				player->status.HP--;
				gameData.isCombo = FALSE;		// コンボ終了
				gameData.numCombo = 0;			// コンボリセット
				if (player->status.HP == 0)
				{
					StopAllSound(INIT_SOUND);	// 音を全て止める
					SetStage(RESULT);			// ステージ遷移
				}
			}
			else
			{
				AddScore(50);
			}
		}
	}

	// ボスと弾(BC) // bullet(heavy) inner loop, enemy(light) outer loop
	enemy = GetEnemy(0);					// エネミーのポインターを初期化
	for (int j = 0; j < ENEMY_MAX; j++, enemy++)
	{
		if (enemy->use == false) continue;
		enemy_center = enemy->pos + D3DXVECTOR3(TEXTURE_ENEMY_SIZE_X / 2, TEXTURE_ENEMY_SIZE_Y / 2, 0);
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{
			if (bullet->use == false) continue;
			bullet_center = bullet->pos + D3DXVECTOR3(TEXTURE_BULLET_SIZE_X / 2, TEXTURE_BULLET_SIZE_Y / 2, 0);
			if (CheckHitBB(bullet_center, enemy_center, bullet_size, enemy_size) &&
				player->pos.z == enemy->pos.z
				)
			{
				if (enemy->type == 1)
				{// エネミー
					enemy->use = false;
					gameData.isCombo = FALSE;		// コンボ終了
					gameData.numCombo = 0;			// コンボリセット

					/* 無敵だったらここでもスコア加算 */
					if (player->status.LUCK > 0)
					{
						gameData.isCombo = TRUE;		// コンボ開始
						gameData.numCombo++;
						AddScore(100);					// ●あたったフレーム数分スコアが入ってしまっている？
						//enemy->direction = 1;
						enemy->use = false;
						AddNumPanty();
					}
				}
				else if(enemy->direction == player->direction)
				{// ターゲット
					gameData.isCombo = TRUE;		// コンボ開始
					gameData.numCombo++;
					AddScore(100);					// ●あたったフレーム数分スコアが入ってしまっている？
					//enemy->direction = 1;
					enemy->use = false;
					AddNumPanty();
				}
			}
		}
	}
}


//=============================================================================
// 描画ソート関数
// [説明]	: 描画順を入れ替える
// [戻り値]	: void
// [引数]	: int型 入れ替えるオブジェクトの数
//-----------------------------------------------------------------------------
// [補足]	: オブジェクトのY座標でバブルソートを行う
//=============================================================================
void SortDraw(int num)
{
	int i, j;
	SORTOBJECT temp;

	for (i = 0; i < (num - 1); i++)
	{
		for (j = (num - 1); j > i; j--)
		{
			if (sortArray[j - 1].pos->y + sortArray[j - 1].adjustCenterY > sortArray[j].pos->y + sortArray[j].adjustCenterY)
			{
				temp = sortArray[j - 1];
				sortArray[j - 1] = sortArray[j];
				sortArray[j] = temp;
			}
		}
	}
}


#ifdef _DEBUG
//=============================================================================
// FPS取得
//-----------------------------------------------------------------------------
// 戻り値：int　現在のフレームレート
// 引数  ：void
//=============================================================================
int GetFPS(void)
{
	return cntFPS;
}
#endif
