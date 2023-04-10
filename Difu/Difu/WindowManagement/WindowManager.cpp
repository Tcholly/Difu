#include "WindowManager.h"

#include "Difu/ScreenManagement/ScreenManager.h"
#include "Difu/ECS/ECSManager.h"
#include "Difu/Utils/Logger.h"

static bool gameRunning;
static Color clear_color = WHITE;

bool WindowManager::InitWindow(const std::string& title, int width, int height, bool resizable)
{
	SetTraceLogLevel(LOG_WARNING);

	if (resizable)
		SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	::InitWindow(width, height, title.c_str());
	// MaximizeWindow();
	gameRunning = true;

	if (IsWindowReady())
	{
		LOG_INFO("Window successfully created:\n\tTitle:\t    {}\n\tSize:\t    {}x{}\n\tResizable:  {}", title, width, height, resizable);
		return true;
	}
	else
	{
		LOG_ERROR("Failed to create window");
		return false;
	}
}

void WindowManager::RunWindow()
{
	while (gameRunning)
	{
		ScreenManager::UpdateScreenManager(); // Change Screen if necessary

		gameRunning = !WindowShouldClose();
		if (IsWindowResized())
			ScreenManager::OnResize();
		ScreenManager::UpdateScreen();
		ECS::Update(GetFrameTime());

		BeginDrawing();
		ClearBackground(clear_color);
		BeginMode2D(ECS::GetPrimaryCamera());
		ECS::Render();
		EndMode2D();
		ScreenManager::RenderScreen();
		ScreenManager::UpdateAndRenderTransitions();
		EndDrawing();
	}

	::CloseWindow();
	LOG_INFO("Window closed successfully");
}

void WindowManager::CloseWindow()
{
	gameRunning = false;
}


Vector2 WindowManager::GetWindowSize()
{
    return {(float)GetScreenWidth(), (float)GetScreenHeight()};
}

void WindowManager::SetBackgroundColor(Color color)
{
	clear_color = color;
}
