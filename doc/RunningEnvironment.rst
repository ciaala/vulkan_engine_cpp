Running Environment for Vulkan
##############################

These three environment options should be set to allow vulkan to find and use the validation layers ( is there anything more ? )

VK_LAYER_PATH=/opt/VulkanSDK/1.0.54.0/x86_64/etc/explicit_layer.d
PATH=${PATH}:/opt/VulkanSDK/1.0.54.0/x86_64/bin
LD_LIBRARY_PATH	=${LD_LIBRARY_PATH}:/opt/VulkanSDK/1.0.54.0/source/lib

The example is already matching the configuration rules needed to debug.
(option: LD_LIBRARY_PATH)