/*
 *============================================================================
 *
 * pgxc_fdw global definitions/variables
 *
 * global definitions,
 * shared memory structure,
 * 
 *
 *============================================================================
 */

#ifndef PGXC_FDW_H
#define PGXC_FDW_H

#include "postgres.h"
#include "fmgr.h"

#define PGXC_NAMELEN (64 + 1)

/* Additional PGPROC information */
/* Actual definition will be in pgxc_fdw_internal.h */
typedef struct pgxc_pgproc PGXC_PGPROC;

#endif
