/* Copyright (C) 1991, 1995, 1997 Aladdin Enterprises.  All rights reserved.

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
/* Requires imemory.h */

#ifndef isave_INCLUDED
#  define isave_INCLUDED

/*
 * According to the PostScript language definition, save objects are simple,
 * not composite.  Consequently, we cannot use their natural representation,
 * namely a t_struct pointing to an alloc_save_t, since we aren't willing to
 * allocate them all in global VM and rely on garbage collection to clean
 * them up.  Instead, we assign each one a unique "save ID", and store this
 * in the alloc_save_t object.  Mapping the number to the object requires
 * at most searching the local save chain for the current gs_dual_memory_t,
 * and this approach means we don't have to do anything to invalidate
 * save objects when we do a restore.
 */
#ifndef alloc_save_t_DEFINED	/* also in inamedef.h */
typedef struct alloc_save_s alloc_save_t;

#  define alloc_save_t_DEFINED
#endif

/* Initialize the save machinery. */
extern void alloc_save_init(P1(gs_dual_memory_t *));

/* Map a save ID to its save object.  Return 0 if the ID is invalid. */
alloc_save_t *alloc_find_save(P2(const gs_dual_memory_t *, ulong));

/* Save the state.  Return 0 if we can't allocate the save object, */
/* otherwise return the save ID.  The second argument is a client data */
/* pointer, assumed to point to an object. */
ulong alloc_save_state(P2(gs_dual_memory_t *, void *));

/* Get the client pointer passed to alloc_saved_state. */
void *alloc_save_client_data(P1(const alloc_save_t *));

/* Return the current level of save nesting. */
int alloc_save_level(P1(const gs_dual_memory_t *));

/* Return (the id of) the innermost externally visible save object. */
ulong alloc_save_current_id(P1(const gs_dual_memory_t *));

#define alloc_save_current(dmem)\
  alloc_find_save(dmem, alloc_save_current_id(dmem))

/* Check whether a pointer refers to an object allocated since a given save. */
bool alloc_is_since_save(P2(const void *, const alloc_save_t *));

/* Check whether a name was created since a given save. */
bool alloc_name_is_since_save(P2(const ref *, const alloc_save_t *));
bool alloc_name_index_is_since_save(P2(uint, const alloc_save_t *));

/* Check whether any names have been created since a given save */
/* that might be released by the restore. */
bool alloc_any_names_since_save(P1(const alloc_save_t *));

/* Do one step of restoring the state.  Return true if the argument */
/* was the innermost save, in which case this is the last (or only) step. */
/* Assume the caller obtained the argument by calling alloc_find_save; */
/* if this is the case, the operation cannot fail. */
bool alloc_restore_state_step(P1(alloc_save_t *));

/* Forget a save -- like committing a transaction (restore is like */
/* aborting a transaction).  Assume the caller obtained the argument */
/* by calling alloc_find_save.  Note that forgetting a save does not */
/* require checking pointers for recency. */
void alloc_forget_save(P1(alloc_save_t *));

/* Release all memory -- like doing a restore "past the bottom". */
void alloc_restore_all(P1(gs_dual_memory_t *));

/*
 * If we are in a save, we want to save the old contents if l_new is
 * not set; if we are not in a save, we never want to save old contents.
 * We can test this quickly with a single mask that is l_new if we are
 * in a save, and -1 if we are not, since type_attrs of a valid ref
 * cannot be 0; this is the test_mask in a gs_dual_memory_t.  Similarly,
 * we want to set the l_new bit in newly allocated objects iff we are in
 * a save; this is the new_mask in a gs_dual_memory_t.
 *
 * We have to pass the pointer to the containing object to alloc_save_change
 * for two reasons:
 *
 *      - We need to know which VM the containing object is in, so we can
 * know on which chain of saved changes to put the new change.
 *
 *      - We need to know whether the object is an array of refs (which
 * includes dictionaries) or a struct, so we can properly trace and
 * relocate the pointer to it from the change record during garbage
 * collection.
 */
int alloc_save_change(P4(gs_dual_memory_t *, const ref * pcont,
			 ref_packed * ptr, client_name_t cname));

/* ------ Internals ------ */

/* Record that we are in a save. */
#define alloc_set_in_save(dmem)\
  ((dmem)->test_mask = (dmem)->new_mask = l_new)

/* Record that we are not in a save. */
#define alloc_set_not_in_save(dmem)\
  ((dmem)->test_mask = ~0, (dmem)->new_mask = 0)

#endif /* isave_INCLUDED */
