#include "graph.h"

#include <algorithm>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

#include "player.h"

using std::pair, std::string, std::stringstream;


string callbacks::eCB(StoryNode *n, MapGraph *g, Player *p) { return ""; }


// Exception implementations.

VertexTitleConflict::VertexTitleConflict(string title) { hint = title; }
string VertexTitleConflict::what() { return hint; }

VertexNotFound::VertexNotFound(string title) { hint = title; }
string VertexNotFound::what() { return hint; }


// StoryNode implementations.

StoryNode::StoryNode(string (* _callback)(StoryNode *, MapGraph *, Player *), string _description, string _title, string _tag) { // O(1)
    callback = _callback;
    description = _description;
    title = _title;
    tag = _tag;
}

StoryNode::StoryNode(string _description, string _title, string _tag) { // O(1)
    callback = callbacks::eCB;
    description = _description;
    title = _title;
    tag = _tag;
}

StoryNode::~StoryNode() { // O(m)
    for (auto pathData : connections) { // O(m)
        delete pathData;
    }
    connections.clear();
}

void StoryNode::addPath(StoryNode *branch, string text) { // O(1)
    // I will allow duplicates.
    connections.push_back(new pair<string, StoryNode *>(text, branch)); // O(1)
}

string StoryNode::getPathMenu() { // O(m)
    if (!connections.size()) { // O(1)
        return "NO PATHS FORWARD.";
    }

    stringstream menu;
    int i = 0;

    for (auto pathData : connections) { // O(m)
        // +1 to keep options > 0.
        menu << "\n[" << i + 1 << "] " << pathData->first;
        i++;
    }

    return menu.str();
}

void StoryNode::removePath(StoryNode *branch) { // O(m)
    for (int i = 0; i < connections.size(); ++i) {
        if (connections[i]->second == branch) {
            // Delete connection, and remove from list.
            delete connections[i];
            connections.erase(connections.begin() + i);
            return;
        }
    }
    // If no matching node is found, I've elected to do nothing.
}


// GraphMap implementations.

MapGraph::MapGraph() { // O(1)
    size = 0;
}

MapGraph::~MapGraph() { // O(n)
    for (auto const& [label, node] : vertices) { // O(n)
        delete node;
    }
    vertices.clear();
}

void MapGraph::addArc(StoryNode *source, StoryNode *destination, string text) { // O(1)
    source->addPath(destination, text);
}

void MapGraph::addArc(string source, string destination, string text) { // O(1)
    StoryNode *sourceNode = getByTitle(source);
    StoryNode *destinationNode = getByTitle(destination);

    sourceNode->addPath(destinationNode, text);
}

void MapGraph::deleteArc(string source, string destination) { // O(m), m being the node's connections rather than graph size.
    StoryNode *sourceNode = getByTitle(source);
    StoryNode *destinationNode = getByTitle(destination);

    sourceNode->removePath(destinationNode);
}

void MapGraph::addVertex(StoryNode *newVertex) { // O(1)
    // Check this doesn't override. Improper check because a value is going to be created here anyhow.
    if (vertices[newVertex->title] != 0) {
        throw VertexTitleConflict(newVertex->title);
    }

    // Add and increase size.
    vertices[newVertex->title] = (newVertex);
    ++size;
}

StoryNode *MapGraph::getByTitle(string title) { // O(1)
    // Check it.
    if (vertices.find(title) == vertices.end()) throw VertexNotFound(title);
    
    // Grab it.
    return vertices[title];
}

pathMap *MapGraph::shortestPath(string nodeTitle) { // O(2n * m) ??
    std::map<string, bool> visited; // Tracks which nodes have been visited
    
    // This map holds path info. Node name mapped to the total distance and parent node.
    auto paths = new pathMap;
    for (auto items : vertices) { // O(n)
        // "Infinite" distance (larger than anything expected) and no parent.
        (*paths)[items.first] = new pair<int, string>(size + 1, "");
    }

    (*paths)[nodeTitle]->first = 0;

    // Prepare and run Dijkstra's algorithm
    StoryNode *current;
    std::queue<StoryNode *> nodeQueue;
    nodeQueue.push(vertices[nodeTitle]);

    while (!nodeQueue.empty()) { // O(n)
        current = nodeQueue.front(); nodeQueue.pop(); // Retrieve next node.

        // Skip if it has been visited.
        if (visited[current->title]) { // O(1)
            continue;
        }

        // Add unvisited connections to queue.
        for (auto connection : current->connections) { // O(m)
            StoryNode *node = connection->second;
            // Skip visited--possibly redundant, but this is safety.
            if (visited[node->title]) {
                continue;
            }
            
            // New distance based on parent node's distance.
            int distance = (*paths)[current->title]->first + 1;

            // Compare.
            if ((*paths)[node->title]->first > distance) {
                // Update distance and parent.
                (*paths)[node->title]->first = distance;
                (*paths)[node->title]->second = current->title;
            }

            nodeQueue.push(node);
        }

        // Mark as visited.
        visited[current->title] = true;
    }

    return paths;
}

string MapGraph::showVertices() { // O(n)
    stringstream message;

    // Comma-separated list of vertices, each one's title in quotes.
    for (auto const& [label, _] : vertices) { // O(n)
        message << "\"" << label << "\", ";
    }

    return message.str();
}
