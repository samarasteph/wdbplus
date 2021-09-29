#pragma once

#ifndef __WDB_H_CPP_
#define __WDB_H_CPP_
#include <string>
#include <stdexcept>
#include "wdb.hpp"

class WhiteDb;

namespace wdb{
    class exception: public std::runtime_error {
    public:
        exception(const std::string& what): std::runtime_error(what){}
    };
    class type_error : public wdb::exception {};

    class WriteLock {
    public:
        WriteLock();
        WriteLock(WriteLock&& );
        WriteLock& operator = (WriteLock&&);
        ~WriteLock();
        void lock();
        void unlock();
    private:
        friend WhiteDb;
        WriteLock(void* db);
        WriteLock(const WriteLock&) = delete;
        WriteLock& operator = (const WriteLock&) = delete;
        void* _db;
        wg_int _lock;
    };

    class ReadLock {
    public:
        ReadLock();
        ReadLock(ReadLock&&);
        ReadLock& operator = (ReadLock&&);
        ~ReadLock();
        void lock();
        void unlock();
    private:
        friend WhiteDb;
        ReadLock(const ReadLock&) = delete;
        ReadLock& operator = (const ReadLock&) = delete;
        ReadLock(void* db);
        void* _db;
        wg_int _lock;
    };
}

class WhiteDb: private wdb::PrimitiveDatabase<WhiteDb> {
public:

    typedef PrimDb_t::BasicRecord_t BasicRecord;
    typedef PrimDb_t::Field Field;
    typedef PrimDb_t::iterator iterator;
    typedef PrimDb_t::Query_t Query;
    template<wg_int NbFields>
    using Record = PrimDb_t::Record<NbFields>;
    template<wg_int NbFields>
    using rec_iterator = PrimDb_t::rec_iterator<NbFields>;

    //let access to those methods
    using PrimDb_t::createQuery;
    using PrimDb_t::createRecord;

    WhiteDb();
    bool open(const std::string& name, size_t size, bool create=false);
    void close();
    void destroy();
    bool isValid() const;
    std::string get_name() const;

    ssize_t dbSize() const;
    ssize_t dbFreeSize() const;

    iterator deleteRecord(WhiteDb::iterator it);

    wdb::Date utcDate() const;
    wdb::Date localDate() const;
    wdb::Time utcTime() const;
    wdb::Time localTime() const;

    iterator begin();
    iterator end();
    template<wg_int NbField>
    rec_iterator<NbField> begin();
    template<wg_int NbField>
    rec_iterator<NbField> end();

    wdb::WriteLock lock_write();
    wdb::ReadLock lock_read();

    //indexes
    wg_int create_index(wg_int column, wg_int type, wg_int *matchrec, wg_int reclen);
    wg_int create_multi_index(wg_int *columns, wg_int col_count, wg_int type, wg_int *matchrec, wg_int reclen);
    wg_int drop_index(wg_int index_id);
    wg_int column_to_index_id(wg_int column, wg_int type, wg_int *matchrec, wg_int reclen);
    wg_int multi_column_to_index_id(wg_int *columns, wg_int col_count, wg_int type, wg_int *matchrec, wg_int reclen);
    wg_int get_index_type(wg_int index_id);
    void * get_index_template(wg_int index_id, wg_int *reclen);
    void * get_all_indexes(wg_int *count);
    
private:
    friend WhiteDb::BasicRecord;
    friend WhiteDb::PrimDb_t;
    void* handle();
    void* _db;
    std::string _name;
};

template<wg_int NbField>
WhiteDb::rec_iterator<NbField> WhiteDb::begin(){
    wdb::first_record first;
    return rec_iterator<NbField>(*this,first(_db));
}

template<wg_int NbField>
WhiteDb::rec_iterator<NbField> WhiteDb::end(){
    return rec_iterator<NbField>(*this,nullptr);
}

#endif