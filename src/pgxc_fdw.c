/*
 *============================================================================
 *
 * Main pgxc_fdw source:
 *
 * _PG_init()
 *
 *============================================================================
 */

#include "postgres.h"
#include "fmgr.h"

#include "pgxc_fdw.h"
#include "pgxc_fdw_guc.h"

PG_MODULE_MAGIC;

void	_PG_init(void);
void	_PG_fini(void);
static void	set_guc(void);
static void register_workers(void);

void
_PG_init(void)
{
    elog(LOG, "pgxc_fdw: _PG_init() called");

	/* Setting up PGXC_FDW specific GUCs */
	set_guc();

	/* Register worker processes */
	register_workers();

	/* Register shared memory allocation routine. */
}

static void
set_guc(void)
{
	/*  pgxc_fdw.admin_user */
	DefineCustomStringVariable(
		"pgxc_fdw.admin_user",		/* Parameter name */
									/* Short description */
		"Sets pgxc_cluster administrator db user name.",
									/* Long description */
		NULL,
		&pgxc_admin_user,			/* Pointer to store the value */
		"pgxc_admin",				/* Default value */
		PGC_SUSET,					/* Only superuser */
		0,							/* Flag */
		NULL,						/* Check_hook */
		NULL,						/* Assign_hook */
		NULL						/* Show_hook */
	);
	/* Cluster name */
	DefineCustomStringVariable(
		"pgxc_fdw.cluster_name",	/* Parameter name */
									/* Short description */
		"Name of pgxc_fdw cluster.",
									/* Long description */
		"Name of pgxc_fdw cluster.  Must be the same among all the pgxc_fdw cluster nodes.",
		&pgxc_cluster_name,			/* Pointer to store the value */
		"pgxc_cluster",				/* Default value */
		PGC_POSTMASTER,				/* Only superuser */
		0,							/* Flag */
		NULL,						/* Check_hook */
		NULL,						/* Assign_hook */
		NULL						/* Show_hook */
	);
	/* String used to create transaction-id in internal PREPARE TRANSACTION name */
	/* This value will be concatinated with XID of the parent transaction */
	DefineCustomStringVariable(
		"pgxc_fdw.prepare_prefix",	/* Parameter name */
									/* Short description */
		"Prefix for internal 2PC transaction-id.",
									/* Long description */
		"Not recommended to change this among there's any outstanding internal prepared transactions",
		&pgxc_prepare_prefix,		/* Pointer to store the value */
		"pgxc_fdw_tid_",			/* Default value */
		PGC_POSTMASTER,				/* Only superuser */
		0,							/* Flag */
		NULL,						/* Check_hook */
		NULL,						/* Assign_hook */
		NULL						/* Show_hook */
	);
	/* pgxc_fdw.max_transactions */
	DefineCustomIntVariable(
        "pgxc_fdw.max_transactions",	/* Paramere name */
										/* Short description */
        "Sets max pgxc_transactions in this node.",
        NULL,                          /* Long descriptions */
        &pgxc_max_transactions,			/* Pointer to store the value */
        100,							/* Default value */
        10,								/* Minimum value */
        INT_MAX,						/* Maximm value */
        PGC_POSTMASTER,					/* Change requests database restart */
        0,								/* Flags */
        NULL,							/* check_hook */
        NULL,							/* assign_hook */
        NULL							/* show_hook */
    );
	/* Distributed deadlock detector timeout */


}

/*
 * Register worker processes:
 *
 *	HLC generator,
 *	Distributed deadlock detector:
 */
static void register_workers(void)
{
}


void
_PG_fini(void)
{
    elog(LOG, "pgxc_fdw: _PG_fini() called");
}
