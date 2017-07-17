Debugging Environment
=====================
In case there's the need to understand/debug a problem in the vulkan library code itself it is required to do at least this two configuration change.

- Set the building and running libraries to be those inside
/opt/VulkanSDK/1.0.54.0/source/lib

- change the run configuration LD_LIBRARY_PATH property
- change the building configuration in FindVulkan.cmake to look for it


Once GDB is running use the following gdb command to map the original source file path to a local path
set substitute-path *from* *to*

- set substitute-path /var/lib/jenkins/workspace/Create-Linux-VulkanSDK/Vulkan-LoaderAndValidationLayers /opt/VulkanSDK/1.0.54.0/source