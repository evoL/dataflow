#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <unordered_map>
#include "ManagedMemoryChunk.h"
#include "interpreter_export.h"

class INTERPRETER_NO_EXPORT Datastore
{
public:
    bool isComputed(int dataId);
    void * operator [](int dataId);

    void * createEntry(int chunkId, std::size_t sizeInBytes, ManagedMemoryChunk::destructor_t destructor = [](void *) {});
    void removeEntry(int dataId);

    void reset();
private:
    std::unordered_map<int, ManagedMemoryChunk> computedResults;
};

#endif
