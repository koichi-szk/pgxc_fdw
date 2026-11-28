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
char[PGXC_NAMELEN]	pgxc_cluster_name;
char[PGXC_STRLEN]	pgxc_prepare_prefix;

int	pgxc_max_transactions;
