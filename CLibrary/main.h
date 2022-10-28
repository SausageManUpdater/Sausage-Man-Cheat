#pragma once
#include <Library.h>
using namespace Library;

namespace Cheat
{
	void run();
	void Draws();

	namespace Memory
	{
		HANDLE hProcess;
		DWORD pid;

		HMODULE event_module;
		HMODULE matrix_module;

		enum event_module_Pointer :DWORD
		{
			Pointer_L1 = 0x5C,
			Pointer_L2 = 0x4,
			Pointer_L3 = 0x10,
			Pointer_L4 = 0x2C,
			Pointer_L5 = 0x8,
			Pointer_L6 = 0x10,
			Pointer_L6_Product = 0x4,
		};
		
		LibMemory RD_Memory;

		void GetMem();
	}

	namespace Draw
	{
		class DrawClass
		{
		public:
			void DrawMainMenu();

		private:
			bool m_IsBlockingInput;
			bool _Customthemes;
		};
	}
}