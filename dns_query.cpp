#include<vector>
#include <stdint.h>
#include <stdint.h>

#include "common.h"
#include "dns_query.h"
#include "dns_qname.h"
DnsQuery::DnsQuery(const char* const global_base, const char *base)
        : base_(base),
          global_base_(global_base) {
    q_names_ = new std::vector<QName>;
    QName::ParseQNames(*q_names_, q_names_size_, global_base, base);
    q_type_ = ReadBigEddianUint16(base += q_names_size_);
    q_class_= ReadBigEddianUint16(base += sizeof(uint16_t));
    whole_query_size_ = q_names_size_ + sizeof(uint16_t) + sizeof(uint16_t);
}

DnsQuery::DnsQuery(std::vector<QName> *qns, size_t qns_size, uint16_t qt, uint16_t qc)
        : base_(nullptr),
          global_base_(nullptr),
          q_names_(qns),
          q_names_size_(qns_size),
          q_type_(qt),
          q_class_(qc),
          whole_query_size_(q_names_size_ + sizeof(uint16_t) + sizeof(uint16_t)){

}

DnsQuery::~DnsQuery(){

}

size_t DnsQuery::GetSize(){
    return whole_query_size_;
}

size_t DnsQuery::GetBytes(char *bytes){
    int base = 0;
    for (size_t i = 0; i < (*q_names_).size(); i++)
    {
        base += (*q_names_)[i].GetBytes(&bytes[base]);
    }
    WriteBigEddianUint16(q_type_, &bytes[base]);
    base += sizeof(uint16_t);
    WriteBigEddianUint16(q_class_, &bytes[base]);
    base += sizeof(uint16_t);
    return base;
}

DnsQuery* DnsQuery::CreateBaseQuery(const std::string &address, uint16_t q_type, uint16_t q_class)
{
    size_t qnames_size;
    auto *qns = QName::AddrToQNames(address, qnames_size);

    if(qnames_size == 0)
        return nullptr;
    return new DnsQuery(qns, qnames_size, q_type, q_class);
}

void DnsQuery::PrintTest(){

}

