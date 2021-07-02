#include "1605041_Header.hpp"
#include "bitmap_image.hpp"

#define MAX_FULL_ROTATION 90
#define MAX_PARTIAL_ROTATION 45



double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double m, theta;

int levelOfRecursion, imageDimension, numberOfObjects, numberOfLightSource;
int floorWidth = 1000, tileWidth = 20;

bitmap_image image;

vector<Object*> objectArray;
vector<Light> lightSourceArray;

Vector pos,u,r,l;

void rotateRight(double theta)
{
    r.x = r.x*cos(theta) + l.x*sin(theta);
    r.y = r.y*cos(theta) + l.y*sin(theta);
    r.z = r.z*cos(theta) + l.z*sin(theta);

    r = unitVector(r);
    l = unitVector(crossProduct(u,r));
}

void rotateLeft(double theta)
{
    r.x = r.x*cos(theta) - l.x*sin(theta);
    r.y = r.y*cos(theta) - l.y*sin(theta);
    r.z = r.z*cos(theta) - l.z*sin(theta);

    r = unitVector(r);
    l = unitVector(crossProduct(u,r));
}

void lookDown(double theta)
{
    u.x = u.x*cos(theta) + l.x*sin(theta);
    u.y = u.y*cos(theta) + l.y*sin(theta);
    u.z = u.z*cos(theta) + l.z*sin(theta);

    u = unitVector(u);
    l = unitVector(crossProduct(u,r));
}

void lookUp(double theta)
{
    u.x = u.x*cos(theta) - l.x*sin(theta);
    u.y = u.y*cos(theta) - l.y*sin(theta);
    u.z = u.z*cos(theta) - l.z*sin(theta);

    u = unitVector(u);
    l = unitVector(crossProduct(u,r));
}

void tiltAntiClockwise(double theta)
{
    u.x = u.x*cos(theta) + r.x*sin(theta);
    u.y = u.y*cos(theta) + r.y*sin(theta);
    u.z = u.z*cos(theta) + r.z*sin(theta);

    u = unitVector(u);
    r = unitVector(crossProduct(l,u));
}

void tiltClockwise(double theta)
{
    u.x = u.x*cos(theta) - r.x*sin(theta);
    u.y = u.y*cos(theta) - r.y*sin(theta);
    u.z = u.z*cos(theta) - r.z*sin(theta);

    u = unitVector(u);
    r = unitVector(crossProduct(l,u));
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 1000,0,0);
			glVertex3f(-1000,0,0);

			glVertex3f(0,-1000,0);
			glVertex3f(0, 1000,0);

			glVertex3f(0,0, 1000);
			glVertex3f(0,0,-1000);
		}glEnd();
	}
}

void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void capture(){
    cout << "Capture Called\n";
    int imageWidth = imageDimension;
    int imageHeight = imageDimension;

    image.setwidth_height(imageWidth, imageHeight, true);

    double viewAngle = fovY;
    double planeDistance = (windowHeight / 2.0) / tan((viewAngle * pi) / 360.0);

    Vector topLeft;

    topLeft.x = pos.x + l.x * planeDistance - r.x * (windowWidth / 2) + u.x * (windowHeight / 2);
    topLeft.y = pos.y + l.y * planeDistance - r.y * (windowWidth / 2) + u.y * (windowHeight / 2);
    topLeft.z = pos.z + l.z * planeDistance - r.z * (windowWidth / 2) + u.z * (windowHeight / 2);

    double du = windowWidth / (imageWidth * 1.0);
    double dv = windowHeight / (imageHeight * 1.0);

    topLeft.x = topLeft.x + r.x * (0.5 * du) - u.x * (0.5 * dv);
    topLeft.y = topLeft.y + r.y * (0.5 * du) - u.y * (0.5 * dv);
    topLeft.z = topLeft.z + r.z * (0.5 * du) - u.z * (0.5 * dv);

    int nearest;
    double t = 100000, tMin;

    for(int i=0; i<imageWidth; i++){
        for(int j=0; j<imageHeight; j++){

            Vector curPixel;

            curPixel.x = topLeft.x + r.x * j * du - u.x * i * dv;
            curPixel.y = topLeft.y + r.y * j * du - u.y * i * dv;
            curPixel.z = topLeft.z + r.z * j * du - u.z * i * dv;

            Ray ray(pos, curPixel.subtract(pos));
            double* color = new double[3];
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
            /*for(int k=0; k<objectArray.size(); k++){
                double tempT = objectArray.at(k)->intersect(ray, color, 0);
                if(t>tempT){
                    t = tempT;
                    nearest = k;
                }
            }*/
            tMin = objectArray.at(8)->intersect(ray, color, 1);
            //cout << tMin << color[0] << color[1] << color[2] << endl;
            if(tMin>nearDist)
                image.set_pixel(j,i,color[0], color[1], color[2]);
            //break;
        }
        //break;
    }

    cout << "Saving Image\n";
    image.save_image("F:/4-1/Graphics Sessionals/Offline-03/CSE410-offline03/outputs/out.bmp");
}

void drawSS()
{
    for(int i=0;i<9;i++){
        objectArray.at(i)->draw();
    }
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case 'i': // instead of pgUp
			pos.x += m*u.x;
		    pos.y += m*u.y;
		    pos.z += m*u.z;
			break;

        case 'k': // instead of pgDown
			pos.x -= m*u.x;
		    pos.y -= m*u.y;
		    pos.z -= m*u.z;
			break;


        case '1':
            rotateRight(theta);
            break;

        case '2':
            rotateLeft(theta);
            break;

        case '3':
            lookUp(theta);
            break;

        case '4':
            lookDown(theta);
            break;

        case '5':
            tiltClockwise(theta);
            break;

        case '6':
            tiltAntiClockwise(theta);
            break;

        case '0':
            capture();

		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){

	switch(key){
		case GLUT_KEY_UP:		//down arrow key
		    pos.x += m*l.x;
		    pos.y += m*l.y;
		    pos.z += m*l.z;
			break;

		case GLUT_KEY_DOWN:		// up arrow key
		    pos.x -= m*l.x;
		    pos.y -= m*l.y;
		    pos.z -= m*l.z;
			break;

		case GLUT_KEY_RIGHT:
            pos.x += m*r.x;
		    pos.y += m*r.y;
		    pos.z += m*r.z;
			break;

		case GLUT_KEY_LEFT:
		    pos.x -= m*r.x;
		    pos.y -= m*r.y;
		    pos.z -= m*r.z;

			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x += m*u.x;
		    pos.y += m*u.y;
		    pos.z += m*u.z;
			break;

		case GLUT_KEY_PAGE_DOWN:
		    pos.x -= m*u.x;
		    pos.y -= m*u.y;
		    pos.z -= m*u.z;
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
			}
			break;

		case GLUT_RIGHT_BUTTON:
		    if(state == GLUT_DOWN){
                drawaxes=1-drawaxes;
		    }
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    drawSS();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	m = 5;
	theta = pi/36.0;


	u.x = 0;
	u.y = 0;
	u.z = 1;

	r.x = -1/sqrt(2);
	r.y = 1/sqrt(2);
	r.z = 0;

	l.x = -1/sqrt(2);
	l.y = -1/sqrt(2);
	l.z = 0;

	pos.x = 100;
	pos.y = 100;
	pos.z = 50;

	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(fovY, aspectRatio, nearDist, farDist);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

void loadData(string sceneFilePath){
    ifstream sceneFile(sceneFilePath);
    string lineSegment;
     //while(getline(sceneFile, line_segment)){
        //cout << line_segment << endl;
    //}
    sceneFile >> levelOfRecursion >> imageDimension >> numberOfObjects;

    for(int i=0; i<numberOfObjects; i++){
        sceneFile >> lineSegment;
        if(lineSegment == "sphere"){
            Object* sphere = new Sphere();

            sceneFile >> sphere->center.x >> sphere->center.y >> sphere->center.z;
            sceneFile >> sphere->length;

            sceneFile >> sphere->color[0] >> sphere->color[1] >> sphere->color[2];
            sceneFile >> sphere->coeffs[0] >> sphere->coeffs[1] >> sphere->coeffs[2] >> sphere->coeffs[3];
            sceneFile >> sphere->shine;

            objectArray.push_back(sphere);

        }
        else if(lineSegment == "triangle"){

            Object* triangle = new Triangle();

            sceneFile >> triangle->vertices[0].x >> triangle->vertices[0].y >> triangle->vertices[0].z;
            sceneFile >> triangle->vertices[1].x >> triangle->vertices[1].y >> triangle->vertices[1].z;
            sceneFile >> triangle->vertices[2].x >> triangle->vertices[2].y >> triangle->vertices[2].z;

            sceneFile >> triangle->color[0] >> triangle->color[1] >> triangle->color[2];
            sceneFile >> triangle->coeffs[0] >> triangle->coeffs[1] >> triangle->coeffs[2] >> triangle->coeffs[3];
            sceneFile >> triangle->shine;

            objectArray.push_back(triangle);

        }
        else if(lineSegment == "general"){
            Object* object = new GeneralObject();

            for(int i=0; i<10; i++){
                sceneFile >> object->eqnCoeffs[i];
            }

            sceneFile >> object->cubeReferencePoint.x >> object->cubeReferencePoint.y >> object->cubeReferencePoint.z;
            sceneFile >> object->length >> object->width >> object->height;

            sceneFile >> object->color[0] >> object->color[1] >> object->color[2];
            sceneFile >> object->coeffs[0] >> object->coeffs[1] >> object->coeffs[2] >> object->coeffs[3];
            sceneFile >> object->shine;

            objectArray.push_back(object);
        }

        else{

        }
    }

    sceneFile >> numberOfLightSource;

    for(int i=0; i< numberOfLightSource; i++){
        Light light;

        sceneFile >> light.position.x >> light.position.y >> light.position.z;
        sceneFile >> light.color[0] >> light.color[1] >> light.color[2];

        lightSourceArray.push_back(light);
    }

    Object* floor = new Floor();

    floor->cubeReferencePoint.x = -floorWidth/2;
    floor->cubeReferencePoint.y = -floorWidth/2;
    floor->cubeReferencePoint.z = 0;

    floor->width = floorWidth;
    floor->length = tileWidth;

    objectArray.push_back(floor);


    /*for(int i=0; i<9; i++){
        objectArray.at(i)->print();
    }

    for(int i=0; i<4; i++){
        lightSourceArray.at(i).print();
    }*/
    objectArray.at(3)->print();
}

int main(int argc, char **argv){


	glutInit(&argc,argv);
	glutInitWindowSize(windowHeight, windowWidth);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Vallagena");


	init();
	loadData("F:/4-1/Graphics Sessionals/Offline-03/CSE410-offline03/inputs/scene.txt");

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL



	return 0;
}
