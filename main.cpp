#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <array>
#include <algorithm>
#include <Eigen/Dense>
#include "response.h"

struct Restriction {
    int n;
    int max_distance;
    int max_degree;
};

const Restriction _5{5,2,2};
const Restriction _10{10,2,3};
const Restriction _50{50,2,7};
const Restriction _3250{3250,2,57};

const auto& use = _10;

/*
 * General info:
 * establishing an edge has value 2
 * accepting an established edge has value 1
 * doing nothing has value 0
 * rejecting an edge has value -1
 *
 */

template<typename T, typename S>
void randomGraph(T &graph, S &degrees, const unsigned int max_degree) {

    int tmp = 0;

    for (int i = 0; i < min_side(graph); i++) {
        for (int j = 0; j < max_degree; j++) {
            tmp = rand() % min_side(graph);
            if (graph(i,tmp) == 0 && tmp != i) {
                graph(i,tmp) = 2;
                graph(tmp,i) = 1;
                degrees[i]++;
                degrees[tmp]++;
            }

            int a = graph(i,tmp);
            int b = graph(tmp,i);

            //std::cout << i << " " << tmp << " " << a << " " << b << std::endl;
        }
    }
}

// Are all nodes happy?
template<typename T, typename S>
int satisfied(T &graph, T &distances, const unsigned int max_distance, S &degrees, const unsigned int max_degree) {
    // Are distances < max_distance?
    for (int i = 0; i < min_side(graph); i++) {
        for (int j = 0; j < min_side(graph); j++) {
            if (i == j) {
                continue;
            }
            if (distances(i,j) == 0 || distances(i,j) > max_distance) {
                return 0;
            }
        }
    }

    // Are there rejected edges?
    if(graph.minCoeff() < 0)
        return 0;

    if(*std::max_element(degrees.begin(), degrees.end()) > max_degree)
        return 0;

    return 1;

}

// chooses an unsatisfied player and improves his situation
template<typename T, typename S>
void performImprovement(int n, int d, int k, T &graph, T &distances, S &degrees) {
    int player = chooseUnsatisfiedNode(n, d, k, graph, distances, degrees);

    int tmp = degrees[player];

    // if the player accepted edges from nodes with too high degree, reject those edges
    if (rejectEdges(player, n, k, graph, degrees)) {
        // update the distances
        apsp(graph, distances);
        int tmp = degrees[player];
        //std::cout << "we rejected some edge" << std::endl;
    }

    // make sure the player reaches every node
    if (establishDistances(player, n, d, graph, distances, degrees)) {
        // if there are rejected edges from the player, delete them
        cleanRejectedEdges(player, n, graph, degrees);
        apsp(graph, distances);
    } else {
        cleanRejectedEdges(player, n, graph, degrees);
        int tmp = degrees[player];
        // if the distances were alright, reduce the players' degree by deleting incoming edges
        reduceDegree(player, n, k, graph, degrees);
        apsp(graph, distances);
    }
}

int main() {
    constexpr int n = use.n;

    auto seed = 1542200294;//time(NULL);//1542200294;//
    std::cout << "Seed: " << seed << std::endl << std::endl;
    srand(seed);

    Eigen::Matrix<int, n, n> graph;
    Eigen::Matrix<int, n, n> distances;
    std::array<int, n> degrees{};

    randomGraph(graph, degrees, use.max_degree);

    apsp(graph, distances);

    int counter = 0;

    while (!satisfied(graph, distances, use.max_distance, degrees, use.max_degree)) {
        counter++;
        if (counter % 1000 == 0) {
            std::cout << counter << std::endl;
        }

        performImprovement(n, use.max_distance, use.max_degree, graph, distances, degrees);
        apsp(graph, distances);
    }

    std::cout << "Adjacency matrix:" << std::endl;
    std::cout << graph << std::endl;

    std::cout << "Distance matrix:" << std::endl;
    std::cout << distances << std::endl;

    std::cout << "degrees:" << std::endl;
    std::ostream_iterator<int> out_it (std::cout," ");
    std::copy(degrees.cbegin(), degrees.cend(), out_it);

    std::cout << std::endl << "number of iterations: " << counter << std::endl;

    return 0;
}

