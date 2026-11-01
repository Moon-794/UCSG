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

# 31/10/2026
Engine is in a lot nicer of a state, renderer is also attached to the engine as it should be.
The main thing that the game has control of that it shouldnt is calculating collisions during movement,
even implementing a simple broadphase pass for the ship, ideally this is done generically on the engine side
between all physics objects, and the player is classed as a kinematic rigidbody object.

Speaking of, the player also needs a class of it's own, there is way to much shit currently that is just sitting
about inside ``Game.hpp``, like the players extents, transform, velocity, stats. All that could easily go in even a
struct of it's own.

Last 2 big things to do before finishing the version 1 checklist is to make it so the player can expand the ship
with the copper and iron collected, and a static test voxel that can be chipped and added onto.

Expanding the ship shouldnt be too hard, but interacting with the voxel probably will be. Need to implement something
called DDA, by someone called woo ( can't remember the other guy). Probably a big ol copy paste job I don't really
think I need to fully understand it yet at this point.

The Debugger also could use some work, theres not a whole lot to put into it at the moment, but even just adding a tab for timers
would be nice.