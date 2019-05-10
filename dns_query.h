#ifndef DNS_QUERY_H
#define DNS_QUERY_H 1
#include<vector>
#include <stdint.h>
#include <iostream>

#include "dns_qname.h"
// Here is the question section format:
//                                 1  1  1  1  1  1
//   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                                               |
// /                     QNAME                     /
// /                                               /
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                     QTYPE                     |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                     QCLASS                    |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+



class DnsQuery{
    
    public:
        const char* const base_;
        const char* const global_base_;
        std::vector<QName> *q_names_;
        //QType values' list
        enum{
            kQueryTypeA              = 1,    // a host address
            kQueryTypeNS             = 2,    // an authoritative name server
            kQueryTypeMD             = 3,    // a mail destination (Obsolete - use MX)
            kQueryTypeMF             = 4,    // a mail forwarder (Obsolete - use MX)
            kQueryTypeCName          = 5,    // the canonical name for an alias
            kQueryTypeSOA            = 6,    // marks the start of a zone of authority
            kQueryTypeMB             = 7,    // a mailbox domain name (EXPERIMENTAL)
            kQueryTypeMG             = 8,    // a mail group member (EXPERIMENTAL)
            kQueryTypeMR             = 9,    // a mail rename domain name (EXPERIMENTAL)
            kQueryTypeNull           = 10,   // a null RR (EXPERIMENTAL)
            kQueryTypeWKS            = 11,   // a well known service description
            kQueryTypePtr            = 12,   // a domain name pointer
            kQueryTypeHInfo          = 13,   // host information
            kQueryTypeMInfo          = 14,   // mailbox or mail list information
            kQueryTypeMX             = 15,   // mail exchange
            kQueryTypeTxt            = 16,   // text strings
            kQueryTypeAXFR           = 252,  // A request for a transfer of an entire zone
            kQueryTypeMAILB          = 253,  // A request for mailbox-related records (MB, MG or MR)
            kQueryTypeMAILA          = 254,  // A request for mail agent RRs (Obsolete - see MX)
            kQueryTypeAll            = 255,  // A request for all records
        };
        uint16_t q_type_;
        //QClass values' list
        enum{
            kQueryClassIn            = 1,       // the Internet
            kQueryClassCS            = 2,       // the CSNET class (Obsolete - used only for examples in
                                                // some obsolete RFCs)
            kQueryClassCh            = 3,       // the CHAOS class
            kQueryClassHS            = 4,       // Hesiod [Dyer 87]
            kQueryClassAll           = 255,     // A request for all records
        };
        uint16_t q_class_;
        DnsQuery(const char* const global_base, const char *base);
        DnsQuery(std::vector<QName> *qns, size_t qns_size, uint16_t qt, uint16_t qc);
        ~DnsQuery();
        size_t GetSize();
        void PrintTest();
        size_t GetBytes(char *bytes);
        static DnsQuery* CreateBaseQuery(const std::string &address, uint16_t q_type, uint16_t q_class);
    private:
        size_t q_names_size_;
        size_t whole_query_size_;


};

#endif