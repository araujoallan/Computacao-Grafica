/*
    --Trabalho Curto IV Computação Gráfica 2016/2
    --Allan Araujo Silva
    --Ciência da Computação UFES
*/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>  
#include <time.h>  
#include <iterator> 
#include <iostream>
#include "reader.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

GLfloat GX = 0.0, GY = 0.0;
GLfloat window_size;
GLfloat shotFreq;

static char text_time[2000];
void *timefont = GLUT_BITMAP_HELVETICA_18;
void *statusfont = GLUT_BITMAP_TIMES_ROMAN_24;

bool startgame = false, gameover = false, youwin = false;
bool checkpoint_1 = false, checkpoint_2 = false, checkpoint_3 = false;
int key_status[256]; //estados das teclas - se alguma tecla está apertada ou nao

Arena *arena = new Arena(); 
Circle *outer_circle = new Circle();
Circle *inner_circle = new Circle();
Rectangle *start = new Rectangle();
Car *car = new Car();

vector<Car*> enemies;
vector<Cannonball> shotList;
vector<Cannonball> enemyShotList;

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

int hitTarget() {
    for(vector<Cannonball>::iterator shot = enemyShotList.begin(); shot != enemyShotList.end(); ++shot) {     
        float cathetus_b = car->getCenter().getX() - shot->getX();
        float cathetus_c = car->getCenter().getY() - shot->getY();
        float distance = sqrt(cathetus_b*cathetus_b + cathetus_c*cathetus_c);

        if(distance <= (0.07*shot->getCarRadius() + car->getRadius()) ) {
            //Colision
            return 1;
        }
        return 0;
    }
}

int crashBetweenEnemies(Car *enemy) {
    for(int i = 0; i < enemies.size(); i++) {
        if(enemies[i]->getCenter().getX() == enemy->getCenter().getX() && enemies[i]->getCenter().getY() == enemy->getCenter().getY()) {
            continue;
        }
        else if(enemy->crashCheck(enemies[i]))
            return 1;
        return 0;
    }
}

void enemyMoveA(Car *enemy) {

    static int cannon_direction = 1;
    static int wheel_direction = 1;
    static int direction = 1;
    float wheel_movement = rand() %  10 + 1;
    float cannon_movement = rand() %  10 + 1;
    static float forward_movement = rand() % 100 + 20;
    float backward_movement = rand() % 100 + 20;

    //Movimento aleatorio dos inimigos
    if(startgame) {

            //Movimento do canhao
            enemy->moveCannon(cannon_movement * cannon_direction* 0.6 * car->getCarSpeed());

            if(enemy->getCannonAngle() < -45) {
                cannon_direction = 1;
                enemy->setCannonAngle(-45.0);  
                      
            }else if(enemy->getCannonAngle() > 45) {
               cannon_direction = -1;
               enemy->setCannonAngle(45);
            }
            //Movimento das rodas
            if(enemy->getWheelAngle() < -45) {
                wheel_direction *= -1;
                enemy->setWheelAngle(-45.0);  
                      
            }else if(enemy->getWheelAngle() > 45) {
               wheel_direction *= -1;
               enemy->setWheelAngle(45);
            }

            enemy->forwardMove(direction * forward_movement/100 * enemy->getEnemySpeed());
            //Movimento dos carros
            if(forward_movement > 0 || forward_movement <= 3) {

                if(enemy->crashCheckBoard(outer_circle)) {
                    direction *= -1;
                    enemy->moveWheel(direction * enemy->getEnemySpeed());
                    enemy->backwardMove(direction * enemy->getEnemySpeed());
                }

                if(enemy->crashCheck(inner_circle)) {
                    direction *= -1;
                    enemy->forwardMove(direction * enemy->getEnemySpeed()); 
                    enemy->moveWheel(direction * wheel_movement/10 * enemy->getEnemySpeed());
                }
                
                if(enemy->crashCheck(car) || car->crashCheck(enemy) ) {
                    direction *= -1;
                    enemy->forwardMove(direction * enemy->getEnemySpeed());  
                    enemy->moveWheel(direction * enemy->getEnemySpeed());
                }
                
                if(crashBetweenEnemies(enemy)) {
                    direction *= -1;
                    enemy->forwardMove(direction * forward_movement/100 * enemy->getEnemySpeed());  
                    enemy->moveWheel(direction * enemy->getEnemySpeed());
                }
            }
            else if(forward_movement > 3 || forward_movement > 7) {
                    direction = -1;
                    enemy->backwardMove(direction * forward_movement/100 * enemy->getEnemySpeed());

                    if(enemy->crashCheckBoard(outer_circle)) {
                        enemy->moveWheel(wheel_movement/10 * enemy->getEnemySpeed());
                        enemy->backwardMove(backward_movement * enemy->getEnemySpeed());
                    }

                    if(enemy->crashCheck(inner_circle)) {
                        direction *= -1;
                        enemy->backwardMove(direction * enemy->getEnemySpeed()); 
                        enemy->moveWheel(direction * wheel_movement/10 * enemy->getEnemySpeed());
                    }
                
                    if(enemy->crashCheck(car) || car->crashCheck(enemy) ) {
                        enemy->forwardMove(direction * enemy->getEnemySpeed());  
                        enemy->moveWheel(direction * enemy->getEnemySpeed());
                    }
                    
                    if(crashBetweenEnemies(enemy)) {
                        direction *= -1;
                        enemy->forwardMove(direction * forward_movement/100 * enemy->getEnemySpeed());  
                        enemy->moveWheel(direction * enemy->getEnemySpeed());
                    }
            }
        }
}

void enemyMoveB(Car *enemy) {

    static int cannon_direction = 1;
    static int wheel_direction = 1;
    static int direction = 1;
    float wheel_movement = rand() %  10 + 1;
    float cannon_movement = rand() %  10 + 1;
    static float forward_movement = rand() % 100 + 20;
    float backward_movement = rand() % 100 + 20;

    //Movimento aleatorio dos inimigos
    if(startgame) {

            //Movimento do canhao
            enemy->moveCannon(cannon_movement * cannon_direction* 0.6 * car->getCarSpeed());

            if(enemy->getCannonAngle() < -45) {
                cannon_direction = 1;
                enemy->setCannonAngle(-45.0);  
                      
            }else if(enemy->getCannonAngle() > 45) {
               cannon_direction = -1;
               enemy->setCannonAngle(45);
            }
            //Movimento das rodas
            if(enemy->getWheelAngle() < -45) {
                wheel_direction *= -1;
                enemy->setWheelAngle(-45.0);  
                      
            }else if(enemy->getWheelAngle() > 45) {
               wheel_direction *= -1;
               enemy->setWheelAngle(45);
            }

            enemy->backwardMove(direction * backward_movement/100 * enemy->getEnemySpeed());
            enemy->moveWheel(direction * 0.3 * enemy->getEnemySpeed());
            //Movimento dos carros
            if(forward_movement > 0 || forward_movement <= 3) {

                if(enemy->crashCheckBoard(outer_circle)) {
                    direction *= -1;
                    enemy->backwardMove(0.7 * direction * enemy->getEnemySpeed());  
                    enemy->moveWheel(direction * enemy->getEnemySpeed());

                }

                if(enemy->crashCheck(inner_circle)) {
                    direction *= -1;
                    enemy->moveWheel(direction * wheel_movement/10 * enemy->getEnemySpeed());
                    enemy->backwardMove(direction * enemy->getEnemySpeed());  

                }
                
                if(enemy->crashCheck(car) || car->crashCheck(enemy) ) {
                    direction *= -1;
                    enemy->forwardMove(direction * enemy->getEnemySpeed());  
                    enemy->moveWheel(1.5 * direction * enemy->getEnemySpeed());
                }
                
                if(crashBetweenEnemies(enemy)) {
                    direction *= -1;
                    enemy->forwardMove(direction * forward_movement/100 * enemy->getEnemySpeed());  
                    enemy->moveWheel(-2.5* direction * enemy->getEnemySpeed());
                }
            }
            else if(forward_movement > 3 || forward_movement > 7) {
                    direction = -1;
                    enemy->backwardMove(direction * forward_movement/100 * enemy->getEnemySpeed());

                    if(enemy->crashCheckBoard(outer_circle)) {
                        enemy->moveWheel(wheel_movement/10 * enemy->getEnemySpeed());
                        enemy->backwardMove(backward_movement * enemy->getEnemySpeed());
                    }

                    if(enemy->crashCheck(inner_circle)) {
                        enemy->forwardMove(direction * enemy->getEnemySpeed()); 
                        enemy->moveWheel(direction * wheel_movement/10 * enemy->getEnemySpeed());
                    }
                
                    if(enemy->crashCheck(car) || car->crashCheck(enemy) ) {
                        enemy->forwardMove(direction * enemy->getEnemySpeed());  
                        enemy->moveWheel(direction * enemy->getEnemySpeed());
                    }
                    
                    if(crashBetweenEnemies(enemy)) {
                        direction *= -1;
                        enemy->forwardMove(direction * forward_movement/100 * enemy->getEnemySpeed());  
                        enemy->moveWheel(direction * enemy->getEnemySpeed());
                    }
            }
        }
}


void checkPoints() {
    //passagem do 1o quadrante para 2o quadrante
    if(car->getCenter().getX() <= inner_circle->getCenter().getX() && 
                        car->getCenter().getY() <= inner_circle->getCenter().getY()) {
            checkpoint_1 = true;
    }
    //passagem do 2o quadrante para 3o quadrante
    if(car->getCenter().getX() <= inner_circle->getCenter().getX() && 
                        car->getCenter().getY() >= inner_circle->getCenter().getX() && checkpoint_1) {
            checkpoint_2 = true;
    }
    //passagem do 3o quadrante para 4o quadrante
    if(car->getCenter().getX() >= inner_circle->getCenter().getX() &&
                        car->getCenter().getY() >= inner_circle->getCenter().getY() && checkpoint_1 && checkpoint_2) {
            checkpoint_3 = true;
    }
    //passagem do 4o quadrante para 1o quadrante
    if(car->getCenter().getX() >= inner_circle->getCenter().getX() &&
                        car->getCenter().getY() <= inner_circle->getCenter().getY() && checkpoint_1 && checkpoint_2 && checkpoint_3) {
            youwin = true;
    }
}

void idle() {
        //float value = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        static GLdouble previousTime = 0.0;
        static GLdouble startshoots;
        GLdouble currentTime;
        GLdouble timeDiference;

        // Elapsed time from the initiation of the game.
        currentTime = glutGet(GLUT_ELAPSED_TIME);
        // Elapsed time from the previous frame.
        timeDiference = currentTime - previousTime;
        //Update previous time 
        previousTime = currentTime; 

        //tiros disparados pelo jogador
        for(vector<Cannonball>::iterator shot = shotList.begin(); shot != shotList.end(); ++shot) {     
            shot->updateCannonball(timeDiference);
            //Inimigo atingido por tiro
            for(int i = 0; i < enemies.size(); i++) {
                if(enemies[i]->hitByShot(*shot) && startgame) {
                    enemies.erase(enemies.begin()+i);
                
                }
            }
        }

        //tiros disparados pelos inimigos
        for(vector<Cannonball>::iterator shot_e = enemyShotList.begin(); shot_e != enemyShotList.end(); ++shot_e) {     
            shot_e->updateCannonball(timeDiference);

            //Jogador atingido por tiro
            if(car->hitByShot(*shot_e)) {
                    gameover = true;
            }
        }

        if(startgame) {
            
            if(currentTime - startshoots > 1.0/shotFreq) {

                for(int i = 0; i < enemies.size(); i++) {
                    enemies[i]->shoot(enemyShotList);
                }
                startshoots = currentTime;
            }
            
        }else startshoots = currentTime;    

        if((key_status['a'] || key_status['A']) && car->getWheelAngle() < 45) {
            car->moveWheel(0.5*car->getCarSpeed());
        }

        if((key_status['d'] || key_status['D']) && car->getWheelAngle() > -45) {
            car->moveWheel(-0.5*car->getCarSpeed());
        }

        if(key_status['w'] || key_status['W']) {
            car->setMoveFlag(true);
            startgame = true;

            car->forwardMove(-car->getCarSpeed());

            if(car->crashCheckBoard(outer_circle)) {
                car->setMoveFlag(false);
                car->forwardMove(car->getCarSpeed());
            }
            if(car->crashCheck(inner_circle)) {
                car->setMoveFlag(false);
                car->forwardMove(car->getCarSpeed());
            }
            for(int i = 0; i < enemies.size(); i++) {
                
                if(car->crashCheck(enemies[i])) {
                   car->setMoveFlag(false);
                   car->forwardMove(car->getCarSpeed());
                }
            }
        }
        else car->setMoveFlag(false);

        if(key_status['s'] || key_status['S']) {
            car->setMoveFlag(true);

            car->backwardMove(car->getCarSpeed());

            if(car->crashCheckBoard(outer_circle))
                car->backwardMove(-car->getCarSpeed());
                car->setMoveFlag(false);

            if(car->crashCheck(inner_circle))
                car->backwardMove(-car->getCarSpeed());
                car->setMoveFlag(false);
        
            for(int i = 0; i < enemies.size(); i++) {
                
                if(car->crashCheck(enemies[i])) 
                    car->backwardMove(-car->getCarSpeed());
                    car->setMoveFlag(false);
            }
        }

        for(int i = 0; i < enemies.size(); i++) {
            if(i % 2 != 0)
                enemyMoveA(enemies[i]);
                    
             else enemyMoveB(enemies[i]);
        }

        checkPoints();
        
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
            car->shoot(shotList);
    }
}

void displayTime(float x, float y) {

    char *tmpStr;
    static GLdouble currentTime;    
    static double start_time = glutGet(GLUT_ELAPSED_TIME);
   
    if(youwin || gameover) {
        sprintf(text_time, "Time : %.2lf s", currentTime/1000.0);
        glColor3f(0.0, 0.0, 1.0);         
    
    }else if(!startgame) {
        sprintf(text_time, "          GO!");
        glColor3f(1.0, 0.0, 0.0);         

        start_time = glutGet(GLUT_ELAPSED_TIME);
    
    }else {
        currentTime = glutGet(GLUT_ELAPSED_TIME) - start_time;
        sprintf(text_time, "Time : %.2lf s",currentTime/1000.0);
        glColor3f(0.0, 0.0, 1.0);       
       
    }

    //Define the position to start printing
    glRasterPos2f(x, y);
    //Print  the first Char with a certain font
    //glutBitmapLength(font,(unsigned char*)str);
    tmpStr = text_time;
    //Print each of the other Char at time
    while(*tmpStr) {
        glutBitmapCharacter(timefont, *tmpStr);
        tmpStr++;
    }
}

void display(void) {

    glClear(GL_COLOR_BUFFER_BIT);

    displayCircle(outer_circle);
    displayCircle(inner_circle);
    displayTime(0.86*window_size, 0.87*window_size);

    displayRectangle(start);
   
    for(int i = 0; i < enemies.size(); i++) {
        enemies[i]->displayCar();
    }

    car->displayCar();

    for(vector<Cannonball>::iterator shot = shotList.begin(); shot != shotList.end(); ++shot) {
        shot->displayCannonball();
    }

    for(vector<Cannonball>::iterator shot_e = enemyShotList.begin(); shot_e != enemyShotList.end(); ++shot_e) {
        shot_e->displayCannonball();
    }

    if(youwin && !gameover) {
        char *tmpStr;

        sprintf(text_time,"YOU WIN!!!");
        glColor3f(0.0, 0.5, 0.0);         

        glRasterPos2f(window_size/2.05, window_size/2.4);
        tmpStr = text_time;
        while(*tmpStr) {
            glutBitmapCharacter(statusfont, *tmpStr);
            tmpStr++;
        }
    }

    if(gameover && !youwin) {
        char *tmpStr;

        sprintf(text_time," GAME OVER");
        glColor3f(1.0, 0.0, 0.0);         

        glRasterPos2f(window_size/2.15, window_size/2.4);

        tmpStr = text_time;
        while(*tmpStr) {
            glutBitmapCharacter(statusfont, *tmpStr);
            tmpStr++;
        }
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

    glutCreateWindow ("Trabalho Curto IV - CG 2016");
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