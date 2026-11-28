/*
 *============================================================================
 *
 * pgxc_fdw internal structure/variabale
 *
 *============================================================================
 */

#ifndef PGXC_FDW_INTERNAL
#define	PGXC_FDW_INTERNAL

#include	"postgres.h"
#include	"fmgr.h"

struct pgxc_remote_blocker
{
	oid				blocker_server;
	TransactionId	blocker_server_xid;
	pid				blocker_server_pid;
}

/*
 * In parallel scan, single locak TX can wait for
 * more than one remote TX.
 */
struct pgxc_remote_blockers
{
	struct pgxc_remote_blocker	*next;
	struct pgxc_remote_blocker	this_blocker;
}

struct pgxc_pgproc
{
	pid				proc_pid;
	TransactionId	proc_xid;
	struct pgxc_remote_blocker	*remote_blocker;
}
	

#endif
