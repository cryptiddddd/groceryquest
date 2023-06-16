// This driver runs a game.
#include <iostream>
#include <string>

#include "game.h"
#include "graph.h"
#include "player.h"
#include "story.h"

using std::cin, std::cout, std::endl, std::string;


/**
 * Runs the entire game.
 * @param playTutorial True: prompts player for the tutorial. False skips.
 * @return No return.
*/
void run(bool playTutorial) {
    Game game(story::getGraph()); // Make game, get the implemented graph.

    // Aesthetic spacing preference.
    cout << endl;

    // Tutorial 
    if (playTutorial) {
        game.runGame("TUTORIAL");
        cout << endl << game.textPartition << endl;
    } else {
        cout << "Enter 'h' at any time for a brief help menu." << endl;
    }

    // Again spacing preference.
    cout << game.textPartition << endl  << endl << endl;

    // Play game.
    game.runGame("START");
    cout << "\nYour journey must end here. Thank you for playing what is only the beginning." << endl << endl;
}

int main(int argc, char *argv[]) {
    // Argument flag `-t` skips tutorial.
    bool playTutorial = !(argc == 2 && string(argv[1]) == "-t");

    try {
        run(playTutorial);
    } catch (EndProgram) {
        cout << "\nEnding game." << endl << endl;
    }

    // Regardless of how the game ends, give the player a moment to see all text.
    cout << "[ Hit enter to close. ]";
    cin.ignore();
    cin.get();

    return 0;
}
