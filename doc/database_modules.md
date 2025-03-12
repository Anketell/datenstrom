# Datenstrom Database Modules
## Policies and implementation notes
### v0.1

## 1 Overview

One of the main goals of the Relational Database Management System (RDBMS) modules is to provide logical drop-in replacements whilst, at the same time, honouring the various strengths of the underlying RDBMS. 

To this end, the following areas are addressed:

* Connection State
* Implicit Transactions
* Binary Large Object Access
* SQL Dialect Support

### 1.1 Connection State

Broadly speaking, connections to RDBMS implementations come in two flavours. Servic based RDBMS implementations typically support the idea that a service can be connection can occur without need to specify a database. Others require the database name and other still require a full file path.

Datenstrom has a logical distinction between a database's location and its name. All database connection implementations MUST support the ability to be constructed with a database location with or without a database name.



### 1.2 Implicit Transactions


### 1.3 Binary Large Object Access


### 1.4 SQL Dialect Support

## 2 Interpretation

The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in this document are to be interpreted as described in RFC 2119.

## 3 Connection State

CON-01 

## 4 Implicit Transactions

TXN-01

## 5 Binary Large Object Access

BOB-01

## 6 Database Classes



