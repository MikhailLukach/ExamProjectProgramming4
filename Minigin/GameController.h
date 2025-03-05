#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h> 
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
namespace dae
{
	class GameController
	{
	public:
		explicit GameController(int controllerIndex);
		void Update();

		bool IsDownThisFrame(unsigned int button) const;
		bool IsUpThisFrame(unsigned int button) const;
		bool IsPressed(unsigned int button) const;

		int GetControllerIndex() const { return m_ControllerIndex; }
		bool IsConnected() const { return m_Connected; }

	private:
		int m_ControllerIndex;
		bool m_Connected;
		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PreviousState{};
		unsigned int buttonsPressedThisFrame = 0;
		unsigned int buttonsReleasedThisFrame = 0;
	};
}

