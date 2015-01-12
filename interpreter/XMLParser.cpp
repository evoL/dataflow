#include "XMLParser.h"
#include "DataflowModel.h"
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>

// PUBLIC

ProjectModel * XMLParser::loadModelFromFile(std::string filePath)
{
    xmlDocPtr document = xmlParseFile(filePath.c_str());

    if (document == NULL)
        throw XMLParserError("Cannot parse document");

    xmlNodePtr node = xmlDocGetRootElement(document);

    if (node == NULL) {
        xmlFreeDoc(document);
        throw XMLParserError("Document is empty");
    }

    ProjectModel * model = new ProjectModel();
    parseXMLTree(node, model);
    return model;
}

void XMLParser::saveModelToFile(const ProjectModel & model, const std::string filePath)
{
    int error = 0;
    xmlTextWriterPtr writer = xmlNewTextWriterFilename(filePath.c_str(), 0);
    xmlTextWriterSetIndent(writer, 4);

    if (writer == NULL) {
        throw XMLParserError("Error creating the xml writer");
    }

    error = xmlTextWriterStartDocument(writer, NULL, NULL, NULL);

    if (error < 0) {
        throw XMLParserError("Cannot start to write to file");
    }

    writeModelToFile(model, writer);

    error = xmlTextWriterEndDocument(writer);

    if (error < 0) {
        throw XMLParserError("Cannot save the file");
    }
    
    xmlFreeTextWriter(writer);
}

// PRIVATE

//// Loading XML model from file

void XMLParser::parseXMLTree(xmlNodePtr node, ProjectModel * model)
{
    model->name = getStringFromProperty(node, "projectname");
    node = node->xmlChildrenNode;

    while (node != NULL) {
        if (xmlStrcmp(node->name, (const xmlChar *)"imports") == 0) {
            parseImportsNode(node, model);
        } else if (xmlStrcmp(node->name, (const xmlChar *)"entry-points") == 0) {
            parseEntryPointsNode(node, model);
        } else if (xmlStrcmp(node->name, (const xmlChar *)"schema") == 0) {
            parseSchemeNode(node, model);
        }

        node = node->next;
    }

    linkInputsWithOutputBlocks(model);
}

void XMLParser::parseImportsNode(xmlNodePtr node, ProjectModel * model)
{
    xmlNodePtr childNode = node->xmlChildrenNode;

    while (childNode != NULL) {
        if (xmlStrcmp(childNode->name, (const xmlChar *)"import") == 0) {
            parseImportNode(childNode, model);
        }

        childNode = childNode->next;
    }
}

void XMLParser::parseEntryPointsNode(xmlNodePtr node, ProjectModel * model)
{
    xmlNodePtr childNode = node->xmlChildrenNode;

    while (childNode != NULL) {
        if (xmlStrcmp(childNode->name, (const xmlChar *)"entry-point") == 0) {
            parseEntryPointNode(childNode, model);
        }

        childNode = childNode->next;
    }
}


void XMLParser::parseSchemeNode(xmlNodePtr node, ProjectModel * model)
{
    xmlNodePtr childNode = node->xmlChildrenNode;

    while (childNode != NULL) {
        if (xmlStrcmp(childNode->name, (const xmlChar *)"constructor") == 0) {
            parseContructorNode(childNode, model);
        } else if (xmlStrcmp(childNode->name, (const xmlChar *)"operation") == 0) {
            parseOperationNode(childNode, model);
        }

        childNode = childNode->next;
    }
}

void XMLParser::parseImportNode(xmlNodePtr node, ProjectModel * model)
{
    std::string libName = getStringFromProperty(node, "module");

    if (model->libraries.find(libName) != model->libraries.end())
        throw XMLParserError("Duplicated import library");

    try {
        model->libraries[libName] = LibraryLoader::load(libName);
    } catch (LibraryLoadError & error) {
        throw XMLParserError(std::string("Error while importing library: ").append(error.what()));
    }
}

void XMLParser::parseEntryPointNode(xmlNodePtr node, ProjectModel * model)
{
    model->entryPoints.push_back(getIntFromProperty(node, "id"));
}


void XMLParser::parseContructorNode(xmlNodePtr node, ProjectModel * model)
{
    int id = getIntFromProperty(node, "id");
    std::string module = getStringFromProperty(node, "module");
    Position position = { getFloatFromProperty(node, "pos_x"), getFloatFromProperty(node, "pos_y") };
    std::string type = getStringFromProperty(node, "type");

    model->blocks[id] = std::shared_ptr<Block>(new Constructor(id, module, type, position));
    parseOutputs(node, *model->blocks[id]);
    parseConstructorData(node, dynamic_cast<Constructor &>(*model->blocks[id]));
}

void XMLParser::parseOperationNode(xmlNodePtr node, ProjectModel * model)
{
    int id = getIntFromProperty(node, "id");
    std::string module = getStringFromProperty(node, "module");
    std::string name = getStringFromProperty(node, "name");
    Position position = { getFloatFromProperty(node, "pos_x"), getFloatFromProperty(node, "pos_y") };

    model->blocks[id] = std::shared_ptr<Block>(new Operation(id, module, name, position));
    parseOutputs(node, *model->blocks[id]);
    parseInputs(node, dynamic_cast<Operation &>(*model->blocks[id]));
}

void XMLParser::parseOutputs(xmlNodePtr node, Block & block)
{
    xmlNodePtr childNode = node->xmlChildrenNode;

    while (childNode != NULL) {
        if (xmlStrcmp(childNode->name, (const xmlChar *)"output") == 0) {
            block.outputs.push_back(OutputTransition { getIntFromProperty(childNode, "id") });
        }

        childNode = childNode->next;
    }
}

void XMLParser::parseInputs(xmlNodePtr node, Operation & operation)
{
    xmlNodePtr childNode = node->xmlChildrenNode;

    while (childNode != NULL) {
        if (xmlStrcmp(childNode->name, (const xmlChar *)"input") == 0) {
            int index = getIntFromProperty(childNode, "index");
            int outputId = getIntFromProperty(childNode, "output_id");

            if (operation.inputs.find(index) != operation.inputs.end())
                throw ("Duplicated input index for a block");

            operation.inputs[index] = InputTransition { outputId };
        }

        childNode = childNode->next;
    }
}

void XMLParser::parseConstructorData(xmlNodePtr node, Constructor & constructor)
{
    xmlNodePtr childNode = node->xmlChildrenNode;

    while (childNode != NULL) {
        if (xmlStrcmp(childNode->name, (const xmlChar *)"data") == 0) {
            constructor.data = (const char *)xmlNodeGetContent(childNode);
        }

        childNode = childNode->next;
    }
}

void XMLParser::linkInputsWithOutputBlocks(ProjectModel * model)
{
    std::unordered_map<int, std::shared_ptr<Block>> outputIdToBlock;

    for (auto it = model->blocks.begin(); it != model->blocks.end(); ++it) {
        for (auto it2 = it->second->outputs.begin(); it2 != it->second->outputs.end(); ++it2) {
            outputIdToBlock[it2->id] = it->second;
        }
    }

    for (auto it = model->blocks.begin(); it != model->blocks.end(); ++it) {
        if (it->second->blockType() == BlockType::Operation) {
            Operation & op = dynamic_cast<Operation &>(*it->second);

            for (auto it2 = op.inputs.begin(); it2 != op.inputs.end(); ++it2) {
                it2->second.outputBlock = outputIdToBlock[it2->second.outputId];
            }
        }
    }
}

//// Writing XML model to file

void XMLParser::writeModelToFile(ProjectModel const & model, xmlTextWriterPtr writer)
{
    xmlTextWriterStartElement(writer, (xmlChar *)"dataflow");
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"projectname", (xmlChar *)model.name.c_str());

    writeImportsToFile(model.libraries, writer);
    writeEntryPointsToFile(model.entryPoints, writer);
    writeSchemaToFile(model, writer);

    xmlTextWriterEndElement(writer);
}

void XMLParser::writeImportsToFile(const LibraryMap & libraries, xmlTextWriterPtr writer)
{
    xmlTextWriterStartElement(writer, (xmlChar *)"imports");

    for (auto & library : libraries) {
        writeImportToFile(writer, library.first);
    }

    xmlTextWriterEndElement(writer);
}

void XMLParser::writeImportToFile(xmlTextWriterPtr writer, const std::string libraryName)
{
    xmlTextWriterStartElement(writer, (xmlChar *)"import");
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"module", (xmlChar *)libraryName.c_str());
    xmlTextWriterEndElement(writer);
}

void XMLParser::writeEntryPointsToFile(const std::vector<int> & entryPoints, xmlTextWriterPtr writer)
{
    xmlTextWriterStartElement(writer, (xmlChar *)"entry-points");

    for (auto entryPoint : entryPoints) {
        xmlTextWriterStartElement(writer, (xmlChar *)"entry-point");
        xmlTextWriterWriteAttribute(writer, (xmlChar *)"id", (xmlChar *)std::to_string(entryPoint).c_str());
        xmlTextWriterEndElement(writer);
    }

    xmlTextWriterEndElement(writer);
}

void XMLParser::writeSchemaToFile(const ProjectModel & model, xmlTextWriterPtr writer)
{
    xmlTextWriterStartElement(writer, (xmlChar *)"schema");
    writeBlocksToFile(model.blocks, writer);
    xmlTextWriterEndElement(writer);
}

void XMLParser::writeBlocksToFile(const BlocksMap & blocks, xmlTextWriterPtr writer)
{
    for (auto & block : blocks) {
        switch (block.second->blockType()) {
        case BlockType::Constructor: {
            writeConstructorToFile(*dynamic_cast<Constructor *>(block.second.get()), writer);
            break;
        }

        case BlockType::Operation: {
            writeOperationToFile(*dynamic_cast<Operation *>(block.second.get()), writer);
            break;
        }

        }
    }
}

void XMLParser::writeConstructorToFile(const Constructor & constructor, xmlTextWriterPtr writer)
{
    xmlTextWriterStartElement(writer, (xmlChar *)"constructor");
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"id", (xmlChar *)std::to_string(constructor.id).c_str());
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"module", (xmlChar *)constructor.module.c_str());
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"type", (xmlChar *)constructor.type.c_str());
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"pos_x", (xmlChar *)std::to_string(constructor.position.x).c_str());
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"pos_y", (xmlChar *)std::to_string(constructor.position.y).c_str());
    writeOutputsTransitionsToFile(constructor.outputs, writer);
    writeConstructorDataToFile(constructor.data, writer);
    xmlTextWriterEndElement(writer);
}

void XMLParser::writeOperationToFile(const Operation & operation, xmlTextWriterPtr writer)
{
    xmlTextWriterStartElement(writer, (xmlChar *)"operation");
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"id", (xmlChar *)std::to_string(operation.id).c_str());
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"module", (xmlChar *)operation.module.c_str());
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"name", (xmlChar *)operation.name.c_str());
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"pos_x", (xmlChar *)std::to_string(operation.position.x).c_str());
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"pos_y", (xmlChar *)std::to_string(operation.position.y).c_str());
    writeInputsTransitionToFile(operation.inputs, writer);
    writeOutputsTransitionsToFile(operation.outputs, writer);
    xmlTextWriterEndElement(writer);
}

void XMLParser::writeOutputsTransitionsToFile(const std::vector<OutputTransition> & outputs, xmlTextWriterPtr writer)
{
    for (auto & transition : outputs) {
        writeOutputTransitionToFile(transition, writer);
    }
}

void XMLParser::writeOutputTransitionToFile(const OutputTransition & transition, xmlTextWriterPtr writer)
{
    xmlTextWriterStartElement(writer, (xmlChar *)"output");
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"id", (xmlChar *)std::to_string(transition.id).c_str());
    xmlTextWriterEndElement(writer);
}

void XMLParser::writeInputsTransitionToFile(const InputTransitionMap & inputs, xmlTextWriterPtr writer)
{
    for (auto & transition : inputs) {
        writeInputTransitionToFile(transition, writer);
    }
}

void XMLParser::writeInputTransitionToFile(const std::pair<int, InputTransition> & transition, xmlTextWriterPtr writer)
{
    xmlTextWriterStartElement(writer, (xmlChar *)"input");
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"index", (xmlChar *)std::to_string(transition.first).c_str());
    xmlTextWriterWriteAttribute(writer, (xmlChar *)"output_id", (xmlChar *)std::to_string(transition.second.outputId).c_str());
    xmlTextWriterEndElement(writer);
}

void XMLParser::writeConstructorDataToFile(const std::string & data, xmlTextWriterPtr writer)
{
    xmlTextWriterWriteElement(writer, (xmlChar *)"data", (xmlChar *)data.c_str());
}
