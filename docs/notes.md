# 18/08/2026

## Renderer User Pointer
Renderer class really needs alot of work, passing the renderer as the user pointer for GLFW calls seems like the move.
Callbacks that would benefit from this:
> Keyboard Input
> Mouse Input
> Window Resizing

## Transform Class
Big fan of unitys way of handling vectors and stuff, bit too low level to avoid making a transform class.
Even adding an interface between glm functions would make things a bit nicer too.

## Engine Class
Game has things it has no right to focus on, such as ``Renderer`` and ``InputMap``, these need to be on the engine side of
the application, currently theres no real area for that, most engine stuff is being handles in Renderer because it has the 
glfw and window structures. An ``Engine.hpp`` class would be ideal to encapsulate all of that then game has the one and only
reference to it.