//=============================================================================
// 四角形処理 [quadrangle.cpp]
//
// Author : HAL東京昼間部 2年制ゲーム学科 GP11B341 24 中込和輝
// 作成日 : 2018/12/26
//=============================================================================


//*****************************************************************************
// インクルード
//*****************************************************************************
#include "quadrangle.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************


//=============================================================================
// 四角形２Ｄポリゴンの頂点情報を作成
//=============================================================================
HRESULT MakeVertexQuad(QUADRANGLE *quad)
{
	// 頂点座標の設定
	SetVertexQuad(quad);

	// rhwの設定
	quad->vertexWk[0].rhw =
	quad->vertexWk[1].rhw =
	quad->vertexWk[2].rhw =
	quad->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	D3DCOLOR color = D3DCOLOR_RGBA(255, 255, 255, 255);
	SetColorQuad(quad, color);

	// テクスチャ座標の設定
	quad->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	quad->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	quad->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	quad->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}


//=============================================================================
// 四角形２Ｄポリゴンの頂点座標の設定
//=============================================================================
void SetVertexQuad(QUADRANGLE *quad)
{
	// アフィン変換
	quad->vertexWk[0].vtx.x = quad->transform.pos.x - cosf(quad->centralAngle + quad->transform.rot.z) * quad->radius;
	quad->vertexWk[0].vtx.y = quad->transform.pos.y - sinf(quad->centralAngle + quad->transform.rot.z) * quad->radius;
	quad->vertexWk[1].vtx.x = quad->transform.pos.x + cosf(quad->centralAngle - quad->transform.rot.z) * quad->radius;
	quad->vertexWk[1].vtx.y = quad->transform.pos.y - sinf(quad->centralAngle - quad->transform.rot.z) * quad->radius;
	quad->vertexWk[2].vtx.x = quad->transform.pos.x - cosf(quad->centralAngle - quad->transform.rot.z) * quad->radius;
	quad->vertexWk[2].vtx.y = quad->transform.pos.y + sinf(quad->centralAngle - quad->transform.rot.z) * quad->radius;
	quad->vertexWk[3].vtx.x = quad->transform.pos.x + cosf(quad->centralAngle + quad->transform.rot.z) * quad->radius;
	quad->vertexWk[3].vtx.y = quad->transform.pos.y + sinf(quad->centralAngle + quad->transform.rot.z) * quad->radius;
}


//=============================================================================
// 四角形２Ｄポリゴン頂点カラーの設定
//=============================================================================
void SetColorQuad(QUADRANGLE *quad, D3DCOLOR setColor)
{
	// 一括設定
	quad->vertexWk[0].diffuse = setColor;
	quad->vertexWk[1].diffuse = setColor;
	quad->vertexWk[2].diffuse = setColor;
	quad->vertexWk[3].diffuse = setColor;
}


//=============================================================================
// 四角形２Ｄポリゴンの初期化
//=============================================================================
void InitQuad(QUADRANGLE *quad, int sizeX, int sizeY)
{
	quad->centralAngle = atan2f((float)sizeY / 2, (float)sizeX / 2);			// 中心座標から頂点への角度を初期化
	D3DXVECTOR2 vec = D3DXVECTOR2((float)sizeX / 2, (float)sizeY / 2);			// 中心座標から頂点へのベクトルを求める
	quad->originalRadius = quad->radius = D3DXVec2Length(&vec);					// 半径の初期化
	quad->transform.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 座標の初期化
	quad->transform.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 回転の初期化
	MakeVertexQuad(quad);														// 四角形２Ｄポリゴンの頂点情報を作成
}




