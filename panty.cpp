//=============================================================================
//
// �p���e�B���� [panty.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP11B341 24 �����a�P
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "panty.h"
#include "font.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���ꏊ
#define TEXTURE_PANTY		_T("data/TEXTURE/pant.png")

// �p���e�B�̃T�C�Y
#define PANTY_SIZE_X		(50)
#define PANTY_SIZE_Y		(50)	

// �p���e�B�̍��W
#define INIT_POS_X			(300)
#define INIT_POS_Y			(50.0f)
#define INTARVAL_X			(10.0f)
#define	AJUST_Y				(5.0f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPanty(void);						// ���_�̍쐬
void SetVertexPanty(void);							// ���_���W�̐ݒ�
void SetColorPanty(void);							// ���_�J���[�̐ݒ�
void SetTexturePanty(void);							// �e�N�X�`�����W�̐ݒ�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMEPANTY panty;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitPanty(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_PANTY,
			&panty.pTexture);
	}

	panty.pos = D3DXVECTOR3(INIT_POS_X, INIT_POS_Y, 0.0f);
	panty.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ZeroMemory(panty.text, sizeof(TCHAR) * TEXT_MAX);
	panty.numPanty = 0;
	MakeVertexPanty();
	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitPanty(void)
{
	SAFE_RELEASE(panty.pTexture)
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdatePanty(void)
{
	// �����̍X�V
	wsprintf(panty.text, _T("�~ %d\n"), panty.numPanty);

	// ���_���W�̐ݒ�
	SetVertexPanty();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_P))
	{
		AddNumPanty();
	}
#endif
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawPanty(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �p���e�B
	pDevice->SetTexture(0, panty.pTexture);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, panty.vertexWk, sizeof(VERTEX_2D));

	// �~ ����
	LPD3DXFONT font = GetFont(IMPACT);
	RECT rect = { panty.pos.x + PANTY_SIZE_X + INTARVAL_X,  panty.pos.y +  AJUST_Y, SCREEN_WIDTH, SCREEN_HEIGHT };
	font->DrawText(NULL, panty.text, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPanty(void)
{
	// ���_���W�̐ݒ�	
	SetVertexPanty();

	// ���_�J���[�̐ݒ�
	SetColorPanty();

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePanty();

	// rhw�̐ݒ�
	panty.vertexWk[0].rhw =
		panty.vertexWk[1].rhw =
		panty.vertexWk[2].rhw =
		panty.vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexPanty(void)
{
	panty.vertexWk[0].vtx = D3DXVECTOR3(panty.pos.x, panty.pos.y, panty.pos.z);
	panty.vertexWk[1].vtx = D3DXVECTOR3(panty.pos.x + PANTY_SIZE_X, panty.pos.y, panty.pos.z);
	panty.vertexWk[2].vtx = D3DXVECTOR3(panty.pos.x, panty.pos.y + PANTY_SIZE_Y, panty.pos.z);
	panty.vertexWk[3].vtx = D3DXVECTOR3(panty.pos.x + PANTY_SIZE_X, panty.pos.y + PANTY_SIZE_Y, panty.pos.z);
}


//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorPanty(void)
{
	panty.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	panty.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	panty.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	panty.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTexturePanty(void)
{
	panty.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	panty.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	panty.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	panty.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


//=============================================================================
// �p���e�B�̐���ǉ�
//=============================================================================
void AddNumPanty(void)
{
	panty.numPanty++;
}


//=============================================================================
// �p���e�B�擾
//=============================================================================
GAMEPANTY *GetPanty(void)
{
	return &panty;
}