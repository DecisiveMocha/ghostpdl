/* Copyright (C) 1997, 1998 Aladdin Enterprises.  All rights reserved.

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
/* Setting trapping parameters and zones */
#include "string_.h"
#include "gx.h"
#include "gserrors.h"
#include "gstrap.h"

/* Parameter utilities, copied from gdevpsdf.c. */
/* These should be merged.... */

/* Compare a C string and a gs_param_string. */
private bool
trap_key_eq(const gs_param_string * pcs, const char *str)
{
    return (strlen(str) == pcs->size &&
	    !strncmp(str, (const char *)pcs->data, pcs->size));
}

/* Put an enumerated value. */
private int
trap_put_enum_param(gs_param_list * plist, gs_param_name param_name,
		    int *pvalue, const char *const pnames[], int ecode)
{
    gs_param_string ens;
    int code = param_read_name(plist, param_name, &ens);

    switch (code) {
	case 1:
	    return ecode;
	case 0:
	    {
		int i;

		for (i = 0; pnames[i] != 0; ++i)
		    if (trap_key_eq(&ens, pnames[i])) {
			*pvalue = i;
			return 0;
		    }
	    }
	    code = gs_error_rangecheck;
	default:
	    ecode = code;
	    param_signal_error(plist, param_name, code);
    }
    return code;
}

/* Put a Boolean, integer, or float parameter. */
private int
trap_put_bool_param(gs_param_list * plist, gs_param_name param_name,
		    bool * pval, int ecode)
{
    int code;

    switch (code = param_read_bool(plist, param_name, pval)) {
	default:
	    ecode = code;
	    param_signal_error(plist, param_name, ecode);
	case 0:
	case 1:
	    break;
    }
    return ecode;
}
private int
trap_put_int_param(gs_param_list * plist, gs_param_name param_name,
		   int *pval, int ecode)
{
    int code;

    switch (code = param_read_int(plist, param_name, pval)) {
	default:
	    ecode = code;
	    param_signal_error(plist, param_name, ecode);
	case 0:
	case 1:
	    break;
    }
    return ecode;
}
private bool
check_unit(float *pval)
{
    return (*pval >= 0 && *pval <= 1);
}
private bool
check_positive(float *pval)
{
    return (*pval > 0);
}
private int
trap_put_float_param(gs_param_list * plist, gs_param_name param_name,
		     float *pval, bool(*check) (P1(float *pval)), int ecode)
{
    int code;

    switch (code = param_read_float(plist, param_name, pval)) {
	case 0:
	    if ((*check) (pval))
		return 0;
	    code = gs_error_rangecheck;
	default:
	    ecode = code;
	    param_signal_error(plist, param_name, ecode);
	    break;
	case 1:
	    break;
    }
    return ecode;
}

/* settrapparams */
int
gs_settrapparams(gs_trap_params_t * pparams, gs_param_list * plist)
{
    gs_trap_params_t params;
    int ecode = 0;
    static const char *const trap_placement_names[] =
    {
	gs_trap_placement_names, 0
    };

    params = *pparams;
    ecode = trap_put_float_param(plist, "BlackColorLimit",
				 &params.BlackColorLimit,
				 check_unit, ecode);
    ecode = trap_put_float_param(plist, "BlackDensityLimit",
				 &params.BlackDensityLimit,
				 check_positive, ecode);
    ecode = trap_put_float_param(plist, "BlackWidth",
				 &params.BlackWidth,
				 check_positive, ecode);
    ecode = trap_put_bool_param(plist, "Enabled",
				&params.Enabled, ecode);
    ecode = trap_put_bool_param(plist, "ImageInternalTrapping",
				&params.ImageInternalTrapping, ecode);
    ecode = trap_put_int_param(plist, "ImageResolution",
			       &params.ImageResolution, ecode);
    if (params.ImageResolution <= 0)
	param_signal_error(plist, "ImageResolution",
			   ecode = gs_error_rangecheck);
    ecode = trap_put_bool_param(plist, "ImageToObjectTrapping",
				&params.ImageToObjectTrapping, ecode);
    {
	int placement = params.ImageTrapPlacement;

	ecode = trap_put_enum_param(plist, "ImageTrapPlacement",
				    &placement, trap_placement_names, ecode);
	params.ImageTrapPlacement = placement;
    }
    ecode = trap_put_float_param(plist, "SlidingTrapLimit",
				 &params.SlidingTrapLimit,
				 check_unit, ecode);
    ecode = trap_put_float_param(plist, "StepLimit",
				 &params.StepLimit, check_unit, ecode);
    ecode = trap_put_float_param(plist, "TrapColorScaling",
				 &params.TrapColorScaling,
				 check_unit, ecode);
    ecode = trap_put_float_param(plist, "TrapWidth",
				 &params.TrapWidth,
				 check_positive, ecode);
    if (ecode < 0)
	return ecode;
    *pparams = params;
    return 0;
}
