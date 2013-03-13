dryGL
=====

dryGL is a lightweight OpenGL ES 2.0 framework for mobile devices, targeting iOS & Android

Current status:
- iOS project
- FreeImage loader
- Texture
- Pixels
- Shader
- BasicShader
- Camera: CameraPerspective, CameraOrthogonal
- Fbo
- Addons/QuadBatch: simple quad drawing, later on it will be a real quadbatch pipeline

Example:
- Rotating 3D cube + cameraPerspective + cameraOrthogonal + Fbo example

Dependencies:
- Glm
- FreeImage

Temp:
- Move FreeImage loader to Addons/FreeImageLoader

Roadmap:
- Renderer utilities (clear, viewports, renderstates, etc)
- Vbo
- Mesh
- Auto uniforms/attributes in shader object, and maybe basic auto attributes/uniforms (positions, texcoords, colors, etc)
- State/Render manager
- Inheritable basic shader
- Debug functions (DebugDrawText, DebugDrawRectangle, etc)
- Addons: Add libpng / libjpg / libdds loaders
- Addons: Scene, Object, Material, SceneRenderer
- Addons: EffectComposer, Postprocess effects / shaders

