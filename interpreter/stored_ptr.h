#pragma once
#include <memory>

using stored_ptr = std::shared_ptr<void>;

template <class TypeBeingHidden, typename ... Args>
stored_ptr make_stored_ptr(Args ... arguments) {
	return std::make_shared<TypeBeingHidden>(arguments...);
}
