#pragma once
#include <Engine/Util/EnumFlags.h>
#include <Engine/Common.h>

#include <Engine/Util/Singleton.h>

namespace core
{
	enum class eKeyCode
	{
		//Alphabet
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L, 
		Z, X, C, V, B, N, M,
		
		//Special Key
		ENTER, ESC, LSHIFT, LALT, LCTRL,
		SPACE, LEFT, RIGHT, UP, DOWN,
		LBTN, RBTN,
		
		//Num Pad
		NUM_0, NUM_1, NUM_2,
		NUM_3, NUM_4, NUM_5,
		NUM_6, NUM_7, NUM_8,
		NUM_9,

		//Num Line
		N_0, N_1, N_2, N_3, N_4, N_5,
		N_6, N_7, N_8, N_9,
		
		END,
	};

	enum eKeyState
	{
		DOWN,
		PRESSED,
		UP,
		NONE,
	};



	class InputManager
	{
		friend class GameEngine;

		DECLARE_SINGLETON(InputManager);
	private:
		InputManager();
		void init();

		~InputManager();

	public:
		struct tKey
		{
			eKeyCode  m_key_type;
			eKeyState m_status;
			bool	  m_b_pressed;
		};

	public:
		inline eKeyState get_key_state(eKeyCode keyCode) { return m_keys[static_cast<uint>(keyCode)].m_status; }
		inline float2 get_mouse_position() { return m_mouse_position; }
		inline float2 get_mouse_direction() { return m_mouse_direction; }

		//is_key_pressed()		키를 누르는 시간만큼 true를 반환
		//is_key_down()	키를 눌렀을 때, 딱 한번 true를 반환
		//is_key_up()	키를 누르다 땠을 때, 딱 한번 true를 반환

		inline bool is_key_pressed(eKeyCode keyCode)
		{
			return (eKeyState::PRESSED == m_keys[static_cast<uint>(keyCode)].m_status);
		}

		inline bool is_key_down(eKeyCode keyCode)
		{
			return (eKeyState::DOWN == m_keys[static_cast<uint>(keyCode)].m_status);
		}

		inline bool is_key_up(eKeyCode keyCode)
		{
			return (eKeyState::UP == m_keys[static_cast<uint>(keyCode)].m_status);
		}	

	private:
		void update();

	private:
		std::vector<tKey> m_keys;
		float2 m_prev_mouse_position;
		float2 m_mouse_position;
		float2 m_mouse_direction;
	};
}
