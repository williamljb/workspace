#include "pcgsolver.hpp"
#include "sparse_matrix.h"
#include "pcg_solver.h"
#include <iostream>
using namespace std;
void sparse_to_pcgsparse(const SpMat<double> &As, SparseMatrix<double>* pcgA){
    int n = As.n;
    #pragma omp parallel for schedule(guided) num_threads(8)
    for(int i=0;i<n;i++){
        for(int k=0;k<As.rows[i].indices.size();k++){
            int j = As.rows[i].indices[k];
            pcgA->add_to_element(i,j,As.rows[i].entries[k]);
        }
    }
}

template <int m>
void sparse_to_pcgsparse(const SpMat< Mat<m,m> > &As, SparseMatrix<double>* pcgA){
    int n = As.n;
    pcgA->zero();
    #pragma omp parallel for schedule(guided) num_threads(8)
    for(int i = 0;i<n;i++){
        for(int jj=0;jj<As.rows[i].indices.size();jj++){
            int j = As.rows[i].indices[jj];
            const Mat<m,m> &Aij = As.rows[i].entries[jj];
            for(int k=0;k<m;k++){
                for(int l=0; l<m; l++){
                    pcgA->add_to_element(i*m+k, j*m+l, Aij(k,l));
                }
            }
        }
    }
}

template <int m>
void rhs_to_pcgrhs(const vector< Vec<m> > &b, vector<double> & pcgB){
    int n = b.size();
    for(int i=0;i<n;i++){
        Vec<m> bi = b[i];
        for(int k=0;k<m;k++)
            pcgB.push_back(bi[k]);
    }
}

template<int m>
void pcgX_to_x(const vector<double> &pcgX, vector< Vec<m> > &x){
    #pragma omp parallel for schedule(guided) num_threads(8)
    for(size_t i=0;i<pcgX.size()/m;i++){
        Vec<m> ele;
        for(int k=0;k<m;k++)
            ele[k] = pcgX[i*m+k];
        x[i] = ele;
    }
}


vector<double> pcgsolver_linear_solve(const SpMat<double> &A, const vector<double> &b){
    SparseMatrix<double> *pcgA = new SparseMatrix<double>(A.n, 0);
    sparse_to_pcgsparse(A, pcgA);
    vector<double> x(b.size());
    PCGSolver<double> pcgSolver;
    double residual;
    int niterations;
    pcgSolver.solve(*pcgA, b, x, residual, niterations);
    delete pcgA;
    return x;
}

template <int m> std::vector< Vec<m> > pcgsolver_linear_solve
    (const SpMat< Mat<m,m> > &A, const std::vector< Vec<m> > &b){
    SparseMatrix<double> *pcgA = new SparseMatrix<double>(A.n*m,0);
    vector<double> pcgX(b.size()*m);
    vector<double> pcgRhs;
    sparse_to_pcgsparse(A, pcgA);
    rhs_to_pcgrhs(b, pcgRhs);
    vector< Vec<m> > x(b.size());
    PCGSolver<double> pcgSolver;
    double residual;
    int niterations;
    ofstream fout("pcgA.mat");
    pcgA->write_matlab(fout,"pcgA");
    pcgSolver.solve(*pcgA, pcgRhs, pcgX, residual, niterations);
    pcgX_to_x(pcgX, x);
    delete pcgA;
    return x;
}

template vector<Vec3> pcgsolver_linear_solve (const SpMat<Mat3x3> &A,
                                              const vector<Vec3> &b);

