# pgxc\_fdw

## Project outline

This project is a spinout of postgres-xc/xl.   Whie postgres-xc/xl is a fork from postgreSQL source code, it was very painful to catch up postgreSQL evolution.
After years, thanks to postgreSQL community effort, postgreSQL now comes with sufficient infrastructure to implement postgres-xc/xl goal as postgreSQL extension, making it much simpler to catch up postgreSQL evolution.

The purpose of this project is to make one or a set of PostgreSQL extensions to provide scale-out offering, where more than PostgreSQL servers work together as a single database, to allow more transaction throughputt for OLTP workloads and to allow bigger and faster SQL execution for analytic workloads.

Many exising scale-out offerings consist of two database components: coordinators and local databases.   Coordinator accepts application transactions, transform them into transactions for each underlying local databases.   In this project, we are not going to distinguish coordinator and local databases.    Any component database can accept application workload and then transform them into each local database workload.

## Components to implement

Pgxc\_fdw will be composed with the following components:

### Node configuration

Defines and manages local databases included in specific "pgxc\_fdw cluster".

### SQL handler

Works as usual FDW module.   However, we allow two types of tables:

* Sharded table: composes each shard.
* Replicated table: this table is copied to all the nodes of pgxc\_fdw cluster.

Pgxc\_fdw will provide consistent means to write to these two types of tables, as usual table.

### Distributed MVCC

This is essential to provide consistent read of the data scattered among nodes.  At present, we are implementing this based upon Hybrid Logical Clock.

### Distributed deadlock detection

At present, our plan is to construct distributed wait-for-graph, presumably forking current deadlock detection function in PostgreSQL itself.

### Embedded connection pooler among the nodes

This helps to save overheads to establish inter-node connection.

### Distrubuted MVCC among standbys

### Synchronization with failover/switchover.

## Sample SQL code for cluster/node/table initialization

### Overview

The purpose of the following example code is to illustrate overview how replicated tables and partitioned tables are represented using exisiting FDW framework.
Details of `OPTIONS` element and value can be change for simplicity, consiteny, for future compatibility and for any other backgrounds.

Reader of this script is free to attempt any other formats.

On thing that we need to maitain is:

* Ther server in `pgxc_dfw` can be a hypothetic one to represent more than one server as a single server.   It is needed to work `pgxc_fdw` plan and execution under PostgreSQL FDW infrastructure.

### Host and tables

#### Global view

* `TableA`: partitioned table,

* `TableB`: replicated table,

#### host1

* `tableA_par1`: one of the partition of `TalbeA`,
* `rtabB`: `TableB`
	
#### host2

* `tableA_par2`: one of the partition of `TalbeA`,
* `rtabB`: `TableB`
	
#### host3

* `tableA_par3`: one of the partition of `TalbeA`,
* `rtabB`: `TableB`
	
#### host4

* `tableA_par4`: one of the partition of `TalbeA`,
* `rtabB`: `TableB`
	

### FDW name:

* `pgxc_fdw`

### Server definition

#### Individual server

```sql
CREATE SERVER host1 TYPE 'postgres' VERSION '17'
	FOREIGN DATA WRAPPER pgxc_fdw
	OPTIONS (host 'host1', user 'foo', port '5432');

CREATE SERVER host2 TYPE 'postgres' VERSION '17'
	FOREIGN DATA WRAPPER pgxc_fdw
	OPTIONS (host 'host1', user 'foo', port '5432');

CREATE SERVER host3 TYPE 'postgres' VERSION '17'
	FOREIGN DATA WRAPPER pgxc_fdw
	OPTIONS (host 'host1', user 'foo', port '5432');

CREATE SERVER host4 TYPE 'postgres' VERSION '17'
	FOREIGN DATA WRAPPER pgxc_fdw
	OPTIONS (host 'host1', user 'foo', port '5432');
```

#### Group server

```sql
CREATE SERVER cluster1 TYPE 'group'
	FOREIGN DATA WRAPPER pgxc_fdw
	OPTIONS(servers 'host1 host2 host3 host4',
			pooler 'cluster1_pooler',
			pool_init '1',
			pool_max  '10',
			pool_limit '15',
			session_string 'SET ....; SET ...;');
```

### User Mapping

```sql
CREATE USER MAPPING FOR USER foo
	SERVER host1;
CREATE USER MAPPING FOR USER foo
	SERVER host2;
CREATE USER MAPPING FOR USER foo
	SERVER host3;
CREATE USER MAPPING FOR USER foo
	SERVER host4;
CREATE USER MAPPING FOR USER foo
	SERVER cluster1;
```

### Replicated Table

#### ELement table

* Please note that `SERVER` in the statement body is used by the planner and executor.
* `server` in `OPTIONS` clause is the actual server for the foreign table.   It is handled by `pgxc_fdw`.

```sql
CREATE FOREIGN TABLE rtab_b_host1
	(column ....)
	SERVER host1
	OPTIONS (repl 'true', is_parent 'false', real_name 'rtabB', parent 'tabB');

CREATE FOREIGN TABLE rtab_b_host2
	(column ....)
	SERVER host2
	OPTIONS (repl 'true', is_parent 'false', real_name 'rtabB', parent 'tabB');

CREATE FOREIGN TABLE rtab_b_host3
	(column ....)
	SERVER host3
	OPTIONS (repl 'true', is_parent 'false', real_name 'rtabB', parent 'tabB');

CREATE FOREIGN TABLE rtab_b_host4
	(column ....)
	SERVER host4
	OPTIONS (repl 'true', is_parent 'false', real_name 'rtabB', parent 'tabB');
```

#### Parent table

```sql
CREATE FOREIGN TABLE tabB
	(column ...)
	SERVER	cluster1
	OPTIONS(repl 'true', is_parent 'true',
			elements 'rtab_b_host1 rtab_b_host2 rtab_b_host3 rtab_b_host4'
			preferred 'rtab_b_host1');
```

### Paritioned table

#### Parent table

```sql
CREATE TABLE tableA
	(column ....)
	PARTITIONED BY ...;
```


#### Partition

```sql
CREATE FOREIGN TABLE tableA_part1
	(columns ...)
	PARTITION OF TableA (... partitioning details ...)
	SERVER cluster1
	OPTIONS (server 'host1', partition 'true', server 'host1', real_name 'tableA_part1');

CREATE FOREIGN TABLE tableA_part2
	(columns ...)
	PARTITION OF TableA (... partitioning details ...)
	SERVER cluster1
	OPTIONS (server 'host2', partition 'true', server 'host2');

CREATE FOREIGN TABLE tableA_part3
	(columns ...)
	PARTITION OF TableA (... partitioning details ...)
	SERVER cluster1
	OPTIONS (server 'host3', partition 'true', server 'host3', real_name 'tableA_part3');

CREATE FOREIGN TABLE tableA_part4
	(columns ...)
	PARTITION OF TableA (... partitioning details ...)
	SERVER cluster1
	OPTIONS (server 'host4', partition 'true', server 'host4', real_name 'tableA_part4');
```

### Points

* With the above definition, all the partitions and (virtually single)replicated table are regarded to be in the server 'cluster1'.
* 'Cluster1' is actually a group of servers.  `Pgxc_fdw` understands this and it can tell the core about join/aggregate pushdown and other operation, where to go.
* Also `pgxc_fdw` can manage write to the replicated table, to write to all the nodes, considering preferred node as well.
