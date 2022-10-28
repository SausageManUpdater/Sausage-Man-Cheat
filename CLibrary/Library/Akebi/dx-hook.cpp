#include <Library.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Library
{
	struct Data
	{
		LPBYTE data;
		DWORD size;
	};

	static Data _customFontData{};
	// D3X HOOK DEFINITIONS
	typedef HRESULT(__stdcall* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	void (*p_)();
	static unordered_set<void*> _inputLockers;
	// Definition of WndProc Hook. Its here to avoid dragging dependencies on <windows.h> types.
	static IDXGISwapChainPresent fnIDXGISwapChainPresent;
	static ID3D11Device* pDevice = nullptr;
	static float _globalFontSize = 16.0f;
	static bool _isCustomFontLoaded = false;
	static constexpr int _fontSizeStep = 13.0f;
	static constexpr int _fontSizeMax = 64;
	static constexpr int _fontsCount = _fontSizeMax / _fontSizeStep;
	static std::array<ImFont*, _fontsCount> _fonts;
	static WNDPROC OriginalWndProcHandler;
	static ID3D11RenderTargetView* mainRenderTargetView;
	
	static void SetupImGuiStyle()
	{
		ImGui::GetStyle().FrameRounding = 4.0f;
		ImGui::GetStyle().GrabRounding = 4.0f;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.17f, 0.21f, 0.24f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.15f, 0.14f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.09f, 0.11f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.15f, 0.22f, 0.25f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.25f, 0.27f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.19f, 0.24f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.19f, 0.22f, 0.24f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.22f, 0.65f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.16f, 0.19f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.14f, 0.18f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.39f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.53f, 0.37f, 0.37f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.23f, 0.23f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.74f, 0.86f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.47f, 0.68f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.68f, 0.80f, 1.00f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.14f, 0.19f, 0.24f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.27f, 0.30f, 0.44f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.28f, 0.29f, 0.41f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.12f, 0.16f, 0.20f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.44f, 0.92f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.31f, 0.45f, 0.60f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.42f, 0.57f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.64f, 0.79f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.65f, 0.75f, 0.87f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.43f, 0.55f, 0.70f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}
	
	static LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void AddUnderLine(ImColor col_)
	{
		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();
		min.y = max.y;
		ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
	}

	void TextURL(const char* name_, const char* URL_, bool SameLineBefore_, bool SameLineAfter_)
	{
		if (SameLineBefore_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
		ImGui::Text(name_);
		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(0))
			{
				ShellExecute(0, 0, URL_, 0, 0, SW_SHOW);
			}
			AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			ImGui::SetTooltip("Open in browser: \n%s", URL_);
		}
		else
		{
			AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]);
		}
		if (SameLineAfter_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
	}

	void AddInputLocker(void* id)
	{
		if (_inputLockers.count(id) == 0)
			_inputLockers.insert(id);
	}

	void RemoveInputLocker(void* id)
	{
		if (_inputLockers.count(id) > 0)
			_inputLockers.erase(id);
	}

	void SetInputLock(void* this_,bool value)
	{
		if (value)
			AddInputLocker(this_);
		else
			RemoveInputLocker(this_);
	}

	bool IsInputLocked()
	{
		return _inputLockers.size() > 0;
	}

	static LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		ImGuiIO& io = ImGui::GetIO();
		POINT mPos;
		GetCursorPos(&mPos);
		ScreenToClient(hWnd, &mPos);
		ImGui::GetIO().MousePos.x = static_cast<float>(mPos.x);
		ImGui::GetIO().MousePos.y = static_cast<float>(mPos.y);

		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

		short key;
		bool keyUpEvent = true;
		switch (uMsg)
		{
		case WM_LBUTTONUP:
			key = VK_LBUTTON;
			break;
		case WM_RBUTTONUP:
			key = VK_RBUTTON;
			break;
		case WM_MBUTTONUP:
			key = VK_MBUTTON;
			break;
		case WM_XBUTTONUP:
			key = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2;
			break;
		case WM_KEYUP:
			key = static_cast<short>(wParam);
			break;
		default:
			keyUpEvent = false;
			break;
		}

		if (IsInputLocked())
			return true;

		return CallWindowProc(OriginalWndProcHandler, hWnd, uMsg, wParam, lParam);
	}

#if _WIN64
	typedef HRESULT(APIENTRY* IDXGISwapChainPresent12)(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags);
	typedef void(APIENTRY* DrawInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
	typedef void(APIENTRY* DrawIndexedInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
	typedef void(APIENTRY* ExecuteCommandLists)(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);
	IDXGISwapChainPresent12 presentFuncDX12;
	ExecuteCommandLists executeCommandListFuncDX12;
	struct DirectX12MainFunctions
	{
		IDXGISwapChainPresent12 presentFunc12;
		DrawInstanced drawInstancedFunc12;
		DrawIndexedInstanced drawIndexedInstancedFunc12;
		ExecuteCommandLists executeCommandListFunc12;
	};

	ID3D12Device* Device12 = nullptr;
	ID3D12DescriptorHeap* DescriptorHeapBackBuffers12;
	ID3D12DescriptorHeap* DescriptorHeapImGuiRender12;
	ID3D12GraphicsCommandList* CommandList12;
	ID3D12CommandQueue* CommandQueue12;

	struct _FrameContext {
		ID3D12CommandAllocator* CommandAllocator12;
		ID3D12Resource* Resource12;
		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle12;
	};

	UINT BuffersCounts12 = -1;
	_FrameContext* FrameContext12;
	void OnInitializeDX12(HWND window, ID3D12Device* pDevice, UINT buffersCounts, ID3D12DescriptorHeap* pDescriptorHeapImGuiRender);
	void OnPostRenderDX12(ID3D12GraphicsCommandList* commandList);
	void OnPreRenderDX12();
	static HRESULT __stdcall Present_Hook12(IDXGISwapChain3* pChain, const UINT SyncInterval, const UINT Flags)
	{
		static BOOL g_bInitialised = false;

		if (!g_bInitialised)
		{
			auto result = (HRESULT)pChain->GetDevice(__uuidof(Device12), reinterpret_cast<void**>(&Device12));

			if (SUCCEEDED(result))
			{
				DXGI_SWAP_CHAIN_DESC Desc;
				pChain->GetDesc(&Desc);
				Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
				Desc.Windowed = ((GetWindowLongPtr(Desc.OutputWindow, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

				BuffersCounts12 = Desc.BufferCount;
				FrameContext12 = new _FrameContext[BuffersCounts12];

				D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender = {};
				DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				DescriptorImGuiRender.NumDescriptors = BuffersCounts12;
				DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

				if (Device12->CreateDescriptorHeap(&DescriptorImGuiRender, IID_PPV_ARGS(&DescriptorHeapImGuiRender12)) != S_OK)
					return presentFuncDX12(pChain, SyncInterval, Flags);

				ID3D12CommandAllocator* Allocator;
				if (Device12->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator)) != S_OK)
					return presentFuncDX12(pChain, SyncInterval, Flags);

				for (size_t i = 0; i < BuffersCounts12; i++) {
					FrameContext12[i].CommandAllocator12 = Allocator;
				}

				if (Device12->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&CommandList12)) != S_OK ||
					CommandList12->Close() != S_OK)
					return presentFuncDX12(pChain, SyncInterval, Flags);

				D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
				DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				DescriptorBackBuffers.NumDescriptors = BuffersCounts12;
				DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				DescriptorBackBuffers.NodeMask = 1;

				if (Device12->CreateDescriptorHeap(&DescriptorBackBuffers, IID_PPV_ARGS(&DescriptorHeapBackBuffers12)) != S_OK)
					return presentFuncDX12(pChain, SyncInterval, Flags);

				const auto RTVDescriptorSize = Device12->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = DescriptorHeapBackBuffers12->GetCPUDescriptorHandleForHeapStart();

				for (size_t i = 0; i < BuffersCounts12; i++) {
					ID3D12Resource* pBackBuffer = nullptr;
					FrameContext12[i].DescriptorHandle12 = RTVHandle;
					pChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
					Device12->CreateRenderTargetView(pBackBuffer, nullptr, RTVHandle);
					FrameContext12[i].Resource12 = pBackBuffer;
					RTVHandle.ptr += RTVDescriptorSize;
				}

				OnInitializeDX12(Desc.OutputWindow, Device12, BuffersCounts12, DescriptorHeapImGuiRender12);

				g_bInitialised = true;
			}
		}

		// render function
		if (!g_bInitialised || CommandQueue12 == nullptr)
			return presentFuncDX12(pChain, SyncInterval, Flags);

		OnPreRenderDX12();

		_FrameContext& CurrentFrameContext = FrameContext12[pChain->GetCurrentBackBufferIndex()];
		CurrentFrameContext.CommandAllocator12->Reset();

		D3D12_RESOURCE_BARRIER Barrier;
		Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		Barrier.Transition.pResource = CurrentFrameContext.Resource12;
		Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		CommandList12->Reset(CurrentFrameContext.CommandAllocator12, nullptr);
		CommandList12->ResourceBarrier(1, &Barrier);
		CommandList12->OMSetRenderTargets(1, &CurrentFrameContext.DescriptorHandle12, FALSE, nullptr);
		CommandList12->SetDescriptorHeaps(1, &DescriptorHeapImGuiRender12);

		OnPostRenderDX12(CommandList12);

		Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		CommandList12->ResourceBarrier(1, &Barrier);
		CommandList12->Close();
		CommandQueue12->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&CommandList12));

		return presentFuncDX12(pChain, SyncInterval, Flags);
	}

	void ExecuteCommandLists_Hook(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists) {
		if (!CommandQueue12)
			CommandQueue12 = queue;
		executeCommandListFuncDX12(queue, NumCommandLists, ppCommandLists);
	}

	static void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter)
	{
		*ppAdapter = nullptr;
		for (UINT adapterIndex = 0; ; ++adapterIndex)
		{
			IDXGIAdapter1* pAdapter = nullptr;
			if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(adapterIndex, &pAdapter))
			{
				// No more adapters to enumerate.
				break;
			}

			// Check to see if the adapter supports Direct3D 12, but don't create the
			// actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				*ppAdapter = pAdapter;
				return;
			}
			pAdapter->Release();
		}
	}

	WNDCLASSEX WindowClass;
	HWND WindowHwnd;

	bool InitWindow() {

		WindowClass.cbSize = sizeof(WNDCLASSEX);
		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = DefWindowProc;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = GetModuleHandle(NULL);
		WindowClass.hIcon = NULL;
		WindowClass.hCursor = NULL;
		WindowClass.hbrBackground = NULL;
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = "MJ";
		WindowClass.hIconSm = NULL;
		RegisterClassEx(&WindowClass);
		WindowHwnd = CreateWindow(WindowClass.lpszClassName, "DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL);
		if (WindowHwnd == NULL) {
			return false;
		}
		return true;
	}

	bool DeleteWindow() {
		DestroyWindow(WindowHwnd);
		UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
		if (WindowHwnd != NULL) {
			return false;
		}
		return true;
	}

	bool FindFunctions(DirectX12MainFunctions& mainFunctions) {

		if (InitWindow() == false) {
			return false;
		}

		HMODULE D3D12Module = GetModuleHandle("d3d12.dll");
		HMODULE DXGIModule = GetModuleHandle("dxgi.dll");
		if (D3D12Module == NULL || DXGIModule == NULL) {
			DeleteWindow();
			return false;
		}

		void* CreateDXGIFactory = GetProcAddress(DXGIModule, "CreateDXGIFactory");
		if (CreateDXGIFactory == NULL) {
			DeleteWindow();
			return false;
		}

		IDXGIFactory* Factory;
		if (((long(__stdcall*)(const IID&, void**))(CreateDXGIFactory))(__uuidof(IDXGIFactory), (void**)&Factory) < 0) {
			DeleteWindow();
			return false;
		}

		IDXGIAdapter* Adapter;
		if (Factory->EnumAdapters(0, &Adapter) == DXGI_ERROR_NOT_FOUND) {
			DeleteWindow();
			return false;
		}

		void* D3D12CreateDevice = GetProcAddress(D3D12Module, "D3D12CreateDevice");
		if (D3D12CreateDevice == NULL) {
			DeleteWindow();
			return false;
		}

		ID3D12Device* Device;
		if (((long(__stdcall*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**))(D3D12CreateDevice))(Adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&Device) < 0) {
			DeleteWindow();
			return false;
		}

		D3D12_COMMAND_QUEUE_DESC QueueDesc;
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		QueueDesc.Priority = 0;
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		QueueDesc.NodeMask = 0;

		ID3D12CommandQueue* CommandQueue;
		if (Device->CreateCommandQueue(&QueueDesc, __uuidof(ID3D12CommandQueue), (void**)&CommandQueue) < 0) {
			DeleteWindow();
			return false;
		}

		ID3D12CommandAllocator* CommandAllocator;
		if (Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&CommandAllocator) < 0) {
			DeleteWindow();
			return false;
		}

		ID3D12GraphicsCommandList* CommandList;
		if (Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&CommandList) < 0) {
			DeleteWindow();
			return false;
		}

		DXGI_RATIONAL RefreshRate;
		RefreshRate.Numerator = 60;
		RefreshRate.Denominator = 1;

		DXGI_MODE_DESC BufferDesc;
		BufferDesc.Width = 100;
		BufferDesc.Height = 100;
		BufferDesc.RefreshRate = RefreshRate;
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC SampleDesc;
		SampleDesc.Count = 1;
		SampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		SwapChainDesc.BufferDesc = BufferDesc;
		SwapChainDesc.SampleDesc = SampleDesc;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.OutputWindow = WindowHwnd;
		SwapChainDesc.Windowed = 1;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* SwapChain;
		if (Factory->CreateSwapChain(CommandQueue, &SwapChainDesc, &SwapChain) < 0) {
			DeleteWindow();
			return false;
		}

		typedef uint64_t uintx_t;
		auto MethodsTable = (uintx_t*)::calloc(150, sizeof(uintx_t));
		memcpy(MethodsTable, *(uintx_t**)Device, 44 * sizeof(uintx_t));
		memcpy(MethodsTable + 44, *(uintx_t**)CommandQueue, 19 * sizeof(uintx_t));
		memcpy(MethodsTable + 44 + 19, *(uintx_t**)CommandAllocator, 9 * sizeof(uintx_t));
		memcpy(MethodsTable + 44 + 19 + 9, *(uintx_t**)CommandList, 60 * sizeof(uintx_t));
		memcpy(MethodsTable + 44 + 19 + 9 + 60, *(uintx_t**)SwapChain, 18 * sizeof(uintx_t));

		mainFunctions.presentFunc12 = reinterpret_cast<decltype(mainFunctions.presentFunc12)>(MethodsTable[140]);
		mainFunctions.drawInstancedFunc12 = reinterpret_cast<decltype(mainFunctions.drawInstancedFunc12)>(MethodsTable[84]);
		mainFunctions.drawIndexedInstancedFunc12 = reinterpret_cast<decltype(mainFunctions.drawIndexedInstancedFunc12)>(MethodsTable[85]);
		mainFunctions.executeCommandListFunc12 = reinterpret_cast<decltype(mainFunctions.executeCommandListFunc12)>(MethodsTable[54]);

		free(MethodsTable);

		Device->Release();
		Device = NULL;
		CommandQueue->Release();
		CommandQueue = NULL;
		CommandAllocator->Release();
		CommandAllocator = NULL;
		CommandList->Release();
		CommandList = NULL;
		SwapChain->Release();
		SwapChain = NULL;
		DeleteWindow();
		return true;
	}

	void OnPreRenderDX12()
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		(*p_)();

		ImGui::EndFrame();
	}

	void OnPostRenderDX12(ID3D12GraphicsCommandList* commandList)
	{
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
	}

	void OnInitializeDX12(HWND window, ID3D12Device* pDevice, UINT buffersCounts, ID3D12DescriptorHeap* pDescriptorHeapImGuiRender)
	{

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		_customFontData.data = getallfont();

		if (_customFontData.data == nullptr)
			return;

		for (int i = 0; i < _fontsCount; i++)
		{
			ImGuiIO& io = ImGui::GetIO();
			auto newFont = io.Fonts->AddFontFromMemoryTTF(_customFontData.data, _customFontData.size, static_cast<float>((i + 1) * _fontSizeStep), NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
			if (newFont == nullptr)
				return;

			_fonts[i] = newFont;
		}
		SetupImGuiStyle();

		//Set OriginalWndProcHandler to the Address of the Original WndProc function
		OriginalWndProcHandler = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(hWndProc)));

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX12_Init(pDevice, buffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM,
			pDescriptorHeapImGuiRender,
			pDescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
			pDescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());

		ImGui_ImplDX12_CreateDeviceObjects();
		ImGui::GetIO().ImeWindowHandle = window;

		io.SetPlatformImeDataFn = nullptr; // F**king bug take 4 hours of my life
	}

	void InitializeDX12Hooks()
	{
		LOG_OUT(L_DEBUG, "InitD3DHook...");
		DirectX12MainFunctions mainFuncs;
		if (!FindFunctions(mainFuncs))
		{
			Library::LOG_OUT(Library::L_ERROR, "Can't find Present");
			return;
		}
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)mainFuncs.presentFunc12, Present_Hook12);
		DetourTransactionCommit();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)mainFuncs.executeCommandListFunc12, ExecuteCommandLists_Hook);
		DetourTransactionCommit();
		LOG_OUT(L_DEBUG, "InitD3DHook Done!");
	}

#endif // _WIN64

	static void OnInitializeDX11(HWND window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pChain)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		//io.Fonts->AddFontFromFileTTF("c:/simheibd.ttf", 13.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

		
		_customFontData.data = getallfont();

		if (_customFontData.data == nullptr)
			return;

		for (int i = 0; i < _fontsCount; i++)
		{
			ImGuiIO& io = ImGui::GetIO();
			auto newFont = io.Fonts->AddFontFromMemoryTTF(_customFontData.data, _customFontData.size, static_cast<float>((i + 1) * _fontSizeStep),NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
			if (newFont == nullptr)
				return;

			_fonts[i] = newFont;
		}
		
		SetupImGuiStyle();
		//Set OriginalWndProcHandler to the Address of the Original WndProc function
		OriginalWndProcHandler = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(hWndProc)));

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(pDevice, pContext);

		ID3D11Texture2D* pBackBuffer;
		pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
		pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mainRenderTargetView);
		pBackBuffer->Release();

		io.SetPlatformImeDataFn = nullptr; // F**king bug take 4 hours of my life
	}
	static void OnRenderDX11(ID3D11DeviceContext* pContext)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::NewFrame();

		(p_)();//绘制函数指针

		ImGui::EndFrame();
		ImGui::Render();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	static HRESULT __stdcall Present_Hook(IDXGISwapChain* pChain, const UINT SyncInterval, const UINT Flags)
	{
		static BOOL g_bInitialised = false;

		// Main D3D11 Objects
		static ID3D11DeviceContext* pContext = nullptr;


		if (!g_bInitialised)
		{
			auto result = (HRESULT)pChain->GetDevice(__uuidof(pDevice), reinterpret_cast<void**>(&pDevice));

			if (SUCCEEDED(result))
			{
				pDevice->GetImmediateContext(&pContext);

				DXGI_SWAP_CHAIN_DESC sd;
				pChain->GetDesc(&sd);

				OnInitializeDX11(sd.OutputWindow, pDevice, pContext, pChain);

				g_bInitialised = true;
			}
		}

		// render function
		if (g_bInitialised)
			OnRenderDX11(pContext);

		return fnIDXGISwapChainPresent(pChain, SyncInterval, Flags);
	}
	static IDXGISwapChainPresent findDirect11Present()
	{
		WNDCLASSEX wc{ 0 };
		wc.cbSize = sizeof(wc);
		wc.lpfnWndProc = DefWindowProc;
		wc.lpszClassName = TEXT("Class");

		if (!RegisterClassEx(&wc))
		{
			return nullptr;
		}

		HWND hWnd = CreateWindow(wc.lpszClassName, TEXT(""), WS_DISABLED, 0, 0, 0, 0, NULL, NULL, NULL, nullptr);

		IDXGISwapChain* pSwapChain;

		D3D_FEATURE_LEVEL featureLevel;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = TRUE;  //((GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Main D3D11 Objects
		ID3D11DeviceContext* pContext = nullptr;
		ID3D11Device* pDevice = nullptr;
		
		if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, nullptr, 1, D3D11_SDK_VERSION,
			&swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pContext)) &&
			FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
				&swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pContext)))
		{
			DestroyWindow(swapChainDesc.OutputWindow);
			UnregisterClass(wc.lpszClassName, GetModuleHandle(nullptr));

			return nullptr;
		}

		const DWORD_PTR* pSwapChainVtable = reinterpret_cast<DWORD_PTR*>(pSwapChain);
		pSwapChainVtable = reinterpret_cast<DWORD_PTR*>(pSwapChainVtable[0]);

		auto swapChainPresent = reinterpret_cast<IDXGISwapChainPresent>(pSwapChainVtable[8]);

		pDevice->Release();
		pContext->Release();
		pSwapChain->Release();

		DestroyWindow(swapChainDesc.OutputWindow);
		UnregisterClass(wc.lpszClassName, GetModuleHandle(nullptr));

		return swapChainPresent;
	}
	void InitializeDX11Hooks()
	{
		LOG_OUT(L_DEBUG, "InitD3DHook...");
		fnIDXGISwapChainPresent = findDirect11Present();
		if (fnIDXGISwapChainPresent == nullptr)
		{
			Library::LOG_OUT(Library::L_ERROR, "Can't find Present");
			return;
		}
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)fnIDXGISwapChainPresent, Present_Hook);
		DetourTransactionCommit();
		LOG_OUT(L_DEBUG, "InitD3DHook Done!");
	}
	void InitializeDX9Hooks()
	{

	}
	void InitializeDX10Hooks()
	{

	}



	void InitD3DHook(DXVersion version,void (*p)())
	{
		p_ = p;
		string Temp_ = "Select DXVersion：";
		Temp_ += to_string((int)version);
		LOG_OUT(L_DEBUG, Temp_.c_str());
		switch (version)
		{
		case DXVersion::D3D11:
			CreateThreads(&InitializeDX11Hooks);
			break;
		case DXVersion::D3D9:
			CreateThreads(&InitializeDX9Hooks);//未完成
			break;
		case DXVersion::D3D10:
			CreateThreads(&InitializeDX10Hooks);//未完成
			break;
		case DXVersion::D3D12:  //你应该知道IMGUI DX12默认只能编译成x64程序
#if _WIN32
			LOG_OUT(L_ERROR, "DX12Hook Only supported x64");
#else
			CreateThreads(&InitializeDX12Hooks);
#endif // _WIN64

			break;
		default:
			Library::LOG_OUT(Library::L_ERROR, "Unsupported DX");
		}
	}

}

