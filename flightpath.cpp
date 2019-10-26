/*Name: Jeremiah Johnson
 *Date: 3/6/2019
 *Overview: This file is the driver function of the flightpath function. Output
 *          is all the shortest weighted paths between 2 given destinations.
 *Assinment: Project 3
 */


#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Graph.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <list>
#include <string>

using namespace std;

void usage(char* program_name) {
    cerr << program_name << " called with incorrect arguments." << endl;
    cerr << "Usage: " << program_name
        << " friendship_pairs_file test_pairs_file output_file"
        << endl;
    exit(-1);
}

int main(int argc, char* argv[]) {

    if (argc != 4) {
        usage(argv[0]);
    }


    char* graph_filename = argv[1];
    char* pairs_filename = argv[2];
    char* output_filename = argv[3];

    ofstream out;
    out.open(output_filename);

    Graph FP;
    if(!FP.loadFL(graph_filename)){return 0;}
    // Get all pairs of paths we want to check
    vector<std::pair<string,string>> pairs = FP.loadFromFilePairFL(pairs_filename);
    if(pairs.empty()){return 0;}
    for(auto& p: pairs){
        //for each path, get the shortest weighted path and print
        list<string> path = FP.flightpath(p.first,p.second);
        auto space = "";
        for(auto& i: path){
            out << space << i;
            space = " ";
        }
        out << '\n';
    }

    out.close();
}

