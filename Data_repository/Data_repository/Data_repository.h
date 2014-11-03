#pragma once
#include <string>
#include <unordered_map>
#include "Flow_data_ptr.h"

class DataRepository {
public:
	bool is_computed(std::string data_id);
	void * operator [](std::string data_id);
	flow_data_ptr * result_location(std::string data_id);

	void reset();
private:
	std::unordered_map<std::string, flow_data_ptr> computed_results;
};
