#ifndef GRAPH_HEADER
#define GRAPH_HEADER

#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "player.h"

using std::string;


class MapGraph;
struct StoryNode;

// How a directioned edge is stored. First member is the descriptive text ("Turn left.", "Go North.").
typedef std::pair<string, StoryNode *> path;

// Map of paths for use in MapGraph.shortestPath()
typedef std::map<string, std::pair<int, string> *> pathMap;


namespace callbacks {
    // Empty callback
    string eCB(StoryNode *n, MapGraph *m, Player *p);
}


/// @brief Error thrown when a graph already has a node of the given name.
class VertexTitleConflict : public std::exception {
    private: 
        string hint;

    public:
        VertexTitleConflict(string title);
        string what();
};


/// @brief Error thrown when a graph does not have a desired node.
class VertexNotFound : public std::exception {
    private:
        string hint;

    public: 
        VertexNotFound(string title);
        string what();
};


/// @brief Single node.
struct StoryNode {
    string (* callback)(StoryNode *, MapGraph *, Player *);
    std::vector<path *> connections; // Vector, so that player input accesses desired node by index.

    string title; // Short, unique.
    string description; // Long, doesn't need to be unique, likely will be.
    string tag; // Short, doesn't need to be unique.

    int visits = 0;

    /**
     * Constructs node.
     * @param _callback Node's callback.
     * @param _description The node's narration.
     * @param _title Unique title for the node, may be omitted when used without a graph. 
     * @param _tag Optional tag for the node. Non-unique. Example: "END" to mark an ending.
     * @overload One overload that constructs node with empty callback.
    */
    StoryNode(string (* _callback)(StoryNode *, MapGraph *, Player *), string _description, string _title, string _tag = "");
    
    /**
     * Constructs node with empty callback.
     * @param _description The node's narration.
     * @param _title Unique title for the node, may be omitted when used without a graph. 
     * @param _tag Optional tag for the node. Non-unique. Example: "END" to mark an ending.
    */
    StoryNode(string _description, string _title, string _tag = "");

    /**
     * Destructs node and its connections.
    */
    ~StoryNode();

    /**
     * Adds a new destination to the current node.
     * @param branch The new connection.
     * @param text The text to display for the player.
     * @return No return value.
    */
    void addPath(StoryNode *branch, string text);

    /**
     * Get all connections' reference messages.
     * @return A menu of options for the player to refer to.
    */
    string getPathMenu();

    /**
     * Removes a branch from the current node's connections.
     * @throws No error thrown if the desired branch isn't found.
     * @param branch The old connection to break.
     * @return No return value.
    */
    void removePath(StoryNode *branch);
};


/// @brief MapGraph class, manages story map.
class MapGraph {
    private:
        int size;
        std::map<string, StoryNode *> vertices;

    public:
        /**
         * Constructs an empty graph.
        */
        MapGraph();

        /**
         * Safely destructs graph and graph data.
        */
        ~MapGraph();

        /**
         * Adds a directioned arc between two nodes on the graph, from source to destination.
         * @param source The starting node.
         * @param destination The ending node.
         * @param text Narration for the connection.
         * @overload One overload to make a connection by node title, rather than the node object itself.
         * @return No return value.
        */
        void addArc(StoryNode *source, StoryNode *destination, string text = "");
        void addArc(string source, string destination, string text = "");

        /**
         * Removes an arc between two nodes of the given names.
         * @param source The title of the source node of the arc.
         * @param destination The title of the arc's destination.
         * @return No return value.
        */
        void deleteArc(string source, string destination);

        /**
         * Adds a vertex to the graph.
         * @param newVertex The new vertex to add to the graph.
         * @throws VertexTitleConflict if the given node's title conflicts with an existing vertex in the graph. 
         * @return No return value.
        */
        void addVertex(StoryNode *newVertex);

        /**
         * Gets a node by its unique title.
         * @param title Title of the desired node.
         * @throws VertexNotFound if node does not exist.
         * @return Pointer to the node with the desired title.
        */
        StoryNode *getByTitle(string title);

        /**
         * Get the size of graph.
         * @return The number of vertices in the graph.
        */
        inline int getSize() const { return size; } // O(1)

        /**
         * Calculates the shortest path to every (accessible) node from the given node.
         * @param nodeTitle The title of the source node.
         * @return A string describing the shortest paths from the given node to all other accessible nodes.
        */
        pathMap *shortestPath(string nodeTitle);

        /**
         * For debug purposes. Returns a string of all node titles and descriptions.
        */
        string showVertices();
};


#endif
