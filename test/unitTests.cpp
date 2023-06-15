// Tests for graph.h/.cpp

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

#include "graph.h"

using namespace std;


// Complex graph generator.
MapGraph *makeSampleGraph() {
    auto graph = new MapGraph();

    graph->addVertex(new StoryNode("", "A"));
    graph->addVertex(new StoryNode("", "B"));
    graph->addVertex(new StoryNode("", "C"));
    graph->addVertex(new StoryNode("", "D"));
    graph->addVertex(new StoryNode("", "E"));
    graph->addVertex(new StoryNode("", "F"));
    graph->addVertex(new StoryNode("", "G"));
    graph->addVertex(new StoryNode("", "H"));
    graph->addVertex(new StoryNode("", "I"));
    graph->addVertex(new StoryNode("", "J"));
    
    graph->addVertex(new StoryNode("", "X"));
    graph->addVertex(new StoryNode("", "Y"));
    graph->addVertex(new StoryNode("", "Z"));

    graph->addArc("A", "B", "ab"); graph->addArc("B", "A", "ba");
    graph->addArc("B", "C", "bc");
    graph->addArc("C", "D", "cd");
    graph->addArc("C", "G", "cg");
    graph->addArc("D", "A", "da");
    graph->addArc("D", "E", "de");
    graph->addArc("E", "F", "ef");
    graph->addArc("G", "H", "gh");
    graph->addArc("H", "F", "hf");
    graph->addArc("H", "I", "hi");
    graph->addArc("I", "J", "ij");
    graph->addArc("J", "H", "jh");

    graph->addArc("X", "Y", "xy"); graph->addArc("Y", "X", "yx");

    return graph;
}


// Unit tests

/// @brief Tests a graph's addVertex() method, returns pass/fail message.
string addVertexTest(MapGraph *graph) {
    graph->addVertex(new StoryNode("a node data", "A"));
    graph->addVertex(new StoryNode("b node data", "B"));
    graph->addVertex(new StoryNode("c node data", "C"));

    string result = graph->showVertices();
    if (result == "\"A\", \"B\", \"C\", ") {
        return "PASSED All nodes added to graph successfully.";
    }

    stringstream message;
    message << "FAILED Expected: '\"A\", \"B\", \"C\", '. graph.showVertices(): '" << result << "'";
    return message.str();
}

/// @brief Tests the getByTitle() method of a graph. 
string getByTitleTest(MapGraph *graph) {
    stringstream message;
    StoryNode *nodeB, *nodeX;

    // Get an existing node.
    try {
        nodeB = graph->getByTitle("B");
    } catch (VertexNotFound) {
        message << "FAILED getByTitle() could not retrieve existing vertex.\n";
    }

    if (nodeB->description == "b node data") {
        message << "PASSED Existing vertex successfully retrieved.\n";
    } else {
        message << "FAILED getByTitle() failed to retrieve existing vertex; did not throw error.\n";
    }

    // Get a non-existent node.
    try {
        nodeX = graph->getByTitle("X");
        message << "FAILED Did not throw error on finding nonexistent vertex. getByTitle() returned: " << nodeX->title << "";
    } catch (VertexNotFound) {
        message << "PASSED Threw error on finding nonexistent vertex.";
    }

    return message.str();
}

/// @brief Tests a graph's addArc() method, returns pass/fail messages.
string addArcTest(MapGraph *graph) {
    stringstream message;

    // Add arc from A->B.
    graph->addArc("A", "B", "ab");

    StoryNode *nodeA, *nodeB;
    nodeA = graph->getByTitle("A");
    nodeB = graph->getByTitle("B");
    // Check A's connections. First connection should be B.

    if (!nodeA->connections.size()) {
        message << "FAILED Node A has no connections, graph.addArc() must have failed.\n";
    } else if (nodeA->connections[0]->first != "ab" || nodeA->connections[0]->second != nodeB) {
        message << "FAILED Node A's connection to node B is not as expected. Expected name 'ab': '" << nodeA->connections[0]->first << "' Expected connected node title 'B': '" << nodeA->connections[0]->second->title << "'\n";
    } else {
        message << "PASSED Node A connects to B as expected.\n";
    }

    // Check B's connections. Should be empty.
    if (!nodeB->connections.size()) {
        message << "PASSED Node B has no connections, as expected.\n";
    } else {
        message << "FAILED Node B has " << nodeB->connections.size() << " connections, when it should have 0.\n";
    }

    // Add arc from A->C
    graph->addArc("A", "C", "ac");

    if (nodeA->connections.size() == 2) {
        message << "PASSED Node A holds multiple connections to nodes: '" << nodeA->connections[0]->second->title << "' and '" << nodeA->connections[1]->second->title << "'";
    } else {
        message << "FAILED Node A should have 2 connections (to B and C). Instead: " << nodeA->connections.size();
    }


    return message.str();
}

/// @brief Tests a graph's deleteArc() method. 
string deleteArcTest(MapGraph *graph) {
    stringstream message;

    StoryNode *nodeA = graph->getByTitle("A");

    // Remove arc AB
    graph->deleteArc("A", "B");

    if (nodeA->connections.size() == 1 && nodeA->connections[0]->first == "ac") {
        message << "PASSED Node A is no longer connected to node B, but is connected to node C.";
    } else {
        message << "FAILED Node A has " << nodeA->connections.size() << " connections.";
    }


    return message.str();
}


string shortestPathTest(MapGraph *sampleGraph, string sourceTitle) {
    pathMap *shortestPaths = sampleGraph->shortestPath(sourceTitle);

    stringstream results;

    results << "SHORTEST PATHS FROM NODE '"<< sourceTitle << "':\n";

    for (auto const& [nodeTitle, data] : (*shortestPaths)) {
        results << "\t";
        results << nodeTitle << " - ";
        if (data->first == sampleGraph->getSize() + 1) {
            results << "NO PATH";
        } else {
            results << data->first << " STEP ";
        }

        if (data->second != "") {
            results << "[PARENT '" << data->second << "']";
        }

        results << "\n";
    }

    delete shortestPaths;

    return results.str();
}



int main() {
    MapGraph *graph = new MapGraph();

    cout << addVertexTest(graph) << endl;
    cout << getByTitleTest(graph) << endl;
    cout << addArcTest(graph) << endl;
    cout << deleteArcTest(graph) << endl << endl;

    delete graph;

    // Build complex graph. Tree tests.
    MapGraph *compGraph = makeSampleGraph();

    cout << shortestPathTest(compGraph, "B") << endl; 
    cout << shortestPathTest(compGraph, "X") << endl; 

    delete compGraph;

    return 0;
}
