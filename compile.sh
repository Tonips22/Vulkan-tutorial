#!/bin/bash

## Linux ##
/home/user/VulkanSDK/x.x.x.x/x86_64/bin/glslc shader.vert -o shaders/vert.spv
/home/user/VulkanSDK/x.x.x.x/x86_64/bin/glslc shader.frag -o shaders/frag.spv

## MacOS ##
# VULKAN_SDK=/path/to/vulkan-sdk/bin
# "$VULKAN_SDK/glslc" shader.vert -o shaders/vert.spv
# "$VULKAN_SDK/glslc" shader.frag -o shaders/frag.spv

