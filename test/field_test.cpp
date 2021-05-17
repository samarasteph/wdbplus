#include <string>
#include <exception>
#include "fixture_class.h"

const wdb::bytes blob_string((const wdb::byte*)"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

typedef class _FieldFixture: public wdb::test::TestFixture {} UT_Field;

TEST_F(UT_Field, CreateFields){
    constexpr uint NB_FIELDS = 7;
    WhiteDb::Record<NB_FIELDS> rec = _db.createRecord<NB_FIELDS>();
    wg_int  val_int = 1024;
    double val_double = 1024.0;
    int hr=10,min=45,sec=20;
    int yr=1974, mnth=12, day=31;
    wdb::Time time(hr,min,sec);
    wdb::Date date(yr,mnth,day);
    wdb::Blob blob(blob_string.c_str(),blob_string.size(),nullptr);
    const char* str = "Hello world!";

    rec.setField<wg_int,0>(val_int);
    rec.setField<double,1>(val_double);
    rec.setField<wdb::Time,2>(time);
    rec.setField<wdb::Date,3>(date);
    rec.setField<wdb::Null,4>(wdb::Null());
    rec.setField<wdb::Blob,5>(blob);
    rec.setField<const char*,6>(str,nullptr);

    WhiteDb::Field f;
    f = rec.getField<0>();
    EXPECT_EQ(f.value<wg_int>(), val_int);
    EXPECT_EQ( (rec.getFieldVal<wg_int,0>()) , val_int);
    EXPECT_EQ(f.type(), wdb::eWG_INTTYPE);

    f = rec.getField<1>();
    EXPECT_EQ(f.value<double>(), val_double);
    EXPECT_EQ(f.type(), wdb::eWG_DOUBLETYPE);

    int i1, i2, i3;

    f =  rec.getField<2>();
    ASSERT_EQ(f.value<wdb::Time>().get(), time.get());
    EXPECT_EQ(f.type(), wdb::eWG_TIMETYPE);
    f.value<wdb::Time>().get(i1,i2,i3);
    EXPECT_EQ(i1,hr);
    EXPECT_EQ(i2,min);
    EXPECT_EQ(i3,sec);

    f = rec.getField<3>();
    EXPECT_EQ(f.value<wdb::Date>().get(), date.get());
    EXPECT_EQ(f.type(), wdb::eWG_DATETYPE);
    f.value<wdb::Date>().get(i1,i2,i3);
    EXPECT_EQ(i1,yr);
    EXPECT_EQ(i2,mnth);
    EXPECT_EQ(i3,day);

    f = rec.getField<4>();
    EXPECT_EQ(f.type(), wdb::eWG_NULLTYPE);
    wdb::Null n = f.value<wdb::Null>();
    ASSERT_TRUE(n.isNull());

    f = rec.getField<5>();
    wdb::bytes b1(f.value<wdb::Blob>().get(), f.value<wdb::Blob>().size()), b2(blob.get(), blob.size());
    EXPECT_EQ(f.type(), wdb::eWG_BLOBTYPE);
    ASSERT_EQ(b1, b2);

    f = rec.getField<6>();
    EXPECT_STREQ(str,f.value<char*>());
    EXPECT_EQ(f.type(), wdb::eWG_STRTYPE);
}

TEST_F(UT_Field, ModifyField){
    constexpr uint NB_FIELDS = 7;
    WhiteDb::Record<NB_FIELDS> rec = _db.createRecord<NB_FIELDS>();
    wg_int  val_int = 1024;
    double val_double = 1024.0;
    int hr=10,min=45,sec=20;
    int yr=1974, mnth=12, day=31;
    wdb::Time time(hr,min,sec);
    wdb::Date date(yr,mnth,day);
    wdb::Blob blob(blob_string.c_str(),blob_string.size(),nullptr);
    const char* str = "Hello world!";

    rec.setField<wg_int,0>(val_int);
    rec.setField<double,1>(val_double);
    rec.setField<wdb::Time,2>(time);
    rec.setField<wdb::Date,3>(date);
    rec.setField<wdb::Null,4>(wdb::Null());
    rec.setField<wdb::Blob,5>(blob);
    rec.setField<const char*,6>(str,nullptr);


    WhiteDb::Field f;
    f = rec.getField<0>();
    EXPECT_EQ(f.value<wg_int>(), val_int);
    EXPECT_EQ( (rec.getFieldVal<wg_int,0>()) , val_int);
    EXPECT_EQ(f.type(), wdb::eWG_INTTYPE);

    rec.setField<int,0>(val_int*2);
    EXPECT_EQ(f.value<wg_int>(), val_int*2);

    f = rec.getField<1>();
    EXPECT_EQ(f.value<double>(), val_double);
    EXPECT_EQ(f.type(), wdb::eWG_DOUBLETYPE);
    
    rec.setField<double,1>(val_double*2);
    EXPECT_EQ(f.value<double>(), val_double*2);

    int i1, i2, i3;

    f =  rec.getField<2>();
    ASSERT_EQ(f.value<wdb::Time>().get(), time.get());
    EXPECT_EQ(f.type(), wdb::eWG_TIMETYPE);
    f.value<wdb::Time>().get(i1,i2,i3);
    EXPECT_EQ(i1,hr);
    EXPECT_EQ(i2,min);
    EXPECT_EQ(i3,sec);

    f = rec.getField<3>();
    EXPECT_EQ(f.value<wdb::Date>().get(), date.get());
    EXPECT_EQ(f.type(), wdb::eWG_DATETYPE);
    f.value<wdb::Date>().get(i1,i2,i3);
    EXPECT_EQ(i1,yr);
    EXPECT_EQ(i2,mnth);
    EXPECT_EQ(i3,day);

    f = rec.getField<4>();
    EXPECT_EQ(f.type(), wdb::eWG_NULLTYPE);
    wdb::Null n = f.value<wdb::Null>();
    ASSERT_TRUE(n.isNull());

    f = rec.getField<5>();
    wdb::bytes b1(f.value<wdb::Blob>().get(), f.value<wdb::Blob>().size()), b2(blob.get(), blob.size());
    EXPECT_EQ(f.type(), wdb::eWG_BLOBTYPE);
    ASSERT_EQ(b1, b2);

    f = rec.getField<6>();
    EXPECT_STREQ(str,f.value<char*>());
    EXPECT_EQ(f.type(), wdb::eWG_STRTYPE);

}