#include "Datastore.h"
#include <iostream>

void input_block(int input, void * result) {
	*((int *) result) = input;
}

void add(void * in_1, void * in_2, void * result) {
	int first_number = *static_cast<int*>(in_1); 
	int second_number = *static_cast<int*>(in_2);
	*((int *)result) = first_number + second_number;
}

void output_integer(void * input) {
	int integer_to_output = *static_cast<int*>(input);
	std::cout << integer_to_output;
}

int main() {
	Datastore repository;
	
	repository.create_entry("input_1", sizeof(int));
	repository.create_entry("input_2", sizeof(int));
	repository.create_entry("add_result", sizeof(int));

	input_block(3, repository["input_1"]);
	input_block(12, repository["input_2"]);
	add(repository["input_1"], repository["input_2"], repository["add_result"]);
	output_integer(repository["add_result"]);
	
	return 0;
}
