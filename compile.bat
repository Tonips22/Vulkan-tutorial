@REM Windows
set VULKAN_SDK=C:\VulkanSDK\VERSION\Bin
"%VULKAN_SDK%\glslc.exe" shader.vert -o vert.spv
"%VULKAN_SDK%\glslc.exe" shader.frag -o frag.spv
pause