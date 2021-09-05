#include "fixture_class.h"
#include <vector>
#include <map>

typedef class _TypesFixture: 
    public wdb::test::TestFixtureT<wdb::test::TestDb> 
    {
    public:
        void* handle() { return this->db().handle(); }
    } UT_Types;


TEST_F(UT_Types,integer){

    wdb::Traits<int>::type t = 100;
    wdb::Traits<int>::codec cod;
    wg_int encoded = cod.encode(handle(),t);
    ASSERT_EQ(cod.decode(handle(), encoded), t);

}

TEST_F(UT_Types,double){

    wdb::Traits<double>::type d = 100.;
    wdb::Traits<double>::codec cod;
    wg_int encoded = cod.encode(handle(),d);
    ASSERT_EQ(cod.decode(handle(), encoded), d);
}

TEST_F(UT_Types,string){
    const wdb::Traits<const char*>::type c = "Hello world!";
    wdb::Traits<const char*>::codec cod;
    wg_int encoded = cod.encode(handle(),c, nullptr);
    ASSERT_STREQ(cod.decode(handle(),encoded),c);
}

TEST_F(UT_Types,date){
    wdb::Traits<wdb::Date>::type date1(2021,5,8);
    wdb::Traits<wdb::Date>::codec cod;
    wg_int encoded = cod.encode(handle(),date1);
    wdb::Date date2 = cod.decode(handle(),encoded);
    int y1,y2,m1,m2,d1,d2;

    date1.get(y1,m1,d1);
    date2.get(y2,m2,d2);
    ASSERT_EQ(y1,y2);
    ASSERT_EQ(m1,m2);
    ASSERT_EQ(d1,d2);
    ASSERT_EQ(date1.get(),date2.get());

    wdb::Date date3;
    date3.set(date2.get());
    date3.get(y1,m1,d1);

    ASSERT_EQ(y1,y2);
    ASSERT_EQ(m1,m2);
    ASSERT_EQ(d1,d2);
}

TEST_F(UT_Types,dateCopy){
    
    typedef wdb::Traits<wdb::Date>::type Date_t;
    typedef wdb::Traits<wdb::Date>::codec Codec_t;
    std::vector<Date_t> dates;

    const int year=2021;
    std::map<int,int> days_per_months = {{ 1, 31}, { 2, 28 }, {3, 31}, {4, 30}, {5, 31}, {6, 30}, {7, 31}, {8, 31}, {9, 30}, {10, 31}, {11, 30}, {12, 31}};

    for (int i=0; i < 2; i+=1){
        std::for_each(std::cbegin(days_per_months), std::cend(days_per_months), [i,year,&dates](const std::pair<const int,int>& item){
            for (int k=1; k<=item.second; k+=1){
                dates.emplace_back(year+i,item.first,k);
            }

        }); 
    }

    std::map<int,int>::const_iterator cit = std::begin(days_per_months);
    int day = 1, i = 0;
    std::for_each(std::begin(dates), std::end(dates),[&i,&day,&cit,&days_per_months](const Date_t& date){
        Date_t copy = date;
        
        int y,m,d;
        copy.get(y,m,d);
        std::cout << y << "-" << m << "-" << d << std::endl;
        ASSERT_EQ(year+i,y);
        ASSERT_EQ(cit->first,m);
        ASSERT_EQ(day,d);

        day = (day+1);
        if (day>cit->second){
            ++cit;
            day=1;
            if (cit==std::end(days_per_months)){
                i+=1;
                cit = std::begin(days_per_months);
            }
        }
    });
}

TEST_F(UT_Types,time){
    wdb::Traits<wdb::Time>::type time1(12,15,20);
    wdb::Traits<wdb::Time>::codec cod;
    wg_int encoded = cod.encode(handle(),time1);
    wdb::Time time2 = cod.decode(handle(),encoded);
    int h1,h2,m1,m2,s1,s2;

    time1.get(h1,m1,s1);
    time2.get(h2,m2,s2);
    ASSERT_EQ(h1,h2);
    ASSERT_EQ(m1,m2);
    ASSERT_EQ(s1,s2);
    ASSERT_EQ(time1.get(),time2.get());

    wdb::Time time3;
    time3.set(time2.get());
    time3.get(h1,m1,s1);
    ASSERT_EQ(h1,h2);
    ASSERT_EQ(m1,m2);
    ASSERT_EQ(s1,s2);
}

TEST_F(UT_Types,blob){
    
    wdb::Traits<wdb::Blob>::type blob;
    wdb::Traits<wdb::Blob>::codec cod;

    const std::string data("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec sodales, purus et rutrum posuere, nulla velit luctus risus fusce.");
    const std::string type("Latin blob");

    blob = wdb::Blob(reinterpret_cast<const wdb::byte*>(data.c_str()),data.size(),reinterpret_cast<const wdb::byte*>(type.c_str()));
    wg_int encoded = cod.encode(handle(),blob);

    wdb::Blob blob2 = cod.decode(handle(), encoded);
    wdb::bytes b1(blob.get(),blob.size()), b2(blob2.get(),blob2.size());
    ASSERT_EQ(b1,b2);
    ASSERT_NE(b1.c_str(),b2.c_str());

    b1.assign(blob.type()); b2.assign(blob2.type());
    ASSERT_EQ(b1,b2);
    ASSERT_NE(b1.c_str(),b2.c_str());
}

TEST_F(UT_Types,var){
    const wg_int val1 = 1, val2 = 2;
    wdb::Traits<wdb::Var>::type var1 = val1;
    wdb::Traits<wdb::Var>::type var2 = val2;
    wdb::Traits<wdb::Var>::codec cod;


    wg_int encoded1 = cod.encode(handle(), var1);
    wg_int encoded2 = cod.encode(handle(), var2);

    ASSERT_EQ(cod.decode(handle(), encoded1), val1);
    ASSERT_EQ(cod.decode(handle(), encoded2), val2);
}