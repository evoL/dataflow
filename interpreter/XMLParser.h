#ifndef XML_PARSER
#define XML_PARSER

#include "DataflowModel.h"
#include <libxml/parser.h>

class XMLParser
{
public:
    ProjectModel * loadModelFromFile(std::string filePath);
    std::string getLastError();
    
private:
    std::string lastError;

    void parseXMLTree(xmlNodePtr node, ProjectModel * model);
    void parseImportsNode(xmlNodePtr node, ProjectModel * model);
    void parseSchemeNode(xmlNodePtr node, ProjectModel * model);
    void parseImportNode(xmlNodePtr node, ProjectModel * model);
    void parseContructorNode(xmlNodePtr node, ProjectModel * model);
    void parseOperationNode(xmlNodePtr node, ProjectModel * model);
    void parseOutputs(xmlNodePtr node, Block & block);
    void parseInputs(xmlNodePtr node, Operation & operation);
    void parseConstructorData(xmlNodePtr node, Constructor & constructor);
    void linkInputsWithOutputBlocks(ProjectModel * model);

    //methods for getting data from xml properties
    int getIntFromProperty(xmlNodePtr node, const char * prop)
    {
        return atoi((const char *)xmlGetProp(node, (const xmlChar *)prop));
    }

    float getFloatFromProperty(xmlNodePtr node, const char * prop)
    {
        return (float)atof((char *)xmlGetProp(node, (const xmlChar *)prop));
    }

    std::string getStringFromProperty(xmlNodePtr node, const char * prop)
    {
        return (char *)xmlGetProp(node, (const xmlChar *)prop);
    }
};

#endif