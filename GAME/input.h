//=============================================================================
//
// 入力処理 [input.h]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_


//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"


//*****************************************************************************
// マクロ・列挙型定義
//*****************************************************************************
// プログラム分けするときに使う
#define	USE_KEYBOARD					// 宣言するとキーボードで操作可能になる
#define	USE_MOUSE						// 宣言するとマウスで操作可能になる
#define	USE_PAD							// 宣言するとパッドで操作可能になる

/* game pad情報 */
#define BUTTON_UP		0x00000001l		// 方向キー上(.IY<0)
#define BUTTON_DOWN		0x00000002l		// 方向キー下(.IY>0)
#define BUTTON_LEFT		0x00000004l		// 方向キー左(.IX<0)
#define BUTTON_RIGHT	0x00000008l		// 方向キー右(.IX>0)

#define BUTTON_A		0x00000010l	
#define BUTTON_B		0x00000020l	
#define BUTTON_X		0x00000080l	
#define BUTTON_Y		0x00000100l		
#define BUTTON_RT		0x00002000l	
#define BUTTON_LT		0x00000200l		
#define BUTTON_L		0x00000400l		
#define BUTTON_R		0x00000800l		
#define BUTTON_SR		0x00000040l	
#define BUTTON_START	0x00001000l	
#define BUTTON_M		0x00004000l
#define GAMEPADMAX		(4)				// 同時に接続するジョイパッドの最大数をセット
#define BUTTON_MAX		(14)


//*****************************************************************************
// 構造体定義
//*****************************************************************************
// JOYSTICK構造体
typedef struct{
	BOOL	use;		// 使用中か
	long	axisX;		// X座標
	long	axisY;		// Y座標
	float	rotAngle;	// 回転角度
}JOYSTICK;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);


//---------------------------- keyboard
bool GetKeyboardPress(int nKey);		// キープレス
bool GetKeyboardTrigger(int nKey);		// キートリガー
bool GetKeyboardRepeat(int nKey);		// キーリピート
bool GetKeyboardRelease(int nKey);		// キーリリース
bool GetKeyState(void);					//キー入力の有無を判定しTRUEかFALSEを返す


//---------------------------- mouse
BOOL IsMouseLeftPressed(void);			// 左クリックした状態
BOOL IsMouseLeftTriggered(void);		// 左クリックした瞬間
BOOL IsMouseRightPressed(void);			// 右クリックした状態
BOOL IsMouseRightTriggered(void);		// 右クリックした瞬間
BOOL IsMouseCenterPressed(void);		// 中クリックした状態
BOOL IsMouseCenterTriggered(void);		// 中クリックした瞬間
long GetMouseX(void);					// マウスがX方向に動いた相対値
long GetMouseY(void);					// マウスがY方向に動いた相対値
long GetMouseZ(void);					// マウスホイールが動いた相対値


//---------------------------- game pad
BOOL IsButtonPressed(int padNo,DWORD button);	// ボタンプレス
BOOL IsButtonTriggered(int padNo,DWORD button);	// ボタントリガー
BOOL IsButtonReleased(int padNo, DWORD button);	// ボタンリリース
BOOL IsButtonRepeat(int padNo, DWORD button);	// ボタンリピート
DWORD GetPadState(int no);						// パッドの状態を調べる
JOYSTICK *GetStickL(int padNo);					// 左スティックを取得
JOYSTICK *GetStickR(int padNo);					// 右スティックを取得
int GetPadCount(void);							// ゲームパッドの接続台数を取得


#endif
