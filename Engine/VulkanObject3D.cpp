#include "VulkanObject3D.h"

void VKENGINE::Entity::init ( Device* pDevice, VkDescriptorPool descriptorPool, SEVIAN::Entity3D obj, std::vector<Frame> frames ) {
	device = pDevice;
	std::cout << "hello";

	std::vector<VulkanObject3D> objects;

	for (auto& mesh : obj.meshes) { 
		VulkanObject3D object;

		//auto _vertices = mesh.vertices;
		//auto _indices = mesh.indices;
        auto x = sizeof ( UniformBufferObject );
        printf ( "%d", x );

		auto texture = device->createTexture ( "textures/fauna-marina.jpg" );

		auto descriptorSets = device->createDescriptorSets ( frames, descriptorPool, texture.imageView, texture.sampler, sizeof ( UniformBufferObject ) );
		auto attributeDescriptions = Entity::getAttributeDescriptions ();
		auto pipeline = createGraphicsPipeline ( Entity::getBindingDescription (), attributeDescriptions, device->descriptorSetLayout );
		//auto pipelineLayout = device->pipelineLayout;

		object.vertex = device->createDataBuffer ( (void*) mesh.indices.data (), sizeof ( mesh.indices[0] ) * mesh.vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		object.indices = device->createDataBuffer ( (void*) mesh.indices.data (), sizeof ( mesh.indices[0] ) * mesh.indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
		object.indicesSizes = mesh.indices.size ();
		object.pipeline = pipeline;
		object.pipelineLayout = pipelineLayout;
        object.descriptorSets = descriptorSets;
		objects.push_back ( object );
	
	}

	//VulkanObject3D obj;

	
	this->objects = objects;
	


}

void VKENGINE::Entity::draw ( VkCommandBuffer commandBuffer, Frame frame ) {
    for (auto& object : objects) {
       
        object.draw ( commandBuffer, frame );
    }
}


VkPipeline VKENGINE::Entity::createGraphicsPipeline (
    VkVertexInputBindingDescription bindingDescription,
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions,
    VkDescriptorSetLayout& descriptorSetLayout ) {

    auto vertShaderCode = device->readFile ( "shaders/solid_vert.spv" );
    auto fragShaderCode = device->readFile ( "shaders/solid_frag.spv" );

    VkShaderModule vertShaderModule = device->createShaderModule ( vertShaderCode );
    VkShaderModule fragShaderModule = device->createShaderModule ( fragShaderCode );

    VkPipelineShaderStageCreateInfo vertShaderStageInfo {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size ());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data ();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencil {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size ());
    dynamicState.pDynamicStates = dynamicStates.data ();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

    if (vkCreatePipelineLayout ( device->device, &pipelineLayoutInfo, nullptr, &pipelineLayout ) != VK_SUCCESS) {
        throw std::runtime_error ( "failed to create pipeline layout!" );
    }

    VkGraphicsPipelineCreateInfo pipelineInfo {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = device->renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    VkPipeline pipeline;
    if (vkCreateGraphicsPipelines ( device->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline ) != VK_SUCCESS) {
        throw std::runtime_error ( "failed to create graphics pipeline!" );
    }

    vkDestroyShaderModule ( device->device, fragShaderModule, nullptr );
    vkDestroyShaderModule ( device->device, vertShaderModule, nullptr );

    return pipeline;
}
