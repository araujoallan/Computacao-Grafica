/*
	--Trabalho Curto II Computação Gráfica 2016/2
	--Allan Araujo Silva
	--Ciência da Computação UFES
*/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string>
#include <vector>    
#include <iostream>
#include <math.h>
#include "tinyxml2.h"

#define PI 3.14159265359

using namespace std;
using namespace tinyxml2;

class Point {
    public: float x, y;
};

class Circle {
    public: Point center;
    public: float radius;
    public: string fill, id;
};

class Rectangle {
    public: Point center;
    public: float width, height;
    public: string fill, id;
};

class Arena {
    public: string name, type, svgFile_path;
};

GLfloat GX = 0.0, GY = 0.0, window_size;
int key_status[256]; //estados das teclas - se alguma tecla está apertada ou nao

Arena *arena = new Arena();
Circle *outer_circle = new Circle();
Circle *inner_circle = new Circle();
Circle *player  = new Circle();
vector<Circle*> enemies;
Rectangle *start = new Rectangle();

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
    glOrtho(outer_circle->center.x - outer_circle->radius, outer_circle->center.x + outer_circle->radius, 
        window_size - (outer_circle->center.y + outer_circle->radius), window_size - (outer_circle->center.y - outer_circle->radius), -1.0, 1.0);
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

void displayRectangle(Rectangle *rect) {

    glBegin(GL_POLYGON);
        colorCheck(rect->fill);
        glVertex3f(rect->center.x, window_size - rect->center.y, 0.0);
        glVertex3f(rect->center.x + rect->width, window_size - (rect->center.y), 0.0);
        glVertex3f(rect->center.x + rect->width, window_size - (rect->center.y + rect->height) , 0.0);
        glVertex3f(rect->center.x, window_size - (rect->center.y + rect->height), 0.0);
    glEnd();
}

void displayCircle(float x, float y, float radius, string color) {
    
    int triangleAmount = 70;
    float twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
        colorCheck(color);
        glVertex2f(x, window_size - y);
 
        for(int i = 0; i <= triangleAmount; i++)  
            glVertex2f(x + (radius * cos(i *  twicePi / triangleAmount)), window_size - y + (radius * sin(i * twicePi / triangleAmount)));
        
    glEnd();
}

int crashCheck(Circle *circle, Circle *player) {
    
    float cathetus_b = player->center.x - circle->center.x;
    float cathetus_c = player->center.y - circle->center.y;
    float distance = sqrt(cathetus_b*cathetus_b + cathetus_c*cathetus_c);

    if(distance < (circle->radius + player->radius) ) {
        //Colisão
        return 1;
    }
    return 0;
}

int crashCheckBoard(Circle *circle, Circle *player) {
    
    float cathetus_b = player->center.x - circle->center.x;
    float cathetus_c = player->center.y - circle->center.y;
    float distance = sqrt(cathetus_b*cathetus_b + cathetus_c*cathetus_c);

    if(distance >= (circle->radius - player->radius) ) {
        //Colisão
        return 1;
    }
    return 0;
}

void idle() {

    if(key_status['a']) {
        player->center.x -= 2.0;

        if(crashCheckBoard(outer_circle, player))
            player->center.x += 2.0;

        if(crashCheck(inner_circle, player))
            player->center.x += 2.0;

        for(int i = 0; i < enemies.size(); i++) {
         
            if(crashCheck(enemies[i], player)) 
                player->center.x += 2.0;
        }
    }

    if(key_status['d']) {
        player->center.x += 2.0;
    
        if(crashCheckBoard(outer_circle, player))
            player->center.x -= 2.0;

         if(crashCheck(inner_circle, player))
            player->center.x -= 2.0;

         for(int i = 0; i < enemies.size(); i++) {
            
            if(crashCheck(enemies[i], player)) 
                player->center.x -= 2.0;
        }
    }

    if(key_status['w']) {
        player->center.y -= 2.0;
   
        if(crashCheckBoard(outer_circle, player))
            player->center.y += 2.0;

         if(crashCheck(inner_circle, player))
            player->center.y += 2.0;

        for(int i = 0; i < enemies.size(); i++) {
            
            if(crashCheck(enemies[i], player)) 
                player->center.y += 2.0;
        }
    }

    if(key_status['s'] && !crashCheck(inner_circle, player)) {
        player->center.y += 2.0;

        if(crashCheckBoard(outer_circle, player))
            player->center.y -= 2.0;

        if(crashCheck(inner_circle, player))
            player->center.y -= 2.0;
    
        for(int i = 0; i < enemies.size(); i++) {
            
            if(crashCheck(enemies[i], player)) 
                player->center.y -= 2.0;
        }
    }

    glutPostRedisplay();
}

void display(void) {

    glClear(GL_COLOR_BUFFER_BIT);

    displayCircle(outer_circle->center.x, outer_circle->center.y, outer_circle->radius, outer_circle->fill);
    displayCircle(inner_circle->center.x, inner_circle->center.y, inner_circle->radius, inner_circle->fill);
   
    for(int i = 0; i < enemies.size(); i++) {
        displayCircle(enemies[i]->center.x, enemies[i]->center.y, enemies[i]->radius, enemies[i]->fill);
    }

    displayRectangle(start);
    displayCircle(player->center.x, player->center.y, player->radius, player->fill);

    glutSwapBuffers();
}

void loadArenaScenario(const char *filePath) {
    /*
        Formato de entrada do programa (especificação):  /PastaDoArquivoSVG/
        concatenar path com o nome do arquivo.
        "PastaDoArquivoSVG" + "arena.svg"
    */
    string svgFileName = arena->name + '.' + arena->type;
    string pathName = filePath + svgFileName; 
    
    XMLDocument svgDoc;
    XMLError eResult  = svgDoc.LoadFile(pathName.data());
    XMLElement *root = svgDoc.FirstChildElement();

    //percorrer filhos diretos de <svg>
    for(XMLElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) { 
        string elem_name = elem->Attribute("fill");

        //PISTA CIRCULO MAIOR
        if (elem_name == "blue") {
            outer_circle->center.x = atof(elem->Attribute("cx"));
            outer_circle->center.y = atof(elem->Attribute("cy"));
            outer_circle->radius = atof(elem->Attribute("r"));
            outer_circle->fill = elem->Attribute("fill");
            outer_circle->id = elem->Attribute("id");
 
            window_size = 2*(outer_circle->radius);
        }
        //PISTA CIRCULO MENOR
        else if (elem_name == "white") {
            inner_circle->center.x = atof(elem->Attribute("cx"));
            inner_circle->center.y = atof(elem->Attribute("cy"));
            inner_circle->radius = atof(elem->Attribute("r"));
            inner_circle->fill = elem->Attribute("fill");
            inner_circle->id = elem->Attribute("id");
        } 
        //RETANGULO LARGADA_CHEGADA
        else if (elem_name == "black") {
            start->center.x = atof(elem->Attribute("x"));
            start->center.y = atof(elem->Attribute("y"));
            start->width = atof(elem->Attribute("width"));
            start->height = atof(elem->Attribute("height"));
            start->fill = elem->Attribute("fill");
            start->id = elem->Attribute("id");
        }
        //INIMIGOS
        else if (elem_name == "red") {
            Circle *circle = new Circle();

            circle->center.x = atof(elem->Attribute("cx"));
            circle->center.y = atof(elem->Attribute("cy"));
            circle->radius = atof(elem->Attribute("r"));
            circle->fill = elem->Attribute("fill");
            circle->id = elem->Attribute("id");

            enemies.push_back(circle);
        }    
        //JOGADOR
        else if (elem_name == "green") {
            player->center.x = atof(elem->Attribute("cx"));
            player->center.y = atof(elem->Attribute("cy"));
            player->radius = atof(elem->Attribute("r"));
            player->fill = elem->Attribute("fill");
            player->id = elem->Attribute("id");
        }
    }  
}

void readFile(const char *filePath) {
    /*
        Formato de entrada do programa (especificação):  /PastaDoArquivoXML/
        concatenar path com o nome do arquivo.
        "PastaDoArquivoXML" + "config.xml"
    */
    string xmlFile = "config.xml";
    string pathName = filePath + xmlFile;

    //Leitura do arquivo de acordo com tutorial: http://www.dinomage.com/2012/01/tutorial-using-tinyxml-part-1/
    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile(pathName.data());
    XMLElement *root_xml = xmlDoc.FirstChildElement();

    XMLElement *elem = root_xml->FirstChildElement(); 
    
    if(elem != NULL) {
        string elem_name = elem->Value();
        
        if(elem_name == "arquivoDaArena") {
            arena->name = elem->Attribute("nome");
            arena->type = elem->Attribute("tipo");
            arena->svgFile_path = elem->Attribute("caminho");
        }
            
        loadArenaScenario(arena->svgFile_path.c_str());
    }
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    readFile(argv[1]);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (window_size, window_size);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Trabalho Curto 2 - CG 2016");
    init ();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp); 
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}