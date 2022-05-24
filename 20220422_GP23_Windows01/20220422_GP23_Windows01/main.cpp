#include <Windows.h>

#pragma comment(lib, "winmm.lib")

// �}�N����`
#define CLASS_NAME	"�T���v��"
#define WINDOW_NAME	"Window01"

#define SCREEN_WIDTH	(320)	// �E�B���h�E�̕�
#define SCREEN_HEIGHT	(320)	// �E�B���h�E�̍���

// �v���g�^�C�v�錾
LRESULT	CALLBACK WndProc(HWND hWnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam);

// �O���[�o���ϐ�
HINSTANCE g_hInstance;


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
		LoadCursor( NULL, IDC_ARROW ),
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
	switch(uMsg)
	{
	case WM_DESTROY:	// �E�B���h�E�j���̃��b�Z�[�W
		PostQuitMessage(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
