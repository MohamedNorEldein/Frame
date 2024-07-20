

#define CDLL

//#include "../headers/Frame.h"
#include "../headers/Frame2D.h"


CDLL void* CreateFrame2D(void){
    return new Frame2D;
} 


CDLL size_t Frame2DAddNode(void*f, double x, double y){
    Frame2D* frame = (Frame2D*)f;
    return frame->addNode({x,y});
} 


CDLL size_t Frame2DAddMember(void*f, size_t i, size_t j, double E, double A, double I){
    Frame2D* frame = (Frame2D*)f;
    return frame->addMember(i,j,E,A,I);
} 


CDLL void Frame2DAddSupport(void*f, size_t i, size_t j, double dis){
    Frame2D* frame = (Frame2D*)f;
     frame->addSupport(i,j,dis);
} 

CDLL void Frame2DAddNodalForce(void*f, size_t i, size_t j, double Force){
    Frame2D* frame = (Frame2D*)f;
    frame->addnodeForce(i,j,Force);
} 

CDLL void Frame2DSolve(void*f){
    Frame2D* frame = (Frame2D*)f;
    frame->solve();
} 

CDLL void Frame2DPrint(void*f){
    Frame2D* frame = (Frame2D*)f;
    frame->print();
} 


CDLL void DeleteFrame2D(void*f){
    Frame2D* frame = (Frame2D*)f;
    delete frame;
} 

CDLL double Frame2DgetNodeDis(void*f,size_t index,UCHAR j ){
    Frame2D* frame = (Frame2D*)f;
    return frame->getNodeDis(index,j);
} 


/*****************FRAME3D***********************/
#include "../headers/Frame3D.h"

CDLL void* CreateFrame3D(void){
    return new Frame3D;
} 


CDLL size_t Frame3DAddNode(void*f, double x, double y, double z){
    Frame3D* frame = (Frame3D*)f;
    return frame->addNode({x,y,z});
} 


CDLL size_t Frame3DAddMember(void*f, size_t i, size_t j, double E,double G, double A, double Ix, double Iy, double J, double LocalXx,double LocalXy,double LocalXz){
    Frame3D* frame = (Frame3D*)f;
    return frame->addMember(i,j,E,G,A,Ix,Iy,J,Eigen::Vector<double,3>(LocalXx,LocalXy,LocalXz));
} 


CDLL void Frame3DAddSupport(void*f, size_t i, size_t j, double dis){
    Frame3D* frame = (Frame3D*)f;
     frame->addSupport(i,j,dis);
} 

CDLL void Frame3DAddNodalForce(void*f, size_t i, size_t j, double Force){
    Frame3D* frame = (Frame3D*)f;
    frame->addnodeForce(i,j,Force);
} 

CDLL void Frame3DSolve(void*f){
    Frame3D* frame = (Frame3D*)f;
    frame->solve();
} 

CDLL void Frame3DPrint(void*f){
    Frame3D* frame = (Frame3D*)f;
    frame->print();
} 


CDLL void DeleteFrame3D(void*f){
    Frame3D* frame = (Frame3D*)f;
    delete frame;
} 

CDLL double Frame3DgetNodeDis(void*f,size_t index,UCHAR j ){
    Frame3D* frame = (Frame3D*)f;
    return frame->getNodeDis(index,j);
} 
