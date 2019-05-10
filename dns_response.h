#ifndef DNS_RESPONSE_H 
#define DNS_RESPONSE_H 1
#include <stdint.h>
#include<vector>

#include "dns_qname.h"

// Here is the RR format definition: 
//                                    1  1  1  1  1
//   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                                               |
// /                                               /
// /                      NAME                     /
// |                                               |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                      TYPE                     |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                     CLASS                     |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                      TTL                      |
// |                                               |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                   RDLENGTH                    |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
// /                     RDATA                     /
// /                                               /
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
class DnsResponse{
    public:
        const char* const global_base_;
        const char* const base_;
        std::vector<QName> *q_names_;
        //type values' list
        enum{
            kResponseTypeA              = 1,    // a host address
            kResponseTypeNS             = 2,    // an authoritative name server
            kResponseTypeMD             = 3,    // a mail destination (Obsolete - use MX)
            kResponseTypeMF             = 4,    // a mail forwarder (Obsolete - use MX)
            kResponseTypeCName          = 5,    // the canonical name for an alias
            kResponseTypeSOA            = 6,    // marks the start of a zone of authority
            kResponseTypeMB             = 7,    // a mailbox domain name (EXPERIMENTAL)
            kResponseTypeMG             = 8,    // a mail group member (EXPERIMENTAL)
            kResponseTypeMR             = 9,    // a mail rename domain name (EXPERIMENTAL)
            kResponseTypeNull           = 10,   // a null RR (EXPERIMENTAL)
            kResponseTypeWKS            = 11,   // a well known service description
            kResponseTypePtr            = 12,   // a domain name pointer
            kResponseTypeHInfo          = 13,   // host information
            kResponseTypeMInfo          = 14,   // mailbox or mail list information
            kResponseTypeMX             = 15,   // mail exchange
            kResponseTypeTxt            = 16,   // text strings
        };
        uint16_t type_;
        
        //class values' list
        enum{
            kResponseClassIn            = 1,    // the Internet
            kResponseClassCS            = 2,    // the CSNET class (Obsolete - used only for examples in
                                                // some obsolete RFCs)
            kResponseClassCh            = 3,    // the CHAOS class
            kResponseClassHS            = 4,    // Hesiod [Dyer 87]
        };
    
        uint16_t class_;
        uint32_t ttl_;
        uint16_t rd_length_;
        const char* rd_data_;
        DnsResponse(const char* const global_base, const char *base);
        size_t GetSize();
        size_t GetBytes(char *bytes);
        void PrintData();

private:
        size_t q_names_size_;
        size_t whole_response_size_;
        
};

#endif