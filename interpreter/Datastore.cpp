#include "Datastore.h"

bool Datastore::isComputed(std::string dataId)
{
    return computedResults.count(dataId) && computedResults.at(dataId);
}

void * Datastore::operator[](std::string dataId)
{
    if (computedResults.count(dataId) == 0)
        return nullptr;

    return computedResults.at(dataId);
}

void * Datastore::createEntry(std::string chunkId, std::size_t sizeInBytes)
{
    return computedResults[chunkId] = ManagedMemoryChunk{ sizeInBytes };
}

void Datastore::reset()
{
    computedResults.clear();
}

void Datastore::removeEntry(std::string dataId)
{
    computedResults.erase(dataId);
}

