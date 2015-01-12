#include "Datastore.h"

bool Datastore::isComputed(int dataId)
{
    return computedResults.count(dataId) && computedResults.at(dataId);
}

void * Datastore::operator[](int dataId)
{
    if (computedResults.count(dataId) == 0)
        return nullptr;

    return computedResults.at(dataId);
}

void * Datastore::createEntry(int chunkId, std::size_t sizeInBytes, ManagedMemoryChunk::destructor_t destructor)
{
    return computedResults[chunkId] = ManagedMemoryChunk{ sizeInBytes, destructor };
}

void Datastore::reset()
{
    computedResults.clear();
}

void Datastore::removeEntry(int dataId)
{
    computedResults.erase(dataId);
}

