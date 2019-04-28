# LagEngine

This project exists as a learning experience about the implementation and design decisions of a 3D rendering engine. It’s also a project to keep my C++ skills on check.

While it tries to expose a friendly programming interface it is not meant to have a GUI or an editor (at least for now).

It includes a demo where an AI plays a version of Pong on a stylized scene.

![](http://www.bmlourenco.com/public/images/lagengine/6.jpg)

### Demo video: https://youtu.be/miMLTeraOLc


# Current features:
- Forward rendering based on simple per-pixel lighting, supporting directional and point lights.
- Supports meshes (loaded with Assimp library), XML material definition defining textures and shaders.
- Support render targets and various render passes. (WIP)
- Interface-based renderer to allow multiple Graphics API to be implemented. Currently has a OpenGL 4.5 implementation.
- Renderer based on a render-queue approach, iterating through all RenderTargets and through each of their Viewports to gather the Renderables. (Culling and queue sorting to be implemented)
- SceneManager containing the various scene representations, each with its own purpose (object hierarchy, culling, fast iteration).
- SceneGraph based on a hierarchy of SceneNodes. SceneNodes have a 3D space transformation relative to their parent.
- SceneObjects can be attached to SceneNodes and are inherit their properties. (Example SceneObjects: Lights, MeshEntities, Cameras)
- XML based ResourceManager for assets like meshes, image textures, shader code.
- Hot reloading of file Resources (images, shaders, materials).


# License
Licensed under the MIT license, see [LICENSE](https://github.com/MadEqua/LagEngine/blob/master/LICENSE) for details.
