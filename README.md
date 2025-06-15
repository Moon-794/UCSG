# UCSG

2D game about exploring an abandoned materials processing ship in space

Player wakes up from cryosleep in a small cargo ship, far in the future.
Their ship had malfunctioned, failing to wake the inhabitants and allowing the ship to slowly come into disarray.
The player is the only survivor.

Now awake, the player must restore functionaility to their spacecraft, and begin to explore a very different universe.

Goal/Win condition - Reach the center of the galaxy, where what remains of humanity orbit an ultramassive black hole.

Optional post win - Enter the black hole, emerge in a new universe with exotic materials to upgrade your ship/gear/factory.

# Mechanic Ideas

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

