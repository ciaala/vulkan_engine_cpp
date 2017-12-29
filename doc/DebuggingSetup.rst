=====================
Debugging Environment
=====================

In case there's the need to understand/debug a problem in the vulkan library code itself it is required to do at least this two configuration change.

- Set the building and running libraries to be those inside /opt/VulkanSDK/1.0.54.0/source/lib

- change the run configuration LD_LIBRARY_PATH property
- change the building configuration in FindVulkan.cmake to look for it


Once GDB is running use the following gdb command to map the original source file path to a local path
set substitute-path *from* *to*

- set substitute-path /var/lib/jenkins/workspace/Create-Linux-VulkanSDK/Vulkan-LoaderAndValidationLayers /opt/VulkanSDK/1.0.54.0/source

if you want to setup gdb automatically then you can set the command should be put inside the file
./.gdbinit

and you also have to enable auto-loading per project ( local path ) by editing the file ~/.gdbinit and add
- add-auto-load-safe-path /user/crypt/projects/vulkan-engine.cpp/.gdbinit
or
- set auto-load safe-path /

