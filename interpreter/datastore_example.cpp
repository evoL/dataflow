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

    repository.createEntry(1, sizeof(int));
    repository.createEntry(2, sizeof(int));
    repository.createEntry(3, sizeof(int));

    input_block(3, repository[1]);
    input_block(12, repository[2]);
    add(repository[1], repository[2], repository[3]);
    output_integer(repository[3]);

    return 0;
}
