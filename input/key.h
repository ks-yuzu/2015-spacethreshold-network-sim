#pragma once
#pragma warning (disable : 4305)

#include <array>
#include <Windows.h>

class Keyboard
{
	public:
		virtual ~Keyboard() = default;

		static Keyboard& GetInstance()
		{
			static Keyboard kb;
			return kb;
		}

		int operator()(int key)
		{
			return putKeyTime[key];
		}

		bool Update()
		{
			std::array<BYTE, 256> keybuf;
			if( !GetKeyboardState(keybuf.data()) )	return -1;

			for(int i = 0; i < 256; i++)
				putKeyTime[i] =  ( keybuf[i] & 0x80 ) ? putKeyTime[i] +1 : 0;

			return ( putKeyTime[VK_ESCAPE] ) ? false : true; 
		}


	private:
		Keyboard(){}; //¶¬‹ÖŽ~
		int putKeyTime[256];
};

