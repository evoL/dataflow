#include "Data_repository.h"

bool DataRepository::is_computed(std::string data_id)
{
	return computed_results.count(data_id) && computed_results.at(data_id);
}

void * DataRepository::operator[](std::string data_id)
{
	if (computed_results.count(data_id) == 0)
		return nullptr;
	return computed_results.at(data_id).get();
}

flow_data_ptr * DataRepository::result_location(std::string data_id)
{
	return &computed_results[data_id];
}

void DataRepository::reset()
{
	computed_results.clear();
}
