#pragma once

#include "Sibyl/Graphic/AbstractAPI/Core/Middle/Texture.h"

namespace SIByL
{
	struct FrameBufferDesc
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Channel;
		Texture2D::Format Format;
		bool SwapChainTarget = false;
	};

	class PtrCudaTexture;
	class PtrCudaSurface;
	class FrameBuffer
	{
	public:
		static Ref<FrameBuffer> Create(const FrameBufferDesc& desc, const std::string& key);
		virtual ~FrameBuffer() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void ClearBuffer() = 0;
		virtual void ClearRgba() = 0;
		virtual void ClearDepthStencil() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void* GetColorAttachment() = 0;

		// Caster
		// -------------------------------------------------
		virtual Ref<Texture2D> ColorAsTexutre() = 0;
		virtual Ref<Texture2D> DepthStencilAsTexutre() = 0;

		// Fetcher
		// -------------------------------------------------
		virtual const FrameBufferDesc& GetDesc() const = 0;

		////////////////////////////////////////////////////
		//					CUDA Interface				  //
		////////////////////////////////////////////////////
	public:
		virtual Ref<PtrCudaTexture> GetPtrCudaTexture() = 0;
		virtual Ref<PtrCudaSurface> GetPtrCudaSurface() = 0;
		virtual void ResizePtrCudaTexuture() = 0;
		virtual void ResizePtrCudaSurface() = 0;

	protected:
		virtual void CreatePtrCudaTexutre() = 0;
		virtual void CreatePtrCudaSurface() = 0;

		Ref<PtrCudaTexture> mPtrCudaTexture = nullptr;
		Ref<PtrCudaSurface> mPtrCudaSurface = nullptr;
	};
}