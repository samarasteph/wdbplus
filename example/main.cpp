#include <iostream>
#include <string>
#include <ctime>
#include  "wdb.h"
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

wdb::Blob copyFile(const std::string& filename){
    wdb::bytes content;
    std::ifstream ifs;
    ifs.open(filename, std::ios_base::in | std::ios_base::binary);
    if(ifs){
        constexpr size_t buffer_len = 1024;
        char buffer[buffer_len];
        size_t file_len = 0, read=0;

        ifs.seekg(0, ifs.end);
        file_len = ifs.tellg();
        ifs.seekg(0, ifs.beg);

        while (read < file_len){
            size_t len = std::min(buffer_len, file_len-read);
            ifs.read(buffer, buffer_len);
            read += len;

            std::copy(reinterpret_cast<wdb::byte*>(buffer), reinterpret_cast<wdb::byte*>(buffer)+len,
                std::back_inserter(content));
        }
        const wdb::byte* type = reinterpret_cast<const wdb::byte*>(filename.c_str());
        
        ifs.close();

        return wdb::Blob(content.c_str(), content.size(), type);
    }
    else {
        std::ostringstream oss;
        oss << "l. " << __LINE__ << "Cannot open file \"" << filename << "\"" ;
        std::cout << oss.str() << std::endl;
        throw std::runtime_error(oss.str());
    }
    return wdb::Blob();
}

void compareToFile(wdb::Blob& b){
    wdb::bytes content;
    std::ifstream ifs;
    const std::string filename( reinterpret_cast<const char*>(b.type()) );
    ifs.open(filename, std::ios_base::in | std::ios_base::binary);
    if(ifs){
        constexpr size_t buffer_len = 1024;
        char buffer[buffer_len];
        size_t file_len = 0, read=0;

        ifs.seekg(0, ifs.end);
        file_len = ifs.tellg();
        ifs.seekg(0, ifs.beg);

        const wdb::byte* content = b.get();

        while (read < file_len){
            size_t len = std::min(buffer_len, file_len-read);
            ifs.read(buffer, buffer_len);
            
            if (std::memcmp(buffer, content+read, len) != 0){
                ifs.close();
                std::string msg = "ERROR: Blob != file !!";
                std::cout << msg << std::endl;
                throw std::runtime_error(msg);
            }

            read += len;
        }
        ifs.close();
    }
    else {
        std::ostringstream oss;
        oss << "l. " << __LINE__ << ": Cannot open file \"" << filename << "\"" ;
        std::cout << oss.str() << std::endl;
        throw std::runtime_error(oss.str());
    }
}

void displayfield(const WhiteDb::Field& field);

void displayRecords1(WhiteDb& db){
    uint record_count = 0;
    for (WhiteDb::BasicRecord rec :db){
        std::cout << "[Record-" <<  record_count++ << "] ";
        
        for (WhiteDb::Field field :rec){
            displayfield(field);
        }
        std::cout << std::endl;
    }
}
void displayRecords2(WhiteDb& db){
    WhiteDb::rec_iterator<5> rec_it = db.begin<5>();
    uint record_count = 0;
    while(rec_it!=db.end<5>()){
        std::cout << "[Record-" <<  record_count++ << "] ";
        WhiteDb::Record<5> rec = *(rec_it);
        for(WhiteDb::Field f :rec){
            displayfield(f);
        }
        rec_it++;
        std::cout << std::endl;
    }
}
void displayfield(const WhiteDb::Field& field){

    switch(field.type()){
    case wdb::eWG_DOUBLETYPE:
        std::cout << "(double)"<< field.value<double>();
        break;
    case wdb::eWG_INTTYPE:
        std::cout << "(int)" << field.value<wg_int>();
        break;
    case wdb::eWG_STRTYPE:
        std::cout << "(char*)" << field.value<char*>();
        break;
    case wdb::eWG_TIMETYPE:{
            int h,m,s;
            field.value<wdb::Time>().get(h,m,s);
            std::cout << "(Time)" << h << ":" << m << ":" << s;
        }
        break;
    case wdb::eWG_DATETYPE:
        {
            int y,m,d;
            field.value<wdb::Date>().get(y,m,d);
            std::cout << "(Date)" << y << "/" << m << "/" << d;
        }
        break;
    case wdb::eWG_RECORDTYPE:
        {
            WhiteDb::BasicRecord r  = field.value<WhiteDb::BasicRecord>();
            std::cout << "(Record) ( ";
            for (const WhiteDb::Field& f :r){
                displayfield(f);
            }
            std::cout << " )";

        }
        break;
    case wdb::eWG_BLOBTYPE:
        {
            wdb::Blob b = field.value<wdb::Blob>();
            const wdb::byte* type = b.type();
            std::cout << "(Blob) type " << reinterpret_cast<const char*>(type) 
                << " data len " << b.size() << std::endl;
        }
        break;
    default:
        break;
    }
    
}

void make_queries(WhiteDb& db){
    WhiteDb::Query q = db.createQuery();
    //q.add(wdb::Greater<double,3>(18.5));
    constexpr wg_int col1 = 4;
    constexpr wg_int col2 = 3;
    wg_int val1 = 9;
    double val2 = 18.5;
    std::cout << "make query col=" << col1 << " <= " << val1 << " and col=" << col2 << " < " << val2 << std::endl;
    q.add(wdb::LessEqual<wg_int,col1>(val1), wdb::Less<double,col2>(val2));
    if (q.exec()){
        std::cout << "query successfull" << std::endl;
        for (WhiteDb::BasicRecord r :q){
            for (WhiteDb::Field field :r){
                displayfield(field);
            }
            std::cout << std::endl;
        }
    }
}

int main(int, char**) {

    WhiteDb db;
    bool ok = db.open("Database test", 1024*1024);

    if (!ok){
        std::cout << "database error" << std::endl;
        return -1;
    }

    for (int i=0; i<12; i++){
        WhiteDb::Record<5> rec = db.createRecord<5>();
        char str[64] ;
        sprintf(str, "Hello world %d", i);
        wdb::Time t;
        wdb::Date d;
        time_t tt;
        t = db.utcTime();
        d = db.utcDate();
        int year, month, day;


        rec.setField<char*,0>(str,(char*)NULL);
        rec.setField<wdb::Time,1>(t);
        rec.setField<wdb::Date,2>(d);
        rec.setField<double,3>(i*3.01);
        rec.setField<wg_int,4>(4L+wg_int(i));
    }

    //Blob
    WhiteDb::BasicRecord r = *db.begin();

    WhiteDb::Record<3> r2 = db.createRecord<3>();
    r2.setField<WhiteDb::BasicRecord,0>(r);
    
    wdb::Blob b = copyFile("/home/aldu/Documents/dev/wdbplus/dump.txt");
    r2.setField<wdb::Blob,1>(b);
    r2.setField<wg_int,2>(12345L);


    b = wdb::Blob();

    WhiteDb::Query q = db.createQuery();
    q.add(wdb::EqualTo<wg_int,2>(12345L));
    if (q.exec()){
        WhiteDb::Query::iterator itq = q.begin();
        if(itq != q.end()){
            WhiteDb::BasicRecord rec = *itq;
            b = rec.getField(1).value<wdb::Blob>();

            compareToFile(b);
        }
        else {
            std::cout << "No record found" << std::endl;
        }
    }
    else {
        std::cout << "Unsuccessfull request" << std::endl;
    }

    std::cout  << "************** display records **************" << std::endl;
    displayRecords2(db);
    std::cout  << "************** queries **************" << std::endl;
    make_queries(db);

    uint record_count = 0;
    WhiteDb::iterator it = db.begin();
    while(it!=db.end()){
        it = db.deleteRecord(it);
        record_count += 1;
    }
    std::cout << std::endl << record_count << " records deleted" << std::endl;
    return 0;
}
