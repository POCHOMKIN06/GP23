#include <Windows.h>
#include <tchar.h>

#pragma comment(lib, "winmm.lib")

// �}�N����`
#define CLASS_NAME	TEXT("�T���v��")
#define WINDOW_NAME	TEXT("Window01")

#define SCREEN_WIDTH	(640)	// �E�B���h�E�̕�
#define SCREEN_HEIGHT	(640)	// �E�B���h�E�̍���

// �v���g�^�C�v�錾
LRESULT	CALLBACK WndProc(HWND hWnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam);

// �O���[�o���ϐ�
HINSTANCE g_hInstance;
POINT g_pos = { 100, 200 };
POINT g_size = { 200, 200 };
POINT g_move = { 4,4 };

// main
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;

	WNDCLASSEX wcex = {
		sizeof( WNDCLASSEX ),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor( NULL, IDC_ARROW ),	//�J�[�\���A�C�R���Ǎ�
		(HBRUSH)(COLOR_WINDOW+1),
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
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,	//�쐬�ʒu(x,y)
		CW_USEDEFAULT,	//����(0,0)
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

	SetTimer(hWnd, 100, 50, NULL);	//ms, 20fps

	// ���b�Z�[�W���[�v
	while (GetMessage(&msg, NULL, 0, 0 ) != 0)
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
	HDC hDC;			//�f�o�C�X�R���e�L�X�g�̃n���h��
	PAINTSTRUCT ps;		//�N���C�A���g�̈�ɕ`�悷�鎞�ɕK�v�ȍ\����
	TCHAR str[256];		//������\���p
	HPEN hPen;			//�y���I�u�W�F�N�g
	HPEN hPenOld;		//�ύX�O�̃I�u�W�F�N�g�ۑ��p
	HBRUSH hBrush;		//�u���V�I�u�W�F�N�g
	HBRUSH hBrushOld;	//�ύX�O�̃I�u�W�F�N�g�ۑ��p


	switch(uMsg)
	{
	case WM_DESTROY:	// �E�B���h�E�j���̃��b�Z�[�W
		PostQuitMessage(0);
		break;
	case WM_TIMER:
	{
		g_pos.x += 2;

		//�ĕ`����w��
		RECT rect = {
			0,0, SCREEN_WIDTH, SCREEN_HEIGHT
		};
		InvalidateRect(hWnd, &rect, TRUE);
		break;
	}
	case WM_PAINT:		//�`�施�߂��o��
	{
		//�`��J�n
		hDC = BeginPaint(hWnd, &ps);

		//�`�揈���͂����ɏ���
		//�e�L�X�g��\��
		wsprintf(str, TEXT("TEST"));	//�o�b�t�@�Ɋi�[
		TextOut(hDC, 100, 100, str, _tcslen(str));	//�\�����W,������ƒ���

		SetTextColor(hDC, RGB(255, 0, 0));			//�e�L�X�g�F�̐ݒ�
		TextOut(hDC, 120, 105, str, _tcslen(str));	//�\�����W,������ƒ���

		//����`��(�n�_->���̓_->���̎��̓_)
		MoveToEx(hDC, 400, 100, NULL);
		LineTo(hDC, 500, 200);
		LineTo(hDC, 400, 200);

		//�y�����쐬
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));	//����,����,�F
		//�y���������ւ�
		hPenOld = (HPEN)SelectObject(hDC, hPen);

		//�~��`��
		Ellipse(hDC,
			g_pos.x - g_size.x / 2,
			g_pos.y - g_size.y / 2,
			g_pos.x + g_size.x / 2,
			g_pos.y + g_size.y / 2
		);

		//�y����߂�
		SelectObject(hDC, hPenOld);
		//�쐬�����y���̍폜
		DeleteObject(hPen);


		//�u���V�̍쐬
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		//�u���V�������ւ�
		hBrushOld = (HBRUSH)SelectObject(hDC, hBrush);

		//�l�p��`��
		Rectangle(hDC, 340, 340, 460, 460);

		//�u���V��߂�
		SelectObject(hDC, hBrushOld);
		//�쐬�����u���V�̍폜
		DeleteObject(hBrush);

		//�`��I��
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_KEYDOWN:	//�L�[���������ꂽ
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			int id = MessageBox(hWnd,
				TEXT("�I�����܂���?"),
				TEXT("�m�F"),
				MB_YESNO);
			if (id == IDYES) {
				DestroyWindow(hWnd);
			}
		}
		case VK_UP:
			g_pos.y -= g_move.y;
			break;
		case VK_DOWN:
			g_pos.y += g_move.y;
			break;
		case VK_RIGHT:
			g_pos.x += g_move.x;
			break;
		case VK_LEFT:
			g_pos.x -= g_move.x;
			break;
			break;
		default:
			break;
		}
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
