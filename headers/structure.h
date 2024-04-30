#pragma once
#include "../Eigen/Sparse"
#include "../Eigen/Dense"
#include <vector>

typedef unsigned char UCHAR;

class structure
{
    Eigen::VectorX<double> y;
    Eigen::VectorX<double> p; // displacment , External Forces
    Eigen::MatrixX<double> K; // stiffness      K =    [k11    k12]
                              //                  [K21    K22]
    Eigen::VectorX<double> isSupport;
    size_t supports;

public:
    //------------STRUCTURE-----------------------
    structure(size_t DF) : y(DF, 1), p(DF, 1), K(DF, DF), isSupport(DF, 1), supports(0)
    {
        y.setZero();
        p.setZero();
        K.setZero();
        isSupport.setZero();
    }

    void StructureAddStiffMat(size_t i, size_t j, const Eigen::MatrixX<double> &Aii, const Eigen::MatrixX<double> &Aij, const Eigen::MatrixX<double> &Aji, const Eigen::MatrixX<double> &Ajj);
    void StructureAddForce(size_t i, double f);
    void StructureAddDis(size_t i, double dis);

    double StructureGetDis(size_t i);

    void print();

    void solve();
};