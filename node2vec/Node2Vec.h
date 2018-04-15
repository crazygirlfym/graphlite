//
// Created by Fu,Yanmei on 17/6/2.
//

#ifndef NODE2VEC_NODE2VEC_H
#define NODE2VEC_NODE2VEC_H

#include "Core"
#include <vector>
using namespace Eigen;
using namespace std;
typedef Array<double, Dynamic, 1> array1d;
typedef Array<int, Dynamic, 1> array1I;
typedef Matrix<double, Dynamic, Dynamic> MatrixDd;
typedef Matrix<array1d *, Dynamic, Dynamic> Matrix3Dd;
typedef Matrix<int, Dynamic, Dynamic> MatrixId;
class Node2Vec {

    int dimension; //dimension of graph
    int walkpnode_r; //walks per node r
    int walklen_l;	//walk length l
    int contextsize_k; //contextsize k
    double p, q; //return p,in-out q;
    int demen_of_feature; //dimension of feature to learn
    MatrixDd *graph, *normalizedgraph; //graph
    Matrix3Dd *modified_matrix; //graph after ProcessModifiedWeights
    MatrixDd *Feature_Vector; //feature vector to learn: dimension multiply demen_of_feature


public:
    vector< vector<int> > *walks;
    Node2Vec();
    ~Node2Vec();
    void config();
    void Initialization();
    void NormalizeGraph();
    Matrix3Dd * ProcessModifiedWeights(MatrixDd *graph, double p, double q);
    void LearnFearture(MatrixDd graph, int dimension, int walkpnode_r, int walklen_l, int contextsize_k, double p , double q);
    vector<int> * node2vecWalk( int startnode_u, int length_l);
    void GetNeighbors(int currentnode, int lastnode, vector<int> *neighbor_of_current_node, vector<double> *pi, int walk_iter);
    int inline DiscreteSampling(vector<int> *vecnode, vector<double> *vecprob);

};


#endif //NODE2VEC_NODE2VEC_H
