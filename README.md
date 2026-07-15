
<img src="assets/lucid_banner.png" width="400">

# Datenstrom

`Datenstrom` (German for *data stream*) is a vendor-agnostic, multiplatform data encoding framework and RDBMS abstraction layer written in modern C++. It synergistically combines strongly typed streaming concepts with a unified interface to streamline database connectivity and structured data parsing.

> **Internal Note:** This project is a work in progress with evolving design concerns. Rough edges are actively being streamlined.

---

## Technical Specifications

* **Language Standard:** C++17 (ISO/IEC 14882:2017)
* **Architecture:** 32-bit and 64-bit clean
* **Design Philosophy:** Strongly influenced by and highly compatible with the C++ Standard Template Library (STL).

---

## Core Features

### 1. Distinct Field & Record Separation
Unlike the standard STL streams which space-separate tokens natively, `Datenstrom` enforces precise atomic field boundaries and discrete data collection (record) endings via standard manipulators.

```cpp
// STL Behavior vs Datenstrom Behavior
std::ostream out; out << "Hello" << "World"; // Result: "HelloWorld"

ds::ostream out;  out << "Hello" << "World"; // Result: Separated fields!

```

### 2. Custom & Aggregate Type Integration

Easily extend the stream interface to handle application-specific data models by overloading the `<<` and `>>` operators.

```cpp
struct job_t {
   job_key_t   key;
   std::string client;
};

// Seamlessly stream out complex structs
ds::ostream out;
job_t job1 = { ... };
out << job1 << ds::endr;

```

### 3. Binary Serialisation

Marshalling for protocols, binary file formats etc.

* Leverages std::streambuf and derived classes

* Supports big and little endian 

### 4. Unified RDBMS Abstraction Layer

Write vendor-agnostic code with automatic translation layer support for multiple database drivers:

* SQLite (`sqlite:///...`)


* Firebird (`firebird://...`)


* MySQL (`mysql://...`)


* MSSQL (`mssql://...`)


* PostgreSQL (`postgresql://...`)


Features include consistent parameter parsing, ISO-8601 date/time uniformity, and scope-bound execution management via explicit RAII structures (`ds::db::transaction` and `ds::db::savepoint`).

---

### 5. Pluggable SQL Modules & Context Abstraction

Datenstrom decouples application code from raw, vendor-specific SQL queries through pluggable SQL modules. Using ds::db::context, you can execute abstract named SQL queries that automatically resolve to the correct SQL variant matching your runtime connection type.

SQL Module Resolution Example

| Logical Query Name | Target Database (Firebird Syntax) | Target Database (MSSQL Syntax) |
|--------------------|-----------------------------------|--------------------------------|
| job.first_5 | SELECT FIRST 5 job_id, job_type, job_client FROM jobs | SELECT job_id, job_type, job_client FROM jobs LIMIT 5 |
| job.insert |INSERT INTO jobs . . . | INSERT INTO jobs . . . |

---

## Usage Examples

### Custom Object Streaming (Field/Record Separation)

```cpp
ds::ostream out;
out << "Hello" << "World" << ds::endr
    << "Goodbye" << "World" << ds::endr;

ds::istream in;
std::string s1, s2;
in >> s1 >> ds::endr >> s2 >> ds::endr;

assert(s1 == "Hello");
assert(s2 == "Goodbye");

```

### RDBMS Query Iterator

```cpp
// Connect using URI syntax
ds::db::connection con("firebird://localhost:3050/C:/tmp#pmdb");

// Leverage C++ range-based loops directly over database results
for (job_t job : con("SELECT FIRST 5 job_id, job_type, job_client FROM jobs")) {
   do_something(job);
}

```

### Scope-Bound Explicit Transactions

```cpp
ds::db::connection con("firebird://localhost:3050/C:/tmp#pmdb");
ds::db::namelist_t params = { "id", "type", "client" };
ds::db::statement insert = con("INSERT INTO jobs VALUES( :id, :type, :client )", params);

{
   ds::db::transaction txn(con); // Starts Transaction context
   for (const job_t & job : jobs) {
      insert << job << ds::endr;
   }
} // End of scope – automatically commits the transaction safely

```

---

### SQL Modules

```cpp
// connection_string specifiest the database type and location
//
// * firebird://localhost:3050/C:/tmp#pmdb
// * mssql://127.0.0.1#pmdb
// ...etc

ds::db::context ctx(connection_string);
ds::db::namelist_t params = { "id", "type", "client" };
ds::db::statement insert = ctx("job.insert", params);

{
   ds::db::transaction txn(ctx);
   for (const job_t & job : jobs) {
      insert << job << ds::endr;
   }
}

```

---

## Multi-Engine Testing

Integration testing can be handled via standard **GoogleTest** suites. The database context allows running identical test cases across  different backends safely by parameterizing the runtime connections:

```cpp
TEST_P(Job, should_provide_job_list) {
   ds::db::context ctx(GetParam());
   db_dal::Job_insert job_insert(ctx);
   db_dal::Job_list   job_list(ctx);
   
   // ... Run transactional tests ...
   EXPECT_EQ(result_vec, source_vec);
}

// Instantiate across disparate internal server modules
INSTANTIATE_NAMESPACE_TEST_SUITE_P(DataBaseFirebird, Job, testing::Values("firebird://localhost:3050/C:/tmp#pmdb"));
INSTANTIATE_NAMESPACE_TEST_SUITE_P(DataBaseMSSQL, Job, testing::Values("mssql://127.0.0.1#pmdb"));

