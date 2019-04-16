/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�G���� [bullet.h]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#ifndef _BULLET_H_
#define _BULLET_H_
// �}�N����`
#define	NUM_BULLET						(2)			// �|���S����
#define TEXTURE_GAME_BULLET				_T("data/TEXTURE/bullet2.png")	// �T���v���p�摜
#define TEXTURE_BULLET_SIZE_X			(120)		// �e�N�X�`���T�C�Y
#define TEXTURE_BULLET_SIZE_Y			(200)		// ����
#define TEXTURE_PATTERN_DIVIDE_X_BULLET	(1)			// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_BULLET	(1)			// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_BULLET			(TEXTURE_PATTERN_DIVIDE_X_BULLET*TEXTURE_PATTERN_DIVIDE_Y_BULLET)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_BULLET			(50)		// �A�j���[�V�����̐؂�ւ��J�E���g
#define BULLET_MAX						(1)		// �o���b�g�̍ő吔
#define BULLET_SPEED					(30.0f)		// �o���b�g�̈ړ��X�s�[�h
/*******************************************************************************
* �\���̒�`
********************************************************************************/
struct  BULLET											// �o���b�g�̍\����
{	
	bool					use;						// true:�g�p  false:���g�p
	D3DXVECTOR3				pos;						// �|���S���̈ړ���
	LPDIRECT3DTEXTURE9		texture;					// �e�N�X�`���ւ̃|���S��
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N	
	float					radius;						// �|���S���̔��a
	float					baseAngle = 0.0;			// �|���S���̊p�x	
	int						atk;
	int						direction;
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(int type);
void	UninitBullet(void);
void	UpdateBullet(void);
void	DrawBullet(void);
void	SetBullet(D3DXVECTOR3 pos,int atk, int dir);
BULLET  *GetBullet(int pno);							//�o���b�g��(�A�h���X)���擾
#endif