My game is the player waking up in a derelict space craft, thousands of years into the future orbiting an uncharted planets moon, 
the only survivor. They must explore the ship gradually, unlocking rooms blocked by debris via upgrades to unlock things, 
like asteroid harvesting, an automated factory floor, and a warp drive to travel around the solar system, then local star cluster, 
then the whole galaxy. They can search for and explore abandonded ships, acting as dungeons to fight and mine their way to cool gear upgrades, 
trade with alien species

The player movement stuff is not that much code so can safely be redone better at a later date, for now its useful
for moving the camera around to test other features, so it stays for now.

The main 2 features to think about for now is the game world and its structure
Each scene the player can walk about in is known as an area.

 - Areas are drawn procedurally, and have several layers that can be drawn independently.
 - Areas also contain properties of the tiles, such as whether or not they are colliders


# OK OK

so there is a master texture_atlas generated at startup, it contains both static and animated tiles.
Tiles are stitched into this atlas at startup, first base tiles, then mod tiles, in order of dependacies, then alphabetically.

Due to the nature of tiles having potentially different ID's based on the specific set of mods loaded, tiles are not
referenced by a numeric ID, and instead by their string name.
Note: it could be possible, by having the mod reference tile `0`, then engine side that is added to a `mod_tile_offset`. But we will see.

# 03/07/2025
Renderer side, areas are going to be split up into chunks of 16x16 tiles. E.g a 20x20 tile area will be made of a 2x2 chunk grid from the renderers
point of view, tiles outside the defined areadata are simply `Tile::Empty` tiles. The logic side of things has no concept of chunks, as the relatively lightweight tiledata structure
permits obscenely large areas for only a couple megabytes of ram. For now anyways.

Each chunk will have its own VAO to minimise updates, at the expense of VRAM.
Each quad in the grid will have 4 vertices with their own UV offsets.

So that means.

1 - Need a function to generate a chunk VAO.
2 - Logic side flags all changed tiles every frame.
3 - CPU side, the renderer will adjust flagged tile chunks VAOs UV coordinates to reflect the tile indices. If this is too slow in brute force tests, a data texture uniform and shader side UV calculations will be tested.
4 - profit