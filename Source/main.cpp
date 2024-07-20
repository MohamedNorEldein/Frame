#define STRUCTUR_DEBUG

#include "../headers/Grid2D.h"
#include "../headers/Frame3D.h"

#include <iostream>


int main()
{

    printf("start\n");
   
   /******FRAME3D *******/

    Frame3D frame;

    frame.addNode({0,0,0});
    frame.addNode({0,0,10});
    frame.addNode({2,0,10});
   

    frame.addMember(0,1,200,120,10,1,1,1,Eigen::Vector<double, 3>(1,0,0));
    frame.addMember(1,2,200,120,10,1,1,1,Eigen::Vector<double, 3>(0,0,1));
    
    frame.addSupport(0,0,0);
    frame.addSupport(0,1,0);
    frame.addSupport(0,2,0);
    frame.addSupport(0,3,0);
    frame.addSupport(0,4,0);
    frame.addSupport(0,5,0);

    frame.addnodeForce(2,1,10);
    frame.solve();
    frame.print();

    return 0;
}

