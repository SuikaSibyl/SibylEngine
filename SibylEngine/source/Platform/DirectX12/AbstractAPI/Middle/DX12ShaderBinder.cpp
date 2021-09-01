#include "SIByLpch.h"
#include "DX12ShaderBinder.h"

#include "Platform/DirectX12/Common/DX12Utility.h"
#include "Platform/DirectX12/Common/DX12Context.h"
#include "Platform/DirectX12/AbstractAPI/Bottom/DX12UploadBuffer.h"
#include "Platform/DirectX12/AbstractAPI/Middle/DX12Texture.h"

namespace SIByL
{
	void DX12ShaderBinder::SetFloat(const std::string& name, const float& value)
	{
		PROFILE_SCOPE_FUNCTION();

		ShaderConstantItem item;
		if (m_ConstantsMapper.FetchConstant(name, item))
		{
			CopyMemoryToConstantsBuffer((void*)&value, item.CBIndex, item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderBinder::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		PROFILE_SCOPE_FUNCTION();

		ShaderConstantItem item;
		if (m_ConstantsMapper.FetchConstant(name, item))
		{
			CopyMemoryToConstantsBuffer((void*)&value[0], item.CBIndex, item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderBinder::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		PROFILE_SCOPE_FUNCTION();

		ShaderConstantItem item;
		if (m_ConstantsMapper.FetchConstant(name, item))
		{
			CopyMemoryToConstantsBuffer((void*)&value[0], item.CBIndex, item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderBinder::SetMatrix4x4(const std::string& name, const glm::mat4& value)
	{
		PROFILE_SCOPE_FUNCTION();

		ShaderConstantItem item;
		if (m_ConstantsMapper.FetchConstant(name, item))
		{
			CopyMemoryToConstantsBuffer((void*)&value[0][0], item.CBIndex, item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderBinder::SetTexture2D(const std::string& name, Ref<Texture2D> texture)
	{
		PROFILE_SCOPE_FUNCTION();

		ShaderResourceItem item;
		if (m_ResourcesMapper.FetchResource(name, item))
		{
			Ref<DX12DynamicDescriptorHeap> sddh = GetSrvDynamicDescriptorHeap();
			DX12Texture2D* dxTexture = dynamic_cast<DX12Texture2D*>(texture.get());
			sddh->StageDescriptors(item.SRTIndex, item.Offset, 1, dxTexture->GetSRVHandle());
		}
	}

	DX12ShaderBinder::DX12ShaderBinder(const ShaderBinderDesc& desc)
	{
		PROFILE_SCOPE_FUNCTION();

		m_Desc = desc;
		InitMappers(desc);
		BuildRootSignature();

		// Init Shader Resource Buffer
		m_SrvDynamicDescriptorHeap = std::make_shared<DX12DynamicDescriptorHeap>(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_SamplerDynamicDescriptorHeap = std::make_shared<DX12DynamicDescriptorHeap>(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
		m_SrvDynamicDescriptorHeap->ParseRootSignature(*m_RootSignature);

		// Init Constant Buffer
		m_ConstantsTableBuffer = new Ref<DX12FrameResourceBuffer>[desc.ConstantBufferCount()];
		for (int i = 0; i < desc.ConstantBufferCount(); i++)
		{
			ConstantBufferLayout& cbLayout = m_Desc.m_ConstantBufferLayouts[i];
			m_ConstantsTableBuffer[i] = std::make_shared<DX12FrameResourceBuffer>(cbLayout.GetStide());
		}
	}

	DX12ShaderBinder::~DX12ShaderBinder()
	{
		PROFILE_SCOPE_FUNCTION();

		for (int i = 0; i < m_Desc.ConstantBufferCount(); i++)
		{
			m_ConstantsTableBuffer[i] = nullptr;
		}
		delete[] m_ConstantsTableBuffer;
	}

	void DX12ShaderBinder::CopyMemoryToConstantsBuffer
		(void* data, int index, uint32_t offset, uint32_t length)
	{
		PROFILE_SCOPE_FUNCTION();

		Ref<DX12FrameResourceBuffer> buffer = m_ConstantsTableBuffer[index];
		buffer->CopyMemoryToConstantsBuffer(data, offset, length);
	}

	void DX12ShaderBinder::UpdateConstantsBuffer(int index)
	{
		PROFILE_SCOPE_FUNCTION();

		Ref<DX12FrameResourceBuffer> buffer = m_ConstantsTableBuffer[index];
		buffer->UploadCurrentBuffer();
	}
	
	void DX12ShaderBinder::BindConstantsBuffer(int index)
	{
		PROFILE_SCOPE_FUNCTION();

		Ref<DX12FrameResourceBuffer> buffer = m_ConstantsTableBuffer[index];
		D3D12_GPU_VIRTUAL_ADDRESS gpuAddr = buffer->GetCurrentGPUAddress();

		ID3D12GraphicsCommandList* cmdList = DX12Context::GetDXGraphicCommandList();
		cmdList->SetGraphicsRootConstantBufferView(index, gpuAddr);
	}

	void DX12ShaderBinder::Bind()
	{
		PROFILE_SCOPE_FUNCTION();

		// Bind Root Signature
		ID3D12GraphicsCommandList* cmdList = DX12Context::GetDXGraphicCommandList();
		cmdList->SetGraphicsRootSignature(GetRootSignature());

		//// Bind Descriptor Table
		//int objCbvIndex = 0;
		//auto handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(cbvHeap->GetGPUDescriptorHandleForHeapStart());
		//handle.Offset(objCbvIndex, cbv_srv_uavDescriptorSize);
		//cmdList->SetGraphicsRootDescriptorTable(0, //����������ʼ����
		//	handle);
	}

	void DX12ShaderBinder::BuildRootSignature()
	{
		PROFILE_SCOPE_FUNCTION();

		// Perfomance TIP: Order from most frequent to least frequent.
		// ----------------------------------------------------------------------
		size_t parameterCount = m_Desc.m_ConstantBufferLayouts.size()
			+ m_Desc.m_TextureBufferLayouts.size();

		// RootSignature could be descriptor table \ Root Descriptor \ Root Constant
		Ref<CD3DX12_ROOT_PARAMETER> slotRootParameter;
		slotRootParameter.reset(new CD3DX12_ROOT_PARAMETER[parameterCount]);
		int indexPara = 0;

		// Create a descriptor table of one sigle CBV
		for (ConstantBufferLayout& buffer : m_Desc.m_ConstantBufferLayouts)
		{
			slotRootParameter.get()[indexPara].InitAsConstantBufferView(indexPara);
			indexPara++;
		}

		// Create srv tables for srv...
		int indexSrvTable = 0;
		CD3DX12_DESCRIPTOR_RANGE* srvTable = new CD3DX12_DESCRIPTOR_RANGE[m_Desc.TextureBufferCount()]{};
		for (ShaderResourceLayout& srLayout : m_Desc.m_TextureBufferLayouts)
		{
			srvTable[indexSrvTable].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
				(UINT)srLayout.SrvCount(),
				indexSrvTable);

			slotRootParameter.get()[indexPara].InitAsDescriptorTable(1,
				&srvTable[indexSrvTable],
				D3D12_SHADER_VISIBILITY_PIXEL);	// Only Readable in pixel shader

			indexPara++; indexSrvTable++;
		}

		auto staticSamplers = DX12Context::GetStaticSamplers();	//��þ�̬����������
		//slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);
		// Root Signature is consisted of a set of root parameters
		CD3DX12_ROOT_SIGNATURE_DESC rootSig((UINT)parameterCount, // Number of root parameters
			slotRootParameter.get(), // Pointer to Root Parameter
			staticSamplers.size(),
			staticSamplers.data(),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		// Create a root signature using a single register
		// The slot is pointing to a descriptor area where is only on econst buffer
		ComPtr<ID3DBlob> serializedRootSig = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		HRESULT hr = D3D12SerializeRootSignature(&rootSig, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &errorBlob);

		if (errorBlob != nullptr)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		DXCall(hr);

		m_RootSignature = std::make_shared<RootSignature>(rootSig);
		delete[] srvTable;
	}
}