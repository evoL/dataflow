#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <unordered_map>
#include "ManagedMemoryChunk.h"

class Datastore
{
public:
    bool isComputed(std::string dataId);
    void * operator [](std::string dataId);

    void * createEntry(std::string chunkId, std::size_t sizeInBytes);
    void removeEntry(std::string dataId);

    void reset();
private:
    std::unordered_map<std::string, ManagedMemoryChunk> computedResults;
};

#endif
