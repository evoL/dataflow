#ifndef PPM_H
#define PPM_H

#include <dataflow.h>
#include <cstdio>

#ifdef PPM_EXPORTS
#   define PPM_API DATAFLOW_MODULE_EXPORT
#else
#   define PPM_API DATAFLOW_MODULE_IMPORT
#endif

#pragma pack(1)

struct dColor {
    unsigned char r, g, b;
};

struct dPpm {
    uint32_t width;
    uint32_t height;
    dColor * data;
};

struct dImageFilter {
    uint32_t size;
    int32_t * data;
};

#pragma pack()

extern "C"
{
    PPM_API const char * DATAFLOW_MODULE module();
    PPM_API const char * DATAFLOW_MODULE types();
    PPM_API const char * DATAFLOW_MODULE operations();
    PPM_API unsigned int DATAFLOW_MODULE Ppm_size();
    PPM_API unsigned int DATAFLOW_MODULE ImageFilter_size();
    PPM_API bool DATAFLOW_MODULE Ppm_construct(const char * data, void * output);
    PPM_API bool DATAFLOW_MODULE ImageFilter_construct(const char * data, void * output);
    PPM_API bool DATAFLOW_MODULE Ppm_destruct(void * data);
    PPM_API bool DATAFLOW_MODULE ImageFilter_destruct(void * data);
    PPM_API const char * DATAFLOW_MODULE SavePpm_inputs();
    PPM_API const char * DATAFLOW_MODULE NegativePpm_inputs();
    PPM_API const char * DATAFLOW_MODULE RotateLeftPpm_inputs();
    PPM_API const char * DATAFLOW_MODULE RotateRightPpm_inputs();
    PPM_API const char * DATAFLOW_MODULE HorizontalReflectionPpm_inputs();
    PPM_API const char * DATAFLOW_MODULE VerticalReflectionPpm_inputs();
    PPM_API const char * DATAFLOW_MODULE ToGrayscalePpm_inputs();
    PPM_API const char * DATAFLOW_MODULE ApplyFilterPpm_inputs();
    PPM_API const char * DATAFLOW_MODULE SavePpm_outputs();
    PPM_API const char * DATAFLOW_MODULE NegativePpm_outputs();
    PPM_API const char * DATAFLOW_MODULE RotateLeftPpm_outputs();
    PPM_API const char * DATAFLOW_MODULE RotateRightPpm_outputs();
    PPM_API const char * DATAFLOW_MODULE HorizontalReflectionPpm_outputs();
    PPM_API const char * DATAFLOW_MODULE VerticalReflectionPpm_outputs();
    PPM_API const char * DATAFLOW_MODULE ToGrayscalePpm_outputs();
    PPM_API const char * DATAFLOW_MODULE ApplyFilterPpm_outputs();
    PPM_API bool DATAFLOW_MODULE SavePpm_execute(void * const * inputs, void * const *);
    PPM_API bool DATAFLOW_MODULE NegativePpm_execute(void * const * inputs, void * const * outputs);
    PPM_API bool DATAFLOW_MODULE RotateLeftPpm_execute(void * const * inputs, void * const * outputs);
    PPM_API bool DATAFLOW_MODULE RotateRightPpm_execute(void * const * inputs, void * const * outputs);
    PPM_API bool DATAFLOW_MODULE HorizontalReflectionPpm_execute(void * const * inputs, void * const * outputs);
    PPM_API bool DATAFLOW_MODULE VerticalReflectionPpm_execute(void * const * inputs, void * const * outputs);
    PPM_API bool DATAFLOW_MODULE ToGrayscalePpm_execute(void * const * inputs, void * const * outputs);
    PPM_API bool DATAFLOW_MODULE ApplyFilterPpm_execute(void * const * inputs, void * const * outputs);

    // private functions
    void try_comment(FILE * f);
    int fpeek(FILE * f);
}

#endif
