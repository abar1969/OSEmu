#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "des.h"
#include "emulator.h"
#include "globals.h"

// Cryptoworks EMU
static char crw_0D00C000[]={0x22,0xEC,0xB8,0xB2,0x43,0x85,0xC3,0xB2,0x94,0x1E,0xF7,0xEC,0xC2,0xB5,0x4A,0x09}; // DigiTurk
static char crw_0D00C001[]={0xC0,0xC6,0x86,0x55,0xB6,0x33,0x04,0x08,0x5B,0x5F,0x72,0x2D,0xD4,0x71,0xFE,0x27}; // DigiTurk
static char crw_0D00C006[]={0x01,0x56,0x12,0xE8,0xEE,0x33}; // DigiTurk
static char crw_0D00C400[]={0x22,0xEC,0xB8,0xB2,0x43,0x85,0xC3,0xB2,0x94,0x1E,0xF7,0xEC,0xC2,0xB5,0x4A,0x09}; // DigiTurk
static char crw_0D00C401[]={0xC0,0xC6,0x86,0x55,0xB6,0x33,0x04,0x08,0x5B,0x5F,0x72,0x2D,0xD4,0x71,0xFE,0x27}; // DigiTurk
static char crw_0D00C406[]={0x01,0x56,0x12,0xE8,0xEE,0x33}; // DigiTurk
static char crw_0D028C00[]={0x6E,0x75,0x6E,0x64,0x85,0x88,0x6E,0x9B,0x08,0x59,0xC4,0x03,0xEA,0xFB,0xD1,0x8A}; // UPC 0.8°W
static char crw_0D028C01[]={0xA9,0xE9,0x97,0x70,0xCC,0x04,0x50,0x48,0x05,0xA5,0x1F,0xEC,0x67,0x9D,0x03,0x28}; // UPC 0.8°W
static char crw_0D02A000[]={0x30,0x72,0x49,0xC2,0xE6,0xCA,0x1B,0xFC,0x69,0x8C,0x79,0xB4,0xC6,0x4E,0x81,0x0C}; // UPC 0.8°W
static char crw_0D02A006[]={0xC4,0xFF,0xB3,0x30,0x79,0x5B}; // UPC 0.8°W
static char crw_0D02A400[]={0x6B,0x9C,0x61,0xF2,0xAD,0xF7,0x6C,0x7F,0x23,0xCF,0x10,0xE2,0xE4,0xD5,0xF5,0xE2}; // UPC 0.8°W
static char crw_0D02A401[]={0x3E,0x55,0x24,0xFC,0x02,0x23,0x8F,0x1B,0xE1,0x5B,0x45,0xA9,0x6D,0x52,0xCC,0xF5}; // UPC 0.8°W
static char crw_0D02A800[]={0xC2,0x29,0xA4,0x03,0x61,0x00,0xE8,0x1A,0x42,0xBA,0xC3,0xC7,0x0E,0xBB,0xC8,0x52}; // UPC 0.8°W
static char crw_0D02A801[]={0xC2,0x29,0xA4,0x03,0x61,0x00,0xE8,0x1A,0x42,0xBA,0xC3,0xC7,0x0E,0xBB,0xC8,0x52}; // UPC 0.8°
static char crw_0D030400[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x19,0xE8,0xF5,0x24,0xC6,0xC0,0x53,0xE4}; // Sky Link 23.5E
static char crw_0D030406[]={0x27,0xB4,0x88,0x34,0x8D,0x54}; // Sky Link 23.5E
static char crw_0D030801[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0xF0,0x7B,0xE9,0x20,0xC7,0xE0,0xF3}; // Sky Link 23.5E
static char crw_0D030806[]={0x27,0xB4,0x88,0x34,0x8D,0x54}; // Sky Link 23.5E
static char crw_0D050400[]={0xA9,0xD7,0x32,0xF5,0xE7,0x38,0xE4,0x8A,0x91,0xC8,0x63,0x8B,0x12,0x74,0x66,0x61}; // ORF
static char crw_0D050406[]={0x09,0x87,0xCF,0x2A,0x61,0x1D}; // ORF
static char crw_0D051000[]={0x00,0x00,0x6A,0xB3,0x25,0xE6,0xAB,0xF4,0xDA,0x47,0x28,0x0F,0x35,0x29,0x4B,0x5C}; // Austriasat
static char crw_0D051006[]={0x09,0x87,0xCF,0x2A,0x61,0x1D}; // ORF

char GetKey(unsigned char *buf, uint32_t CAID, unsigned char ident) {
	if ((CAID>>4)== 0xD02A) ident &=0xFE; else
	if ((CAID>>4)== 0xD00C) CAID = 0x0D00C0; else
	if (ident==6 && ((CAID>>8) == 0x0D05)) CAID = 0x0D0504;
	switch ((CAID << 8) + ident) {
		case 0x0D00C000 : memcpy(buf,crw_0D00C000,16);	return 1; // DigiTurk
		case 0x0D00C001 : memcpy(buf,crw_0D00C001,16);	return 1; // DigiTurk
		case 0x0D00C006 : memcpy(buf,crw_0D00C006,6);	return 1; // DigiTurk
		case 0x0D00C400 : memcpy(buf,crw_0D00C400,16);	return 1; // DigiTurk
		case 0x0D00C401 : memcpy(buf,crw_0D00C401,16);	return 1; // DigiTurk
		case 0x0D00C406 : memcpy(buf,crw_0D00C406,6);   return 1; // DigiTurk
		case 0x0D028C00 : memcpy(buf,crw_0D028C00,16);  return 1; // UPC 0.8°W
		case 0x0D028C01 : memcpy(buf,crw_0D028C01,16);  return 1; // UPC 0.8°W
		case 0x0D02A000 : memcpy(buf,crw_0D02A000,16);  return 1; // UPC 0.8°W
		case 0x0D02A006 : memcpy(buf,crw_0D02A006,6);	return 1; // UPC 0.8°W
		case 0x0D02A400 : memcpy(buf,crw_0D02A400,16);  return 1; // UPC 0.8°W
		case 0x0D02A401 : memcpy(buf,crw_0D02A401,16);  return 1; // UPC 0.8°W
		case 0x0D02A800 : memcpy(buf,crw_0D02A800,16);  return 1; // UPC 0.8°W
		case 0x0D02A801 : memcpy(buf,crw_0D02A801,16);  return 1; // UPC 0.8°W
		case 0x0D030400 : memcpy(buf,crw_0D030400,16);	return 1; // Sky Link 23.5E
		case 0x0D030406 : memcpy(buf,crw_0D030406,6);	return 1; // Sky Link 23.5E
		case 0x0D030801 : memcpy(buf,crw_0D030801,16);	return 1; // Sky Link 23.5E
		case 0x0D030806 : memcpy(buf,crw_0D030806,6);	return 1; // Sky Link 23.5E
		case 0x0D050400 : memcpy(buf,crw_0D050400,16);	return 1; // ORF
		case 0x0D050406 : memcpy(buf,crw_0D050406,6);	return 1; // ORF
		case 0x0D051000 : memcpy(buf,crw_0D051000,16);  return 1; // Austriasat
		case 0x0D051006 : memcpy(buf,crw_0D051006,6);   return 1; // Austriasat
	}
	return 0;
}

static const unsigned char cw_sbox1[64] =
{
    0xD8,0xD7,0x83,0x3D,0x1C,0x8A,0xF0,0xCF,0x72,0x4C,0x4D,0xF2,0xED,0x33,0x16,0xE0,
    0x8F,0x28,0x7C,0x82,0x62,0x37,0xAF,0x59,0xB7,0xE0,0x00,0x3F,0x09,0x4D,0xF3,0x94,
    0x16,0xA5,0x58,0x83,0xF2,0x4F,0x67,0x30,0x49,0x72,0xBF,0xCD,0xBE,0x98,0x81,0x7F,
    0xA5,0xDA,0xA7,0x7F,0x89,0xC8,0x78,0xA7,0x8C,0x05,0x72,0x84,0x52,0x72,0x4D,0x38
};
static const unsigned char cw_sbox2[64] =
{
    0xD8,0x35,0x06,0xAB,0xEC,0x40,0x79,0x34,0x17,0xFE,0xEA,0x47,0xA3,0x8F,0xD5,0x48,
    0x0A,0xBC,0xD5,0x40,0x23,0xD7,0x9F,0xBB,0x7C,0x81,0xA1,0x7A,0x14,0x69,0x6A,0x96,
    0x47,0xDA,0x7B,0xE8,0xA1,0xBF,0x98,0x46,0xB8,0x41,0x45,0x9E,0x5E,0x20,0xB2,0x35,
    0xE4,0x2F,0x9A,0xB5,0xDE,0x01,0x65,0xF8,0x0F,0xB2,0xD2,0x45,0x21,0x4E,0x2D,0xDB
};
static const unsigned char cw_sbox3[64] =
{
    0xDB,0x59,0xF4,0xEA,0x95,0x8E,0x25,0xD5,0x26,0xF2,0xDA,0x1A,0x4B,0xA8,0x08,0x25,
    0x46,0x16,0x6B,0xBF,0xAB,0xE0,0xD4,0x1B,0x89,0x05,0x34,0xE5,0x74,0x7B,0xBB,0x44,
    0xA9,0xC6,0x18,0xBD,0xE6,0x01,0x69,0x5A,0x99,0xE0,0x87,0x61,0x56,0x35,0x76,0x8E,
    0xF7,0xE8,0x84,0x13,0x04,0x7B,0x9B,0xA6,0x7A,0x1F,0x6B,0x5C,0xA9,0x86,0x54,0xF9
};
static const unsigned char cw_sbox4[64] =
{
    0xBC,0xC1,0x41,0xFE,0x42,0xFB,0x3F,0x10,0xB5,0x1C,0xA6,0xC9,0xCF,0x26,0xD1,0x3F,
    0x02,0x3D,0x19,0x20,0xC1,0xA8,0xBC,0xCF,0x7E,0x92,0x4B,0x67,0xBC,0x47,0x62,0xD0,
    0x60,0x9A,0x9E,0x45,0x79,0x21,0x89,0xA9,0xC3,0x64,0x74,0x9A,0xBC,0xDB,0x43,0x66,
    0xDF,0xE3,0x21,0xBE,0x1E,0x16,0x73,0x5D,0xA2,0xCD,0x8C,0x30,0x67,0x34,0x9C,0xCB
};
static const unsigned char AND_bit1[8] = {0x00,0x40,0x04,0x80,0x21,0x10,0x02,0x08};
static const unsigned char AND_bit2[8] = {0x80,0x08,0x01,0x40,0x04,0x20,0x10,0x02};
static const unsigned char AND_bit3[8] = {0x82,0x40,0x01,0x10,0x00,0x20,0x04,0x08};
static const unsigned char AND_bit4[8] = {0x02,0x10,0x04,0x40,0x80,0x08,0x01,0x20};

void CW_SWAP_KEY(unsigned char *key)
{
    unsigned char k[8];
    memcpy(k, key, 8);
    memcpy(key, key + 8, 8);
    memcpy(key + 8, k, 8);
}
void CW_SWAP_DATA(unsigned char *k)
{
    unsigned char d[4];
    memcpy(d, k + 4, 4);
    memcpy(k + 4 ,k ,4);
    memcpy(k, d, 4);
}
void CW_DES_ROUND(unsigned char *d,unsigned char *k)
{
    unsigned char aa[44] = {1,0,3,1,2,2,3,2,1,3,1,1,3,0,1,2,3,1,3,2,2,0,7
    ,6,5,4,7,6,5,7,6,5,6,7,5,7,5,7,6,6,7,5,4,4},
    bb[44] = {0x80,0x08,0x10,0x02,0x08,0x40,0x01,0x20,0x40,0x80,0x04,0x10,0x04,
        0x01,0x01,0x02,0x20,0x20,0x02,0x01,
        0x80,0x04,0x02,0x02,0x08,0x02,0x10,0x80,0x01,0x20,0x08,0x80,0x01
    ,0x08,0x40,0x01,0x02,0x80,0x10,0x40,0x40,0x10,0x08,0x01},
    ff[4] = {0x02,0x10,0x04,0x04},
    l[24] = {0,2,4,6,7,5,3,1,4,5,6,7,7,6,5,4,7,4,5,6,4,7,6,5};
    unsigned char des_td[8], i, o, n, c = 1, m = 0, r = 0, *a = aa,*b = bb, *f = ff, *p1 = l,*p2 = l+8,*p3 = l+16;
    for (m = 0; m < 2; m++)
    for(i = 0; i < 4; i++)
    des_td[*p1++] = (m) ? ((d[*p2++]*2) & 0x3F) | ((d[*p3++] & 0x80) ? 0x01 : 0x00):
    (d[*p2++]/2) | ((d[*p3++] & 0x01) ? 0x80 : 0x00);
    for (i = 0; i < 8; i++)
    {
        c = (c) ? 0 : 1; r = (c) ? 6 : 7; n = (i) ? i-1 : 1;
        o = (c) ? ((k[n] & *f++) ? 1 : 0) : des_td[n];
        for (m = 1; m < r; m++)
        o = (c) ? (o*2) | ((k[*a++] & *b++) ? 0x01 : 0x00) :
        (o/2) | ((k[*a++] & *b++) ? 0x80 : 0x00);
        n = (i) ? n+1 : 0;
        des_td[n] = (c) ? des_td[n] ^ o : (o ^ des_td[n] )/4;
    }
    for( i = 0; i < 8; i++)
    {
        d[0] ^= (AND_bit1[i] & cw_sbox1[des_td[i]]);
        d[1] ^= (AND_bit2[i] & cw_sbox2[des_td[i]]);
        d[2] ^= (AND_bit3[i] & cw_sbox3[des_td[i]]);
        d[3] ^= (AND_bit4[i] & cw_sbox4[des_td[i]]);
    }
    CW_SWAP_DATA(d);
}
void CW_48_Key(unsigned char *inkey, unsigned char *outkey, unsigned char algotype)
{
    unsigned char Round_Counter ,i = 8,*key128 = inkey, *key48 = inkey + 0x10;
    Round_Counter = 7 - (algotype & 7);
    memset (outkey, 0, 16);
    memcpy(outkey, key48, 6);
    for( ; i > Round_Counter;i--)
    if (i > 1)
    outkey[i-2] = key128[i];
}
void CW_LS_DES_KEY(unsigned char *key,unsigned char Rotate_Counter)
{
    unsigned char round[] = {1,2,2,2,2,2,2,1,2,2,2,2,2,2,1,1}, i, n;
    unsigned short k[8];
    n = round[Rotate_Counter];
    for (i = 0; i < 8; i++) k[i] = key[i];
    for (i = 1; i < n + 1; i++)
    {
        k[7] = (k[7]*2) | ((k[4] & 0x008) ? 1 : 0);
        k[6] = (k[6]*2) | ((k[7] & 0xF00) ? 1 : 0); k[7] &=0xff;
        k[5] = (k[5]*2) | ((k[6] & 0xF00) ? 1 : 0); k[6] &=0xff;
        k[4] = ((k[4]*2) | ((k[5] & 0xF00) ? 1 : 0)) & 0xFF; k[5] &= 0xff;
        k[3] = (k[3]*2) | ((k[0] & 0x008) ? 1 : 0);
        k[2] = (k[2]*2) | ((k[3] & 0xF00) ? 1 : 0); k[3] &= 0xff;
        k[1] = (k[1]*2) | ((k[2] & 0xF00) ? 1 : 0); k[2] &= 0xff;
        k[0] = ((k[0]*2) | ((k[1] & 0xF00) ? 1 : 0)) & 0xFF; k[1] &= 0xff;
    }
    for (i = 0; i < 8; i++) key[i] = (unsigned char) k[i];
}
void CW_RS_DES_KEY(unsigned char *k, unsigned char Rotate_Counter)
{
    unsigned char i,c;
    for (i = 1; i < Rotate_Counter+1; i++)
    {
        c = (k[3] & 0x10) ? 0x80 : 0;
        k[3] /= 2; if (k[2] & 1) k[3] |= 0x80;
        k[2] /= 2; if (k[1] & 1) k[2] |= 0x80;
        k[1] /= 2; if (k[0] & 1) k[1] |= 0x80;
        k[0] /= 2; k[0] |= c ;
        c = (k[7] & 0x10) ? 0x80 : 0;
        k[7] /= 2; if (k[6] & 1) k[7] |= 0x80;
        k[6] /= 2; if (k[5] & 1) k[6] |= 0x80;
        k[5] /= 2; if (k[4] & 1) k[5] |= 0x80;
        k[4] /= 2; k[4] |= c;
        c=0;
    }
}
void CW_RS_DES_SUBKEY(unsigned char *k, unsigned char Rotate_Counter)
{
    unsigned char round[] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
    CW_RS_DES_KEY(k, round[Rotate_Counter]);
}
void CW_PREP_KEY(unsigned char *key )
{
    unsigned char DES_key[8],j;
    int Round_Counter = 6,i,a;
    key[7] = 6;
    memset(DES_key, 0 , 8);
    do
    {
        a = 7;
        i = key[7];
        j = key[Round_Counter];
        do
        {
            DES_key[i] = ( (DES_key[i] * 2) | ((j & 1) ? 1: 0) ) & 0xFF;
            j /=2;
            i--;
            if (i < 0) i = 6;
            a--;
        } while (a >= 0);
        key[7] = i;
        Round_Counter--;
    } while ( Round_Counter >= 0 );
    a = DES_key[4];
    DES_key[4] = DES_key[6];
    DES_key[6] = a;
    DES_key[7] = (DES_key[3] * 16) & 0xFF;
    memcpy(key,DES_key,8);
    CW_RS_DES_KEY(key,4);
}
void CW_L2DES(unsigned char *data, unsigned char *key, unsigned char algo)
{
    unsigned char i, k0[22], k1[22];
    memcpy(k0,key,22);
    memcpy(k1,key,22);
    CW_48_Key(k0, k1,algo);
    CW_PREP_KEY(k1);
    for (i = 0; i< 2; i++)
    {
        CW_LS_DES_KEY( k1,15);
        CW_DES_ROUND( data ,k1);
    }
}
void CW_R2DES(unsigned char *data, unsigned char *key, unsigned char algo)
{
    unsigned char i, k0[22],k1[22];
    memcpy(k0,key,22);
    memcpy(k1,key,22);
    CW_48_Key(k0, k1, algo);
    CW_PREP_KEY(k1);
    for (i = 0;i< 2; i++)
    CW_LS_DES_KEY(k1,15);
    for (i = 0;i< 2; i++)
    {
        CW_DES_ROUND( data ,k1);
        CW_RS_DES_SUBKEY(k1,1);
    }
    CW_SWAP_DATA(data);
}
void CW_DES(unsigned char *data, unsigned char *inkey, unsigned char m)
{
    unsigned char key[22], i;
    memcpy(key, inkey + 9, 8);
    CW_PREP_KEY( key );
    for (i = 16; i > 0; i--)
    {
        if (m == 1) CW_LS_DES_KEY(key, (unsigned char) (i-1));
        CW_DES_ROUND( data ,key);
        if (m == 0) CW_RS_DES_SUBKEY(key, (unsigned char) (i-1));
    }
}
void CW_DEC_ENC(unsigned char *d, unsigned char *k, unsigned char a,unsigned char m)
{
    unsigned char n = m & 1;
    CW_L2DES(d , k, a);
    CW_DES (d , k, n);
    CW_R2DES(d , k, a);
    if (m & 2) CW_SWAP_KEY(k);
}
void CW_DEC(unsigned char *d, unsigned char *key, unsigned char algo)
{
    unsigned char k0[22], algo_type = algo & 7,mode = 0, i;
    memcpy(k0,key,22);
    if (algo_type < 7)
    CW_DEC_ENC(d , k0, algo_type, mode);
    else
    for (i = 0; i < 3; i++)
    {
        mode = !mode;
        CW_DEC_ENC(d , k0, algo_type ,(unsigned char) (mode | 2));
    }
}
void CW_ENC(unsigned char *d, unsigned char *key, unsigned char algo)
{
    unsigned char k0[22], algo_type = algo & 7,mode = 1, i;
    memcpy(k0,key,22);
    if (algo_type < 7)
    CW_DEC_ENC(d , k0, algo_type, mode);
    else
    for (i = 0; i < 3; i++)
    {
        mode = !mode;
        CW_DEC_ENC(d , k0, algo_type , (unsigned char) (mode | 2));
    }
}

unsigned char CW_PDUSDEC(unsigned char* d, int l)
{
    unsigned char i = 0, pi, li, buf[128];
    memset(buf,0,sizeof(buf));
    while ( i < l )
    {
        pi = d[i];
        li = d[i + 1];
        if (i == 0) li = 1;
        switch (pi)
        {
            case 0x86 :
            {
                memcpy(&d[i], &d[i + 2 + li],l-i-li+2);
                l -= (li +2);
                d[2] -= (li +2);
                continue;
            }
            case 0xdf : return l;
        }
        i += ( 2 + li );
    }
    return l;
}
void CW_PDUDEC(unsigned char* d, int l, unsigned char* k )
{
    unsigned char pi, li, i = 0, j,key[22], algo = d[0], buf[250];
    memcpy(key,k,22);
    memcpy(buf, d, l);
    memset(d,0,l);
    l = CW_PDUSDEC(buf,l);
    memcpy(d,buf,l);
    while ( i < l )
    {
        pi = d[i];
        li = d[i + 1];
        if (i == 0 && li == 0) li = 1;
        if(pi == 0xdf) return;
        if ( ( pi == 0xda ) || ( pi == 0xdb ) || ( pi == 0xdc ) )
        for ( j = 0; j < ( li / 8 ); j++ )
        CW_DEC(&d[i + 2 + ( j * 8 )], key, algo);
        i += ( 2 + li );
    }
    return;
}

char CW_DCW(unsigned char *d ,unsigned char l , unsigned char *dcw)
{
    unsigned char i = 0, pi, li;
    while ( i < l )
    {
        pi = d[i];
        li = d[i + 1];
        if (i == 0) li = 1;
        if ( pi == 0xdf ) return 5;
        if ( pi == 0xdb ) {
	    memcpy(dcw,&d[i+2], li);
	    if(dcw[3]==((dcw[0]+dcw[1]+dcw[2])&0xFF))
	     if(dcw[7]==((dcw[4]+dcw[5]+dcw[6])&0xFF))
	      if(dcw[11]==((dcw[8]+dcw[9]+dcw[10])&0xFF))
	       if(dcw[15]==((dcw[12]+dcw[13]+dcw[14])&0xFF))
		return 0;
	    return 6;
        }
        i += ( 2 + li );
    }
    return 4;
}

#define DES_LEFT	    0
#define DES_IP              1
#define DES_IP_1            2
#define DES_RIGHT           4
#define DES_HASH            8
#define DES_ECS2_DECRYPT    (DES_IP | DES_IP_1 | DES_RIGHT)
#define DES_ECS2_CRYPT      (DES_IP | DES_IP_1) 

void cryptoworks_3des(unsigned char *data, unsigned char *key) {
	doPC1(key);
	doPC1(&key[8]);
	des(key,DES_ECS2_DECRYPT,data);
	des(&key[8],DES_ECS2_CRYPT,data);
	des(key,DES_ECS2_DECRYPT,data);
}

char nano80(unsigned char *buf, unsigned char *key, unsigned char *ECM, unsigned char len) {
  unsigned char t[16],b[16],k[16],dat1[16],dat2[16],deskey[16],i,j;
  memset(t,0,16);
  memcpy(b,buf,8);
  memcpy(k,key,16);
  cryptoworks_3des(b,k);
  memcpy(deskey,b,8);
  memcpy(&k[8],key,8);
  memcpy(k,&key[8],8);
  memcpy(b,buf,8);
  cryptoworks_3des(b,k);
  memcpy(&deskey[8],b,8);
  for(i=8;i<len;i+=8) {
    memcpy(dat1,&buf[i],8);
    memcpy(dat2,dat1,8);
    memcpy(k,deskey,16);
    cryptoworks_3des(dat1,k);
    for(j=0;j<8;j++)
        dat1[j] ^= t[j];
    if(i==8)
	if (dat1[2]!=(len-11)) return 0;
    memcpy(&ECM[i-3],dat1,8);
    memcpy(t,dat2,8);
  }
  ECM[0] = 0x80; ECM[1] = 0x70; ECM[2] = len - 6; ECM[3] = 0x81; ECM[4] = 0xFF;
  return 1;
}

char CryptoworksDec(unsigned char *cw, unsigned char *ecm, uint32_t CAID)
{
    unsigned char key[22],prov=-1, can8060=0 ,i , len, keyid=-1;

    ecm += 5; len = ecm[2] + 3;

    for (i = 3; i < len;) {
	    if(ecm[i]==0x84){
		    can8060 = ecm[i + 3];
	            break;
	    } else if(ecm[i]==0x83) {
 	            prov = (ecm[i + 2]) & 0xFC;
        	    keyid = (ecm[i + 2]) & 3;
	    }
        i += ecm[i + 1] + 2;
    }
    if(!(GetKey(key+16,(CAID<<8) | (prov),0x06)))
        return -2;
    if(can8060==0x01 && ecm[3]==0x80)
    if((CAID==0x0D00 && (prov&0xF0)==0xC0) || (CAID==0x0D02 && (prov&0xF0)==0xA0)){
        if(!(GetKey(key,(CAID<<8) | prov, 1 - (keyid>0)))) return 2;
	if (!nano80(&ecm[5],key,ecm,ecm[4])) return 3;
	ecm += 5; len = ecm[2] + 3;
    }
    if(!(GetKey(key,(CAID<<8) | (prov),keyid))) return 2;
    CW_PDUDEC (ecm, len-10, key);
    return CW_DCW(ecm, len-10, cw);
}

char CryptoworksProcessECM(unsigned char *ecm, unsigned char *dw) {
	uint32_t caid;
	if (ecm[2]==0x99 && ecm[0x73]==0x0B && ecm[0x74]==0x05)
		caid = 0x0D03; else
	if (ecm[2]==0x99 && ecm[0x73]==0x08 && ecm[0x74]==0x00)
		caid = 0x0D05; else
	if (ecm[2]>=0x6C && (ecm[ecm[9]+12]&0xF0)==0xA0)
	        caid = 0x0D02; else
	if (ecm[2]>=0x6C && (ecm[ecm[9]+12]&0xF0)==0xC0)
		caid = 0x0D00; else
	return 1;
	return CryptoworksDec(dw,ecm,caid);
}

// SoftNDS EMU
const unsigned char nds_const[]={0x0F,0x1E,0x2D,0x3C,0x4B,0x5A,0x69,0x78,0x87,0x96,0xA5,0xB4,0xC3,0xD2,0xE1,0xF0};
// Viaccess-2.3 EMU
const unsigned char viasat_key[]=
{
	0x15,0x85,0xC5,0xE4,0xB8,0x52,0xEC,0xF7,0xC3,0xD9,0x08,0xBA,0x22,0x4A,0x66,0xF2,
	0x82,0x15,0x4F,0xB2,0x18,0x48,0x63,0x97,0xDC,0x19,0xD8,0x51,0x9A,0x39,0xFC,0xCA,
	0x1C,0x24,0xD0,0x65,0xA9,0x66,0x2D,0xD6,0x53,0x3B,0x86,0xBA,0x40,0xEA,0x4C,0x6D,
	0xD9,0x1E,0x41,0x14,0xFE,0x15,0xAF,0xC3,0x18,0xC5,0xF8,0xA7,0xA8,0x01,0x00,0x01,
};
char SoftNDSECM(unsigned char *ecm, unsigned char *dw)
{
	int i;
	unsigned char *tDW;
	unsigned char digest[16];
	MD5_CTX mdContext;
	memset(dw,0,16);
	tDW = &dw[ecm[0]==0x81 ? 8 : 0];
	if (ecm[6]!=0x21) return 1;
	MD5_Init (&mdContext);
 	MD5_Update (&mdContext, ecm+7, 10);
 	MD5_Update (&mdContext, ecm+0x20, 4);
 	MD5_Update (&mdContext, viasat_key, 0x40);
 	MD5_Update (&mdContext, nds_const, 0x10);
 	MD5_Final (digest, &mdContext);
	for (i=0; i<8; i++) tDW[i] = digest[i+8] ^ ecm[0x17+i];
	if(((tDW[0]+tDW[1]+tDW[2])&0xFF)-tDW[3])return 6;
	if(((tDW[4]+tDW[5]+tDW[6])&0xFF)-tDW[7])return 6;
	return 0;
}

/* Error codes
0	OK
1	ECM not supported	
2	Key not found
3	Nano80 problem
4	Corrupt data
5	CW not found
6	CW checksum error
*/

char ProcessECM(uint16_t CAID, unsigned char *ecm, unsigned char *dw) {
	if (CAID==0x090F)
		return SoftNDSECM(ecm,dw); else
	if ((CAID>>8)==0x0D)
		return CryptoworksProcessECM(ecm,dw);
	return 1;
}

