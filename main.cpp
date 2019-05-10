#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <getopt.h>
#include <string>
#include <vector>

#include "dns_format.h"
#include "data.h"

const size_t BUF_SIZE = 1024;
#define DNS_HOST "114.114.114.114"
#define PORT 53

#define USAGE "Usage:\t dnstest -a [address] -t [type]"
#define TYPE_LIST "Type list: {A}"



int main(int argc, char *argv[]){

    int ret;
    opterr = 0;
    std::string address, type;
    int kQueryType = DnsQuery::kQueryTypeA;

    while ((ret = getopt(argc, argv, "a:t:h")) != -1)
    {
        switch (ret) {
            case 'a':
                address = optarg;
                break;
            case 't':
                type = optarg;
                //fixme
                if (type == "A")
                    kQueryType = DnsQuery::kQueryTypeA;
                else{
                    std::cout << TYPE_LIST << std::endl;
                    return 0;
                }
                break;
            case 'h':
                std::cout << USAGE << std::endl;
                return 0;
            default:
                std::cout << USAGE << std::endl;
                return 0;
        }
    }

    if (address.size() <= 0)
    {
        std::cout << USAGE << std::endl;
        return 0;
    }




    int fd= socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket fd is created error");
        return -1;
    }

    struct sockaddr_in addr_serv;
    memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = inet_addr(DNS_HOST);
    addr_serv.sin_port = htons(PORT);
    size_t struct_len = sizeof(addr_serv);
    char *buf = static_cast<char *>(malloc(sizeof(char) *BUF_SIZE));

    DnsFormat *format = DnsFormat::CreateDnsQuest(address, kQueryType);

    if(format == nullptr){
        return 1;
    }
    size_t buf_len  = format->GetBytes(buf);

    sendto(fd, buf, buf_len, 0, (struct sockaddr *)&addr_serv, struct_len);
    recvfrom(fd, buf, BUF_SIZE, 0, (struct sockaddr *)&addr_serv, (socklen_t *)&(struct_len));
    DnsFormat *ResponseDns = new DnsFormat(buf);
    ResponseDns->PrintData();


    return 0;
}
