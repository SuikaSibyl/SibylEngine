module;
#include <vulkan/vulkan.h>
module RHI.IRenderPass.VK;
import Core.Log;
import RHI.IRenderPass;
import RHI.ILogicalDevice.VK;

namespace SIByL
{
	namespace RHI
	{
		auto createSubpass() noexcept -> void
		{
			// Subpasses and attachment references
			// Every subpass references one or more of the attachments
			VkAttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			// The subpass is described using a VkSubpassDescription structure :
			VkSubpassDescription subpass{};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;
			// The index of the attachment in this array is directly referenced from the fragment shader with the layout(location = 0) out vec4 outColor directive!
			// - pInputAttachments: Attachments that are read from a shader
			// - pResolveAttachments: Attachments used for multisampling color attachments
			// - pDepthStencilAttachment : Attachment for depthand stencil data
			// - pPreserveAttachments : Attachments that are not used by this subpass, but for which the data must be preserved

		}

		auto createVKRenderPass(
			VkAttachmentDescription* attachments, 
			VkSubpassDescription* subpasses, 
			uint32_t const& subpass_num, 
			ILogicalDeviceVK* device
		) noexcept -> void
		{
			VkRenderPass renderPass;
			VkRenderPassCreateInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = attachments;
			renderPassInfo.subpassCount = subpass_num;
			renderPassInfo.pSubpasses = subpasses;

			if (vkCreateRenderPass(device->getDeviceHandle(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
				SE_CORE_ERROR("VULKAN :: failed to create render pass!");
			}
		}

		IRenderPassVK::~IRenderPassVK()
		{
			if (renderPass)
				vkDestroyRenderPass(logicalDevice->getDeviceHandle(), renderPass, nullptr);
		}

		auto IRenderPassVK::createRenderPass() noexcept -> void
		{
			VkAttachmentDescription colorAttachment{};
			//colorAttachment.format = swapChainImageFormat;
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

			// determine what to do with the data in the attachment before rendering
			// - VK_ATTACHMENT_LOAD_OP_LOAD: Preserve the existing contents of the attachment
			// - VK_ATTACHMENT_LOAD_OP_CLEAR : Clear the values to a constant at the start
			// - VK_ATTACHMENT_LOAD_OP_DONT_CARE : Existing contents are undefined; we don't care about them
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

			// determine what to do with the data in the attachment after rendering
			// - VK_ATTACHMENT_STORE_OP_STORE: Rendered contents will be stored in memoryand can be read later
			// - VK_ATTACHMENT_STORE_OP_DONT_CARE : Contents of the framebuffer will be undefined after the rendering operation
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

			// apply to stencil data
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

			// the layout of the pixels in memory
			// - VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: Images used as color attachment
			// - VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : Images to be presented in the swap chain
			// - VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL : Images to be used as destination for a memory copy operation

			// The initialLayout specifies which layout the image will have before the render pass begins
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			// The finalLayout specifies the layout to automatically transition to when the render pass finishes
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		}

		auto IRenderPassVK::getRenderPass() noexcept -> VkRenderPass*
		{
			return &renderPass;
		}
	}
}
