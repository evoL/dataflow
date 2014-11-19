#include "Datastore.h"
#include <iostream>

void input_block(int input, stored_ptr * result) {
    *result = make_stored_ptr<int>(input);
}

void add(void * in_1, void * in_2, stored_ptr * result) {
    int first_number = *static_cast<int*>(in_1); 
    int second_number = *static_cast<int*>(in_2);
    *result = make_stored_ptr<int>(first_number + second_number);
}

void output_integer(void * input) {
    int integer_to_output = *static_cast<int*>(input);
    std::cout << integer_to_output;
}

void main() {
    Datastore repository;

    input_block(3, repository.result_location("input_1"));
    input_block(12, repository.result_location("input_2"));
    add(repository["input_1"], repository["input_2"], repository.result_location("add_result"));
    output_integer(repository["add_result"]);

    std::cin.get();
}
