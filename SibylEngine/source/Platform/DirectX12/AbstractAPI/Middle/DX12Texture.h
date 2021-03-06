#pragma once

#include "Sibyl/Graphic/AbstractAPI/Core/Middle/Texture.h"
#include "Platform/DirectX12/AbstractAPI/Bottom/DX12DescriptorAllocation.h"
#include "Platform/DirectX12/ImGui/ImGuiLayerDX12.h"

namespace SIByL
{
	class Image;
	class DX12Texture2D :public Texture2D
	{
	public:
		DX12Texture2D(const std::string& path);
		DX12Texture2D(Ref<Image> image);
		void InitFromImage(Image* img);
		virtual ~DX12Texture2D();

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual void Bind(uint32_t slot) const override;

		virtual void RegisterImGui() override;
		virtual void* GetImGuiHandle() override;

		D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle();

	private:
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_Channel;
		Format	 m_Type;

		uint32_t m_ID;
		std::string m_Path;

		ComPtr<ID3D12Resource> m_Resource;
		ComPtr<ID3D12Resource> m_Uploader;
		DX12DescriptorAllocation m_DescriptorAllocation;
		ImGuiLayerDX12::ImGuiAllocation m_ImGuiAllocation;


		////////////////////////////////////////////////////
		//					CUDA Interface				  //
		////////////////////////////////////////////////////
	public:
		virtual Ref<PtrCudaTexture> GetPtrCudaTexture() override;
		virtual Ref<PtrCudaSurface> GetPtrCudaSurface() override;
		virtual void ResizePtrCudaTexuture() override;
		virtual void ResizePtrCudaSurface() override;

	protected:
		virtual void CreatePtrCudaTexutre() override;
		virtual void CreatePtrCudaSurface() override;
	};
}