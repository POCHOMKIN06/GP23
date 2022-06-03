#pragma once

#include <Windows.h>
#include <tchar.h>

class Window
{
public:
	static const TCHAR* ClassName;	//!< ウィンドウクラス登録時のクラス名

public:
	/**
	* @brief ウィンドウプロシージャ
	* @param[in] window_handle イベントが発生したウィンドウのハンドル
	* @param[in] message_id イベントメッセージ
	* @param[in] wparam イベント情報
	* @param[in] lparam イベント情報
	* @return メッセージ処理結果
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
	* @brief ウィンドウ生成関数@n
	* ウィンドウを生成して成功したらtrue、失敗したらfalseを返す
	* @return 生成の成否 成功(true)
	*/
	bool Create();

	/* @brief getter */
	HWND GetWinHandle() const { return WindowHandle_; }

private:
	/**
	* @brief ウィンドウクラス登録関数@n
	* ウィドウを作成するためのウィンドウクラス情報を登録して成功したらtrue、失敗したらfalseを返す
	* @return 登録の成否 成功(true)
	*/
	bool EntryWindowClass();

	/**
	* @brief ウィドウのリサイズ関数@n
	* 生成後にクライアント領域のサイズにリサイズする
	* @param[in] リサイズ対象となるウィンドウハンドル
	*/
	void ResizeWindow(HWND window_handle);

private:
	HWND WindowHandle_;		//!< @brief ウィンドウハンドル
	const TCHAR* Title_;	//!< @brief ウィンドウタイトル名
	UINT Width_;			//!< @brief 横幅(クライアント領域)
	UINT Height_;			//!< @brief 縦幅(クライアント領域)
};
