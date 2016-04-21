#ifndef MD5_H
#define MD5_H

#ifdef __alpha
typedef unsigned int uint32d;
#else
typedef unsigned long uint32d;
#endif

struct MD5Context 
{
	uint32d buf[4];     
	uint32d bits[2];        
	unsigned char in[64];
};

typedef struct MD5Context MD5_CTX;

void MD5Init(struct MD5Context *ctx);
void MD5Update(struct MD5Context *ctx, unsigned char *buf, unsigned len);
void MD5Final(unsigned char digest[16], struct MD5Context *ctx);
void MD5Transform(uint32d buf[4], uint32d in[16]);
void dec2hex(unsigned char digest[16], char dst[32]);
int dec2hexs(unsigned char *pin, int inlen, char *pdst);
//对以'\0'结尾的字符串进行加密
char*  getMd5(const char* key,size_t keylen,char *pmd5out);

#endif
