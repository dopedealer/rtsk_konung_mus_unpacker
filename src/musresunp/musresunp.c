
#include <stdio.h>
#include <stdlib.h>

#include "musresunp.h"

#define _BSD_SOURCE

#define KNG_MUSRES_FILENAME "MUSICS.RES"
#define KNG_MUSRES_MAXSONGS 9

void printShortInfo(void);
int searchAndExtract(void);
long int getFileSize(FILE * fp);
int readFileAndUnpack(FILE * fpSrc, char * pool, long int poolSize);
void printErrorOccured(int err);

int
main(int argc, char ** argv)
{
    int err;
    printShortInfo();
    err = searchAndExtract();
    if (err)
    {
        printErrorOccured(err);
    }

    return 0;
}

void
printShortInfo(void)
{
    printf("Konung - Legends of the North music unpacker\n"
           "Searches MUSICS.RES file and extracts files from it\n");
}


/* 0 - success */
int searchAndExtract(void)
{
    int err = KNG_RESUNP_ERR_UNKNOWN;
    FILE * fp = 0x0;
    long int fileSz;
    char * pool = 0x0;

    do 
    {
        fp = fopen(KNG_MUSRES_FILENAME, "rb");
        if (fp == 0x0)
        {
            err =  KNG_RESUNP_ERR_NOFILE;
            break;
        }

        fileSz = getFileSize(fp);
        pool = malloc(fileSz);
        if (pool == 0x0)
        {
            err = KNG_RESUNP_ERR_MALLOC;
            break;
        }

        err = readFileAndUnpack(fp, pool, fileSz);
    } while (0); 

    if (pool) {
        free(pool);
    }
    if (fp) {
        fclose(fp);
    }

    return err;
}

long int
getFileSize(FILE * fp)
{
    long int oldPos, size;
    int rc;

    oldPos = ftell(fp);
    if (oldPos == -1)
        return 0;

    rc = fseek(fp, 0, SEEK_END);
    if (rc != 0)
        return 0;
    size = ftell(fp);
    if (size == -1)
        return 0;
    fseek(fp, oldPos, SEEK_SET);

    return size; 
}

int
readFileAndUnpack(FILE * fpSrc, char * pool, long int poolSize)
{
    int err = KNG_RESUNP_ERR_UNKNOWN;
    size_t frc;
    struct rtsc_resfile_startdesc startDescr;
    struct rtsc_musres_descr musfileDescr;
    long int baseDescPos, baseFilePos;
    int cnt;

    frc = fread(&startDescr, 1, sizeof(struct rtsc_resfile_startdesc), fpSrc);
    if (frc < sizeof(struct rtsc_resfile_startdesc))
    {
        err = KNG_RESUNP_ERR_UEOF;
        return err;
    }

    baseDescPos = ftell(fpSrc);
    if (baseDescPos == -1)
    {
        err = KNG_RESUNP_ERR_UEOF;
        return err;
    }
    baseFilePos = baseDescPos + (KNG_MUSRES_MAXSONGS * sizeof(struct rtsc_musres_descr));
    printf("size of first bytes: %u\n", frc);
    /*printf("size 0x%08x, %u\n", fileSz, fileSz);*/
    // {UNPACK AND WRITE TO FILES}
    // {FREE MEM}

    return err;
}

void
printErrorOccured(int err)
{
    char * desc = 0x0;
    if (err == KNG_RESUNP_SUCCESS)
        return;

    printf("Extraction failed!\n");
    switch (err)
    {
        case KNG_RESUNP_ERR_NOFILE:
            desc = "No " KNG_MUSRES_FILENAME " found!";
            break;
        case KNG_RESUNP_ERR_MALLOC:
            desc = "Memory allocation error!\n";
            break;
        case KNG_RESUNP_ERR_UEOF:
            desc = "Resource file: unexpected end of file\n";
            break;
        default:
            desc = "Unknown error";
            break;
    }
    printf("Error: %s\n", desc);
}
