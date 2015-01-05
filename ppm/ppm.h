#ifndef PPM_H
#define PPM_H

#include <dataflow.h>
//#include <dstring.h>
#include <cstdio>

#ifdef PPM_EXPORTS
#   define PPM_API DATAFLOW_MODULE_EXPORT
#else
#   define PPM_API DATAFLOW_MODULE_IMPORT
#endif

#pragma pack(1)

struct dColor
{
	unsigned char r, g, b;
};

struct dPpm
{
    uint32_t width;
	uint32_t height;
	dColor * data;
};

#pragma pack()

extern "C"
{
    PPM_API const char * DATAFLOW_MODULE module();
    PPM_API const char * DATAFLOW_MODULE types();
    PPM_API const char * DATAFLOW_MODULE operations();
    PPM_API unsigned int DATAFLOW_MODULE Ppm_size();
    PPM_API bool DATAFLOW_MODULE Ppm_construct(const char * data, void * output);
    PPM_API bool DATAFLOW_MODULE Ppm_destruct(void * data);
	PPM_API const char * DATAFLOW_MODULE SavePpm_inputs();
	PPM_API const char * DATAFLOW_MODULE SavePpm_outputs();
	PPM_API bool DATAFLOW_MODULE SavePpm_execute(void * const * inputs, void * const * outputs);

	// private functions
	void try_comment(FILE * f);
	void skip_whitespaces(FILE * f);
	int fpeek(FILE * f);
}

#endif
