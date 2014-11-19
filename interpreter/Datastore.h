#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <unordered_map>
#include "stored_ptr.h"

class Datastore 
{
public:
    bool is_computed(std::string data_id);
    void * operator [](std::string data_id);
    stored_ptr * result_location(std::string data_id);

    void reset();
private:
    std::unordered_map<std::string, stored_ptr> computed_results;
};

#endif
