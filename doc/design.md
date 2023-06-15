# Grocery Quest Design

Grocery Quest is the test game for its from-scratch framework.

## Design outline

![Current UML Design](uml_design.png)

## File skeleton:

- `graph.h` & `graph.cpp`
    + `Graph` class.
    + `StoryNode` struct.
    + `VertexTitleConflict` exception.
    + `VertexNotFound` exception.
- `player.h` & `player.cpp`
    + `Player` class.
- `story.h` & `story/groceryQuest.cpp`
    + `getStory()` function.
    - Files within `src/story/` implement `story.h`, providing the `getStory()` function with a real return value.
- `game.h` & `game.cpp`
    + `Game` class.
    + `EndProgram` exception.
