#include "TriangulationDisplay.h"
#include "hinge_flip.h"
#include "special_flip.h"

namespace TriangulationDisplay {

//opengl variables will start with an underscore
//these variables hold state info pertaining to the camera
float _dist;
float _hori;
float _vert;

float _angle;
float _camera_angle_yaw;
float _camera_angle_pitch;
float _gap_factor;
float _alpha = 1;

//flip algorithm specific variables
//kinda out of place
FlipAlgorithm * flipAlg = NULL;
int lastFlip;

//triangulation state info
TriangulationDevelopment coordSystem;

char* fileName;

vector<triangle_parts> listOfTriangles;

map<int,Edge>::iterator selectedEdge;
map<int, Vertex>::iterator selectedVertex;

map<int, Point> points;

int flat;
bool showInfo;
bool showWeights;
bool tickMarks;
bool edgesNotVerts = true;
int voronoi; // 0 is nothing, 1 is the whole voronoi diagram, 2 is the hinge only

//gl vars

int main_window_id;

//gl funcs

void handleKeypress(unsigned char key, int x, int y) {
  map<int, Face>::iterator fit;
  map<int, Edge>::iterator eit;
  vector<int> localVerts;
  struct simps hingeInfo;
  ofstream out();

  bool somethingHappened = true;
  Line l;
	switch (key) {
	   case 27: //Escape key
			exit(0); //Exit the program
            break;
        case 91: // [
            _dist-=1;
            break;
        case 93: // ]
            _dist+=1;
            break;
        case 'd': // d
            cout << "\n\n" << (isWeightedDelaunay(getCurrentEdge()) ? "Edge is weighted Delaunay\n" : "Edge is NOT weighted Delaunay\n");
            cout << "\n" << getDual(getCurrentEdge());
            break;
        case 'e':
          cout << isWeightedDelaunay();
          break;
        case 'p':
          //printStuff(&cout);
          break;
        case ' ': //space bar
            if (edgesNotVerts) {
                flipCurrentEdge();
            } else {
                flip3to1(selectedVertex->second);
            }

            break;
        case 'l': //l
            cout << "geoquant length is " << Length::valueAt(Triangulation::edgeTable[getCurrentEdge().getIndex()]) << "\n";
            l = currentEdgeToLine();
            cout << "distance between vertices is " << sqrt(pow(l.getInitialX() - l.getEndingX(),2) + pow(l.getInitialY() - l.getEndingY(),2)) << "\n\n";
            break;
        case 'k':
          //prep_for_flip(_triDisp->getCurrentEdge(), &hingeInfo);
          //add stuff here to print out hingeInfo values
          break;
        case 'o':
          cout << "writing triangulation file to flip_test/out.txt";
          writeTriangulationFile("../../Data/flip_test/out.txt");
          cout << "finished writing triangulation";
          break;
        case 'r': //r toggles circles centered on the vertices, which represent the weights
            showWeights = !showWeights;
            break;
        //move forward and backward through the list of edges
        case ',': // ,
            if (edgesNotVerts) {
                previousEdge();
            } else {
                previousVertex();
            }
            break;
        case '.': // .
            if (edgesNotVerts) {
                nextEdge();
            } else {
                nextVertex();
            }
            break;
        //grow of shrink the gaps between triangles in the 3d view
        case '\'':
          _gap_factor += .1;
          break;
        case ';':
          _gap_factor -= .1;
          break;
        case 's'://s    switch veiw modes
            flat = (flat + 1)%2;
            if (flat == 1 || flat == 2) {
              glMatrixMode(GL_MODELVIEW);
              glLoadIdentity();
              glTranslatef(0, 0, -10);
            }
            //setup_view_parameters();
            break;
        case 'v': //v toggles the dual edges on and off
            voronoi = !voronoi;
            if (voronoi) {
                coordSystem.computeDuals();
            }
            break;
        case 'n': //n runs the flipalgorithm
            if (flipAlg == NULL) {
                flipAlg = new FlipAlgorithm();
            }
            flipAlg->runFlipAlgorithm();
            break;
        case 'm': //m performs a step of the flip algorithm
            if (flipAlg == NULL) {
                flipAlg = new FlipAlgorithm();
            }
            lastFlip = flipAlg->step();
            setCurrentEdge(lastFlip);
            cout << "\ngonna draw " << lastFlip;
            break;
        case 't'://t toggles tickmarks in the 2d view
            tickMarks = !tickMarks;
            break;
        case 'x':
            edgesNotVerts = !edgesNotVerts;
            break;
        case 'b'://b resets the triangulation
            Triangulation::vertexTable.clear();
            Triangulation::edgeTable.clear();
            Triangulation::faceTable.clear();
            readTriangulationFile(fileName);
            //construct the coordinates
            coordSystem.generatePlane();
            //listOfTriangles = coordSystem.getTriangles();
            selectedEdge = Triangulation::edgeTable.begin();
            makePoints();
            setup_view_parameters();
            flipAlg->resetCurrEdge();
            break;
        case 'i': //prints info about triangulation to the screen
            showInfo = !showInfo;
            break;
        case 'c':
            break;
        default:
            somethingHappened = false;
            break;

	}
    if (somethingHappened) {
        glutPostRedisplay();
    }
}

void handleSpecialKeypress(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
            if (flat == 0) {
                _hori += .5;
            } else {
                _camera_angle_yaw -= _alpha;
            }
            break;
		case GLUT_KEY_RIGHT:
            if (flat == 0) {
                _hori -= .5;
            } else {
                _camera_angle_yaw += _alpha;
            }
            break;
		case GLUT_KEY_UP:
            if (flat == 0) {
                _vert -= .5;
            } else {
                _camera_angle_pitch -= _alpha;
            }
            break;
        case GLUT_KEY_DOWN:
            if (flat == 0) {
                _vert += .5;
            } else {
               _camera_angle_pitch += _alpha;
            }
            break;
	}
    glutPostRedisplay();
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	//Set the camera perspective
	glLoadIdentity();
	gluPerspective(45.0,                  //The camera angle
				   (double)w / (double)h, //The width-to-height ratio
				   1.0,                   //The near z clipping coordinate
				   200.0);                //The far z clipping coordinate
}

//draws ticks along the edge petween p1 and p2 in the direction of p3
void drawTicks(double x1, double y1, double x2, double y2, double x3, double y3, int depth) {
    if (0 == depth) {
        return;
    }
    double avx, avy;
    avx = (x1 + x2) / 2.0;
    avy = (y1 + y2) / 2.0;
    double vecx, vecy;
    vecx = (x3 - avx)/10.0;
    vecy = (y3 - avy)/10.0;
    glBegin(GL_LINES);
        glVertex3d(avx, avy, 0);
        glVertex3d(avx + vecx, avy + vecy, 0);
    glEnd();
    drawTicks(x1, y1, avx, avy, x3, y3, depth-1);
    drawTicks(avx, avy, x2, y2, x3, y3, depth-1);
}

//TODO: THIS IS NEW, TEST IT ADD IT TO .H
void drawText(float x, float y, string text) {
        glPushMatrix();
            glTranslatef(x, y, 0);
            string::iterator sit = text.begin();
            for (; sit != text.end(); sit++) {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *sit);
            }
        glPopMatrix();
}

void drawSceneFlat() {
    glLoadIdentity();
    glTranslatef(_hori, _vert, _dist);

    Edge e;
    e = getCurrentEdge();
    //draws all the edges
    map<int, Edge>::iterator eit;
    eit = Triangulation::edgeTable.begin();
    Point p1, p2;
    glBegin(GL_LINES);
    for (; eit != Triangulation::edgeTable.end(); eit++) {

        if (e.getIndex() == eit->second.getIndex()) {
            //glColor3d(255, 127, 0); //yellow
            glColor3d(0, 0, 255);
        } else {
            glColor3d(0, 0, 255);
        }

        p1 = getVertexCoords((*(eit->second).getLocalVertices())[0]);
        p2 = getVertexCoords((*(eit->second).getLocalVertices())[1]);
        glVertex3d(p1.x, p1.y, 0);
        glVertex3d(p2.x, p2.y, 0);
    }
    glEnd();

    if (edgesNotVerts) {
        //draw the Edge that is currently selected
        p1 = getVertexCoords((*e.getLocalVertices())[0]);
        p2 = getVertexCoords((*e.getLocalVertices())[1]);

        double radius = .2;
        if (Length::valueAt(e) == 0) {
            glBegin(GL_LINE_STRIP);
            glColor3d(255, 127, 0);
            for (double i = 0; i <= 2*PI; i+=0.0174532925) {
                    glVertex3d(p1.x + cos(i) * radius, p1.y + sin(i) * radius, 0);
            }
            glEnd();
        } else { // the lines length is greater than zero
        //draws the selected edge, should appear on top
        glBegin(GL_LINES);
        glColor3d(255, 127, 0);
        glVertex3d(p1.x, p1.y, 0.01);
        glVertex3d(p2.x, p2.y, 0.01);
        glEnd();
        }
    } else { //circle the selected vertex
        p1 = getVertexCoords(selectedVertex->first);
        double radius = .2;
        glBegin(GL_LINE_STRIP);
        glColor3d(255, 127, 0);
        for (double i = 0; i <= 2*PI; i+=0.0174532925) {
            glVertex3d(p1.x + cos(i) * radius, p1.y + sin(i) * radius, 0);
        }
        glEnd();
    }
    
    //draw the duals
    if (voronoi == 1) {
        vector<Line> ds;
        ds = getDuals();
        glBegin(GL_LINES);
        glColor3d(0, 255, 0);
        for(int i = 0; i < ds.size(); i++) {
            glVertex3d(ds[i].getInitialX(),ds[i].getInitialY(),0);
            glVertex3d(ds[i].getEndingX(),ds[i].getEndingY(),0);
        }
        glEnd();
    }//TODO: need to figure out a good way to display only the duals of the current hinge

    //draws the weights around the vertices
    if (showWeights) {
        map<int,Vertex>::iterator vit;
        for (vit = Triangulation::vertexTable.begin(); vit != Triangulation::vertexTable.end(); vit++) {
            Point p;
            p = getPoint(vit->first);
            double radius = Radius::valueAt(vit->second);
            glBegin(GL_LINE_STRIP);
            glColor3d(255,250,250);
            for (double i = 0; i <= 2*PI; i+=0.0174532925) {
                glVertex3d(p.x + cos(i) * radius, p.y + sin(i) * radius, 0);
            }
            glEnd();
        }
    }

    //draw little tick marks along the inside of each triangle
    if (tickMarks) {
        map<int, Face>::iterator fit = Triangulation::faceTable.begin();
        glColor3d(0, 0, 255);
        for (; fit != Triangulation::faceTable.end(); fit++) {

            if (fit->second.isNegative()) {
                glColor3d(255,0,0);
            } else {
                glColor3d(0,0,255);
            }

            vector<int>* verts;
            verts = (fit->second).getLocalVertices();
            Point p1, p2, p3;
            p1 = getPoint((*verts)[0]);
            p2 = getPoint((*verts)[1]);
            p3 = getPoint((*verts)[2]);

            drawTicks(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, 3);
            drawTicks(p3.x, p3.y, p1.x, p1.y, p2.x, p2.y, 3);
            drawTicks(p2.x, p2.y, p3.x, p3.y, p1.x, p1.y, 3);
        }
    }

    if (showInfo) {
        map<int, Face>::iterator fit = Triangulation::faceTable.begin();
        for (; fit != Triangulation::faceTable.end(); fit++) {
            Point p1;
            int count = 0;
            vector<int>::iterator vit = fit->second.getLocalVertices()->begin();
            for (; vit != fit->second.getLocalVertices()->end(); vit++) {
                count += 1;
                p1.x += getPoint(*vit).x;
                p1.y += getPoint(*vit).y;
                p1.x /= count;
                p1.y /= count;
            }

            //build a string holding this number
            int num = fit->first;
            string s;
            while (num) {
                s.insert(0, 1, (num % 10) + 48);
                num /= 10;
            }
            drawText(p1.x, p2.y, s);
        }
    }

}

//##################################################################################################
void drawSceneStacked() {
    //vector<triangle_parts> tps;
    //tps = getTriangles();
    //vector<triangle_parts>::iterator iter;
    //iter = tps.begin();

    //int depth = tps.size();
    map<int,Face>::iterator faces;
    faces = Triangulation::faceTable.begin();
    int depth = Triangulation::faceTable.size();

    glLoadIdentity();
    glTranslatef(0, 0, _dist + (depth * 2 * _gap_factor));
    glRotatef(_camera_angle_yaw, 0, 1, 0);
    glRotatef(_camera_angle_pitch, 1, 0, 0);
    glTranslatef(0, 0, -(depth * _gap_factor)/2.0);
    glTranslatef(_hori, _vert, 0);

    //draws the triangulation
    vector<int>::iterator edges;
    Point p1, p2;
    Edge e;
    for (; faces != Triangulation::faceTable.end(); faces++) {
        glColor3d(255, 127, 0);
        glBegin(GL_LINE_STRIP);

        //draws each edge adjacent to a face
        edges = ((faces->second).getLocalEdges())->begin();
        for (; edges != ((faces->second).getLocalEdges())->end(); edges++) {
          e = Triangulation::edgeTable[*edges];
          p1 = getVertexCoords((*e.getLocalVertices())[0]);
          p2 = getVertexCoords((*e.getLocalVertices())[1]);
          glVertex3d(p1.x, p1.y, 0);
          glVertex3d(p2.x, p2.y, 0);
        }

        glEnd();

        glTranslatef(0, 0, (2* _gap_factor));
    }
}

//Draws the 3D or 2D scene
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective

    //_triDisp->update();

    if (flat == 0) {
        drawSceneFlat();
    } else if (flat == 1) {
        drawSceneStacked();
    }

    glutSwapBuffers();
}

void setup_view_parameters(void) {
    _camera_angle_yaw = 0.0f;
    _camera_angle_pitch = 0.0f;
    double count = 0;
    double sumx = 0;
    double sumy = 0;
    //average all the coordinates to figure out where the view should be centered
    map<int,Vertex>::iterator vit;
    vit = Triangulation::vertexTable.begin();
    Point p;
    p = getPoint(vit->first);
    double minx = p.x;
    double maxx = p.x;
    double miny = p.y;
    double maxy = p.y;
    for (; vit != Triangulation::vertexTable.end(); vit++) {
        p = getPoint(vit->first);
        sumx += p.x;
        sumy += p.y;
        count++;
        if (p.x < minx) {minx = p.x;}
        if (p.x > maxx) {maxx = p.x;}
        if (p.y < miny) {miny = p.y;}
        if (p.y > maxy) {maxy = p.y;}
    }
    double centerx = sumx / count;
    double centery = sumy / count;
    _hori = 0 - centerx;
    _vert = 0 - centery;

    //the screen will be shifted in the positive z direction by _dist
    if (maxx - minx > maxy - miny) {
        _dist = 0 - 2 * abs(maxx - minx);
    } else {
        _dist = 0 - 2 * abs(maxy - miny);
    }
    if (flat == false && _dist > -20) {
      _dist = -20;
    }
}

void startGLStuff(int argc, char * argv[]) {

    //initialize GLUT
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600); //Set the window size

	//create the window
	main_window_id = glutCreateWindow("flip algorithm");
	initRendering(); //Initialize rendering

	//set handler functions for drawing, keypresses, and window resizes
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleSpecialKeypress);
	glutReshapeFunc(handleResize);


    glutMainLoop();
}

//TriangulationDisplay stuff

void ShowTriangulation() {
  ShowTriangulation("");
}

void ShowTriangulation(char * f) {
  char * row = "nothing";
  char ** vec = &row;
  ShowTriangulation(f, 1, vec);
}

void ShowTriangulation(char *f, int argc, char * argv[])
{
    if ((int) strlen(f) > 0l) {
      if (readTriangulationFile(f)) {
          fileName = f;
          //load up the specified file into the maps contained in triangulation.cpp
      } else {
          cout << "BAD FILE NAME!!!\nPerhaps your file ends in a blank line?";
          system("PAUSE");
      }
    }
    
    showWeights = false;
    flat = 0;
    tickMarks = false;
    voronoi = 0;

    //construct the coordinates
    coordSystem.generatePlane();

    //this will be used to display the triangles
    listOfTriangles = coordSystem.getTriangles();
    
    //this will be used to show which edge is currently selected
    selectedEdge = Triangulation::edgeTable.begin();
    selectedVertex = Triangulation::vertexTable.begin();

    //set up the vertex coordinates just this once
    makePoints();
    
    _dist = -20.0f;
    _hori = -5.0f;
    _vert = -5.0f;

    _angle = 30.0f;
    _camera_angle_yaw = 0.0f;
    _camera_angle_pitch = 0.0f;
    _gap_factor = 1;

    setup_view_parameters();

    startGLStuff(argc, argv);
}

void reGeneratePlane() {
    coordSystem.generatePlane();
    listOfTriangles = coordSystem.getTriangles();
    selectedEdge = Triangulation::edgeTable.begin();
    selectedVertex = Triangulation::vertexTable.begin();
}

void setFlipAlgorithm(FlipAlgorithm * f) {
    flipAlg = f;
}

void setFile(char* f)
{
    //very trustworthy, expects user to have called readTriangulationFile somewhere else
    fileName = f;
}

void makePoints(void) {
  map<int, Vertex>::iterator vit;
  vit = Triangulation::vertexTable.begin();
  for (; vit != Triangulation::vertexTable.end(); vit++) {
    Point * p = new Point(coordSystem.getPoint(vit->first).x, coordSystem.getPoint(vit->first).y);
    points.insert(pair<int, Point>(vit->first, *p));
  }
}

Point getVertexCoords(int index) {
  return points[index];
}

char * getCurrentFile()
{
    return fileName;
}

void update() {
    coordSystem.update();
    listOfTriangles = coordSystem.getTriangles();
}

Line currentEdgeToLine() {
    int edgeIndex;
    edgeIndex = (selectedEdge->second).getIndex();
    int v0,v1;
    v0 = (*(selectedEdge->second).getLocalVertices())[0];
    v1 = (*(selectedEdge->second).getLocalVertices())[1];
    return Line(points[v0].x, points[v0].y, points[v1].x, points[v1].y);
}

Edge getCurrentEdge(void) {
    return selectedEdge->second;
}

void setCurrentEdge(int cei) {
    map<int,Edge>::iterator eit = Triangulation::edgeTable.begin();
    while (eit != Triangulation::edgeTable.end()) {
        if (cei == (eit->first)) {
            selectedEdge = eit;
            return;
        }
        eit++;
    }
}

Edge nextEdge(void) {
    selectedEdge++;
    if (Triangulation::edgeTable.end() == selectedEdge) {
        selectedEdge = Triangulation::edgeTable.begin();
    }
    return selectedEdge->second;
}

Edge previousEdge(void) {
    if (Triangulation::edgeTable.begin() == selectedEdge) {
        selectedEdge = Triangulation::edgeTable.end();
        selectedEdge--;
    } else {
        selectedEdge--;
    }
    return selectedEdge->second;
}

Vertex nextVertex(void) {
    selectedVertex++;
    if (Triangulation::vertexTable.end() == selectedVertex) {
        selectedVertex = Triangulation::vertexTable.begin();
    }
    return selectedVertex->second;
}

Vertex previousVertex(void) {
    if (Triangulation::vertexTable.begin() == selectedVertex) {
        selectedVertex = Triangulation::vertexTable.end();
        selectedVertex--;
    } else {
        selectedVertex--;
    }
    return selectedVertex->second;
}

void flipCurrentEdge() {
    flip(Triangulation::edgeTable[(selectedEdge->second).getIndex()]);
}

vector<triangle_parts> getTriangles(void) {
    return listOfTriangles;
}

vector<Line> getDuals(void) {
    coordSystem.computeDuals();
    return coordSystem.getDuals();
}

Point getPoint(int index){
    return coordSystem.getPoint(index);
}

Line getDualAtEdge(int e) {
    return coordSystem.getDual(e);
}

}//end of namespace
