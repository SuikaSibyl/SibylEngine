#include "SIByLpch.h"
#include "DX12FrameResources.h"

#include "Platform/DirectX12/Common/DX12Utility.h"
#include "Platform/DirectX12/Common/DX12Context.h"

namespace SIByL
{
	DX12FrameResourcesManager* DX12FrameResourcesManager::m_Instance;
	int DX12FrameResourcesManager::m_FrameResourcesCount;
	int DX12FrameResourcesManager::m_CurrentFrameIndex;
	UINT64* DX12FrameResourcesManager::m_Fence;
	ComPtr<ID3D12CommandAllocator>* DX12FrameResourcesManager::m_CommandAllocators;

	DX12FrameResourcesManager::
		DX12FrameResourcesManager(int frameResourcesCount)
	{
		SIByL_CORE_ASSERT(!m_Instance, "DX12FrameResourcesManager Instance Redefinition");
		m_Instance = this;
		m_FrameResourcesCount = frameResourcesCount;
		// Create Upload Buffers for All Frame Resources
		m_UploadBuffers = new Ref<DX12UploadBuffer>[frameResourcesCount];
		m_Fence = new UINT64[frameResourcesCount];
		m_CommandAllocators = new ComPtr<ID3D12CommandAllocator>[frameResourcesCount];
		for (int i = 0; i < frameResourcesCount; i++)
		{
			m_UploadBuffers[i].reset(new DX12UploadBuffer);
			m_Fence[i] = 0;
			DXCall(DX12Context::GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocators[i])));
		}
	}

	void DX12FrameResourcesManager::SetCurrentFence(UINT64 cpuFence)
	{
		m_Fence[m_CurrentFrameIndex] = cpuFence;
	}

	DX12FrameResourcesManager* DX12FrameResourcesManager::Get()
	{
		return m_Instance;
	}

	DX12FrameResourcesManager::~DX12FrameResourcesManager()
	{
		for (int i = 0; i < m_FrameResourcesCount; i++)
		{
			m_UploadBuffers[i] = nullptr;
			m_CommandAllocators[i] = nullptr;
			m_Fence[i] = 0;
		}
		delete[] m_UploadBuffers;
		delete[] m_Fence;
		delete[] m_CommandAllocators;
	}

	DX12FrameResourceBuffer::DX12FrameResourceBuffer(uint32_t size)
		:m_SizeByte(size)
	{
		DX12FrameResourcesManager::Get()->RegistFrameResource(this);
	}

	void* DX12FrameResourceBuffer::GetCurrentBuffer() 
	{ 
		return m_CpuBuffer; 
	}

	void DX12FrameResourceBuffer::UploadCurrentBuffer() 
	{
		return UploadBufferToGpu(DX12FrameResourcesManager::GetCurrentIndex()); 
	}

	D3D12_GPU_VIRTUAL_ADDRESS DX12FrameResourceBuffer::GetCurrentGPUAddress()
	{
		return m_GpuBuffers.get()[DX12FrameResourcesManager::GetCurrentIndex()].GPU;
	}
}