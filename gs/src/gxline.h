/* Copyright (C) 1995, 1996, 1998 Aladdin Enterprises.  All rights reserved.

   This file is part of Aladdin Ghostscript.

   Aladdin Ghostscript is distributed with NO WARRANTY OF ANY KIND.  No author
   or distributor accepts any responsibility for the consequences of using it,
   or for whether it serves any particular purpose or works at all, unless he
   or she says so in writing.  Refer to the Aladdin Ghostscript Free Public
   License (the "License") for full details.

   Every copy of Aladdin Ghostscript must include a copy of the License,
   normally in a plain ASCII text file named PUBLIC.  The License grants you
   the right to copy, modify and redistribute Aladdin Ghostscript, but only
   under certain conditions described in the License.  Among other things, the
   License requires that the copyright notice and this notice be preserved on
   all copies.
 */

/*$Id$ */
/* Private line parameter definitions */

#ifndef gxline_INCLUDED
#  define gxline_INCLUDED

#include "gslparam.h"

/* Line parameter structures */
/* gx_dash_params are never instantiated by themselves. */
typedef struct gx_dash_params_s {
    float *pattern;
    uint pattern_size;
    float offset;
    bool adapt;
    /* The rest of the parameters are computed from the above */
    float pattern_length;	/* total of all pattern elements */
    bool init_ink_on;		/* true if ink is initially on */
    int init_index;		/* initial index in pattern */
    float init_dist_left;
} gx_dash_params;

#define gx_dash_params_initial\
  NULL, 0, 0.0, 0/*false*/, 0.0, 1/*true*/, 0, 0.0
typedef struct gx_line_params_s {
    float half_width;		/* one-half line width */
    gs_line_cap cap;
    gs_line_join join;
    float miter_limit;
    float miter_check;		/* computed from miter limit, */
    /* see gx_set_miter_limit and */
    /* gs_stroke */
    float dot_length;
    bool dot_length_absolute;	/* if true, dot_length is 1/72" units */
    gx_dash_params dash;
} gx_line_params;

#define gx_set_line_width(plp, wid)\
  ((plp)->half_width = (wid) / 2)
#define gx_current_line_width(plp)\
  ((plp)->half_width * 2)
int gx_set_miter_limit(P2(gx_line_params *, floatp));

#define gx_current_miter_limit(plp)\
  ((plp)->miter_limit)
int gx_set_dash(P5(gx_dash_params *, const float *, uint, floatp,
		   gs_memory_t *));

#define gx_set_dash_adapt(pdp, adpt) ((pdp)->adapt = (adpt))
int gx_set_dot_length(P3(gx_line_params *, floatp, bool));

/* See gsline.c for the computation of miter_check. */
#define gx_line_params_initial\
  0.0, gs_cap_butt, gs_join_miter, 10.0, 0.20305866, 0.0, 0/*false*/,\
  { gx_dash_params_initial }

#endif /* gxline_INCLUDED */
