#pragma once
#include "Singleton.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
		typedef struct XINPUT_STATE {
			DWORD dwPacketNumber;
			XINPUT_GAMEPAD Gamepad;
		} XINPUT_STATE;

		typedef struct XINPUT_GAMEPAD {
			WORD wButtons;
			BYTE bLeftTrigger;
			BYTE bRightTrigger;
			SHORT sThumbLX;
			SHORT sThumbLY;
			SHORT sThumbRX;
			SHORT sThumbRY;
		} XINPUT_GAMEPAD;

	public:
		bool ProcessInput();
	};

}
