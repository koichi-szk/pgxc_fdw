/*
 *============================================================================
 *
 * pgxc_fdw GUC variables
 *
 *============================================================================
 */

#include "postgres.h"
#include "fmgr.h"

char[PGXC_NAMELEN]	pgxc_admin_user;

int	pgxc_max_transactions;
