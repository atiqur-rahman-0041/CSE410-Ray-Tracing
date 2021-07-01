#include<bits/stdc++.h>

using namespace std;

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

class Light {
 public:
     Vector position;
     double color[3];

     Light() {}

     void draw() {}

     void print(){
        cout << "Light Source: \n";
        cout << "Position(x,y,z): (" << position.x << "," << position.y << "," << position.z << ")" << endl;
        cout << "Color(r,g,b): (" << color[0] << "," << color[1] << "," << color[2] << ")" << endl << endl;
     }
};

class Object {
 public:
     Vector cubeReferencePoint;
     Vector center;
     Vector vertices[3];
     double eqnCoeffs[10];
     double height;
     double width;
     double length;
     double color[3];
     double coeffs[4];
     int shine;

     Object() {}

     virtual void draw() {}

     virtual void print(){
         cout << "why I am here -.- " << endl;
     }
};

class Sphere: public Object {
 public:
    Sphere() {}

    void print(){
        cout << "Sphere: \n";
        cout << "Center(x,y,z): (" << center.x << "," << center.y << "," << center.z << ")" << endl;
        cout << "Radius: " << length << endl;
        cout << "Color(r,g,b): (" << color[0] << "," << color[1] << "," << color[2] << ")" << endl;
        cout << "Coeffs(a,d,s,r): (" << coeffs[0] << "," << coeffs[1] << "," << coeffs[2] << "," << coeffs[3] << ")" << endl;
        cout << "Shininess: " << shine << endl << endl;
    }
};

class Triangle: public Object {
 public:
    Triangle() {}

    void print(){
        cout << "Triangle: \n";
        cout << "Vertices: " << endl;
        cout << "   V1(x1,y1,z1) : (" << vertices[0].x << "," << vertices[0].y << "," << vertices[0].z << ")" << endl;
        cout << "   V2(x2,y2,z2) : (" << vertices[1].x << "," << vertices[1].y << "," << vertices[1].z << ")" << endl;
        cout << "   V3(x3,y3,z3) : (" << vertices[2].x << "," << vertices[2].y << "," << vertices[2].z << ")" << endl;
        cout << "Color(r,g,b): (" << color[0] << "," << color[1] << "," << color[2] << ")" << endl;
        cout << "Coeffs(a,d,s,r): (" << coeffs[0] << "," << coeffs[1] << "," << coeffs[2] << "," << coeffs[3] << ")" << endl;
        cout << "Shininess: " << shine << endl << endl;
    }
};

class GeneralObject: public Object {
 public:
    GeneralObject() {}

    void print(){
        cout << "General Object: \n";
        cout << "Equation Coeffs: (";
        for(int i=0;i<10;i++){
            cout << eqnCoeffs[i];
            if(i<9) cout << ",";
        }
        cout << ")" << endl;
        cout << "Cube Reference Point(x,y,z): (" << cubeReferencePoint.x << "," << cubeReferencePoint.y << "," << cubeReferencePoint.z << ")" << endl;
        cout << "Length: " << length << endl;
        cout << "Width: " << width << endl;
        cout << "Height: " << height << endl;
        cout << "Color(r,g,b): (" << color[0] << "," << color[1] << "," << color[2] << ")" << endl;
        cout << "Coeffs(a,d,s,r): (" << coeffs[0] << "," << coeffs[1] << "," << coeffs[2] << "," << coeffs[3] << ")" << endl;
        cout << "Shininess: " << shine << endl << endl;
    }
};

class Floor: public Object {
 public:
    Floor() {}

    void print(){
        cout << "Floor: \n";
        cout << "Reference Point(x,y,z): (" << cubeReferencePoint.x << "," << cubeReferencePoint.y << "," << cubeReferencePoint.z << ")" << endl;
        cout << "Length: " << length << endl;
        cout << "Width: " << width << endl << endl;
    }
};


