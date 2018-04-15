//
// Created by Fu,Yanmei on 17/6/2.
//

#include "Node2Vec.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdlib.h>
Node2Vec::Node2Vec() {
    srand((unsigned)time(0));
    this->config();
    this->Initialization();
    this->NormalizeGraph();

    this->ProcessModifiedWeights(this->graph, this->p, this->q);
    this->LearnFearture(*this->graph, this->dimension, this->walkpnode_r, this->walklen_l, this->contextsize_k, this->p, this->q);

    /*
    vector<int> *vecnode = new vector<int>;
    vector<double> *vecprob = new vector<double>;
    for (int i(0); i != 4;++i)
    {
        vecnode->push_back(i);
    }
    vecprob->push_back(0);
    vecprob->push_back(0.444444);
    vecprob->push_back(0.333333);
    vecprob->push_back(0.222222);
    int j = this->DiscreteSampling(vecnode,vecprob);
    cout << j << endl;
    int i(0);
*/

}
Node2Vec::~Node2Vec() {}

void Node2Vec::config(){
    this->dimension = 4;
    this->walkpnode_r = 10 ;
    this->walklen_l = 5;
    this->contextsize_k = 4;
    this->p = 2;
    this->q = 1;
    this->demen_of_feature = 100;

}

void Node2Vec::Initialization(){

    std::cout << "Start inilization........." << std::endl;
    std::ifstream in_stream("Graph.txt");


    try
    {
        std::string tmp;
        this->graph = new MatrixDd(this->dimension, this->dimension);
        this->graph->setZero();
        /*
        for (int i=0; i != this->dimension;++i)
        {
            for (int j=0; j != this->dimension;++j)
            {
                std::cout << "----" << std::endl;
                in_stream >> tmp;

                std::cout << tmp << std::endl;

                in_stream >> (*this->graph)(i, j);

            }
        }
         */

        (*this->graph)(0, 0) = 0;
        (*this->graph)(0, 1) = 2;
        (*this->graph)(0, 2) = 3;
        (*this->graph)(0, 3) = 4;
        (*this->graph)(1, 0) = 3;
        (*this->graph)(1, 1) = 0;
        (*this->graph)(1, 2) = 5;
        (*this->graph)(1, 3) = 5;
        (*this->graph)(2, 0) = 4;
        (*this->graph)(2, 1) = 5;
        (*this->graph)(2, 2) = 0;
        (*this->graph)(2, 3) = 7;
        (*this->graph)(3, 0) = 3;
        (*this->graph)(3, 1) = 4;
        (*this->graph)(3, 2) = 7;
        (*this->graph)(3, 3) = 0;
       /*
        for (int i=0; i != this->dimension; ++i)
		{
			for (int j=0; j != this->dimension; ++j)
			{
				std::cout << (*this->graph)(i, j) <<" ";
			}
			std::cout << std::endl;
		}
*/
    }
    catch (...)
    {
        std::cout << "Inilization Exception...." << std::endl;
        return;
    }
}
void Node2Vec::NormalizeGraph() {
    std::cout << "Start normalize........." << std::endl;
    this->normalizedgraph = new MatrixDd(this->dimension, this->dimension);
    this->normalizedgraph->setZero();
    for (int i(0); i != this->dimension; i++)
    {
        int sum_of_rows = 0;
        if (!(sum_of_rows = (*this->graph).row(i).sum()))
            std::cout << "There are isolated point in the graph : " << i << std::endl;
        else
            sum_of_rows = (*this->graph).row(i).sum();


            for (int j(0); j != this->dimension; j++)
            {


                (*this->normalizedgraph)(i, j) = (double)(*this->graph)(i, j) / sum_of_rows;
            }
    }

    std::cout << "end normalize........." << std::endl;
    for (int i=0; i != this->dimension; ++i)
    {
        for (int j=0; j != this->dimension; ++j)
        {
            std::cout << (*this->normalizedgraph)(i, j) <<" ";
        }
        std::cout << std::endl;
    }



}

Matrix3Dd * Node2Vec::ProcessModifiedWeights(MatrixDd *graph, double p, double q) {
    std::cout << "start Process..." << std::endl;

    this->modified_matrix = new Matrix3Dd(this->dimension, this->dimension);
    this->modified_matrix->setZero();
    for (int i(0); i != this->dimension;++i)
        for (int j(0); j != this->dimension; ++j)
            (*this->modified_matrix)(i, j) = new array1d(this->dimension);

    double sum_of_rows(0);
    double alphapq0 = 1.0 / p, alphapq1 = 1.0, alphapq2 = 1.0 / q;

    for (int i(0); i != this->dimension;++i)
    {
        if (!(sum_of_rows = (*this->graph).row(i).sum()))
            std::cout << "There are isolated point in the graph : " << i << std::endl;
        for (int j(0); j != this->dimension;++j)
        {
            sum_of_rows = 0;
            for (int k(0); k != this->dimension;++k)
            {
                if (k == j)
                    (*(*this->modified_matrix)(i, j))(k)= alphapq0*(*this->graph)(i, k);
                else if ((*this->graph)(j, k))
                    (*(*this->modified_matrix)(i, j))(k) = (*this->graph)(i, k);
                else
                    (*(*this->modified_matrix)(i, j))(k) = (*this->graph)(i, k)*alphapq2;

                sum_of_rows += (*(*this->modified_matrix)(i, j))(k);
            }
            for (int k(0); k != this->dimension; ++k)
            {
                (*(*this->modified_matrix)(i, j))(k) = (*(*this->modified_matrix)(i, j))(k) / sum_of_rows;
            }

        }
    }
        cout << "end modifiedweights" << endl;

    for (int i(0); i != this->dimension; ++i)
        for (int j(0); j != this->dimension; ++j)
        {

            for (int k(0); k != this->dimension; ++k)
            {
                std::cout << (*(*this->modified_matrix)(i, j))(k) << " ";
            }
            std::cout << std::endl;
        }


}

void Node2Vec::LearnFearture(MatrixDd graph, int dimension, int walkpnode_r, int walklen_l, int contextsize_k, double p,
                             double q) {
    ProcessModifiedWeights(&graph, p, q);
    this->walks = new vector< vector<int> >;
    vector< vector< vector<int> > > *multiwalks = new vector< vector< vector<int> > >;
    for (int iter(0); iter != walkpnode_r; iter++)
    {
        for (int startnode_u(0); startnode_u != dimension; startnode_u++)
            this->walks->push_back(*node2vecWalk(startnode_u, walklen_l));

        multiwalks->push_back(*this->walks);
    }
    return;

}




vector<int> * Node2Vec::node2vecWalk( int startnode_u, int length_l) {
    cout << "nodewalk" << endl;
    vector<int> *walk = new vector<int>;
    walk->push_back(startnode_u);
    int currentnode;
    vector<int> *neighbor_of_current_node = new vector<int>;
    vector<double> *pi;
    vector<int> *vecnode = new vector<int>;
    for (int i(0); i != 4;++i)
    {
        vecnode->push_back(i);
    }
    vector<double> *vecprob = new vector<double>;

    for (int walk_iter(0); walk_iter != length_l; ++walk_iter)
    {
        vector<double> *vecprob = new vector<double>;
        int lastnode = -1;
        currentnode = walk->back();
        if(!walk_iter) {
            for (int j = 0; j < this->dimension; j++) {
                vecprob->push_back((*this->normalizedgraph)(currentnode, j));
            }
            int s = DiscreteSampling(vecnode, vecprob);
            walk->push_back(s);
        }
        else{
            int prenode = walk->at(walk_iter);
            for (int j = 0; j < this->dimension; j++) {
                vecprob->push_back((*(*this->modified_matrix)(prenode, currentnode))(j));
            }
            int s = DiscreteSampling(vecnode, vecprob);
            walk->push_back(s);
        }
        /*
        if (!walk_iter) lastnode = *(walk->end() - 2);
        GetNeighbors(currentnode, lastnode, neighbor_of_current_node, pi, walk_iter);
        cout << DiscreteSampling(neighbor_of_current_node, pi) << endl;
        walk->push_back(DiscreteSampling(neighbor_of_current_node, pi));
        */
    }
    /*
    for (int i = 0 ;i < walk->size();i++){
        cout << walk->at(i) ;
    }
    cout << endl;
    */

    return walk;

}

int inline Node2Vec::DiscreteSampling(vector<int> *vecnode,vector<double> *vecprob)
{

    double temp;
    double randomnum;

    randomnum = rand()/ (double)RAND_MAX;
    //cout << "number.." << endl;
    //cout << randomnum << endl;
    for(int i = 0;i < vecprob->size();i++){
        temp += vecprob->at(i);
        if (randomnum < temp){
            //cout << i << endl;
            return i;
        }

    }
    return 0;

}
