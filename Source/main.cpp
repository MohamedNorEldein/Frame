#define STRUCTUR_DEBUG

#include "../headers/Frame.h"

#include <iostream>

int main(){


    printf("start\n");
    Frame2D frame;

    frame.addNode({0,0});
    frame.addNode({0,1});
    frame.addNode({0,2});

    frame.addNode({0,3});
    frame.addNode({1,3});
    frame.addNode({2,3});
    frame.addNode({3,3});

    frame.addNode({3,2});
    frame.addNode({3,1});
    frame.addNode({3,0});

    frame.addMember(0,1,1,100,1);
    frame.addMember(1,2,1,100,1);
    frame.addMember(2,3,1,100,1);
    frame.addMember(3,4,1,100,1);
    frame.addMember(4,5,1,100,1);
    frame.addMember(5,6,1,100,1);
    frame.addMember(6,7,1,100,1);
    frame.addMember(7,8,1,100,1);
    frame.addMember(8,9,1,100,1);

    frame.addSupport(0,0,0);
    frame.addSupport(0,1,0);
    frame.addSupport(9,1,0);

    frame.addnodeForce(3,1,-1);
    frame.addnodeForce(4,1,-1);
    frame.addnodeForce(5,1,-1);
    frame.addnodeForce(6,1,-1);

    frame.solve();
    frame.print();


    return 0;
}