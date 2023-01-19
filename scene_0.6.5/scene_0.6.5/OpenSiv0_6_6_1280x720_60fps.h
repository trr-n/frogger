#pragma once
# include <Siv3D.hpp> // OpenSiv3D v0.6.6
using namespace std::chrono;

/*
このヘッダーファイルの機能
画素数を１２８０×７２０にして、家庭用ゲームと同じフレームレートにする（６０）


PCスペックによって、起動時に一瞬ちらつく、または、画素数を自分で決めたい場合は、
Main関数直後に画面を設定して下さい

  例：
	void Main()
	{
		System::DisplaySet(DisplayResolution::～～～～);
		・・・
		・・・
		・・・



注意：1時的に
	s3d::System
	を、System2で乗っ取っています。それゆえ System::～ 使える機能が少ないです。
	足りない機能は、このソースの最下部の Sleep関数のように追加して下さい

	s3d::System を使いたい場合は、#undef System してください
*/

class System2
{
	System2() {}
public:
	inline static high_resolution_clock::time_point start; // 計測スタート時刻を保存 //XXXX

	static const int FPS = 60; // 1秒間に1画面を書き換える回数
	/// @brief フルスクリーンの画素数
	inline static Size displayResolution = DisplayResolution::HD_1280x720;
#pragma optimize("", off)
	/// @brief 画面表示前の初期化
	/// @return 
	static bool Initialize()
	{
		start = high_resolution_clock::now();      // 計測スタート時刻を保存
		displayResolution = { 0,0 };

		auto end = high_resolution_clock::now();
		auto durtion = end - start;        // 要した時間を計算
		auto msec = duration_cast<std::chrono::nanoseconds>(durtion).count();
		return true;	// ここでは最適化無視しないと怖い
	}
	/// @brief System::Update() を乗っ取り
	/// @return 
	static bool Update()
	{
		if (displayResolution.x==0)
		{
			displayResolution = DisplayResolution::HD_1280x720;
			DisplaySet();	// 本当はMain()に入る直前に行いたいのだが仕方なく
		}

		auto elapset = [&]()
		{
			auto end = high_resolution_clock::now();
			auto durtion = end - start;        // 要した時間を計算
			auto msec = duration_cast<std::chrono::nanoseconds>(durtion).count();
			return msec;
		};
		long long sleepTime = elapset()/1000000/2;
		System::Sleep(sleepTime);
		while (elapset() < 1000 * 1000 * 1000 / FPS); //16

		start = high_resolution_clock::now();      // 計測スタート時刻を保存

		return System::Update();
	}

private:
	/// @brief 画面サイズ変更
	/// @param displayResolution 画素数指定 
	/// @return 画面変更成功
	static bool DisplaySet()
	{
		bool ok = Window::Resize(displayResolution);
		Scene::Resize(displayResolution);
		return ok;
	}
public:
	/// @brief 画面の画素数を設定する
	/// 基本的には、引数は一つだけにするが、
	/// Windowサイズとシーンサイズの画素数が違う場合、２つ引数を書く
	/// @param windowsSize ウィンドウの実画素数 ex  DisplayResolution::HD_1280x720
	/// @param sceneSize ウィンドウ内の仮想の画素数  ここは書かなくても可能
	/// @return ウィンドウサイズの変更に成功したら true
	static bool DisplaySet(Size windowsSize, Size sceneSize = { 0,0 })
	{
		displayResolution = windowsSize;
		bool ok = Window::Resize(windowsSize);
		if (sceneSize.x != 0)
		{
			Scene::Resize(windowsSize);
		}
		return ok;
	}
	/// @brief  指定したミリ秒待つ
	/// @param millisecond ミリ秒
	static void Sleep(int32 millisecond)
	{
		System::Sleep(millisecond);
	}
	/// @brief 指定した秒数待つ
	/// @param duration 秒数（少数可能）
	static void Sleep(Duration& duration)
	{
		System::Sleep(duration);
	}

	static void Exit() {
		System::Exit();
	}
};
/// <summary>アプリケーションの終了</summary>
void Exit()
{
	System::Exit();
}

bool startSysytem = { System2::Initialize() };

#define System System2
constexpr auto S3D_SYSTEM = "s3d::System を使いたい場合は、#undef System してください" ;
#define S3D_SYSTEM_NO_USE using System = System2;

