# Complexity Analysis

## Graph Analysis

As the graph structure is the main structure of the program, I want to keep an eye on its complexity and efficiency.


### `StoryNode` behavior:

| Method            | Complexity | Notes                                                     |
|:----------------  |:----------:|:----------------------------------------------------------|
| `StoryNode()`     | O(1)       | Both `StoryNode` constructors have the same complexity.
| `~StoryNode()`    | O(n)       | Where 'n' is the number of connections.
| `addPath()`       | O(1)       |
| `getPathMenu()`   | O(n)       | 
| `getPathName()`   | O(n)       | 
| `removePath()`    | O(n)       |


### `MapGraph` behavior:

| Method            | Complexity | Notes                                                     |
|:----------------  |:----------:|:----------------------------------------------------------|
| `MapGraph()`      | O(1)       |
| `~MapGraph()`     | O(n)       | 
| `addArc()`        | O(1)       | Both overloads have the same complexity.
| `deleteArc()`     | O(m)       | 'm' being the relevant node's number of connections.
| `addVertex()`     | O(1)       | 
| `getByTitle()`    | O(1)       | I actually don't know what the efficiency for accessing a map is. I assumed average case from [Big-O cheat sheet](https://www.bigocheatsheet.com/).
| `arborescence()`  | O(3n * m)  | Uses shortest path, iterates through the information another time.
| `shortestPath()`  | O(2n * m)  |
| `showVertices()`  | O(n)       | Necessary to visit all nodes. 

Generally speaking, I'm not super satisfied by my time complexities.

One optimization I have in mind is for my node. Currently connections are stored in a vector, however it could help to instead keep them in a map. If I could access a connection based on the node it points toward, this would save time on deleting and getting the associated name. However, that would create more complexity when the `Game` class accesses the player's next selected node.
