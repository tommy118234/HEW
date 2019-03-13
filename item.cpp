//=============================================================================
//
// �A�C�e������ [item.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP11B341 24 �����a�P
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "road.h"
#include "item.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���ꏊ
#define TEXTURE_ITEM		_T("data/TEXTURE/pop_corn.png")

// �A�C�e���̍��W
#define INIT_POS_X			(100.0f)
#define MOVE_SPEED_X		(2.0f)

/* �A�C�e���Z�b�g�֌W */
#define IEMSET_RAND_RANGE	(700)				// �A�C�e���o���p�x�̒����l
#define ITEM_SET_NUMBER		(0)					// �A�C�e���o��

// ���|�b�v��
#define SET_POS_Y(_lane)	(SCREEN_HEIGHT * 0.5f + _lane * (SCREEN_HEIGHT * 0.1f))	// ���W
#define SET_SIZE_X(_lane)	(ITEM_SIZE_X + _lane * ITEM_SIZE_X * 0.5)				// �T�C�YX
#define SET_SIZE_Y(_lane)	(ITEM_SIZE_Y + _lane * ITEM_SIZE_Y * 0.5)				// �T�C�YY

// �T�C���g
#define WAVE_CNT_UNIT		(0.02f)				// �J�E���g�P��radian
#define WAVE_HEIGHT			(2.0f)				// ����

#define LANE_MAX			(4)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexItem(ITEM *item);				// ���_�̍쐬
void SetVertexItem(ITEM *item);					// ���_���W�̐ݒ�
void SetTextureItem(ITEM *item);				// �e�N�X�`�����W�̐ݒ�
void SetItemGameStage(void);					// �A�C�e�����Z�b�g


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ITEM item;
float waveAngleCounter;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitItem(int type)
{
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_ITEM,
			&item.pTexture);
	}

	item.pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	item.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	item.use = FALSE;
	item.sizeX = ITEM_SIZE_X;
	item.sizeY = ITEM_SIZE_Y;
	MakeVertexItem(&item);

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitItem(void)
{
	SAFE_RELEASE(item.pTexture)
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateItem(void)
{
	if (item.use == FALSE)
	{
		item.cntNonUseTime++;
		if (item.cntNonUseTime > SECOND(10))
		{
			SetItemGameStage();
		}
		return;
	}

	  
	// �A�C�e���̈ړ�
	item.pos.x -= MOVE_SPEED_X;
	//item.pos.y += WAVE_HEIGHT * sin(waveAngleCounter);

	// ��ʒ[�ŏ���
	if (item.pos.x + ITEM_SIZE_X < 0.0f)
	{
		item.use = FALSE;
	}

	SetVertexItem(&item);
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(int itemNo)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (item.use)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, item.pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, item.vertexWk, sizeof(VERTEX_2D));
	}
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexItem(ITEM *item)
{
	// ���_���W�̐ݒ�	
	SetVertexItem(item);

	// ���_�J���[�̐ݒ�
	SetColorItem(item, D3DCOLOR_RGBA(255, 255, 255, 255));

	// �e�N�X�`�����W�̐ݒ�
	SetTextureItem(item);

	// rhw�̐ݒ�
	item->vertexWk[0].rhw =
		item->vertexWk[1].rhw =
		item->vertexWk[2].rhw =
		item->vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexItem(ITEM *item)
{
	item->vertexWk[0].vtx = D3DXVECTOR3(item->pos.x, item->pos.y, item->pos.z);
	item->vertexWk[1].vtx = D3DXVECTOR3(item->pos.x + item->sizeX, item->pos.y, item->pos.z);
	item->vertexWk[2].vtx = D3DXVECTOR3(item->pos.x, item->pos.y + item->sizeY, item->pos.z);
	item->vertexWk[3].vtx = D3DXVECTOR3(item->pos.x + item->sizeX, item->pos.y + item->sizeY, item->pos.z);
}


//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorItem(ITEM *item, D3DCOLOR setCol)
{
	item->vertexWk[0].diffuse = setCol;
	item->vertexWk[1].diffuse = setCol;
	item->vertexWk[2].diffuse = setCol;
	item->vertexWk[3].diffuse = setCol;
}


//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureItem(ITEM *item)
{
	item->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	item->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	item->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	item->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


//=============================================================================
// ITEM�̎擾
//=============================================================================
ITEM *GetItem(void)
{
	return &item;
}


//=============================================================================
// �A�C�e�����Z�b�g
//=============================================================================
void SetItemGameStage(void)
{
	// �o���p�x����
	int number = rand() % IEMSET_RAND_RANGE;
	if (number != ITEM_SET_NUMBER)
	{
		return;
	}
	
	// ���[���������_���Ɍ���
	int lane = -1;
	lane = rand() % LANE_MAX;

	// ���W�Ƒ傫����ݒ肵���|�b�v�̏���
	item.use = TRUE;
	item.cntNonUseTime = 0;
	item.pos = D3DXVECTOR3((float)SCREEN_WIDTH, (float)SET_POS_Y(lane), (float)lane);
	item.sizeX = SET_SIZE_X(lane);
	item.sizeY = SET_SIZE_Y(lane);
}
