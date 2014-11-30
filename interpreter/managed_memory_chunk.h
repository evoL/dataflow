#ifndef MANAGED_MEMORY_CHUNK_H
#define MANAGED_MEMORY_CHUNK_H
#include <cstddef>

class ManagedMemoryChunk
{
    void * managed_data{};
    std::size_t managed_data_size{};
    void release_managed_data();
public:
    ManagedMemoryChunk() {};
    explicit ManagedMemoryChunk(std::size_t chunk_size);
    ~ManagedMemoryChunk();

    ManagedMemoryChunk(const ManagedMemoryChunk &) = delete;
    ManagedMemoryChunk & operator=(const ManagedMemoryChunk &) = delete;

    ManagedMemoryChunk(ManagedMemoryChunk && other);
    ManagedMemoryChunk & operator=(ManagedMemoryChunk && other);

    operator void * ();
};

#endif
