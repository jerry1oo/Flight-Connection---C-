/*Name: Jeremiah Johnson
 *Date: 3/6/2019
 *Overview: This file contains the Graph class and the Nodes it uses.
 *          Pathfinder,socialgathering function modified from starter as well
 *          as an added function loadFromFilePair that makes from and to pairs.
 *Assignment: Project 3
 */


#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <list>
#include <string>

using namespace std;
// My node struct with a set of friends it has and its ID
struct Node{
    std::unordered_set<Node*> FBF;
    int ID;
    int friends;
    Node(int id): ID(id) {}
};

//Node's for Airports
struct NodeF{
    std::unordered_map<NodeF*,int> Dest;
    std::string Orig;
    int D = 32767; // largest int val
    NodeF(std::string O): Orig(O) {}
};
// Pair struct for Priority Queue
struct Pair{
    NodeF* F;
    int S;
    Pair(NodeF* f, int s): F(f), S(s){}
    bool operator<(Pair const &) const;
};
// fuctions for sort
bool LessThanF(Node* x, Node* y);
bool LessThanID(Node* x, Node* y);

class Graph {
    protected:
        // my Graph representation for FB
        std::unordered_map<int, Node*> ALLN;
        // my Graph representation for flightpath
        std::unordered_map<std::string, NodeF*> ALLNFL;
    public:
        Graph(void);

        ~Graph(void);
        // Load in Airports
        bool loadFL(const char* in_filename);

        bool loadFromFile(const char* in_filename);
        //Get pairs of paths we want to check
        vector<std::pair<int,int>> loadFromFilePair(const char* in_filename);
        vector<std::pair<string,string>> loadFromFilePairFL(const char* in_filename);
        //Get shortest path
        list<int> pathfinder(int from,int to);
        //Get K-Core
        vector<Node*> socialgathering(const int& k);
        // find shortest weighted path between 2 Airports
        list<string> flightpath(string from , string to);


};

#endif  // GRAPH_HPP
