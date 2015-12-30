# gled
OpenGL client for neovim.
The purpose of gled is to provide a powerful tool for creating projects with both resources and source (similar to the GEOS assembler).  The project also aims to be a sort of "Hollywood" editor with the inclusion of per-character animation, materials, and fullscreen post processing.  

### Special Characters
The Private Use Area range of U+E000..U+E008 are reserved for future use.

The Private Use Area Unicode range U+E008..U+F8FF (plane 0) is used to represent various graphical data on the display.
In the buffer representation, for each file, resources are assigned a unique code from U+E008 on up.  A simple key-value metadata file maps this code to the corresponding resource.
Each block of adjacent matching ID's is rendered to texture and displayed at the offset of the adjacency-block's upper-left corner (typically only the upper left corner is responsible for this rendering).  Since such a block may begin well outside the viewable buffer, the virtual buffer must be large enough to handle the maximum character size of a resource.  This is currently set to 80x80 characters (and the virtual buffer therefore an additional 79 characters in width and height).

### Done
The rendering library that will be implemented by the client is working.  The library exposes a simple interface for updating the screen buffer.  Meshes may be loaded from a wide variety of 3d formats thanks to assimp.

### To do
Previous neovim integration was to be achieved via the messagepack API.  Since there is supposedly effort (soon to be) underway to refactor neovim into a library, the actual integration with neovim is temporarily on hold.

### Ideas
Vulkan, baby
