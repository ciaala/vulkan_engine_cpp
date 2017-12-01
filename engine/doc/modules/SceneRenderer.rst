SceneRenderer
=============

From top to down this is a documentation of the approach used in this engine.
Currently the approach that is used depends on the current knowledge of these following topics:

- 3D Graphic Rendering
- The Vulkan API
- Effects that can be obtained
- Shaders Language
- Modelling

The engine and myself we would keep playing catchup between my current knowledge level and the bugs I encounter and features I wants.
An other limitation would come from the Models used in the game which they might be more complex than what the engine would be currently be able to render.

For all this reason I consider that maintaining this document is the most important part of the engine.

Features of the engine
----------------------

In this stage the engine support loading of vertices and textures and render the vertices as triangles.
There's support to load both Vertex and Fragment shaders.

Loading 3D Model
----------------

In the current status we are importing object as both resources compiled in and
On the horizon there's the use of a loader library that would load any format.

The current implementation of the JSon Loader

{
 "vertex": [],
 "uv": [],
 "textures": [],
 "vertexShaders" : [],
 "fragmetShaders" : []
}

I think that this format and its loader should be changed to support only one single combination of

 - texture
 - vertexShader
 - fragmentShader

But this is part of a more general topic "Binding of resources to shaders".

Pipeline Binding
----------------
You can describe per pipeline a binding like a parameter of vertices and attributes, textures and variables to resources (textures,fragments)

**Current Knowledge**
A set of vertices are drawn as a surface and are mapped into a texture through the UV map.
Some of the vertices can be part of another surface.
Multiple surfaces can use the same texture if we unroll the 3d to a single image file.
**What is the name of this technique ?**

In a complex object, there can be part with different textures attached together.

*Should we write/use a fragment shader that supports multiple textures ?*
*Should we just split the object in multiple sub blocks connected together.*
Up to now both these problems should be resolved with composition of multiple rendering block.

Binding {VX, UV, TX}
--------------------

Vertex, Indices, UV, Shaders and

Vulkan Render
=============
A scene is composed by
- transparent object, a way to collect other objects together and apply a transformation to all of them.
- something  that composition is made of a list of which shares

Primary CommandBuffer.draw
--------------------------
It is responsible to do the screen clean up and setup the render pass.

Game Object to CommandBuffer.draw
---------------------------------
The engine uses a Secondary CommandBuffer to render a Model,
because it can isolate the binding {VX,UV,TX} of elements to it.

The model which is loaded by library is wrapped into an interface something like **VulkanModelRenderer**
to associate the loading format from what a the Render is able to handle.

A **ModelToVulkanCommandBuffer** is used to both setup the Command buffer prepare phase
and to do the draw by updating the MPV (ModelProjectView) matrix of the current object passed to the shaders as uniform buffer.




