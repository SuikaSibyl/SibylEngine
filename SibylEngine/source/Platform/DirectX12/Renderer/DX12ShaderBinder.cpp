#include "SIByLpch.h"
#include "DX12ShaderBinder.h"

#include "Platform/DirectX12/Common/DX12Utility.h"
#include "Platform/DirectX12/Common/DX12Context.h"
#include "Platform/DirectX12/Core/UploadBuffer.h"

namespace SIByL
{
	DX12ShaderBinder::DX12ShaderBinder(const ShaderBinderDesc& desc)
	{
		m_Desc = desc;
		BuildRootSignature();
		m_SrvDynamicDescriptorHeap = std::make_shared<DynamicDescriptorHeap>(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_SamplerDynamicDescriptorHeap = std::make_shared<DynamicDescriptorHeap>(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

		m_SrvDynamicDescriptorHeap->ParseRootSignature(*m_RootSignature);
	}

	void DX12ShaderBinder::Bind()
	{
		// Bind Root Signature
		ID3D12GraphicsCommandList* cmdList = DX12Context::GetDXGraphicCommandList();
		cmdList->SetGraphicsRootSignature(GetRootSignature());

		//// Bind Descriptor Table
		//int objCbvIndex = 0;
		//auto handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(cbvHeap->GetGPUDescriptorHandleForHeapStart());
		//handle.Offset(objCbvIndex, cbv_srv_uavDescriptorSize);
		//cmdList->SetGraphicsRootDescriptorTable(0, //根参数的起始索引
		//	handle);
	}

	void DX12ShaderBinder::BuildRootSignature()
	{
		// Perfomance TIP: Order from most frequent to least frequent.
		// ----------------------------------------------------------------------
		int parameterCount = m_Desc.m_ConstantBufferLayouts.size()
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
		for (ShaderResourceLayout& srLayout : m_Desc.m_TextureBufferLayouts)
		{
			CD3DX12_DESCRIPTOR_RANGE srvTable;
			srvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
				srLayout.SrvCount(),
				indexSrvTable);

			slotRootParameter.get()[indexPara].InitAsDescriptorTable(1,
				&srvTable,
				D3D12_SHADER_VISIBILITY_PIXEL);	// Only Readable in pixel shader

			indexPara++; indexSrvTable;
		}

		auto staticSamplers = DX12Context::GetStaticSamplers();	//获得静态采样器集合
		//slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);
		// Root Signature is consisted of a set of root parameters
		CD3DX12_ROOT_SIGNATURE_DESC rootSig(parameterCount, // Number of root parameters
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
	}

}