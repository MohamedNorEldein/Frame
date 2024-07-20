
/******************************************************************/
/****FRAME 2D*************************/
#pragma once
#include "Frame.h"

class NodeF3D : public Node<6>
{
public:
    NodeF3D(std::initializer_list<double> data) : Node(data)
    {
    }
    ~NodeF3D() = default;
    void print()
    {
        printf("Node\n"

               "\tposition \tx = %f \ty = %f\tz=%f\n"
               "\tnodal force \tpx = %f \tpy = %f \tpz = %f\n"
               "\tnodal Moment \tMx = %f \tMy = %f \tMz = %f\n"
               "\tdisplacment \tdx = %f \tdy = %f \tdz = %f\n",
               "\tdisplacment \tRx = %f \tRy = %f \tRz = %f\n",

               pos[0], pos[1], pos[2],
               nodalForce[0], nodalForce[1], nodalForce[2],
               nodalForce[3], nodalForce[4], nodalForce[5],

               dis[0], dis[1], dis[2],
               dis[3], dis[4], dis[5]

        );
    }
};

class MemberF3D : public Member<6u>
{
public:
    double l, E, G, A, Ix, Iy, J;
    Eigen::Vector<double, 3> LocalX;

    MemberF3D(size_t iNode, size_t jNode, double E, double G, double A, double Ix, double Iy, double J, const Eigen::Vector<double, 3> &Localx)
        : Member<6>(iNode, jNode), E(E), G(G), A(A), Ix(Ix), Iy(Iy), J(J)
    {
        K11.setZero();
        K12.setZero();
        K21.setZero();
        K22.setZero();

        TM.setZero();
        LocalX = Localx;
    }

    MemberF3D(MemberF3D &) = default;
    MemberF3D(MemberF3D &&) = default;

    ~MemberF3D() {}

public:
    void print()
    {
        printf("member\n"
                "\tstart node %d , end node %d \n"
                "\tE = %f \tG=%f\tA=%f \n"
                "\tIx=%f\tIy=%f\tJ=%f\n"
                "\tstart force in member \tQx = %f \tQy = %f \tN = %f\n"
                "\tstart force in member \t Mx= %f \tMy = %f \tT = %f\n"
                
                "\n\tend force in member \tQx = %f \tQy = %f \tN = %f\n"
                "\tend force in member \t Mx= %f \tMy = %f \tT = %f\n",
               

               iNode, jNode, E,G, A, Ix, Iy, J,
               IendForces[0], IendForces[1], IendForces[2],
               IendForces[3], IendForces[4], IendForces[5],
               JendForces[0], JendForces[1], JendForces[2],
               JendForces[3], JendForces[4], JendForces[5]

        );
    }
};

class Frame3D : public Frame<NodeF3D, MemberF3D, 6>
{
public:
    Frame3D() : Frame<NodeF3D, MemberF3D, 6>()
    {
    }
    ~Frame3D() = default;

public:
    size_t addNode(std::initializer_list<double> data)
    {
        nodes.push_back(NodeF3D(data));
        return nodes.size() - 1;
    }
    void addnodeForce(size_t nodeIndex, size_t forceIndex, double force)
    {
        nodes[nodeIndex].nodalForce[forceIndex] = force;
    }
    void addSupport(size_t nodeIndex, size_t disIndex, double dis)
    {
        nodes[nodeIndex].dis[disIndex] = dis;
        nodes[nodeIndex].support[disIndex] = 1;
    }
    size_t addMember(size_t i, size_t j, double E, double G, double A, double Ix, double Iy, double J, Eigen::Vector<double, 3> LocalX)
    {
        members.push_back(MemberF3D(i, j, E, G, A, Ix, Iy, J, LocalX));
        return members.size() - 1;
    }

public:
    void buildMember(size_t index)
    {

        auto &member = members[index];

        size_t i = member.iNode;
        size_t j = member.jNode;
        printf("%u %u \n", i, j);

        auto &INode = nodes[i];
        auto &JNode = nodes[j];

        double dx, dy, dz, ex, ey, ez;
        dx = INode.pos[0] - JNode.pos[0];
        dy = INode.pos[1] - JNode.pos[1];
        dz = INode.pos[2] - JNode.pos[2];

        member.l = sqrt(dx * dx + dy * dy + dz * dz);
        ex = dx / member.l;
        ey = dy / member.l;
        ez = dz / member.l;

        Eigen::Vector<double, 3> LocalY, LocalZ(ex, ey, ez);
        Eigen::Vector<double, 3> &LocalX = member.LocalX;
        
        //create and normalize local system
        LocalY = LocalX.cross(LocalZ);
        LocalY.normalize();
        LocalX = LocalZ.cross(LocalY);

        member.TM << 
            LocalX[0], LocalY[0], LocalZ[0], 0, 0, 0,
            LocalX[1], LocalY[1], LocalZ[1], 0, 0, 0,
            LocalX[2], LocalY[2], LocalZ[2], 0, 0, 0,

            0, 0, 0, LocalX[0], LocalY[0], LocalZ[0],
            0, 0, 0, LocalX[1], LocalY[1], LocalZ[1],
            0, 0, 0, LocalX[2], LocalY[2], LocalZ[2];

        member.TMT = member.TM.transpose();

        double S, Ax, Bx, Cx, Ay, By, Cy, G;
        S = member.E * member.A / member.l;
        G = member.G * member.J / member.l;

        Ax = 12 * member.E * member.Ix / member.l / member.l / member.l;
        Bx = 6 * member.E * member.Ix / member.l / member.l;
        Cx = 4 * member.E * member.Ix / member.l;

        Ay = 12 * member.E * member.Iy / member.l / member.l / member.l;
        By = 6 * member.E * member.Iy / member.l / member.l;
        Cy = 4 * member.E * member.Iy / member.l;

        //printf("s = %f\n", S);
        /******************** K11 ************************/
        member.K11(0, 0) = Ay;
        member.K11(0, 4) = By;
        member.K11(4, 0) = By;
        member.K11(4, 4) = Cy;

        member.K11(1, 1) = Ax;
        member.K11(1, 3) = Bx;
        member.K11(3, 1) = Bx;
        member.K11(3, 3) = Cx;

        member.K11(2, 2) = S;

        member.K11(5, 5) = G;

        /******************** K11 ************************/
        member.K12(0, 0) = -Ay;
        member.K12(0, 4) = By;
        member.K12(4, 0) = -By;
        member.K12(4, 4) = Cy / 2;

        member.K12(1, 1) = -Ax;
        member.K12(1, 3) = Bx;
        member.K12(3, 1) = -Bx;
        member.K12(3, 3) = Cx / 2;

        member.K12(2, 2) = -S;

        member.K12(5, 5) = -G;

        /******************** K11 ************************/
        member.K21(0, 0) = -Ay;
        member.K21(0, 4) = -By;
        member.K21(4, 0) = By;
        member.K21(4, 4) = Cy / 2;

        member.K21(1, 1) = -Ax;
        member.K21(1, 3) = -Bx;
        member.K21(3, 1) = Bx;
        member.K21(3, 3) = Cx / 2;

        member.K21(2, 2) = -S;

        member.K21(5, 5) = -G;
        /******************** K11 ************************/
        member.K22(0, 0) = Ay;
        member.K22(0, 4) = -By;
        member.K22(4, 0) = -By;
        member.K22(4, 4) = Cy;

        member.K22(1, 1) = Ax;
        member.K22(1, 3) = -Bx;
        member.K22(3, 1) = -Bx;
        member.K22(3, 3) = Cx;

        member.K22(2, 2) = S;

        member.K22(5, 5) = G;
        
    
    }
};