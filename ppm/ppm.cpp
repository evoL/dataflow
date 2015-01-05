#include "ppm.h"
#include <cstdlib>
#include <cstring>

#define _PX(ppm, x, y) ((ppm).data[(y)*(ppm).width + (x)])

extern "C"
{
    PPM_API const char * DATAFLOW_MODULE module()
    {
        return "ppm";
    }

    PPM_API const char * DATAFLOW_MODULE types()
    {
        return "Ppm\0";
    }

    PPM_API const char * DATAFLOW_MODULE operations()
    {
        return "SavePpm\0NegativePpm\0RotateLeftPpm\0RotateRightPpm\0HorizontalReflectionPpm\0VerticalReflectionPpm\0";
    }

    PPM_API unsigned int DATAFLOW_MODULE Ppm_size()
    {
        return sizeof(dPpm);
    }

    PPM_API bool DATAFLOW_MODULE Ppm_construct(const char * data, void * output)
    {
        dPpm& ppm = *(dPpm *)output;

        FILE * f = fopen(data, "rb");
        if (!f)
            return false;

        try_comment(f);
        bool bin_mode;
        char buf[1024];
        fscanf(f, "%s", buf);
        if (strcmp(buf, "P3") == 0)
            bin_mode = false;
        else if (strcmp(buf, "P6") == 0)
            bin_mode = true;
        else
            return false;
    
        try_comment(f);
        fscanf(f, "%d", &ppm.width);

        try_comment(f);
        fscanf(f, "%d", &ppm.height);
    
        try_comment(f);
        int bits = 0;
        fscanf(f, "%d", &bits);
    
        ppm.data = (dColor *)malloc(sizeof(dColor) * ppm.width * ppm.height);

        if (bin_mode) {
            fgetc(f);
            fread((char*)&ppm.data[0], 1, ppm.width * ppm.height * 3, f);
        }
        else
        {
            for (int i = 0; i < ppm.width * ppm.height; i++)
            {
                int v;
                fscanf(f, "%d", &v);
                ppm.data[i].r = v*255/bits;
                fscanf(f, "%d", &v);
                ppm.data[i].g = v*255/bits;
                fscanf(f, "%d", &v);
                ppm.data[i].b = v*255/bits;
            }
        }

        fclose(f);
        return true;
    }

    PPM_API bool DATAFLOW_MODULE Ppm_destruct(void * data)
    {
        dPpm& ppm = *(dPpm *)data;
        free(ppm.data);
        return true;
    }

    PPM_API const char * DATAFLOW_MODULE SavePpm_inputs()
    {
        return "ppm.Ppm\0dstring.String\0";
    }

    PPM_API const char * DATAFLOW_MODULE NegativePpm_inputs()
    {
        return "ppm.Ppm\0";
    }

    PPM_API const char * DATAFLOW_MODULE RotateLeftPpm_inputs()
    {
        return "ppm.Ppm\0";
    }

    PPM_API const char * DATAFLOW_MODULE RotateRightPpm_inputs()
    {
        return "ppm.Ppm\0";
    }

    PPM_API const char * DATAFLOW_MODULE HorizontalReflectionPpm_inputs()
    {
        return "ppm.Ppm\0";
    }

    PPM_API const char * DATAFLOW_MODULE VerticalReflectionPpm_inputs()
    {
        return "ppm.Ppm\0";
    }

    PPM_API const char * DATAFLOW_MODULE SavePpm_outputs()
    {
        return "";
    }

    PPM_API const char * DATAFLOW_MODULE NegativePpm_outputs()
    {
        return "ppm.Ppm\0";
    }

    PPM_API const char * DATAFLOW_MODULE RotateLeftPpm_outputs()
    {
        return "ppm.Ppm\0";
    }

    PPM_API const char * DATAFLOW_MODULE RotateRightPpm_outputs()
    {
        return "ppm.Ppm\0";
    }

    PPM_API const char * DATAFLOW_MODULE HorizontalReflectionPpm_outputs()
    {
        return "ppm.Ppm\0";
    }

    PPM_API const char * DATAFLOW_MODULE VerticalReflectionPpm_outputs()
    {
        return "ppm.Ppm\0";
    }

    PPM_API bool DATAFLOW_MODULE SavePpm_execute(void * const * inputs, void * const * )
    {
        dPpm& ppm = *(dPpm *)(inputs[0]);
        dString str = *(dString *)(inputs[1]);

        FILE* f = fopen(str, "w");
        if (!f)
            return false;

        fprintf(f, "P3\n");
        fprintf(f, "%d %d\n", ppm.width, ppm.height);
        fprintf(f, "255\n");

        for (int i=0; i<ppm.width * ppm.height; i++)
            fprintf(f, "%d %d %d\n", ppm.data[i].r, ppm.data[i].g, ppm.data[i].b);

        fclose(f);

        return true;
    }

    PPM_API bool DATAFLOW_MODULE NegativePpm_execute(void * const * inputs, void * const * outputs)
    {
        dPpm& ppm = *(dPpm *)(inputs[0]);
        dPpm& out_ppm = *(dPpm *)(outputs[0]);

        out_ppm.width = ppm.width;
        out_ppm.height = ppm.height;
        out_ppm.data = (dColor *)malloc(sizeof(dColor) * ppm.width * ppm.height);

        for (int i=0; i<ppm.width*ppm.height; i++) {
            out_ppm.data[i].r = 255 - ppm.data[i].r;
            out_ppm.data[i].g = 255 - ppm.data[i].g;
            out_ppm.data[i].b = 255 - ppm.data[i].b;
        }

        return true;
    }

    PPM_API bool DATAFLOW_MODULE RotateLeftPpm_execute(void * const * inputs, void * const * outputs)
    {
        dPpm& ppm = *(dPpm *)(inputs[0]);
        dPpm& out_ppm = *(dPpm *)(outputs[0]);

        out_ppm.width = ppm.height;
        out_ppm.height = ppm.width;
        out_ppm.data = (dColor *)malloc(sizeof(dColor) * ppm.width * ppm.height);

        for (int y=0; y<out_ppm.height; y++) {
            for (int x=0; x<out_ppm.width; x++) {
                _PX(out_ppm, x, y).r = _PX(ppm, ppm.width-1-y, x).r;
                _PX(out_ppm, x, y).g = _PX(ppm, ppm.width-1-y, x).g;
                _PX(out_ppm, x, y).b = _PX(ppm, ppm.width-1-y, x).b;
            }
        }

        return true;
    }

    PPM_API bool DATAFLOW_MODULE RotateRightPpm_execute(void * const * inputs, void * const * outputs)
    {
        dPpm& ppm = *(dPpm *)(inputs[0]);
        dPpm& out_ppm = *(dPpm *)(outputs[0]);

        out_ppm.width = ppm.height;
        out_ppm.height = ppm.width;
        out_ppm.data = (dColor *)malloc(sizeof(dColor) * ppm.width * ppm.height);

        for (int y=0; y<out_ppm.height; y++) {
            for (int x=0; x<out_ppm.width; x++) {
                _PX(out_ppm, x, y).r = _PX(ppm, y, ppm.height-1-x).r;
                _PX(out_ppm, x, y).g = _PX(ppm, y, ppm.height-1-x).g;
                _PX(out_ppm, x, y).b = _PX(ppm, y, ppm.height-1-x).b;
            }
        }

        return true;
    }

    PPM_API bool DATAFLOW_MODULE HorizontalReflectionPpm_execute(void * const * inputs, void * const * outputs)
    {
        dPpm& ppm = *(dPpm *)(inputs[0]);
        dPpm& out_ppm = *(dPpm *)(outputs[0]);

        out_ppm.width = ppm.width;
        out_ppm.height = ppm.height;
        out_ppm.data = (dColor *)malloc(sizeof(dColor) * ppm.width * ppm.height);

        for (int y=0; y<out_ppm.height; y++) {
            for (int x=0; x<out_ppm.width; x++) {
                _PX(out_ppm, x, y).r = _PX(ppm, x, ppm.height-1-y).r;
                _PX(out_ppm, x, y).g = _PX(ppm, x, ppm.height-1-y).g;
                _PX(out_ppm, x, y).b = _PX(ppm, x, ppm.height-1-y).b;
            }
        }

        return true;
    }

    PPM_API bool DATAFLOW_MODULE VerticalReflectionPpm_execute(void * const * inputs, void * const * outputs)
    {
        dPpm& ppm = *(dPpm *)(inputs[0]);
        dPpm& out_ppm = *(dPpm *)(outputs[0]);

        out_ppm.width = ppm.width;
        out_ppm.height = ppm.height;
        out_ppm.data = (dColor *)malloc(sizeof(dColor) * ppm.width * ppm.height);

        for (int y=0; y<out_ppm.height; y++) {
            for (int x=0; x<out_ppm.width; x++) {
                _PX(out_ppm, x, y).r = _PX(ppm, ppm.width-1-x, y).r;
                _PX(out_ppm, x, y).g = _PX(ppm, ppm.width-1-x, y).g;
                _PX(out_ppm, x, y).b = _PX(ppm, ppm.width-1-x, y).b;
            }
        }

        return true;
    }

    
    // private

    void try_comment(FILE * f)
    {
        char linebuf[1024];
        char ppp;
        while (true) {
            while (ppp = fpeek(f), ppp == '\n' || ppp == '\r')
                fgetc(f);
            if (ppp == '#')
                fgets(linebuf, 1023, f);
            else
                break;
        }
    }

    int fpeek(FILE * f)
    {
        int c = fgetc(f);
        ungetc(c, f);
        return c;
    }
}
