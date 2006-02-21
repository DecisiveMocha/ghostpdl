/* Copyright (C) 1992, 1995-2004 artofcode LLC. All rights reserved.
  
  This software is provided AS-IS with no warranty, either express or
  implied.
  
  This software is distributed under license and may not be copied,
  modified or distributed except as expressly authorized under the terms
  of the license contained in the file LICENSE in this distribution.
  
  For more information about licensing, please refer to
  http://www.ghostscript.com/licensing/. For information on
  commercial licensing, go to http://www.artifex.com/licensing/ or
  contact Artifex Software, Inc., 101 Lucas Valley Road #110,
  San Rafael, CA  94903, U.S.A., +1(415)492-9861.
*/

/* $Id: $ */
/* Rom File System settings */

#ifndef gsiorom_INCLUDED
#  define gsiorom_INCLUDED

#define ROMFS_BLOCKSIZE 16384
#define ROMFS_CBUFSIZE ((int)((ROMFS_BLOCKSIZE) * 1.001) + 12)

/* enble the ROMFS_COMPRESSION as optional
  #define ROMFS_COMPRESSION
*/

/*
 * in memory structure is:
 *
 *	offset_to_next_inode (total length of this inode)	[32-bit big-endian]
 *	length_of_uncompressed_file				[32-bit big-endian]
 *	data_block_struct[]		count is (length+ROMFS_BLOCKSIZE-1)/ROMFS_BLOCKSIZE
 *	padded_file_name (char *)	includes as least one terminating <nul>
 *	padded_data_blocks
 */
/*
 *	data_block_struct:
 *	    data_length			(not including pad)	[32-bit big-endian]
 *	    data_block_offset		(start of each block)	[32-bit big-endian]
 */

#endif /* gsiorom_INCLUDED */
