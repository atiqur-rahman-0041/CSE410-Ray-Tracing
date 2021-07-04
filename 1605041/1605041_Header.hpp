#include<bits/stdc++.h>
#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))
#define T_INF 10000 

using namespace std;

int windowHeight = 500, windowWidth = 500, fovY = 80, aspectRatio = 1, nearDist = 1, farDist = 1000;

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

    void draw(){
        
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);

        int slices=48,stacks=20;
        int radius = 1;

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

extern vector<Light> lightSourceArray;
extern vector<Object*> objectArray;
extern int levelOfRecursion;

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
        double minPosT = T_INF;
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
                    minPosT = t2;
                }
            }
            else{
                return minPosT;
            }
        }
        else{
            return minPosT;
        }



        if(level == 0) return minPosT;
        
        // phong lighting
        

        color[0] = this->color[0]*coeffs[0];
        color[1] = this->color[1]*coeffs[0];
        color[2] = this->color[2]*coeffs[0];

        
        Vector intersectionPoint = ray.start.add(ray.dir.scalarMultiply(minPosT));
        Vector normal = unitVector(intersectionPoint.subtract(this->center));
        
        for(int i=0;i<lightSourceArray.size();i++){

            bool inShadow = false;

            Vector direction = unitVector(lightSourceArray[i].position.subtract(intersectionPoint));
            Ray rayTemp(intersectionPoint.add(direction.scalarMultiply(0.0001)), direction);

            double* dummyColor = new double[3];
            double dist = valueOfVector(intersectionPoint.subtract(lightSourceArray[i].position));

            for(int k=0; k<objectArray.size(); k++){
                double tempT = objectArray.at(k)->intersect(rayTemp, dummyColor, 0);
                if(dist>tempT && tempT > 0){
                    inShadow = true;
                    break;
                }
            }

            delete[] dummyColor;

            if(!inShadow){

                double lambertValue = dotProduct(normal, rayTemp.dir);
                Vector reflectedRay = normal.scalarMultiply(2*lambertValue).subtract(rayTemp.dir);
                double phongValue = dotProduct(reflectedRay,ray.dir.scalarMultiply(-1));

                if(lambertValue < 0) lambertValue = 0;

                color[0] += lightSourceArray[i].color[0]*coeffs[1]*lambertValue*this->color[0];
                color[1] += lightSourceArray[i].color[1]*coeffs[1]*lambertValue*this->color[1];
                color[2] += lightSourceArray[i].color[2]*coeffs[1]*lambertValue*this->color[2];

                if(phongValue < 0) phongValue = 0;

                color[0] += lightSourceArray[i].color[0]*coeffs[2]*pow(phongValue, this->shine)*this->color[0];
                color[1] += lightSourceArray[i].color[1]*coeffs[2]*pow(phongValue, this->shine)*this->color[1];
                color[2] += lightSourceArray[i].color[2]*coeffs[2]*pow(phongValue, this->shine)*this->color[2];
            }
        }

        color[0] = (color[0]<0) ? 0 : (color[0] > 1)? 1:color[0];
        color[1] = (color[1]<0) ? 0 : (color[1] > 1)? 1:color[1];
        color[2] = (color[2]<0) ? 0 : (color[2] > 1)? 1:color[2];

        

        // recursive reflection

        if(level >= levelOfRecursion){
            return minPosT;
        }

        double rNearest=-1, rT=T_INF, rTempT, rTmin;

        double rLambertValue = dotProduct(normal, ray.dir.scalarMultiply(-1));
        Vector rReflectedRay = normal.scalarMultiply(2*rLambertValue).subtract(ray.dir.scalarMultiply(-1));
        Vector recursiveReflectionDirection = unitVector(rReflectedRay);
        Ray recursiveReflectionRay(intersectionPoint.add(recursiveReflectionDirection.scalarMultiply(0.0001)), recursiveReflectionDirection);

        double* colorReflected = new double[3];

        for(int k=0; k<objectArray.size(); k++){
            rTempT = objectArray.at(k)->intersect(recursiveReflectionRay, colorReflected, 0);
            if(rT>rTempT && rTempT <T_INF){
                rT = rTempT;
                rNearest = k;
            }
        }

        colorReflected[0] = 0;
        colorReflected[1] = 0;
        colorReflected[2] = 0;

        if(rNearest >= 0){
            rTmin = objectArray.at(rNearest)->intersect(recursiveReflectionRay, colorReflected, level+1);
            if(rTmin > 0 && rTmin <T_INF){
                color[0] += colorReflected[0]*this->coeffs[3];
                color[1] += colorReflected[1]*this->coeffs[3];
                color[2] += colorReflected[2]*this->coeffs[3];
            }
        }

        delete[] colorReflected;

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

        double matA[3][3]       =  {{vertices[0].x - vertices[1].x, vertices[0].x - vertices[2].x, ray.dir.x},
                                    {vertices[0].y - vertices[1].y, vertices[0].y - vertices[2].y, ray.dir.y},
                                    {vertices[0].z - vertices[1].z, vertices[0].z - vertices[2].z, ray.dir.z}};

        double matBeta[3][3]    =  {{vertices[0].x - ray.start.x, vertices[0].x - vertices[2].x, ray.dir.x},
                                    {vertices[0].y - ray.start.y, vertices[0].y - vertices[2].y, ray.dir.y},
                                    {vertices[0].z - ray.start.z, vertices[0].z - vertices[2].z, ray.dir.z}};


        double matGamma[3][3]   =  {{vertices[0].x - vertices[1].x, vertices[0].x - ray.start.x, ray.dir.x},
                                    {vertices[0].y - vertices[1].y, vertices[0].y - ray.start.y, ray.dir.y},
                                    {vertices[0].z - vertices[1].z, vertices[0].z - ray.start.z, ray.dir.z}};

        double matT[3][3]       =  {{vertices[0].x - vertices[1].x, vertices[0].x - vertices[2].x, vertices[0].x - ray.start.x},
                                    {vertices[0].y - vertices[1].y, vertices[0].y - vertices[2].y, vertices[0].y - ray.start.y},
                                    {vertices[0].z - vertices[1].z, vertices[0].z - vertices[2].z, vertices[0].z - ray.start.z}};

        double t = T_INF;
        double detA = determinant(matA);
        
        if(detA != 0){

            double beta = determinant(matBeta)/detA;
            double gamma = determinant(matGamma)/detA;
            t = determinant(matT)/detA;

            if((beta + gamma < 1) && (beta > 0) && (gamma > 0) && (t > nearDist)){
                
                if(level == 0) return t;
            
                // phong lighting 

                color[0] = this->color[0]*coeffs[0];
                color[1] = this->color[1]*coeffs[0];
                color[2] = this->color[2]*coeffs[0];

                Vector intersectionPoint = ray.start.add(ray.dir.scalarMultiply(t));
                Vector normal = unitVector(crossProduct(vertices[1].subtract(vertices[0]),vertices[2].subtract(vertices[0])));
                
                if(dotProduct(normal,ray.dir) > 0) normal = normal.scalarMultiply(-1);

                for(int i=0;i<lightSourceArray.size();i++){

                    bool inShadow = false;
                    
                    Vector direction = unitVector(lightSourceArray[i].position.subtract(intersectionPoint));
                    Ray rayTemp(intersectionPoint.add(direction.scalarMultiply(0.0001)), direction);
                    
                    double* dummyColor = new double[3];
                    double dist = valueOfVector(intersectionPoint.subtract(lightSourceArray[i].position));

                    for(int k=0; k<objectArray.size(); k++){
                        double tempT = objectArray.at(k)->intersect(rayTemp, dummyColor, 0);
                        if(dist>tempT && tempT > 0){
                            inShadow = true;
                            break;
                        }
                    }

                    delete[] dummyColor;

                    if(!inShadow){

                        double lambertValue = dotProduct(normal, rayTemp.dir);
                        Vector reflectedRay = normal.scalarMultiply(2*lambertValue).subtract(rayTemp.dir);
                        double phongValue = dotProduct(reflectedRay,ray.dir.scalarMultiply(-1));

                        if(lambertValue < 0) lambertValue = 0;

                        color[0] += lightSourceArray[i].color[0]*coeffs[1]*lambertValue*this->color[0];
                        color[1] += lightSourceArray[i].color[1]*coeffs[1]*lambertValue*this->color[1];
                        color[2] += lightSourceArray[i].color[2]*coeffs[1]*lambertValue*this->color[2];

                        if(phongValue < 0) phongValue = 0;

                        color[0] += lightSourceArray[i].color[0]*coeffs[2]*pow(phongValue, this->shine)*this->color[0];
                        color[1] += lightSourceArray[i].color[1]*coeffs[2]*pow(phongValue, this->shine)*this->color[1];
                        color[2] += lightSourceArray[i].color[2]*coeffs[2]*pow(phongValue, this->shine)*this->color[2];
                    }
                }

                color[0] = (color[0]<0) ? 0 : (color[0] > 1)? 1:color[0];
                color[1] = (color[1]<0) ? 0 : (color[1] > 1)? 1:color[1];
                color[2] = (color[2]<0) ? 0 : (color[2] > 1)? 1:color[2];

                
                // recursive reflection

                if(level >= levelOfRecursion){
                    return t;
                }

                double rNearest=-1, rT=T_INF, rTempT, rTmin;

                double rLambertValue = dotProduct(normal, ray.dir.scalarMultiply(-1));
                Vector rReflectedRay = normal.scalarMultiply(2*rLambertValue).subtract(ray.dir.scalarMultiply(-1));
                Vector recursiveReflectionDirection = unitVector(rReflectedRay);
                Ray recursiveReflectionRay(intersectionPoint.add(recursiveReflectionDirection.scalarMultiply(0.0001)), recursiveReflectionDirection);
                
                double* colorReflected = new double[3];

                for(int k=0; k<objectArray.size(); k++){
                    rTempT = objectArray.at(k)->intersect(recursiveReflectionRay, colorReflected, 0);
                    if(rT>rTempT && rTempT <T_INF){
                        rT = rTempT;
                        rNearest = k;
                    }
                }

                colorReflected[0] = 0;
                colorReflected[1] = 0;
                colorReflected[2] = 0;

                if(rNearest >= 0){
                    rTmin = objectArray.at(rNearest)->intersect(recursiveReflectionRay, colorReflected, level+1);
                    if(rTmin > 0 && rTmin <T_INF){
                        color[0] += colorReflected[0]*this->coeffs[3];
                        color[1] += colorReflected[1]*this->coeffs[3];
                        color[2] += colorReflected[2]*this->coeffs[3];
                    }
                }

                delete[] colorReflected;
            }
            else
            {
                t = T_INF;
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
        double minPosT = T_INF;
        double t1,t2;

        if(aq == 0)
        {
            t1 = (-1)*(cq/bq);
            t2 = t1;
        }
        else
        {
            if(det >= 0)
            {
                t1 =(- bq - sqrt(det)) / (2 * aq);
                t2 =(- bq + sqrt(det)) / (2 * aq);
            }
            else
            {
                return minPosT;
            }
        }

        Vector intersectionPointT1 = ray.start.add(ray.dir.scalarMultiply(t1));
        Vector intersectionPointT2 = ray.start.add(ray.dir.scalarMultiply(t2));

        bool insideCubeT1 = true;
        bool insideCubeT2 = true;

        if(height != 0)
        {
            if(abs(intersectionPointT1.z) < abs(cubeReferencePoint.z) || abs(intersectionPointT1.z) > abs(cubeReferencePoint.z) + height)
            {
                insideCubeT1 = false;
            }

            if(abs(intersectionPointT2.z) < abs(cubeReferencePoint.z) || abs(intersectionPointT2.z) > abs(cubeReferencePoint.z) + height)
            {
                insideCubeT2 = false;
            }
        }

        if(length != 0)
        {
            if(abs(intersectionPointT1.x) < abs(cubeReferencePoint.x) || abs(intersectionPointT1.x) > abs(cubeReferencePoint.x) + length)
            {
                insideCubeT1 = false;
            }

            if(abs(intersectionPointT2.x) < abs(cubeReferencePoint.x) || abs(intersectionPointT2.x) > abs(cubeReferencePoint.x) + length)
            {
                insideCubeT2 = false;
            }
        }

        if(width != 0){
            if(abs(intersectionPointT1.y) < abs(cubeReferencePoint.y) || abs(intersectionPointT1.y) > abs(cubeReferencePoint.y) + width)
            {
                insideCubeT1 = false;
            }

            if(abs(intersectionPointT2.y) < abs(cubeReferencePoint.y) || abs(intersectionPointT2.y) > abs(cubeReferencePoint.y) + width)
            {
                insideCubeT2 = false;
            }
        }

        if(!insideCubeT1 && !insideCubeT2)
        {
            minPosT = T_INF;
            return minPosT;
        }
        else if(!insideCubeT1)
        {
            minPosT = t2;
        }
        else if(!insideCubeT2)
        {
            minPosT = t1;
        }
        else
        {
            minPosT = t1;
        }

        if(minPosT <= 0) return T_INF;

        if(level == 0) return minPosT;
        
        // phong lighting

        color[0] = this->color[0]*coeffs[0];
        color[1] = this->color[1]*coeffs[0];
        color[2] = this->color[2]*coeffs[0];

        Vector intersectionPoint = ray.start.add(ray.dir.scalarMultiply(minPosT));

        double xi = intersectionPoint.x;
        double yi = intersectionPoint.y;
        double zi = intersectionPoint.z;

        Vector surfaceNormal;

        surfaceNormal.x = 2*a*xi + d*yi + e*zi + g;
        surfaceNormal.y = 2*b*yi + d*xi + f*zi + h;
        surfaceNormal.z = 2*c*zi + e*xi + f*yi + i;

        if(dotProduct(ray.start.subtract(intersectionPoint),ray.dir) > 0) surfaceNormal = surfaceNormal.scalarMultiply(-1);

        Vector normal = unitVector(surfaceNormal);

        for(int i=0;i<lightSourceArray.size();i++){

            bool inShadow = false;

            Vector direction = unitVector(lightSourceArray[i].position.subtract(intersectionPoint));
            Ray rayTemp(intersectionPoint.add(direction.scalarMultiply(0.0001)), direction);
            double dist = valueOfVector(intersectionPoint.subtract(lightSourceArray[i].position));
            
            double* dummyColor = new double[3];
            
            for(int k=0; k<objectArray.size(); k++){
                double tempT = objectArray.at(k)->intersect(rayTemp, dummyColor, 0);
                if(dist>tempT && tempT > 0){
                    inShadow = true;
                    break;
                }
            }

            delete[] dummyColor;

            if(!inShadow){

                double lambertValue = dotProduct(normal, rayTemp.dir);
                Vector reflectedRay = normal.scalarMultiply(2*lambertValue).subtract(rayTemp.dir);
                double phongValue = dotProduct(reflectedRay,ray.dir.scalarMultiply(-1));

                if(lambertValue < 0) lambertValue = 0;

                color[0] += lightSourceArray[i].color[0]*coeffs[1]*lambertValue*this->color[0];
                color[1] += lightSourceArray[i].color[1]*coeffs[1]*lambertValue*this->color[1];
                color[2] += lightSourceArray[i].color[2]*coeffs[1]*lambertValue*this->color[2];

                if(phongValue < 0) phongValue = 0;

                color[0] += lightSourceArray[i].color[0]*coeffs[2]*pow(phongValue, this->shine)*this->color[0];
                color[1] += lightSourceArray[i].color[1]*coeffs[2]*pow(phongValue, this->shine)*this->color[1];
                color[2] += lightSourceArray[i].color[2]*coeffs[2]*pow(phongValue, this->shine)*this->color[2];
            }
        }

        color[0] = (color[0]<0) ? 0 : (color[0] > 1)? 1:color[0];
        color[1] = (color[1]<0) ? 0 : (color[1] > 1)? 1:color[1];
        color[2] = (color[2]<0) ? 0 : (color[2] > 1)? 1:color[2];

        
        // recursive reflection

        if(level >= levelOfRecursion){
            return minPosT;
        }

        double rNearest=-1, rT=T_INF, rTempT, rTmin;

        double rLambertValue = dotProduct(normal, ray.dir.scalarMultiply(-1));
        Vector rReflectedRay = normal.scalarMultiply(2*rLambertValue).subtract(ray.dir.scalarMultiply(-1));
        Vector recursiveReflectionDirection = unitVector(rReflectedRay);
        
        Ray recursiveReflectionRay(intersectionPoint.add(recursiveReflectionDirection.scalarMultiply(0.0001)), recursiveReflectionDirection);

        double* colorReflected = new double[3];

        for(int k=0; k<objectArray.size(); k++){
            rTempT = objectArray.at(k)->intersect(recursiveReflectionRay, colorReflected, 0);
            if(rT>rTempT && rTempT<T_INF){
                rT = rTempT;
                rNearest = k;
            }
        }

        colorReflected[0] = 0;
        colorReflected[1] = 0;
        colorReflected[2] = 0;

        if(rNearest >= 0){
            rTmin = objectArray.at(rNearest)->intersect(recursiveReflectionRay, colorReflected, level+1);
            if(rTmin > 0 && rTmin <T_INF){
                color[0] += colorReflected[0]*this->coeffs[3];
                color[1] += colorReflected[1]*this->coeffs[3];
                color[2] += colorReflected[2]*this->coeffs[3];
            }
        }

        delete[] colorReflected;

        return minPosT;
    }

    void draw() {}
};

class Floor: public Object {
 public:
    Floor() {
        this->coeffs[0] = 0.4;
        this->coeffs[1] = 0.4;
        this->coeffs[2] = 0.3;
        this->coeffs[3] = 0.3;
        this->shine = 20;
    }

    void print(){
        cout << "Floor: \n";
        cout << "Reference Point(x,y,z): (" << cubeReferencePoint.x << "," << cubeReferencePoint.y << "," << cubeReferencePoint.z << ")" << endl;
        cout << "Length: " << length << endl;
        cout << "Width: " << width << endl << endl;
    }

    double intersect(Ray ray, double* color, int level){

        double d = 0, t = T_INF;
        
        Vector normal(0,0,1), intersectionPoint;

        if(dotProduct(normal,ray.dir) > 0) normal = normal.scalarMultiply(-1);

        if(dotProduct(normal,ray.dir) != 0){

            t = -1 * (d + dotProduct(normal,ray.start)) / dotProduct(normal,ray.dir);

            intersectionPoint = ray.start.add(ray.dir.scalarMultiply(t));


            if((intersectionPoint.x >= this->cubeReferencePoint.x && intersectionPoint.x <= this->cubeReferencePoint.x + width) && (intersectionPoint.y >= this->cubeReferencePoint.y && intersectionPoint.y <= this->cubeReferencePoint.y + width) && (t > nearDist)){
                
                if(level == 0) return t;

                int pixelX = (int)((cubeReferencePoint.x - intersectionPoint.x) / length);
                int pixelY = (int)((cubeReferencePoint.y - intersectionPoint.y) / length);
                
                double colorValue = ((pixelX + pixelY)%2==0)?0.8:0;
                
                // phong lighting

                color[0] = colorValue*coeffs[0];
                color[1] = colorValue*coeffs[0];
                color[2] = colorValue*coeffs[0];


                for(int i=0;i<lightSourceArray.size();i++){

                    bool inShadow = false;

                    Vector direction = unitVector(lightSourceArray[i].position.subtract(intersectionPoint));
                    Ray rayTemp(intersectionPoint.add(direction.scalarMultiply(0.0001)), direction);
                    double dist = valueOfVector(intersectionPoint.subtract(lightSourceArray[i].position));

                    double* dummyColor = new double[3];
                    
                    for(int k=0; k<objectArray.size(); k++){
                        double tempT = objectArray.at(k)->intersect(rayTemp, dummyColor, 0);
                        if(dist>tempT && tempT > 0){
                            inShadow = true;
                            break;
                        }
                    }

                    delete[] dummyColor;

                    if(!inShadow){

                        double lambertValue = dotProduct(normal, rayTemp.dir);
                        Vector reflectedRay = normal.scalarMultiply(2*lambertValue).subtract(rayTemp.dir);
                        double phongValue = dotProduct(reflectedRay,ray.dir.scalarMultiply(-1));

                        if(lambertValue < 0) lambertValue = 0;

                        color[0] += lightSourceArray[i].color[0]*coeffs[1]*lambertValue*colorValue;
                        color[1] += lightSourceArray[i].color[1]*coeffs[1]*lambertValue*colorValue;
                        color[2] += lightSourceArray[i].color[2]*coeffs[1]*lambertValue*colorValue;

                        if(phongValue < 0) phongValue = 0;

                        color[0] += lightSourceArray[i].color[0]*coeffs[2]*pow(phongValue, this->shine)*colorValue;
                        color[1] += lightSourceArray[i].color[1]*coeffs[2]*pow(phongValue, this->shine)*colorValue;
                        color[2] += lightSourceArray[i].color[2]*coeffs[2]*pow(phongValue, this->shine)*colorValue;
                    }
                }

                color[0] = (color[0]<0) ? 0 : (color[0] > 1)? 0.8:color[0];
                color[1] = (color[1]<0) ? 0 : (color[1] > 1)? 0.8:color[1];
                color[2] = (color[2]<0) ? 0 : (color[2] > 1)? 0.8:color[2];
                

                // recursive reflection

                if(level >= levelOfRecursion){
                    return t;
                }

                double rNearest=-1, rT=T_INF, rTempT, rTmin;

                double rLambertValue = dotProduct(normal, ray.dir.scalarMultiply(-1));
                Vector rReflectedRay = normal.scalarMultiply(2*rLambertValue).subtract(ray.dir.scalarMultiply(-1));
                Vector recursiveReflectionDirection = unitVector(rReflectedRay);
                Ray recursiveReflectionRay(intersectionPoint.add(recursiveReflectionDirection.scalarMultiply(0.0001)), recursiveReflectionDirection);

                double* colorReflected = new double[3];

                for(int k=0; k<objectArray.size(); k++){
                    rTempT = objectArray.at(k)->intersect(recursiveReflectionRay, colorReflected, 0);
                    if(rT>rTempT && rTempT < T_INF){
                        rT = rTempT;
                        rNearest = k;
                    }
                }

                colorReflected[0] = 0;
                colorReflected[1] = 0;
                colorReflected[2] = 0;

                if(rNearest >= 0){
                    rTmin = objectArray.at(rNearest)->intersect(recursiveReflectionRay, colorReflected, level+1);
                    if(rTmin > 0 && rTmin < T_INF){
                        color[0] += colorReflected[0]*this->coeffs[3];
                        color[1] += colorReflected[1]*this->coeffs[3];
                        color[2] += colorReflected[2]*this->coeffs[3];
                    }
                }

                delete[] colorReflected;
            }
            else
            {
                t = T_INF;
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



