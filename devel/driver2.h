
Here is an example of how the driver interface will be used. While reading 
this notice how templates and inheratence is used to link the "driver" in
at compile time and staticly.  Also notice how you can add functionality 
by simply defining the method in the Driver Classes.

class Mysql {
  // The Driver Class
  class Connection {
    // The connection that will be inherited by SQLConnection
  private:
    static pointer_tracker<MYSQL, Connection> others;
    
    bool throw_exceptions;
    MYSQL mysql;
    bool is_connected;
    bool locked;
    bool Success;
    
    void         close() {mysql_close(&mysql);}
    
  public:
    // The REQUIRED METHODS
    bool   connected() const {return is_connected;}
    bool   success() {return Success;}
    bool   connect (cchar *db, bool te);

    Result  store(const string &str, bool te);
    ResUse  use(const string &str, bool te);
    ResNSel execute(const string &str, bool te);

    ~Connection ();

    //the rest of the methods add extra functionality

    bool   connect (cchar *db = "", cchar *host="", cchar *user = "", char *passed = "");
    bool   connect (cchar *db, cchar *host, cchar *user, char *passed, bool te);
    
    bool   lock() {if (locked) return true; locked = true; return false;}
    void   unlock() {locked = false;}
    
    void purge (MYSQL *m) {mysql_close(&mysql); }
    
    Query query() {return Query(this, throw_exceptions);}
    
    operator bool () {return success();}

    string error () {return string(mysql_error(&mysql));}
    string clinet_info () {return string(mysql_get_client_info());}
    string host_info () {return string(mysql_get_host_info(&mysql));}
    int    proto_info () {return mysql_get_proto_info(&mysql);}
    string server_info () {return string(mysql_get_server_info(&mysql));}
    string stat() {return string(mysql_stat(&mysql));}

    bool   create_db (string db) {return !(execute( "CREATE DATABASE " + db ));}
    bool   drop_db (string db) {return !(execute( "DROP DATABASE " + db ));}
    bool   select_db (string db) {return select_db(db.c_str());}
    bool   select_db (const char *db);
    bool   reload();
    bool   shutdown ();
  };

  class Connection2 : public SQLConnection<Mysql> {
    // This class solely exists to define constructures
    
    include_basic_constructures
    // this micro inculdes the basic and needed conetructures 
    
    // then we can define additional ones.
    Connection (const char *host, const char *user = "",
		const char *passwd = "", bool te = false);
    Connection (const char *db, const char *host, const char *user = "", 
		const char *passwd = "", bool te = false);
  };

};  

// This is the actual SQLConnection class.  Noitice how little is
// defined in it, as most of the methods are in the driver class as
// the most of the SQLConnection functionality is communicating with the
// SQL Server.

template <class Driver>
class SQLConnection : public Driver::Connection {
  Result  store(const string &str) {return store(str, throw_exceptions);}
  ResUse  use(const string &str)   {return use(str, throw_exceptions);}
  ResNSel execute(const string &str) {return execute(str, throw_exceptions);}
  
  SQLConnection(te = false) : Driver::Connection(te) {}
  SQLConnection(const char* db, te = false) : Driver::Connection(db,te) {}
  
  template <class Sequence> void storein_sequence(Sequence &, const string &);
  template <class Set>      void storein_set(Set &, const string &); 
  
                            void storein(MutableRes &con, const string &s);
  template <class T>        void storein(vector<T> &con, const string &s)
    {storein_sequence(con,s);}
  template <class T>        void storein(deque<T> &con, const string &s)
     {storein_sequence(con,s);}
  template <class T>        void storein(list<T> &con, const string &s)
    {storein_sequence(con,s);}
  template <class T>        void storein(slist<T> &con, const string &s)
    {storein_sequence(con,s);}
  template <class T>        void storein(set<T> &con, const string &s)
    {storein_set(con,s);}
  template <class T>        void storein(multiset<T> &con, const string &s)
    {storein_set(con,s);}
}

typedef Mysql::Connection2 MysqlConnection;
