#include<bits/stdc++.h>
#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

using namespace std;

int windowHeight = 500, windowWidth = 500, fovY = 80, aspectRatio = 1, nearDist = 1, farDist = 1000;


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

    Vector subtract(Vector a){
        Vector temp;
        temp.x = x - a.x;
        temp.y = y - a.y;
        temp.z = z - a.z;
        return temp;
    }

    Vector add(Vector a){
        Vector temp;
        temp.x = x + a.x;
        temp.y = y + a.y;
        temp.z = z + a.z;
        return temp;
    }

    Vector scalarMultiply(double a){
        Vector temp;
        temp.x = x*a;
        temp.y = y*a;
        temp.z = z*a;
        return temp;
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

double determinant(double a[3][3]){

    double a1 = a[0][0];
    double a2 = a[1][0];
    double a3 = a[2][0];
    double b1 = a[0][1];
    double b2 = a[1][1];
    double b3 = a[2][1];
    double c1 = a[0][2];
    double c2 = a[1][2];
    double c3 = a[2][2];

    return (a1*b2*c3 + b1*c2*a3 + c1*a2*b3) - (a3*b2*c1 + b3*c2*a1 + c3*a2*b1);
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

class Ray {
 public:
    Vector start;
    Vector dir;

    Ray() {}

    Ray(Vector s, Vector d){
        this->start = s;
        this->dir = unitVector(d);
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

     virtual double intersect(Ray ray, double color[3], int level){
         return -1;
     }

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

    double intersect(Ray ray, double color[3], int level){

        Vector oc = ray.start.subtract(center);
        double a = dotProduct(ray.dir,ray.dir);
        double b = 2 * dotProduct(ray.dir, oc);
        double c = dotProduct(oc,oc) - length*length;
        double minPosT = 10000;
        double det = b*b - 4*a*c;

        if(det >=0){
            double d = sqrt(det);
            double t1 = (-b+d)/(2*a);
            double t2 = (-b-d)/(2*a);

            if(t1>0 || t2>0){
                if(t1 <= 0){
                    minPosT = t2;
                }
                else if(t2 <= 0){
                    minPosT = t1;
                }
                else{
                    minPosT = (t1<t2)?t1:t2;
                }
            }
            else{
                return minPosT;
            }
        }
        else{
            return minPosT;
        }

        color[0] = this->color[0]*255;
        color[1] = this->color[1]*255;
        color[2] = this->color[2]*255;

        return minPosT;
    }

    void draw(){
        glPushMatrix();
        glTranslatef(center.x, center.y, center.z);

        int slices=48,stacks=20;
        int radius = length;

        Vector points[100][100];
        int i,j;
        double h,r;

        //generate points
        for(i=0;i<=stacks;i++)
        {
            h=radius*sin(((double)i/(double)stacks)*(pi/2));
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0;j<=slices;j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }

        //draw quads using generated points
        glColor3f(color[0],color[1],color[2]);
        for(i=0;i<stacks;i++)
        {
            for(j=0;j<slices;j++)
            {
                glBegin(GL_QUADS);{
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                }glEnd();
            }
        }
        glPopMatrix();
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

    double intersect(Ray ray, double color[3], int level){

        double matA[3][3] ={{vertices[0].x - vertices[1].x, vertices[0].x - vertices[2].x, ray.dir.x},
                            {vertices[0].y - vertices[1].y, vertices[0].y - vertices[2].y, ray.dir.y},
                            {vertices[0].z - vertices[1].z, vertices[0].z - vertices[2].z, ray.dir.z}};

        double matBeta[3][3] = {{vertices[0].x - ray.start.x, vertices[0].x - vertices[2].x, ray.dir.x},
                                {vertices[0].y - ray.start.y, vertices[0].y - vertices[2].y, ray.dir.y},
                                {vertices[0].z - ray.start.z, vertices[0].z - vertices[2].z, ray.dir.z}};


        double matGamma[3][3] = {{vertices[0].x - vertices[1].x, vertices[0].x - ray.start.x, ray.dir.x},
                                {vertices[0].y - vertices[1].y, vertices[0].y - ray.start.y, ray.dir.y},
                                {vertices[0].z - vertices[1].z, vertices[0].z - ray.start.z, ray.dir.z}};

        double matT[3][3] = {{vertices[0].x - vertices[1].x, vertices[0].x - vertices[2].x, vertices[0].x - ray.start.x},
                             {vertices[0].y - vertices[1].y, vertices[0].y - vertices[2].y, vertices[0].y - ray.start.y},
                             {vertices[0].z - vertices[1].z, vertices[0].z - vertices[2].z, vertices[0].z - ray.start.z}};

        double t = 10000;
        double detA = determinant(matA);
        if(detA != 0){
            double beta = determinant(matBeta)/detA;
            double gamma = determinant(matGamma)/detA;
            t = determinant(matT)/detA;

            if((beta+gamma < 1)&(beta > 0)&(gamma > 0)&(t> nearDist)){
                color[0] = this->color[0]*255;
                color[1] = this->color[1]*255;
                color[2] = this->color[2]*255;
            }
            else{
                t = 10000;
            }
        }

        return t;
    }

    void draw(){
        glPushMatrix();
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_TRIANGLES);{
            glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
            glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
            glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
        }glEnd();
        glPopMatrix();
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

    double intersect(Ray ray, double color[3], int level){
        double a = eqnCoeffs[0];
        double b = eqnCoeffs[1];
        double c = eqnCoeffs[2];
        double d = eqnCoeffs[3];
        double e = eqnCoeffs[4];
        double f = eqnCoeffs[5];
        double g = eqnCoeffs[6];
        double h = eqnCoeffs[7];
        double i = eqnCoeffs[8];
        double j = eqnCoeffs[9];

        double xd = ray.dir.x;
        double yd = ray.dir.y;
        double zd = ray.dir.z;

        double xo = ray.start.x;
        double yo = ray.start.y;
        double zo = ray.start.z;

        double aq = a*xd*xd + b*yd*yd + c*zd*zd + d*xd*yd + e*xd*zd + f*yd*zd;
        double bq = 2*a*xo*xd + 2*b*yo*yd + 2*c*zo*zd + d*(xo*yd + yo*xd) + e*(xo*zd + zo*xd) + f*(yo*zd + yd*zo) + g*xd + h*yd + i*zd;
        double cq = a*xo*xo + b*yo*yo + c*zo*zo + d*xo*yo + e*xo*zo + f*yo*zo + g*xo + h*yo + i*zo + j;

        double det = bq*bq - 4*aq*cq;
        double minPosT = 10000;
        double maxPosT = -10000;

        if(aq == 0){
            minPosT = (-1)*(cq/bq);
        }
        else{
            if(det >= 0){
                double t1 =( - bq - sqrt(det))/ (2*aq);
                /*if(t1 > 0){
                    minPosT = t1;
                }
                else{*/
                double t2 =( - bq + sqrt(det))/ (2*aq);
                minPosT = (t1<t2)?t1:t2;
                //}
                /*double t2 =( - bq + sqrt(det))/ (2*aq);
                if(t1>0 || t2>0){
                    if(t1 <= 0){
                        minPosT = t2;
                    }
                    else if(t2 <= 0){
                        minPosT = t1;
                    }
                    else{
                        minPosT = (t1<t2)?t1:t2;
                    }
                }
                else{
                    return minPosT;
                }*/
            }
            else{
                return minPosT;
            }
        }

        Vector intersection_point = ray.start.add(ray.dir.scalarMultiply(minPosT));
        bool insideCube = true;

        if(height != 0){
            if(abs(intersection_point.z) < abs(cubeReferencePoint.z) || abs(intersection_point.z) > abs(cubeReferencePoint.z) + height){
                insideCube = false;
                minPosT = 10000;
            }
        }

        if(length != 0){
            if(abs(intersection_point.x) < abs(cubeReferencePoint.x) || abs(intersection_point.x) > abs(cubeReferencePoint.x) + length){
                insideCube = false;
                minPosT = 10000;
            }
        }

        if(width != 0){
            if(abs(intersection_point.y) < abs(cubeReferencePoint.y) || abs(intersection_point.y) > abs(cubeReferencePoint.y) + width){
                insideCube = false;
                minPosT = 10000;
            }
        }

        if(insideCube)
        {
            color[0] = this->color[0]*255;
            color[1] = this->color[1]*255;
            color[2] = this->color[2]*255;
        }

        return minPosT;
    }

    void draw() {}
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

    double intersect(Ray ray, double* color, int level){
        double d = 0;
        Vector n(0,0,1);
        double t = 10000;
        if(dotProduct(n,ray.dir) != 0){
            t = -1 * (d + dotProduct(n,ray.start)) / dotProduct(n,ray.dir);

            Vector intersection_point = ray.start.add(ray.dir.scalarMultiply(t));

            if((intersection_point.x >= -500 && intersection_point.x <= 500) && (intersection_point.y >= -500 && intersection_point.y <= 500)){
                int pixelX = (int)((cubeReferencePoint.x -intersection_point.x)/length);
                int pixelY = (int)((cubeReferencePoint.y -intersection_point.y)/length);
                if((pixelX + pixelY)%2==0 && t>nearDist){
                    color[0] = 255;
                    color[1] = 255;
                    color[2] = 255;
                } else{
                    color[0] = 0;
                    color[1] = 0;
                    color[2] = 0;
                }
            }
            else{
                t = 10000;
            }
        }
        return t;
    }

    void draw(){
        glPushMatrix();

        int k = cubeReferencePoint.z;
        double rgb = 1;

        for(int i=cubeReferencePoint.x; i<=width/2-length; i+=length){
            rgb = 1 - rgb;
            for(int j=cubeReferencePoint.y; j<=width/2-length; j+=length){
                rgb = 1 - rgb;
                glColor3f(rgb,rgb,rgb);
                glBegin(GL_QUADS);{
                    glVertex3f(i, j, k);
                    glVertex3f(i+length, j, k);
                    glVertex3f(i+length, j+length, k);
                    glVertex3f(i, j+length, k);
                }glEnd();
            }
        }

        glPopMatrix();
    }
};



