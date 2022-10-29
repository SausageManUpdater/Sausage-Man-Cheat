#pragma once
#include <Library.h>
using namespace Library;

namespace Cheat
{
	void run();
	void Draws();

	namespace Memory
	{

		HMODULE event_module;
		HMODULE matrix_module;

		int event_Base;
		int matrix_Base;

		enum event_module_Pointer :int
		{
			eventBase = 0x3532484,
			event_Pointer_L1 = 0x5C,
			event_Pointer_L2 = 0x4,
			event_Pointer_L3 = 0x10,
			event_Pointer_L4 = 0x2C,
			event_Pointer_L5 = 0x8,
			event_Pointer_L6 = 0x10,
			Pointer_L6_Product = 0x4,
			Pointer_X = 0x110,
			Pointer_Y = 0x114,
			Pointer_Z = 0x118,
			Pointer_Kills = 0x90,
			Pointer_Name = 0xC,
			Pointer_Helmet = 0x240,
			Pointer_Armor = 0x244, 
			Pointer_Healthy = 0x218,
			Pointer_Energy = 0x224,
			Pointer_Team = 0xE8,
			Pointer_Mate = 0xE4,

		};
		
		enum matrix_module_Pointer :int
		{
			matrixBase = 0x129B0C0,
			matrix_Pointer_L1 = 0x10,
			matrix_Pointer_L2 = 0x3C,
			matrix_Pointer_L3 = 0x28,
			matrix_Pointer_L4 = 0x18,
			matrix_Pointer_L5 = 0x8,
			matrix_Pointer_L6 = 0xC4,
		};

		void GetMem();
	}

	namespace Draw
	{
		void DrawESP();
		class DrawClass
		{
		public:
			void DrawMainMenu();

		private:

		};
	}
}