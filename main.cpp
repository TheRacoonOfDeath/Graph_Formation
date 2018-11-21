#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <array>
#include <Eigen/Dense>
#include "matrix.h"
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

// All Pair Shortest Path
template<typename T>
void apsp(T &inMat, T &outMat) {
    for (int i = 0; i < inMat.min_side(); i++) {
        for (int j = 0; j < inMat.min_side(); j++) {
            if (inMat[i][j] == 1 || (inMat[i][j] == 2 && inMat[j][i] == 1) || inMat[i][j] == -1) {
                outMat[i][j] = 1;
            } else {
                outMat[i][j] = 0;
            }
        }
    }

    for (int k = 0; k < inMat.min_side(); k++) {
        for (int i = 0; i < inMat.min_side(); i++) {
            for (int j = 0; j < inMat.min_side(); j++) {
                if (outMat[i][k] == 0 || outMat[k][j] == 0 || i == j) {
                    continue;
                }
                if (outMat[i][j] == 0 || outMat[i][j] > outMat[i][k] + outMat[k][j]) {
                    outMat[i][j] = outMat[i][k] + outMat[k][j];
                }
            }
        }
    }

}

template<typename T, typename S>
void randomGraph(T &graph, S &degrees, const unsigned int max_degree) {

    int tmp = 0;

    for (int i = 0; i < graph.min_side(); i++) {
        for (int j = 0; j < max_degree; j++) {
            tmp = rand() % graph.min_side();
            if (graph[i][tmp] == 0 && tmp != i) {
                graph[i][tmp] = 2;
                graph[tmp][i] = 1;
                degrees[i]++;
                degrees[tmp]++;
            }

            int a = graph[i][tmp];
            int b = graph[tmp][i];

            //std::cout << i << " " << tmp << " " << a << " " << b << std::endl;
        }
    }
}

// Are all nodes happy?
template<typename T, typename S>
int satisfied(T &graph, T &distances, const unsigned int max_distance, S &degrees, const unsigned int max_degree) {
    // Are distances < max_distance?
    for (int i = 0; i < graph.min_side(); i++) {
        for (int j = 0; j < graph.min_side(); j++) {
            if (i == j) {
                continue;
            }
            if (distances[i][j] == 0 || distances[i][j] > max_distance) {
                return 0;
            }
        }
    }

    // Are there rejected edges?
    for (auto &i : graph) {
        for (auto &j : i) {
            if (j == -1) {
                return 0;
            }
        }
    }

    // All degrees < max_degree
    for (auto &i : degrees) {
        if (i > max_degree) {
            return 0;
        }
    }

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

    auto seed = time(NULL);//1542200294;//
    std::cout << "Seed: " << seed << std::endl << std::endl;
    srand(seed);

    Matrix<int, n, n> graph, distances;
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

    graph.print("Adjacency matrix:");

    distances.print("Distance matrix:");

    std::cout << "degrees:" << std::endl;
    std::ostream_iterator<int> out_it (std::cout," ");
    std::copy(degrees.cbegin(), degrees.cend(), out_it);

    std::cout << std::endl << "number of iterations: " << counter << std::endl;

    return 0;
}

