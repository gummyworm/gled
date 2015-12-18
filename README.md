# gled
OpenGL client for neovim.

### What's done:
The rendering library that will be implemented by the client is working.  The library exposes a simple interface for updating the screen buffer.

### TODO
Previous neovim integration was to be achieved via the messagepack API.  Since there is supposedly effort (soon to be) underway to refactor neovim into a library, the actual integration with neovim is temporarily on hold.

Once neovim integration is done the project will split into two.  One will be a simple text-based client while the other will allow for the insertion of images, 3d-models, and even procedurally generated geometry to be integrated into the source file.  The idea behind this is to allow realtime in-editor debugging of parametric OpenGL code or the integration of textures/sound/whatever with the code itself.  This idea is borrowed from ye olde GEOS development suite.  For specific applications (especially in the domain of procedural graphics) I think there is a lot of potential in that idea.  There is a whole lotta graphics code that can be explained so much easier with visualization than a comment, and it's worth exploring a system where the two are directly intertwined (IMO). I think it will prove an excellent middle ground between blueprint type scripting systems, and purely text-based coding.  The obvious counterpoint is that the code should reside in its own location and the visualization being an independent program simply updating according to changes.  That's valid, but it creates a lot of work loading/running/etc. just to see what various code does.

Until the neovim library is ready, work will continue on improving the fancy features for the graphical version of this editor.

### Ideas
Vulkan, baby
