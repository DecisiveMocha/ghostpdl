/*
 * Copyright (C) 1998 Aladdin Enterprises.
 * All rights reserved.
 *
 * This file is part of Aladdin Ghostscript.
 *
 * Aladdin Ghostscript is distributed with NO WARRANTY OF ANY KIND.  No author
 * or distributor accepts any responsibility for the consequences of using it,
 * or for whether it serves any particular purpose or works at all, unless he
 * or she says so in writing.  Refer to the Aladdin Ghostscript Free Public
 * License (the "License") for full details.
 *
 * Every copy of Aladdin Ghostscript must include a copy of the License,
 * normally in a plain ASCII text file named PUBLIC.  The License grants you
 * the right to copy, modify and redistribute Aladdin Ghostscript, but only
 * under certain conditions described in the License.  Among other things, the
 * License requires that the copyright notice and this notice be preserved on
 * all copies.
 */

/* pccprint.c - PCL5c print model commands */

#include "std.h"
#include "pcommand.h"
#include "pcstate.h"
#include "pcfont.h"
#include "gsmatrix.h"		/* for gsstate.h */
#include "gsstate.h"
#include "gsrop.h"

/*
 * ESC * l <rop> O
 *
 * Set logical operation.
 */
  private int
pcl_logical_operation(
    pcl_args_t *    pargs,
    pcl_state_t *   pcs
)
{
    uint            rop = uint_arg(pargs);

    if (rop > 255)
	return e_Range;

    pcl_break_underline(pcs);   /* use the 5c convention; in 5e, the
                                 * underline is not broken by a change in
                                 * the logical operation */
    pcs->logical_op = rop;
    return 0;
}

/*
 * ESC * l <bool> R
 *
 * Set prixel placement. Note that this feature is not yet properly
 * implemented.
 */
  private int
pcl_pixel_placement(
    pcl_args_t *    pargs,
    pcl_state_t *   pcs
)
{
    uint            i = uint_arg(pargs);

    if (i > 1)
	return 0;
    pcs->pp_mode = i;
    return 0;
}

/*
 * Initialization
 */
  private int
pccprint_do_init(
    gs_memory_t *   mem
)
{
    /* Register commands */
    DEFINE_CLASS('*')
    {
        'l', 'O',
	PCL_COMMAND( "Logical Operation",
                     pcl_logical_operation,
		     pca_neg_ok | pca_big_error
                     )
    },
    {
        'l', 'R',
        PCL_COMMAND( "Pixel Placement",
                     pcl_pixel_placement,
		     pca_neg_ok | pca_big_ignore
                     )
    },
    END_CLASS
    return 0;
}

  private void
pccprint_do_reset(
    pcl_state_t *       pcs,
    pcl_reset_type_t    type
)
{
    static  const uint  mask = (  pcl_reset_initial
                                | pcl_reset_printer
                                | pcl_reset_overlay );

    if ((type & mask) != 0) {
        pcs->logical_op = 252;
        pcs->pp_mode = 0;
    }
}

const pcl_init_t    pccprint_init = { pccprint_do_init, pccprint_do_reset, 0 };
