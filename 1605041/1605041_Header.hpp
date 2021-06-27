#include<bits/stdc++.h>

using namespace std;
/*
struct point
{
	double x,y,z;
};

struct Vector
{
	double x,y,z;
};

point pos;
Vector u,r,l;

Vector crossProduct(Vector a, Vector b)
{
    Vector cross_prod;
    cross_prod.x = a.y*b.z-a.z*b.y;
    cross_prod.y = a.z*b.x-a.x*b.z;
    cross_prod.z = a.x*b.y-a.y*b.x;

    return cross_prod;
}

double dotProduct(Vector a, Vector b)
{
    return a.x*b.x+a.y*b.y+a.z*b.z;
}

double valueOfVector(Vector a)
{
    return sqrt(pow(a.x,2)+pow(a.y,2)+pow(a.z,2));
}

Vector unitVector(Vector a)
{
    Vector unit_vector;
    double val = valueOfVector(a);

    unit_vector.x = a.x/val;
    unit_vector.y = a.y/val;
    unit_vector.z = a.z/val;

    return unit_vector;
}

*/

class Point {

 public:

    double x;
    double y;
    double z;

    Point(){}

    Point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void print(){
        cout << "Point(x,y,z): (" << this->x << "," << this->y << "," << this->z << ")" << endl;
    }
};

class Vector {

 public:

    double x;
    double y;
    double z;

    Vector(){}

    Vector(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

     void print(){
        cout << "Vector(x,y,z): (" << this->x << "," << this->y << "," << this->z << ")" << endl;
    }
};

Vector crossProduct(Vector a, Vector b)
{
    Vector cross_prod;

    cross_prod.x = a.y*b.z-a.z*b.y;
    cross_prod.y = a.z*b.x-a.x*b.z;
    cross_prod.z = a.x*b.y-a.y*b.x;

    return cross_prod;
}

double dotProduct(Vector a, Vector b)
{
    return a.x*b.x+a.y*b.y+a.z*b.z;
}

double valueOfVector(Vector a)
{
    return sqrt(pow(a.x,2)+pow(a.y,2)+pow(a.z,2));
}

Vector unitVector(Vector a)
{
    double val = valueOfVector(a);
    Vector unit_vector(a.x/val, a.y/val, a.z/val);
    return unit_vector;
}



