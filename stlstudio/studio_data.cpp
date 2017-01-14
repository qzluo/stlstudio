#include "stdafx.h"
#include "studio_data.h"

static STUDIOPARAMS s_studioparams = {
    400, 400, 300, 10, 10
};

int InitData(void)
{
    return 0;
}

int SetStduioParams(PSTUDIOPARAMS params)
{
    if (!params)
        return -1;

    if (params->coord_gap <= 0 || params->axis_len <= 0 ||
        params->axis_linewidth <= 0 || params->ws_gap <= 0 ||
        params->ws_length <= 0 || params->ws_width <= 0 ||
        params->ws_height <= 0 || params->align_margin <= 0 ||
        params->align_gap <= 0)
        return -1;

    s_studioparams.coord_gap = params->coord_gap;
    s_studioparams.axis_len = params->axis_len;
    s_studioparams.axis_linewidth = params->axis_linewidth;
    s_studioparams.ws_length = params->ws_length;
    s_studioparams.ws_width = params->ws_width;
    s_studioparams.ws_height = params->ws_height;
    s_studioparams.ws_gap = params->ws_gap;
    s_studioparams.align_margin = params->align_margin;
    s_studioparams.align_gap = params->align_gap;

    return 0;
}

int GetStduioParams(PSTUDIOPARAMS params)
{
    if (!params)
        return -1;

    params->coord_gap = s_studioparams.coord_gap;
    params->axis_len = s_studioparams.axis_len;
    params->axis_linewidth = s_studioparams.axis_linewidth;
    params->ws_length = s_studioparams.ws_length;
    params->ws_width = s_studioparams.ws_width;
    params->ws_height = s_studioparams.ws_height;
    params->ws_gap = s_studioparams.ws_gap;
    params->align_margin = s_studioparams.align_margin;
    params->align_gap = s_studioparams.align_gap;

    return 0;
}

