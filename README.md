# gled
OpenGL client for neovim.

### What's done:
The rendering library that will be implemented by the client is working.  The library exposes a simple interface for updating the screen buffer.

### TODO
messagepack integration is currently in progress (and nearly complete).  Once this is done, the editor will be a completely functional, albeit simple, neovim client.  At this point, this project will branch into two projects.  
The first will stay a simple text-only client for neovim.  The other will allow for the insertion of images, 3d-models, and even procedurally generated geometry to be integrated into the source file.  The idea behind this is to allow realtime in-editor debugging of parametric OpenGL code or the integration of textures/sound/whatever with the code itself.  This idea is borrowed from ye olde GEOS development suite.  For specific applications (especially in the domain of procedural graphics) I think there is a lot of potential in that idea.  I think it will prove an excellent middle ground between blueprint type scripting systems, and purely text-based coding.

### Ideas
Vulkan, baby
