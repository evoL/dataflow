#include "Datastore.h"

bool Datastore::is_computed(std::string data_id)
{
    return computed_results.count(data_id) && computed_results.at(data_id);
}

void * Datastore::operator[](std::string data_id)
{
    if (computed_results.count(data_id) == 0)
        return nullptr;

    return computed_results.at(data_id);
}

void * Datastore::create_entry(std::string chunk_id, std::size_t size_in_bytes)
{
    return computed_results[chunk_id] = ManagedMemoryChunk{ size_in_bytes };
}

void Datastore::reset()
{
    computed_results.clear();
}

void Datastore::remove_entry(std::string data_id)
{
    computed_results.erase(data_id);
}

