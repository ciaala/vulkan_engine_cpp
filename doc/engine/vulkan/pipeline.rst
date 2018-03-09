Pipeline
--------

The pipeline of a GPU is made of a combination of fixed function and programmable stages.
Each stage receives an input data from the previous stage and produce an output data for the next stage.

A GPU contains 2 different functional pipelines one for the **Graphic** processing and the other to handle the **Compute** functionality.
Input and output data can be either an Image or a Buffer.




RenderPass
==========

Encapsulate the stage that are run, and describe the input and output images.



GraphicPipeline
===============
Works inside the RenderPass

* Draw [ Indirect Buffer ]
* Input Assembly [ Index Buffer, Vertex Buffer ]
* Vertex Shader [ Descriptors ]
* Tessellation Control Shader [ Descriptors ]
* Tessellation Evaluation Shader [ Descriptors ]
* Geometry Shader [ Descriptors ]
* Primitive Assembly
* Clip And Cull
* Rasterizer
* Prefragment Operations
* Fragment Shader [ Descriptors]
* Postfragment Operations
* ColorBlending

Descriptors
===========

* Push Constants
* Images
* Uniform and Storage Buffers
* Texel Buffers

Framebuffer
===========

* Depth-Stencil
* Input-Attachments
* Color-Attachments



Callgraph
=========

