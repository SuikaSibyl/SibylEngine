module;
#include <cstdint>
#include <vulkan/vulkan.h>
module RHI.IEnum.VK;
import RHI.IEnum;
import Core.Log;

namespace SIByL::RHI
{
	inline auto getVkTopology(TopologyKind type) noexcept -> VkPrimitiveTopology
	{
		switch (type)
		{
		case SIByL::RHI::TopologyKind::TriangleStrip:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			break;
		case SIByL::RHI::TopologyKind::TriangleList:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			break;
		case SIByL::RHI::TopologyKind::LineStrip:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			break;
		case SIByL::RHI::TopologyKind::LineList:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			break;
		case SIByL::RHI::TopologyKind::PointList:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			break;
		default:
			break;
		}

#ifdef _DEBUG
		SE_CORE_ERROR("VULKAN :: Wrong Topology Kind");
#endif // _DEBUG

		return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	}

	inline auto getVkShaderStage(ShaderStage stage) noexcept -> VkShaderStageFlagBits
	{
		switch (stage)
		{
		case SIByL::RHI::ShaderStage::MESH:
			return VK_SHADER_STAGE_MESH_BIT_NV;
			break;
		case SIByL::RHI::ShaderStage::COMPUTE:
			return VK_SHADER_STAGE_COMPUTE_BIT;
			break;
		case SIByL::RHI::ShaderStage::TESSELLATION:
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			break;
		case SIByL::RHI::ShaderStage::GEOMETRY:
			return VK_SHADER_STAGE_GEOMETRY_BIT;
			break;
		case SIByL::RHI::ShaderStage::FRAGMENT:
			return VK_SHADER_STAGE_FRAGMENT_BIT;
			break;
		case SIByL::RHI::ShaderStage::VERTEX:
			return VK_SHADER_STAGE_VERTEX_BIT;
			break;
		default:
			break;
		}
#ifdef _DEBUG
		SE_CORE_ERROR("VULKAN :: Wrong Shader Stage");
#endif // _DEBUG'
		return VK_SHADER_STAGE_ALL;
	}

	inline auto getVkPolygonMode(PolygonMode mode) noexcept -> VkPolygonMode
	{
		switch (mode)
		{
		case SIByL::RHI::PolygonMode::POINT:
			return VK_POLYGON_MODE_POINT;
			break;
		case SIByL::RHI::PolygonMode::LINE:
			return VK_POLYGON_MODE_LINE;
			break;
		case SIByL::RHI::PolygonMode::FILL:
			return VK_POLYGON_MODE_FILL;
			break;
		default:
			break;
		}
		return VK_POLYGON_MODE_MAX_ENUM;
	}

	inline auto getVkCullMode(CullMode mode) noexcept -> VkCullModeFlagBits
	{
		switch (mode)
		{
		case SIByL::RHI::CullMode::FRONT:
			return VK_CULL_MODE_FRONT_BIT;
			break;
		case SIByL::RHI::CullMode::BACK:
			return VK_CULL_MODE_BACK_BIT;
			break;
		case SIByL::RHI::CullMode::NONE:
			return VK_CULL_MODE_NONE;
			break;
		default:
			break;
		}
		return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
	}

	inline auto getVkBlendOperator(BlendOperator mode) noexcept -> VkBlendOp
	{
		switch (mode)
		{
		case SIByL::RHI::BlendOperator::MAX:
			return VK_BLEND_OP_MAX;
			break;
		case SIByL::RHI::BlendOperator::MIN:
			return VK_BLEND_OP_MIN;
			break;
		case SIByL::RHI::BlendOperator::REVERSE_SUBTRACT:
			return VK_BLEND_OP_REVERSE_SUBTRACT;
			break;
		case SIByL::RHI::BlendOperator::SUBTRACT:
			return VK_BLEND_OP_SUBTRACT;
			break;
		case SIByL::RHI::BlendOperator::ADD:
			return VK_BLEND_OP_ADD;
			break;
		default:
			break;
		}
		return VK_BLEND_OP_MAX_ENUM;
	}

	inline auto getVkBlendFactor(BlendFactor factor) noexcept -> VkBlendFactor
	{
		switch (factor)
		{
		case SIByL::RHI::BlendFactor::ONE_MINUS_SRC1_ALPHA:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
			break;
		case SIByL::RHI::BlendFactor::SRC1_ALPHA:
			return VK_BLEND_FACTOR_SRC1_ALPHA;
			break;
		case SIByL::RHI::BlendFactor::ONE_MINUS_SRC1_COLOR:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
			break;
		case SIByL::RHI::BlendFactor::SRC1_COLOR:
			return VK_BLEND_FACTOR_SRC1_COLOR;
			break;
		case SIByL::RHI::BlendFactor::SRC_ALPHA_SATURATE:
			return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			break;
		case SIByL::RHI::BlendFactor::ONE_MINUS_CONSTANT_ALPHA:
			return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
			break;
		case SIByL::RHI::BlendFactor::CONSTANT_ALPHA:
			return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			break;
		case SIByL::RHI::BlendFactor::ONE_MINUS_CONSTANT_COLOR:
			return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			break;
		case SIByL::RHI::BlendFactor::CONSTANT_COLOR:
			return VK_BLEND_FACTOR_CONSTANT_COLOR;
			break;
		case SIByL::RHI::BlendFactor::ONE_MINUS_DST_ALPHA:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			break;
		case SIByL::RHI::BlendFactor::DST_ALPHA:
			return VK_BLEND_FACTOR_DST_ALPHA;
			break;
		case SIByL::RHI::BlendFactor::ONE_MINUS_SRC_ALPHA:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			break;
		case SIByL::RHI::BlendFactor::SRC_ALPHA:
			return VK_BLEND_FACTOR_SRC_ALPHA;
			break;
		case SIByL::RHI::BlendFactor::ONE_MINUS_DST_COLOR:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			break;
		case SIByL::RHI::BlendFactor::DST_COLOR:
			return VK_BLEND_FACTOR_DST_COLOR;
			break;
		case SIByL::RHI::BlendFactor::ONE_MINUS_SRC_COLOR:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			break;
		case SIByL::RHI::BlendFactor::SRC_COLOR:
			return VK_BLEND_FACTOR_SRC_COLOR;
			break;
		case SIByL::RHI::BlendFactor::ZERO:
			return VK_BLEND_FACTOR_ZERO;
			break;
		case SIByL::RHI::BlendFactor::ONE:
			return VK_BLEND_FACTOR_ONE;
			break;
		default:
			break;
		}
		return VK_BLEND_FACTOR_MAX_ENUM;
	}

	inline auto getVkDynamicState(PipelineState state) noexcept -> VkDynamicState
	{
		switch (state)
		{
		case SIByL::RHI::PipelineState::LINE_WIDTH:
			return VK_DYNAMIC_STATE_LINE_WIDTH;
			break;
		case SIByL::RHI::PipelineState::VIEWPORT:
			return VK_DYNAMIC_STATE_VIEWPORT;
			break;
		default:
			break;
		}
		return VK_DYNAMIC_STATE_MAX_ENUM;
	}

	inline auto getVkSampleCount(SampleCount count) noexcept -> VkSampleCountFlagBits
	{
		switch (count)
		{
		case SIByL::RHI::SampleCount::COUNT_64_BIT:
			return VK_SAMPLE_COUNT_64_BIT;
			break;
		case SIByL::RHI::SampleCount::COUNT_32_BIT:
			return VK_SAMPLE_COUNT_32_BIT;
			break;
		case SIByL::RHI::SampleCount::COUNT_16_BIT:
			return VK_SAMPLE_COUNT_16_BIT;
			break;
		case SIByL::RHI::SampleCount::COUNT_8_BIT:
			return VK_SAMPLE_COUNT_8_BIT;
			break;
		case SIByL::RHI::SampleCount::COUNT_4_BIT:
			return VK_SAMPLE_COUNT_4_BIT;
			break;
		case SIByL::RHI::SampleCount::COUNT_2_BIT:
			return VK_SAMPLE_COUNT_2_BIT;
			break;
		case SIByL::RHI::SampleCount::COUNT_1_BIT:
			return VK_SAMPLE_COUNT_1_BIT;
			break;
		default:
			break;
		}

		return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
	}

	inline auto getVKFormat(ResourceFormat format) noexcept -> VkFormat
	{
		switch (format)
		{
		case ResourceFormat::FORMAT_B8G8R8A8_RGB:
			return VK_FORMAT_B8G8R8A8_UINT;
			break;
		case ResourceFormat::FORMAT_B8G8R8A8_SRGB:
			return VK_FORMAT_B8G8R8A8_SRGB;
			break;
		case ResourceFormat::FORMAT_R8G8B8A8_SRGB:
			return VK_FORMAT_R8G8B8A8_SRGB;
			break;
		default:
			break;
		}
		return VK_FORMAT_UNDEFINED;
	}
	
	inline auto getVkDataFormat(DataType datatype) noexcept -> VkFormat
	{
		switch (datatype)
		{
		case SIByL::RHI::DataType::Bool:
			return VK_FORMAT_R8_SINT;
			break;
		case SIByL::RHI::DataType::Int4:
			return VK_FORMAT_R32G32B32A32_SINT;
			break;
		case SIByL::RHI::DataType::Int3:
			return VK_FORMAT_R32G32B32_SINT;
			break;
		case SIByL::RHI::DataType::Int2:
			return VK_FORMAT_R32G32_SINT;
			break;
		case SIByL::RHI::DataType::Int:
			return VK_FORMAT_R32_SINT;
			break;
		case SIByL::RHI::DataType::Mat4:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
			break;
		case SIByL::RHI::DataType::Mat3:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
			break;
		case SIByL::RHI::DataType::Float4:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
			break;
		case SIByL::RHI::DataType::Float3:
			return VK_FORMAT_R32G32B32_SFLOAT;
			break;
		case SIByL::RHI::DataType::Float2:
			return VK_FORMAT_R32G32_SFLOAT;
			break;
		case SIByL::RHI::DataType::Float:
			return VK_FORMAT_R32_SFLOAT;
			break;
		case SIByL::RHI::DataType::None:
			return VK_FORMAT_MAX_ENUM;
			break;
		default:
			break;
		}
		return VK_FORMAT_MAX_ENUM;
	}

	#define MACRO_BUFFER_USAGE_BITMAP(USAGE) \
		if ((uint32_t)usage & (uint32_t)SIByL::RHI::BufferUsageFlagBits::USAGE)\
		{\
			flags |= VK_BUFFER_USAGE_##USAGE;\
		}\

	inline auto getVkBufferUsage(uint32_t usage) noexcept -> VkBufferUsageFlags
	{
		uint32_t flags{};
		MACRO_BUFFER_USAGE_BITMAP(SHADER_DEVICE_ADDRESS_BIT)
		MACRO_BUFFER_USAGE_BITMAP(INDIRECT_BUFFER_BIT)
		MACRO_BUFFER_USAGE_BITMAP(VERTEX_BUFFER_BIT)
		MACRO_BUFFER_USAGE_BITMAP(INDEX_BUFFER_BIT)
		MACRO_BUFFER_USAGE_BITMAP(STORAGE_BUFFER_BIT)
		MACRO_BUFFER_USAGE_BITMAP(UNIFORM_BUFFER_BIT)
		MACRO_BUFFER_USAGE_BITMAP(STORAGE_TEXEL_BUFFER_BIT)
		MACRO_BUFFER_USAGE_BITMAP(UNIFORM_TEXEL_BUFFER_BIT)
		MACRO_BUFFER_USAGE_BITMAP(TRANSFER_DST_BIT)
		MACRO_BUFFER_USAGE_BITMAP(TRANSFER_SRC_BIT)
		return (VkBufferUsageFlags)flags;
	}

	inline auto getVkBufferShareMode(BufferShareMode usage) noexcept -> VkSharingMode
	{
		switch (usage)
		{
		case SIByL::RHI::BufferShareMode::CONCURRENT:
			return VK_SHARING_MODE_CONCURRENT;
			break;
		case SIByL::RHI::BufferShareMode::EXCLUSIVE:
			return VK_SHARING_MODE_EXCLUSIVE;
			break;
		default:
			break;
		}
		return VK_SHARING_MODE_MAX_ENUM;
	}

	inline auto flagBitSwitch(uint32_t const& input, uint32_t const& flag, uint32_t const& vendor_flag, uint32_t& target) noexcept -> void
	{
		if (input & flag)
		{
			target |= vendor_flag;
		}
	}

	inline auto getVkMemoryProperty(MemoryPropertyFlags usage) noexcept -> VkMemoryPropertyFlags
	{
		uint32_t flags{};
		flagBitSwitch(usage, (uint32_t)SIByL::RHI::MemoryPropertyFlagBits::DEVICE_LOCAL_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, flags);
		flagBitSwitch(usage, (uint32_t)SIByL::RHI::MemoryPropertyFlagBits::HOST_VISIBLE_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, flags);
		flagBitSwitch(usage, (uint32_t)SIByL::RHI::MemoryPropertyFlagBits::HOST_COHERENT_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, flags);
		flagBitSwitch(usage, (uint32_t)SIByL::RHI::MemoryPropertyFlagBits::HOST_CACHED_BIT, VK_MEMORY_PROPERTY_HOST_CACHED_BIT, flags);
		flagBitSwitch(usage, (uint32_t)SIByL::RHI::MemoryPropertyFlagBits::LAZILY_ALLOCATED_BIT, VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT, flags);
		flagBitSwitch(usage, (uint32_t)SIByL::RHI::MemoryPropertyFlagBits::PROTECTED_BIT, VK_MEMORY_PROPERTY_PROTECTED_BIT, flags);
		flagBitSwitch(usage, (uint32_t)SIByL::RHI::MemoryPropertyFlagBits::DEVICE_COHERENT_BIT_AMD, VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD, flags);
		flagBitSwitch(usage, (uint32_t)SIByL::RHI::MemoryPropertyFlagBits::DEVICE_UNCACHED_BIT_AMD, VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD, flags);
		flagBitSwitch(usage, (uint32_t)SIByL::RHI::MemoryPropertyFlagBits::RDMA_CAPABLE_BIT_NV, VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV, flags);
		return (VkMemoryPropertyFlags)flags;
	}

	inline auto getVkCommandPoolCreateFlags(CommandPoolAttributeFlags _flag) noexcept -> VkCommandPoolCreateFlags
	{
		uint32_t flags{};
		flagBitSwitch(_flag, (uint32_t)SIByL::RHI::CommandPoolAttributeFlagBits::RESET, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, flags);
		flagBitSwitch(_flag, (uint32_t)SIByL::RHI::CommandPoolAttributeFlagBits::TRANSIENT, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT, flags);
		return (VkMemoryPropertyFlags)flags;
	}

	inline auto getVkCommandBufferUsageFlags(CommandBufferUsageFlags _flag) noexcept -> VkCommandBufferUsageFlags
	{
		uint32_t flags{};
		flagBitSwitch(_flag, (uint32_t)SIByL::RHI::CommandBufferUsageFlagBits::ONE_TIME_SUBMIT_BIT, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, flags);
		flagBitSwitch(_flag, (uint32_t)SIByL::RHI::CommandBufferUsageFlagBits::RENDER_PASS_CONTINUE_BIT, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, flags);
		flagBitSwitch(_flag, (uint32_t)SIByL::RHI::CommandBufferUsageFlagBits::SIMULTANEOUS_USE_BIT, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, flags);
		return (VkCommandBufferUsageFlags)flags;
	}

}