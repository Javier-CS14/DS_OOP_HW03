//********************************************
// Student Name			: Javier Evano Cahyadi
// Student ID			: 114550201
// Student Email Address: javierevano.cs14@nycu.edu.tw
//********************************************
//
//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// National Yang Ming Chiao Tung University, Taiwan
// Computer Science
// Date: 2026/02
//
//


#include <iostream>
#include "mySystem_GraphSystem.h"
#include <time.h>
#include <cfloat>
#include <climits>
#include <Windows.h>
#include <vector>
#include <cmath>

using namespace std;

namespace SYS_CONSTANTS {
    int max_int = INT_MAX;
    double max_double = DBL_MAX;
};

int Param::GRAPH_MAX_NUM_NODES = 10000;
int Param::GRAPH_MAX_NUM_EDGES = 10000;


GRAPH_SYSTEM::GRAPH_SYSTEM()
{
    mFlgAutoNodeDeletion = false;

    mFlgShowNodeDepth = false;

    //mOperation = GRAPH_OPERATION_ADD_EDGE;
    //mFlgShowNodeDepth = false;

    //mOperation = GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH;
    //mFlgShowNodeDepth = true;

    mOperation = GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH;
    mOperationName = "SHORTEST_PATH";

    mMaxNodeDepth = 0.0;

    mStartNode = nullptr;
    mDestinationNode = nullptr;

    initMemoryPool();
    createDefaultGraph();
}


void GRAPH_SYSTEM::initMemoryPool()
{
    mMaxNumNodes = Param::GRAPH_MAX_NUM_NODES;
    mMaxNumEdges = Param::GRAPH_MAX_NUM_EDGES;

    mNodeArr_Pool = new GRAPH_NODE[mMaxNumNodes];
    mEdgeArr_Pool = new GRAPH_EDGE[mMaxNumEdges];

    mCurNumOfActiveNodes = 0;
    mCurNumOfActiveEdges = 0;
    mActiveNodeArr = new int[mMaxNumNodes];
    mActiveEdgeArr = new int[mMaxNumEdges];

    mFreeNodeArr = new int[mMaxNumNodes];
    mFreeEdgeArr = new int[mMaxNumEdges];
    //
    for (int i = 0; i < mMaxNumNodes; ++i) {
        mNodeArr_Pool[i].id = i;
    }
    for (int i = 0; i < mMaxNumEdges; ++i) {
        mEdgeArr_Pool[i].id = i;
    }
    //
    reset();
}


void GRAPH_SYSTEM::reset()
{
    stopAutoNodeDeletion();

    //mNumPoints_DoubleCircles = 0;

    mCurNumOfActiveNodes = 0;
    mCurNumOfActiveEdges = 0;

    mCurNumOfFreeNodes = mMaxNumNodes;
    mCurNumOfFreeEdges = mMaxNumEdges;

    for (int i = 0; i < mCurNumOfFreeNodes; ++i) {
        mFreeNodeArr[i] = i;
    }
    for (int i = 0; i < mCurNumOfFreeEdges; ++i) {
        mFreeEdgeArr[i] = i;
    }

    mPassiveSelectedNode = 0;
    mSelectedNode = 0;
    //
    mFlgAutoNodeDeletion = false;

    mFlgShowNodeDepth = false;

    mMaxNodeDepth = 0.0;

    mStartNode = nullptr;
    mDestinationNode = nullptr;
}


GRAPH_NODE* GRAPH_SYSTEM::getFreeNode()
{
    if (mCurNumOfFreeNodes == 0) return 0;
    --mCurNumOfFreeNodes;
    int id = mFreeNodeArr[mCurNumOfFreeNodes];
    GRAPH_NODE* n = &mNodeArr_Pool[id];
    mActiveNodeArr[mCurNumOfActiveNodes] = id;
    n->dynamicID = mCurNumOfActiveNodes;
    ++mCurNumOfActiveNodes;
    return n;
}


GRAPH_EDGE* GRAPH_SYSTEM::getFreeEdge()
{
    if (mCurNumOfFreeEdges == 0) return 0;
    --mCurNumOfFreeEdges;
    int id = mFreeEdgeArr[mCurNumOfFreeEdges];
    GRAPH_EDGE* e = &mEdgeArr_Pool[id];
    mActiveEdgeArr[mCurNumOfActiveEdges] = id;
    e->dynamicID = mCurNumOfActiveEdges;

    ++mCurNumOfActiveEdges;
    return e;
}


void GRAPH_SYSTEM::createDefaultGraph()
{
    cout << "here" << endl;
    reset();

    float offset_x = 90.;
    float offset_z = 15.;

    int n_0 = addNode(offset_x + 0.0, 0.0, offset_z + 0.0, 4.0);
    cout << "n_0:" << n_0 << endl;

    // modify and add your code heres
    int n_1 = addNode(offset_x + 10.0, 0.0, offset_z + 0.0, 2.0);
    int n_2 = addNode(offset_x + 20.0, 0.0, offset_z + 0.0, 2.0);

    addEdge(n_0, n_1); //addEdge( n_0, n_1 );
    addEdge(n_1, n_2); //addEdge( n_1, n_2 );
}


void GRAPH_SYSTEM::createRandomGraph_DoubleCircles(int n)
{
    reset();

    float offset_x = 90.0f;
    float offset_z = 15.0f;

    float r1 = 10.0f;
    float r2 = 20.0f;

    vector<int> inner;
    vector<int> outer;

    for (int i = 0; i < n; i++)
    {
        float angle = 2.0f * 3.1415926f * i / n;

        float x1 = offset_x + r1 * cos(angle);
        float z1 = offset_z + r1 * sin(angle);

        float x2 = offset_x + r2 * cos(angle);
        float z2 = offset_z + r2 * sin(angle);

        inner.push_back(addNode(x1, 0, z1, 1.5f));
        outer.push_back(addNode(x2, 0, z2, 1.5f));
    }

    // inner circle
    for (int i = 0; i < n; i++)
    {
        addEdge(inner[i], inner[(i + 1) % n]);
    }

    // outer circle
    for (int i = 0; i < n; i++)
    {
        addEdge(outer[i], outer[(i + 1) % n]);
    }

    // random connections
    for (int i = 0; i < n; i++)
    {
        int shift = rand() % 5 - 2; // -2,-1,0,1,2

        int j = (i + shift + n) % n;

        addEdge(inner[i], outer[j]);
    }
}



void GRAPH_SYSTEM::createNet_Circular(int n, int num_layers)
{
    reset();

    float r0 = 5.0f;
    float d = 5.0f;

    float offset_x = 90.0f;
    float offset_z = 30.0f;

    // modify and add your code heres
    vector<vector<int>> layers;

    for (int layer = 0; layer <= num_layers; layer++) {

        float radius = r0 + layer * d;

        vector<int> currentLayer;

        for (int i = 0; i < n; i++) {

            float angle = 2.0f * 3.1415926f * i / n;

            float x = offset_x + radius * cos(angle);
            float z = offset_z + radius * sin(angle);

            int nodeID = addNode(x, 0, z, 1.5f);

            currentLayer.push_back(nodeID);
        }

        layers.push_back(currentLayer);
    }

    // connect circular edges
    for (int layer = 0; layer < num_layers; layer++) {

        for (int i = 0; i < n; i++) {

            int next = (i + 1) % n;

            addEdge(layers[layer][i], layers[layer][next]);
        }
    }

    // connect radial edges
    for (int layer = 0; layer < num_layers; layer++) {

        for (int i = 0; i < n; i++) {

            addEdge(layers[layer][i], layers[layer + 1][i]);
        }
    }
}



void GRAPH_SYSTEM::createNet_Square(int n, int num_layers)
{
    reset();

    float spacing = 3.0f;

    float offset_x = 20.0f;
    float offset_z = 20.0f;

    // modify and add your code heres
    int size = n + 2 * num_layers;

    vector<vector<int>> grid(size, vector<int>(size, -1));

    // create nodes only on square rings
    for (int layer = 0; layer < num_layers; layer++)
    {
        int top = layer;
        int left = layer;
        int bottom = size - 1 - layer;
        int right = size - 1 - layer;

        // top row
        for (int j = left; j <= right; j++)
        {
            if (grid[top][j] == -1)
            {
                float x = offset_x + j * spacing;
                float z = offset_z + top * spacing;
                grid[top][j] = addNode(x, 0, z, 1.0f);
            }
        }

        // bottom row
        for (int j = left; j <= right; j++)
        {
            if (grid[bottom][j] == -1)
            {
                float x = offset_x + j * spacing;
                float z = offset_z + bottom * spacing;
                grid[bottom][j] = addNode(x, 0, z, 1.0f);
            }
        }

        // left column
        for (int i = top; i <= bottom; i++)
        {
            if (grid[i][left] == -1)
            {
                float x = offset_x + left * spacing;
                float z = offset_z + i * spacing;
                grid[i][left] = addNode(x, 0, z, 1.0f);
            }
        }

        // right column
        for (int i = top; i <= bottom; i++)
        {
            if (grid[i][right] == -1)
            {
                float x = offset_x + right * spacing;
                float z = offset_z + i * spacing;
                grid[i][right] = addNode(x, 0, z, 1.0f);
            }
        }
    }

    // connect each ring
    for (int layer = 0; layer < num_layers; layer++)
    {
        int top = layer;
        int left = layer;
        int bottom = size - 1 - layer;
        int right = size - 1 - layer;

        // top
        for (int j = left; j < right; j++)
            addEdge(grid[top][j], grid[top][j + 1]);

        // right
        for (int i = top; i < bottom; i++)
            addEdge(grid[i][right], grid[i + 1][right]);

        // bottom
        for (int j = left; j < right; j++)
            addEdge(grid[bottom][j], grid[bottom][j + 1]);

        // left
        for (int i = top; i < bottom; i++)
            addEdge(grid[i][left], grid[i + 1][left]);
    }
}



void GRAPH_SYSTEM::createNet_RadicalCircular(int n)
{
    reset();

    float offset_x = 90.0f;
    float offset_z = 15.0f;

    float r = 15.0f;

    // modify and add your code heres
    int center = addNode(offset_x, 0, offset_z, 2.0f);

    vector<int> outer;

    for (int i = 0; i < n; i++) {

        float angle = 2.0f * 3.1415926f * i / n;

        float x = offset_x + r * cos(angle);
        float z = offset_z + r * sin(angle);

        int nodeID = addNode(x, 0, z, 1.5f);

        outer.push_back(nodeID);

        addEdge(center, nodeID);
    }
}

//
// return the node id
// return -1: no free node
//
int GRAPH_SYSTEM::addNode(float x, float y, float z, float r)
{

    GRAPH_NODE* g;     //GRAPH_NODE *g;
    g = getFreeNode();  //g = getFreeNode( );

    // modify and add your code heres
    if (g == 0) return -1;

    g->p.x = x;
    g->p.y = y;
    g->p.z = z;

    g->r = r;

    g->edgeID.clear();

    g->depth = FLT_MAX;
    g->visited = false;

    g->path_cost = FLT_MAX;
    g->path_parent = nullptr;

    return g->id;
}


//
// return the edge id
// return -1: no free edge
//
int GRAPH_SYSTEM::addEdge(int nodeID_0, int nodeID_1)
{
    // modify and add your code heres
    if (nodeID_0 == nodeID_1)
        return -1;

    GRAPH_EDGE* e; //GRAPH_EDGE *e;

    e = getFreeEdge(); //e = getFreeEdge( );

    if (e == 0) return -1;

    e->nodeID[0] = nodeID_0;
    e->nodeID[1] = nodeID_1;

    mNodeArr_Pool[nodeID_0].edgeID.push_back(e->id);
    mNodeArr_Pool[nodeID_1].edgeID.push_back(e->id);

    return e->id;
}

void GRAPH_SYSTEM::askForInput()
{
    //
    // modify and add your code heres
    //
    cout << "GRAPH_SYSTEM" << endl;
    cout << "Key usage:" << endl;
    cout << "1: create a default graph" << endl;
    cout << "2: create a graph ..." << endl;
    cout << "3: create a graph ..." << endl;
    cout << "4: create a graph ..." << endl;
    cout << "5: create a graph ..." << endl;
    cout << "Delete: delete a selected node and all the edges incident to it" << endl;
    cout << "Spacebar: Change operation" << endl;
    cout << " " << endl;
    cout << "Use the mouse to select nodes and add edges" << endl;
    cout << "Click the left button to select/unselect or create an edge" << endl;
    cout << " " << endl;
    cout << "A selected node is highlighted as red." << endl;

}


GRAPH_NODE* GRAPH_SYSTEM::findNearestNode(double x, double z, double& cur_distance2) const
{
    GRAPH_NODE* n = nullptr;

    // modify and add your code heres
    cur_distance2 = DBL_MAX; //cur_distance2 = -1.0;

    for (int i = 0; i < mCurNumOfActiveNodes; ++i) {

        int nodeID = mActiveNodeArr[i];

        GRAPH_NODE* cur = &mNodeArr_Pool[nodeID];

        double dx = cur->p.x - x;
        double dz = cur->p.z - z;

        double d2 = dx * dx + dz * dz;

        if (d2 < cur_distance2) {

            cur_distance2 = d2;
            n = cur;
        }
    }

    return n;
}


void GRAPH_SYSTEM::moveTo(double x, double y)
{
    if (!mSelectedNode) return;
    mSelectedNode->p.x = x;
    mSelectedNode->p.z = y;
}


//
// handle the mouse click event at position (x, z).
//
void GRAPH_SYSTEM::clickAt(double x, double z)
{
    double cur_d2;
    GRAPH_NODE* nearest_node = findNearestNode(x, z, cur_d2);
    if (nearest_node == 0) {
        mSelectedNode = 0;
        return;
    }
    if (cur_d2 > nearest_node->r * nearest_node->r) {
        mSelectedNode = 0;
        return;

    }
    if (mSelectedNode == nearest_node) {
        mSelectedNode = 0;
        return;
    }

    performOperation(nearest_node);

}


void GRAPH_SYSTEM::performOperation(GRAPH_NODE* node) {
    switch (mOperation) {
    case GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH:
        mSelectedNode = node;
        mFlgShowNodeDepth = true;
        computeDepthOfAllNodesFromSelectedNode();
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE:
        if (mSelectedNode != 0) {
            addEdge(mSelectedNode->id, node->id);
            mSelectedNode = 0;
            return;
        }
        mSelectedNode = node;
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH:
        if (mSelectedNode != 0) {
            mStartNode = mSelectedNode;
            mDestinationNode = node;
            computeShortestPath();
            mSelectedNode = 0;
            return;
        }
        mSelectedNode = node;
        break;
    }
}


void GRAPH_SYSTEM::deleteEdge(int edgeID)
{
    GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID]; //GRAPH_EDGE *e = &mEdgeArr_Pool[ edgeID ];

    // modify and add your code heres
    removeEdgeFromNode(e, e->nodeID[0]);
    removeEdgeFromNode(e, e->nodeID[1]);

    int dynamicID = e->dynamicID; //int dynamicID = e->dynamicID;

    --mCurNumOfActiveEdges;

    int lastEdgeID = mActiveEdgeArr[mCurNumOfActiveEdges];

    mActiveEdgeArr[dynamicID] = lastEdgeID;

    mEdgeArr_Pool[lastEdgeID].dynamicID = dynamicID;

    mFreeEdgeArr[mCurNumOfFreeEdges] = edgeID;

    ++mCurNumOfFreeEdges;

}


void GRAPH_SYSTEM::removeEdgeFromNode(const GRAPH_EDGE* e, int nodeID)
{
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID]; //GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];

    // modify and add your code heres
    for (int i = 0; i < n->edgeID.size(); ++i) {

        if (n->edgeID[i] == e->id) {

            n->edgeID.erase(n->edgeID.begin() + i);

            return;
        }
    }

}


void GRAPH_SYSTEM::deleteEdgesOfNode(int nodeID)
{
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID];    // GRAPH_NODE *n  = &mNodeArr_Pool[ nodeID ];

    // modify and add your code heres
    while (!n->edgeID.empty()) {
        int edgeID = n->edgeID.back();

        deleteEdge(edgeID);
    }

}


void GRAPH_SYSTEM::deleteNode(int nodeID) {
    // modify and add your code heres
    if (mCurNumOfActiveNodes <= 0) return; //if ( mCurNumOfActiveNodes <= 0 ) return;

    GRAPH_NODE* n = &mNodeArr_Pool[nodeID]; //GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];

    deleteEdgesOfNode(nodeID);

    int dynamicID = n->dynamicID;

    --mCurNumOfActiveNodes;

    int lastNodeID = mActiveNodeArr[mCurNumOfActiveNodes];

    mActiveNodeArr[dynamicID] = lastNodeID;

    mNodeArr_Pool[lastNodeID].dynamicID = dynamicID;

    mFreeNodeArr[mCurNumOfFreeNodes] = nodeID;

    ++mCurNumOfFreeNodes;

}



void GRAPH_SYSTEM::deleteSelectedNode() {
    if (mSelectedNode == 0) return;

    // modify and add your code heres
    deleteNode(mSelectedNode->id);
    mSelectedNode = 0;
}



bool GRAPH_SYSTEM::isSelectedNode() const
{
    return mSelectedNode != 0;
}



void GRAPH_SYSTEM::getInfoOfSelectedPoint(double& r, vector3& p) const
{
    if (isSelectedNode() == false) return;
    r = mSelectedNode->r;
    p = mSelectedNode->p;

}



void GRAPH_SYSTEM::changeOperation()
{
    switch (mOperation) {
    case GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE;
        mOperationName = "ADD_EDGE";
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH;
        mOperationName = "SHORTEST_PATH";
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH;
        mOperationName = "NODE_DEPTH";

        break;
    }
}




void GRAPH_SYSTEM::resetDepthOfAllNodes()
{
    mMaxNodeDepth = 0;

    // modify and add your code heres
    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];  //int nodeID = mActiveNodeArr[i];

        GRAPH_NODE* n = &mNodeArr_Pool[nodeID]; //GRAPH_NODE* n = &mNodeArr_Pool[nodeID];

        // set node's depth
        // and others if necessary
        n->depth = FLT_MAX;
        n->visited = false;
    }
}



/*
* k := computeDepthOfAllNodesFromSelectedNode

* void k( Node *n, int depth ) {
    if n is null, return
    set n’s depth to depth
    for each adjacent node m of n				; note m should not be n
        if (m’s depth < depth + 1) k(m, depth+1)
}

void computeDepthOfAllNodesFromSelectedNode( ) {
    for each node n, set its depth to FLT_MAX
    k(selected_node, 0)
    update mMaxNodeDepth
}
*/




void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode(GRAPH_NODE* node, int depth)
{
    if (node == 0) return;
    node->depth = depth;
    node->visited = true;

    // modify and add your code heres
    for (int i = 0; i < node->edgeID.size(); ++i) { // for all edges incident to the node: node

        int edgeID = node->edgeID[i]; // get edge ID

        GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID]; // get e = &mEdgeArr_Pool[edgeID];

        GRAPH_NODE* n0 = &mNodeArr_Pool[e->nodeID[0]]; // n0 = &mNodeArr_Pool[e->nodeID[0]];
        GRAPH_NODE* n1 = &mNodeArr_Pool[e->nodeID[1]]; // n1 = &mNodeArr_Pool[e->nodeID[1]];

        GRAPH_NODE* next;

        // determine the next node, which is set as the current node
        // update the current node's depth
        // recursively update all the nodes connected to the current node

        if (n0 == node) {
            next = n1;
        }
        else {
            next = n0;
        }

        node->visited = true;

        if (next->depth > depth + 1) {
            computeDepthOfAllNodesFromSelectedNode(next, depth + 1);
        }
    }
}




void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode()
{
    // modify and add your code heres
    resetDepthOfAllNodes(); // reset the depth of all nodes

    if (mSelectedNode == nullptr) // if mSelectedNode is nullpute, return
        return;

    mMaxNodeDepth = 0; // set  mMaxNodeDepth = 0;
    mSelectedNode->depth = 0; // set mSelectedNode->depth as zero
    computeDepthOfAllNodesFromSelectedNode(mSelectedNode, 0); // invoke computeDepthOfAllNodesFromSelectedNode

    // Determine the mMaxNodeDepth // modify and add your code heres
    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {

        int nodeID = mActiveNodeArr[i];

        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];

        if (n->depth != FLT_MAX && n->depth > mMaxNodeDepth)
            mMaxNodeDepth = n->depth;
    }
}




// This member function is not used.
float GRAPH_SYSTEM::getNodeDepthFromSelectedNode(int nodeIndex) const
{
    float d = 0.0;

    // modify and add your code heres
    int nodeID = mActiveNodeArr[nodeIndex];

    d = mNodeArr_Pool[nodeID].depth;

    return d;
}




void GRAPH_SYSTEM::resetPathInformationOfAllNodes()
{
    // modify and add your code heres
    int numNodes = getNumOfNodes();

    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];

        // modify and add your code heres
        n->path_cost = FLT_MAX; // set path cost of node
        n->path_parent = nullptr; // set path_parent of node
        n->visited = false;
    }
}




void GRAPH_SYSTEM::computeShortestPath(GRAPH_NODE* node)
{
    if (node == 0) return;

    // modify and add your code heres
    for (int i = 0; i < node->edgeID.size(); ++i) { // for each edge incident to node, node 

        int edgeID = node->edgeID[i]; // get an edge //int edgeID = node->edgeID[i];

        GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID]; // determine the next node. Set it as the current node. //GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID];

        // compute distance d: node->p.distance(next->p);
        GRAPH_NODE* n0 = &mNodeArr_Pool[e->nodeID[0]]; //GRAPH_NODE* n0 = &mNodeArr_Pool[e->nodeID[0]];
        GRAPH_NODE* n1 = &mNodeArr_Pool[e->nodeID[1]]; //GRAPH_NODE* n1 = &mNodeArr_Pool[e->nodeID[1]];

        GRAPH_NODE* next;

        // if new path cost is not better, check for the other edges
        if (n0 == node) {
            next = n1;
        }
        else {
            next = n0;
        }

        node->visited = true;

        float d = node->p.distance(next->p);
        float new_cost = node->path_cost + d;

        // if new path cost is better, update the node's path cost and path_parent
        if (new_cost >= next->path_cost)
            continue;

        next->path_cost = new_cost;
        next->path_parent = node;

        // Also, invokte computeShortestPath for the current node.
        if (!next->visited) 
            computeShortestPath(next); 
    }

    node->visited = false;
}




void GRAPH_SYSTEM::computeShortestPath()
{
    // modify and add your code heres
    resetPathInformationOfAllNodes(); // reset path information of all nodes

    if (mStartNode == nullptr || mDestinationNode == nullptr) // if mStartNode == nullptr || mDestinationNode == nullptr, return
        return;

    mStartNode->path_cost = 0;

    computeShortestPath(mStartNode); // invokte computeShortestPath with mStartNode
}




void GRAPH_SYSTEM::handleKeyPressedEvent(unsigned char key)
{

    switch (key) {
    case 127: // delete
        mFlgAutoNodeDeletion = false;
        deleteSelectedNode();
        break;
    case '1':
        mFlgAutoNodeDeletion = false;
        createDefaultGraph();
        mSelectedNode = 0;
        break;
    case '2':
        mFlgAutoNodeDeletion = false;
        createNet_Circular(12, 6);
        mSelectedNode = 0;

        break;
    case '3':
        mFlgAutoNodeDeletion = false;
        createNet_Square(15, 4);
        mSelectedNode = 0;

        break;
    case '4':
        mFlgAutoNodeDeletion = false;
        createNet_RadicalCircular(24);
        mSelectedNode = 0;

        break;
    case '5':
        mNumPoints_DoubleCircles = 24;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;
    case '<':
        mNumPoints_DoubleCircles--;
        if (mNumPoints_DoubleCircles < 3)
            mNumPoints_DoubleCircles = 3;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;
    case '>':
        mNumPoints_DoubleCircles++;
        if (mNumPoints_DoubleCircles > 36)
            mNumPoints_DoubleCircles = 36;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;

    case 'r':
        mFlgAutoNodeDeletion = false;
        mSelectedNode = 0;
        break;
    case ' ':
        changeOperation();
        break;
    case 'd':
    case 'D':
        mFlgAutoNodeDeletion = !mFlgAutoNodeDeletion;
        break;
    }
}




void GRAPH_SYSTEM::handlePassiveMouseEvent(double x, double z)
{
    double cur_d2;
    GRAPH_NODE* n = findNearestNode(x, z, cur_d2);
    if (n == 0) return;
    if (cur_d2 > n->r * n->r) {
        mPassiveSelectedNode = 0;
        return;
    }
    mPassiveSelectedNode = n;
}




int GRAPH_SYSTEM::getNumOfNodes() const
{
    return mCurNumOfActiveNodes;
}




void GRAPH_SYSTEM::getNodeInfo(int nodeIndex, double& r, vector3& p) const
{
    int nodeID = mActiveNodeArr[nodeIndex];
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
    r = n->r;
    p = n->p;
}




void GRAPH_SYSTEM::getNodeInfo(int nodeIndex, double& r, vector3& p, float& depth) const
{
    int nodeID = mActiveNodeArr[nodeIndex];
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
    r = n->r;
    p = n->p;
    depth = n->depth;
}




int GRAPH_SYSTEM::getNumOfEdges() const
{
    return mCurNumOfActiveEdges;
}




vector3 GRAPH_SYSTEM::getNodePositionOfEdge(int edgeIndex, int nodeIndex) const
{
    int edgeID = mActiveEdgeArr[edgeIndex];
    GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID];
    return mNodeArr_Pool[e->nodeID[nodeIndex]].p;
}




void GRAPH_SYSTEM::stopAutoNodeDeletion()
{
    mFlgAutoNodeDeletion = false;
}



// For every frame, update( ) function is called.
void GRAPH_SYSTEM::update()
{
    if (!mFlgAutoNodeDeletion) {

        return;
    }

    if (mCurNumOfActiveNodes <= 0) {
        mFlgAutoNodeDeletion = false;
        return;
    }
    Sleep(250);


    // modify and add your code
    if (mCurNumOfActiveNodes > 0) { // delete the selected node? // delete all the edges incident to the selected node?

        int index = rand() % mCurNumOfActiveNodes;
        int nodeID = mActiveNodeArr[index];
        deleteNode(nodeID);
    }

    mSelectedNode = 0;
    mPassiveSelectedNode = 0;
}