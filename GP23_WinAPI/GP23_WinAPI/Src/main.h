#pragma once
//#define STRICT				// Windows�v���O�����ɑ΂���u�^�ۏ؁v������
#include <Windows.h>
#include <tchar.h>

#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib, "winmm.lib")

// �}�N����`
//#define CLASS_NAME	TEXT("�T���v��")
#define WINDOW_NAME	TEXT("CG�͌|�p���I")

const UINT	SCREEN_WIDTH  = 640;				// �X�N���[�����r�b�g�}�b�v��
const UINT	SCREEN_HEIGHT = 480;				// �X�N���[�����r�b�g�}�b�v����

#define FPS		(20)	//�X�VFPS

//�J���[�w��
#define BLACK	RGB(0,0,0)
#define RED		RGB(255,0,0)
#define GREEN	RGB(0,255,0)
#define BLUE	RGB(0,0,255)
