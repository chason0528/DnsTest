#ifndef DNS_QNAME_H 
#define DNS_QNAME_H 1
#include <vector>
#include <stdint.h>
#include <iostream>
#include <cstring>
#include <map>

#include "common.h"

class QName{
    public:
        const char* const base_;
        uint8_t size_;
        //pointer to a new QName
        const static uint8_t POINTER_FLAG8 = 0xc0;
        const static uint16_t POINTER_FLAG16 = 0xc000;
        uint16_t pointer_;
        bool is_pointer_;
        const char *name_;
        size_t qn_size_;
        static const char* kClassesDex;

        //static std::map<size_t, std::vector<QName>*> *qnames_list_;


        QName(const char* base);
        ~QName();
        size_t GetSize();
        size_t GetPointerOffset();

        std::string *GetString();

        static uint8_t ParseQNames(std::vector<QName>&qns, size_t &len, const char* global_base, const char *base);


        #define IS_DOMAIN_ALL_CHAR(A) (((A)=='.') && isDomainChar(A))
        static bool inline isDomainChar(const char v){
            return  ((((v) >= 'A') && ((v) <= 'Z')) ||\
                                            (((v) >= 'a') && ((v) <= 'z')) ||\
                                                (((v) >= '0') && ((v) <= '9')) || ((v)=='-'));
        }

        size_t GetBytes(char *base);

        static std::vector<QName>* AddrToQNames(const std::string& address, size_t &len);


        static std::string *GetQNamesString(std::vector<QName> &qns);
        static void AddQNamesMap(size_t base, std::vector<QName> *qns);
    private:


};


#endif