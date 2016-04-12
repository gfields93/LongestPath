//
//  main.cpp
//  LongPath
//
//  Created by Glen Fields
//  on 11/24/15.
//  Copyright © 2015 gfields. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <string>
#include <map>
#include <algorithm>
#include <stack>
std::vector<int> GetNeighbors(int, const std::multimap<int, int>);

int main(int argc, const char * argv[]) {
    int numOfSets;
    int vertices;
    int edges;
    std::fstream is;
    is.open(argv[1]);
    if (!is.is_open()) {
        perror("Couldn't open file.\n");
        return 1;
    }
    std::ofstream os;
    os.open("output.txt");
    if (!os.is_open()) {
        perror("Couldn't open file.\n");
        return 1;
    }
    is >> numOfSets;
    for (int i = 0; i < numOfSets; ++i) {
        is >> vertices >> edges;
        std::map<std::pair<int, int>, int> EdgeNum;
        std::multimap<int, int> EdgeList;
        int x, y;
        for (int j = 0; j < edges; ++j) {
            is >> x >> y;
            EdgeNum.insert(std::make_pair(std::make_pair(x, y), j+1));
            EdgeList.insert(std::make_pair(x, y));
        }
        std::vector<std::pair<int, int> > vertexVec(vertices+1);
        std::fill(vertexVec.begin(), vertexVec.end(), std::make_pair(-1, -1));
        std::pair<int, int> max = std::make_pair(0, 1);
        for (int i = vertices; i >= 1; --i) {
            std::vector<int> neighbors = GetNeighbors(i, EdgeList);
            if(neighbors.size() == 0){
                vertexVec[i].first = 0;
                vertexVec[i].second = -2;
            }
            else{
                for (int j = 0; j < neighbors.size(); ++j) {
                    if (1 + vertexVec[neighbors[j]].first > vertexVec[i].first) {
                        vertexVec[i].first = 1 + vertexVec[neighbors[j]].first;
                        vertexVec[i].second = neighbors[j];
                        if (max.first < vertexVec[i].first) {
                            max.first = vertexVec[i].first;
                            max.second = i;
                        }
                    }
                }
            }
        }
        os << max.first << std::endl;
        int parent = max.second;
        int nextVertex = vertexVec[parent].second;
        while (vertexVec[parent].second != -2) {
            os << EdgeNum.at(std::make_pair(parent, nextVertex)) << " ";
            parent = nextVertex;
            nextVertex = vertexVec[parent].second;
        }
        os << std::endl << std::endl;
    }
    is.close();
    os.close();
    return 0;
}

//Find Neighbors of vertex
std::vector<int> GetNeighbors(int vertex, std::multimap<int, int> edgelist){
    std::vector<int> neighbors;
    std::multimap<int, int>::iterator i = edgelist.lower_bound(vertex);
    std::multimap<int, int>::iterator j = edgelist.upper_bound(vertex);
    for (; i != j; ++i) {
        neighbors.push_back(i->second);
    }
    return neighbors;
}