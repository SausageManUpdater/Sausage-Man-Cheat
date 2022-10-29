#include "main.h"
BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LOG_START("LOG", false);
        LOG_OUT(L_INFO, "inject successfully!");
        CreateThreads(&Cheat::run);//启动线程
        break;
    case DLL_THREAD_ATTACH:
        break;
    }
    return TRUE;
}
namespace Cheat
{
    void run()
    {
        Memory::GetMem();
        InitD3DHook(D3D11, &Draws);
    }

    void Draws()
    {
        Draw::DrawClass MENU;
        MENU.DrawMainMenu();
    }
}

namespace Cheat::Memory
{
	void Memory::GetMem()
	{
		string Logs = "";
		Memory::event_module = GetModuleHandleA("GameAssembly.dll");
		Memory::event_module = GetModuleHandleA("GameAssembly.dll");
		Memory::matrix_module = GetModuleHandleA("UnityPlayer.dll");

		Memory::pid = getpid();
		Memory::hProcess = (HANDLE)GetHandleById(Memory::pid);



		string Logs = "";
		Logs += "pid:";
		Logs += to_string((int)Memory::pid);
		LOG_OUT(L_DEBUG, Logs.c_str());

		Logs.clear();
		Logs += "hProcess:";
		Logs += to_string((int)Memory::hProcess);
		LOG_OUT(L_DEBUG, Logs.c_str());

		Logs.clear();
		Logs += "event_module:";
		Logs += to_string((int)Memory::event_module);
		LOG_OUT(L_DEBUG, Logs.c_str());
		Memory::matrix_module = GetModuleHandleA("UnityPlayer.dll");

		Logs.clear();
		Logs += "matrix_module:";
		Logs += to_string((int)Memory::matrix_module);
		LOG_OUT(L_DEBUG, Logs.c_str());
		Memory::event_Base = ((int)event_module + Memory::event_module_Pointer::eventBase);
		Memory::matrix_Base = ((int)matrix_module + Memory::matrix_module_Pointer::matrixBase);
	}
}

bool m_IsBlockingInput;
bool _Customthemes;

namespace Cheat::Draw
{
	void Draw::DrawClass::DrawMainMenu()
	{
		ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);

		ImGui::Begin("SoFunny Cheat");

		ImGui::BeginGroup();

		if (ImGui::Checkbox("Block key/mouse", &m_IsBlockingInput))
		{
			SetInputLock(this, m_IsBlockingInput);
		}


		if (ImGui::BeginListBox("##listbox 2", ImVec2(175, -FLT_MIN)))
		{
			ImGui::Selectable("ESP");
			ImGui::Selectable("Memory");
			ImGui::Selectable(tf8("测试").c_str());

			ImGui::TextColored(ImColor(255, 136, 0, 255), tf8("源码:").c_str());
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xC0D5C8FF);
			TextURL("GitHub", "http://github.com", true, false);
			ImGui::PopStyleColor();

			ImGui::EndListBox();
		}

		ImGui::EndGroup();


		ImGui::SameLine();

		constexpr float width = 200.0f;
		ImGui::SetNextItemWidth(width);
		ImGui::BeginCombo("Profile", "Test");

		ImGui::BeginGroup();

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildR", ImVec2(0, 0), true, window_flags);

		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::EndGroup();


		ImGui::Checkbox("Show ImGui Dome", &_Customthemes);


		ImGui::End();

		ImGui::ShowDemoWindow(&_Customthemes);
		
	}
}
namespace Cheat::Draw
{
	void DrawESP()
	{
		ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
		float WindowsH, WindowsW;
		WindowsH = ImGui::GetWindowHeight() / 2;
		WindowsW = ImGui::GetWindowWidth() / 2;
		int matrix_L1 = *((int*)(Memory::matrix_Base)) + Memory::matrix_module_Pointer::matrix_Pointer_L1;
		int matrix_L2 = *(int*)matrix_L1 + Memory::matrix_module_Pointer::matrix_Pointer_L2;
		int matrix_L3 = *(int*)matrix_L2 + Memory::matrix_module_Pointer::matrix_Pointer_L3;
		int matrix_L4 = *(int*)matrix_L3 + Memory::matrix_module_Pointer::matrix_Pointer_L4;
		int matrix_L5 = *(int*)matrix_L4 + Memory::matrix_module_Pointer::matrix_Pointer_L5;
		int matrix_address = *(int*)matrix_L5 + Memory::matrix_module_Pointer::matrix_Pointer_L6;
		float matrix[4][4] =
		{
		*(float*)(matrix_address + 0x00),*(float*)(matrix_address + 0x04),*(float*)(matrix_address + 0x08),*(float*)(matrix_address + 0x0C),
		*(float*)(matrix_address + 0x10),*(float*)(matrix_address + 0x14),*(float*)(matrix_address + 0x18),*(float*)(matrix_address + 0x1C),
		*(float*)(matrix_address + 0x20),*(float*)(matrix_address + 0x24),*(float*)(matrix_address + 0x28),*(float*)(matrix_address + 0x2C),
		*(float*)(matrix_address + 0x30),*(float*)(matrix_address + 0x34),*(float*)(matrix_address + 0x38),*(float*)(matrix_address + 0x3C)
		};
		int event_L1 = *((int*)(Memory::event_Base)) + Memory::event_module_Pointer::event_Pointer_L1;
		int event_L2 = *(int*)event_L1 + Memory::event_module_Pointer::event_Pointer_L2;
		int event_L3 = *(int*)event_L2 + Memory::event_module_Pointer::event_Pointer_L3;
		int event_L4 = *(int*)event_L3 + Memory::event_module_Pointer::event_Pointer_L4;
		int event_L5 = *(int*)event_L4 + Memory::event_module_Pointer::event_Pointer_L5;
		for (int i = 0; i < 100; i++)
		{
			int event_L6 = *(int*)event_L5 + (Memory::event_module_Pointer::event_Pointer_L6 + (i * Memory::event_module_Pointer::Pointer_L6_Product));
			float X, Y, Z, ViewW, ViewX, ViewY;
			X = *(float*)(*(int*)event_L6 + Memory::event_module_Pointer::Pointer_X);
			Y = *(float*)(*(int*)event_L6 + Memory::event_module_Pointer::Pointer_Y);
			Z = *(float*)(*(int*)event_L6 + Memory::event_module_Pointer::Pointer_Z);
			ViewW = matrix[3][0] * X + matrix[3][1] * Y + matrix[3][2] * Z + matrix[3][3];
			ViewW = 1 / ViewW;
			ViewX = WindowsW + (matrix[0][0] * X + matrix[0][1] * Y + matrix[0][2] * Z + matrix[0][3]) * ViewW * WindowsW;
			ViewY = WindowsH - (matrix[1][0] * X + matrix[1][1] * Y + matrix[1][2] * (Z + 1.75) + matrix[1][3]) * ViewW * WindowsH;

		}
	}
}
