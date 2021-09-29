#pragma once
#include <type_traits>
#include <string>
#include <iterator>
#include <cassert>
#include <whitedb/dbapi.h>
extern "C" {
    #include <whitedb/indexapi.h>
}

#ifndef __WDB_TEMPLATE_HPP__
#define __WDB_TEMPLATE_HPP__

namespace wdb {

    typedef unsigned char byte;
    typedef std::basic_string<wdb::byte> bytes;

    template<class Ret_t,class ... ARGS>
    class primitive {
    public:
        primitive(const primitive& prim): _fptr(prim.fptr){}
        primitive(Ret_t (*fptr)(ARGS...)): _fptr(fptr){}
        Ret_t operator () (ARGS... args){ return (*_fptr)(args...); }
    private:
        Ret_t (*_fptr)(ARGS...) ;
    };

    /* primitive classes */

    class encode_int: public wdb::primitive<wg_int,void*,wg_int>  { public: encode_int(); };
    class decode_int: public wdb::primitive<wg_int,void*,wg_int>  { public: decode_int(); };
    class encode_double: public wdb::primitive<wg_int,void*,double>  { public: encode_double(); };
    class decode_double: public wdb::primitive<double,void*,wg_int>  { public: decode_double(); };
    class encode_null: public wdb::primitive<wg_int,void*,wg_int>  { public: encode_null(); };
    class decode_null: public wdb::primitive<wg_int,void*,wg_int>  { public: decode_null(); };
    class encode_str: public wdb::primitive<wg_int,void*,const char*,const char*>  { public: encode_str(); };
    class decode_str: public wdb::primitive<char*,void*,wg_int>  { public: decode_str(); };
    class encode_date: public wdb::primitive<wg_int,void*,int>  { public: encode_date(); };
    class decode_date: public wdb::primitive<int,void*,wg_int>  { public: decode_date(); };
    class encode_time: public wdb::primitive<wg_int,void*,int>  { public: encode_time(); };
    class decode_time: public wdb::primitive<int,void*,wg_int>  { public: decode_time(); };
    class encode_blob: public wdb::primitive<wg_int,void*,const char*,const char*,wg_int>  { public: encode_blob(); };
    class decode_blob: public wdb::primitive<char*,void*,wg_int>  { public: decode_blob(); };
    class decode_blob_len: public wdb::primitive<wg_int,void*,wg_int>  { public: decode_blob_len(); };
    class decode_blob_type: public wdb::primitive<char*,void*,wg_int>  { public: decode_blob_type(); };
    class decode_blob_type_len: public wdb::primitive<wg_int,void*,wg_int>  { public: decode_blob_type_len(); };
    class encode_var_type: public wdb::primitive<wg_int,void*,wg_int> { public: encode_var_type(); };
    class decode_var_type: public wdb::primitive<wg_int,void*,wg_int> { public: decode_var_type(); };

    class create_record:    public wdb::primitive<void*,void*,wg_int> { public: create_record(); };
    class delete_record:    public wdb::primitive<wg_int,void*,void*>  { public: delete_record(); };
    class first_record:     public wdb::primitive<void*,void*> { public: first_record(); };
    class next_record:      public wdb::primitive<void*,void*,void*> { public: next_record(); };
    class set_field:        public wdb::primitive<wg_int,void*,void*,wg_int,wg_int> { public: set_field(); };
    class get_field:        public wdb::primitive<wg_int,void*,void*,wg_int> { public: get_field(); };
    class get_field_type:   public wdb::primitive<wg_int,void*,void*,wg_int> { public: get_field_type(); };
    class get_record_len:   public wdb::primitive<wg_int,void*,void*> { public: get_record_len(); };

    class ymd_to_date: public wdb::primitive<int,void*,int,int,int> { public: ymd_to_date(); };
    class hms_to_time: public wdb::primitive<int,void*,int,int,int,int> { public: hms_to_time(); };
    class date_to_ymd: public wdb::primitive<void,void*,int,int*,int*,int*> { public: date_to_ymd(); };
    class time_to_hms: public wdb::primitive<void,void*,int,int*,int*,int*,int*> { public: time_to_hms(); };

    class make_query: public wdb::primitive<wg_query*,void*,void*,wg_int,wg_query_arg*,wg_int> { public: make_query(); };
    class fetch_query: public wdb::primitive<void*,void*,wg_query*> { public: fetch_query(); };
    class destroy_query: public wdb::primitive<void,void*,wg_query*> { public: destroy_query(); };
    class encode_query_param_null:  public wdb::primitive<wg_int,void*,const char*> { public: encode_query_param_null(); };
    class encode_query_param_record: public wdb::primitive<wg_int,void*,void*> { public: encode_query_param_record(); };
    class encode_query_param_char: public wdb::primitive<wg_int,void*,char> { public: encode_query_param_char(); };
    class encode_query_param_fixpoint: public wdb::primitive<wg_int,void*,double> { public: encode_query_param_fixpoint(); };
    class encode_query_param_date: public wdb::primitive<wg_int,void*,int> { public: encode_query_param_date(); };
    class encode_query_param_time: public wdb::primitive<wg_int,void*,int> { public: encode_query_param_time(); };
    class encode_query_param_var:  public wdb::primitive<wg_int,void*,wg_int> { public: encode_query_param_var(); };
    class encode_query_param_int:  public wdb::primitive<wg_int,void*,wg_int> { public: encode_query_param_int(); };
    class encode_query_param_double: public wdb::primitive<wg_int,void*,double> { public: encode_query_param_double(); };
    class encode_query_param_str: public wdb::primitive<wg_int,void*,const char*,const char*> { public: encode_query_param_str(); };
    class free_query_param: public wdb::primitive<wg_int,void*,wg_int> { public: free_query_param(); };

    class Date final { 
    public: 
        Date();
        Date(int y, int m, int d);
        Date(int date);
        int get() const;
        int get(int& y, int& m, int& d) const;
        void set(int date);
        bool operator == (Date const& d) const;
    private:
        int date;
    };
    class Time final {
    public: 
        Time();
        Time(int t);
        Time(int h, int m, int s);
        int get() const;
        int get(int& h, int& m, int& s) const;
        void set(int time);
        bool operator == (Time const& d) const;
    private:    
        int time;
    };
    
    class Null final { 
    public:
        Null(wg_int p=0):data(0){ } 
        wg_int get() { return data; } 
        bool isNull() const { return data == 0; }
    private: 
        wg_int data; 
    };
    
    class Blob final {
    public:
        Blob();
        ~Blob();
        Blob(const wdb::byte* p, size_t len, const wdb::byte* type=nullptr);
        Blob(Blob&& b);
        Blob& operator = (Blob&& b);
        const wdb::byte* get() const;
        const wdb::byte* type() const;
        size_t size() const;
        size_t type_size() const;
        size_t loadFromFile(const std::string& fileName, wdb::byte* type=nullptr);
    private:
        Blob(const Blob&) = delete;
        Blob& operator = (const Blob&) = delete;
        wdb::bytes _buffer;
        wdb::byte* _type;
    };

    class Var {
    public:
        Var(const wg_int id=0);
        Var(const Var& var);
        wg_int id() const;
        operator wg_int () const;
    private:
        wg_int _id;
    };

    template<class Type_t>
    class codec {
    public:
        template<class...ARGS> wg_int encode(void* db, Type_t data,ARGS...args);
        template<class...ARGS> Type_t decode(void* db, wg_int data,ARGS...args);
    };

    template<> class codec<wdb::Null> {
        public:
        wg_int encode(void* db, Null data=Null()) { return encode_null()(db,wg_int(0)); }
        Null decode(void* db, wg_int data) { return Null(decode_null()(db,data)); }
    };

    template<> class codec<wg_int> {
    public:
        wg_int encode(void* db, wg_int data) { return encode_int()(db,data); }
        wg_int decode(void* db, wg_int data) { return decode_int()(db,data); }
    };

    template<> class codec<int> {
    public:
        wg_int encode(void* db, int data) { return encode_int()(db,(wg_int)data); }
        int decode(void* db, wg_int data) { return (int)decode_int()(db,data); }
    };
    
    template<> class codec<double> {
    public:
        wg_int encode(void* db, double data) { return encode_double()(db,data); }
        double decode(void* db, wg_int data) { return decode_double()(db,data); }
    };
    
    template<> class codec<char*> {
    public:
        wg_int encode(void* db, char* data, char* lang) { return encode_str()(db,data,lang); }
        char*  decode(void* db, wg_int data) { return decode_str()(db,data); }
    };

    template<> class codec<const char*> {
    public:
        wg_int encode(void* db, const char* data, const char* lang) { 
            return encode_str()(db,const_cast<char*>(data),const_cast<char*>(lang)); 
        }
        const char*  decode(void* db, wg_int data) { return decode_str()(db,data); }
    };

    template<> class codec<Date> {
    public:
        wg_int encode(void* db, Date& data) { return encode_date()(db,data.get()); }
        Date   decode(void* db, wg_int data) { 
            int y,m,d, date;
            date = decode_date()(db,data); 
            return Date(date);
        }
    };

    template<> class codec<Time> {
    public:
        wg_int encode(void* db, Time& data) { return encode_time()(db,data.get()); }
        Time   decode(void* db, wg_int data) { 
            return Time(decode_time()(db,data));
        }
    };

    template<> class codec<wdb::Blob> {
    public:
        wg_int encode(void* db, wdb::Blob& blob) const
        {  
            const char* payload = reinterpret_cast<const char*>(blob.get());
            const char* type = reinterpret_cast<const char*>(blob.type());
            return encode_blob()(db, const_cast<char*>(payload), const_cast<char*>(type), blob.size());
        }
        wdb::Blob decode(void* db, wg_int data) { 
            char* payload = decode_blob()(db, data);
            wg_int len = decode_blob_len()(db, data);
            char* type = decode_blob_type()(db, data);
            wg_int type_len = decode_blob_type_len()(db, data);

            return Blob(reinterpret_cast<wdb::byte*>(payload), len, reinterpret_cast<wdb::byte*>(type));
        }
    };

    template<> class codec<wdb::Var> {
    public:
        wg_int encode(void* db, const wdb::Var& var) const
        {  
            const wg_int id = static_cast<wg_int>(var);
            return encode_var_type()(db, id);
        }
        wg_int decode(void* db, wg_int data) { 
            return decode_var_type()(db, data);;
        }
    };

    typedef enum eWG_FieldType {
        eWG_NULLTYPE        = WG_NULLTYPE,
        eWG_RECORDTYPE      = WG_RECORDTYPE,
        eWG_INTTYPE         = WG_INTTYPE,
        eWG_DOUBLETYPE      = WG_DOUBLETYPE,
        eWG_STRTYPE         = WG_STRTYPE,
        eWG_XMLLITERALTYPE  = WG_XMLLITERALTYPE,
        eWG_URITYPE         = WG_URITYPE,
        eWG_BLOBTYPE        = WG_BLOBTYPE,
        eWG_CHARTYPE        = WG_CHARTYPE,
        eWG_FIXPOINTTYPE    = WG_FIXPOINTTYPE,
        eWG_DATETYPE        = WG_DATETYPE,
        eWG_TIMETYPE        = WG_TIMETYPE,
        eWG_ANONCONSTTYPE   = WG_ANONCONSTTYPE,
        eWG_VARTYPE         = WG_VARTYPE
    } WG_FieldType;

    class UnitaryType {};
    class NullType: public UnitaryType {};
    class NumericType: public UnitaryType {};
    class StringType: public UnitaryType {};
    class DateTimeType: public NumericType {};
    class CompositeType{};
    class RecordType: public CompositeType {};

    template<class Type>
    class TypeCategory {
        public: typedef typename Type::type_category  type_category;
    };
    

    template<> class TypeCategory<wg_int>{ public: typedef NumericType type_category; };
    template<> class TypeCategory<int>{ public: typedef NumericType type_category; };
    template<> class TypeCategory<double>{ public: typedef NumericType type_category; };
    template<> class TypeCategory<char*>{ public: typedef StringType type_category; };
    template<> class TypeCategory<const char*>{ public: typedef StringType type_category; };
    template<> class TypeCategory<Date>{ public: typedef NumericType type_category; };
    template<> class TypeCategory<Time>{ public: typedef DateTimeType type_category; };
    template<> class TypeCategory<Null>{ public: typedef NullType type_category; };
    template<> class TypeCategory<Blob>{ public: typedef StringType type_category; };

    template <class Type_t>
    struct is_unitary_type {
        static constexpr bool value = std::is_base_of<UnitaryType, typename TypeCategory<Type_t>::type_category>::value;
    };

    static_assert(is_unitary_type<wg_int>::value, "should be true");

    template<class Type_t> class Traits {
        public:
        typedef Type_t type;
        typedef codec<Type_t> codec_t;
        typedef typename Type_t::encode_query_param encode_param;
    };
    template<> class Traits<wg_int>{ public: typedef wg_int type; typedef codec<wg_int> codec_t; typedef encode_query_param_int encode_param; };
    template<> class Traits<int>{ public: typedef int type; typedef codec<wg_int> codec_t; typedef encode_query_param_int encode_param; };
    template<> class Traits<double>{ public: typedef double type; typedef codec<double> codec_t; typedef encode_query_param_double encode_param; };
    template<> class Traits<char*>{ public: typedef char* type; typedef codec<char*> codec_t; typedef encode_query_param_str encode_param; };
    template<> class Traits<const char*>{ public: typedef const char* type; typedef codec<const char*> codec_t; typedef encode_query_param_str encode_param; };
    template<> class Traits<Date>{ public: typedef Date type; typedef codec<Date> codec_t; typedef encode_query_param_date encode_param; };
    template<> class Traits<Time>{ public: typedef Time type; typedef codec<Time> codec_t; typedef encode_query_param_time encode_param; };
    template<> class Traits<Null>{ public: typedef Null type; typedef codec<Null> codec_t; typedef encode_query_param_null encode_param; };
    template<> class Traits<Blob>{ public: typedef Blob type; typedef codec<Blob> codec_t; };
    template<> class Traits<Var>{ public: typedef Var type; typedef codec<Var> codec_t; typedef encode_query_param_var encode_param; };

    template<class Impl_t, class IteratedType_t>
    class iterator {
    public:
        typedef Impl_t  impl_t;
        typedef IteratedType_t type_t;
        typedef iterator<Impl_t, IteratedType_t> myit_t;

        impl_t& operator = (const iterator& i){
            if (static_cast<const impl_t*>(&i)!=static_cast<impl_t*>(this)){
                static_cast<impl_t*>(this)->assign(static_cast<const impl_t&>(i));
            }
            return static_cast<impl_t&>(*this);
        }
        bool operator == (const impl_t& i) const {
            return static_cast<const impl_t*>(this)->equal(i);
        }
        bool operator != (const impl_t& i) const {
            return !static_cast<const impl_t*>(this)->equal(i);
        }
        impl_t operator ++ (){
            static_cast<impl_t*>(this)->next();
            return static_cast<impl_t&>(*this);
        }
        Impl_t operator ++ (int){
            Impl_t i(static_cast<Impl_t&>(*this));
            static_cast<impl_t*>(this)->next();
            return i;
        }
        type_t operator * (){
            return static_cast<impl_t*>(this)->data();
        }
        protected:
        iterator(){
            static_assert(std::is_base_of<myit_t,Impl_t>::value,
            "First template parameter \"class Impl_t\" must be child class of wdb::iterator<class Impl_t,...>");
        }
        iterator(const iterator& i){
            static_cast<impl_t*>(this)->assign(static_cast<const impl_t&>(i));
        }
    };

    template <class Database_t>
    class Field{
        public:
        friend Database_t;
        Field(): _db(nullptr), _rec(nullptr), _index(0) {}
        Field operator = (const Field& f) {
            if(&f!=this){
                _db  = f._db;
                _rec = f._rec;
                _index = f._index;
            }
            return Field(*this);
        }
        Field operator = (Field&& f) {
            if(&f!=this){
                _db  = f._db;
                _rec = f._rec;
                _index = f._index;
                f._db = nullptr;
                f._rec = nullptr;
                f._index = 0;
            }
            return Field(*this);
        }
        bool operator == (const Field& f) const {
            return (_db->checkDb(f) && _rec == f._rec && _index == f._index);
        }
        bool operator != (const Field& f) const {
            return (!_db->checkDb(f) || _rec != f._rec || _index != f._index);
        }
        Field(const Field& f): _db(f._db), _rec(f._rec), _index(f._index){}
        Field(Field&& f): _db(f._db), _rec(f._rec), _index(f._index){ f._db = nullptr; f._rec = nullptr; _index = 0; }

        template<class Type_t>
        Type_t value() const{
            get_field get;
            return  _db->template decodeField<Type_t>( _db->callPrimitive(get,_rec, _index) );
        }
        WG_FieldType type() const{
            wdb::get_field_type get_type;
            return (WG_FieldType) _db->callPrimitive(get_type,_rec,_index);
        }
        Field next(){
            if (isEnd())
                return  *this;
            return Field(_db,_rec,_index+1);
        }
        bool isEnd() const {
            get_record_len rec_len;
            return _db->callPrimitive(rec_len,_rec) <= _index;
        }
        Field end() const {
            get_record_len rec_len;
            return Field(_db, _rec, _db->callPrimitive(rec_len,_rec));
        }
        bool isValid() const {
            return _db != nullptr && _rec != nullptr;
        }
    private:
        Field(Database_t* db, void* rec, uint index): _db(db), _rec(rec), _index(index){}

        Database_t* _db;
        void *_rec;
        long _index;
    };

    template <class Database_t, uint nbFields>
    class Record {
    public:
        typedef encode_query_param_record encode_query_param;
        typedef RecordType  type_category;

        class iterator: public wdb::iterator<iterator,Field<Database_t>> {
        public:
            iterator(){}
        private:
            friend Record<Database_t, nbFields>;
            friend wdb::iterator<iterator,Field<Database_t>>;
            wdb::Field<Database_t> data(){
                return _f;
            }
            wdb::Field<Database_t> next(){
                return (_f = _f.next());
            }
            void assign(const iterator& i){
                _f = i._f;
            }
            bool equal(const iterator& i) const {
                return _f == i._f;
            }
            iterator(wdb::Field<Database_t>& f) { _f = f; }
            iterator(wdb::Field<Database_t>&& f) { _f = f; }
            Field<Database_t> _f;
        };
        Record(Record&& r): _db(r._db),  _rec(r._rec) { r._rec = nullptr; }
        Record(const Record& r): _db(r._db),  _rec(r._rec) {}
        Record operator = (const Record& r){
            if(&r!=this){
                _db = r._db;
                _rec = r._rec;
            }
            return *this;
        }

        template<wg_int pos>
        Field<Database_t> getField(){
            static_assert(pos < nbFields, "Record field position always lesser than fields count");
            return _db->template getRecordField<nbFields,pos>(*this);
        }

        template<class Type_t,wg_int pos>
        Type_t getFieldVal() const{
            static_assert(pos < nbFields, "Record field position always lesser than fields count");
            return _db->template getRecordField<nbFields,pos>(*this).template value<Type_t>();
        }
        template<class Type_t,wg_int pos,class ...ARGS>
        wg_int setField(Type_t& data, ARGS...args) {
            static_assert(pos < nbFields, "Record field position always lesser than fields count");
            return _db->template setRecordField<nbFields,Type_t>(*this,data,pos,args...);
        }

        template<class Type_t,wg_int pos,class ...ARGS>
        wg_int setField(Type_t&& data, ARGS...args) {
            static_assert(pos < nbFields, "Record field position always lesser than fields count");
            return _db->template setRecordField<nbFields,Type_t>(*this,data,pos,args...);
        }

        iterator   begin() const {  return iterator(_db->template fieldIterator<nbFields>(*this) );  }
        iterator   end()   const {  return iterator(_db->template fieldIterator<nbFields>(*this).end() );  }

    private:
        friend Database_t;
        friend codec<Record<Database_t,nbFields>>;
        void* _rec;
        Database_t* _db;
        Record(Database_t* db, void *rec): _rec(rec), _db(db){};
        Record() = delete;
    };

    template<class Database_t,uint nbFields> 
    class codec<Record<Database_t,nbFields>> {
    public:
        wg_int encode(void* db, Record<Database_t,nbFields>& data) { return primitive<wg_int,void*,void*>(&wg_encode_record)(db,data._rec); }
        void*   decode(void* db, wg_int data) { 
            return primitive<void*,void*,wg_int>(&wg_decode_record)(db,data); 
        }
    };

    template<class Database_t,wg_int NbFields>
    class TypeCategory<Record<Database_t,NbFields>>{
        public: typedef RecordType type_category;
    };

    template<class Database_t>
    class BasicRecord {
    public:
        typedef encode_query_param_record encode_query_param;
        typedef RecordType  type_category;

        class iter: public wdb::iterator<iter,wdb::Field<Database_t>>{
        private:
            friend BasicRecord<Database_t>;
            friend wdb::iterator<iter,wdb::Field<Database_t>>;
            wdb::Field<Database_t> data(){
                return _f;
            }
            wdb::Field<Database_t> next(){
                return (_f = _f.next());
            }
            void assign(const iter& i){
                _f = i._f;
            }
            bool equal(const iter& i) const {
                return _f == i._f;
            }
            iter(wdb::Field<Database_t>& f) { _f = f; }
            wdb::Field<Database_t> _f;
        public:
            iter(){}
        };
        typedef iter iterator;

        BasicRecord(): _db(nullptr), _rec(nullptr) {}
        BasicRecord(const BasicRecord& r): _db(r._db), _rec(r._rec) { }
        BasicRecord& operator = (const BasicRecord& r){
            if (this!=&r){
                _db = r._db;
                _rec = r._rec;
            }
            return *this;
        }
        bool operator == (const BasicRecord& r) const {
            return _db == r._db && _rec == r._rec;
        }
        bool operator != (const BasicRecord& r) const {
            return _db != r._db || _rec != r._rec;
        }
        iter begin(){ 
            wdb::Field<Database_t> f = _db->fieldIterator(*this); 
            return iter( f );
        }
        iter end(){ 
            wdb::Field<Database_t> f = _db->fieldIterator(*this).end(); 
            return iter( f ); 
        }
        Field<Database_t> getField(wg_int index) const{
            get_record_len primitive;
            wg_int rec_len = _db->callPrimitive(primitive, _rec);
            return _db->getRecordField(*this, index, rec_len);
        }
    private:
        friend Database_t;
        friend codec<BasicRecord>;
        friend BasicRecord::iter;
        BasicRecord(Database_t* db, void* rec): _db(db), _rec(rec){}
        BasicRecord next() {
            if (_db && _rec){
                _rec = _db->nextRecord(*this)._rec;
            }
            return BasicRecord(_db, nullptr); 
        }
        void *_rec;
        Database_t* _db;
    };

    template<class Database_t> 
    class codec<BasicRecord<Database_t>> {
    public:
        typedef BasicRecord<Database_t> myrec_t;
        wg_int encode(void* db, myrec_t& data) { return primitive<wg_int,void*,void*>(&wg_encode_record)(db,data._rec); }
        void*  decode(void* db, wg_int data) {
            return wdb::primitive<void*,void*,wg_int>(&wg_decode_record)(db,data);
        }
    };

    template<class Database_t>
    class TypeCategory<BasicRecord<Database_t>>{
        public: typedef RecordType type_category;
    };

    template<class Type_t, wg_int iCondition, wg_int iColumn>
    class Condition{
    public:
        Condition(const Type_t t): _t(t) {}
        wg_int op() const { return iCondition; }
        wg_int column() const { return iColumn; }
        Type_t& value() { return _t; }
        size_t serialize(wdb::bytes& b, wg_int encoded_val){

            wg_query_arg qarg {iColumn, iCondition, encoded_val};
            const wdb::byte* ptr = reinterpret_cast<const wdb::byte*>(&qarg);
            std::copy(ptr ,ptr + sizeof(qarg) ,std::back_inserter(b));
            return sizeof(qarg);
        }
    private:
        Type_t _t;
    };

    template<class Type_t, wg_int iColumn>
    using EqualTo = Condition<Type_t, WG_COND_EQUAL, iColumn>;

    template<class Type_t, wg_int iColumn>
    using NotEqualTo = Condition<Type_t, WG_COND_NOT_EQUAL, iColumn>;

    template<class Type_t, wg_int iColumn>
    using Less = Condition<Type_t, WG_COND_LESSTHAN, iColumn>;

    template<class Type_t, wg_int iColumn>
    using LessEqual = Condition<Type_t, WG_COND_LTEQUAL, iColumn>;

    template<class Type_t, wg_int iColumn>
    using Greater = Condition<Type_t, WG_COND_GREATER, iColumn>;

    template<class Type_t, wg_int iColumn>
    using GreaterEqual = Condition<Type_t, WG_COND_GTEQUAL, iColumn>;
    
    template <class Database_t>
    class Query {
      public:

        class iterator: public wdb::iterator<Query::iterator,wdb::BasicRecord<Database_t>>{
        private:
            friend BasicRecord<Database_t>;
            friend wdb::iterator<iterator,wdb::BasicRecord<Database_t>>;
            friend wdb::Query<Database_t>;

            wdb::BasicRecord<Database_t> data(){
                return _r;
            }
            wdb::BasicRecord<Database_t> next(){

                return (_r = _db->fetch(*_q));
            }
            void assign(const iterator& i){
                _r = i._r;
            }
            bool equal(const iterator& i) const {
                return (_q == i._q && _r == i._r);
            }
            iterator(Database_t* db, Query* q, wdb::BasicRecord<Database_t> r): _q(q), _db(db){
                _r = r;
            }
            iterator(Database_t* db, Query* q): _q(q), _db(db){
                _r =_db->template createRecord<wdb::BasicRecord<Database_t>>(nullptr);
            }
            wdb::BasicRecord<Database_t> _r;
            wdb::Query<Database_t>* _q;
            Database_t* _db;
        };

        Query(): _db(nullptr), _q(nullptr), _nb_args(0) {}
        ~Query() {
            close();
        }
        Query(Query&& q): _db(q._db), _args(std::move(q._args)), _nb_args(0) { q._db = nullptr; }
        Query& operator = (Query&& q) { 
            close();
            _db = q._db;
            _q = q._q;
            _args = std::move(q._args);
            _nb_args =  q._nb_args;
            q._args.clear();
            q._db = q._q = nullptr;
            q._nb_args = 0;
        }

        template<class Type_t, wg_int iCondition, wg_int iColumn, class...ARGS>
        Query<Database_t>& add (Condition<Type_t,iCondition,iColumn>& cond, ARGS...args){
            return add(cond).add(args...);
        }
        template<class Type_t, wg_int iCondition, wg_int iColumn, class...ARGS>
        Query<Database_t>& add (Condition<Type_t,iCondition,iColumn>&& cond, ARGS...args){
            return add(cond).add(args...);
        }

        template<class Type_t, wg_int iCondition, wg_int iColumn>
        Query<Database_t>& add (Condition<Type_t,iCondition,iColumn>&& cond){
            return add(cond);
        }
        template<class Type_t, wg_int iCondition, wg_int iColumn>
        Query<Database_t>& add (Condition<Type_t,iCondition,iColumn>& cond){
            if (_db){
               wg_int encoded = _db->template encodeParam<Type_t>( cond.value() );
               cond.serialize(_args, encoded);
               _nb_args += 1;
            }
            return *this;
        }
        bool exec(){
            if (_q == nullptr){
                _q = _db->makeQuery(*this);
            }
            return _q != nullptr;
        }
        void clear() { _args.clear(); }
        void close() {
            if (_db) {
                _db->destroyQuery(*this);
            }
        }
        Query<Database_t>::iterator begin(){ return iterator(_db,this,_db->fetch(*this)); }
        Query<Database_t>::iterator end(){ return iterator(_db,this); }
      private:

        Query(Database_t* db): _db(db), _q(nullptr), _nb_args(0) {}
        Query(const Query&) = delete;
        Query& operator = (const Query&) = delete;

        friend Database_t;
        Database_t* _db;
        wdb::bytes _args;
        wg_query* _q;
        int _nb_args;
    };

    template<template <class D, unsigned n> class Type_t, class Database_t, unsigned int nbFields=0>
    struct is_record {
        static constexpr wg_int n = static_cast<wg_int>(nbFields);
        static constexpr bool value = std::is_same<Type_t<Database_t,n>, Record<Database_t,n>>::value;
    };

    template <class Type_t, class Database_t, bool unitary_type = is_unitary_type<Type_t>::value>
    class __CodecDbAdapter__;

    template <class Type_t, class Database_t>
    class __CodecDbAdapter__<Type_t,Database_t,true>{
        public:
        template<class...ARGS>
        wg_int encode(Database_t& db, void* dbhandle, Type_t& data, ARGS...args){
            typename wdb::Traits<Type_t>::codec_t encoder;
            return encoder.encode(dbhandle,data,args...);
        }
        template<class...ARGS>
        Type_t decode(Database_t& db, void* dbhandle, wg_int f, ARGS...args){
            typename wdb::Traits<Type_t>::codec_t decoder;
            return decoder.decode(dbhandle,f,args...);
        }
    };

    template <class Record_t, class Database_t>
    class __CodecDbAdapter__<Record_t,Database_t,false> {
        public:
        wg_int encode(Database_t& db, void* dbhandle, Record_t& data){
            typename wdb::Traits<Record_t>::codec_t encoder;
            return encoder.encode(dbhandle,data);
        }
        Record_t decode(Database_t& db, void* dbhandle, wg_int f){
            typename wdb::Traits<Record_t>::codec_t decoder;
            return db.template createRecord<Record_t>(decoder.decode(dbhandle,f));
        }
    };

    template<class DatabaseImpl_t>
    class PrimitiveDatabase {
        public:

        typedef PrimitiveDatabase<DatabaseImpl_t> PrimDb_t;
        typedef DatabaseImpl_t  DbType_t;
        typedef wdb::Field<PrimDb_t> Field;
        typedef BasicRecord<PrimDb_t> BasicRecord_t;
        typedef Query<PrimDb_t> Query_t;
        template<wg_int NbFields>
        using Record = wdb::Record<PrimDb_t,NbFields>;

        PrimitiveDatabase(DatabaseImpl_t& db): _db(db){
            static_assert(std::is_base_of<PrimitiveDatabase,DatabaseImpl_t>::value, 
                "PrimitiveDatabase type should be base class of its database implem template parameter");
        }

        template <class Ret, class...ARGS>
        Ret callPrimitive(primitive<Ret,void*,ARGS...>& p, ARGS...args){
            return p(_db.handle(), args...);
        } 
        template <wg_int NbFields>
        Record<NbFields> createRecord(){
            wdb::create_record create;
            void* rec = create(_db.handle(), NbFields);
            return Record<NbFields>(this,rec);
        }
      
        template <class Record_t>
        Record_t createRecord(void* rec){
            return Record_t(this,rec);
        }

        template <wg_int NbFields>
        wg_int deleteRecord(Record<NbFields>& rec){
            if (checkDb(rec)){
                wdb::delete_record del;
                return del(_db.handle(), rec._rec);
            }   
            return -1;
        }

        wg_int deleteRecord(BasicRecord_t& rec){
            if (checkDb(rec)){
                wdb::delete_record del;
                return del(_db.handle(), rec._rec);
            }
            return -1;
        }

        template <class Type_t,class...ARGS>
        wg_int  encodeField(Type_t& data,ARGS...args){
            __CodecDbAdapter__<Type_t,PrimDb_t> encoder;
            return encoder.encode(*this,_db.handle(),data,args...);
        }

        template <class Type_t, class...ARGS>
        Type_t  decodeField(wg_int f,ARGS...args){
            __CodecDbAdapter__<Type_t,PrimDb_t> decoder;
            return decoder.decode(*this,_db.handle(),f,args...);
        }

        template <wg_int NbFields, class Type_t,class...ARGS>
        wg_int  setRecordField(Record<NbFields>& rec, Type_t& data, wg_int field_pos,ARGS...args){
            wg_int data_encoded = encodeField<Type_t>(data,args...);
            wdb::set_field set;
            return set(_db.handle(), rec._rec, field_pos, data_encoded);
        }

        template <wg_int NbFields, wg_int field_pos>
        Field  getRecordField(const Record<NbFields>& rec){
            wdb::get_field get;
            wg_int field_encoded = get(_db.handle(), rec._rec, field_pos);
            // TODO: error checking
            return Field(rec._db,rec._rec,field_pos);
        }

        Field getRecordField(const BasicRecord_t& rec, wg_int field_pos, wg_int record_len){

            if (rec._db && rec._db->_db.handle() == _db.handle()){
                if (field_pos > -1 && field_pos < record_len){
                    return Field(this, rec._rec, field_pos);
                }
            }
            
            return Field(rec._db, rec._rec, record_len);
        }

        BasicRecord_t nextRecord(const BasicRecord_t& rec){
           return BasicRecord_t(this, next_record()(_db.handle(), rec._rec));
        }
        template <wg_int NbFields>
        Field fieldIterator(const Record<NbFields>& rec) {
            return Field(this,rec._rec,0);
        }
        Field fieldIterator(const BasicRecord_t& rec) {
           return Field(this,rec._rec,0);
        }
        Query_t createQuery(){
            return Query_t(this);
        }
        wg_query* makeQuery(Query_t& q){
            if (checkDb(q)  && !q._args.empty()){
                wg_query_arg* args = const_cast<wg_query_arg*>(reinterpret_cast<const wg_query_arg*>(q._args.data()));
                return make_query()(_db.handle(), NULL, 0, args, q._nb_args);
            }
            return nullptr;
        }

        BasicRecord_t fetch(Query_t& q){
            if (checkDb(q) && q._q){
                void* rec = fetch_query()(_db.handle(),q._q);
                if (rec){
                    return createRecord<BasicRecord_t>(rec);
                }
            }
            return createRecord<BasicRecord_t>(nullptr);
        }
        void destroyQuery(Query_t& q){
            if (q._q && checkDb(q)){
                wg_query_arg* args = const_cast<wg_query_arg*>(reinterpret_cast<const wg_query_arg*>(q._args.data()));
                free_query_param freer;
                for (uint i=0; i < q._nb_args; i+=1){
                    freer(_db.handle(), args[i].value);
                }
                destroy_query()(_db.handle(),q._q);
                q._q = nullptr;;
                q._db = nullptr;
                q._nb_args = 0;
                q.clear();
            }

        }
        template <class Type_t>
        wg_int encodeParam(Type_t& t){
            typename wdb::Traits<Type_t>::encode_param encoder;
            return encoder( _db.handle(), t );
        }
        template<class Object_t>
        bool checkDb(const Object_t& obj) const{
            return obj._db && obj._db->_db.handle() == _db.handle();
        }
        private:

        class iterator: public wdb::iterator<
            iterator,
            wdb::BasicRecord<PrimDb_t>> 
        {
        public:
            typedef wdb::iterator<iterator,wdb::BasicRecord<PrimDb_t>> base_t;
            iterator() {}
        private:
            typedef BasicRecord_t it_t; 

            iterator(BasicRecord_t& r){ _r = r; }
            void assign(const iterator& i){
                _r = i._r;
            }
            bool equal(const iterator& i) const {
                return _r == i._r;
            }
            it_t next(){
                return _r.next();
            }
            it_t data(){
                return _r;
            }
            it_t& get() { return _r; }
            friend PrimDb_t;
            friend DbType_t;
            friend base_t;
            wdb::BasicRecord<PrimDb_t> _r;
        };

        template<wg_int NbField>
        class rec_iterator{
        public:
            rec_iterator(const rec_iterator& i) : _db(i._db), _rec_obj(i._rec_obj) {}
            rec_iterator operator = (const rec_iterator& i){
                if(&i!=this){
                    _db = i._db;
                    _rec_obj = i._rec_obj;
                }
                return *this;
            }
            bool operator == (const rec_iterator& i) const {
                return (_db._db.handle()==i._db._db.handle() && _rec_obj._rec==i._rec_obj._rec);
            }
            bool operator != (const rec_iterator& i) const {
                return (_db._db.handle()!=i._db._db.handle() || _rec_obj._rec!=i._rec_obj._rec);
            }
            rec_iterator operator ++ (){
                wdb::next_record next;
                _rec_obj = _db.createRecord<Record<NbField>>(next(_db._db.handle(),_rec_obj._rec));
                return rec_iterator<NbField>(*this);
            }
            rec_iterator operator ++ (int){
                rec_iterator<NbField> i(*this);
                wdb::next_record next;
                _rec_obj = _db.createRecord<Record<NbField>>(next(_db._db.handle(),_rec_obj._rec));
                return i;
            }
            Record<NbField> operator * (){
                return _rec_obj;
            }
            Record<NbField>* operator -> (){
                return & _rec_obj;
            }
        private:
            friend PrimDb_t;
            friend DbType_t;
            rec_iterator(PrimDb_t& db, void* rec): 
                _db(db), _rec_obj(&db,rec){}
            rec_iterator() = delete;
            PrimDb_t& _db;
            Record<NbField> _rec_obj;
        };
        
        friend DatabaseImpl_t;
        DatabaseImpl_t& _db;
    };

    class Index {
    public:
        Index(wg_int id);
        Index(const Index& i);
        const Index& operator = (const Index& i);
        Index(Index&&);
        Index& operator = (Index&&);
        bool operator == (const Index& i) const;
        bool operator != (const Index& i) const;
    private:
        wg_int _id;
    };
}

#endif