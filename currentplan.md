The player movement stuff is not that much code so can safely be redone better at a later date, for now its useful
for moving the camera around to test other features, so it stays for now.

The main 2 features to think about for now is the game world and its structure
Each scene the player can walk about in is known as an area.

 - Areas are drawn procedurally, and have several layers that can be drawn independently.
 - Areas also contain properties of the tiles, such as whether or not they are colliders

TODO area wise
- Areas should have an ID associated with them
- Tiles can contain a property "AreaTransition"
    - "AreaTransition" contains an areaID and a position to move the player to
    