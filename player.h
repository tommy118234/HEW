//=============================================================================
//
// �|���S������ [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

#define TEXTURE_GAME_PLAYER		_T("data/TEXTURE/player.png")	// �T���v���p�摜
#define TEXTURE_GAME_PLAYER2	_T("data/TEXTURE/attack.png")	// �T���v���p�摜
#define TEXTURE_GAME_PLAYER3	_T("data/TEXTURE/player_star.png")	// �T���v���p�摜
#define TEXTURE_PLAYER_SIZE_X	(140)							// �e�N�X�`���T�C�Y
#define TEXTURE_PLAYER_SIZE_Y	(200)							// ����


#define TEXTURE_PATTERN_DIVIDE_X	(5)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

#define PLAYER_MAX	(1)			// �v���C���[�̐�
#define MOVE_MAX	(3)			// �v���C���[�̐�

// �X�N���[���J�n���W
#define LEFT_SCROLL_LINE_X	(SCREEN_WIDTH * 0.05f)
#define RIGHT_SCROLL_LINE_X	(SCREEN_WIDTH * 0.5f)


/*******************************************************************************
* �\���̒�`
********************************************************************************/
struct	PLAYER										// �L�����N�^�[�̊�{�\����
{
	CHARACTER				status;					// ���ʃp�����[�^�[
	D3DXVECTOR3				pos;					// �|���S���̈ړ���
	int						patternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	float					countAnim;				// �A�j���[�V�����J�E���g	
	LPDIRECT3DTEXTURE9		texture[MOVE_MAX];		// �e�N�X�`���ւ̃|���S��
	VERTEX_2D				vtx[NUM_VERTEX];		// ���_���i�[���[�N
	float					radius;					// �|���S���̊p�x	
	float					baseAngle;
	int						direction = -1;
	int						moving_cooldown = -1;	
	int						speed;
	float					speed_boost;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT		InitPlayer(int type);
void		UninitPlayer(void);
void		UpdatePlayer(void);
void		DrawPlayer(int pno);
PLAYER		*GetPlayer(int pno);	// �v���C���[��(�A�h���X)���擾
#endif