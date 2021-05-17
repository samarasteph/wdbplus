#pragma once

#ifndef __WDB_FIX_CLASS_H__
#define __WDB_FIX_CLASS_H__

#include <gtest/gtest.h>
#include "wdb.h"

namespace wdb{
    namespace test {

        const std::string db_name("unit test"); 

        class TestFixture: public ::testing::Test {
            public:
            void SetUp(){
                if(!_db.open(db_name, 1024*1024)){
                    throw std::runtime_error("Cannot open database");
                }
            }
            void TearDown() {
                _db.destroy();
            }
            WhiteDb _db;
        };

        template<class DatabaseImpl_t>
        class TestFixtureT: public ::testing::Test {
        public:
            
            typedef DatabaseImpl_t Db_t ;

            void SetUp(){
                if(!_db.open(db_name, 1024*1024)){
                    throw std::runtime_error("Cannot open database");
                }
            }
            void TearDown() {
                _db.destroy();
            }
            Db_t& db() { return _db; }
            Db_t _db;
        };

        class TestDb {
        public:
            TestDb(): _db(nullptr){}
            bool  open(const std::string name, size_t size) { 
                _db = wg_attach_database((char*)_name.c_str(), size);
                if (_db) _name = name;
                return _db != nullptr;
            }
            void destroy() {  wg_delete_database(const_cast<char*>(_name.c_str())); }
            void* handle() { return _db; }
            void* _db;
            std::string _name;
        };
    }
}

#endif