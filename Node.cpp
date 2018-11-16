//
// Created by nickerl on 09.11.18.
//

# include "Node.h"
# include <list>

// "This" established a node to v and informs v about it
int Node::establishEdge(Node v) {
    m_degree++;
    m_established.push_back(v);
    m_distances[v.getId()] = 1;
    v.notifyEstablishment(*this);

    std::list<Node> accepted = v.getAccepted();
    std::list<Node> established = v.getEstablished();
    int* blocked = v.getBlocked();

    for(std::list<Node>::iterator it = accepted.begin(); it != accepted.end(); it++){
        int* distances = it->getDistances();
        if(distances[v.getId()] == 0){
            it->setDistance(v.getId(),2);
            v.setDistance(it->getId(),2);
        }
    }

    for(std::list<Node>::iterator it = established.begin(); it != established.end(); it++){
        if(blocked[it->getId()]){
            continue;
        }
        int* distances = it->getDistances();
        if(distances[v.getId()] == 0){
            it->setDistance(v.getId(),2);
            v.setDistance(it->getId(),2);
        }
    }

    m_satisfied = true;

    for(int i = 0; i < m_numNodes; i++){
        if(m_distances[i] == 0 && i != m_id){
            m_satisfied = false;
            break;
        }
    }

    if(m_satisfied && m_degree > m_maxDeg) {
        m_satisfied = false;
    }

    return 0;
}

// Declines a previously accepted edge and informs the origin node
int Node::declineEdge(Node v) {
    m_degree--;

    v.notifyDecline(*this);

    m_satisfied = true;

    for(int i = 0; i < m_numNodes; i++){
        if(m_distances[i] == 0 && i != m_id){
            m_satisfied = false;
            break;
        }
    }

    if(m_satisfied && m_degree > m_maxDeg) {
        m_satisfied = false;
    }

    return 0;
}


// v established a node to "this". "This" updates it's local data.
int Node::notifyEstablishment(Node v) {
    m_accepted.push_back(v);
    m_degree++;

    m_distances[v.getId()] = 1;

    std::list<Node> accepted = v.getAccepted();
    std::list<Node> established = v.getEstablished();
    int* blocked = v.getBlocked();

    for(std::list<Node>::iterator it = accepted.begin(); it != accepted.end(); it++){
        int* distances = it->getDistances();
        if(distances[v.getId()] == 0){
            it->setDistance(v.getId(),2);
            v.setDistance(it->getId(),2);
        }
    }

    for(std::list<Node>::iterator it = established.begin(); it != established.end(); it++){
        if(blocked[it->getId()]){
            continue;
        }
        int* distances = it->getDistances();
        if(distances[v.getId()] == 0){
            it->setDistance(v.getId(),2);
            v.setDistance(it->getId(),2);
        }
    }


    m_satisfied = true;

    for(int i = 0; i < m_numNodes; i++){
        if(m_distances[i] == 0 && i != m_id){
            m_satisfied = false;
            break;
        }
    }

    if(m_satisfied && m_degree > m_maxDeg) {
        m_satisfied = false;
    }

    return 0;
}

// v declined a node established by "this". "This" updates its local data
int Node::notifyDecline(Node v) {
    //TODO: Is "this" still connected to any of the neighbours of $v$? check em all!

    m_blocked[v.getId()] = 1;
    int vId = v.getId();
    setDistance(vId, 0);

    //If there exists a node in the neighbourhood of "this" with distance 1 to v, set this.dist[v] to 2.


    for(std::list<Node>::iterator it = m_accepted.begin(); it != m_accepted.end(); it++){
        int* distances = it->getDistances();
        if(distances[vId] == 1){
            setDistance(vId,2);
            break;
        }
    }

    if(m_distances[vId] == 0) {
        for (std::list<Node>::iterator it = m_established.begin(); it != m_established.end(); it++) {
            if (m_blocked[it->getId()]) {
                continue;
            }
            int* distances = it->getDistances();
            if (distances[vId] == 1) {
                setDistance(vId, 2);
            }
        }
    }

    //TODO: For all neighbours of "this", check if they are connected to a node with dist[v] = 1. If yes, continue, else, set dist[v] = 0
    for(std::list<Node>::iterator it = m_accepted.begin(); it != m_accepted.end(); it++){
        it->setDistance(vId,0);
        std::list<Node> accepted = it->getAccepted();
        std::list<Node> established = it->getEstablished();
        int* blocked = it->getBlocked();
        for(std::list<Node>::iterator it2 = accepted.begin(); it2 != accepted.end(); it2++) {
            int* distances = it2->getDistances();
            if(distances[vId] == 1){
                it->setDistance(vId,2);
                break;
            }
        }

        for(std::list<Node>::iterator it2 = accepted.begin(); it2 != accepted.end(); it2++) {
            if(blocked[it2->getId()] == 1){
                continue;
            }
            int* distances = it2->getDistances();
            if(distances[vId] == 1){
                it->setDistance(vId,2);
                break;
            }
        }

    }


    for (std::list<Node>::iterator it = m_established.begin(); it != m_established.end(); it++) {
        if (m_blocked[it->getId()]) {
            continue;
        }
        int *distances = it->getDistances();
        if (distances[vId] == 1) {
            setDistance(vId, 2);
        }
    }


    return 0;
}


// getter for local variables
int Node::getId() {
    return m_id;
}
int* Node::getDistances() {
    return m_distances;
}
int* Node::getBlocked() {
    return m_blocked;
}
int Node::getNumNodes() {
    return m_numNodes;
}
std::list<Node> Node::getAccepted() {
    return m_accepted;
}
std::list<Node> Node::getEstablished() {
    return m_established;
}

// setter for local variables

int Node::setDistance(int id, int distance) {
    m_distances[id] = distance;
    return 0;
}