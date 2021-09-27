#include "fixture_class.h"

namespace {
    typedef wdb::test::TestDb Db_t;
}

typedef class _RecFixture: 
    public wdb::test::TestFixtureT<Db_t> 
    {
    public:
        void* handle() { return this->db().handle(); }
} Rec_fx;


TEST_F(Rec_fx, CreateBasicRecord){
    Db_t& d = db();

    // create record of 5 fields
    Db_t::Record<5> r = d.createRecord<5>();
    r.setField<const char*,0>("Hello world",(char*) nullptr); // first is char* 
    r.setField<wg_int,1>(1); // 2nd int
    r.setField<wdb::Time,2>(wdb::Time(12,30,00)); // 3rd, time 12h30 PM
    
    // create other record to put in first record 4th field
    Db_t::Record<3> r2 = d.createRecord<3>();
    r2.setField<int,0>(1);
    r2.setField<double,1>(3.14116);
    r2.setField<const char*,2>("Test",(char*)nullptr);
    r.setField<Db_t::Record<3>,3>(r2); // set fourth field with record

    //5th field is Null
    r.setField<wdb::Null,4>(wdb::Null());
    
    Db_t::rec_iterator<5> it = d.begin<5>();
    Db_t::Record<5> rec = *it++;
    Db_t::Record<5>::iterator it_f; it_f = rec.begin();
    ASSERT_EQ((*it_f).type(),wdb::eWG_STRTYPE);
    ASSERT_STREQ("Hello world", (*it_f++).value<const char*>());
    ASSERT_EQ((*it_f).type(),wdb::eWG_INTTYPE);
    ASSERT_EQ(1, (*it_f++).value<int>());
    ASSERT_EQ((*it_f).type(),wdb::eWG_TIMETYPE);
    ASSERT_EQ(wdb::Time(12,30,00), (*it_f++).value<wdb::Time>());
    ASSERT_EQ((*it_f).type(),wdb::eWG_RECORDTYPE);
    r2 = (*it_f++).value<Db_t::Record<3>>();
    ASSERT_EQ(r2.getField<0>().type(),wdb::eWG_INTTYPE);
    ASSERT_EQ( (r2.getFieldVal<int,0>()), 1 );
    ASSERT_EQ(r2.getField<1>().type(),wdb::eWG_DOUBLETYPE);
    ASSERT_EQ((r2.getFieldVal<double,1>()),3.14116);
    ASSERT_EQ(r2.getField<2>().type(),wdb::eWG_STRTYPE);
    ASSERT_STREQ((r2.getFieldVal<const char*,2>()),"Test");
    ASSERT_EQ((*it_f++).type(),wdb::eWG_NULLTYPE);

    ASSERT_EQ(rec.end(),it_f) << "End not reached: Field type is " << (*it_f).type();
    ASSERT_EQ(d.end<5>(), ++it);
    

    Db_t::Record<3> r3 = d.createRecord<3>();
    r3.setField<wdb::Null,0>(wdb::Null());
    r3.setField<wdb::Null,1>(wdb::Null());
    r3.setField<wdb::Null,2>(wdb::Null());

    // basic record
    Db_t::iterator itbasic = d.begin();
    Db_t::BasicRecord_t br;
    Db_t::BasicRecord_t::iterator itfield;
    Db_t::Field f;
    
    ++itbasic;
    ++itbasic;

    br = *itbasic;
    f = br.getField(0);
    ASSERT_EQ(f.type(), wdb::eWG_NULLTYPE);
    ASSERT_TRUE(f.value<wdb::Null>().isNull());
    f = br.getField(1);
    ASSERT_EQ(f.type(), wdb::eWG_NULLTYPE);
    ASSERT_TRUE(f.value<wdb::Null>().isNull());
    f = br.getField(2);
    ASSERT_EQ(f.type(), wdb::eWG_NULLTYPE);
    ASSERT_TRUE(f.value<wdb::Null>().isNull());

}
