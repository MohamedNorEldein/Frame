

/******************************************************************/
/****FRAME 2D*************************/
#pragma once
#include "Frame.h"
#include <iostream>

class NodeG2D : public Node<6>
{
public:
    NodeG2D(std::initializer_list<double> data) : Node(data)
    {
    }
    ~NodeG2D() = default;
    void print()
    {
        printf("Node\n"

               "\tposition \tx = %f \ty = %f\n"
               "\tnodal force \t px = %f \t py = %f \t pz = %f\n"
               "\tnodal Moment \t Mx = %f \t My = %f \t Mz = %f\n"

               "\tdisplacment \t dx = %f \t dy = %f \t dz = %f\n",
               "\tRotation \t Rx = %f \t Ry = %f \t Rz = %f\n",

               pos[0], pos[1], pos[2],
               nodalForce[0], nodalForce[1], nodalForce[2],
               nodalForce[3], nodalForce[4], nodalForce[5],
               dis[0], dis[1], dis[2],
               dis[3], dis[4], dis[5]

        );
    }
};

class MemberG2D : public Member<3>
{
public:
    double l, E, G, Ix, Iy, J;

    MemberG2D(size_t iNode, size_t jNode, double E, double G, double Ix, double Iy, double J)
        : Member<3>(iNode, jNode), E(E), G(G), Ix(Ix), Iy(Iy), J(J)
    {
        K11.setZero();
        K12.setZero();
        K21.setZero();
        K22.setZero();

        TM.setZero();
    }

    MemberG2D(MemberG2D &) = default;
    MemberG2D(MemberG2D &&) = default;

    ~MemberG2D() {}

public:
    void print()
    {
        printf("member\n"
               "\tstart node %d , end node %d \n"
               "\tE = %f \tG=%f \tIx=%f\tIy = %f J=%f\n "
               "\tstart force in member \tN = %f \tQX = %f \tQy = %f\n"
               "\tstart force in member \tT = %f \tMX = %f \tMY = %f\n"
               "\tstart force in member \tN = % f \tQX = % f \tQy = % f\n "
               "\tstart force in member \tT = %f \tMX = %f \tMY = %f\n",

               iNode, jNode, E, G, Ix, Iy, J,
               IendForces[0], IendForces[1], IendForces[2],
               IendForces[3], IendForces[4], IendForces[5],

               JendForces[0], JendForces[1], JendForces[2],
               JendForces[3], JendForces[4], JendForces[5]

        );
    }
};

class Grid2D : public Frame<NodeG2D, MemberG2D, 3>
{
public:
    Grid2D() : Frame<NodeG2D, MemberG2D, 3>()
    {
    }
    ~Grid2D() = default;

public:
    size_t addNode(std::initializer_list<double> data)
    {
        nodes.push_back(NodeG2D(data));
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
    size_t addMember(size_t i, size_t j, double E, double G, double Ix, double Iy, double J)
    {
        members.push_back(MemberG2D(i, j, E, G, Ix, Iy, J));
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
        dy = INode.pos[2] - JNode.pos[2];

        member.l = sqrt(dx * dx + dy * dy + dz * dz);
        ex = dx / member.l;
        ey = dy / member.l;
        ez = dz / member.l;

        member.TM.setZero();

        // MT.....

        member.TMT = member.TM.transpose();

        double Ax, Bx, Cx, Ay, By, Cy, G;

        Ax = 12 * member.E * member.Ix / member.l / member.l / member.l;
        Bx = 6 * member.E * member.Ix / member.l / member.l;
        Cx = 4 * member.E * member.Ix / member.l;

        Ay = 12 * member.E * member.Ix / member.l / member.l / member.l;
        By = 6 * member.E * member.Ix / member.l / member.l;
        Cy = 4 * member.E * member.Ix / member.l;

        G = 4 * member.G * member.J / member.l;
        //K.....
    }
};