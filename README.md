# UCSG

2D game about exploring an abandoned materials processing ship in space

Player wakes up from cryosleep in a small cargo ship, far in the future.
Their ship had malfunctioned, failing to wake the inhabitants and allowing the ship to slowly come into disarray.
The player is the only survivor.

Now awake, the player must restore functionaility to their spacecraft, and begin to explore a very different universe.

Goal/Win condition - Reach the center of the galaxy, where what remains of humanity orbit an ultramassive black hole.

Optional post win - Enter the black hole, emerge in a new universe with exotic materials to upgrade your ship/gear/factory.

# Mechanic Ideas
Thats obviously a lot for a solo dev project. The game needs to settle on 1 main playstyle. It would be ok to expand to the
other stuff if thats all working fine.

- Definitely want the player waking up in the far future, derelict spacecraft, only survivor etc
- Do I want a set story? Might cramp replayability. Could be a more memorable experience? I don't have a good story to tell tho.
- Optional story would be cool, maybe even a secret story?
- I think the pillar of the ship being a mobile asteroid processing factory is one I really like.
    - How does the player harvest asteroids?
        - i dont know
    - Is there any manual crafting?
- What kind of gear should the player have?
    - Weapons
    - Armor
    - Tools?
- Why would anyone bother modding this?

# Focusing in Mechanics
For the alpha build, the following features are good enough I think:

- No ship travel
- Unlocking 1-3 ship rooms
- Factory floor + asteroid stuff
- Goal: Repair ship thrusters

# State machine discussion

It would be nice to handle the varying states of game logic with a state machine.

The main game state is just the walking around business. However what if the player decides to look at the interstellar map?

The entire player controller would be disabled, same with drawing the ship as it'd be wasteful.
Instead the map and all its UI would be drawn, and the map navigation controls would take over.

That seems like a very well defined state, but its the only one I can think of for now. So a simple enum might even be better for now.

There are other things, though. The player can be walking about, then interact with a person and begin a dialogue. "Talking" could be considered a
state and control would be disabled, and time stopped too. However the same world is drawn, just with UI dialogue elements.

The pause menu would be the same, stop time and dont let the player move, display the main menu.

Not sure how to resolve.

```switch (currentGameState) {
    case GameState::Walking:
        updatePlayer();
        drawWorld();
        break;
    case GameState::MapView:
        updateMap();
        drawMapUI();
        break;
    case GameState::Dialogue:
        drawWorld(); // Still draw the world
        updateDialogue();
        drawDialogueUI();
        break;
    case GameState::Paused:
        drawWorld(); // Optional: maybe fade it?
        drawPauseMenu();
        break;
}```

