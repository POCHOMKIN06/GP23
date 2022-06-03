#pragma once

#include <Windows.h>
#include <tchar.h>

class Window
{
public:
	static const TCHAR* ClassName;	//!< �E�B���h�E�N���X�o�^���̃N���X��

public:
	/**
	* @brief �E�B���h�E�v���V�[�W��
	* @param[in] window_handle �C�x���g�����������E�B���h�E�̃n���h��
	* @param[in] message_id �C�x���g���b�Z�[�W
	* @param[in] wparam �C�x���g���
	* @param[in] lparam �C�x���g���
	* @return ���b�Z�[�W��������
	*/
	static LRESULT CALLBACK WindowProc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam);
public:
	/*
	* @brief Constructor
	* @param[in] title	title name
	* @param[in] width client area width
	* @param[in] height client area height
	*/
	Window(const TCHAR* title, UINT width, UINT height)
		: Title_(title)
		, Width_(width)
		, Height_(height)
	{}
	/* @brief Destructor */
	~Window()
	{
		UnregisterClass(ClassName, (HINSTANCE)GetWindowLongPtr(WindowHandle_, GWLP_HINSTANCE));
	}

	/**
	* @brief �E�B���h�E�����֐�@n
	* �E�B���h�E�𐶐����Đ���������true�A���s������false��Ԃ�
	* @return �����̐��� ����(true)
	*/
	bool Create();

	/* @brief getter */
	HWND GetWinHandle() const { return WindowHandle_; }

private:
	/**
	* @brief �E�B���h�E�N���X�o�^�֐�@n
	* �E�B�h�E���쐬���邽�߂̃E�B���h�E�N���X����o�^���Đ���������true�A���s������false��Ԃ�
	* @return �o�^�̐��� ����(true)
	*/
	bool EntryWindowClass();

	/**
	* @brief �E�B�h�E�̃��T�C�Y�֐�@n
	* ������ɃN���C�A���g�̈�̃T�C�Y�Ƀ��T�C�Y����
	* @param[in] ���T�C�Y�ΏۂƂȂ�E�B���h�E�n���h��
	*/
	void ResizeWindow(HWND window_handle);

private:
	HWND WindowHandle_;		//!< @brief �E�B���h�E�n���h��
	const TCHAR* Title_;	//!< @brief �E�B���h�E�^�C�g����
	UINT Width_;			//!< @brief ����(�N���C�A���g�̈�)
	UINT Height_;			//!< @brief �c��(�N���C�A���g�̈�)
};
