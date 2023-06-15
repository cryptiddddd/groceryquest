[![GROCERY QUEST BANNER!](doc/banner.png)](https://cryptidddd.itch.io/groceryquest)

# Grocery Quest

Grocery Quest is a non-linear text adventure I'm building from scratch. Its framework is something I would one day like to make available as a tool for other developers, but currently I don't think I'm ahead of anyone.

## Quickstart

To compile game with G++:
```
g++ -Iinclude src/driver.cpp src/game.cpp src/graph.cpp src/player.cpp src/story/groceryquest.cpp -o bin/quest
```

To compile graph unit tests: 
```
g++ -Iinclude src/graph.cpp test/unitTests.cpp -o bin/tests
```
