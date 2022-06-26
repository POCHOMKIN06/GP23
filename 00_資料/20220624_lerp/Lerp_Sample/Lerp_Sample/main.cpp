#include <Windows.h>

#pragma comment(lib, "winmm.lib")

// �}�N����`
#define CLASS_NAME	"�T���v��"
#define WINDOW_NAME	"���`��ԉ��K"

#define SCREEN_WIDTH	(640)	// �E�B���h�E�̕�
#define SCREEN_HEIGHT	(480)	// �E�B���h�E�̍���

// �\���̒�`
typedef struct _tRootData
{
	POINT	start;	// 
	POINT	end;	// 
	int		t0;		// 
	int		t1;		// 
}tRootData;

// �v���g�^�C�v�錾
LRESULT	CALLBACK WndProc(HWND hWnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam);

// �O���[�o���ϐ�
HINSTANCE g_hInstance;
int g_nTime = 0;	// ����
POINT g_pos = { 100,100 };	// �~�̈ʒu
tRootData root[] = {
	
	
	
	
	{-1},
};
int	g_nRootIdx = 0;

// main
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;

	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	HWND	hWnd;
	MSG		msg;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// �^�C�}�[�̃Z�b�g(��3�������^�C�}�[�Ԋu�~���b)
	SetTimer(hWnd, 1, 20, NULL);

	// ���b�Z�[�W���[�v
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// �E�B���h�E�̓o�^��j��
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	switch (uMsg)
	{
	case WM_DESTROY:	// �E�B���h�E�j���̃��b�Z�[�W
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		// ���Ԃ̌o��
		
		// ���`��Ԍv�Z
		
		
		// ���[�g�̍X�V
		if (g_nTime >= root[g_nRootIdx].t1)
		{
			g_nRootIdx++;
			if (root[g_nRootIdx].start.x == -1)	// 
			{	// �I�_�Ȃ̂ŏ�����
				g_nRootIdx = 0;
				g_nTime = 0;
			}
		}
		// �ĕ`��w��
		InvalidateRect(hWnd, &rect, TRUE);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			// �ϐ��̍ď�����
			break;
		}
		break;

	case WM_PAINT:
	{
		HDC			hDC;
		PAINTSTRUCT	ps;
		char		aStr[256];

		// �`��̊J�n
		hDC = BeginPaint(hWnd, &ps);

		// �e�L�X�g�\��
		SetTextColor(hDC, RGB(255, 0, 0));
		wsprintf(aStr, "SPACE�ōď�����");
		TextOut(hDC, 10, 10, aStr, (int)strlen(aStr));

		// ����`��
		for (int i = 0; i < sizeof(root)/sizeof(tRootData) - 1; i++)
		{
			
			
		}

		// �ȉ~��`��
		// �y�����쐬
		HPEN hPen;		// ���݂̃y��
		HPEN hOldPen;	// �o�b�N�A�b�v�p
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		// �y���������ւ���
		hOldPen = (HPEN)SelectObject(hDC, hPen);
		// �`��
		Ellipse(hDC, g_pos.x - 20, g_pos.y - 20, g_pos.x + 20, g_pos.y + 20);
		// �y�������Ƃɖ߂�
		SelectObject(hDC, hOldPen);
		// �쐬�����y�����폜
		DeleteObject(hPen);

		// �`��̏I��
		EndPaint(hWnd, &ps);
	}
	break;

	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
