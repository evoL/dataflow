#include "Datastore.h"

bool Datastore::is_computed(std::string data_id)
{
    return computed_results.count(data_id) && computed_results.at(data_id);
}

void * Datastore::operator[](std::string data_id)
{
    if (computed_results.count(data_id) == 0)
        return nullptr;
    return computed_results.at(data_id).get();
}

stored_ptr * Datastore::result_location(std::string data_id)
{
    return &computed_results[data_id];
}

void Datastore::reset()
{
    computed_results.clear();
}
