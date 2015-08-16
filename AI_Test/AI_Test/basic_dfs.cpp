#include <iostream>
#include <list>

using namespace std;

class Graph {
	int V;
	list<int> *adj;		//pointer to array of adjacency lists (is vertex adjacent)
	void isVisted(int v, bool visited[]);	//
};