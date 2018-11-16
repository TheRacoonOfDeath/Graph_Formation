//
// Created by nickerl on 08.11.18.
//

#ifndef UNTITLED_NODE_H
#define UNTITLED_NODE_H

#include <list>

class Node {
    int m_id;

    int m_numNodes;
    int m_maxDeg;
    int m_maxDiam;

    int m_degree;
    int* m_distances;
    bool m_satisfied;

    std::list<Node> m_established;
    std::list<Node> m_accepted;
    int* m_blocked;


public:
    Node(int id,int numNodes = 3250, int maxDeg = 57, int maxDiam = 2) {
        m_id = id;

        m_numNodes = numNodes;
        m_maxDeg = maxDeg;
        m_maxDiam = maxDiam;

        m_degree = 0;
        m_distances = new int [numNodes];
        m_distances = {};
        m_blocked = new int [numNodes];
        m_blocked = {};
    }

    int establishEdge(Node);
    int declineEdge(Node);
    int notifyEstablishment(Node);
    int notifyDecline(Node);

    int setDistance(int,int);

    int getId();
    int* getDistances();
    std::list<Node> getEstablished();
    std::list<Node> getAccepted();
    int getNumNodes();
    int* getBlocked();

};




#endif //UNTITLED_NODE_H
