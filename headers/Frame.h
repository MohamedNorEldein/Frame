#pragma once

#include "structure.h"

template <UCHAR DIMMINSION>
class Node
{

public:
    double pos[DIMMINSION];           //  position tensor
    double dis[DIMMINSION]{0};        //  displacment
    double nodalForce[DIMMINSION]{0}; //  nodal Force
    bool support[DIMMINSION]{0};

public:
    Node(std::initializer_list<double> data)
    {
        UCHAR i =0; 
        for(auto& a : data){
            pos[i] = a;
            i++;
        }
    }
   

    void setDis(UCHAR i, double value)
    {
        dis[i] = value;
        support[i] = 1;
    }

    void setNodalForce(UCHAR i, double value) { nodalForce[i] = value; }
    
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

template <UCHAR DIMMINSION>
class Member
{
public:
    size_t iNode, jNode;
    double IendForces[DIMMINSION]{0}, JendForces[DIMMINSION]{0};
    double IFixedForces[DIMMINSION]{0}, JFixedForces[DIMMINSION]{0};

    Eigen::MatrixX<double> K11, K12, K21, K22, TM, TMT;

    double l, E, A, I;

    Member(size_t iNode, size_t jNode, double E, double A, double I)
        : iNode(iNode), jNode(jNode),
          E(E), A(A), I(I),
          K11(DIMMINSION, DIMMINSION),
          K12(DIMMINSION, DIMMINSION),
          K21(DIMMINSION, DIMMINSION),
          K22(DIMMINSION, DIMMINSION),
          TM(DIMMINSION, DIMMINSION)
    {
        K11.setZero();
        K12.setZero();
        K21.setZero();
        K22.setZero();

        TM.setZero();
    }

    Member(Member &) = default;
    Member(Member &&) = default;

    ~Member() {}

public:
    Eigen::MatrixX<double> getGlopalK11()
    {
        return (TM * K11) * TMT;
    }
    Eigen::MatrixX<double> getGlopalK12()
    {
        return (TM * K12) * TMT;
    }
    Eigen::MatrixX<double> getGlopalK21()
    {
        return (TM * K21) * TMT;
    }
    Eigen::MatrixX<double> getGlopalK22()
    {
        return (TM * K22) * TMT;
    }

public:
    size_t getIndex_I()
    {
        return iNode * DIMMINSION;
    }
    size_t getIndex_J()
    {
        return jNode * DIMMINSION;
    }

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

template <UCHAR DIMMINSION>
class Frame
{
    std::vector<Node<DIMMINSION>> nodes;
    std::vector<Member<DIMMINSION>> members;

public:
    Frame()
    {
    }

    size_t addNode(std::initializer_list<double> data)
    {
        nodes.push_back(Node<DIMMINSION>(data));
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
        members.push_back(Member<DIMMINSION>(i, j, E, A, I));
        return members.size() - 1;
    }

    void buildMember(size_t index)
    {
        Member<DIMMINSION> &member = members[index];

        size_t i = member.iNode;
        size_t j = member.jNode;
        printf("%u %u \n", i, j);

        Node<DIMMINSION> &INode = nodes[i];
        Node<DIMMINSION> &JNode = nodes[j];

        double dx, dy, ex, ey;
        dx = INode.pos[0] - JNode.pos[0];
        dy = INode.pos[1] - JNode.pos[1];

        member.l = sqrt(dx * dx + dy * dy);
        ex = dx / member.l;
        ey = dy / member.l;

        member.TM.setZero();

        member.TM(0, 0) = ex;
        member.TM(0, 1) = -ey;
        member.TM(1, 0) = ey;
        member.TM(1, 1) = ex;
        member.TM(2, 2) = 1;

        member.TMT = member.TM.transpose();

        member.K11(0, 0) = member.E * member.A / member.l;
        member.K11(1, 1) = 12 * member.E * member.I / member.l / member.l / member.l;
        member.K11(1, 2) = 6 * member.E * member.I / member.l / member.l;
        member.K11(2, 1) = 6 * member.E * member.I / member.l / member.l;
        member.K11(2, 2) = 4 * member.E * member.I / member.l;

        member.K12(0, 0) = -member.E * member.A / member.l;
        member.K12(1, 1) = -12 * member.E * member.I / member.l / member.l / member.l;
        member.K12(1, 2) = 6 * member.E * member.I / member.l / member.l;
        member.K12(2, 1) = -6 * member.E * member.I / member.l / member.l;
        member.K12(2, 2) = 2 * member.E * member.I / member.l;

        member.K21(0, 0) = -member.E * member.A / member.l;
        member.K21(1, 1) = -12 * member.E * member.I / member.l / member.l / member.l;
        member.K21(1, 2) = -6 * member.E * member.I / member.l / member.l;
        member.K21(2, 1) = 6 * member.E * member.I / member.l / member.l;
        member.K21(2, 2) = 2 * member.E * member.I / member.l;

        member.K22(0, 0) = member.E * member.A / member.l;
        member.K22(1, 1) = 12 * member.E * member.I / member.l / member.l / member.l;
        member.K22(1, 2) = -6 * member.E * member.I / member.l / member.l;
        member.K22(2, 1) = -6 * member.E * member.I / member.l / member.l;
        member.K22(2, 2) = 4 * member.E * member.I / member.l;
    }

    void solve()
    {
        printf("start solving\n");
        structure system(nodes.size() * DIMMINSION);

        for (size_t i = 0; i < members.size(); i++)
        {
            Member<DIMMINSION> &m = members[i];
            buildMember(i);

            system.StructureAddStiffMat( m.iNode * DIMMINSION, m.jNode * DIMMINSION, m.getGlopalK11(), m.getGlopalK12(), m.getGlopalK21(), m.getGlopalK22());
        }

        printf("build matrix\n");

        for (size_t i = 0; i < nodes.size(); i++)
        {
            for (size_t j = 0; j < DIMMINSION; j++)
            {
                if (nodes[i].support[j] == 1)
                    system.StructureAddDis( i * DIMMINSION + j, nodes[i].dis[j]);

                system.StructureAddForce( i * DIMMINSION + j, nodes[i].nodalForce[j]);
            }
        }
        printf("solve structure system\n");

        system.solve();

        printf("read solution\n");

        for (size_t i = 0; i < nodes.size(); i++)
        {
            for (size_t j = 0; j < DIMMINSION; j++)
            {

                nodes[i].dis[j] = system.StructureGetDis( i * DIMMINSION + j);
            }
        }

        printf("out solution\n");

        Eigen::MatrixX<double> y1(DIMMINSION, 1), y2(DIMMINSION, 1);
        Eigen::MatrixX<double> p1(DIMMINSION, 1), p2(DIMMINSION, 1);

        for (size_t i = 0; i < members.size(); i++)
        {
            Member<DIMMINSION> &m = members[i];
            memcpy(y1.data(), nodes[m.iNode].dis, DIMMINSION * sizeof(m.JendForces[0]));
            memcpy(y2.data(), nodes[m.jNode].dis, DIMMINSION * sizeof(m.IendForces[0]));

            p1 = m.K11 * m.TMT * y1 + m.K12 * m.TMT * y2;
            p2 = (m.K21 * m.TMT * y1 + m.K22 * m.TMT * y2) * (-1);

            memcpy(m.IendForces, p1.data(), DIMMINSION * sizeof(m.JendForces[0]));
            memcpy(m.JendForces, p2.data(), DIMMINSION * sizeof(m.IendForces[0]));
        }
        printf("finish solving\n");
    }

    void print()
    {
        printf("-------------Nodes----------------------\n");

        for (Node<DIMMINSION> &n : nodes)
        {
            n.print();
        }
        printf("-------------Members----------------------\n");
        for (Member<DIMMINSION> &m : members)
        {
            m.print();
        }
    }
};

class Frame2D : public Frame<3>
{
};


class Frame3D : public Frame<3>
{
};





















