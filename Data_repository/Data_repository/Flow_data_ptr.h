#pragma once
#include <memory>

using flow_data_ptr = std::shared_ptr<void>;

template <class TypeBeingHidden, typename ... Args>
flow_data_ptr make_flow_data_ptr(Args ... arguments) {
	return std::make_shared<TypeBeingHidden>(arguments...);
}