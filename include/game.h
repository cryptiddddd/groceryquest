#ifndef GAME_HEADER
#define GAME_HEADER

#include <string>

#include "graph.h"
#include "player.h"


class EndProgram : public std::exception {
    public:
        std::string what();
};


class Game {
    private:
        MapGraph *graph;
        Player player;

        /**
         * Validates a player's input. "Valid" being input that selects the next page in the story.
         * @param options The number of options to pick from.
         * @param input The player's input.
         * @return True if the player has provided satisfactory input. False if not.
        */
        bool isValidInput(int options, std::string input);

        /**
         * Loops until the player has provided the appropriate input (selecting the next story page).
         * @note This handles alternative input, such as "q" for quit.
         * @param options The total number of options to pick from. Valid input range 1 - `options`, inclusive.
         * @return The index of the next page the player wants to navigate to.
        */
        int getInput(int options);

        /**
         * Takes and validates user input to select the next node.
         * @param node The current node.
         * @return The next page of the story.
        */
        StoryNode *pickNextNode(StoryNode *node);

    public:
        // Configurable text separator between nodes' text.
        std::string textPartition = ". . . . . . . . . . . . . . . . . . . . . . . . . . . . .";

        /**
         * Constructs the game using the current graph.
        */
        Game(MapGraph *_graph);

        ~Game();

        /**
         * Main game loop.
         * @param game Pointer to the mapgraph containing game data.
         * @param closingMessage Message to display after the game has ended.
        */
        void gameLoop(StoryNode *start);

        /**
         * Runs game from the given node [by title.]
        */
        void runGame(std::string startTitle);
};


#endif
