#include "ScreenManager.h"
#include "Difu/Utils/Assert.h"

#include <raylib.h>

namespace ScreenManager
{
	static Screen currentScreen;
	static Screen nextScreen;
    
	static bool IsInTransition = false;
	static float TransitionBeginTime = 0.0f;

	static bool ReturnTrue(float) { return true; }
	static void DoNothing(float) {}

	void UpdateScreen()
	{
		if (currentScreen.UpdateFunction)
			currentScreen.UpdateFunction(GetFrameTime());
	}

	void RenderScreen()
	{
		if (currentScreen.RenderFunction)
			currentScreen.RenderFunction();
	}

	void UpdateScreenManager()
	{
		if (IsInTransition)
			return;

		if (nextScreen.LoadFunction)
		{
			if (currentScreen.UnloadFunction)
			{
				currentScreen.UnloadFunction();
				// delete currentScreen;
			}

			currentScreen = nextScreen;
			nextScreen = {};

			currentScreen.LoadFunction();
			OnResize();

			if (!currentScreen.IsStartTransitionDoneFunction)
				currentScreen.IsStartTransitionDoneFunction = &ReturnTrue;
			if (!currentScreen.IsEndTransitionDoneFunction)
				currentScreen.IsEndTransitionDoneFunction = &ReturnTrue;

			if (!currentScreen.RenderStartTransitionFunction)
				currentScreen.RenderStartTransitionFunction = &DoNothing;
			if (!currentScreen.RenderEndTransitionFunction)
				currentScreen.RenderEndTransitionFunction = &DoNothing;

			LOG_INFO("Screen successfully loaded");

			IsInTransition = true;
			TransitionBeginTime = GetTime();
		}
	}

	void UpdateAndRenderTransitions()
	{
		if (!IsInTransition)
			return;

		if (nextScreen.LoadFunction)
		{
			if (currentScreen.LoadFunction)
			{
				float time = (float)GetTime() - TransitionBeginTime;
				if (!currentScreen.IsEndTransitionDoneFunction(time))
					currentScreen.RenderEndTransitionFunction(time);
				else
				{
					currentScreen.RenderEndTransitionFunction(time - GetFrameTime());
					IsInTransition = false;
				}
			}
			else
				IsInTransition = false;
		}
		else
		{
			if (currentScreen.LoadFunction)
			{
				float time = (float)GetTime() - TransitionBeginTime;
				if (!currentScreen.IsStartTransitionDoneFunction(time))
					currentScreen.RenderStartTransitionFunction(time);
				else
				{
					currentScreen.RenderStartTransitionFunction(time - GetFrameTime());
					IsInTransition = false;
				}
			}
			else
			{
				IsInTransition = false;
			}
		}
	}

	void OnResize()
	{
		if (currentScreen.OnResize)
			currentScreen.OnResize(GetScreenWidth(), GetScreenHeight());
	}

    
	void ChangeScreen(const Screen screen)
	{
		if (IsInTransition)
			return;

		nextScreen = screen;

		ASSERT(nextScreen.UpdateFunction, "Missing Update Function");
		ASSERT(nextScreen.RenderFunction, "Missing Render Function");
		ASSERT(nextScreen.LoadFunction, "Missing Load Function");
		ASSERT(nextScreen.UnloadFunction, "Missing Unload Function");

		IsInTransition = true;
		TransitionBeginTime = GetTime();
	}
} // namespace ScreenManager
