#pragma once
#include "Engine/Util/EnumFlags.h"
#include "Engine/Common.h"

#include "Engine/Util/Singleton.h"

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
		: public Singleton<InputManager>
	{
		friend class Singleton<InputManager>;
		friend class GameEngine;
	public:
		struct tKey
		{
			eKeyCode  eType;
			eKeyState eState;
			bool	  bPressed;
		};

	private:
		InputManager();
		~InputManager();

	public:
		__forceinline eKeyState GetKeyState(eKeyCode keyCode) { return mKeys[static_cast<uint>(keyCode)].eState; }
		__forceinline float2 GetMousePos() { return mMousePos; }
		__forceinline float2 GetMouseDir() { return mMouseDir; }

		//GetKeyPress()		키를 누르는 시간만큼 true를 반환
		//GetKeyDown()	키를 눌렀을 때, 딱 한번 true를 반환
		//GetKeyUp()	키를 누르다 땠을 때, 딱 한번 true를 반환

		__forceinline bool GetKeyPress(eKeyCode keyCode)
		{
			return (eKeyState::PRESSED == mKeys[static_cast<uint>(keyCode)].eState);
		}

		__forceinline bool GetKeyDown(eKeyCode keyCode)
		{
			return (eKeyState::DOWN == mKeys[static_cast<uint>(keyCode)].eState);
		}

		__forceinline bool GetKeyUp(eKeyCode keyCode)
		{
			return (eKeyState::UP == mKeys[static_cast<uint>(keyCode)].eState);
		}	

	private:
		void update();

	private:
		std::vector<tKey> mKeys;
		float2 mMousePos;
		float2 mMousePosPrev;
		float2 mMouseDir;
	};
}
