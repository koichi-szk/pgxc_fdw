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

void _PG_init(void);

void _PG_fini(void);

void
_PG_init(void)
{
    elog(LOG, "pgxc_fdw: _PG_init() called");

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
	/* Register shared memory allocation routine. */
}

void
_PG_fini(void)
{
    elog(LOG, "pgxc_fdw: _PG_fini() called");
}
