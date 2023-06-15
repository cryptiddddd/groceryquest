# Grocery Quest Design

Grocery Quest is the test game for its from-scratch framework.

## Design outline

![Current UML Design](uml_design.png)

Skeleton:

- `graph.h` & `graph.cpp`
    + `Graph` class.
    + `StoryNode` struct.
    + `VertexTitleConflict` exception.
    + `VertexNotFound` exception.
- `player.h` & `player.cpp`
    + `Player` class.
- `story.h` & `wizardville.cpp`
    + `getStory()` function.
    - `wizardville.cpp` contains the implementation expected by `story.h`. (Any `.cpp` that implements `getStory()` could be substituted.)
- `game.h` & `game.cpp`
    + `Game` class.
    + `EndProgram` exception.
