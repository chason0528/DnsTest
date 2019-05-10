#ifndef DNS_COMMON_H 
#define DNS_COMMON_H 1

#include <string>
#include <stdint.h>

inline uint16_t ReadBigEddianUint16(const char *p){
    return ((uint16_t)p[0] << 8) + (uint16_t)p[1];
}

inline char* WriteBigEddianUint16(uint16_t v, char *p){
    p[0] = (char)((v & 0xff00) >> 8);
    p[1] = (char)(v & 0x00ff);
    return p;
}
inline uint32_t ReadBigEddianUint32(const char *p){
    return ((uint32_t)p[0] << 24) + ((uint32_t)p[1] << 16)  
                        + ((uint32_t)p[2] << 8)  + (uint32_t)p[3];
}
inline char* WriteBigEddianUint32(uint16_t v, char *p){
    p[0] = (char)((v & 0xff000000) >> 24);
    p[1] = (char)((v & 0x00ff0000) >> 16);
    p[2] = (char)((v & 0x0000ff00) >> 8);
    p[3] = (char)(v & 0x000000ff);
    return p;
}
inline uint16_t Big2Little16(uint16_t v){
    return ((v & 0xff00) >> 8) | ((v & 0x00ff) << 8);
}
inline uint32_t Big2Little32(uint32_t v){

    return ((v & 0xff000000) >> 24) | ((v & 0x00ff0000) >> 8) | 
                        ((v & 0x0000ff00) << 8) | ((v & 0x000000ff) << 24);
}


inline std::string *GetIpv4String(const char *p) {
    auto str = new std::string();
    for (int i = 0; i < 4; ++i) {
        (*str) += std::to_string((uint8_t)p[i]);
        (*str) += '.';
    }
    str->pop_back();
    return str;
}


#define Little2Big16(V) Big2Little16(V)
#define Little2Big32(V) Big2Little32(V)


#endif