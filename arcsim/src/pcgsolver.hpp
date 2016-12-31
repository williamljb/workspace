#ifndef PCGSOLVER_HPP
#define PCGSOLVER_HPP
#include "sparse.hpp"
#include "vectors.hpp"
using namespace std;
vector<double> pcgsolver_linear_solve(const SpMat<double> &A, const vector<double> &b);


template <int m> std::vector< Vec<m> > pcgsolver_linear_solve
    (const SpMat< Mat<m,m> > &A, const std::vector< Vec<m> > &b);


#endif
