#include "Datastore.h"
#include <iostream>

void input_block(int input, void * result)
{
    *((int *) result) = input;
}

void add(void * in1, void * in2, void * result)
{
    int firstNumber = *static_cast<int *>(in1);
    int secondNumber = *static_cast<int *>(in2);
    *((int *)result) = firstNumber + secondNumber;
}

void output_integer(void * input)
{
    int integerToOutput = *static_cast<int *>(input);
    std::cout << integerToOutput;
}

int main()
{
    Datastore repository;

    repository.createEntry("input_1", sizeof(int));
    repository.createEntry("input_2", sizeof(int));
    repository.createEntry("add_result", sizeof(int));

    input_block(3, repository["input_1"]);
    input_block(12, repository["input_2"]);
    add(repository["input_1"], repository["input_2"], repository["add_result"]);
    output_integer(repository["add_result"]);

    return 0;
}
