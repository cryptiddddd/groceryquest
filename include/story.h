// This file is the glue between a text adventure game and the rest of the framework.
#ifndef STORY_HEADER
#define STORY_HEADER

#include "graph.h"

namespace story {
    /**
     * Get a graph of the implemented story.
     * @return A graph loaded with a game's data.
    */
    MapGraph *getGraph();
}

#endif
