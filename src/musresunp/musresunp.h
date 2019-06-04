#ifndef _TESTAPP_H
#define _TESTAPP_H

#include <stdint.h>

#if defined(_MSC_VER) 
#   pragma warning(disable:4996)
#endif /* defined(_MSC_VER)  */


#define KNG_RESUNP_SUCCESS      0
#define KNG_RESUNP_ERR_NOFILE   1
#define KNG_RESUNP_ERR_MALLOC   2
#define KNG_RESUNP_ERR_UEOF     3
#define KNG_RESUNP_ERR_FWRITE   4

#define KNG_RESUNP_ERR_UNKNOWN -1

struct rtsc_resfile_startdesc
{
    uint32_t field0;
    uint32_t size;
};

struct rtsc_musres_descr
{
    uint32_t offset;
    uint32_t size;
};

#endif // _TESTAPP_H
