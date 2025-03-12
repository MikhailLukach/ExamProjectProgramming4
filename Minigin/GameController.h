#pragma once
#include <memory>

namespace dae
{
	class GameController
	{
	public:
		explicit GameController(int controllerIndex);
		~GameController();

		void Update();
		bool IsDownThisFrame(unsigned int button) const;
		bool IsUpThisFrame(unsigned int button) const;
		bool IsPressed(unsigned int button) const;

		int GetControllerIndex() const;
		bool IsConnected() const;

		enum GamepadButton
		{
			DPAD_UP = 0x0001,
			DPAD_DOWN = 0x0002,
			DPAD_LEFT = 0x0004,
			DPAD_RIGHT = 0x0008,
			START = 0x0010,
			BACK = 0x0020,
			LEFT_THUMB = 0x0040,
			RIGHT_THUMB = 0x0080,
			LEFT_SHOULDER = 0x0100,
			RIGHT_SHOULDER = 0x0200,
			A = 0x1000,
			B = 0x2000,
			X = 0x4000,
			Y = 0x8000
		};

		GameController(const GameController&) = delete;
		GameController& operator=(const GameController&) = delete;
		GameController(GameController&&) noexcept = default;
		GameController& operator=(GameController&&) noexcept = default;

	private:
		class GameControllerImpl;
		std::unique_ptr<GameControllerImpl> m_pImpl; // Pointer to the implementation
	};
}

