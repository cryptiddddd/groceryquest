// This driver runs a game.
#include <iostream>
#include <string>

#include "game.h"
#include "graph.h"
#include "player.h"
#include "story.h"

using std::cin, std::cout, std::endl, std::string;


void run(bool tutorial) {
    Game game(story::getGraph());

    // Aesthetic preference.
    cout << endl << endl;
    if (tutorial) {
        game.runGame("TUTORIAL");
        cout << endl << game.textPartition << endl << game.textPartition << endl  << endl << endl;
    }

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

    cout << "[ Hit enter to close. ]";
    cin.ignore();
    cin.get();

    return 0;
}
