//lite_gamma.h
#ifndef COMMON_LITE_GAMMA_H
#define COMMON_LITE_GAMMA_H

#include <fcntl.h>
#include <cstring>
#include <stdlib.h>

namespace lite_gamma {
#define DEFAULT_WP "t h $V jk $#  fFF_78 M#!~  f l j;kl oem0xFFA345 "

//unsigned int aul = 60976; // authentification file length
//unsigned int aup = 986; // authentification data pos (in file)
//char *FNF_MSG = "File not found\r\n";

/*!
 * \fn  inline void ec(void *pData, unsigned long nBytes, const char *wp, char ca = 0)
 * \brief   Encode.
 * \param [in,out]  pData   The data.
 * \param   nBytes          The ount of bytes.
 * \param   wp              The password.
 * \param   ca              The algorithm.
 */
inline void ec(
                void *pData/*pData to mem range to encrypt*/,
                unsigned long nBytes/*count of bytes*/,
                const char *wp/*password*/,
                char ca = 0/*algorithm*/) {
    if (pData == nullptr || nBytes == 0)
        return;

    char *awp; // actual pw
    unsigned long nawp, // pw length
        i1;
    int nWpLen = strlen(wp),
        nDWpLen = strlen(DEFAULT_WP);
    nawp = (nDWpLen < nWpLen) ? nWpLen : nDWpLen;
    awp = new char[nawp + 1];
    memset(awp, 0, nawp - 1);
#pragma warning(push)
#pragma warning(disable: 4995)
    strcpy(awp, DEFAULT_WP);
#pragma warning(pop)


    if (*wp != 0) {
        if (strcmp(wp, DEFAULT_WP) != 0) {
            int nStepBy = 1;
            if ((nDWpLen / nWpLen) > 1)
                nStepBy = 2;
            int nDWpPos = 0;
            for (int i = 0; i < nWpLen; i++) {
                *(awp + nDWpPos) ^= *(wp + i);
                nDWpPos += nStepBy;
            }
        }
    }
    char *x = (char*)pData;
    switch (ca) {
    case 0:
        ec(x, nBytes, awp, 1);
        ec(x, nBytes, awp, 2);
        ec(x, nBytes, awp, 3);
        break;
    case 1:
        i1 = 0;
        *x = (char)~*x;
        for (unsigned long i = 1; i < nBytes; i++) {
            *(x + i) = *(x + i) ^ * (x + i - 1);
            *(x + i) = *(x + i) ^ * (awp + i1);
            if (i1 < nawp - 1)
                i1++;
            else
                i1 = 0;
        }
        break;
    case 2:
        char c1;
        for (unsigned long i = 8; i < nBytes; i++) {
            *(x + i) = *(x + i) ^ *(x + i - 7);
            c1 = *(x + i - 2);
            *(x + i - 2) = *(x + i - 7);
            *(x + i - 7) = c1;
        }
        break;
    case 3:
        for (unsigned long i = 2; i < nBytes; i++)
            if ((i^nawp) % 2)
                *(x + i) = *(x + i) ^ *(awp + i % nawp);
            else
                *(x + i) = (char)~*(x + i);
    }
    delete[] awp;
}


/*!
 * \fn  inline void dc(void *pData, unsigned long nBytes, const char *wp, char ca = 0)
 * \brief   Decode.
 * \param [in,out]  pData   The data.
 * \param   nBytes          The count of bytes.
 * \param   wp              The password.
 * \param   ca              The algirithm.
 */
inline void dc(
                void *pData,
                unsigned long nBytes,
                const char *wp,
                char ca = 0) {
    char *awp; // actual pw
    unsigned long nawp; // pw length
    unsigned long i1;

    int nWpLen = strlen(wp),
        nDWpLen = strlen(DEFAULT_WP);
    nawp = (nDWpLen < nWpLen) ? nWpLen : nDWpLen;
    awp = new char[nawp + 1];
    memset(awp, 0, nawp - 1);
#pragma warning(push)
#pragma warning(disable: 4995)
    strcpy(awp, DEFAULT_WP);
#pragma warning(pop)

    if (*wp != 0) {
        if (strcmp(wp, DEFAULT_WP) != 0) {
            int nStepBy = 1;
            if ((nDWpLen / nWpLen) > 1)
                nStepBy = 2;
            int nDWpPos = 0;
            for (int i = 0; i < nWpLen; i++) {
                *(awp + nDWpPos) ^= *(wp + i);
                nDWpPos += nStepBy;
            }
        }
    }

    char *x = (char*)pData;
    switch (ca) {
    case 0:
        dc(x, nBytes, awp, 3);
        dc(x, nBytes, awp, 2);
        dc(x, nBytes, awp, 1);
        break;
    case 1:
        if ((nBytes - 1) % (nawp) != 0)
            i1 = (nBytes - 1) % (nawp)-1;
        else
            i1 = nawp - 1;
        for (unsigned long i = nBytes; i > 1; i--) {
            *(x + i - 1) = *(x + i - 1) ^ *(awp + i1);
            *(x + i - 1) = *(x + i - 1) ^ *(x + i - 1 - 1);
            if (i1 > 0)
                i1--;
            else
                i1 = nawp - 1;
        }
        *x = (char)~*x;
        break;
    case 2:
        char c1;
        for (unsigned long i = nBytes - 1; i > 7; i--) {
            c1 = *(x + i - 7);
            *(x + i - 7) = *(x + i - 2);
            *(x + i - 2) = c1;
            *(x + i) = *(x + i) ^ *(x + i - 7);
        }
        /*            unsigned long nawp=lstrlen(awp);
        for (unsigned long i=0;i<nBytes;i++){
        if (i<255)
        x[i]=x[i]^awp[div(i,nawp).rem];
        }
        for (unsigned long i=0;i<nBytes;i++){
        x[i]=(char)~x[i];
        }     */
        break;
    case 3:
        for (unsigned long i = nBytes; i > 2; i--)
            if (((i - 1) ^ nawp) % 2)
                *(x + i - 1) = *(x + i - 1) ^ * (awp + (i - 1) % nawp);
            else
                *(x + i - 1) = (char)~*(x + i - 1);
    }
    delete[] awp;
}

}

#endif //COMMON_LITE_GAMMA_H
