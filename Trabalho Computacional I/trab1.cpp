/*
	--Trabalho Curto I Computação Gráfica 2016/2
	--Allan Araujo Silva
	--Ciência da Computação UFES
*/
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Point {
    public: float x, y;
};

class Square {
    public: int size;
    public: float colorR, colorG, colorB;
};

GLint width, height;
GLfloat GX, GY, background[3];
GLboolean left_click = false, right_click = false, insertion = false;
const char* title;
Point point;
Square *sq = new Square();

void init(void) {

    glClearColor(background[0], background[1], background[2], 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
}

void display(void) {

    glClear(GL_COLOR_BUFFER_BIT);

    if(insertion) {
        glColor3f(sq->colorR, sq->colorG, sq->colorB);
        glBegin(GL_POLYGON);
            glVertex3f(GX - (sq->size)/2, height - (GY - (sq->size)/2), 0.0);
            glVertex3f(GX + (sq->size)/2, height - (GY - (sq->size)/2), 0.0);
            glVertex3f(GX + (sq->size)/2, height - (GY + (sq->size)/2), 0.0);
            glVertex3f(GX - (sq->size)/2, height - (GY + (sq->size)/2), 0.0);
        glEnd();
    }
    //Não esperar!
    glFlush();
}

void  mouse(int button, int state, int x, int y) {
    //Inserir Quadrado
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && left_click == false && insertion == false) {
        insertion = true;
        left_click = true;
        GX = x;
        GY = y;

        glutPostRedisplay();
    }
    //Arrastar Quadrado
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (x >= (GX - sq->size/2)) &&
                 (x <= (GX + sq->size/2)) && (y >= (GY - sq->size/2)) && (y <= (GY + sq->size/2))) {
        left_click = true;
        point.x = x;
        point.y = y;
    }
    else left_click = false;

    //Deletar Quadrado
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && right_click == false && (x >= (GX - sq->size/2)) &&
                 (x <= (GX + sq->size/2)) && (y >= (GY - sq->size/2)) && (y <= (GY + sq->size/2))) {
        right_click = true;
        insertion = false;

        glutPostRedisplay();
    }
    else right_click = false;
}

void move(int x, int y) {

    if(left_click) {
        GX = GX + (x - point.x);
        GY = GY + (y - point.y);

        point.x = x;
        point.y = y;
    }

    if(right_click) {
        insertion = false;
        display();
    }

    glutPostRedisplay();
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
    XMLElement *root = xmlDoc.FirstChildElement();

    //percorrer filhos diretos de <aplicação>
        for(XMLElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
            string elem_name = elem->Value();
            if(elem_name == "janela") {
                //percorre filhos diretos de <janela>
                for(XMLElement *e = elem->FirstChildElement(); e!= NULL; e = e->NextSiblingElement()) {
                    string e_name = e->Value();

                    if(e_name == "largura") {
                        width = atoi(e->GetText());
                    }
                    else if(e_name == "altura") {
                        height = atoi(e->GetText());
                    }
                    else if(e_name == "fundo") {
                        background[0] = atof(e->Attribute("corR"));
                        background[1] = atof(e->Attribute("corG"));
                        background[2] = atof(e->Attribute("corB"));
                    }

                    else if(e_name == "titulo") {
                        title = e->GetText();
                    }
                }
            }
            //percorre filhos diretos de <janela>
            else if(elem_name == "quadrado") {
                    sq->size = atoi(elem->Attribute("tamanho"));
                    sq->colorR = atof(elem->Attribute("corR"));
                    sq->colorG = atof(elem->Attribute("corG"));
                    sq->colorB = atof(elem->Attribute("corB"));
            }
        }
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    readFile(argv[1]);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (title);
    init ();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(move);
    glutMainLoop();

    return 0;
}
