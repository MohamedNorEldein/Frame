
/******************************************************************/
/****FRAME 2D*************************/
#pragma once 
#include "Frame.h"

class NodeF2D : public Node<3>
{
public:
    NodeF2D(std::initializer_list<double> data) : Node(data)
    {
    }
    ~NodeF2D() = default;
    void print()
    {
        printf("Node\n"

               "\tposition \tx = %f \ty = %f\n"
               "\tnodal force \tpx = %f \tpy = %f \tM = %f\n"
               "\tdisplacment \tdx = %f \tdy = %f \tdR = %f\n",

               pos[0], pos[1],
               nodalForce[0], nodalForce[1], nodalForce[2],
               dis[0], dis[1], dis[2]

        );
    }
};

class MemberF2D : public Member<3>
{
public:
    double l, E, A, I;

    MemberF2D(size_t iNode, size_t jNode, double E, double A, double I)
        : Member<3>(iNode, jNode), E(E), A(A), I(I)
    {
        K11.setZero();
        K12.setZero();
        K21.setZero();
        K22.setZero();

        TM.setZero();
    }

    MemberF2D(MemberF2D &) = default;
    MemberF2D(MemberF2D &&) = default;

    ~MemberF2D() {}

public:
    void print()
    {
        printf("member\n"
               "\tstart node %d , end node %d \n"
               "\tE = %f \tA=%f \tI=%f\n"
               "\tstart force in member \tN = %f \tQ = %f \tM = %f\n"
               "\tend force in member \tN = %f \tQ = %f \tM = %f\n",

               iNode, jNode, E, A, I,
               IendForces[0], IendForces[1], IendForces[2],
               JendForces[0], JendForces[1], JendForces[2]

        );
    }
};

class Frame2D : public Frame<NodeF2D, MemberF2D, 3>
{
public:
    Frame2D() : Frame<NodeF2D, MemberF2D, 3>()
    {
    }
    ~Frame2D() = default;

public:


    size_t addNode(std::initializer_list<double> data)
    {
        nodes.push_back(NodeF2D(data));
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
    size_t addMember(size_t i, size_t j, double E, double A, double I)
    {
        members.push_back(MemberF2D(i, j, E, A, I));
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

        member.TM << ex, ey, 0, -ey, ex, 0, 0, 0, 1;

        member.TMT = member.TM.transpose();

        double R, A, B, C;
        R = member.E * member.A / member.l;
        A = 12 * member.E * member.I / member.l / member.l / member.l;
        B = 6 * member.E * member.I / member.l / member.l;
        C = 4 * member.E * member.I / member.l;

        member.K11 << R, 0, 0, 0, A, B, 0, B, C;
        member.K12 << -R, 0, 0, 0, -A, B, 0, -B, C / 2;
        member.K21 << -R, 0, 0, 0, -A, -B, 0, B, C / 2;
        member.K22 << R, 0, 0, 0, A, -B, 0, -B, C;
    }
};