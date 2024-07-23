#pragma once
#include <iostream>
#include "structure.h"

template <UCHAR DIMMINSION>
class Node
{
public:
    Eigen::Vector<double, DIMMINSION> pos;        //  position tensor
    Eigen::Vector<double, DIMMINSION> dis;        //  displacment
    Eigen::Vector<double, DIMMINSION> nodalForce; //  nodal Force
    Eigen::Vector<bool, DIMMINSION> support;

public:
    Node(std::initializer_list<double> data) : dis(), pos(), nodalForce(), support()
    {
        UCHAR i = 0;
        for (auto &a : data)
        {
            pos[i] = a;
            i++;
        }

        dis.setZero();
        nodalForce.setZero();
        support.setZero();
    }
    ~Node() = default;

    void setRestrain(UCHAR i, double value)
    {
        dis[i] = value;
        support[i] = 1;
    }

    void setNodalForce(UCHAR i, double value) { nodalForce[i] = value; }
    double getNodalForce(UCHAR i) { return nodalForce[i]; }

    
    static const size_t const getDIMMINSION()
    {
        return DIMMINSION;
    }
};

template <UCHAR DIMMINSION>
class Member
{
public:
    size_t iNode, jNode;
    Eigen::Vector<double, DIMMINSION> IendForces, JendForces;
    Eigen::Vector<double, DIMMINSION> IFixedForces, JFixedForces;
    Eigen::Matrix<double, DIMMINSION, DIMMINSION> K11, K12, K21, K22, TM, TMT;

    Member(size_t iNode, size_t jNode)
        : iNode(iNode), jNode(jNode)

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
    Eigen::Matrix<double, DIMMINSION, DIMMINSION> getGlopalK11()
    {

        return (TMT * K11) * TM;
    }
    Eigen::Matrix<double, DIMMINSION, DIMMINSION> getGlopalK12()
    {
        return (TMT * K12) * TM;
    }
    Eigen::Matrix<double, DIMMINSION, DIMMINSION> getGlopalK21()
    {
        return (TMT * K21) * TM;
    }
    Eigen::Matrix<double, DIMMINSION, DIMMINSION> getGlopalK22()
    {
        return (TMT * K22) * TM;
    }

public:
    size_t getIndex_I()
    {
        return iNode * IendForces.size();
    }
    size_t getIndex_J()
    {
        return jNode * IendForces.size();
    }
};

template <typename NODE, typename MEMBER, UCHAR DIMMINSION>
class Frame
{
protected:
    std::vector<NODE> nodes;
    std::vector<MEMBER> members;

public:
    Frame()
    {
    }

public:
    size_t getMemberCount()
    {
        return members.size();
    }
    size_t getNodeCount()
    {
        return nodes.size();
    }
    MEMBER &getMember(UCHAR memberId)
    {
        return members[memberId];
    }

    NODE &getNode(UCHAR nodeId)
    {
        return nodes[nodeId];
    }

public:
    virtual void buildMember(size_t index) = 0;

    void solve()
    {
        printf("start solving\n");
        structure system(nodes.size() * DIMMINSION);

        for (size_t i = 0; i < members.size(); i++)
        {
            auto &m = members[i];
            buildMember(i);
            system.StructureAddStiffMat(m.iNode * DIMMINSION, m.jNode * DIMMINSION, m.getGlopalK11(), m.getGlopalK12(), m.getGlopalK21(), m.getGlopalK22());
        }

        printf("build matrix %d\n", DIMMINSION);

        for (size_t i = 0; i < nodes.size(); i++)
        {
            for (size_t j = 0; j < DIMMINSION; j++)
            {
                if (nodes[i].support[j] == 1)
                    system.StructureAddDis(i * DIMMINSION + j, nodes[i].dis[j]);

                system.StructureAddForce(i * DIMMINSION + j, nodes[i].nodalForce[j]);
            }
        }
        printf("solve structure system\n");

        system.solve();

        printf("read solution\n");

        for (size_t i = 0; i < nodes.size(); i++)
        {
            for (size_t j = 0; j < DIMMINSION; j++)
            {

                nodes[i].dis[j] = system.StructureGetDis(i * DIMMINSION + j);
            }
        }

        printf("out solution\n");

        Eigen::Vector<double, DIMMINSION> y1, y2;
        Eigen::Vector<double, DIMMINSION> p1, p2;

        for (size_t i = 0; i < members.size(); i++)
        {
            auto &m = members[i];
            y1 = nodes[m.iNode].dis;
            y2 = nodes[m.jNode].dis;

            p1 = m.K11 * m.TM * y1 + m.K12 * m.TM * y2;
            p2 = (m.K21 * m.TM * y1 + m.K22 * m.TM * y2) * (-1);

            m.IendForces = p1;
            m.JendForces = p2;
        }
        printf("finish solving\n");
    }

    void print()
    {
        printf("-------------Nodes----------------------\n");

        for (auto &n : nodes)
        {
            n.print();
        }
        printf("-------------Members----------------------\n");
        for (auto &m : members)
        {
            m.print();
        }
    }

    double getNodeDis(size_t index, UCHAR i)
    {
        return nodes[index].dis[i];
    }

    double getMemberInternalAction_Start(size_t index, UCHAR i)
    {
        return members[index].IendForces[i];
    }

    double getMemberInternalAction_End(size_t index, UCHAR i)
    {
        return members[index].JendForces[i];
    }
};
