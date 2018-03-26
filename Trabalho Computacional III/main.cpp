/*
    --Trabalho Curto III Computação Gráfica 2016/2
    --Allan Araujo Silva
    --Ciência da Computação UFES
*/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string>
#include <vector>  
#include <iterator> 
#include <iostream>
#include "reader.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

GLfloat GX = 0.0, GY = 0.0;
GLfloat window_size;
//flag tiro do canhao
int shot = 0;
//flag para modificar o efeito da roda
bool moveflag = false;

int key_status[256]; //estados das teclas - se alguma tecla está apertada ou nao

Arena *arena = new Arena(); 
Circle *outer_circle = new Circle();
Circle *inner_circle = new Circle();
Rectangle *start = new Rectangle();
vector<Circle*> enemies;

Car *car = new Car();
vector<Cannonball> shotList;

void keyUp (unsigned char key, int x, int y) {
    
    key_status[key] = 0;
}

void keyPress(unsigned char key, int x, int y) {
    
    key_status[key] = 1;
}

void init(void) {

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(outer_circle->getCenter().getX() - outer_circle->getRadius(), outer_circle->getCenter().getX() + outer_circle->getRadius(), 
        window_size - (outer_circle->getCenter().getY() + outer_circle->getRadius()),
        window_size - (outer_circle->getCenter().getY() - outer_circle->getRadius()), -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void colorCheck(string color) {

    if (color == "blue") 
        glColor3f(0.0, 0.0, 1.0);
    
    else if (color == "red") 
        glColor3f(1.0, 0.0, 0.0);
    
    else if (color == "green") 
        glColor3f(0.0, 1.0, 0.0);
    
    else if (color == "black") 
        glColor3f(0.0, 0.0, 0.0);
    
    else if (color == "white") 
        glColor3f(1.0, 1.0, 1.0);
}

void displayCircle(Circle *circle) {
    
    int triangleAmount = 70;
    float twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
        circle->colorCheck(circle->getFill());
        glVertex2f(circle->getCenter().x, window_size - circle->getCenter().y);
 
        for(int i = 0; i <= triangleAmount; i++)  
            glVertex2f(circle->getCenter().x + (circle->getRadius() * cos(i *  twicePi / triangleAmount)), 
                            window_size - circle->getCenter().y + (circle->getRadius() * sin(i * twicePi / triangleAmount)));
        
    glEnd();
}

void displayRectangle(Rectangle *rect) {
    glBegin(GL_POLYGON);
        colorCheck(rect->getFill());
        glVertex2f(rect->getCenter().getX(), window_size - rect->getCenter().getY());
        glVertex2f(rect->getCenter().getX() + rect->getWidth(), window_size - (rect->getCenter().getY()));
        glVertex2f(rect->getCenter().getX() + rect->getWidth(), window_size - (rect->getCenter().getY() + rect->getHeight()));
        glVertex2f(rect->getCenter().getX(), window_size - (rect->getCenter().getY() + rect->getHeight()));
    glEnd();
}

int crashCheck(Circle *circle, Car *player) {
    
    float cathetus_b = player->getCenter().getX() - circle->getCenter().getX();
    float cathetus_c = player->getCenter().getY() - circle->getCenter().getY();
    float distance = sqrt(cathetus_b*cathetus_b + cathetus_c*cathetus_c);

    if(distance < (circle->getRadius() + player->getRadius()) ) {
        //Colision
        return 1;
    }
    return 0;
}

int crashCheckBoard(Circle *circle, Car *player) {
    
    float cathetus_b = player->getCenter().getX() - circle->getCenter().getX();
    float cathetus_c = player->getCenter().getY() - circle->getCenter().getY();
    float distance = sqrt(cathetus_b*cathetus_b + cathetus_c*cathetus_c);

    if(distance >= (circle->getRadius() - player->getRadius()) ) {
        //Colision
        return 1;
    }
    return 0;
}

void idle() {

    static GLdouble previousTime = 0.0;
    GLdouble currentTime;
    GLdouble timeDiference;
    
    // Elapsed time from the initiation of the game.
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    // Elapsed time from the previous frame.
    timeDiference = currentTime - previousTime;
    //Update previous time 
    previousTime = currentTime; 

    for(vector<Cannonball>::iterator shot = shotList.begin(); shot != shotList.end(); ++shot){
        shot->updateCannonball(timeDiference);
    }

    if((key_status['a'] || key_status['A']) && car->getWheelAngle() <= 45) {
        car->moveWheel(0.7*car->getCarSpeed());
    }

    if((key_status['d'] || key_status['D']) && car->getWheelAngle() >= -45) {
        car->moveWheel(-0.7*car->getCarSpeed());
    }

    if(key_status['w'] || key_status['W']) {
        moveflag = true;
        car->forwardMove(-(0.07*timeDiference)*car->getCarSpeed());

        if(crashCheckBoard(outer_circle, car))
            car->forwardMove((0.07*timeDiference)*car->getCarSpeed());

         if(crashCheck(inner_circle, car))
            car->forwardMove((0.07*timeDiference)*car->getCarSpeed());

        for(int i = 0; i < enemies.size(); i++) {
            
            if(crashCheck(enemies[i], car)) 
               car->forwardMove((0.07*timeDiference)*car->getCarSpeed());
        }
    }
    else moveflag = false;

    if(key_status['s'] || key_status['S']) {
        moveflag = true;
        car->backwardMove((0.07*timeDiference)*car->getCarSpeed());

        if(crashCheckBoard(outer_circle, car))
            car->backwardMove(-(0.07*timeDiference)*car->getCarSpeed());

        if(crashCheck(inner_circle, car))
            car->backwardMove(-(0.07*timeDiference)*car->getCarSpeed());
    
        for(int i = 0; i < enemies.size(); i++) {
            
            if(crashCheck(enemies[i], car)) 
                car->backwardMove(-(0.07*timeDiference)*car->getCarSpeed());
        }
    }
    glutPostRedisplay();
}

void mouseMove(int x, int y) {

    static float positionX = car->getCenter().x;
   
    if((x - positionX > 0) && car->getCannonAngle() >= -45) {
        car->moveCannon(-0.7*car->getCarSpeed());   
    }
    
    if((x - positionX < 0) && car->getCannonAngle() <= 45) {
        car->moveCannon(0.7*car->getCarSpeed());
    }
    positionX = x;
}

void mouseClick(int button, int state, int x, int y) {
    
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { 

            float bodyheight = 2*car->getRadius()/1.7;
            float cannonheight = bodyheight/3;

            Cannonball *newshot = new Cannonball(car->getCenter().x + (cos(car->getCarAngle()*bodyheight)) + (cos(car->getCannonAngle()*cannonheight)),
                                                 car->getCenter().y + (sin(car->getCarAngle()*bodyheight)) + (sin(car->getCannonAngle()*cannonheight)), 
                                                 car->getRadius(), car->getShotSpeed(), car->getShotSize(), 
                                                 car->getCarAngle(), car->getCannonAngle());
           
            newshot->displayCannonball();
            shotList.push_back(*newshot);
            
            delete newshot;
    }
}

void display(void) {

    glClear(GL_COLOR_BUFFER_BIT);

    displayCircle(outer_circle);
    displayCircle(inner_circle);
   
    for(int i = 0; i < enemies.size(); i++) {
        displayCircle(enemies[i]);
    }

    displayRectangle(start);
    car->displayCar();

    for(vector<Cannonball>::iterator shot = shotList.begin(); shot != shotList.end(); ++shot) {
        shot->displayCannonball();
    }
    glutSwapBuffers();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    
    Reader *reader; 
    reader->readFile(argv[1], arena, outer_circle, inner_circle, car, start, enemies);
    window_size = 2*(outer_circle->getRadius());

    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (window_size, window_size);
    glutInitWindowPosition (100, 100);

    glutCreateWindow ("Trabalho Curto II - CG 2016");
    init ();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp); 
    glutPassiveMotionFunc(mouseMove);
    glutMouseFunc(mouseClick);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}