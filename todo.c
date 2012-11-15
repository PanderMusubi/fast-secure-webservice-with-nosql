#include <openssl/aes.h> 
//#include <gbase64.h>

#include <openssl/bio.h>
#include <openssl/evp.h>

BIO_METHOD *   BIO_f_base64(void);


/*char *decode(unsigned char *input, int length)
{
BIO *b64, *bmem;

char *buffer = (char *)malloc(length);
memset(buffer, 0, length);

b64 = BIO_new(BIO_f_base64());
bmem = BIO_new_mem_buf(input, length);
bmem = BIO_push(b64, bmem);

BIO_read(bmem, buffer, length);

BIO_free_all(bmem);

return buffer;
}

char *  dgstdecode(char *pReadBuffer, int pLength)
{
     char *msg = (char *)malloc(pLength);
    memset(msg, 0x00, pLength);
    int readbytes = -1;

    printf("inside dgstdecode\n");
    printf("\n pReadBuffer = %s \n", pReadBuffer);
    BIO *b64, *bio = NULL;
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(pReadBuffer, pLength);
    bio = BIO_push(b64, bio);
   //BIO_set_flags(bio,BIO_FLAGS_BASE64_NO_NL);

    while ((readbytes = BIO_read(bio, msg, pLength)) > 0)
    {
    printf("readbytes:  %d\n", readbytes);
    printf("inside dgstdecode\n");
    }
    int aaa = BIO_flush(bio);
    printf("%d", aaa);
    printf("msg = %s\n", msg);
    BIO_free_all(bio);
    //BIO_free_all(b64);
    return msg;

}*/

int
main (void)
{

/*  static const unsigned char* key = (unsigned char*)"Sixteen byte key";
  printf("key = %s\n", key);

  unsigned char text[]="virident";
  unsigned char out[10]; 
  unsigned char decout[10];
 
  AES_KEY wctx;
  AES_set_encrypt_key(key, 128, &wctx);
  AES_encrypt(text, out, &wctx);  
  printf("encryp data = %s\n", out);
  AES_decrypt(out, decout, &wctx);
  printf(" Decrypted o/p: %s \n", decout);*/

/*  char* dec64 = "asdf";
  char* enc64 = "";
  
  enc64 = g_base64_encode(dec64, 4); 
  
  printf("%s", enc64);*/

/*
  BIO *bio, *b64;
  char message[] = "Hello World \n";

  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new_fp(stdout, BIO_NOCLOSE);
  bio = BIO_push(b64, bio);
  BIO_write(bio, message, strlen(message));
  printf("%d", BIO_flush(bio));

  BIO_free_all(bio);

  char *decoded;
  decoded = decode((unsigned char*)"Rtc03zL2OvzwCKyzEWmKRgN7h3aKaZZKlkWeulYy3aQ=", strlen("Rtc03zL2OvzwCKyzEWmKRgN7h3aKaZZKlkWeulYy3aQ="));
  printf("%s %d\n", (unsigned char*)"Rtc03zL2OvzwCKyzEWmKRgN7h3aKaZZKlkWeulYy3aQ=", (int)strlen("Rtc03zL2OvzwCKyzEWmKRgN7h3aKaZZKlkWeulYy3aQ="));
  printf("decoded: %s %d\n", decoded, (int)strlen(decoded));

//  char buff[] = "Rtc03zL2OvzwCKyzEWmKRgN7h3aKaZZKlkWeulYy3aQ=";
  char buff[] = "SGVsbG8gV29ybGQgCg==";
  decoded = dgstdecode(buff, strlen(buff));
  printf("decoded: %s %d\n", decoded, (int)strlen(decoded));
*/

/*  int inlen;
  char decoded[512] = "";

  BIO *bio, *b64;//, *bio_decoded;
  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);
  inlen = BIO_read(bio, inbuf, 512);
  BIO_puts();
  printf("%s, %d", decoded, inlen);*/
  
 
//  bio = BIO_new_fp(stdin, BIO_NOCLOSE);
//  bio_out = BIO_new_fp(stdout, BIO_NOCLOSE);
//  bio = BIO_push(b64, bio);
//  while((inlen = BIO_read(bio, inbuf, 512)) > 0)
//    BIO_write(bio_out, inbuf, inlen);

//  BIO_free_all(bio);



  return 0;
}
