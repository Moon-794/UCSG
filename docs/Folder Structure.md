# Folder structure
This document specifies the standards used to organize the game's resources. 

## Sprites
Sprite is the general term for any asset that ends up being an OpenGL texture. Upon game startup each sprite will be read from the files, assigned a unique string identifier + corresponding numerical ID, and stitched into the master spritesheet. Individual sprite data is retrieved from the `SpriteManager` object. Each `Tile` that exists within a `Chunk` will have a corresponding sprite ID. When drawing a `Chunk`, the renderer uses the `Tile[][]`  array within to retrieve the sprite index into the spritesheet. UV's are then calculated shader side, and presented to the screen.

### Static Sprites
Static sprites, as the name implies, have a single texture associated with their ID. As a result the sprite does not appear to be animated over time. This kind of sprite will be used for things that are not expected to change rapidly, such as floors and walls etc.

### Dynamic Sprites
Dynamic, or ***animated*** sprites, contain a base sprite alongside ***n*** animation frames. Additional information such as `currentFrame`, `frameOffset`, `isPaused` etc is instance specific.

### Chunk Format
How chunks are represented then, in ***data*** is as an array of base ID's such as `Tile[][] tiles`. Tiles are logical entities that are generate from game file definitions by `TileManager`. Each tile has a member variable `Sprite` containing the base sprite ID and any relevant animation information. All tiles with a dynamic sprite are cached in the `DynamicTiles` array, to allow for faster looping over all dynamic sprites during animation updates.

#### Note
Is this the best way of doing it? I'm not sure, there's still a few issues such as storing general vs instance specific information in `Tile`, but perhaps an iterative design will work better to solve that as its still not clear how most variables will piece together. At the end of the day I'll take a bit more memory per tile, as long as a late game run takes up no less than 256MB of RAM.

### Sprite Folder Structure
```
C:.
└───Sprites
    ├───Dynamic
    │   ├───DynamicSprite1
    │   │       frame1.png
    │   │       frame2.png
    │   │       frame3.png
    │   │
    │   ├───DynamicSprite2
    │   │       frame1.png
    │   │       frame2.png
    │   │       frame3.png
    │   │
    │   └───DynamicSprite3
    │           frame1.png
    │           frame2.png
    │           frame3.png
    │
    └───Static
            Sprite1.png
            Sprite2.png
            Sprite3.png

```
