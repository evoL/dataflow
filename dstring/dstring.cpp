#include "dstring.h"
#include <cstdlib>
#include <cstring>

extern "C"
{
    DSTRING_API const char * DATAFLOW_MODULE module()
    {
        return "dstring";
    }

    DSTRING_API const char * DATAFLOW_MODULE types()
    {
        return "String\0";
    }

    DSTRING_API const char * DATAFLOW_MODULE operations()
    {
        return "";
    }

    DSTRING_API unsigned int DATAFLOW_MODULE String_size()
    {
        return sizeof(dString);
    }

    DSTRING_API bool DATAFLOW_MODULE String_construct(const char * data, void * output)
    {
        int len = strlen(data);
        dString & s = *(dString *)output;
        s = (dString)malloc(len * sizeof(char));
        strcpy(s, data);

        return true;
    }

    DSTRING_API bool DATAFLOW_MODULE String_destruct(void * data)
    {
        dString & s = *(dString *)data;
        free(s);
        return true;
    }
}
