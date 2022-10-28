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
		Memory::RD_Memory.initialize(Memory::RD_Memory.Ntdll);
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

		Logs.clear();
		Logs += "matrix_module:";
		Logs += to_string((int)Memory::matrix_module);
		LOG_OUT(L_DEBUG, Logs.c_str());
	}
}

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