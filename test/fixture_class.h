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

        class TestDb: private wdb::PrimitiveDatabase<TestDb> {
        public:
            using PrimDb_t::createRecord;
            typedef PrimDb_t::iterator iterator;
            typedef PrimDb_t::Field Field;
            template<wg_int NbFields>
            using rec_iterator = PrimDb_t::rec_iterator<NbFields>;
            typedef PrimDb_t::BasicRecord_t BasicRecord_t;

            template<wg_int NbFields>
            using Record = PrimDb_t::Record<NbFields>;

            TestDb(): PrimitiveDatabase(*this), _db(nullptr){}
            bool  open(const std::string name, size_t size) { 
                _db = wg_attach_database((char*)_name.c_str(), size);
                if (_db) _name = name;
                return _db != nullptr;
            }
            void destroy() {  wg_delete_database(const_cast<char*>(_name.c_str())); }
            void* handle() { return _db; }
            iterator begin(){
                first_record firstrec_primitive;
                PrimDb_t::BasicRecord_t br =  createRecord<PrimDb_t::BasicRecord_t>(firstrec_primitive(_db));
                return iterator(br);
            }
            iterator end(){ 
                BasicRecord_t br = PrimDb_t::createRecord<BasicRecord_t>(nullptr);
                return iterator(br);
            }

            template<wg_int NbFields>
            rec_iterator<NbFields> begin(){
                first_record firstrec_primitive;
                return rec_iterator<NbFields>(*this,firstrec_primitive(_db));
            }

            template<wg_int NbFields>
            rec_iterator<NbFields> end(){
                first_record firstrec_primitive;
                return rec_iterator<NbFields>(*this,nullptr);
            }

            void* _db;
            std::string _name;
        };

    }
}

#endif