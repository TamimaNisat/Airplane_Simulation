#include <windows.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#define BLOCKSPEED 0.03 //for going forward boost
#define BOOSTER_MAX 500 //for going up boost

int SCREENH = 600, SCREENW = 1250 ; //window size
const char BOOST_KEY = 'x'; //going up boost variable declaration
//---------------- Obstacles declaration----------
struct building {
    float block_x, block_y;
    bool state;
    int no_floors;
};

struct Cloud {
    float block_x, block_y;
    bool state;
};
//-------------------declarations---------------
float bspd = BLOCKSPEED; // block speed in line 7
bool pause = false, lflag = true, wflag = true, gameEndStatus = false, instructionflag = false, abtflag = false, start = false; // flags
float plane_mvmt = 0.0; // jet movement up or down
float score = 1; //game starts with score 1
bool GOING_UP = false;
char score_Str[20], slevel[20]; // score string and level-string
int level = 1, buildColor; // initial level=1
building b; // building structure
Cloud s; // cloud structure
float booster = BOOSTER_MAX, boost = 0;
// plane bounds
///-------------function prototypes------------------
void keyPressed(unsigned char, int, int);
void mouse(int button, int state, int x, int y);
void printString(float x, float y, float z, void* font, char* string); // for printing
void BuildingBlockInit();
void CloudBlockInit();
void setupNewCloudAndBuilding();
void drawJet();
void gameEnd();
void drawBg();
void welcome();
void drawBuilding();
void drawCloud();
bool cloudHit();
bool buildingHit();
void printScore();
void display();
void moveJetU();
void moveJetD();

// initialization of building
void BuildingBlockInit() {
    b.block_x = 50.0; //x axis fixed
    b.no_floors = rand() % 3 + 4; //y axis random
    buildColor = rand() % 3;
    b.block_y = b.no_floors * 10 + 15; // generate block y coordinate depending on no of floors
    b.state = true;
    s.state = false; //while building is coming cloud will not coming
}
// initialization of cloud
void CloudBlockInit() {
    s.block_x = 50.0; //x axis fixed
    s.block_y = (rand() % 30) + 50; // randomly generate block y coordinate
    s.state = true;
    b.state = false; //while cloud is coming building will not coming
}

void drawJet() { //drawing the plane

    // left tail wing

    glColor3f(1.0, 1.0, 0.6);
    glBegin(GL_POLYGON);
    glVertex2f(5.5, 47.0);
    glVertex2f(8.5, 47.0);
    glVertex2f(5.5, 48.0);
    glVertex2f(4.5, 48.0);
    glEnd();

    // left front wing

    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
    glVertex2f(13.0, 47.0);
    glVertex2f(20.0, 47.0);
    glVertex2f(13.0, 50.0);
    glVertex2f(11.0, 50.0);
    glEnd();

    // tail
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
    glVertex2f(4.7, 45.0);
    glVertex2f(5.5, 51.0);
    glVertex2f(7.0, 51.0);
    glVertex2f(9.0, 45.0);
    glEnd();

    // body
    glColor3f(0.5f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(5.0, 48.0);
    glVertex2f(11.0, 48.0);
    glVertex2f(22.0, 46.5);
    glVertex2f(22.0, 45.0);
    glVertex2f(5.0, 45.0);
    glEnd();

    // right front wing
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
    glVertex2f(13.0, 46.0);
    glVertex2f(18.0, 46.0);
    glVertex2f(13.0, 41.0);
    glVertex2f(11.0, 41.0);
    glEnd();

    // dome
    glColor3f(1, 1, 0.9);
    glBegin(GL_POLYGON);
    glVertex2f(13.0, 47.0);
    glVertex2f(15.0, 48.5);
    glVertex2f(17.0, 49.0);
    glVertex2f(19.0, 48.0);
    glVertex2f(21.0, 46.0);
    glVertex2f(17.0, 46.0);
    glVertex2f(15.0, 47.5);
    glVertex2f(13.0, 47.0);
    glEnd();

    // right tail wing
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
    glVertex2f(5.5, 47.0);
    glVertex2f(8.5, 47.0);
    glVertex2f(5.5, 43.0);
    glVertex2f(4.5, 43.0);
    glEnd();

    // front tip
    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_POLYGON);
    glVertex2f(22.0, 45.0);
    glVertex2f(22.3, 45.375);
    glVertex2f(22.6, 45.75);
    glVertex2f(22.3, 46.125);
    glVertex2f(22.0, 46.5);
    glEnd();
}
//ending of drawing plane
void drawString(float x, float y, float z, void* font, char* string) { //for printing
    char* c;
    glRasterPos3f(x, y, z);

    for (c = string;*c != '\0'; c++) {
        glutBitmapCharacter(font, *c); //for writing text
    }
}

void gameEnd() { //game end function
    gameEndStatus = true;        //game end hoile ja ja hobe
    glColor3f(0.97, 0.98, 0.76); // game end background screen

    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glColor3f(0.137, 0.137, 0.556); //game end background screen
    glVertex3f(100.0, 0.0, 0.0);
    glColor3f(0.196, 0.196, 0.8); //game end background screen
    glVertex3f(100.0, 100.0, 0.0);
    glVertex3f(0.0, 100.0, 0.0);
    glEnd();
    glPushMatrix();

    glScalef(0.7, 0.7, 0); //To convert the plane in small size (scaling)
    drawJet();
    glPopMatrix(); //To save previous works
// display box in the game end window
    glColor3f(0.29, 0.16, 0.21); //border color of display box
    glRectf(20.0, 20.0, 80.0, 80.0);
    glColor3f(1, 1, 1); //display box color
    glRectf(21.0, 21.0, 79.0, 79.0);
// restart button in the game end window
    glColor3f(0.29, 0.16, 0.21); //border color of restart button
    glRectf(40, 5, 60, 10);
    glColor3f(1, 1, 1); //box color of restart button
    glRectf(40.5, 5.5, 59.5, 9.5);

    glColor3f(0.42, 0.08, 0.28); //color of the printed characters
    drawString(46, 6, 0, GLUT_BITMAP_TIMES_ROMAN_24, "RESTART");
    drawString(43, 71, 0, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER!!!");
    drawString(27, 55, 0, GLUT_BITMAP_HELVETICA_18, "DISTANCE :");
    drawString(40, 55, 0, GLUT_BITMAP_TIMES_ROMAN_24, score_Str);
    drawString(27, 50, 0, GLUT_BITMAP_HELVETICA_18, "LEVEL         :");
    drawString(40, 50, 0, GLUT_BITMAP_TIMES_ROMAN_24, slevel);
    drawString(40, 30, 0, GLUT_BITMAP_HELVETICA_18, " ENJOY PLAYING THE GAME");

    glutPostRedisplay(); //To continue the window
}
//Creating the background of the window while we play the game
void drawBg() {
    glPushMatrix();
// green floor
    glColor3f(0.26, 0.47, 0.27);

    glBegin(GL_POLYGON);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(100.0, 9.0, 0.0);
    glColor3f(0.0, 0.3, 0.03);
    glVertex3f(100.0, 10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);
    glVertex3f(0.0, 9.0, 0.0);
    glEnd();
 // brown ground
    glColor3f(0.1f, 0.1f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(100.0, 0.0, 0.0);
    glColor3f(0.3, 0.1, 0.03);
    glVertex3f(100.0, 9.0, 0.0);
    glVertex3f(0.0, 9.0, 0.0);
    glEnd();
// ceiling
    glColor3f(0.31, 0.43, 0.52);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 100.0, 0.0);
    glVertex3f(100.0, 100.0, 0.0);
    glColor3f(0.6, 0.7, 0.89);
    glVertex3f(100.0, 80.0, 0.0);
    glVertex3f(0.0, 80.0, 0.0);
    glEnd();
// sky
    glColor3f(0.47, 0.85, 0.82); //0.5, 0.6, 0.79 // sky blue
    glBegin(GL_POLYGON); // background screen
    glVertex3f(0.0, 90.0, 5.0);
    glVertex3f(100.0, 90.0, 5.0);
    glColor3f(0.9, 0.96, 1);
    glVertex3f(100.0, 10.0, 5.0);
    glVertex3f(0.0, 10.0, 5.0);
    glEnd();

    glPopMatrix();
}
//Creating the welcome window, while we run the project
void welcome() {
    glColor3f(0.68, 0.5, 0.5); // welcome background
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glColor3f(0.137, 0.137, 0.556);
    glVertex3f(100.0, 0.0, 0.0);
    glColor3f(0.196, 0.196, 0.8);
    glVertex3f(100.0, 100.0, 0.0);
    glVertex3f(0.0, 100.0, 0.0);
    glEnd();
    drawJet();

    glColor3f(1, 1, 1);
    drawString(32.5, 75, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Simulation For Real-world Events of Airplane");
    glColor3f(1, 1, 1);
    drawString(46.5, 70, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Sky Rider");
    glColor3f(0.137, 0.137, 0.556);

    // button 1 .. PLAY
    glColor3f(0.196, 0.196, 0.8);
    glRectf(39.5, 39.5, 60.5, 45.5);

    glColor3f(1, 1, 1);
    glRectf(40, 40, 60, 45);
    glColor3f(0.24, 0.05, 0.2);
    drawString(48, 42, 0, GLUT_BITMAP_HELVETICA_18, "PLAY");

    // button 2 .. instructions
    glColor3f(0.196, 0.196, 0.8);
    glRectf(39.5, 29.5, 60.5, 35.5);

    glColor3f(1, 1, 1);
    glRectf(40, 30, 60, 35);
    glColor3f(0.24, 0.05, 0.2);
    drawString(44, 31, 0, GLUT_BITMAP_HELVETICA_18, "INSTRUCTIONS");

    // button 3 .. ABOUT
    glColor3f(0.196, 0.196, 0.8);
    glRectf(39.5, 19.5, 60.5, 25.5);

    glColor3f(1, 1, 1);
    glRectf(40, 20, 60, 25);
    glColor3f(0.24, 0.05, 0.2);
    drawString(47, 21, 0, GLUT_BITMAP_HELVETICA_18, "ABOUT");

    // button 4 .. exit
    glColor3f(0.196, 0.196, 0.8);
    glRectf(39.5, 9.5, 60.5, 15.5);

    glColor3f(1, 1, 1);
    glRectf(40, 10, 60, 15);
    glColor3f(0.24, 0.05, 0.2); //0.137, 0.137, 0.556
    drawString(48, 11, 0, GLUT_BITMAP_HELVETICA_18, "EXIT");
}

void drawBuilding() {
    glPushMatrix();
    //Drawing side shape of the buildings
    if (buildColor == 0)
        glColor3f(0.1, 0.0, 0.0);
    else if (buildColor == 1)
        glColor3f(0.1, 0.1, 0.0);
    else
        glColor3f(0.0, 0.1, 0.1);

    glTranslatef(b.block_x, b.no_floors * 10.0 + 10, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(5.0, 3.0, 0.0);
    glVertex3f(20.0, 3.0, 0.0);
    glVertex3f(20.0, -b.no_floors * 10.0, 0.0);
    glVertex3f(0.0, -b.no_floors * 10.0, 0.0);
    glEnd();
    glPopMatrix();

    for (int i = 1; i <= b.no_floors; i++) {
        glPushMatrix();
//Drawing Upper part of the building
        if (buildColor == 0)
            glColor3f(0.53, 0.3, 0.46); //purple
        else if (buildColor == 1)
            glColor3f(0.81, 0.82, 0.45);  //yellow
        else
            glColor3f(0.85, 0.65, 0.5); //orange

        glTranslatef(b.block_x, 10.0 * i, 0.0); // base
        glBegin(GL_POLYGON);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(15.0, 0.0, 0.0);
        glVertex3f(15.0, 10.0, 0.0);
        glVertex3f(0.0, 10.0, 0.0);
        glEnd();
        glColor3f(1.0, 1.0, 1.0); // left window
        glBegin(GL_POLYGON);
        glVertex3f(2.5, 5.0, 0.0);
        glVertex3f(5.5, 5.0, 0.0);
        glVertex3f(5.5, 8.0, 0.0);
        glVertex3f(2.5, 8.0, 0.0);
        glEnd();
        glColor3f(1.0, 1.0, 1.0); // left window
        glBegin(GL_POLYGON);
        glVertex3f(2.5, 0.0, 0.0);
        glVertex3f(5.5, 0.0, 0.0);
        glVertex3f(5.5, 3.0, 0.0);
        glVertex3f(2.5, 3.0, 0.0);
        glEnd();
        glColor3f(1.0, 1.0, 1.0); // right window
        glBegin(GL_POLYGON);
        glVertex3f(12.5, 5.0, 0.0);
        glVertex3f(9.5, 5.0, 0.0);
        glVertex3f(9.5, 8.0, 0.0);
        glVertex3f(12.5, 8.0, 0.0);
        glEnd();
        glColor3f(1.0, 1.0, 1.0); // right window
        glBegin(GL_POLYGON);
        glVertex3f(12.5, .0, 0.0);
        glVertex3f(9.5, 0.0, 0.0);
        glVertex3f(9.5, 3.0, 0.0);
        glVertex3f(12.5, 3.0, 0.0);
        glEnd();
        glPopMatrix();
    }
    glPushMatrix();
//Drawing Lower part of the building
    if (buildColor == 0)
        glColor3f(0.53, 0.3, 0.46); //purple
    else if (buildColor == 1)
        glColor3f(0.81, 0.82, 0.45); //yellow
    else
        glColor3f(0.85, 0.65, 0.5); //Orange

    glTranslatef(b.block_x, 10.0, 0.0); // base
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(15.0, 0.0, 0.0);
    glVertex3f(15.0, 10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);
    glEnd();
    glColor3f(1.0, 1.0, 1.0); // door
    glBegin(GL_POLYGON);
    glVertex3f(5.5, 0.0, 0.0);
    glVertex3f(9.5, 0.0, 0.0);
    glVertex3f(9.5, 6.0, 0.0);
    glVertex3f(5.5, 6.0, 0.0);
    glEnd();
    glPopMatrix();
}

void drawCloud() {
    glColor3f(1.0, 1.0, 1.0);

    glTranslatef(s.block_x, s.block_y, 0.0);
    glutSolidSphere(5, 100, 10);

    glTranslatef(6, -3.0, 0.0);
    glutSolidSphere(5, 100, 10);

    glTranslatef(0, 6.0, 0.0);
    glutSolidSphere(5, 100, 10);

    glTranslatef(6, -3.0, 0.0);
    glutSolidSphere(5, 100, 10);
}
//4 logics of when the plane hits the cloud
bool cloudHit() {
    if (s.block_x < 13 && s.block_x > -5)
        if (plane_mvmt - 3 + 50 > s.block_y - 3 && plane_mvmt - 3 + 50 < s.block_y + 3) // plane front to cloud mid box1
            return true;

    if (s.block_x < 12 && s.block_x > -4)
        if (plane_mvmt - 3 + 50 > s.block_y - 5 && plane_mvmt - 3 + 50 < s.block_y + 5) // plane front to cloud mid box2
            return true;

    if (s.block_x < 10 && s.block_x > -1)
        if (plane_mvmt - 3 + 50 > s.block_y - 6 && plane_mvmt - 3 + 50 < s.block_y - 2)
            return true;

    // for top wing and bottom wing
    if (s.block_x < 5 && s.block_x > -3)
        if (plane_mvmt - 3 + 50 > s.block_y - 11 && plane_mvmt - 3 + 50 < s.block_y + 13)
            return true;

    return false;
}
//4 logics of when the plane hits the building
bool buildingHit() {
    if (((int)b.block_x <= 8 && (int)b.block_x >= -7 && ((int)plane_mvmt + 50) - b.block_y <= 3)) // building back  body to tail
        return true;
    else if (((int)b.block_x <= 10 && (int)b.block_x >= -5 && ((int)plane_mvmt + 50) - b.block_y <= 0)) // body to tail
        return true;
    else if (((int)b.block_x <= 6 && (int)b.block_x >= -3 && ((int)plane_mvmt + 47) - b.block_y <= 0)) // right wing
        return true;
    else if (((int)b.block_x <= 4 && (int)b.block_x >= -4 && ((int)plane_mvmt + 47) - b.block_y <= 3)) //   building back right wing
        return true;
    else
        return false;
}
//2 logics of when the plane hits the upper and lower boundary of the window
bool boundHit() {
    if (plane_mvmt + 50 >= 100 || plane_mvmt + 50 <= 18) // top and bottom boundary
        return true;
    else
        return false;
}

void printScore() {
    glColor3f(1.0, 1.0, 0.0); // score

    sprintf(slevel, "%d", (int)level);
    drawString(58, 1.8, 0, GLUT_BITMAP_TIMES_ROMAN_10, "Level");
    drawString(58, 3.5, 0, GLUT_BITMAP_TIMES_ROMAN_24, slevel);

    if (booster > 0 && boost)
        score += 0.03; // SCORE with booster (When we use boost score will increase slower)
    else
        score += 0.005; // SCORE without booster(When don't use boost score will increase faster)

    drawString(38, 1.5, 0, GLUT_BITMAP_TIMES_ROMAN_10, "Distance");
    sprintf(score_Str, "%d m", (int)score);
    drawString(38, 3, 0, GLUT_BITMAP_TIMES_ROMAN_24, score_Str);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // GameOver Checking
    if (gameEndStatus == true) {
        gameEnd();
    }
    else if (wflag == true) // Welcome Screen
    {
        welcome();
    }
    else if (instructionflag == true) { //instruction window
        glColor3f(1, 1, 1); //0.3, 0.56, 0.84// background
        glBegin(GL_POLYGON);
        glVertex3f(0.0, 0.0, 0.0);
        glColor3f(0.31, 0.2, 0.29);
        glVertex3f(100.0, 0.0, 0.0);
        glColor3f(0.2, 0.22, 0.31);
        glVertex3f(100.0, 100.0, 0.0);
        glVertex3f(0.0, 100.0, 0.0);
        glEnd();
        glPushMatrix();
        glScalef(0.8, 0.8, 0);
        drawJet();
        glPopMatrix();
        glColor3f(0.29, 0.16, 0.21); //Border color of Instruction box
        glRectf(20.0, 14.0, 80.0, 87.0);
        glColor3f(1, 1, 1); //Color of Instruction box
        glRectf(21.0, 16.0, 79.0, 85.0);

        glColor3f(0.29, 0.16, 0.21); //Border color of back button
        glRectf(40, 5, 60, 10);
        glColor3f(1, 1, 1); //Color of back button
        glRectf(40.5, 5.5, 59.5, 9.5);

        glColor3f(0.42, 0.08, 0.28);
        drawString(47, 6.3, 0, GLUT_BITMAP_TIMES_ROMAN_24, "BACK");

        glColor3f(0.42, 0.08, 0.28);//glColor3f(0.137, 0.137, 0.556);
        drawString(44, 77, 0, GLUT_BITMAP_TIMES_ROMAN_24, "HOW TO PLAY");
        drawString(23, 68, 0, GLUT_BITMAP_HELVETICA_18, "-  Click and hold mouse left key to gain altitude of the plane.");
       // drawString(23, 65, 0, GLUT_BITMAP_HELVETICA_18, "    the plane.");
        drawString(23, 64, 0, GLUT_BITMAP_HELVETICA_18, "-  Release the mouse left key to reduce the altitude.");
        drawString(23, 60, 0, GLUT_BITMAP_HELVETICA_18, "-  Use the Right mouse key to speed up the plane(NOS)");
        drawString(23, 56, 0, GLUT_BITMAP_HELVETICA_18, "-  Use the 'x' button of the keyboard for going up fast");
        drawString(23, 52, 0, GLUT_BITMAP_HELVETICA_18, "-  Press 'p' to pause the game and 'e' to end the game, from the keyboard");
        drawString(23, 48, 0, GLUT_BITMAP_HELVETICA_18, "-  Main aim is to avoid the obstacles such as buildings and clouds and");
        drawString(23, 44, 0, GLUT_BITMAP_HELVETICA_18, "    falling on the ground or hit the ceiling");
        //drawString(23, 49, 0, GLUT_BITMAP_HELVETICA_18, "    ");
        drawString(23, 40, 0, GLUT_BITMAP_HELVETICA_18, "-  Also the meter at the bottom shows the distance traveled,");
        drawString(23, 36, 0, GLUT_BITMAP_HELVETICA_18, "    NITROS left, Altitude and the LEVEL.");
        drawString(23, 32, 0, GLUT_BITMAP_HELVETICA_18, "-  As you reach distance multiples of 50 tour level increases as well ");
        drawString(23, 28, 0, GLUT_BITMAP_HELVETICA_18, "    as the speed of the plane.");
        drawString(38, 22, 0, GLUT_BITMAP_HELVETICA_18, "[....ENJOY PLAYING THE GAME....]");

        glutPostRedisplay();
    }
    else if (abtflag == true) {
        glColor3f(0.3, 0.56, 0.84); // background
        glBegin(GL_POLYGON);
        glVertex3f(0.0, 0.0, 0.0);
        glColor3f(0.18, 0.19, 0.27);
        glVertex3f(100.0, 0.0, 0.0);
        glColor3f(0.8, 0.59, 0.72);
        glVertex3f(100.0, 100.0, 0.0);
        glVertex3f(0.0, 100.0, 0.0);
        glEnd();
        glPushMatrix();
        glScalef(0.7, 0.8, 0);
        drawJet();
        glPopMatrix();
        glColor3f(0.29, 0.16, 0.21); //Border color of the box
        glRectf(20.0, 15.0, 80.0, 87.0);
        glColor3f(1, 1, 1); //Color of the box
        glRectf(21.0, 17.0, 79.0, 85.0);

        glColor3f(0.29, 0.16, 0.21); //Color of border of back button
        glRectf(40, 5, 60, 10);
        glColor3f(1, 1, 1); //Color of back button
        glRectf(40.5, 5.5, 59.5, 9.5);
        glColor3f(0.42, 0.08, 0.28);
        drawString(47, 6, 0, GLUT_BITMAP_TIMES_ROMAN_24, "BACK");

        glColor3f(0.1, 0.1, 0.1); //0.137, 0.137, 0.556
        drawString(47, 78, 0, GLUT_BITMAP_TIMES_ROMAN_24, "ABOUT");
        drawString(45.5, 77, 0, GLUT_BITMAP_TIMES_ROMAN_24, "_________");
        glColor3f(0.42, 0.08, 0.28);
        drawString(46, 70, 0, GLUT_BITMAP_HELVETICA_18, "Published by: ");
        drawString(39, 62, 0, GLUT_BITMAP_HELVETICA_18, "Tasnim Islam   (ID 19202103313)");
        drawString(39, 57, 0, GLUT_BITMAP_HELVETICA_18, "Al Rifat Hasan  (ID 19202103311)");
        drawString(39, 52, 0, GLUT_BITMAP_HELVETICA_18, "Kaniz Farzana  (ID 19202103413)");
        drawString(39, 47, 0, GLUT_BITMAP_HELVETICA_18, "Tamima Nisat   (ID 19202103309)");
        drawString(43, 38, 0, GLUT_BITMAP_HELVETICA_18, "Department of CSE");
        drawString(30, 33, 0, GLUT_BITMAP_HELVETICA_18, "BANGLADESH UNIVERSIY OF BUSINESS & TECHNOLOGY");

        drawString(38.5, 22, 0, GLUT_BITMAP_HELVETICA_18, "[...ENJOY PLAYING THE GAME...]");
        glutPostRedisplay();
    }
    else if (pause == true) {
        drawBg();
        glPushMatrix();
        glScalef(0.7, 0.7, 0);
        drawJet();
        glPopMatrix();
        glPushMatrix();
        glColor3f(0.29, 0.16, 0.21);
        glRectf(35.0, 40.0, 65.0, 60.0);
        glColor3f(1, 1, 1);
        glRectf(36.0, 41.0, 64.0, 59.0);
        glPopMatrix();
        glColor3f(0.29, 0.16, 0.21);
        drawString(44,55,0,GLUT_BITMAP_HELVETICA_18," GAME PAUSED");
        drawString(42,45,0,GLUT_BITMAP_HELVETICA_18," PRESS 'P' to continue");
        glutPostRedisplay();
    }
    else if (b.state == true && buildingHit() == true) { //game will end when plane hit building
        gameEndStatus = true;
        gameEnd();
    }
    else if (s.state == true && cloudHit() == true) { //game will end when plane hit cloud
        gameEndStatus = true;
        gameEnd();
    }
    else if (boundHit() == true) { //game will end when plane hit boundary
        gameEndStatus = true;
        gameEnd();
    }
    else {
        if ((int)score % 50 == 0 && lflag == true) // l-level
        {
            lflag = false;
            level++;
            bspd += 0.001;
        }
        else if ((int)score % 50 != 0 && lflag == false) {
            lflag = true;
        }

        glPushMatrix();
        drawBg();

        glPushMatrix();
        glTranslatef(0.0, plane_mvmt, 0.0);

        if (GOING_UP) {
            printf("State: GOING UP\n");
            glRotatef(1.0, 0, 0, 1); // anticlockwise rotation while going up
        }
        else {
            printf("State: GOING DOWN\n");
            glRotatef(-0.6, 0, 0, 1); // Clock wise, while going down
        }

        drawJet(); // code for jet
        glPopMatrix();

        if (booster <= BOOSTER_MAX && !boost) // booster charging
            booster += 0.005;

        if ((b.state == true && b.block_x < -10) || (s.state == true && s.block_x < -10)) // for new building //building has gone outside the screen- state=true
        {
            setupNewCloudAndBuilding();
        }
        else if (b.state == true) {
            if (booster > 0 && boost) {
                b.block_x -= bspd + boost;
                booster = booster - 0.02; // reduce to normal speed after leaving boost key
            }
            else {
                b.block_x -= bspd;
            }
            glTranslatef(b.block_x, 0.0, 0.0);
            drawBuilding();
        }
        else if (s.state == true) {
            if (booster > 0 && boost) {
                s.block_x -= bspd + boost;
                booster = booster - 0.02;
            }
            else {
                s.block_x -= bspd;
            }
            glTranslatef(s.block_x, 0.0, 0.0);
            drawCloud();
        }

        glPopMatrix();
        printScore();
    }
    glFlush();
    glutSwapBuffers();
}

void moveJetU() // jet moving up
{
    if (start == false)
        glutPostRedisplay();
    else if (pause == false) {
        // alti_ang-=0.15;
        plane_mvmt += 0.05;
        glutPostRedisplay();
    }
}

void moveJetUBoost() // jet moving up
{
    if (start == false)
        glutPostRedisplay();
    else if (pause == false) {
        // alti_ang-=0.15;
        plane_mvmt += 0.08;
        glutPostRedisplay();
    }
}

void moveJetD() // jet moving down
{
    if (start == false)
        glutPostRedisplay();
    else if (pause == false) {
        // alti_ang+=0.15;
        plane_mvmt -= 0.04;
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) // takes input from mouse
{
    int mx = x * 100 / SCREENW, my = (SCREENH - y) * 100 / SCREENH; // m = mouse coordinate to graphics

    /*		mouse calculation//converting to screen coordinates-ortho values
    SCREENSIZE  ---->  ORTHO
    x(read val) ---->  ???
    */
    if (instructionflag || abtflag || gameEndStatus) {
        if (mx > 40 && mx < 60) {
            if (my > 5 && my < 10) {
                wflag = true;
                if (instructionflag)
                    instructionflag = false;
                else if (abtflag)
                    abtflag = false;
                if (gameEndStatus) {
                    wflag = true;
                    gameEndStatus = false;
                    plane_mvmt = 0;
                    start = false;
                    setupNewCloudAndBuilding();
                    bspd = BLOCKSPEED; // restarting the game
                    booster = BOOSTER_MAX;
                    score = 1;
                    level = 1;
                    glutPostRedisplay();
                }
            }
        }
    }

    if (wflag == true) {
        if (mx > 40 && mx < 60) {
            if (my > 40 && my < 45) {
                start = true;
                wflag = false;
            }
            else if (my > 30 && my < 35) {
                instructionflag = true;
                wflag = false;
            }
            else if (my > 20 && my < 25) {
                abtflag = true;
                wflag = false;
            }
            else if (my > 10 && my < 15) {
                exit(0);
            }
        }
    }

    else {
        if (button == GLUT_LEFT_BUTTON) {
            if (state == GLUT_DOWN) {
                GOING_UP = true;
                glutIdleFunc(moveJetU);
            }
            else if (state == GLUT_UP) {
                GOING_UP = false;
                glutIdleFunc(moveJetD);
            }
        }
        if (button == GLUT_RIGHT_BUTTON) {
            if (state == GLUT_DOWN) {
                if (booster > 0) {
                    boost = 0.03;
                }
            }
            if (state == GLUT_UP) {
                boost = 0;
            }
        }
    }
}
void keyPressed(unsigned char key, int x, int y) // int x and y are mouse pos at time of press
{
    if (key == BOOST_KEY) {
        glutIdleFunc(moveJetUBoost);
    }
    if (key == 'e') {
        exit(0);
    }
    else if (key == 'p' || key == 'P') {
        if (pause == true)
            pause = false;
        else
            pause = true;
    }
    glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y) {
    if (key == BOOST_KEY) {
        glutIdleFunc(moveJetD);
    }
}

void myReshape(int w, int h) {
    SCREENH = h, SCREENW = w;
    printf("width = %d\theight= %d", w, h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 100.0, 0.0, 100.0, -5.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

/** @brief  In this function, no object is drawn.
 * We are initializing the object structure i.e.
 * the x and y values, building color etc.
 * **/

void setupNewCloudAndBuilding() {
    srand(time(0));
    int random = rand() % 2;
    if (random == 0) {
        BuildingBlockInit();
    }
    else {
        CloudBlockInit();
    }
}

void printInstruction() {
    printf("\nHow To Play:\n");
    printf(" -Click and hold the mouse left key to gain altitude of the plane.\n");
    printf(" -Release the mouse left key to reduce the altitude\n");
    printf(" -Use the Right mouse key to speed up the plane(NOS)\n");
    printf(" -Use the 'x' button of the keyboard for going up fast\n");
    printf(" -Press 'p' to pause the game and 'e' to end the game, from the keyboard\n");
    printf("\n");
    printf("-->The main aim of the game is to avoid the obstacles: buildings and clouds, and avoid falling on\n");
    printf("   the ground or hit the ceiling\n");
    printf("-->Also the meter at the bottom shows the distance traveled,NITROS left,Altitude and LEVEL.\n");
    printf("\n");
    printf("-->As you reach distance multiples of 50 tour level increases as well as the speed of the plane.\n");
    printf("==>ENJOY PLAYING THE GAME\n");
}

int main(int argc, char** argv) {
    printInstruction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(SCREENW, SCREENH);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Airplane Simulation Game");
    setupNewCloudAndBuilding();
    glutDisplayFunc(display);
    glutReshapeFunc(myReshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutMainLoop();
    return 0;
}
