#include "ManagedMemoryChunk.h"

ManagedMemoryChunk::ManagedMemoryChunk(std::size_t chunkSize, destructor_t destructor) :
    managedDataSize{chunkSize},
    destructor{destructor}
{
    managedData = operator new(chunkSize);
}

void ManagedMemoryChunk::releaseManagedData()
{
    if (managedData && managedDataSize > 0) {
        destructor(managedData);
        operator delete(managedData);
    }

    managedData = nullptr;
    managedDataSize = 0;
}

ManagedMemoryChunk::~ManagedMemoryChunk()
{
    releaseManagedData();
}

ManagedMemoryChunk::ManagedMemoryChunk(ManagedMemoryChunk && other) :
    managedData{other.managedData},
    managedDataSize{other.managedDataSize},
    destructor{other.destructor}
{
    other.managedData = nullptr;
    other.managedDataSize = 0;
}

ManagedMemoryChunk & ManagedMemoryChunk::operator=(ManagedMemoryChunk && other)
{
    releaseManagedData();
    managedData = other.managedData;
    other.managedData = nullptr;
    managedDataSize = other.managedDataSize;
    other.managedDataSize = 0;
    destructor = other.destructor;
    return *this;
}

ManagedMemoryChunk::operator void * ()
{
    return managedData;
}
