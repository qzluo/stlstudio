#ifndef __STUDIO_DATA_H__
#define __STUDIO_DATA_H__

typedef struct tagStudioParams {
    double coord_gap;
    double axis_len;
    double axis_linewidth;
    double ws_length;
    double ws_width;
    double ws_height;
    double ws_gap;
    double align_margin;
    double align_gap;
}STUDIOPARAMS, *PSTUDIOPARAMS;

int InitData(void);

int SetStduioParams(PSTUDIOPARAMS params);
int GetStduioParams(PSTUDIOPARAMS params);

#endif  //__STUDIO_DATA_H__