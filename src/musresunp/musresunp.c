
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "musresunp.h"

#define _BSD_SOURCE

#define KNG_MUSRES_FILENAME "MUSICS.RES"
#define KNG_MUSRES_MAXSONGS 9

void printShortInfo(void);
int searchAndExtract(void);
long int getFileSize(FILE * fp);
int readFileAndUnpack(FILE * fpSrc, char * pool, long int poolSize);
void printRetCode(int err);
int writeToNewFile(char * buffer, uint32_t size, int index);

int
main(int argc, char ** argv)
{
    int err;
    printShortInfo();
    err = searchAndExtract();
    printRetCode(err);

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
    int err = KNG_RESUNP_SUCCESS;
    size_t frc;
    struct rtsc_resfile_startdesc startDescr;
    struct rtsc_musres_descr musfileDescr;
    long int baseDescPos, baseFilePos;
    int cnt;
    int rc;
    FILE * fpOut = 0x0;

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

    for (cnt = 0; cnt < KNG_MUSRES_MAXSONGS; cnt++)
    {
        rc = fseek(fpSrc, baseDescPos + cnt*(sizeof(struct rtsc_musres_descr)), SEEK_SET);
        if (rc != 0) {
            err = KNG_RESUNP_ERR_UEOF;
            break;
        }
        frc = fread(&musfileDescr, 1, sizeof(struct rtsc_musres_descr), fpSrc);
        if (frc < sizeof(struct rtsc_resfile_startdesc)) {
            err = KNG_RESUNP_ERR_UEOF;
            break;
        }
        rc = fseek(fpSrc, baseFilePos + musfileDescr.offset, SEEK_SET);
        if (rc != 0) {
            err = KNG_RESUNP_ERR_UEOF;
            break;
        }
        frc = fread(pool, 1, musfileDescr.size, fpSrc);
        if (frc < musfileDescr.size) {
            err = KNG_RESUNP_ERR_UEOF;
            break;
        }
        err = writeToNewFile(pool, musfileDescr.size, cnt);
        if (err != 0) {
            break;
        } 
    }

    return err;
}

void
printRetCode(int err)
{
    char * desc = 0x0;
    if (err == KNG_RESUNP_SUCCESS)
    {
        printf("done.\n");
        return;
    }

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

int
writeToNewFile(char * buffer, uint32_t size, int index)
{
    int rc = 0;
    size_t frc = 0;
    FILE * fp;
    enum {namebuf_size = 32};
    char name[namebuf_size];
    memset(name, 0x0, namebuf_size);

    snprintf(name, namebuf_size, "%04d%s", index, ".ogg"); 
    fp = fopen(name, "wb");
    if (fp == 0x0) {
        return KNG_RESUNP_ERR_FWRITE;
    }
    frc = fwrite(buffer, 1, size, fp);
    if (frc < size) {
        rc = KNG_RESUNP_ERR_FWRITE; 
    } else {
        rc = KNG_RESUNP_SUCCESS;
    }

    fclose(fp);

    return rc;
}
