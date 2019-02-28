//=============================================================================
//
// 入力処理 [input.cpp]
// Author : GP11B341 24 中込和輝
//
//=============================================================================


//*****************************************************************************
// インクルード
//*****************************************************************************
#include "input.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)			// キー最大数

// リピート用
#define FIRST_REPEAT_COUNT	(25)			// カウンタ1
#define SECOND_REPEAT_COUNT	(7)				// カウンタ2

// game pad用設定値
#define DEADZONE			(2500)			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX			(1000)			// 有効範囲の最大値
#define RANGE_MIN			(-1000)			// 有効範囲の最小値


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);			// キーボードの初期化
void UninitKeyboard(void);									// キーボード終了処理
HRESULT UpdateKeyboard(void);								// キーボードの更新処理

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow);		// マウスの初期化
void UninitMouse();											// マウスの終了処理
HRESULT UpdateMouse();										// マウスの更新処理

HRESULT InitializePad(void);								// パッド初期化
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);// パッド検査コールバック
void UninitPad(void);										// パッドの終了処理
void UpdatePad(void);										// パッドの更新処理


//*****************************************************************************
// グローバル変数
//*****************************************************************************
//------------------------------- keyboard
LPDIRECTINPUT8			DInput = NULL;						// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	DIDevKeyboard = NULL;				// IDirectInputDevice8インターフェースへのポインタ(キーボード)
BYTE					keyState[NUM_KEY_MAX];				// キーボードの状態を受け取るワーク
BYTE					keyStateTrigger[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					keyStateRepeat[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					keyStateRelease[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
int						keyStateRepeatCnt[NUM_KEY_MAX];		// キーボードのリピートカウンタ

//--------------------------------- mouse
static LPDIRECTINPUTDEVICE8 mouse = NULL;					// mouse
static DIMOUSESTATE2	mouseState;							// マウスのダイレクトな状態
static DIMOUSESTATE2	mouseTrigger;						// 押された瞬間だけON

//--------------------------------- game pad
static LPDIRECTINPUTDEVICE8	gamePad[GAMEPADMAX] = {NULL,NULL,NULL,NULL};// パッドデバイス
static DWORD			padState[GAMEPADMAX];				// パッド情報（複数対応）
DWORD					lastPadState[GAMEPADMAX];			// 直前のパッド情報（複数対応）
static DWORD			padTrigger[GAMEPADMAX];				// トリガー情報
static DWORD			padRepeat[GAMEPADMAX];				// リピート情報
static int				padCount = 0;						// 検出したパッドの数

int						padRepeatCnt1[GAMEPADMAX][BUTTON_MAX];// リピートカウンタ1
int						padRepeatCnt2[GAMEPADMAX];			// リピートカウンタ2

JOYSTICK				stickL[GAMEPADMAX];					// 左スティック構造体
JOYSTICK				stickR[GAMEPADMAX];					// 右スティック構造体


//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!DInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&DInput, NULL);
	}

	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

 	// マウスの初期化
	//InitializeMouse(hInst, hWnd);
	
	// パッドの初期化
	InitializePad();

	return S_OK;
}


//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	// マウスの終了処理
	//UninitMouse();

	// パッドの終了処理
	UninitPad();

	if(DInput)
	{
		DInput->Release();
		DInput = NULL;
	}
}


//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
	// キーボードの更新
	UpdateKeyboard();

	// マウスの更新
	//UpdateMouse();
	
	// パッドの更新
	UpdatePad();
}


//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = DInput->CreateDevice(GUID_SysKeyboard, &DIDevKeyboard, NULL);
	if(FAILED(hr) || DIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = DIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = DIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	DIDevKeyboard->Acquire();

	return S_OK;
}


//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	if(DIDevKeyboard)
	{
		DIDevKeyboard->Release();
		DIDevKeyboard = NULL;
	}
}


//=============================================================================
// キーボードの更新
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// 前回のデータを保存
	memcpy(keyStateOld, keyState, NUM_KEY_MAX);

	// デバイスからデータを取得
	hr = DIDevKeyboard->GetDeviceState(sizeof(keyState), keyState);
	if(SUCCEEDED(hr))
	{
		for(int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			keyStateTrigger[cnt] = (keyStateOld[cnt] ^ keyState[cnt]) & keyState[cnt];
			keyStateRelease[cnt] = (keyStateOld[cnt] ^ keyState[cnt]) & ~keyState[cnt];
			keyStateRepeat[cnt] = keyStateTrigger[cnt];

			if(keyState[cnt])
			{
				keyStateRepeatCnt[cnt]++;
				if(keyStateRepeatCnt[cnt] >= FIRST_REPEAT_COUNT)
				{
					/* 自分で追記 : 一度停止した後のリピートの速度を遅くする処理*/
					padRepeatCnt2[0]++;
					if (padRepeatCnt2[0] >= SECOND_REPEAT_COUNT )
					{
						padRepeatCnt2[0] = 0;
						keyStateRepeat[cnt] = keyState[cnt];
					}
				}
			}
			else
			{
				keyStateRepeatCnt[cnt] = 0;
				keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		DIDevKeyboard->Acquire();
	}

	return S_OK;
}


//----------------------------------------------- 検査

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int key)
{
	return (keyState[key] & 0x80) ? true : false;
}


//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return (keyStateTrigger[key] & 0x80) ? true : false;
}


//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return (keyStateRepeat[key] & 0x80) ? true : false;
}


//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return (keyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// キー入力判定関数
//=============================================================================
bool GetKeyState(void)
{
	bool flag = FALSE;
	for (int i = 0; i < NUM_KEY_MAX; i++)
	{
		if (keyState[i] != NULL)
		{
			flag = TRUE;
		}
	}
	return flag;
}


//////////////////////////////////// Mouse ////////////////////////////////////

//=============================================================================
// マウスの初期化
//=============================================================================
HRESULT InitializeMouse(HINSTANCE hInst,HWND hWindow)
{
	HRESULT result;
	// デバイス作成
	result = DInput->CreateDevice(GUID_SysMouse,&mouse,NULL);
	if(FAILED(result) || mouse==NULL)
	{
		MessageBox(hWindow,"No mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// データフォーマット設定
	result = mouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Can't setup mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// 他のアプリと協調モードに設定
	result = mouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse mode error","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	
	// デバイスの設定
	DIPROPDWORD prop;
	
	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// マウスの移動値　相対値

	result = mouse->SetProperty(DIPROP_AXISMODE,&prop.diph);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse property error","Warning",MB_OK | MB_ICONWARNING);
		return result;	
	}
	
	// アクセス権を得る
	mouse->Acquire();
	return result;
}


//=============================================================================
// マウスの終了処理
//=============================================================================
void UninitMouse()
{
	if(mouse)
	{
		mouse->Unacquire();
		mouse->Release();
		mouse = NULL;
	}

}


//=============================================================================
// マウスの更新処理
//=============================================================================
HRESULT UpdateMouse()
{
	HRESULT result;
	// 前回の値保存
	DIMOUSESTATE2 lastMouseState = mouseState;
	// データ取得
	result = mouse->GetDeviceState(sizeof(mouseState),&mouseState);
	if(SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// マウスのボタン状態
		for(int i=0;i<8;i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else	// 取得失敗
	{
		// アクセス権を得てみる
		result = mouse->Acquire();
	}
	return result;
	
}


//----------------------------------------------- 検査

//=============================================================================
// 左プレス
//=============================================================================
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// 押されたときに立つビットを検査
}


//=============================================================================
// 左クリック
//=============================================================================
BOOL IsMouseLeftTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80);
}


//=============================================================================
// 右プレス
//=============================================================================
BOOL IsMouseRightPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[1] & 0x80);
}


//=============================================================================
// 右クリック
//=============================================================================
BOOL IsMouseRightTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[1] & 0x80);
}


//=============================================================================
// ホイールプレス
//=============================================================================
BOOL IsMouseCenterPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[2] & 0x80);
}


//=============================================================================
// ホイールクリック
//=============================================================================
BOOL IsMouseCenterTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[2] & 0x80);
}


//=============================================================================
// マウス座標取得
//=============================================================================
// X座標
long GetMouseX(void)
{
	return mouseState.lX;
}

// Y座標
long GetMouseY(void)
{
	return mouseState.lY;
}

// Z座標
long GetMouseZ(void)
{
	return mouseState.lZ;
}


//////////////////////////////////Game Pad/////////////////////////////////////

//=============================================================================
// コールバック関数
//=============================================================================

BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID )
{
	HRESULT result;

	result = DInput->CreateDevice(lpddi->guidInstance, &gamePad[padCount++], NULL);
	return DIENUM_CONTINUE;	// 次のデバイスを列挙

}


//=============================================================================
// ゲームパッドの初期化
//=============================================================================
HRESULT InitializePad(void)	
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// ジョイパッドを探す
	DInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

	for ( i = 0 ; i < padCount; i++ ) {
		// ジョイスティック用のデータ・フォーマットを設定
		result = gamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(result) )
			return false; // データフォーマットの設定に失敗

		// モードを設定（フォアグラウンド＆非排他モード）
//		result = gamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(result) )
//			return false; // モードの設定に失敗

		// 軸の値の範囲を設定
		// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
		// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
		// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_X; 
		gamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj		= DIJOFS_RZ;
		gamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_Y;
		gamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj		= DIJOFS_Z;
		gamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		// X軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_X;
		gamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		// Y軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_Y;
		gamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
			
		// ジョイスティック入力制御開始
		gamePad[i]->Acquire();

		// スティック使用フラグを初期化
		stickL[i].use = FALSE;
		stickR[i].use = FALSE;
	}

	return true;
}


//=============================================================================
// ゲームパッドの終了
//=============================================================================
void UninitPad(void)
{
	for (int i = 0 ; i < GAMEPADMAX ; i++) {
		if ( gamePad[i] )
		{
			gamePad[i]->Unacquire();
			gamePad[i]->Release();
		}
	}

}


//=============================================================================
// ゲームパッドの更新
//=============================================================================
void UpdatePad(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs[GAMEPADMAX];
	int				i;

	for (i = 0; i < padCount; i++ ) 
	{
		lastPadState[i] = padState[i];
		padState[i] = 0x00000000l;	// 初期化

		result = gamePad[i]->Poll();	// ジョイスティックにポールをかける
		if ( FAILED(result) ) {
			result = gamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = gamePad[i]->Acquire();
		}

		result = gamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs[i]);	// デバイス状態を読み取る
		if ( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED ) {
			result = gamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = gamePad[i]->Acquire();
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if ( dijs[i].lY < 0 )					padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if ( dijs[i].lY > 0 )					padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if ( dijs[i].lX < 0 )					padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if ( dijs[i].lX > 0 )					padState[i] |= BUTTON_RIGHT;
		//* 1 - Xボタン
		if ( dijs[i].rgbButtons[0] & 0x80 )		padState[i] |= BUTTON_X;
		//* 2 - Yボタン
		if ( dijs[i].rgbButtons[1] & 0x80 )		padState[i] |= BUTTON_Y;
		//* 3 - Aボタン
		if ( dijs[i].rgbButtons[2] & 0x80 )		padState[i] |= BUTTON_A;
		//* 4 - Bボタン
		if ( dijs[i].rgbButtons[3] & 0x80 )		padState[i] |= BUTTON_B;
		//* 5 - Lボタン
		if (dijs[i].rgbButtons[4] & 0x80)		padState[i] |= BUTTON_L;
		//* 6 - Rボタン
		if ( dijs[i].rgbButtons[5] & 0x80)		padState[i] |= BUTTON_R;
		//* 7 - LTボタン
		if (dijs[i].rgbButtons[6] & 0x80)		padState[i] |= BUTTON_LT;
		//* 8 - RTボタン
		if (dijs[i].rgbButtons[7] & 0x80)		padState[i] |= BUTTON_RT;
		//* 10 - Rスティックボタン
		if ( dijs[i].rgbButtons[9] & 0x80 )		padState[i] |= BUTTON_SR;
		//* 12 - STARTボタン
		if ( dijs[i].rgbButtons[11] & 0x80)		padState[i] |= BUTTON_START;
		//* 13 - Guideボタン
		if ( dijs[i].rgbButtons[12] & 0x80)		padState[i] |= BUTTON_M;


		// *Trigger設定
		padTrigger[i] = ((lastPadState[i] ^ padState[i])	// 前回と違っていて
						& padState[i]);						// しかも今ONのやつ

		// *Repeat設定
		padRepeat[i] = padTrigger[i];
		// 押されているボタンのリピートカウンタを回す　押されていないボタンは0に戻す
		padState[i] & BUTTON_UP		?	padRepeatCnt1[i][0]++ : padRepeatCnt1[i][0] = 0;
		padState[i] & BUTTON_DOWN	?	padRepeatCnt1[i][1]++ : padRepeatCnt1[i][1] = 0;
		padState[i] & BUTTON_LEFT	?	padRepeatCnt1[i][2]++ : padRepeatCnt1[i][2] = 0;
		padState[i] & BUTTON_RIGHT	?	padRepeatCnt1[i][3]++ : padRepeatCnt1[i][3] = 0;
		for (int j = 0; j < BUTTON_MAX; j++)
		{
			if (padRepeatCnt1[i][j] >= FIRST_REPEAT_COUNT)
			{
				padRepeatCnt2[i]++;
				/* 一度停止した後のリピートの速度を遅くする処理 */
				if (padRepeatCnt2[i] >= SECOND_REPEAT_COUNT)
				{
					padRepeatCnt2[i] = 0;
					padRepeat[i] = padState[i];
				}
			}
		}

		// 左ティック
		stickL[i].axisX = dijs[i].lX;
		stickL[i].axisY = dijs[i].lY;
		if (dijs[i].lX == 0 && dijs[i].lY == 0)
		{
			stickL[i].use = FALSE;
		}
		else
		{
			// スティック入力時に座標・回転角度を記録
			stickL[i].use = TRUE;
			stickL[i].rotAngle = (float)atan2(-dijs[i].lY, dijs[i].lX);
		}

		// 右スティック
		stickR[i].axisX = dijs[i].lZ;
		stickR[i].axisY = dijs[i].lRz;
		if (dijs[i].lZ == 0 && dijs[i].lRz == 0)
		{
			stickR[i].use = FALSE;
		}
		else
		{
			// スティック入力時に回転角度を記録
			stickR[i].use = TRUE;
			stickR[i].rotAngle = (float)atan2(-dijs[i].lRz, dijs[i].lZ);
		}
	}
}


//----------------------------------------------- 検査

//=============================================================================
// ボタンプレス
//=============================================================================
BOOL IsButtonPressed(int padNo,DWORD button)
{
	return (button & padState[padNo]);
}


//=============================================================================
// ボタントリガー
//=============================================================================
BOOL IsButtonTriggered(int padNo,DWORD button)
{
	return (button & padTrigger[padNo]);
}


//=============================================================================
// ボタンリリース
//=============================================================================
BOOL IsButtonReleased(int padNo, DWORD button)
{
	if (!(padState[padNo] & button)				// 調べたいボタンが押されていない
		&& lastPadState[padNo] & button)		// かつ直前は押されていた場合
	{
		return TRUE;
	}
	return FALSE;
}


//=============================================================================
// ボタンリピート
//=============================================================================
BOOL IsButtonRepeat(int padNo, DWORD button)
{	
	if (button & padRepeat[padNo])
	{
		padRepeat[padNo] = 0x00000000l;			// 初期化
		return TRUE;
	}
	return FALSE;
}


//=============================================================================
// ゲームパッドの状態取得
//=============================================================================
DWORD GetPadState(int no)
{
	return padState[no];
}


//=============================================================================
// 左スティック取得関数
//=============================================================================
JOYSTICK *GetStickL(int padNo)
{
	return &stickL[padNo];
}


//=============================================================================
// 右スティック取得関数
//=============================================================================
JOYSTICK *GetStickR(int padNo)
{
	return &stickR[padNo];
}


//=============================================================================
// ゲームパッドの接続台数を取得
//=============================================================================
int GetPadCount(void)
{
	return padCount;
}