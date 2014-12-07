#include "XMLParser.h"

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

void XMLParser::parseEntryPointsNode(xmlNodePtr node, ProjectModel *model) {
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
		model->libraries[libName] = libraryLoader.load(libName);
	}
	catch (LibraryLoadError& error)
	{
		throw XMLParserError("Error while importing library: " + error.what)
	}
}

void XMLParser::parseEntryPointNode(xmlNodePtr node, ProjectModel *model)
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
        if (it->second->blockType() == BlockTypeOperation) {
            Operation & op = dynamic_cast<Operation &>(*it->second);

            for (auto it2 = op.inputs.begin(); it2 != op.inputs.end(); ++it2) {
                it2->second.outputBlock = outputIdToBlock[it2->second.outputId];
            }
        }
    }
}