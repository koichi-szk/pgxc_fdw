/*
 *============================================================================
 *
 * pgxc_fdw GUC variables
 *
 *============================================================================
 */

#include "postgres.h"
#include "fmgr.h"

/* pgxc_fdw administorator db user */
extern char		*pgxc_admin_user;

/* Total number of pgxc_fdw transactions in this node */
extern int		pgxc_max_transactions;



