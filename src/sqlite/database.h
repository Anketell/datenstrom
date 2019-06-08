//-----------------------------------------------------------------------------

#ifndef DS_SQLITE_DATABASE_H
#define DS_SQLITE_DATABASE_H

//-----------------------------------------------------------------------------

#include <db/database.h>
#include <sqlite3.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace sqlite
{

//-----------------------------------------------------------------------------

class database : public db::database
{
   const std::string m_path;
   sqlite3         * m_db = nullptr;

   std::string get_full_path( const std::string & name ) const;

   void close( void );

public:

   static constexpr char TYPE[] = "sqlite";

   database( const std::string & path );

   virtual ~database( void );

   virtual const char * type( void ) const override;

   virtual void create( const std::string & name ) override;
   virtual void use( const std::string & name ) override;
   virtual bool drop( const std::string & name ) override;

   virtual db::statement operator()( const std::string     & query,
                                     const db::name_list_t & parameters = {} ) override;

   virtual void begin_transaction( void ) override;
   virtual void commit_transaction( void ) override;
   virtual void rollback_transaction( void ) override;

   virtual void savepoint( const std::string & ) override;
   virtual void release_savepoint( const std::string & ) override;
   virtual void rollback_to_savepoint( const std::string & ) override;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
