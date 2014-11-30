#include "managed_memory_chunk.h"

ManagedMemoryChunk::ManagedMemoryChunk(std::size_t chunk_size) :
    managed_data_size{chunk_size}
{
    managed_data = operator new(chunk_size);
}

void ManagedMemoryChunk::release_managed_data()
{
    if (managed_data && managed_data_size > 0)
        operator delete(managed_data);

    managed_data = nullptr;
    managed_data_size = 0;
}

ManagedMemoryChunk::~ManagedMemoryChunk()
{
    release_managed_data();
}

ManagedMemoryChunk::ManagedMemoryChunk(ManagedMemoryChunk && other) :
    managed_data{other.managed_data},
    managed_data_size{other.managed_data_size}
{
    other.managed_data = nullptr;
    other.managed_data_size = 0;
}

ManagedMemoryChunk & ManagedMemoryChunk::operator=(ManagedMemoryChunk && other)
{
    release_managed_data();
    managed_data = other.managed_data;
    other.managed_data = nullptr;
    managed_data_size = other.managed_data_size;
    other.managed_data_size = 0;
    return *this;
}

ManagedMemoryChunk::operator void * ()
{
    return managed_data;
}
