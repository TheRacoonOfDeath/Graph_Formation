#include <iostream>
#include <stdlib.h>
#include <array>
#include "matrix.h"
#include "response.h"

const int n = 50;
const int MAX_DISTANCE = 2;
const int MAX_DEGREE = 7;

/*
 * General info:
 * establishing an edge has value 2
 * accepting an established edge has value 1
 * doing nothing has value 0
 * rejecting an edge has value -1
 *
 */







template <typename T>
void apsp(T& inMat, T& outMat){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(inMat[i][j] == 1 || (inMat[i][j] == 2 && inMat[j][i] == 1) || inMat[i][j] == -1){
                outMat[i][j] = 1;
            } else {
                outMat[i][j] = 0;
            }
        }
    }


    for(int k = 0; k < n; k++ ){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if (outMat[i][k] == 0 || outMat[k][j] == 0 || i == j){
                    continue;
                }
                if(outMat[i][j] == 0 || outMat[i][j] > outMat[i][k] + outMat[k][j]){
                    outMat[i][j] = outMat[i][k] + outMat[k][j];
                }
            }
        }
    }

}

template <typename T,typename S>
void randomGraph(T& graph, S& degrees){

    int tmp = 0;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < MAX_DEGREE; j++){
            tmp = rand() % n;
            if(graph[i][tmp] == 0 && tmp != i){
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

template <typename T,typename S>
int satisfied(T& graph, T& distances, S& degrees){
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                continue;
            }
            if (distances[i][j] == 0 || distances[i][j] > MAX_DISTANCE) {
                return 0;
            }
        }
    }

    for(auto& i : graph){
        for(auto& j : i){
            if(j == -1){
                return 0;
            }
        }
    }

    for(auto& i : degrees){
        if(i > MAX_DEGREE){
            return 0;
        }
    }

    return 1;

}

// chooses an unsatisfied player and improves his situation
template <typename T,typename S>
void performImprovement(int n, int d, int k, T& graph, T& distances, S& degrees){
    int player = chooseUnsatisfiedNode(n,d,k,graph,distances,degrees);

    int tmp = degrees[player];

    //std::cout << "unsat player: " << player << std::endl;

    if(rejectEdges(player, n, k, graph, degrees)){ // if the player accepted edges from nodes with too high degree, reject those edges
        apsp(graph,distances); // update the distances
        int tmp = degrees[player];
        //std::cout << "we rejected some edge" << std::endl;



    }
    if(establishDistances(player, n, d, graph, distances, degrees)){ // make sure the player reaches every node
        cleanRejectedEdges(player, n, graph, degrees); // if there are rejected edges from the player, delete them
        apsp(graph,distances);
        //std::cout << "we established some distances and possibly cleaned rejected edges" << std::endl;
    } else {
        cleanRejectedEdges(player, n, graph, degrees);
        int tmp = degrees[player];
        reduceDegree(player, n, k, graph, degrees); // if the distances were alright, reduce the players' degree by deleting incoming edges
        apsp(graph,distances);
        //std::cout << "we didn't have to establish distances, thus tried to reduce the degree" << std::endl;
    }
}

int main() {

    auto seed = time(NULL);//1542200294;//
    std::cout << "Seed: " << seed << std::endl << std::endl;
    srand(seed);

    Matrix<int, n, n> graph,distances;
    std::array<int,n> degrees{};

    randomGraph(graph,degrees);

    apsp(graph,distances);


    /*
    std::cout << std::endl;
    for(auto& i : graph){
        for(auto& j: i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for(auto& i : distances){
        for(auto& j: i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    for(auto& i : degrees){
        std::cout << i << " ";

    }
    std::cout << std::endl;

     */
    int counter = 0;

    while(!satisfied(graph, distances, degrees)){
        counter++;
        if(counter%100==0){
            std::cout << counter << std::endl;
        }

        performImprovement(n, MAX_DISTANCE, MAX_DEGREE, graph, distances, degrees);
        apsp(graph,distances);


        /*
        std::cout << std::endl;
        for(auto& i : graph){
            for(auto& j: i){
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        for(auto& i : distances){
            for(auto& j: i){
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
        for(auto& i : degrees){
            std::cout << i << " ";

        }
        std::cout << std::endl << std::endl;

         */
    }





    std::cout << "Adjacency matrix:" << std::endl;
    for(auto& i : graph){
        for(auto& j: i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Distance matrix:" << std::endl;
    for(auto& i : distances){
        for(auto& j: i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "degrees:" << std::endl;
    std::cout << std::endl;
    for(auto& i : degrees){
        std::cout << i << " ";

    }
    std::cout << std::endl;

    std::cout << "number of iterations: " << counter << std::endl;

    int a = satisfied(graph, distances, degrees);

    return 0;


  /*  const int n = NUMBER_OF_NODES;
    int d = MAX_DISTANCE;
    int k = MAX_DEGREE;
    int seed = 1234;

    srand(seed);

    //int **graph;
    //int **distances;
    //int *degrees;

    Matrix graph(n);
    Matrix distances(n);
    std::array<int,n> degrees;

    randomGraph(graph,degrees);
    apsp(graph,distances);


    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            std::cout << graph(i,j);
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            std::cout << distances(i,j);
        }
        std::cout << std::endl;
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;*/
}

