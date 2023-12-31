
#include "pch.h"
#include "Application.h"

#ifdef DEBUG
#define main int main()
#else
//Hide cmd prompt in windows cuz it's annoying af
#define main int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif


	

main {

	auto app = std::make_unique<uvy::Application>();

	while (app->IsRunning()) {
		app->Update();
		app->LateUpdate();
		app->Render();
	}

}