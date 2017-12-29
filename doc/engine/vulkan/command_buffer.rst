==============
Command Buffer
==============

The command buffer is a collection of commands that the application builds and submits to the  vulkan's graphic/processing driver.
The driver receives and bakes them: it validates and compiles the command buffers to make them ready for the execution on the device.
The device will then just have run all the commands in a main primary buffer.


Example of Operation
--------------------

    - binding vertex buffer
    - binding pipeline
    - recording Render Pass commands
    - setting viewport and scissor
    - specifying drawing commands
    - controlling copy operations on image and buffer contents,

Type Of Commands
----------------

Commands can be organized in three macro categories
- Action

    - draw
    - dispatch
    - clear
    - copy
    - query / timestamp ( what is timestamp ? )
    - start / end subCommands

- State Management

    - descriptor sets
    - binding pipelines and buffers
    - set dynamic state ( What are dynamic states ? )
    - push constants
    - render pass to sub-pass state

- Synchronization

    - pipeline barriers
    - set events
    - wait events
    - render pass to sub-pass dependencies ( What are they ? )


Allocation
----------

Command buffer are expensive so re-using them and re-using also their memory is convenient for this reason they are allocated from a **Command Buffer Pool**
In command pool creation it is possible to specify a flag to specify the allowed behaviour in resetting the single command buffer.
**VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT** specifies that the pool's buffers can only be reset all together with a pool wide reset command **vkResetCommandPool**
( is it more efficient )

Hierarchy: Primary and Secondary
--------------------------------

