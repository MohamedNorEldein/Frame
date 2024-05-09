

/******************************************************************/
/****FRAME 2D*************************/
#pragma once
#include "Frame.h"
#include <iostream>

class NodeG2D : public Node<3>
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
               "\tnodal force \t pz = %f \t M = %f \t T = %f\n"
               "\tdisplacment \t dz = %f \t RX = %f \t Ry = %f\n",

               pos[0], pos[1],
               nodalForce[0], nodalForce[1], nodalForce[2],
               dis[0], dis[1], dis[2]

        );
    }
};

class MemberG2D : public Member<3>
{
public:
    double l, E, G, Ix, J;

    MemberG2D(size_t iNode, size_t jNode, double E, double G, double Ix, double J)
        : Member<3>(iNode, jNode), E(E), G(G), Ix(Ix), J(J)
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
               "\tE = %f \tG=%f \tIx=%f\t J=%f\n "
               "\tstart force in member \tQ = %f \tM = %f \tT = %f\n"
               "\tend force in member \tQ = %f \tM = %f \tT = %f\n",

               iNode, jNode, E, G, Ix, J,
               IendForces[0], IendForces[1], IendForces[2],
               JendForces[0], JendForces[1], JendForces[2]

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
    size_t addMember(size_t i, size_t j, double E, double G, double I, double J)
    {
        members.push_back(MemberG2D(i, j, E, G, I, J));
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

        double dx, dy, ex, ey;
        dx = INode.pos[0] - JNode.pos[0];
        dy = INode.pos[1] - JNode.pos[1];

        member.l = sqrt(dx * dx + dy * dy);
        ex = dx / member.l;
        ey = dy / member.l;

        member.TM.setZero();

        member.TM << 1, 0, 0, 0, ey, ex, 0, -ex, ey;
        
        member.TMT = member.TM.transpose();

        double A, B, C, G;

        A = 12 * member.E * member.Ix / member.l / member.l / member.l;
        B = 6 * member.E * member.Ix / member.l / member.l;
        C = 4 * member.E * member.Ix / member.l;
        G = 4 * member.G * member.J / member.l;

        member.K11 << A, B, 0, B, C, 0, 0, 0, G;
        member.K12 << -A, B, 0, -B, C / 2, 0, 0, 0, -G;
        member.K21 << -A, -B, 0, B, C / 2, 0, 0, 0, -G;
        member.K22 << A, -B, 0, -B, C, 0, 0, 0, G;
    }
};