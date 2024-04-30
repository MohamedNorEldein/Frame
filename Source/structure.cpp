
#include "../headers/structure.h"
#include <iostream>

void addSubSpace(Eigen::MatrixX<double> &K, size_t si, size_t sj, const Eigen::MatrixX<double> &Aij)
{
    for (size_t i = 0; i < Aij.cols(); i++)
    {
        for (size_t j = 0; j < Aij.rows(); j++)
        {
            // Add each element of Aij to the corresponding position in K, offset by si
            K(si + i, sj + j) += Aij(i, j);
        }
    }
}

void structure::StructureAddStiffMat(size_t i, size_t j, const Eigen::MatrixX<double> &Aii, const Eigen::MatrixX<double> &Aij, const Eigen::MatrixX<double> &Aji, const Eigen::MatrixX<double> &Ajj)
{
    addSubSpace((K), i, i, Aii);
    addSubSpace((K), i, j, Aij);
    addSubSpace((K), j, i, Aji);
    addSubSpace((K), j, j, Ajj);
}

void structure::StructureAddForce(size_t i, double f)
{
    p(i) += f;
}

void structure::StructureAddDis(size_t i, double dis)
{
    y(i) += dis;
    isSupport(i) = 1;
    supports++;
}

double structure::StructureGetDis(size_t i)
{
    return y[i];
}

void structure::print()
{
    std::cout << (K) << "\n";
}

void structure::solve()
{
    size_t DF = K.cols() - supports;

    std::vector<size_t> map_a, map_b;
    Eigen::MatrixX<double> K11(DF, DF), K12(DF, supports);
    Eigen::VectorX<double> y1(DF, 1), y2(supports, 1);
    Eigen::VectorX<double> p1(DF, 1), p2(supports, 1);

    K11.setZero();
    K12.setZero();

    size_t i = 0, j = 0, s = 0;

    // decompose
    for (size_t i = 0; i < K.cols(); i++)
    {
        if (isSupport[i] == 0)
        {
            map_a.push_back(i);
            j++;
        }
        else
        {
            map_b.push_back(i);
            s++;
        }
    }

    for (i = 0; i < DF; i++)
    {
        for (j = 0; j < DF; j++)
        {
            K11(i, j) = K(map_a[i], map_a[j]);
        }
        y1[i] = y[map_a[i]];
        p1[i] = p[map_a[i]];
    }

    for (i = 0; i < DF; i++)
    {
        for (j = 0; j < supports; j++)
        {
            K12(i, j) = K(map_a[i], map_b[j]);
        }
    }

    for (i = 0; i < supports; i++)
    {
        y2[i] = y[map_b[i]];
        p2[i] = p[map_b[i]];
    }

    // solve

    y1 = K11.inverse() * (p1 - K12 * y2);

    // out put
    for (i = 0; i < DF; i++)
    {
        y[map_a[i]] = y1[i];
    }
    p = K * y;
}
