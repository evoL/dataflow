cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
cmake_policy(SET CMP0042 NEW) # suppresses warning on OS X

project(dataflow)

add_subdirectory(basicmath)

source_group("Headers" "\\.h$")
source_group("Sources" "\\.c(pp)?$")
