#include "GameController.h"
#include <windows.h> 
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

namespace dae
{
    class GameController::GameControllerImpl
    {
    public:
        explicit GameControllerImpl(int controllerIndex)
            : m_ControllerIndex(controllerIndex), m_Connected(false)
        {
            ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
        }

        void Update()
        {
            m_PreviousState = m_CurrentState;
            ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

            if (XInputGetState(m_ControllerIndex, &m_CurrentState) == ERROR_SUCCESS)
            {
                m_Connected = true;
                unsigned int previousButtons = m_PreviousState.Gamepad.wButtons;
                unsigned int currentButtons = m_CurrentState.Gamepad.wButtons;

                buttonsPressedThisFrame = (currentButtons) & (~previousButtons);
                buttonsReleasedThisFrame = (~currentButtons) & (previousButtons);
            }
            else
            {
                m_Connected = false;
                buttonsPressedThisFrame = 0;
                buttonsReleasedThisFrame = 0;
            }
        }

        bool IsDownThisFrame(unsigned int button) const { return buttonsPressedThisFrame & button; }
        bool IsUpThisFrame(unsigned int button) const { return buttonsReleasedThisFrame & button; }
        bool IsPressed(unsigned int button) const { return m_CurrentState.Gamepad.wButtons & button; }

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


    GameController::GameController(int controllerIndex) : m_pImpl(std::make_unique<GameControllerImpl>(controllerIndex)) {}

    GameController::~GameController() = default;

    void GameController::Update() 
    { 
        m_pImpl->Update(); 
    }
    bool GameController::IsDownThisFrame(unsigned int button) const
    { 
        return m_pImpl->IsDownThisFrame(button); 
    }
    bool GameController::IsUpThisFrame(unsigned int button) const 
    { 
        return m_pImpl->IsUpThisFrame(button); 
    }
    bool GameController::IsPressed(unsigned int button) const 
    { 
        return m_pImpl->IsPressed(button); 
    }
    int GameController::GetControllerIndex() const 
    { 
        return m_pImpl->GetControllerIndex(); 
    }
    bool GameController::IsConnected() const 
    { 
        return m_pImpl->IsConnected(); 
    }
}

