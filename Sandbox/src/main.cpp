#include <Difu/ScreenManagement/ScreenManager.h>
#include "Screens/GameScreen.h"
#include <Difu/WindowManagement/WindowManager.h>

int main()
{
	if (WindowManager::InitWindow("Sandbox", 800, 480, true))
	{
		ScreenManager::ChangeScreen(GameScreen::GetScreen());
		WindowManager::RunWindow();
	}
}
