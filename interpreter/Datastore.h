#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <unordered_map>
#include "managed_memory_chunk.h"

class Datastore
{
public:
    bool is_computed(std::string data_id);
    void * operator [](std::string data_id);

    void * create_entry(std::string chunk_id, std::size_t size_in_bytes);
    void remove_entry(std::string data_id);

    void reset();
private:
    std::unordered_map<std::string, ManagedMemoryChunk> computed_results;
};

#endif
