#include "SIByLpch.h"
#include "ImGuiLayerDX12.h"

#include "Platform/Windows/Window/WindowsWindow.h"
#include "Platform/Windows/ImGui/ImGuiWin32Renderer.h"
#include "Platform/DirectX12/ImGui/ImGuiDX12Renderer.h"

namespace SIByL
{
	ImGuiLayerDX12* ImGuiLayerDX12::m_Instance;

	ImGuiLayerDX12::ImGuiLayerDX12()
	{
		m_Instance = this;
	}

	void ImGuiLayerDX12::OnDrawAdditionalWindowsImpl()
	{
		ImGuiIO& io = ImGui::GetIO();
		ID3D12GraphicsCommandList* g_pd3dCommandList = DX12Context::Main->GetInFlightDXGraphicCommandList();
		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault(NULL, (void*)g_pd3dCommandList);
		}
	}

	static int const NUM_FRAMES_IN_FLIGHT = 3;

	void ImGuiLayerDX12::PlatformInit()
	{
		g_pd3dSrvDescHeap = DX12Context::Main->CreateSRVHeap();
		ImGui_ImplWin32_Init(*(WindowsWindow::Main->GetHWND()));
		ImGui_ImplDX12_Init(DX12Context::Main->GetDevice(), NUM_FRAMES_IN_FLIGHT,
			DXGI_FORMAT_R8G8B8A8_UNORM, g_pd3dSrvDescHeap.Get(),
			g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
			g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart());
	}

	void ImGuiLayerDX12::NewFrameBegin()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
	}

	void ImGuiLayerDX12::DrawCall()
	{
		ID3D12GraphicsCommandList* g_pd3dCommandList = DX12Context::GetInFlightDXGraphicCommandList();
		ID3D12DescriptorHeap* heap = g_pd3dSrvDescHeap.Get();
		g_pd3dCommandList->SetDescriptorHeaps(1, &heap);
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_pd3dCommandList);
	}

	ImGuiLayerDX12::~ImGuiLayerDX12()
	{
		PlatformDestroy();
	}

	void ImGuiLayerDX12::PlatformDestroy()
	{
		DX12Synchronizer* synchronizer = DX12Context::GetSynchronizer();
		synchronizer->ForceSynchronize();
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	ImGuiLayerDX12::ImGuiAllocation ImGuiLayerDX12::RegistShaderResource()
	{
		ImGuiLayerDX12::ImGuiAllocation allocation;
		D3D12_CPU_DESCRIPTOR_HANDLE my_texture_srv_cpu_handle = g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart();
		my_texture_srv_cpu_handle.ptr += (DX12Context::GetCbvSrvUavDescriptorSize() * m_HeapIndex);
		D3D12_GPU_DESCRIPTOR_HANDLE my_texture_srv_gpu_handle = g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart();
		my_texture_srv_gpu_handle.ptr += (DX12Context::GetCbvSrvUavDescriptorSize() * m_HeapIndex);
		allocation.m_CpuHandle = my_texture_srv_cpu_handle;
		allocation.m_GpuHandle = my_texture_srv_gpu_handle;
		m_HeapIndex++;

		return allocation;
	}
}