#include "ManagedMemoryChunk.h"

ManagedMemoryChunk::ManagedMemoryChunk(std::size_t chunkSize) :
    managedDataSize{chunkSize}
{
    managedData = operator new(chunkSize);
}

void ManagedMemoryChunk::releaseManagedData()
{
    if (managedData && managedDataSize > 0)
        operator delete(managedData);

    managedData = nullptr;
    managedDataSize = 0;
}

ManagedMemoryChunk::~ManagedMemoryChunk()
{
    releaseManagedData();
}

ManagedMemoryChunk::ManagedMemoryChunk(ManagedMemoryChunk && other) :
    managedData{other.managedData},
    managedDataSize{other.managedDataSize}
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
    return *this;
}

ManagedMemoryChunk::operator void * ()
{
    return managedData;
}
