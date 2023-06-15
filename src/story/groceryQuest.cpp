/* This file is the script for wizardville. Most of the file is writing. I suggest enabling softwrap. */
#include "story.h"

#include <string>

#include "game.h"
#include "graph.h"

using namespace std;


// Creating the tutorial
// Callbacks

string tutDoubt(StoryNode *n, MapGraph *m, Player *p) {
    p->addFlag("tutorialDoubt");
    return "";
}

string tutAssurance(StoryNode *n, MapGraph *m, Player *p) {
    if (p->checkFlag("tutorialDoubt")) {
        return "I believe in you.";
    }
    return "";
}

// Tutorial subgraph.
void createTutorial(MapGraph *graph) {
    graph->addVertex(new StoryNode(
        "This is a nonlinear text adventure. To play, you will be given a simple menu of options to pick from. To make your selection, you will enter the corresponding number.\n\nHere is your first test:", 
        "TUTORIAL"
    ));

    graph->addVertex(new StoryNode(
        "Great. The input parser for this game is picky, and will not accept '   2 ' when you mean to select option '2'. You will have unlimited attempts to enter valid input.\n\nIn addition to the provided options, you may enter:\n\t'h' for help\n\t'i' for game inventory\n\t'o' to view objectives\n\t'q' to quit",
        "tut.1"
    ));

    graph->addVertex(new StoryNode(
        tutDoubt,
        "You'll get it eventually.",
        "tut.1.b"
    ));

    graph->addVertex(new StoryNode(
        "Grocery Quest is made from scratch over a couple of weeks for my data structure class. Each 'page' of the adventure is a point on a map (or a node on a graph), that holds data representing one location, or point in conversation.\n\nEach node may also have a callback function, and any callback function may update a player variable (add something to your inventory, set a flag to remember your decision) or check how many times you've visited that location, in order to create a more responsive experience.\n\nGrocery Quest is written in C++ and compiled with GCC. For this project, I wrote a simple framework which could easily be customized for other text adventures, by anyone with some familiarity with C++ (or with a good template :]). I attempted to make it as approachable to work with as possible.\n\nTo view source files, visit https://github.com/cryptiddddd/groceryquest",
        "tut.2"
    ));

    graph->addVertex(new StoryNode(
        tutAssurance,
        "Thank you for playing.",
        "tutEnd",
        "END"
    ));

    graph->addArc("TUTORIAL", "tut.1", "Got it!");
    graph->addArc("TUTORIAL", "tut.1.b", "I don't get it.");

    graph->addArc("tut.1.b", "tut.1", "Ok.");

    graph->addArc("tut.1", "tutEnd", "I'm ready for my adventure");
    graph->addArc("tut.1", "tut.2", "Wait! Tell me more about this program.");

    graph->addArc("tut.2", "tutEnd", "Okay thank you, I will adventure now.");
}


// Creating the story.
// Callbacks 

string bedroom(StoryNode *n, MapGraph *m, Player *p) {
    if (!n->visits) {
        return "\"It's good you're up,\" Simon meows. \"You have a busy day today.\"\n\nYou should have a to-do list on the kitchen counter.";
    }
    return ""; 
}

string bedroomHat(StoryNode *n, MapGraph *m, Player *p) {
    if (!n->visits) {
        p->addFlag("wizardHat");
        m->deleteArc("hill.house.bedroom", n->title); // Theoretically, there will only be one visit to this node.
    }
    return "You put it on your head.";
}

string kitchen(StoryNode *n, MapGraph *m, Player *p) {
    if (n->visits == 1) { // On the second visit, shorten the descriptive text.
        n->description = "Your kitchen is cluttered, countertops decorated with all kinds of science-y equipment.";
    }
    
    if (!p->checkFlag("errandList")) { // If the player hasn't checked the list yet, prompt that it's here.
        return "Amidst the mess is your to-do list. You've written it in a glitter gel pen.";
    }

    return "";
}

string errandList(StoryNode *n, MapGraph *m, Player *p) {
    if (n->visits == 1) { // Second visit, change descriptive text.
        n->description = "You check in on your list...";
    }

    if (!p->checkFlag("errandList")) { // If player hasn't checked the list before, add the objectives, set the flag.
        p->setObjective("wizardHat", "wear my favorite hat c:");
        p->setObjective("paperworkDelivered", "deliver paperwork to the bookkeeper (important!!!)");
        p->setObjective("herbalistPickup", "pickup from the herbalist");
        p->setObjective("bakeryTrade", "trade @ the bakery");
        p->setObjective("tailorDelivery", "deliver order to tailor");
        p->setObjective("illusionistCheck", "check on the illusionist....");

        p->addFlag("errandList");
    }

    // Print the updated objectives menu.
    return p->readObjectives() + "\n(You can enter 'o' to view this list again at any time.)";
}

string livingRoom(StoryNode *n, MapGraph *m, Player *p) {
    if (!n->visits) {
        p->addItem("my paperwork");
        return "Your paperwork waits on your desk, the one part of this room you sometimes claim. You go ahead and grab it.\n\n* Paperwork added to inventory. *";
    }

    return "";
}

string trail(StoryNode *n, MapGraph *m, Player *p) {
    if (!p->checkFlag("wizardHat")) {
        return "Hold on, you forgot your hat! You can't be seen without your hat!";
    } else if (n->connections.size() == 1) {
        m->addArc(n->title, "town.edge", "Go into town.");
    }

    return "";
}


// Creates story subgraph.
void createGame(MapGraph *graph) {
    const string CONTINUE = "(Continue.)";
    const string RETURN = "(Return.)";
    const string EXIT = "(Exit.)";

    // Opening
    graph->addVertex(new StoryNode(
        "You live atop a forested hill, in a small and cluttered home. You live alone, with the exception of Simon, a three-legged cat.\n\nAt the base of the hill is a town, and in this town is your potion shop. You are the potion master, the Alchemist.\n\nIt is early morning, and you are awoken by Simon's three paws.",
        "START"
    ));

    // THE HILL
    // Alchemist's home
    graph->addVertex(new StoryNode(
        bedroom,
        "Your bedroom is draped in quilts, tapestries, and rugs. It is not a very organized space, but you need to keep a mess somewhere to keep the rest of your life in order.",
        "hill.house.bedroom"
    ));
    
    graph->addVertex(new StoryNode(
        bedroomHat,
        "You can't leave without your hat. Bright pink and sparkling. Patched and worn. Embroidered and tasteful. Wide-rimmed. Glass beads dangle off the edge, each bead handcrafted by your friend, the Illusionist.",
        "hill.house.bedroom.hat"
    ));
    
    graph->addVertex(new StoryNode(
        kitchen,
        "You enter the kitchen. You may have a shop dedicated to your potion activities, but that doesn't stop the business from spilling over. Your countertops are decorated with all kinds of science-y equipment.\n\nYou cook sometimes too, but it never comes out very good. It's important you keep on good terms with the town's baker.",
        "hill.house.kitchen"
    ));

    graph->addVertex(new StoryNode(
        errandList,
        "Your list reads:",
        "hill.house.kitchen.errandList"
    ));
    
    graph->addVertex(new StoryNode(
        livingRoom,
        "You enter the living room. You don't use this room much. It's fair to call it Simon's nest at this point.",
        "hill.house.living"
    ));

    // Outdoors
    graph->addVertex(new StoryNode(
        "Outside your home is shaded and green. Your home looks a little lop-sided.\n\nIn your yard rests the remains of many an attempt to garden. Alas, the Gardener lost all hope in you long ago.",
        "hill.house.exterior"
    ));

    graph->addVertex(new StoryNode(
        "Yep, they're dead.",
        "hill.house.exterior.garden"
    ));

    graph->addVertex(new StoryNode(
        trail,
        "A dirt path weaves through the trees between town and your home. Broken fence posts and lanterns lead the way, though the lights haven't been lit in years.",
        "hill.trail"
    ));

    // Hill location connections
    graph->addArc("START", "hill.house.bedroom", "Get up.");
    
    graph->addArc("hill.house.bedroom", "hill.house.bedroom.hat", "Don wizard hat.");
    graph->addArc("hill.house.bedroom", "hill.house.kitchen", "Enter kitchen.");

    graph->addArc("hill.house.bedroom.hat", "hill.house.bedroom", CONTINUE);
    
    graph->addArc("hill.house.kitchen", "hill.house.kitchen.errandList", "Check errand list.");
    graph->addArc("hill.house.kitchen", "hill.house.bedroom", "Enter bedroom.");
    graph->addArc("hill.house.kitchen", "hill.house.living", "Enter living room.");
    graph->addArc("hill.house.kitchen", "hill.house.exterior", "Go outside.");

    graph->addArc("hill.house.kitchen.errandList", "hill.house.kitchen", CONTINUE);
    
    graph->addArc("hill.house.living", "hill.house.kitchen", "Enter kitchen.");
    graph->addArc("hill.house.living", "hill.house.exterior", "Go outside.");

    graph->addArc("hill.house.exterior", "hill.house.living", "Go inside.");
    graph->addArc("hill.house.exterior", "hill.house.exterior.garden", "Inspect garden.");
    graph->addArc("hill.house.exterior", "hill.trail", "Head into town.");

    graph->addArc("hill.house.exterior.garden", "hill.house.living", "Go inside.");
    graph->addArc("hill.house.exterior.garden", "hill.trail", "Head into town.");

    graph->addArc("hill.trail", "hill.house.exterior", "Go home.");

    // TOWN
    graph->addVertex(new StoryNode(
        "A stone fence surrounds the town. It isn't very high, nor thick, but it makes the town feel stronger, safer.\n\nYou never know what could happen.\n\nThe fence breaks for an open archway, here at the South entrance. Inside, townspeople wander about. You can hear the racket of children playing.",
        "town.edge", 
        "END"
    ));

}


// Filling the getGraph() method declared by story.h. This provides the driver with game data.
MapGraph *story::getGraph() {
    auto graph = new MapGraph();

    createTutorial(graph);
    createGame(graph);

    return graph;
}
