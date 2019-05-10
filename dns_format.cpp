#include<vector>
#include <stdint.h>
#include <cstring>

#include "dns_format.h"
#include "common.h"

DnsFormat::DnsFormat(const char *base)
    : base_(base) {
  const DnsHeader *raw_header = reinterpret_cast<const DnsHeader*>(base_);
  header_ = InitDnsHeader(Big2Little16(raw_header->id),
                          Big2Little16(raw_header->flags),
                          Big2Little16(raw_header->qd_count),
                          Big2Little16(raw_header->an_count),
                          Big2Little16(raw_header->ns_count),
                          Big2Little16(raw_header->ar_count));
//  header_ = new DnsHeader();
//  header_->id = Big2Little16(raw_header->id);
//  header_->flags = Big2Little16(raw_header->flags);
//  header_->qd_count = Big2Little16(raw_header->qd_count);
//  header_->an_count = Big2Little16(raw_header->an_count);
//  header_->ns_count = Big2Little16(raw_header->ns_count);
//  header_->ar_count = Big2Little16(raw_header->ar_count);
  queries_ = new std::vector<DnsQuery>();
  queries_size_= ParseQueries(base_, base += sizeof(DnsHeader), *queries_, header_->qd_count);
  answers_ = new std::vector<DnsResponse>();
  answers_size_ = ParseResponses(base_, base += queries_size_, *answers_, header_->an_count);
  authorities_ = new std::vector<DnsResponse>();
  authorities_size_ = ParseResponses(base_, base += answers_size_, *authorities_, header_->ns_count);
  additions_ = new std::vector<DnsResponse>();
  additions_size_ = ParseResponses(base_, base += authorities_size_, *additions_, header_->ar_count);
  whole_size_= sizeof(DnsHeader) + queries_size_ +
                        answers_size_ + authorities_size_ + additions_size_;

    
}

DnsFormat::DnsFormat(DnsHeader *header,
                     std::vector<DnsQuery> *queries,
                     std::vector<DnsResponse> *answers,
                     std::vector<DnsResponse> *,
                     std::vector<DnsResponse> *)
                     : base_(nullptr),
                       header_(header),
                       queries_(queries),
                       answers_(answers){
    queries_size_ = 0;
    for (int i = 0; i < header_->qd_count; ++i) {
        queries_size_ += (*queries_)[i].GetSize();
    }

    answers_size_ = 0;
    for (int i = 0; i < header_->an_count; ++i) {
        answers_size_ += (*answers_)[i].GetSize();
    }

    authorities_size_ = 0;
    for (int i = 0; i < header_->ns_count; ++i) {
        authorities_size_ += (*authorities_)[i].GetSize();
    }

    additions_size_ = 0;
    for (int i = 0; i < header_->ar_count; ++i) {
        additions_size_ += (*additions_)[i].GetSize();
    }

    whole_size_= sizeof(DnsHeader) + queries_size_ +
                 answers_size_ + authorities_size_ + additions_size_;

}

DnsFormat::~DnsFormat(){
}



size_t DnsFormat::ParseQueries(const char* global_base, const char *base, std::vector<DnsQuery> &qs, uint16_t queries_count)
{
    const char * old_base = base;
    for(int i = 0; i < queries_count; i++)
    {
      DnsQuery *query = new DnsQuery(global_base, base);
      qs.push_back(*query);
      base += query->GetSize();
    }
    return (base - old_base);
}

size_t DnsFormat::ParseResponses(const char* global_base, const char *base, std::vector<DnsResponse> &rs, uint16_t responses_count)
{
    const char * old_base = base;
    for(int i = 0; i < responses_count; i++)
    {
      DnsResponse *query = new DnsResponse(global_base, base);
      rs.push_back(*query);
      base += query->GetSize();
    }
    return (base - old_base);
}

DnsFormat::DnsHeader * DnsFormat::InitDnsHeader(uint16_t id, uint16_t flags,
                                            uint16_t qd_count, uint16_t an_count,
                                            uint16_t ns_count, uint16_t ar_count)
{
    DnsFormat::DnsHeader *header = new DnsFormat::DnsHeader();
    header->id = id;
    header->flags = flags;
    header->qd_count =  qd_count;
    header->an_count = an_count;
    header->ns_count = ns_count;
    header->ar_count = ar_count;
}

#define DNS_UDP_MAX 512
size_t DnsFormat::GetBytes(char* bytes) {

    size_t base = 0;

    //flush header
    DnsHeader *toHeader = reinterpret_cast<DnsHeader*>(bytes);
    toHeader->id = Little2Big16(header_->id);
    toHeader->flags = Little2Big16(header_->flags);
    toHeader->qd_count = Little2Big16(header_->qd_count);
    toHeader->an_count = Little2Big16(header_->an_count);
    toHeader->ns_count = Little2Big16(header_->ns_count);
    toHeader->ar_count =  Little2Big16(header_->ar_count);
    toHeader = nullptr;
    base += sizeof(DnsHeader);
    //flush queries block
    for (int i = 0; i < header_->qd_count; ++i) {
        base += (*queries_)[i].GetBytes(&bytes[base]);
    }
    //flush answers block
    for (int i = 0; i < header_->an_count; ++i) {
        base += (*answers_)[i].GetBytes(&bytes[base]);
    }

    //flush authorities block
    for (int i = 0; i < header_->ns_count; ++i) {
        base += (*authorities_)[i].GetBytes(&bytes[base]);
    }

    //flush additions block
    for (int i = 0; i < header_->ar_count; ++i) {
        base += (*additions_)[i].GetBytes(&bytes[base]);
    }

    return base;
}
#define STANDARD_QUERY 0x0100
DnsFormat* DnsFormat::CreateDnsQuest(const std::string &address, uint16_t type){
    //create a random id value
    auto id = static_cast<uint16_t>(rand());
    uint16_t flag = STANDARD_QUERY;
    DnsHeader* header = InitDnsHeader(id, flag, 1, 0, 0, 0);
    DnsQuery *query = DnsQuery::CreateBaseQuery(address, type, DnsQuery::kQueryClassIn);
    if (query == nullptr)
        return nullptr;
    std::vector<DnsQuery> *queries = new std::vector<DnsQuery>;
    (*queries).push_back(*query);
    DnsFormat *format = new DnsFormat(header, queries, nullptr, nullptr, nullptr);
    return format;

}
#define PRINT_BLANK_LINE (std::cout << std::endl)
void DnsFormat::PrintData() {
    //print answers block
    std::cout << "answers:" << std::endl;
    for (int i = 0; i < header_->an_count; ++i) {
        (*answers_)[i].PrintData();
    }
    PRINT_BLANK_LINE;
    //print authorities block
    std::cout << "authorities:" << std::endl;
    for (int i = 0; i < header_->ns_count; ++i) {
        (*answers_)[i].PrintData();
    }
    PRINT_BLANK_LINE;
    //print additions block
    std::cout << "addtions:" << std::endl;
    for (int i = 0; i < header_->ar_count; ++i) {
        (*answers_)[i].PrintData();
    }
    PRINT_BLANK_LINE;
}