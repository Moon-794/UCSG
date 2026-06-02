# Notes
In terms of complexity i mean walls can have multiple thin pipes running up and across them, and slight recesses into walls for custom control panels etc
The game is intended for 3D, it will be developed in mind as a 3D game, this is final.
However for a large part of development, gameplay will be the primary focus, not the 3D rendering system.

The solution will be to develop the game with a "simulation first" approach, and have a temporary 2D
"debug" view whilst the 3D engine is being worked on.

It is important to not sleep on the 2D view, as it will likely become an extremely powerful debug tool
over the course of development.

The complex ship interiors I have in my head is probably not possible to plan for as a first pass,
so scope will need to be limited till I have a better hand on things, and it will need iterated from there.

In terms of complexity i mean walls can have multiple thin pipes running up and across them, 
and slight recesses into walls for custom control panels etc

# Version 1 World Data layout:
 
2D grid of floor tiles.
Each tile has a ceiling height in meters.
That is it

# Version 2 World Data Layout
 
Each tile has 4 edges (North, East, South, West)
Edges can define walls of various thicknesses / styles
Walls have independent coordinates where wires and pipes can be attached.
Wires and pipes can also be attached to ceilings.

# Version 1 Asteroid harvesting:
 
Asteroids of 2 types (copper/iron) appear randomly at a static height around ship.
Player can spacewalk with basically no restrictions and mine them with a pick.
Mining fills two global resource meters.
Spend resources to expand ship.
