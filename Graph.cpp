/*Name: Jeremiah Johnson
 *Date: 3/6/2019
 *Overview: This file contains all functions of the Graph.hpp
 *          Pathfinder function modified from starter as well as an added 
 *          function loadFromFilePair that makes from and to pairs.          
 *Assignment: Project 3
 */  
#include "Graph.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <list>
#include <string>
using namespace std;

Graph::Graph(void)
    : ALLN(0) {}

    Graph::~Graph(void) { // delete all Nodes
        for (auto itr : ALLN) {
            delete itr.second;
        }
    }

/* Add a node to the graph representing person with id idNumber and add a connection between two nodes in the graph. */



/* Read in relationships from an inputfile to create a graph */

bool Graph::loadFromFile(const char* in_filename) {
    ifstream infile(in_filename);

    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ' ')) break;
            record.push_back(s);
        }

        if (record.size() != 2) {
            continue;
        }
        // Get ID's for Graph
        int f0 = std::stoi(record[0]); 
        int f1 = std::stoi(record[1]);
        // Check if already in Graph
        std::unordered_map<int,Node*>::const_iterator find0 = ALLN.find (f0);
        if(find0 == ALLN.end()){
            ALLN[f0] = new Node(f0);
        }
        std::unordered_map<int,Node*>::const_iterator find1 = ALLN.find (f1);
        if(find1 == ALLN.end()){
            ALLN[f1] = new Node(f1);
        }
        // Add edges between nodes 
        ALLN[f0]->FBF.insert(ALLN[f1]);
        ALLN[f1]->FBF.insert(ALLN[f0]);

    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }

    infile.close();
    return true;
}

bool Graph::loadFL(const char* in_filename) {
    ifstream infile(in_filename);

    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ' ')) break;
            record.push_back(s);
        }

        if (record.size() != 3) {
            continue;
        }
        // Get Airports for Graph
        int D = std::stoi(record[2]); 
        auto find0 = ALLNFL.find(record[0]);
        if(find0 == ALLNFL.end()){
            ALLNFL[record[0]] = new NodeF(record[0]);
        }
        auto find1 = ALLNFL.find(record[1]);
        if(find1 == ALLNFL.end()){
            ALLNFL[record[1]] = new NodeF(record[1]);
        }
        // Add edges between nodes 
        ALLNFL[record[0]]->Dest[ALLNFL[record[1]]] = D;
        ALLNFL[record[1]]->Dest[ALLNFL[record[0]]] = D;
        
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }

    infile.close();
    return true;
}
vector<std::pair<int,int>> Graph::loadFromFilePair(const char* in_filename) {
    ifstream infile(in_filename);
    vector<std::pair<int,int>> pairs;
    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ' ')) break;
            record.push_back(s);
        }

        if (record.size() != 2) {
            continue;
        }

        // make pairs of 2 ID's of from and to 
        int f0 = std::stoi(record[0]); 
        int f1 = std::stoi(record[1]);
        std::pair<int,int> temp(f0,f1);	   
        pairs.push_back(temp);   
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        pairs.clear();
        return pairs;
    }

    infile.close();
    return pairs;
}


vector<std::pair<string,string>> Graph::loadFromFilePairFL(const char* in_filename) {
    ifstream infile(in_filename);
    vector<std::pair<string,string>> pairs;
    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ' ')) break;
            record.push_back(s);
        }

        if (record.size() != 2) {
            continue;
        }

        // make pairs of 2 Airtports of from and to 
        std::pair<string,string> temp(record[0],record[1]);	   
        pairs.push_back(temp);   
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        pairs.clear();
        return pairs;
    }

    infile.close();
    return pairs;
}

/* Implement pathfinder*/

list<int> Graph::pathfinder(int from, int to) {
    // check if inputs exist
    auto Nf = ALLN.find(from);
    auto Nt = ALLN.find(to);
    if(Nf == ALLN.end() || Nt == ALLN.end()){
        std::list<int> path;
        return path;	
    }

    // get the nodes with these ID's
    Node* F = ALLN[from];
    Node* T = ALLN[to];

    if(from == to){
        std::list<int> path;
        auto Find = F->FBF.find(T);
        if(Find != F->FBF.end()){
            path.push_front(T->ID);
            return path;			
        }
        else{
            return path;
        }
    }

    // All nodes to search
    std::queue<Node*> Q;
    // All edges we see
    std::unordered_map<Node*, Node*> AP;
    Q.push(F);
    AP[F] = nullptr;
    // while we still have nodes to visit
    while(!Q.empty()){
        // get next node in queue and check if its what we want
        Node* curr = Q.front();
        Q.pop();
        if(curr->ID == to){
            break;
        }
        // Node is not what we want so we check all its friends
        for(auto t : curr->FBF){
            // dont add an edge if we have already visited this ID
            auto find = AP.find(t);
            if(find == AP.end()){
                Q.push(t);
                AP[t] = curr;
            }
        }
    }
    // list of the path between 2 nodes
    std::list<int> path;
    // check if we found the node we were looking for
    auto find = AP.find(T);
    if(find == AP.end()){
        return path;
    }
    // while we havent reached end of path, add this ID to shortest path
    Node* curr = T;
    while(curr != nullptr){
        path.push_front(curr->ID);
        curr = AP[curr];
    }
    return path;  
}

/* Implement social gathering*/
//TODO
bool LessThanF(Node* x, Node* y){
    return x->friends < y->friends;
}
bool LessThanID(Node* x, Node* y){
    return x->ID < y->ID;
}
vector<Node*> Graph::socialgathering(const int& k) {
    vector<Node*> map;
    //get all sizes of Map
    for(auto it : ALLN){
        it.second->friends = it.second->FBF.size();
        map.push_back(it.second);
    }
    //Sort Map by number of friends then erase all with less tham k friends
    sort(map.begin(),map.end(),LessThanF);
    while(map[0]->friends < k){
        for(auto i : map[0]->FBF){
            i->FBF.erase(map[0]);
            i->friends--;
        }
        map.erase(map.begin());
        if(map[0]->friends > k){
            sort(map.begin(),map.end(),LessThanF);//only sort when nessesary
        }
    }
    return map;
}

list<string> Graph::flightpath(string from , string to){

    // Reset Dist on Node
    for(auto it : ALLNFL){
        it.second->D = 32767;
    }
    // Check If Node exist
    auto Nf = ALLNFL.find(from);
    auto Nt = ALLNFL.find(to);
    if(Nf == ALLNFL.end() || Nt == ALLNFL.end()){
        std::list<string> path;
        return path;	
    }

    // get the nodes with these ID's
    NodeF* F = ALLNFL[from];
    NodeF* T = ALLNFL[to];

    if(from == to){
        std::list<string> path;
        auto Find = F->Dest.find(T);
        if(Find != F->Dest.end()){
            path.push_front(T->Orig);
            return path;			
        }
        else{
            return path;
        }
    }

    // All nodes to search in order of shortest dist
    std::priority_queue<Pair> Q;
    // All edges we see    
    std::unordered_map<NodeF*, NodeF*> AP;
    //F->D = 0;
    Pair temp(F,0);	   
    Q.push(temp);
    AP[F] = nullptr;
    // while we still have nodes to visit
    while(!Q.empty()){
        // get next node in queue and check if its what we want
        Pair curr = Q.top();
        Q.pop();
        if(curr.F->Orig == to){
            break;
        }
        // Node is not what we want so we check all its friends
        for(auto t : curr.F->Dest){
            // dont add an edge if we have already visited this ID
            auto find = AP.find(t.first);
            if(find == AP.end()){
                int c = curr.F->Dest[t.first] + curr.S;
                if(c < t.first->D){
                    t.first->D = c;
                    Pair temp1(t.first,c);	   
                    Q.push(temp1);
                    AP[t.first] = curr.F;
                }
            }
        }
    }
    // list of the path between 2 nodes
    std::list<string> path;
    // check if we found the node we were looking for
    auto find = AP.find(T);
    if(find == AP.end()){
        return path;
    }
    // while we havent reached end of path, add this ID to shortest path
    NodeF* curr = T;
    while(curr != nullptr){
        path.push_front(curr->Orig);
        curr = AP[curr];
    }
    return path;  



}
// for priority queue
bool Pair::operator<(Pair const & other) const{
    return S > other.S;
}
