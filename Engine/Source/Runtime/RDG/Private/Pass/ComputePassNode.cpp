module;
#include <vector>
module GFX.RDG.ComputePassNode;

import Core.Log;
import Core.BitFlag;

import RHI.IEnum;
import RHI.IShader;
import RHI.IPipeline;
import RHI.IPipelineLayout;
import RHI.IDescriptorPool;
import RHI.IDescriptorSet;
import RHI.IDescriptorSetLayout;
import RHI.IFactory;
import RHI.IPipelineLayout;

import Core.MemoryManager;
import GFX.RDG.Common;
import GFX.RDG.RenderGraph;
import GFX.RDG.StorageBufferNode;

namespace SIByL::GFX::RDG
{
	ComputePassNode::ComputePassNode(void* graph, RHI::IShader* shader, std::vector<NodeHandle>&& ios, uint32_t const& constant_size)
		: shader(shader)
		, ios(ios) 
		, constantSize(constant_size)
	{	
		type = NodeDetailedType::COMPUTE_PASS;
		RenderGraph* rg = (RenderGraph*)graph;
		for (unsigned int i = 0; i < ios.size(); i++)
		{
			rg->getResourceNode(ios[i])->shaderStages |= (uint32_t)RHI::ShaderStageFlagBits::COMPUTE_BIT;
			switch (rg->getResourceNode(ios[i])->type)
			{
			case NodeDetailedType::STORAGE_BUFFER:
				rg->storageBufferDescriptorCount += rg->getMaxFrameInFlight();
				break;
			case NodeDetailedType::UNIFORM_BUFFER:
				rg->uniformBufferDescriptorCount += rg->getMaxFrameInFlight();
				break;
			case NodeDetailedType::SAMPLER:
				rg->samplerDescriptorCount += rg->getMaxFrameInFlight();
				break;
			case NodeDetailedType::COLOR_TEXTURE:
				rg->getColorBufferNode(ios[i])->usages |= (uint32_t)RHI::ImageUsageFlagBits::STORAGE_BIT;
				rg->storageImageDescriptorCount += rg->getMaxFrameInFlight();
				break;
			default:
				break;
			}
		}
	}

	auto ComputePassNode::devirtualize(void* graph, RHI::IResourceFactory* factory) noexcept -> void
	{
		RenderGraph* rg = (RenderGraph*)graph;

		// create descriptor set layout
		RHI::DescriptorSetLayoutDesc descriptor_set_layout_desc;
		descriptor_set_layout_desc.perBindingDesc.resize(ios.size());
		for (unsigned int i = 0; i < ios.size(); i++)
		{
			ResourceNode* resource = rg->getResourceNode(ios[i]);
			switch (resource->type)
			{
			case NodeDetailedType::STORAGE_BUFFER:
				descriptor_set_layout_desc.perBindingDesc[i] = {
					i, 1, RHI::DescriptorType::STORAGE_BUFFER, rg->getResourceNode(ios[i])->shaderStages, nullptr
				};
				break;
			case NodeDetailedType::UNIFORM_BUFFER:
				descriptor_set_layout_desc.perBindingDesc[i] = {
					i, 1, RHI::DescriptorType::UNIFORM_BUFFER, rg->getResourceNode(ios[i])->shaderStages, nullptr
				};
				break;
			case NodeDetailedType::SAMPLER:
				descriptor_set_layout_desc.perBindingDesc[i] = {
					i, 1, RHI::DescriptorType::COMBINED_IMAGE_SAMPLER, rg->getResourceNode(ios[i])->shaderStages, nullptr
				};
				break;
			case NodeDetailedType::COLOR_TEXTURE:
				descriptor_set_layout_desc.perBindingDesc[i] = {
					i, 1, RHI::DescriptorType::STORAGE_IMAGE, rg->getResourceNode(ios[i])->shaderStages, nullptr
				};
				break;
			default:
				SE_CORE_ERROR("GFX :: Compute Pass Node Binding Resource Type unsupported!");
				break;

			}
		}
		MemScope<RHI::IDescriptorSetLayout> compute_desciptor_set_layout = factory->createDescriptorSetLayout(descriptor_set_layout_desc);

		// create pipeline layout
		RHI::PipelineLayoutDesc pipelineLayout_desc =
		{ {compute_desciptor_set_layout.get()} };
		if (constantSize > 0)
		{
			pipelineLayout_desc.pushConstants = { {0,constantSize, (uint32_t)RHI::ShaderStageFlagBits::COMPUTE_BIT} };
		}
		compute_pipeline_layout = factory->createPipelineLayout(pipelineLayout_desc);

		// create comput pipeline
		RHI::ComputePipelineDesc pipeline_desc =
		{
			compute_pipeline_layout.get(),
			shader
		};
		pipeline = factory->createPipeline(pipeline_desc);

		uint32_t MAX_FRAMES_IN_FLIGHT = rg->getMaxFrameInFlight();
		// create descriptor sets
		RHI::IDescriptorPool* descriptor_pool = rg->getDescriptorPool();
		compute_descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
		RHI::DescriptorSetDesc descriptor_set_desc =
		{ descriptor_pool,
			compute_desciptor_set_layout.get() };
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			compute_descriptorSets[i] = factory->createDescriptorSet(descriptor_set_desc);

		// configure descriptors in sets
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			int textureIdx = 0;
			for (unsigned int j = 0; j < ios.size(); j++)
			{
				ResourceNode* resource = rg->getResourceNode(ios[j]);
				switch (resource->type)
				{
				case NodeDetailedType::STORAGE_BUFFER:
					compute_descriptorSets[i]->update(((StorageBufferNode*)resource)->getStorageBuffer(), j, 0);
					break;
				case NodeDetailedType::UNIFORM_BUFFER:
					compute_descriptorSets[i]->update(rg->getUniformBufferFlight(ios[j], i), j, 0);
					break;
				case NodeDetailedType::SAMPLER:
					compute_descriptorSets[i]->update(rg->getTextureBufferNode(textures[textureIdx++])->getTextureView(),
						rg->getSamplerNode(ios[j])->getSampler(), j, 0);
					break;
				case NodeDetailedType::COLOR_TEXTURE:
				{
					compute_descriptorSets[i]->update(rg->getTextureBufferNode(ios[j])->getTextureView(), j, 0);
				}
				break;
				default:
					SE_CORE_ERROR("GFX :: Compute Pass Node Binding Resource Type unsupported!");
					break;
				}
			}
		}
	}

	auto ComputePassNode::onCompile(void* graph, RHI::IResourceFactory* factory) noexcept -> void 
	{
		barriers.clear();
		RenderGraph* rg = (RenderGraph*)graph;
		unsigned texture_id = 0;
		for (unsigned int i = 0; i < ios.size(); i++)
		{
			switch (rg->getResourceNode(ios[i])->type)
			{
			case NodeDetailedType::STORAGE_BUFFER:
			{
				if (hasBit(attributes, NodeAttrbutesFlagBits::ONE_TIME_SUBMIT))
					rg->getResourceNode(ios[i])->consumeHistoryOnetime.emplace_back
					(ConsumeHistory{ handle, ConsumeKind::BUFFER_READ_WRITE });
				else
					rg->getResourceNode(ios[i])->consumeHistory.emplace_back
					(ConsumeHistory{ handle, ConsumeKind::BUFFER_READ_WRITE });
			}
				break;
			case NodeDetailedType::UNIFORM_BUFFER:
				break;
			case NodeDetailedType::SAMPLER:
			{
				if (hasBit(attributes, NodeAttrbutesFlagBits::ONE_TIME_SUBMIT))
					rg->getTextureBufferNode(textures[texture_id++])->consumeHistoryOnetime.emplace_back
					(ConsumeHistory{ handle, ConsumeKind::IMAGE_SAMPLE });
				else
					rg->getTextureBufferNode(textures[texture_id++])->consumeHistory.emplace_back
					(ConsumeHistory{ handle, ConsumeKind::IMAGE_SAMPLE });
			}
				break;
			case NodeDetailedType::COLOR_TEXTURE:
			{
				if (hasBit(attributes, NodeAttrbutesFlagBits::ONE_TIME_SUBMIT))
					rg->getTextureBufferNode(ios[i])->consumeHistoryOnetime.emplace_back
					(ConsumeHistory{ handle, ConsumeKind::IMAGE_STORAGE_READ_WRITE });
				else
					rg->getTextureBufferNode(ios[i])->consumeHistory.emplace_back
					(ConsumeHistory{ handle, ConsumeKind::IMAGE_STORAGE_READ_WRITE });
			}
				break;
			default:
				break;
			}
		}
	}
	
	auto ComputePassNode::execute(RHI::ICommandBuffer* buffer, unsigned int x, unsigned int y, unsigned int z, unsigned int frame) noexcept -> void
	{
		RHI::IDescriptorSet* compute_tmp_set = compute_descriptorSets[frame].get();

		buffer->cmdBindComputePipeline(pipeline.get());
		buffer->cmdBindDescriptorSets(RHI::PipelineBintPoint::COMPUTE,
			compute_pipeline_layout.get(), 0, 1, &compute_tmp_set, 0, nullptr);
		buffer->cmdDispatch(x, y, z);
	}

	auto ComputePassNode::onCommandRecord(RHI::ICommandBuffer* commandbuffer, uint32_t flight) noexcept -> void
	{
		customDispatch(this, commandbuffer, flight);
	}

}