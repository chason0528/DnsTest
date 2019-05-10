#include <stdint.h>

#include "dns_response.h"
#include "dns_qname.h"
#include "common.h"
DnsResponse::DnsResponse(const char* const global_base, const char *base)
    : global_base_(global_base),
      base_(base){
    q_names_ = new std::vector<QName>;
    QName::ParseQNames(*q_names_, q_names_size_, global_base, base);
    type_ = ReadBigEddianUint16(base += q_names_size_);
    class_ = ReadBigEddianUint16(base += sizeof(uint16_t));
    ttl_ = ReadBigEddianUint32(base += sizeof(uint16_t));
    rd_length_ = ReadBigEddianUint16(base += sizeof(uint32_t));
    rd_data_ = base += sizeof(uint16_t); 
    whole_response_size_ = q_names_size_ + sizeof(uint16_t) + sizeof(uint16_t) +
                                    sizeof(uint32_t) + sizeof (uint16_t) + sizeof(char)*rd_length_ ; 
//    if (type_ == kResponseTypeCName){
//        size_t len;
//        auto qns = QName::ParseQNames(global_base,rd_data_, len, rd_length_);
//        QName::AddQNamesMap(reinterpret_cast<size_t>(rd_data_) -
//                                  reinterpret_cast<size_t> (global_base_), qns);
//    }
}
size_t DnsResponse::GetSize(){
    return whole_response_size_; 
}

size_t DnsResponse::GetBytes(char *bytes){

}

void DnsResponse::PrintData(){
    switch (type_)
    {
        case kResponseTypeA:
            std::cout << QName::GetQNamesString(*q_names_)->c_str() <<
                                ":\t" << GetIpv4String(rd_data_)->c_str() << std::endl;

    }
}