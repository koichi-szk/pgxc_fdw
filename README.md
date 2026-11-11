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
