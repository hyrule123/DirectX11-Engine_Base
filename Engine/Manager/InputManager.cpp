
#include "InputManager.h"


#include <Engine/GameEngine.h>

#include <Engine/Util/AtExit.h>

namespace core
{
	constexpr int ASCII[(uint)eKeyCode::END] =
	{
		//Alphabet
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		
		//Special Key
		VK_RETURN, VK_ESCAPE, VK_LSHIFT, VK_LMENU, VK_LCONTROL,
		VK_SPACE, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		VK_LBUTTON, VK_RBUTTON,

		//Num Pad
		VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2,
		VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5,
		VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8,
		VK_NUMPAD9,

		//Num Line
		'0', '1', '2', '3', '4', '5',
		'6', '7', '8', '9',
	};

	InputManager::InputManager()
		: m_keys{}
		, m_prev_mouse_position{}
		, m_mouse_position{}
		, m_mouse_direction{}
	{}
	void InputManager::init()
	{
		for (uint i = 0; i < (uint)eKeyCode::END; i++)
		{
			tKey key;
			key.m_key_type = (eKeyCode)i;
			key.m_status = eKeyState::NONE;
			key.m_b_pressed = false;

			m_keys.push_back(key);
		}

		m_mouse_position = {};
		m_prev_mouse_position = {};
		m_mouse_direction = {};

		AtExit::add_func(InputManager::destroy_inst);
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		if (GetFocus())
		{
			//KEY
			for (uint i = 0; i < (uint)eKeyCode::END; ++i)
			{
				// 해당키가 현재 눌려있다.
				if (GetAsyncKeyState(ASCII[i]) & 0x8000)
				{
					// 이전 프레임에도 눌려 있었다.
					if (m_keys[i].m_b_pressed)
						m_keys[i].m_status = eKeyState::PRESSED;
					else
						m_keys[i].m_status = eKeyState::DOWN;

					m_keys[i].m_b_pressed = true;
				}
				else // 해당키가 현재 안눌려있다.
				{
					// 이전 프레임에는 눌려 있었다.
					if (m_keys[i].m_b_pressed)
						m_keys[i].m_status = eKeyState::UP;
					else // 이전 프레임에도 안눌려 있었다.
						m_keys[i].m_status = eKeyState::NONE;

					m_keys[i].m_b_pressed = false;
				}
			}
			
			//Mouse 위치 갱신
			m_prev_mouse_position = m_mouse_position;

			POINT mousePos = {};
			GetCursorPos(&mousePos);
			ScreenToClient(GameEngine::get_inst().get_HWND(), &mousePos);
			m_mouse_position.x = static_cast<float>(mousePos.x);
			m_mouse_position.y = static_cast<float>(mousePos.y);

			m_mouse_direction = m_mouse_position - m_prev_mouse_position;
			m_mouse_direction.y *= -1.f;
		}
		else
		{
			for (uint i = 0; i < (uint)eKeyCode::END; ++i)
			{
				if (eKeyState::DOWN == m_keys[i].m_status || eKeyState::PRESSED == m_keys[i].m_status)
					m_keys[i].m_status = eKeyState::UP;
				else if (eKeyState::UP == m_keys[i].m_status)
					m_keys[i].m_status = eKeyState::NONE;

				m_keys[i].m_b_pressed = false;
			}
		}
	}
}
