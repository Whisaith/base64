#include "base64.h"

static const uint8_t base64_encode_table[64] = {

        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',

        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',

        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',

        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',

        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',

        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',

        'w', 'x', 'y', 'z', '0', '1', '2', '3',

        '4', '5', '6', '7', '8', '9', '+', '/'
};


static const uint8_t base64_decode_table[128] = {
    /*********************************************/
        255, 255, 255, 255, 255, 255, 255, 255,

    /*********************************************/
        255, 255, 255, 255, 255, 255, 255, 255,

    /*********************************************/
        255, 255, 255, 255, 255, 255, 255, 255,

    /*********************************************/
        255, 255, 255, 255, 255, 255, 255, 255,

    /*********************************************/
        255, 255, 255, 255, 255, 255, 255, 255,

    /**                '+'                 '/' **/
        255, 255, 255, 62 , 255, 255, 255, 63 ,

    /** '0'  '1'  '2'  '3'  '4'  '5'  '6'  '7' **/
        52 , 53 , 54 , 55 , 56 , 57 , 58 , 59 ,

    /** '8'  '9'                               **/
        60 , 61 , 255, 255, 255, 255, 255, 255,

    /**     'A'  'B'  'C'  'D'  'E'  'F'  'G'  **/
        255, 0  , 1  , 2  , 3  , 4  , 5  , 6  ,

    /**'H'  'I'  'J'   'K'  'L'  'M'  'N'  'O' **/
        7  , 8  , 9  , 10 , 11 , 12 , 13 , 14 ,

    /** 'P'  'Q'  'R'  'S'  'T'  'U'  'V'  'W' **/
        15 , 16 , 17 , 18 , 19 , 20 , 21 , 22 ,

    /** 'X'  'Y'  'Z'                          **/
        23 , 24 , 25 , 255, 255, 255, 255, 255,

    /**      'a'  'b'  'c'  'd'  'e'  'f'  'g' **/
        255, 26 , 27 , 28 , 29 , 30 , 31 , 32 ,

    /** 'h'  'i'  'j'  'k'  'l'  'm'  'n   'o' **/
        33 , 34 , 35 , 36 , 37 , 38 , 39 , 40 ,

    /** 'p'  'q'  'r'  's'  't'  'u'  'v'  'w' **/
        41 , 42 , 43 , 44 , 45 , 46 , 47 , 48 ,

    /** 'x'  'y'  'z'                          **/
        49 , 50 , 51 , 255, 255, 255, 255, 255,
};

uint32_t base64_encode(const uint8_t *in, uint32_t inlen, uint8_t *out)
{
    uint32_t i, j;
    uint8_t rc = 0;
    uint8_t curr_byte, prev_byte;

    for (i = 0, j = 0; i < inlen; i++)
    {
        curr_byte = in[i];
        switch (rc)
        {
            case 0:
                rc = 1;
                out[j++] = base64_encode_table[(curr_byte >> 2)];
                break;
            case 1:
                rc = 2;
                out[j++] = base64_encode_table[((prev_byte & 0x3) << 4) | ((curr_byte >> 4))];
                break;
            case 2:
                rc = 0;
                out[j++] = base64_encode_table[((prev_byte & 0xF) << 2) | ((curr_byte >> 6))];
                out[j++] = base64_encode_table[curr_byte & 0x3F];
                break;
        }
        prev_byte = curr_byte;
    }

    switch (rc)
    {
        case 1:
            out[j++] = base64_encode_table[(prev_byte & 0x3) << 4];
            out[j++] = '=';
            out[j++] = '=';
            break;
        case 2:
            out[j++] = base64_encode_table[(prev_byte & 0xF) << 2];
            out[j++] = '=';
            break;
    }

    out[j] = '\0';
    return j;
}

uint32_t base64_decode(const uint8_t *in, uint32_t inlen, uint8_t *out)
{
    uint8_t ch;
    uint32_t i, j;

    if ( (inlen == 0) || ((inlen % 4) != 0) )
    {
        return 0;
    }

    for (i = 0, j = 0; i < inlen; i++)
    {
        if (in[i] == '=')
        {
            break;
        }

        if (in[i] < '+' || in[i] > 'z')
        {
            return 0;
        }

        ch = base64_decode_table[in[i]];
        if (ch == 255)
        {
            return 0;
        }

        switch (i & 0x3)
        {
            case 0:
                out[j] = (ch << 2);
                break;
            case 1:
                out[j++] |= (ch >> 4);
                out[j] = (ch << 4);
                break;
            case 2:
                out[j++] |= (ch >> 2);
                out[j] = (ch << 6);
                break;
            case 3:
                out[j++] |= ch;
                break;
        }
    }

    return j;
}