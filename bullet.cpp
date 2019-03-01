//=============================================================================
//
// ポリゴン処理 [bullet.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "bullet.h"
#include "input.h"
//#include <Windows.h>
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBullet(void);
void SetVertexBullet(void);
void SetTextureBullet(int cntPattern);
BULLET	 bullet[BULLET_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);
	for (int i = 0; i < BULLET_MAX; i++) {
		BULLET *bullet = GetBullet(i);
		bullet->g_posBullet = D3DXVECTOR3(player->g_posPlayer.x , player->g_posPlayer.y, 0.0f);
		bullet->use = FALSE;
		// 頂点情報の作成
		MakeVertexBullet();
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
		TEXTURE_GAME_BULLET,				// ファイルの名前
		&bullet->g_pD3DTextureBullet);		// 読み込むメモリのポインタ			
	}
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	if (bullet->g_pD3DTextureBullet != NULL)	//
	{	// テクスチャの開放
		bullet->g_pD3DTextureBullet->Release();
		bullet->g_pD3DTextureBullet = NULL;
	}	
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	// アニメーション 
	PLAYER *player = GetPlayer(0);
	for (int i = 0; i < player->bullet_num + 1; i++) {
		BULLET *bullet = GetBullet(i);
		if (bullet->g_posBullet.y < (-TEXTURE_SAMPLE00_SIZE_Y3) || bullet->g_posBullet.x < (-TEXTURE_SAMPLE00_SIZE_X3)) {
			bullet->g_posBullet = D3DXVECTOR3(player->g_posPlayer.x + TEXTURE_SAMPLE00_SIZE_X / 2, player->g_posPlayer.y, 0.0f);
			bullet->use = FALSE;
		}
		if (bullet->use) {
			//OutputDebugStringA("B ");
			bullet->g_posBullet.x -= 8;
		}
		else
		{
			bullet->g_posBullet = D3DXVECTOR3(player->g_posPlayer.x + TEXTURE_SAMPLE00_SIZE_X / 2, player->g_posPlayer.y, 0.0f);
		}	
	}
	MakeVertexBullet();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);
	BULLET *bullet = GetBullet(0);
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, bullet->g_pD3DTextureBullet);	
	for (int i = 0; i < player->bullet_num; i++) {
		BULLET *bullet = GetBullet(i);
		if (bullet->use) {
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, bullet->g_vertexWk, sizeof(VERTEX_2D));
		}
	}	
}
//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBullet(void)
{	
	for (int i = 0; i < BULLET_MAX; i++) {		
		BULLET *bullet = GetBullet(i);
		// 頂点座標の設定
		SetVertexBullet();
		// rhwの設定
		bullet->g_vertexWk[0].rhw =
		bullet->g_vertexWk[1].rhw =
		bullet->g_vertexWk[2].rhw =
		bullet->g_vertexWk[3].rhw = 1.0f;
		// 反射光の設定
		bullet->g_vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		bullet->g_vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		bullet->g_vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		bullet->g_vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		// テクスチャ座標の設定
		SetTextureBullet(bullet->g_nPatternAnim);
	}
	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++) {
		BULLET *bullet = GetBullet(i);
		// 頂点座標の設定
		bullet->g_vertexWk[0].vtx = D3DXVECTOR3(bullet->g_posBullet.x, bullet->g_posBullet.y, bullet->g_posBullet.z);
		bullet->g_vertexWk[1].vtx = D3DXVECTOR3(bullet->g_posBullet.x + TEXTURE_SAMPLE00_SIZE_X3, bullet->g_posBullet.y, bullet->g_posBullet.z);
		bullet->g_vertexWk[2].vtx = D3DXVECTOR3(bullet->g_posBullet.x, bullet->g_posBullet.y + TEXTURE_SAMPLE00_SIZE_Y3, bullet->g_posBullet.z);
		bullet->g_vertexWk[3].vtx = D3DXVECTOR3(bullet->g_posBullet.x + TEXTURE_SAMPLE00_SIZE_X3, bullet->g_posBullet.y + TEXTURE_SAMPLE00_SIZE_Y3, bullet->g_posBullet.z);
	}	
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureBullet(int cntPattern)
{
	for (int i = 0; i < BULLET_MAX; i++) {
		BULLET *bullet = GetBullet(i);
		// テクスチャ座標の設定	
		bullet->g_vertexWk[0].tex = D3DXVECTOR2(0, 0);
		bullet->g_vertexWk[1].tex = D3DXVECTOR2(1, 0);
		bullet->g_vertexWk[2].tex = D3DXVECTOR2(0, 1);
		bullet->g_vertexWk[3].tex = D3DXVECTOR2(1, 1);
	}
	
}
/*******************************************************************************
関数名:	BULLET *GetBulletAdr( int pno )
引数:	int pno : Bullet番号
戻り値:	BULLET *
説明:	Bulletのアドレスを取得する
*******************************************************************************/
BULLET *GetBullet(int pno)
{
	return &bullet[pno];
}