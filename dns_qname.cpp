//
// Created by x on 5/9/19.
//
#include "dns_qname.h"


#define DOMAIN_ADDR_MAX 128
std::vector<QName>* QName::AddrToQNames(const std::string& address, size_t &len) {

    const char* address_chars = address.c_str();
    // malloc memory

    char * qns_chars= static_cast<char *>(malloc(DOMAIN_ADDR_MAX));
    int block_len = 0;
    int index_recorder = 0;
    int i = 0;
    // 0123456789
    //  s.ss.sss
    // 1s2ss3sss0
    for (i = 0; i < address.length() && (i+1) < DOMAIN_ADDR_MAX; ++i) {
        if (isDomainChar(address_chars[i]))
        {
            block_len++;
            qns_chars[i+1]=address_chars[i];
        }
        else if (address_chars[i] == '.')
        {
            if (block_len == 0)
                // cannot start with '.' or have continuous '.'
                return 0;
            //insert the index
            qns_chars[index_recorder] = block_len;
            //record the position of '.'
            // add the previous block's len
            // len = i + 1
            index_recorder += (block_len + 1);
            block_len = 0;

        }else{//unknown character
            std::cout << "invalid domain address" << std::endl;
            return nullptr;
        }
    }

    qns_chars[index_recorder] = block_len;
    //remember to end with '\0'
    qns_chars[i+1] = 0;
    auto *qns = new std::vector<QName>;
    ParseQNames(*qns, len, 0, qns_chars);
    return qns;
}

size_t QName::GetBytes(char *base) {
    if (is_pointer_)
    {
        WriteBigEddianUint16(pointer_, base);
        return sizeof(uint16_t);
    }
    memcpy(base, base_, qn_size_);
    return qn_size_;
}

uint8_t QName::ParseQNames(std::vector<QName>&qns, size_t &len, const char* global_base, const char *base) {


    const char* old_base = base;
    QName *qn;
    do{
        qn = new QName(base);
        if(qn->is_pointer_){
            base += sizeof(uint16_t);
            if(ParseQNames(qns, len, global_base, global_base + qn->GetPointerOffset()) == '\0')
            {
                len = base - old_base;
                return '\0';
            }
        }else
        {
            base += qn->GetSize();
            qns.push_back(*qn);
        }
    }while(qn->size_ != 0);
    len = base - old_base;
    return '\0';
}

std::string *QName::GetString() {
    if (size_ == 0)
        return new std::string();
    auto str = new std::string(name_, size_);
    return str;
}

size_t QName::GetPointerOffset() {
    if(is_pointer_)
        return (pointer_ & (~POINTER_FLAG16));
    else{
        std::cout << "Fbi warning: using error function QName::GetPointerOffset()" << std::endl;
        return 0;
    }
}

size_t QName::GetSize() {
    return qn_size_;
}

QName::QName(const char *base)
    : base_(base) {
        size_ = base[0];
        if((size_ & POINTER_FLAG8) == POINTER_FLAG8){
            name_ = 0;
            qn_size_ = sizeof(pointer_);
            pointer_ = ReadBigEddianUint16(base);
            is_pointer_ = true;
        }else{
            name_ = base + sizeof(uint8_t);
            qn_size_= sizeof(uint8_t) + size_ * sizeof(char);
            pointer_ = 0;
            is_pointer_ = false;
        }
}

QName::~QName() {

}

std::string *QName::GetQNamesString(std::vector<QName> &qns) {
    auto string = new std::string();
    for(int i = 0; i < qns.size(); i++){
        if (*(qns[i].GetString())!="")
        {
            *string += *(qns[i].GetString());
            *string += ".";
        }
    }
    //delete last character '.'
    string->pop_back();
    return string;
}
//std::map<size_t, std::vector<QName>*>* QName::qnames_list_ = new std::map<size_t, std::vector<QName>*>();
//
//void QName::AddQNamesMap(size_t base, std::vector<QName> *qns) {
//
//    //qnames_list_->insert(std::pair<size_t, std::vector<QName>*>(base, qns));
//    (*qnames_list_)[base] = qns;
//
//}
