//
// Created by nickerl on 14.11.18.
//

#include <cstdlib>

#ifndef MOORE_RESPONSE_H
#define MOORE_RESPONSE_H

void randperm(int *a, int n) {
    int r, tmp;
    for (int i = 0; i < n; i++) {
        a[i] = i;
    }
    for (int i = n - 1; i > 1; i--) {
        r = rand() % (i + 1);
        tmp = a[r];
        a[r] = a[i];
        a[i] = tmp;
    }
}

template<typename T, typename S>
int chooseUnsatisfiedNode(int n, int d, int k, T &graph, T &distances, S &degrees) {
    int a[n];
    randperm(a, n);


    for (int l = 0; l < n; l++) {
        int idx = a[l];
        if (degrees[idx] > k) {
            return idx;
        }
        for (int j = 0; j < n; j++) {
            if (idx == j) {
                continue;
            }
            if (graph[j][idx] == -1 || distances[idx][j] > d || distances[idx][j] == 0) {
                return idx;
            }
        }

    }


    std::cout << "no unsatisfied Node found!!" << std::endl;
    return -1;
}

template<typename T, typename S>
void cleanRejectedEdges(int player, int n, T &graph, S &degrees) {
    for (int j = 0; j < n; j++) {
        if (graph[j][player] == -1) { // this only happens while graph[player][j] == 2
            graph[j][player] = 0;
            graph[player][j] = 0;
            degrees[player]--;
        }
    }
}

template<typename T, typename S>
int rejectEdges(int player, int n, int k, T &graph, S &degrees) {
    int ret = 0;
    for (int j = 0; j < n; j++) {
        if (graph[player][j] == 1 && degrees[j] > k) {
            graph[player][j] = -1;
            degrees[player]--;
            ret = 1;
        }
    }
    return ret;
}

template<typename T>
int findFittingNeighbour(int player, int target, int n, int d, T &graph, T &distances) {
    int a[n];
    randperm(a, n);


    for (int i = 0; i < n; i++) {
        int idx = a[i];
        if (distances[idx][target] != 0 && distances[idx][target] < d && graph[idx][player] != -1) {
            return idx;
        }
    }

    // std::cout << "We didn't find a fitting neighbour!" << std::endl;

    return -1;
}

template<typename T, typename S>
int establishDistances(int player, int n, int d, T &graph, T &distances, S &degrees) {
    int ret = 0;

    for (int j = 0; j < n; j++) {
        if (j != player && (distances[player][j] > d || distances[player][j] == 0)) {
            if (graph[player][j] == 0) { // no edge between the two exists --> establish an edge
                graph[player][j] = 2;
                graph[j][player] = 1;
                degrees[player]++;
                degrees[j]++;

                ret = 1;
                apsp(graph, distances);
            } else { //edge between them is blocked --> search the neighbourhood for suitable node
                int idx = findFittingNeighbour(player, j, n, d, graph, distances);
                if (idx != -1) {
                    graph[player][idx] = 2;
                    graph[idx][player] = 1;
                    degrees[player]++;
                    degrees[idx]++;

                    ret = 1;
                    apsp(graph, distances);
                }
            }
        }

    }

    return ret;
}

template<typename T, typename S>
void reduceDegree(int player, int n, int k, T &graph, S &degrees) {


    int a[n];
    randperm(a, n);

    int tmp = degrees[player];


    for (int i = 0; i < n; i++) {
        if (degrees[player] <= k) {
            break;
        }
        if (graph[player][a[i]] == 1) {
            graph[player][a[i]] = -1;
            degrees[player]--;
        } else if (graph[player][a[i]] == 2) {
            graph[player][a[i]] = 0;
            graph[a[i]][player] = 0;
            degrees[player]--;
            degrees[a[i]]--;
        }
    }
}

#endif //MOORE_RESPONSE_H
