Render Loop
-----------

Render Loop works differently from the Scene Updaters




Main Thread request each commandBuffer
1 Thread is producer
1 Queue for X consumers
X Thread x consumers
Each Consumer 2 tasks

    - prepare command buffer
    - enque command buffer



CommandBuffer 0# Cleaning background color

.. code::python
    FOREACH area IN scene :
    area.commandBuffer

    // TODO refactor to use a completion queue
    if (area.isVisible() ):
        if (!area.wasShown) :

            FOREACH obj IN Vulkan Renderable objects:
                commandBuffer ...
            area.wasShown = true
        end
        primaryCommand.add commandBuffer
    else
