#include "reader.h"
#include <string>
#include <vector>
#include <stdio.h>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

Reader::Reader() {

}

void Reader::loadArenaScenario(const char *filePath, Arena *arena, Circle *out_circ, Circle *in_circ, Car *car,
                                Rectangle *start, vector<Circle*> &enemies) {
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
            Circle *circle = new Circle();

            circle->center.x = atof(elem->Attribute("cx"));
            circle->center.y = atof(elem->Attribute("cy"));
            circle->setRadius(atof(elem->Attribute("r")));
            circle->setFill(elem->Attribute("fill"));
            circle->setId(elem->Attribute("id"));

            enemies.push_back(circle);
        }  
        //CARRO JOGADOR
        else if (elem_name == "green") {
            car->center.x = atof(elem->Attribute("cx"));
            car->center.y = atof(elem->Attribute("cy"));
            car->setRadius(atof(elem->Attribute("r")));
            car->setId(elem->Attribute("id"));
        }
    } 
}

void Reader::readFile(const char *filePath, Arena *arena, Circle *out_circ, Circle *in_circ, Car *car,
                             Rectangle *start, vector<Circle*> &enemies) {
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
        }
        else if(elem_name == "carro") {
            car->setCarSpeed(atof(elem->Attribute("velCarro")));
            car->setShotSpeed(atof(elem->Attribute("velTiro")));
        }
        loadArenaScenario(arena->svgFile_path.c_str(), arena, out_circ, in_circ, car, start, enemies);
    }
}