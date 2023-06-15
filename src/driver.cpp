// This driver runs a game.
#include <iostream>
#include <string>

#include "game.h"
#include "graph.h"
#include "player.h"
#include "story.h"

using std::cin, std::cout, std::endl, std::string;


int main(int argc, char *argv[]) {
    // Aesthetic preference.
    cout << endl << endl;

    Game game(story::getGraph());

    try {
        // Argument flag `-t` skips tutorial.
        if (!(argc == 2 && string(argv[1]) == "-t")) {
            game.runGame("TUTORIAL");
            cout << endl << game.textPartition << endl << game.textPartition << endl  << endl << endl;
        }

        // Play game.
        game.runGame("START");
        cout << "\nYour journey must end here. Thank you for playing what is only the beginning." << endl << endl;
    } catch (EndProgram) {
        cout << "\nEnding game." << endl << endl;
    }

    string doorStop;
    cout << "[ Enter any key to close. ]";
    cin >> doorStop;

    return 0;
}
