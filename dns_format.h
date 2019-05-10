#ifndef DNS_FORMAT_H
#define DNS_FORMAT_H 1
#include<vector>
#include <stdint.h>

#include "dns_query.h"
#include "dns_response.h"

// Refer to https://tools.ietf.org/html/rfc1035 to
// get more info about dns protocal.Here is the dns 
//     +---------------------+
//     |        Header       |
//     +---------------------+
//     |       Question      | the question for the name server
//     +---------------------+
//     |        Answer       | RRs answering the question
//     +---------------------+
//     |      Authority      | RRs pointing toward an authority
//     +---------------------+
//     |      Additional     | RRs holding additional information
//     +---------------------+
//
// header section format:
//                                 1  1  1  1  1  1
//   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                      ID                       |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                    QDCOUNT                    |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                    ANCOUNT                    |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                    NSCOUNT                    |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                    ARCOUNT                    |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//

class DnsFormat{
    public:
        struct DnsHeader{
            uint16_t id;
            uint16_t flags;
            uint16_t qd_count;
            uint16_t an_count;
            uint16_t ns_count;
            uint16_t ar_count;
        };
        DnsHeader* header_;

        DnsFormat(const char *base);
          DnsFormat(DnsHeader *header,
                  std::vector<DnsQuery> *queries,
                  std::vector<DnsResponse> *answers,
                  std::vector<DnsResponse> *,
                  std::vector<DnsResponse> *);
        ~DnsFormat();
        const char* const base_;
        size_t whole_size_;
        std::vector<DnsQuery> *queries_;
        size_t queries_size_;
        std::vector<DnsResponse> *answers_;
        size_t answers_size_;
        std::vector<DnsResponse> *authorities_;
        size_t authorities_size_;
        std::vector<DnsResponse> *additions_;
        size_t additions_size_;
               

        size_t GetBytes(char* bytes);
        static DnsFormat* CreateDnsQuest(const std::string& address, uint16_t type);
        void PrintData();

    private:
        static bool isDomainAddr(const std::string& address, QName &qn);
        static size_t ParseQueries(const char* const global_base, const char *base, std::vector<DnsQuery> &qs, uint16_t queries_count);
        static size_t ParseResponses(const char* const global_base, const char *base, std::vector<DnsResponse> &rs, uint16_t responses_count);
        static DnsHeader * InitDnsHeader(uint16_t id, uint16_t flags, uint16_t qd_count, uint16_t an_count, uint16_t ns_count, uint16_t ar_count);


};

#endif // DNS_FORMAT_H
