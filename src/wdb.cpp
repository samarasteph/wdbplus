#include <whitedb/dbapi.h>
#include "wdb.h"
#include <cstring>
#include <fstream>

template<class Type_t> class FieldAssignPrimitive{
    wg_int operator ()(void* db, void* record, wg_int fieldnr, Type_t data){
        return (*primitive_fnc_ptr)(db,record,fieldnr,data);
    }
    static wg_int (*primitive_fnc_ptr) (void*, void*, wg_int, Type_t);
};

WhiteDb::WhiteDb(): PrimitiveDatabase(*this), _db(nullptr) {}

void* WhiteDb::handle(){ return _db; }

bool WhiteDb::open(const std::string& name, size_t size, bool create){
    if(_db) close();
    _name = name;
    
    if (create) _db = wg_attach_database((char*)_name.c_str(), size);
    else        _db = wg_attach_database((char*)_name.c_str(), size);

    return _db != nullptr;
}
void WhiteDb::close(){
    if(_db){
        wg_detach_database(_db);
        _db = nullptr;
        _name.clear();
    }
}

void WhiteDb::destroy(){
    if (not _name.empty()){
        wg_delete_database(const_cast<char*>(_name.c_str()));
    }
}

bool WhiteDb::isValid() const {
    return _db != nullptr;
}

std::string WhiteDb::get_name() const{
    return _name;
}

WhiteDb::iterator WhiteDb::deleteRecord(WhiteDb::iterator it){
    if (checkDb(*it)){
        WhiteDb::BasicRecord next = nextRecord(*it), r = *it;
        PrimDb_t::deleteRecord(r);
        return iterator( next ); 
    }
    return end();
}

wdb::Date WhiteDb::utcDate() const {
    wdb::Date d;
    d.set(wg_current_utcdate(_db));
    return d;
}

wdb::Date WhiteDb::localDate() const {
    wdb::Date d;
    d.set(wg_current_localdate(_db));
    return d;
}

wdb::Time WhiteDb::utcTime() const {
    wdb::Time t;
    t.set(wg_current_utctime(_db));
    return t;
}

wdb::Time WhiteDb::localTime() const {
    wdb::Time t;
    t.set(wg_current_localtime(_db));
    return t;
}

WhiteDb::iterator WhiteDb::begin(){
    WhiteDb::BasicRecord br =  createRecord(wg_get_first_record(_db));
    return iterator(br);
}
WhiteDb::iterator WhiteDb::end(){
    WhiteDb::BasicRecord br = createRecord(nullptr);
    return iterator(br);
}

wdb::WriteLock WhiteDb::lock_write(){
    return wdb::WriteLock(_db);
}

wdb::ReadLock WhiteDb::lock_read(){
    return wdb::ReadLock(_db);
}

ssize_t WhiteDb::dbSize() const{
    return wg_database_size(_db);
}
ssize_t WhiteDb::dbFreeSize() const{
    return wg_database_freesize(_db);
}

wg_int WhiteDb::create_index(wg_int column, wg_int type, wg_int *matchrec, wg_int reclen){
    return wg_create_index(_db,column,type,matchrec,reclen);
}
wg_int WhiteDb::create_multi_index(wg_int *columns, wg_int col_count, wg_int type, wg_int *matchrec, wg_int reclen){
    return wg_create_multi_index(_db,columns,col_count,type,matchrec,reclen);
}
wg_int WhiteDb::drop_index(wg_int index_id){
    return wg_drop_index(_db,index_id);
}
wg_int WhiteDb::column_to_index_id(wg_int column, wg_int type, wg_int *matchrec, wg_int reclen){
    return wg_column_to_index_id(_db,column,type,matchrec,reclen);
}
wg_int WhiteDb::multi_column_to_index_id(wg_int *columns, wg_int col_count, wg_int type, wg_int *matchrec, wg_int reclen){
    return wg_multi_column_to_index_id(_db,columns,col_count,type,matchrec,reclen);
}
wg_int WhiteDb::get_index_type(wg_int index_id){
    return wg_get_index_type(_db,index_id);
}
void * WhiteDb::get_index_template(wg_int index_id, wg_int *reclen){
    return wg_get_index_template(_db,index_id,reclen);
}
void * WhiteDb::get_all_indexes(wg_int *count){
    return wg_get_all_indexes(_db,count);
}

namespace wdb{

    encode_int::encode_int(): wdb::primitive<wg_int,void*,wg_int>(&wg_encode_int)  {}
    decode_int::decode_int(): wdb::primitive<wg_int,void*,wg_int>(&wg_decode_int)  {}
    encode_double::encode_double(): wdb::primitive<wg_int,void*,double>(&wg_encode_double)  {}
    decode_double::decode_double(): wdb::primitive<double,void*,wg_int>(&wg_decode_double)  {}
    encode_null::encode_null(): wdb::primitive<wg_int,void*,wg_int>(&wg_encode_null)  {}
    decode_null::decode_null(): wdb::primitive<wg_int,void*,wg_int>(&wg_decode_null) {}
    encode_str::encode_str(): wdb::primitive<wg_int,void*,char*,char*>(&wg_encode_str)  {}
    decode_str:: decode_str(): wdb::primitive<char*,void*,wg_int>(&wg_decode_str)  {}
    encode_date::encode_date(): wdb::primitive<wg_int,void*,int>(&wg_encode_date)  {}
    decode_date::decode_date(): wdb::primitive<int,void*,wg_int>(&wg_decode_date)  {}
    encode_time::encode_time(): wdb::primitive<wg_int,void*,int>(&wg_encode_time)  {}
    decode_time::decode_time(): wdb::primitive<int,void*,wg_int>(&wg_decode_time)  {}
    encode_blob::encode_blob(): wdb::primitive<wg_int,void*,char*,char*,wg_int>(&wg_encode_blob)  {}
    decode_blob::decode_blob(): wdb::primitive<char*,void*,wg_int>(&wg_decode_blob)  {}
    decode_blob_len::decode_blob_len(): wdb::primitive<wg_int,void*,wg_int>(&wg_decode_blob_len)  {}
    decode_blob_type::decode_blob_type(): wdb::primitive<char*,void*,wg_int>(&wg_decode_blob_type)  {}
    decode_blob_type_len::decode_blob_type_len(): wdb::primitive<wg_int,void*,wg_int>(&wg_decode_blob_type_len)  {}
    encode_var_type::encode_var_type(): wdb::primitive<wg_int,void*,wg_int>(&wg_encode_var) {}
    decode_var_type::decode_var_type(): wdb::primitive<wg_int,void*,wg_int>(&wg_decode_var) {}

    create_record::create_record(): wdb::primitive<void*,void*,wg_int>(&wg_create_record){}
    delete_record::delete_record(): wdb::primitive<wg_int,void*,void*>(&wg_delete_record){}
    first_record::first_record(): wdb::primitive<void*,void*>(&wg_get_first_record) {}
    next_record::next_record(): wdb::primitive<void*,void*,void*>(&wg_get_next_record) {}
    set_field::set_field(): wdb::primitive<wg_int,void*,void*,wg_int,wg_int>(&wg_set_field) {}
    get_field::get_field(): wdb::primitive<wg_int,void*,void*,wg_int>(&wg_get_field) {}
    get_field_type::get_field_type(): wdb::primitive<wg_int,void*,void*,wg_int>(&wg_get_field_type) {}
    get_record_len::get_record_len(): wdb::primitive<wg_int,void*,void*>(&wg_get_record_len) {}

    ymd_to_date::ymd_to_date(): wdb::primitive<int,void*,int,int,int>(&wg_ymd_to_date){}
    hms_to_time::hms_to_time(): wdb::primitive<int,void*,int,int,int,int>(&wg_hms_to_time) {}
    date_to_ymd::date_to_ymd(): wdb::primitive<void,void*,int,int*,int*,int*>(&wg_date_to_ymd) {}
    time_to_hms::time_to_hms(): wdb::primitive<void,void*,int,int*,int*,int*,int*>(&wg_time_to_hms) {}

    Date::Date(): date(0) {}
    Date::Date(int d): date(d) {}
    Date::Date(int y, int m, int d) {
        date = ymd_to_date()(nullptr,y,m,d);
    }
    int Date::get() const {
        return date;
    }    
    int Date::get(int& y, int& m, int& d) const{
        wdb::date_to_ymd()(nullptr,date,&y,&m,&d);
        return date;
    }
    void Date::set(int d){
        this->date = d;    
    }

    Time::Time(): time(0) {}
    Time::Time(int t): time(t) {}
    Time::Time(int h, int m, int s) {
        time = wdb::hms_to_time()(nullptr,h,m,s,0);
    }
    int Time::get() const {
        return time;
    }
    int Time::get(int& h, int& m, int& s) const{
        int prt;
        wdb::time_to_hms()(nullptr,time,&h,&m,&s,&prt);
        return time;
    }
    void Time::set(int t){
        time = t;
    }

    Blob::Blob():_type(nullptr){}
    Blob::~Blob(){
        delete [] _type;
    }
    Blob::Blob(const wdb::byte* p, size_t len, const wdb::byte* type) : _buffer(p,len), _type(nullptr) {
        if (type){
            size_t len = std::strlen(reinterpret_cast<const char*>(type)) + 1;
            _type = new wdb::byte[len];
            std::memcpy(_type, type, len);
        }
    }
    Blob::Blob(wdb::Blob&& b): _buffer(std::move(b._buffer)){
        _type = b._type;
        b._type = nullptr;
    }
    const wdb::byte* Blob::get() const{
        return _buffer.c_str();
    }
    const wdb::byte* Blob::type() const{
        return _type;
    }
    size_t Blob::size() const{
        return _buffer.size();
    }
    size_t Blob::type_size() const {
        if (_type==nullptr)
            return 0;
        return std::strlen(reinterpret_cast<char*>(_type));
    }
    size_t Blob::loadFromFile(const std::string& fileName, wdb::byte* type){
        std::ifstream ifile(fileName, std::ios_base::in | std::ios_base::binary );
        if (ifile){
            constexpr size_t buffer_size = 1024;
            char buffer[buffer_size];
            size_t file_size = 0, read = 0;

            ifile.seekg(0, ifile.end);
            file_size = ifile.tellg();
            ifile.seekg(0, ifile.beg);

            while (read < file_size){
                size_t len = std::min(buffer_size,file_size-read);
                ifile.read(buffer,len);
                std::copy(reinterpret_cast<wdb::byte*>(buffer), reinterpret_cast<wdb::byte*>(buffer)+len,
                    std::back_inserter (_buffer));
                read += len;
            }

        }
        return 0;
    }
    Blob& Blob::operator = (Blob&& b){
        _type = b._type;
        _buffer = std::move(b._buffer);
        b._type = nullptr;
        return *this;
    }

    Var::Var(const wg_int id): _id(id) {}
    Var::Var(const Var& var): _id(var._id) {}
    wg_int Var::id() const { return _id; }
    Var::operator wg_int () const { return _id; }

    make_query::make_query(): wdb::primitive<wg_query*,void*,void*,wg_int,wg_query_arg*,wg_int>(&wg_make_query) {};
    fetch_query::fetch_query(): wdb::primitive<void*,void*,wg_query*>(&wg_fetch) {};
    destroy_query::destroy_query(): wdb::primitive<void,void*,wg_query*>(&wg_free_query) {};

    encode_query_param_null::encode_query_param_null(): wdb::primitive<wg_int,void*,char*>(&wg_encode_query_param_null) {}
    encode_query_param_record::encode_query_param_record(): wdb::primitive<wg_int,void*,void*>(&wg_encode_query_param_record) {}
    encode_query_param_char::encode_query_param_char(): wdb::primitive<wg_int,void*,char>(&wg_encode_query_param_char) {}
    encode_query_param_fixpoint::encode_query_param_fixpoint(): wdb::primitive<wg_int,void*,double>(&wg_encode_query_param_fixpoint) {}
    encode_query_param_date::encode_query_param_date(): wdb::primitive<wg_int,void*,int>(&wg_encode_query_param_date) { }
    encode_query_param_time::encode_query_param_time(): wdb::primitive<wg_int,void*,int>(&wg_encode_query_param_time) { }
    encode_query_param_var::encode_query_param_var(): wdb::primitive<wg_int,void*,wg_int>(&wg_encode_query_param_var) { }
    encode_query_param_int:: encode_query_param_int(): wdb::primitive<wg_int,void*,wg_int>(&wg_encode_query_param_int) { }
    encode_query_param_double::encode_query_param_double(): wdb::primitive<wg_int,void*,double>(&wg_encode_query_param_double) { }
    encode_query_param_str::encode_query_param_str(): wdb::primitive<wg_int,void*,char*,char*>(&wg_encode_query_param_str) { }
    free_query_param::free_query_param(): wdb::primitive<wg_int,void*,wg_int>(&wg_free_query_param) {};
}

wdb::WriteLock::WriteLock(): _db(nullptr), _lock(0) {}
wdb::WriteLock::~WriteLock(){
   unlock();
}
wdb::WriteLock::WriteLock(wdb::WriteLock&& rval){
    _db = rval._db;
    _lock = rval._lock;
    
    rval._db = nullptr;
    rval._lock = 0;
}
wdb::WriteLock& wdb::WriteLock::operator = (WriteLock&& rval){
    unlock();
    _db = rval._db;
    _lock = rval._lock;
    
    rval._db = nullptr;
    rval._lock = 0;
    return *this;
}
void wdb::WriteLock::lock(){
    if(_db && _lock==0){
        _lock = wg_start_write(_db);
    }
}
void wdb::WriteLock::unlock(){
    if (_db){
        if (_lock!=0){
            wg_end_write(_db,_lock);
            _lock = 0;
        }
    }
}
wdb::WriteLock::WriteLock(void* db): _db(db), _lock(0) {}


wdb::ReadLock::ReadLock(): _db(nullptr), _lock(0) {}
wdb::ReadLock::~ReadLock() {
    unlock();
}

wdb::ReadLock::ReadLock(wdb::ReadLock&& rval){
    _db = rval._db;
    _lock = rval._lock;

    rval._db = nullptr;
    rval._lock = 0;
}

wdb::ReadLock& wdb::ReadLock::operator = (wdb::ReadLock&& rval){
    unlock();
    
    _db = rval._db;
    _lock = rval._lock;

    rval._db = nullptr;
    rval._lock = 0;

    return *this;
}

void wdb::ReadLock::lock(){
    if(_db && _lock==0){
        _lock = wg_start_read(_db);
    }
}
void wdb::ReadLock::unlock(){
    if (_db){
        if (_lock!=0){
            wg_end_read(_db,_lock);
            _lock = 0;
        }
    }
}
wdb::ReadLock::ReadLock(void* db): _db(db), _lock(0){}
