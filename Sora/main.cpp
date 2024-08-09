#include "Application.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	using namespace sora;

#if DEBUG || _DEBUG
	// メモリリークを検出する。
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// コンソールウィンドウを作成する。
	AllocConsole();
	FILE* fp = nullptr;
	freopen_s(&fp, "CONOUT$", "w", stdout);

	// コンソールウィンドウを最大化する。
	HWND consoleWindow = GetConsoleWindow();
	ShowWindow(consoleWindow, SW_MAXIMIZE);
#endif

	Application::Create();
	Application::Run();
	Application::Destroy();

#if DEBUG || _DEBUG
	// コンソールウィンドウを解放する。
	if (fp)
	{
		fclose(fp);
	}
	FreeConsole();
#endif

	return 0;
}
