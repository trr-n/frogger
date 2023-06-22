#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <winreg.h>
#include <atlstr.h>

#pragma region read reg

//int main()
//{
//	// https://www.typea.info/blog/index.php/2022/07/31/cpp_reg_get_strings/
//	// Windowsレジストリから文字列値を取得するために、Win32 RegGetValue() APIを使用する
//	// 取得する文字列のサイズは事前には分からないので、RegGetValue()関数を2回呼び出す
//	// 最初の呼び出しの目的は、文字列を格納するのに十分な大きさのバッファを割り当てるためのサイズ値を取得する
//	// サイズ値が決まれば、適切なバッファサイズを持つ CString インスタンスを割り当てることができる
//	// 次に、RegGetValue()関数を再度呼び出し、今度はpvDataパラメータに有効なバッファポインタと、有効な最大バッファサイズを渡す
//	// 2回目の呼び出しが成功すると、RegGetValue()関数は提供されたバッファに実際の文字列値を格納
//
//	DWORD keyType = 0;
//	DWORD dataSize = 0;
//	const DWORD flags = RRF_RT_REG_SZ; // 文字列（REG_SZ）のみ読み込み可
//	LONG result = ::RegGetValue(
//		HKEY_CURRENT_USER,
//		_T("Software\\レジストリサブキー"),
//		_T("レジストリ値"),
//		flags,
//		&keyType,
//		nullptr, // pvData == nullptr ? Request buffer size
//		&dataSize);
//
//	if (result != ERROR_SUCCESS)
//	{
//		return 1;
//	}
//
//	CString text;
//	const DWORD bufferLength = dataSize / sizeof(WCHAR); // WCHARの長さ
//	WCHAR* const textBuffer = text.GetBuffer(bufferLength);
//
//	// レジストリから文字列値をローカルの文字列バッファに読み込む
//	result = ::RegGetValue(
//		HKEY_CURRENT_USER,
//		_T("Software\\レジストリサブキー"),
//		_T("レジストリ値"),
//		flags,
//		nullptr,
//		textBuffer, // このバッファに文字列を書き込む
//		&dataSize);
//
//	if (result != ERROR_SUCCESS)
//	{
//		return 1;
//	}
//
//	const DWORD actualStringLength = dataSize / sizeof(WCHAR);
//
//	// -1 to exclude the terminating NUL
//	text.ReleaseBufferSetLength(actualStringLength - 1);
//
//	// std::wcout << text.GetString() << std::endl;
//
//	// CStringをマルチバイト文字に変換
//	CT2A scr(text);
//
//	// std::wcout << scr << std::endl;
//
//	// プログラムを起動
//	system(scr);
//
//	//FreeConsole();
//	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
//
//	return 0;
//
	/*
	MSEdgeHTM - Edge
	ChromeHTML - Chrome
	VivaldiHTM.GUXOMTPISLLFOBXOQGDWIWRECU - vivaldi
	*/
	//}

#pragma endregion

void MyGame::Credits()
{
	while (Update())
	{
		// auto browser = textBuffer;

		if (KeySpace.down())
		{
			ChangeScene(&MyGame::Title);
		}

		if (SimpleGUI::Button(U"Title", Vec2{ 5, 5 }))
		{
			ChangeScene(&MyGame::Title);
		}

		odc.resized(100, 100).drawAt(160, 430);
		s3d.resized(75, 75).drawAt(1048, 430);
		credit.resized(1280, 720).drawAt(Scene::Center());

		int y60 = font60.fontSize(), y = y60 / 2;
		int y30 = font30.fontSize() + 10;

		auto alpha = 0.0;
		auto& sr = Rect{ 411, 192, 460, 60 }. draw(ColorF{ Palette::Darkcyan, alpha });
		auto& cg = Rect{ 441, 290, 400, 60 }. draw(ColorF{ Palette::Darkmagenta, alpha });
		auto& od = Rect{ 110, 380, 100, 100 }.draw(ColorF{ Palette::Darkred , alpha });
		auto& si = Rect{ 1000, 380, 100, 100 }.draw(ColorF{ Palette::Darkblue, alpha });
		auto& gt = Rect{ 570, 385, 140, 60 }.draw(ColorF{ Palette::Darkkhaki, alpha });

		if (sr.leftClicked())
			system("start chrome.exe https://spriters-resource.com");

		if (cg.leftClicked())
			system("start chrome.exe https://classicgaming.cc");

		if (od.leftClicked())
			system("start chrome.exe https://odc.ac.jp");

		if (si.leftClicked())
			system("start chrome.exe https://siv3d.github.io");

		if (gt.leftClicked())
			system("start chrome.exe https://www.bf-lessson.com/1062");

#if _DEBUG
		font30(Cursor::Pos()).draw(0, font30.fontSize(), Palette::Black);
#endif
	}
}
