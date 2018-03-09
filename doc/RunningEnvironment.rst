Running Environment for Vulkan
##############################


Setup to Run the executables
These three environment options should be set to allow vulkan to find and use the validation layers ( is there anything more ? )

.. code:: sh
given that the Vulkan libraries are located in the directory /opt/VulkanSDK/1.0.54.0
    VK_LIBRARY_ROOT=/opt/VulkanSDK/1.0.54.0
    VK_LAYER_PATH=${VK_LIBRARY_ROOT}/x86_64/etc/explicit_layer.d
    PATH=${PATH}:${VK_LIBRARY_ROOT}/x86_64/bin
    LD_LIBRARY_PATH	=${LD_LIBRARY_PATH}:${VK_LIBRARY_ROOT}/source/lib

    The example is already matching the configuration rules needed to debug.
    (option: LD_LIBRARY_PATH)


Disable OpenAL Soft
===================

In case the following messages appears at startup:
.. code::
Cannot connect to server socket err = No such file or directory
Cannot connect to server request channel
jack server is not running or cannot be started
JackShmReadWritePtr::~JackShmReadWritePtr - Init not done for 4294967295, skipping unlock
JackShmReadWritePtr::~JackShmReadWritePtr - Init not done for 4294967295, skipping unlock

then OpenAL is trying to start in soft mode.

You can disable the behaviour by setting the configuration file: **~/.alsoftrc**
    - [general]
    - drivers=-jack,

(Note that the ending comma is important, as it tells OpenAL Soft to try other backends.)


