#define STRUCTUR_DEBUG

#include "../headers/Grid2D.h"

#include <iostream>

int main()
{

    printf("start\n");
    Grid2D frame;

    frame.addNode({0, 0});
   frame.addNode({5, 0});
    frame.addNode({10, 0});
    
    frame.addNode({0, 5});
    frame.addNode({5, 5});
    frame.addNode({10, 5});

    frame.addNode({0, 10});
    frame.addNode({5, 10});
    frame.addNode({10, 10});

    frame.addMember(0, 1, 3, 1, 1, 0.001);
    frame.addMember(1, 2, 3, 1, 1, 0.001);

    frame.addMember(3, 4, 3, 1, 1,0.001);
    frame.addMember(4, 5, 3, 1, 1,0.001);
    frame.addMember(6, 7, 3, 1, 1,0.001);
    frame.addMember(7, 8, 3, 1, 1,0.001);

    frame.addMember(0, 3, 3, 1, 1,0.001);
    frame.addMember(3, 6, 3, 1, 1,0.001);
    frame.addMember(1, 4, 3, 1, 1,0.001);
    frame.addMember(4, 7, 3, 1, 1,0.001);
    frame.addMember(2, 5, 3, 1, 1,0.001);
    frame.addMember(5, 8, 3, 1, 1,0.001);

    frame.addSupport(0, 0, 0);
    frame.addSupport(2, 0, 0);
    frame.addSupport(6, 0, 0);
    frame.addSupport(8, 0, 0);


    frame.addnodeForce(4, 0, -1);

    frame.solve();
    frame.print();

    return 0;
}