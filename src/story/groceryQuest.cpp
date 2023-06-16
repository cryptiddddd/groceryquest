/* This file is the script for GROCERY QUEST. Most of the file is writing. I suggest enabling softwrap. */
#include "story.h"

#include <string>

#include "game.h"
#include "graph.h"

using namespace std;


// Consts for consistency
const string CONTINUE = "(Continue.)";
const string RETURN = "(Return.)";
const string EXIT = "(Exit.)";

const string INCOMPLETE = "ERROR 404 CONVERSATION NOT FOUND.";

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


/**
 * Creates the Hill Zone for the graph. Separated into its own function for coherency and organization.
*/
void createHillZone(MapGraph *graph) {
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
}


// TOWN ZONE CALLBACKS

string townEdge(StoryNode *n, MapGraph *m, Player *p) {
    if (n->visits == 1) {
        n->description = "A stone fence surrounds the town. You stand at the open archway. Inside, townspeople wander about. You can hear the racket of children playing.";
    }

    return "";
}

string potionShop(StoryNode *n, MapGraph *m, Player *p) {
    if (!n->visits) return "It's your potion shop. You may not have built it from the ground up, but it is a labor of your love. You weren't alone in that renovation (and love) of course. A potion shop was in high demand long ago.";

    return "";
}

string potionShopBack(StoryNode *n, MapGraph *m, Player *p) {
    if (!p->hasItem("order for tailor")) return "You left an order in the fridge yesterday. It should be ready for delivery now.";

    return "";
}

string potionShopFridge(StoryNode *n, MapGraph *m, Player *p) {
    if (n->visits == 1) n->description = "Your work fridge is tidy, sorted vials and flasks pushed to the back. There's a soda in the door that you'll save for later.";

    return "";
}

string potionShopFridgeOrder(StoryNode *n, MapGraph *m, Player *p) {
    p->addItem("order for tailor");
    m->deleteArc("town.potionShop.back.fridge", n->title);
    return "* Tailor's order added to inventory *";
}

string townSquare(StoryNode *n, MapGraph *m, Player *p) {
    if (!n->visits) {
        return "The town buzzes with daily activities. Performers come and go throughout the day: musicians who play for the children, bands that play in the evenings. Your friend, the Illusionist, does shows after dark.";
    }

    return "";
}

string fountainFigure(StoryNode *n, MapGraph *m, Player *p) {
    if (n->visits == 1) n->description = "She reminds you of beloved pop idol Hatsune Miku.";
    return "";
}

string vendors(StoryNode *n, MapGraph *m, Player *p) {
    if (!n->visits) return "The Gardener at her stand waves you over.";
    return "";
}

string artStudio(StoryNode *n, MapGraph *m, Player *p) {
    if (!n->visits) return "The studio reminds you of home. It's colorful, and a little messy.";
    return "";
}

string bakery(StoryNode *n, MapGraph *m, Player *p) {
    if (n->visits == 1) n->description = "The bakery is quieter than usual. You can actually appreciate the pastel walls and floral finish.";
    return "";
}

string tailor(StoryNode *n, MapGraph *m, Player *p) {
    if (n->visits == 1) n->description = "The Tailor's shop is as clean and quiet as always. Clothing is hung neatly along the walls, or folded on tables. The Jeweler sits at the counter, concentrated on tiny pieces of wire.";

    return "";
}

string tailorJeweler(StoryNode *n, MapGraph *m, Player *p) {
    if (n->connections.size() == 1 && p->hasItem("order for tailor")) {
        m->addArc(n->title, "town.tailor.jeweler.order", "I have an order..."); // Add the option if it is not there yet, and the player has the item.
    } else if (n->connections.size() == 2 && p->checkFlag("tailorDelivery")) {
        m->deleteArc(n->title, "town.tailor.jeweler.order"); // Remove the option if there are two options and if the delivery has been.
    }

    return "";
}

string tailorJewelerAfter(StoryNode *n, MapGraph *m, Player *p) {
    if (!n->visits) return "He looks at you, shrugs. Says it's alright.";

    return "\"Haven't you asked me that already?\"";
}

string tailorOrder(StoryNode *n, MapGraph *m, Player *p) {
    p->addFlag("tailorDelivery");
    p->removeItem("order for tailor");

    // Reconfigure connections.
    StoryNode *tailorNode = m->getByTitle("town.tailor");
    tailorNode->connections[0]->second = m->getByTitle("town.tailor.jeweler.after");

    return "* Tailor's order removed from inventory *";
}


void createTownZone(MapGraph *graph) {
    // TOWN
    // Town edge
    graph->addVertex(new StoryNode(
        townEdge,
        "A stone fence surrounds the town. It isn't very high, nor thick, but it makes the town feel stronger, safer.\n\nYou never know what could happen.\n\nThe fence breaks for an open archway, here at the South entrance. Inside, townspeople wander about. You can hear the racket of children playing.",
        "town.edge"
    ));

    // Potion shop
    graph->addVertex(new StoryNode(
        potionShop,
        "The shop floor is polished and wooden. Most details are wooden. The counter is pristine, and ready for any visitor. Just behind the counter is your stock of herbs, animal hair, teeth, live lizards, and gift bags.",
        "town.potionShop"
    ));

    graph->addVertex(new StoryNode(
        potionShopBack,
        "You enter the back room. This is your workspace, though you won't need it very much today. You keep it more orderly here than you do in your home.",
        "town.potionShop.back"
    ));

    graph->addVertex(new StoryNode(
        potionShopFridge, 
        "An order for the Tailor sits on the middle shelf: bundled vials of potions, jewel tones and viscous. The customer's title is marked neatly on a bright pink tag, and yes you are proud of your cute packaging.",
        "town.potionShop.back.fridge"
    ));

    graph->addVertex(new StoryNode(
        potionShopFridgeOrder,
        "You grab the order for the Tailor.",
        "town.potionShop.back.fridge.order"
    ));

    // Town square
    graph->addVertex(new StoryNode(
        townSquare,
        "You stand at the edge of the town's square. Red and grey bricks are laid underfoot. Small crowds of children squawk together.\n\nAround the square sit vendor stands and tents. The town's shops branch out to the West, with the exception of your potion shop. That sits to the East, beside the Bookkeeper's tower.",
        "town.square"
    ));

    graph->addVertex(new StoryNode(
        "A stone fountain sits along the edge of town square.\n\nWishes made at this fountain do come true. Only two gold coins shimmer under the water's surface.\n\nBeside the fountain stands a figure, a life-sized music box.",
        "town.square.fountain"
    ));

    graph->addVertex(new StoryNode(
        fountainFigure,
        "Nobody knows who put her here. She stands beside the fountain on a crystal disk, and music hums from beneath her feet. The song is familiar to you.\n\nShe looks like a young woman, maybe your age (younger, if you're honest). Her eyes are closed, her head tipped aside in concentration. The instrument in her hands resembles a lute, though thinner and sleeker. Her fingers slide along the instrument's neck and pluck invisible strings.\n\nSome kind of antennae pokes out from atop her head, pointed straight up. It flickers like a firefly.",
        "town.square.fountain.figure"
    ));

    // Vendors
    graph->addVertex(new StoryNode(
        vendors,
        "The market vendors tend to their goods, chatting with one another, and interested customers.",
        "town.square.vendors"
    ));

    graph->addVertex(new StoryNode(
        "You approach the Gardener at her stand. She hands you a stalk of celery.\n\n\"It's good for you,\" she says.",
        "town.square.vendors.veggie"
    ));

    graph->addVertex(new StoryNode(
        "error 404: conversation not found",
        "town.square.vendors.veggie.404"
    ));

    // Main stree + shops
    graph->addVertex(new StoryNode(
        "The main street is a little quieter than the town square.",
        "town.mainStreet"
    ));

    // Art studio
    graph->addVertex(new StoryNode(
        artStudio,
        "The art studio's covered entrance space sits just outside the building. A woven sheet stretches overhead, providing shade. Woven seats and benches line the space.",
        "town.artStudio"
    ));

    graph->addVertex(new StoryNode(
        "error 404: artistic description not found.",
        "town.artStudio.inside"
    ));

    // Bakery
    graph->addVertex(new StoryNode(
        bakery,
        "You enter the bakery, and it's surprisingly quiet today. One of the Baker's many assistants stands behind the counter, scoring dough.",
        "town.bakery"
    ));

    graph->addVertex(new StoryNode(
        "The assistant doesn't even look up from their loaf.\n\n\"The Baker is out,\" they tell you.",
        "town.bakery.assistant"
    ));

    graph->addVertex(new StoryNode(
        "\"Don't worry about it.\"",
        "town.bakery.assistant.reply"
    ));

    // Tailor's
    graph->addVertex(new StoryNode(
        tailor,
        "The door to the Tailor's shop is noticeably intricate. The wood is carved with delicate contours; the small window is stained glass, patterns that seem to weave together. The shapes remind you of a hummingbird, but you can't name why.\n\nYou enter. Inside is cool and calm. Rich and cool browns dominate the room, despite the color and detail of clothing.\n\nAt the shop's front counter sits a young man, you recognize him as the Jeweler. He glances up to you, and then back to his tools.\n\nIt isn't unfriendly, but a hello would be nice.",
        "town.tailor"
    ));

    graph->addVertex(new StoryNode(
        tailorJeweler,
        "You approach the counter and wave hello.\n\nThe Jeweler puts his tools down, and gives you his full attention. He even flips his hair aside to get it out of his eyes, how considerate.\n\n\"Welcome. How can I help?\"",
        "town.tailor.jeweler"
    ));

    graph->addVertex(new StoryNode(
        tailorOrder,
        "\"Oh, right. We're expecting that.\"\n\nHe leans over and reaches beneath the counter. He pulls out a ticket.\n\nYou trade him the ticket for the order you grabbed earlier, which he tucks back beneath the counter.",
        "town.tailor.jeweler.order"
    ));

    graph->addVertex(new StoryNode(
        tailorJewelerAfter,
        "You ask the Jeweler how his day is going.",
        "town.tailor.jeweler.after"
    ));

    graph->addVertex(new StoryNode(
        "error 404: artistic description not found.",
        "town.tailor.clothes"
    ));


    // Making connections

    graph->addArc("town.edge", "town.potionShop", "Visit potion shop.");
    graph->addArc("town.edge", "town.square", "Enter town square.");
    graph->addArc("town.edge", "hill.trail", "Head home.");

    graph->addArc("town.potionShop", "town.potionShop.back", "Enter back room.");
    graph->addArc("town.potionShop", "town.edge", "Leave shop.");

    graph->addArc("town.potionShop.back", "town.potionShop.back.fridge", "Open fridge.");
    graph->addArc("town.potionShop.back", "town.potionShop", "Exit back room.");

    graph->addArc("town.potionShop.back.fridge", "town.potionShop.back.fridge.order", "Grab order.");
    graph->addArc("town.potionShop.back.fridge", "town.potionShop.back", RETURN);

    graph->addArc("town.potionShop.back.fridge.order", "town.potionShop.back.fridge", CONTINUE);

    graph->addArc("town.square", "town.square.fountain", "Go to the fountain.");
    graph->addArc("town.square", "town.mainStreet", "Visit shops.");
    graph->addArc("town.square", "town.square.vendors", "Visit vendor stands.");
    graph->addArc("town.square", "town.edge", "Get outta town.");
    
    graph->addArc("town.square.fountain", "town.square.fountain.figure", "Inspect figure.");
    graph->addArc("town.square.fountain", "town.square", RETURN);

    graph->addArc("town.square.fountain.figure", "town.square.fountain", RETURN);

    graph->addArc("town.square.vendors", "town.square.vendors.veggie", "Approach Gardener.");
    graph->addArc("town.square.vendors", "town.square", RETURN);

    graph->addArc("town.square.vendors.veggie", "town.square.vendors.veggie.404", "Thank her.");
    graph->addArc("town.square.vendors.veggie", "town.square.vendors.veggie.404", "Turn down her offer.");

    graph->addArc("town.square.vendors.veggie.404", "town.square.vendors", RETURN);

    graph->addArc("town.mainStreet", "town.artStudio", "Visit the art studio.");
    graph->addArc("town.mainStreet", "town.bakery", "Visit the bakery.");
    graph->addArc("town.mainStreet", "town.tailor", "Visit the Tailor's.");
    graph->addArc("town.mainStreet", "town.square", "Go to town square.");

    graph->addArc("town.artStudio", "town.artStudio.inside", "Enter building.");
    graph->addArc("town.artStudio", "town.mainStreet", EXIT);

    graph->addArc("town.artStudio.inside", "town.artStudio", RETURN);

    graph->addArc("town.bakery", "town.bakery.assistant", "Talk to the assistant.");
    graph->addArc("town.bakery", "town.mainStreet", "Leave bakery.");

    graph->addArc("town.bakery.assistant", "town.bakery.assistant.reply", "Okay, sorry.");

    graph->addArc("town.bakery.assistant.reply", "town.bakery", CONTINUE);

    graph->addArc("town.tailor", "town.tailor.jeweler", "Talk to the Jeweler.");
    graph->addArc("town.tailor", "town.tailor.clothes", "Inspect clothing.");
    graph->addArc("town.tailor", "town.mainStreet", "Leave shop.");

    graph->addArc("town.tailor.clothes", "town.tailor", RETURN);

    graph->addArc("town.tailor.jeweler", "town.tailor", "Nevermind.");

    graph->addArc("town.tailor.jeweler.order", "town.tailor", "Pleasure doing business with you.");
    
    graph->addArc("town.tailor.jeweler.after", "town.tailor.clothes", "Inspect clothing.");
    graph->addArc("town.tailor.jeweler.after", "town.mainStreet", "Leave shop.");
}


// Creates story subgraph.
void createGame(MapGraph *graph) {
    createHillZone(graph);
    createTownZone(graph);

}


// Filling the getGraph() method declared by story.h. This provides the driver with game data.
MapGraph *story::getGraph() {
    auto graph = new MapGraph();

    createTutorial(graph);
    createGame(graph);

    return graph;
}
