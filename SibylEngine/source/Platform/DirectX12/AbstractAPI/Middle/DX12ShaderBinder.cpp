#include "SIByLpch.h"
#include "DX12ShaderBinder.h"

#include "Platform/DirectX12/Common/DX12Utility.h"
#include "Platform/DirectX12/Common/DX12Context.h"
#include "Platform/DirectX12/AbstractAPI/Bottom/DX12UploadBuffer.h"
#include "Platform/DirectX12/AbstractAPI/Bottom/DX12RootSignature.h"
#include "Platform/DirectX12/AbstractAPI/Middle/DX12Texture.h"

namespace SIByL
{
	///////////////////////////////////////////////////////////////////////////////
	//							DX12ShaderConstantsBuffer						///
	///////////////////////////////////////////////////////////////////////////////
	DX12ShaderConstantsBuffer::DX12ShaderConstantsBuffer(ShaderConstantsDesc* desc)
	{
		m_ConstantsTableBuffer = std::make_shared<DX12FrameResourceBuffer>(desc->Size);
		m_ConstantsMapper = &desc->Mapper;

		InitConstant();
	}

	void DX12ShaderConstantsBuffer::SetInt(const std::string& name, const int& value)
	{
		m_IsDirty = true;
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			m_ConstantsTableBuffer->CopyMemoryToConstantsBuffer((void*)&value, item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderConstantsBuffer::SetFloat(const std::string& name, const float& value)
	{
		m_IsDirty = true;
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			m_ConstantsTableBuffer->CopyMemoryToConstantsBuffer((void*)&value, item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderConstantsBuffer::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		m_IsDirty = true;
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			m_ConstantsTableBuffer->CopyMemoryToConstantsBuffer((void*)&value[0], item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderConstantsBuffer::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		m_IsDirty = true;
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			m_ConstantsTableBuffer->CopyMemoryToConstantsBuffer((void*)&value[0], item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderConstantsBuffer::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		m_IsDirty = true;
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			m_ConstantsTableBuffer->CopyMemoryToConstantsBuffer((void*)&value[0], item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderConstantsBuffer::SetMatrix4x4(const std::string& name, const glm::mat4& value)
	{
		m_IsDirty = true;
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			m_ConstantsTableBuffer->CopyMemoryToConstantsBuffer((void*)&value[0][0], item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderConstantsBuffer::GetInt(const std::string& name, int& value)
	{
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			m_ConstantsTableBuffer->CopyMemoryFromConstantsBuffer((void*)&value, item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderConstantsBuffer::GetFloat(const std::string& name, float& value)
	{
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			m_ConstantsTableBuffer->CopyMemoryFromConstantsBuffer((void*)&value, item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderConstantsBuffer::GetFloat3(const std::string& name, glm::vec3& value)
	{
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			m_ConstantsTableBuffer->CopyMemoryFromConstantsBuffer((void*)&value[0], item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderConstantsBuffer::GetFloat4(const std::string& name, glm::vec4& value)
	{
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			m_ConstantsTableBuffer->CopyMemoryFromConstantsBuffer((void*)&value[0], item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	void DX12ShaderConstantsBuffer::GetMatrix4x4(const std::string& name, glm::mat4& value)
	{
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			m_ConstantsTableBuffer->CopyMemoryToConstantsBuffer((void*)&value[0][0], item.Offset, ShaderDataTypeSize(item.Type));
		}
	}

	float* DX12ShaderConstantsBuffer::PtrFloat(const std::string& name)
	{
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			return (float*)m_ConstantsTableBuffer->GetPtrFromConstantsBuffer(item.Offset, ShaderDataTypeSize(item.Type));
		}
		return nullptr;
	}

	float* DX12ShaderConstantsBuffer::PtrFloat3(const std::string& name)
	{
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			return (float*)m_ConstantsTableBuffer->GetPtrFromConstantsBuffer(item.Offset, ShaderDataTypeSize(item.Type));
		}
		return nullptr;
	}

	float* DX12ShaderConstantsBuffer::PtrFloat4(const std::string& name)
	{
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			return (float*)m_ConstantsTableBuffer->GetPtrFromConstantsBuffer(item.Offset, ShaderDataTypeSize(item.Type));
		}
		return nullptr;
	}

	float* DX12ShaderConstantsBuffer::PtrMatrix4x4(const std::string& name)
	{
		ShaderConstantItem item;
		if (m_ConstantsMapper->FetchConstant(name, item))
		{
			return (float*)m_ConstantsTableBuffer->GetPtrFromConstantsBuffer(item.Offset, ShaderDataTypeSize(item.Type));
		}
		return nullptr;
	}

	void DX12ShaderConstantsBuffer::UploadDataIfDirty(ShaderBinder* shaderBinder)
	{
		if (m_IsDirty)
		{
			m_ConstantsTableBuffer->UploadCurrentBuffer();
			m_IsDirty = false;
		}
	}
	
	void DX12ShaderConstantsBuffer::SetDirty()
	{
		m_IsDirty = true;
	}

	void DX12ShaderConstantsBuffer::InitConstant()
	{
		for each (auto & constant in *m_ConstantsMapper)
		{
			switch (constant.second.Type)
			{
			case ShaderDataType::RGBA:
				SetFloat4(constant.first, { 0, 0, 0, 1 });
				break;
			case ShaderDataType::Float4:
				SetFloat4(constant.first, { 0, 0, 0, 0 });
				break;

			default:
				break;
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////
	//							DX12ShaderConstantsBuffer						///
	///////////////////////////////////////////////////////////////////////////////
	DX12ShaderResourcesBuffer::DX12ShaderResourcesBuffer(ShaderResourcesDesc* desc, RootSignature* rootsignature)
	{
		m_ShaderResourcesDesc = *desc;

		// Init Shader Resource Buffer
		m_SrvDynamicDescriptorHeap = std::make_shared<DX12DynamicDescriptorHeap>(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_SamplerDynamicDescriptorHeap = std::make_shared<DX12DynamicDescriptorHeap>(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

		DX12RootSignature* rs = dynamic_cast<DX12RootSignature*>(rootsignature);
		m_SrvDynamicDescriptorHeap->ParseRootSignature(*rs);
	}

	void DX12ShaderResourcesBuffer::SetTexture2D(const std::string& name, Ref<Texture2D> texture)
	{
		m_IsDirty = true;

		ShaderResourceItem item;
		if (m_ShaderResourcesDesc.Mapper.FetchResource(name, item))
		{
			DX12Texture2D* dxTexture = dynamic_cast<DX12Texture2D*>(texture.get());
			m_SrvDynamicDescriptorHeap->StageDescriptors(item.SRTIndex, item.Offset, 1, dxTexture->GetSRVHandle());
			m_ShaderResourcesDesc.Mapper.SetTextureID(name, texture->Identifer);
		}
	}

	void DX12ShaderResourcesBuffer::SetTextureCubemap(const std::string& name, Ref<TextureCubemap> texture)
	{

	}

	ShaderResourcesDesc* DX12ShaderResourcesBuffer::GetShaderResourceDesc()
	{
		return &m_ShaderResourcesDesc;
	}

	void DX12ShaderResourcesBuffer::UploadDataIfDirty(ShaderBinder* shaderBinder)
	{
		if (m_IsDirty)
		{
			m_SrvDynamicDescriptorHeap->CommitStagedDescriptorsForDraw();
			m_IsDirty = false;
		}
		else
		{
			m_SrvDynamicDescriptorHeap->SetAsShaderResourceHeap();
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	//								DX12ShaderBinder							///
	///////////////////////////////////////////////////////////////////////////////
	void DX12ShaderBinder::BindConstantsBuffer(unsigned int slot, ShaderConstantsBuffer& buffer)
	{
		DX12ShaderConstantsBuffer& dxBuffer = dynamic_cast<DX12ShaderConstantsBuffer&>(buffer);
		Ref<DX12FrameResourceBuffer> frameBuffer = dxBuffer.m_ConstantsTableBuffer;
		D3D12_GPU_VIRTUAL_ADDRESS gpuAddr = frameBuffer->GetCurrentGPUAddress();

		ID3D12GraphicsCommandList* cmdList = DX12Context::GetInFlightDXGraphicCommandList();
		cmdList->SetGraphicsRootConstantBufferView(slot, gpuAddr);
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

	DX12ShaderBinder::~DX12ShaderBinder()
	{

	}

	void DX12ShaderBinder::Bind()
	{
		PROFILE_SCOPE_FUNCTION();

		// Bind Root Signature
		ID3D12GraphicsCommandList* cmdList = DX12Context::GetInFlightDXGraphicCommandList();
		cmdList->SetGraphicsRootSignature(GetDXRootSignature());
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

		auto staticSamplers = DX12Context::GetStaticSamplers();	//??????????????????
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

		m_RootSignature = std::make_shared<DX12RootSignature>(rootSig);
		delete[] srvTable;
	}
}