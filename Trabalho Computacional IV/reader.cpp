#include "reader.h"
#include <string>
#include <vector>
#include <stdio.h>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

extern float shotFreq;

Reader::Reader() {

}

void Reader::loadArenaScenario(const char *filePath, Arena *arena, Circle *out_circ, Circle *in_circ, Car *car,
                                Rectangle *start, vector<Car*> &enemies) {
 /*   
         Formato de entrada do programa (especificação):  /PastaDoArquivoSVG/
        concatenar path com o nome do arquivo.
        "PastaDoArquivoSVG" + "arena.svg"
    */ 
    
    string svgFileName = arena->getName() + '.' + arena->getType();

    string pathName = filePath + svgFileName; 
    
    XMLDocument svgDoc;
    XMLError eResult  = svgDoc.LoadFile(pathName.data());
    XMLElement *root = svgDoc.FirstChildElement();
    
    //printf("passei aqui.\n");
    //percorrer filhos diretos de <svg>
    for(XMLElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) { 
        string elem_name = elem->Attribute("fill");

        //PISTA CIRCULO MAIOR
        if (elem_name == "blue") {
            out_circ->center.x = atof(elem->Attribute("cx"));
            out_circ->center.y = atof(elem->Attribute("cy"));
            out_circ->setRadius(atof(elem->Attribute("r")));
            out_circ->setFill(elem->Attribute("fill"));
            out_circ->setId(elem->Attribute("id"));
 
        }

        //PISTA CIRCULO MENOR
        else if (elem_name == "white") {
            in_circ->center.x = atof(elem->Attribute("cx"));
            in_circ->center.y = atof(elem->Attribute("cy"));
            in_circ->setRadius(atof(elem->Attribute("r")));
            in_circ->setFill(elem->Attribute("fill"));
            in_circ->setId(elem->Attribute("id"));

        } 
        //RETANGULO LARGADA_CHEGADA
        else if (elem_name == "black") {
            start->center.x = atof(elem->Attribute("x"));
            start->center.y = atof(elem->Attribute("y"));
            start->setWidth(atof(elem->Attribute("width")));
            start->setHeight(atof(elem->Attribute("height")));
            start->setFill(elem->Attribute("fill"));
            start->setId(elem->Attribute("id"));
        }
        //INIMIGOS
        else if (elem_name == "red") {
            Car *enemy = new Car();

            enemy->center.x = atof(elem->Attribute("cx"));
            enemy->center.y = atof(elem->Attribute("cy"));
            enemy->setRadius(atof(elem->Attribute("r")));
            enemy->setFill(elem->Attribute("fill"));
            enemy->setId(elem->Attribute("id"));

            //printf("%.2f\n", enemy->center.x);
            //printf("%.2f\n", enemy->center.y);
            //printf("\n");

            enemies.push_back(enemy);
        }  
        //CARRO JOGADOR
        else if (elem_name == "green") {
            car->center.x = atof(elem->Attribute("cx"));
            car->center.y = atof(elem->Attribute("cy"));
            car->setRadius(atof(elem->Attribute("r")));
            car->setId(elem->Attribute("id"));
            car->setFill(elem->Attribute("fill"));
        }
    } 
}

void Reader::readFile(const char *filePath, Arena *arena, Circle *out_circ, Circle *in_circ, Car *car,
                             Rectangle *start, vector<Car*> &enemies) {
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

    for(XMLElement *elem = root_xml->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
        string elem_name = elem->Value();

        if(elem_name == "arquivoDaArena") {
            arena->setName(elem->Attribute("nome"));
            arena->setType(elem->Attribute("tipo"));
            arena->setSVGPath(elem->Attribute("caminho"));

            loadArenaScenario(arena->svgFile_path.c_str(), arena, out_circ, in_circ, car, start, enemies);

        }
        else if(elem_name == "carro") {
            car->setCarSpeed(atof(elem->Attribute("velCarro")));
            car->setShotSpeed(atof(elem->Attribute("velTiro")));
        }
        else if(elem_name == "carroInimigo") {
            for(int i = 0; i < enemies.size(); i++) {
                enemies[i]->setShotFrequence(atof(elem->Attribute("freqTiro"))); 
                enemies[i]->setEnemySpeed(atof(elem->Attribute("velCarro")));
                enemies[i]->setShotSpeed(atof(elem->Attribute("velTiro")));

               // printf("E1 %.2f\n", enemies[i]->getShotFrequence());
                //printf("E2 %.2f\n", enemies[i]->getCarSpeed());
                //printf("E3 %.2f\n", enemies[i]->getShotSpeed());
                //printf("\n");
                shotFreq = enemies[i]->getShotFrequence();

            }   
        }
    }
}