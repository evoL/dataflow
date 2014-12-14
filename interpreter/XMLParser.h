#ifndef XML_PARSER
#define XML_PARSER

#include <vector>
#include <stdexcept>
#include <libxml/parser.h>
#include <libxml/xmlwriter.h>
#include "DataflowModel.h"
#include "LibraryLoader.h"

class XMLParserError : public std::runtime_error
{
public:
    XMLParserError(const std::string & what): std::runtime_error(what) {}
};

class XMLParser
{
public:
    ProjectModel * loadModelFromFile(std::string filePath);
    void saveModelToFile(const ProjectModel& model, const std::string filePath);
    std::string getLastError();
    
private:
    void parseXMLTree(xmlNodePtr node, ProjectModel * model);
    void parseImportsNode(xmlNodePtr node, ProjectModel * model);
    void parseEntryPointsNode(xmlNodePtr node, ProjectModel *model);
    void parseSchemeNode(xmlNodePtr node, ProjectModel * model);
    void parseImportNode(xmlNodePtr node, ProjectModel * model);
    void parseEntryPointNode(xmlNodePtr node, ProjectModel *model);
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

    void writeModelToFile(const ProjectModel &model, _xmlTextWriter *writer);
    void writeImportsToFile(const LibraryMap &libraries, xmlTextWriterPtr writer);
    void writeImportToFile(xmlTextWriterPtr writer, const std::string libraryName);
    void writeEntryPointsToFile(const std::vector<int> &entryPoints, xmlTextWriterPtr writer);
    void writeSchemaToFile(const ProjectModel &model, xmlTextWriterPtr writer);
    void writeBlocksToFile(const BlocksMap &blocks, xmlTextWriterPtr writer);
    void writeConstructorToFile(const Constructor &constructor, xmlTextWriterPtr writer);
    void writeOperationToFile(const Operation &operation, xmlTextWriterPtr writer);
    void writeOutputsTransitionsToFile(const std::vector<OutputTransition> &outputs, xmlTextWriterPtr writer);
    void writeOutputTransitionToFile(const OutputTransition &transition, xmlTextWriterPtr writer);
    void writeInputsTransitionToFile(const InputTransitionMap &inputs, xmlTextWriterPtr writer);
    void writeInputTransitionToFile(const std::pair<int, InputTransition> &transition, xmlTextWriterPtr writer);
    void writeConstructorDataToFile(const std::string& data, xmlTextWriterPtr writer);
};

#endif