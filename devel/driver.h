// SQL++ Template Driver Header File 1998 Kevin Atkinson

// This is the tenitive template file for setting up a driver for use
// in Mysql++ (Soon to be known as SQL++).  Unless otherwise states
// these classes will be inherited via template class (ie template
// <class Der> class SQLConnection : public Der) and then type
// definded to <Driver>Objeect (MysqlConnection for example) with
// optional shortcut definded with out the Driver prefix.
// 
// Natuarally you will need to add data members to these classes
// in order to allow the public data members to do their job.
// Also feel free to add aditional methods to take advantage of 
// specific features of the SQL Database.  For example you can
// define methods for getting the exact type information about
// the columns.

#include <sql/driver-int>

#define SQL_DRIVER Driver
// Replace Driver with the name you wish to give the SQL driver

class SQL_DRIVER {
  class Connection {
  public:
    bool connect(const char* database, bool te = false);
    // At very least this connect needs to be defined it should make a
    // connection to the server and return true on success or false
    // otherwise.  If throw_exceptions is set it also needs to throws
    // BadQuery("Error Message") on error.  te here stands for throw
    // exceptions
    
    bool close();
    // closes the connection

    bool execute(const char* query);
    // this should execute the query

    Result store();
    // this should store the result of the query in a fashion where
    // the rows and collums can be accessed randmely
    
    ResUse use();
    // this should store the result of the query in a facion that (at
    // very least) the data can be accessed one row at a time.  Once
    // one row is accessed that row does not have to be accesable
    // again.
  };
  class Connection2 : public Connection0 {
    // This class is defined so that you can create additional
    // constructures when creating Connection option (for example you
    // might want to include ones that specify a host name, or user
    // names to use).  It inherates the actual SQL Connection class
    // which already inherated SQL_DRIVER::Connection.

  public:
    include_basic_constructures
    // this micro inculdes the basic and needed conetructures defined
    // in Connection0.

    // include additional contrutures here and ONLY contructures
    // anything else you wish to include should go in Connection
  }
  
  class ResUse {
    // This class is generally used when the Result set will be
    // immendtly copied to some other conatiner.  After the data is
    // copied this result set is relisied.  Thus this set should be
    // designed to make that proccess go as fast as possible.  In
    // partically this means that this result set should NOT generally
    // copy the entire result set from the server into local memory.
    // Instead it should use the data as it comes in from the server.

  public:
    RowUse fetch_row();
    // fetches the next row.  That is once fetched it will
    // automatically advance to the next row
    
    const FieldNamesUse& names_use() const;
    // both of these should return a container which stores the
    // current names of the fields for the current result set.
    
    FieldTypesUse& types();
    const FieldTypes& types_use() const;
    // both of these should resturn a container which will return
    // to a reference to the type_info object of the c++ type which 
    // most closly matches the orginal sql type.
    
    size_type columns() const;
    // returns the number of columns in the reuslt set

    bool free();
    // frees the result set
  };

  class Result : public ResUse {
    // This result set unlike the other one should generally copy the
    // data into local memery as it is desined to act like asn STL
    // random access container.  Most of methodes this set requires
    // are also in ResUse thus this class inherates ResUse.
  public:
    RowUse operator[] (size_type n) const;
    // fetched the nth row 

    size_type rows() const;
    // returns the number of rows in the result set
  };
    
  class RowUse {
    // have the results in a fasion in which any column can be
    // retervied randomly.  I should not in generally make a copy of
    // the row.  Instead it should use the results from the parent
    // Result Set.
    
  protected:
    virtual ResUse& parent() = 0;
    // this virtual method is definded in case you need it to
    // communicate to the Result set.

  public:
    ColData operator[] (size_type n) const;
    // returnes the nth item in the row
  };

  class FieldNamesUse {
  protected:
    virtual ResUse& parent() = 0;
    // this virtual method is definded in case you need it to
    // communicate to the Result set.

  public:
    string operator[] (size_type n) const;
    // returnes the field name of the nth item in the row.
  };

  class FieldTypesUse {
  protected:
    virtual ResUse& parent() = 0;
    // this virtual method is definded in case you need it to
    // communicate to the Result set.

  public:
    sql_type_info operator[] (size_type n) const;
    // returnes the sql_type_info object of the C++ type that most
    // closly matches the actual SQL type.  This type can also be the
    // type of a class or struture designed to hold the info such as
    // Date, Time DateTime, Set (with a capital S), etc...  the
    // sql_type info object is composed from type_info (and is desined
    // to behave exactly like type_info) and contains extentended type
    // if such as if it can be null and the actual sql type info as a
    // string.
  };

  class sql_type_info {
    // you actually don't need to define anything here however you can
    // if you wish to add type info that is specific to the drivers
    // SQL database
  }
};
    
#include <sql/driver-rest>
// this will include the rest of the information to set stuff up

