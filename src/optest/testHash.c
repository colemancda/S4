//
//  testHash.c
//  S4
//
//  Created by vincent Moscaritolo on 11/2/15.
//  Copyright © 2015 4th-A Technologies, LLC. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "s4.h"
#include "optest.h"


S4Err TestHashKAT(
            int                 algor,
            char				*name,
            uint8_t				*msg,
            size_t				msgsize,
            int                 passes,
            uint8_t *              expected,
            size_t              expectedLen)

{
    uint8_t        hashBuf [256];
    
    uint8_t        hashState [kHASH_ContextAllocSize];
    size_t         hashStateLen = 0;
    
    S4Err err = kS4Err_NoErr;
    
    int i;
    
    HASH_ContextRef hash = kInvalidHASH_ContextRef;
    
    err = HASH_Init(algor, &hash);
    
    err = HASH_Export(hash, hashState, sizeof(hashState), &hashStateLen);
    HASH_Free(hash); hash = NULL;
    err = HASH_Import(hashState, hashStateLen, &hash); CKERR;
    ZERO(hashState, sizeof(hashState));
    
    /* calculate the hash..  */
    for (i = 0; i < passes; i++)
    {
        err = HASH_Update( hash, msg, msgsize); CKERR;
        
        err = HASH_Export(hash, hashState, sizeof(hashState), &hashStateLen);
        HASH_Free(hash); hash = NULL;
        err = HASH_Import(hashState, hashStateLen, &hash); CKERR;
        ZERO(hashState, sizeof(hashState));
        
    }
    
    err = HASH_Final (hash, hashBuf); CKERR;
    
    err = ( compareResults( expected, hashBuf, expectedLen , kResultFormat_Byte, "Message Digest")); CKERR;
    
    
    /* quick HASH API */
    if(passes == 1)
    {
        err = HASH_DO(algor, msg, msgsize, sizeof(hashBuf), hashBuf); CKERR;
        err = ( compareResults( expected, hashBuf, expectedLen , kResultFormat_Byte, "Quick HASH")); CKERR;
    }
    
    
done:
    
    if( HASH_ContextRefIsValid(hash))
        HASH_Free(hash);
    
    return err;
}



/*
 Run Hash Algorithm known answer self test
 */

S4Err TestHash()
{
    S4Err err = kS4Err_NoErr;
    
    unsigned int i;
    int last_algor = -1;
    
    
    /* Test vectors, first line from each FIPS-180 SHA-1 known answer test */
    typedef struct  {
        int                 algor;
        char				*name;
        char				*msg;
        int					passes;
        uint8_t*               kat;
        size_t              kat_len;
    } katvector;
    
    katvector kat_vector_array[] =
    {
        
        {
            kHASH_Algorithm_SHA1,
            "Short",
            "abc",
            1,
            (uint8_t*)"\xA9\x99\x3E\x36\x47\x06\x81\x6A\xBA\x3E\x25\x71\x78\x50\xC2\x6C"
            "\x9C\xD0\xD8\x9D",
            20
        },
        {
            kHASH_Algorithm_SHA1,
            "Multi",
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            1,
            (uint8_t*)"\x84\x98\x3E\x44\x1C\x3B\xD2\x6E\xBA\xAE\x4A\xA1\xF9\x51\x29\xE5"
            "\xE5\x46\x70\xF1",
            20
        },
        {
            kHASH_Algorithm_SHA1,
            "Long",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            15625,
            (uint8_t*)"\x34\xAA\x97\x3C\xD4\xC4\xDA\xA4\xF6\x1E\xEB\x2B\xDB\xAD\x27\x31"
            "\x65\x34\x01\x6F",
            20
        },
        
        {
            kHASH_Algorithm_SHA224,
            "Short",
            "abc",
            1,
            (uint8_t*)"\x23\x09\x7d\x22\x34\x05\xd8\x22\x86\x42\xa4\x77\xbd\xa2\x55\xb3"
            "\x2a\xad\xbc\xe4\xbd\xa0\xb3\xf7\xe3\x6c\x9d\xa7",
            28
        },
        
        {
            kHASH_Algorithm_SHA224,
            "Multi",
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            1,
            (uint8_t*)"\x75\x38\x8b\x16\x51\x27\x76\xcc\x5d\xba\x5d\xa1\xfd\x89\x01\x50"
            "\xb0\xc6\x45\x5c\xb4\xf5\x8b\x19\x52\x52\x25\x25",
            28
        },
        {
            kHASH_Algorithm_SHA224,
            "Long",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            15625,
            (uint8_t*)"\x20\x79\x46\x55\x98\x0c\x91\xd8\xbb\xb4\xc1\xea\x97\x61\x8a\x4b"
            "\xf0\x3f\x42\x58\x19\x48\xb2\xee\x4e\xe7\xad\x67",
            28
        },
        
        {
            kHASH_Algorithm_SHA256,
            "Short",
            "abc",
            1,
            (uint8_t*)"\xBA\x78\x16\xBF\x8F\x01\xCF\xEA\x41\x41\x40\xDE\x5D\xAE\x22\x23"
            "\xB0\x03\x61\xA3\x96\x17\x7A\x9C\xB4\x10\xFF\x61\xF2\x00\x15\xAD",
            32
        },
        
        {
            kHASH_Algorithm_SHA256,
            "Multi",
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            1,
            (uint8_t*)"\x24\x8D\x6A\x61\xD2\x06\x38\xB8\xE5\xC0\x26\x93\x0C\x3E\x60\x39"
            "\xA3\x3C\xE4\x59\x64\xFF\x21\x67\xF6\xEC\xED\xD4\x19\xDB\x06\xC1",
            32
        },
        {
            kHASH_Algorithm_SHA256,
            "Long",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            15625,
            (uint8_t*)"\xCD\xC7\x6E\x5C\x99\x14\xFB\x92\x81\xA1\xC7\xE2\x84\xD7\x3E\x67"
            "\xF1\x80\x9A\x48\xA4\x97\x20\x0E\x04\x6D\x39\xCC\xC7\x11\x2C\xD0",
            32
        },
        {
            kHASH_Algorithm_SHA384,
            "Short",
            "abc",
            1,
            (uint8_t*)"\xCB\x00\x75\x3F\x45\xA3\x5E\x8B\xB5\xA0\x3D\x69\x9A\xC6\x50\x07"
            "\x27\x2C\x32\xAB\x0E\xDE\xD1\x63\x1A\x8B\x60\x5A\x43\xFF\x5B\xED"
            "\x80\x86\x07\x2B\xA1\xE7\xCC\x23\x58\xBA\xEC\xA1\x34\xC8\x25\xA7",
            48
        },
        {
            kHASH_Algorithm_SHA384,
            "Multi",
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
            "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            1,
            (uint8_t*)"\x09\x33\x0C\x33\xF7\x11\x47\xE8\x3D\x19\x2F\xC7\x82\xCD\x1B\x47"
            "\x53\x11\x1B\x17\x3B\x3B\x05\xD2\x2F\xA0\x80\x86\xE3\xB0\xF7\x12"
            "\xFC\xC7\xC7\x1A\x55\x7E\x2D\xB9\x66\xC3\xE9\xFA\x91\x74\x60\x39",
            48
        },
        {
            kHASH_Algorithm_SHA384,
            "Long",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            15625,
            (uint8_t*)"\x9D\x0E\x18\x09\x71\x64\x74\xCB\x08\x6E\x83\x4E\x31\x0A\x4A\x1C"
            "\xED\x14\x9E\x9C\x00\xF2\x48\x52\x79\x72\xCE\xC5\x70\x4C\x2A\x5B"
            "\x07\xB8\xB3\xDC\x38\xEC\xC4\xEB\xAE\x97\xDD\xD8\x7F\x3D\x89\x85",
            48
        },
        {
            kHASH_Algorithm_SHA512,
            "Short",
            "abc",
            1,
            (uint8_t*)"\xDD\xAF\x35\xA1\x93\x61\x7A\xBA\xCC\x41\x73\x49\xAE\x20\x41\x31"
            "\x12\xE6\xFA\x4E\x89\xA9\x7E\xA2\x0A\x9E\xEE\xE6\x4B\x55\xD3\x9A"
            "\x21\x92\x99\x2A\x27\x4F\xC1\xA8\x36\xBA\x3C\x23\xA3\xFE\xEB\xBD"
            "\x45\x4D\x44\x23\x64\x3C\xE8\x0E\x2A\x9A\xC9\x4F\xA5\x4C\xA4\x9F",
            64
        },
        {
            kHASH_Algorithm_SHA512,
            "Multi",
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
            "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            1,
            (uint8_t*)"\x8E\x95\x9B\x75\xDA\xE3\x13\xDA\x8C\xF4\xF7\x28\x14\xFC\x14\x3F"
            "\x8F\x77\x79\xC6\xEB\x9F\x7F\xA1\x72\x99\xAE\xAD\xB6\x88\x90\x18"
            "\x50\x1D\x28\x9E\x49\x00\xF7\xE4\x33\x1B\x99\xDE\xC4\xB5\x43\x3A"
            "\xC7\xD3\x29\xEE\xB6\xDD\x26\x54\x5E\x96\xE5\x5B\x87\x4B\xE9\x09",
            64
            
        },
        {
            kHASH_Algorithm_SHA512,
            "Long",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            15625,
            (uint8_t*)"\xE7\x18\x48\x3D\x0C\xE7\x69\x64\x4E\x2E\x42\xC7\xBC\x15\xB4\x63"
            "\x8E\x1F\x98\xB1\x3B\x20\x44\x28\x56\x32\xA8\x03\xAF\xA9\x73\xEB"
            "\xDE\x0F\xF2\x44\x87\x7E\xA6\x0A\x4C\xB0\x43\x2C\xE5\x77\xC3\x1B"
            "\xEB\x00\x9C\x5C\x2C\x49\xAA\x2E\x4E\xAD\xB2\x17\xAD\x8C\xC0\x9B",
            64
            
        },
       
        {
            kHASH_Algorithm_SHA512_256,
            "Short",
            "abc",
            1,
            (uint8_t*)
            "\x53\x04\x8e\x26\x81\x94\x1e\xf9"
            "\x9b\x2e\x29\xb7\x6b\x4c\x7d\xab"
            "\xe4\xc2\xd0\xc6\x34\xfc\x6d\x46"
            "\xe0\xe2\xf1\x31\x07\xe7\xaf\x23",
            32
        },
        {
            kHASH_Algorithm_SHA512_256,
            "Multi",
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
            "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            1,
            (uint8_t*)
            "\x39\x28\xe1\x84\xfb\x86\x90\xf8"
            "\x40\xda\x39\x88\x12\x1d\x31\xbe"
            "\x65\xcb\x9d\x3e\xf8\x3e\xe6\x14"
            "\x6f\xea\xc8\x61\xe1\x9b\x56\x3a",
            32
        },
        
        {
            kHASH_Algorithm_SHA512_256,
            "Long",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            15625,
            (uint8_t*)
            "\x9A\x59\xA0\x52\x93\x01\x87\xA9\x70\x38\xCA\xE6\x92\xF3\x07\x08"
            "\xAA\x64\x91\x92\x3E\xF5\x19\x43\x94\xDC\x68\xD5\x6C\x74\xFB\x21",
            32
            
        },
        
        {
            kHASH_Algorithm_SKEIN256,
            "Short",
            (char*)
            "\xFF\xFE\xFD\xFC\xFB\xFA\xF9\xF8\xF7\xF6\xF5\xF4\xF3\xF2\xF1\xF0"
            "\xEF\xEE\xED\xEC\xEB\xEA\xE9\xE8\xE7\xE6\xE5\xE4\xE3\xE2\xE1\xE0",
            1,
            (uint8_t*)
            "\x8D\x0F\xA4\xEF\x77\x7F\xD7\x59\xDF\xD4\x04\x4E\x6F\x6A\x5A\xC3"
            "\xC7\x74\xAE\xC9\x43\xDC\xFC\x07\x92\x7B\x72\x3B\x5D\xBF\x40\x8B",
            32
        },
        
        {
            kHASH_Algorithm_SKEIN256,
            "Multi",
            "\xFF\xFE\xFD\xFC\xFB\xFA\xF9\xF8\xF7\xF6\xF5\xF4\xF3\xF2\xF1\xF0"
            "\xEF\xEE\xED\xEC\xEB\xEA\xE9\xE8\xE7\xE6\xE5\xE4\xE3\xE2\xE1\xE0"
            "\xDF\xDE\xDD\xDC\xDB\xDA\xD9\xD8\xD7\xD6\xD5\xD4\xD3\xD2\xD1\xD0"
            "\xCF\xCE\xCD\xCC\xCB\xCA\xC9\xC8\xC7\xC6\xC5\xC4\xC3\xC2\xC1\xC0"
            "\xBF\xBE\xBD\xBC\xBB\xBA\xB9\xB8\xB7\xB6\xB5\xB4\xB3\xB2\xB1\xB0"
            "\xAF\xAE\xAD\xAC\xAB\xAA\xA9\xA8\xA7\xA6\xA5\xA4\xA3\xA2\xA1\xA0"
            "\x9F\x9E\x9D\x9C\x9B\x9A\x99\x98\x97\x96\x95\x94\x93\x92\x91\x90"
            "\x8F\x8E\x8D\x8C\x8B\x8A\x89\x88\x87\x86\x85\x84\x83\x82\x81\x80",
            1,
            (uint8_t*)
            "\x18\x0D\xE1\x06\xA7\x04\x01\xBA\x38\xF2\x59\x7C\x25\xCB\xEF\xC7"
            "\x36\xDF\xD8\x8D\x90\xF2\xD3\x35\x2E\x0E\xB2\x55\xAF\xB6\xDB\x63",
            32
        },
        
        {
            kHASH_Algorithm_SKEIN256,
            "Long",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            15625,
            (uint8_t*)
            "\x57\x0C\x70\x90\x1E\x31\x99\x4C\x1F\x7B\x96\x0F\x3F\xBD\xCF\x8D"
            "\xB0\x03\xE5\x33\x39\x6D\x48\x38\x9F\x46\xD3\x7C\x3E\xD1\x47\x38",
            32
        },
        {
            kHASH_Algorithm_SKEIN512,
            "Short",
            (char*)
            "\xFF\xFE\xFD\xFC\xFB\xFA\xF9\xF8\xF7\xF6\xF5\xF4\xF3\xF2\xF1\xF0"
            "\xEF\xEE\xED\xEC\xEB\xEA\xE9\xE8\xE7\xE6\xE5\xE4\xE3\xE2\xE1\xE0",
            1,
            (uint8_t*)
            "\x0B\x7F\xD0\x53\xAE\x63\x5E\xE8\xE5\x19\x64\x6E\xB4\x1E\xA0\xCF"
            "\x7E\xA3\x40\x15\x23\x78\x06\x2F\xB2\x44\x0A\xA0\x25\x0F\xF1\x95"
            "\xFE\x32\xD9\xA0\x69\x1E\x68\xA0\xFE\xB1\x7D\xC2\x85\xAA\x67\x56"
            "\xCE\xF1\x94\x04\xE4\xDB\x92\xBF\x83\x6C\x4A\xE6\x53\x81\x50\x4A",
            64
        },
        {
            kHASH_Algorithm_SKEIN512,
            "Multi",
            "\xFF\xFE\xFD\xFC\xFB\xFA\xF9\xF8\xF7\xF6\xF5\xF4\xF3\xF2\xF1\xF0"
            "\xEF\xEE\xED\xEC\xEB\xEA\xE9\xE8\xE7\xE6\xE5\xE4\xE3\xE2\xE1\xE0"
            "\xDF\xDE\xDD\xDC\xDB\xDA\xD9\xD8\xD7\xD6\xD5\xD4\xD3\xD2\xD1\xD0"
            "\xCF\xCE\xCD\xCC\xCB\xCA\xC9\xC8\xC7\xC6\xC5\xC4\xC3\xC2\xC1\xC0"
            "\xBF\xBE\xBD\xBC\xBB\xBA\xB9\xB8\xB7\xB6\xB5\xB4\xB3\xB2\xB1\xB0"
            "\xAF\xAE\xAD\xAC\xAB\xAA\xA9\xA8\xA7\xA6\xA5\xA4\xA3\xA2\xA1\xA0"
            "\x9F\x9E\x9D\x9C\x9B\x9A\x99\x98\x97\x96\x95\x94\x93\x92\x91\x90"
            "\x8F\x8E\x8D\x8C\x8B\x8A\x89\x88\x87\x86\x85\x84\x83\x82\x81\x80",
            1,
            (uint8_t*)
            "\x91\xCC\xA5\x10\xC2\x63\xC4\xDD\xD0\x10\x53\x0A\x33\x07\x33\x09"
            "\x62\x86\x31\xF3\x08\x74\x7E\x1B\xCB\xAA\x90\xE4\x51\xCA\xB9\x2E"
            "\x51\x88\x08\x7A\xF4\x18\x87\x73\xA3\x32\x30\x3E\x66\x67\xA7\xA2"
            "\x10\x85\x6F\x74\x21\x39\x00\x00\x71\xF4\x8E\x8B\xA2\xA5\xAD\xB7",
            64
        },
        {
            kHASH_Algorithm_SKEIN512,
            "Long",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            15625,
            (uint8_t*)
            "\xC9\xD4\x1B\x77\xB7\x7B\x77\xE9\x54\x28\x41\x85\xAF\x68\x2A\x5A"
            "\x8B\x25\xB9\xD3\x1E\x6D\x58\xEB\x9F\xD3\x29\xF5\xBC\xCA\x34\xD7"
            "\xB2\x85\xAB\x13\x0A\x9C\x14\xC8\x72\x19\x2B\xCD\xF2\xB6\x7D\x88"
            "\x32\x80\xA7\x54\xAC\xBA\x94\x2A\x7C\xF4\x48\xE8\x41\xA7\x4E\xD2",
            64
        },
        {
            kHASH_Algorithm_SKEIN1024,
            "Short",
            (char*)
            "\xFF\xFE\xFD\xFC\xFB\xFA\xF9\xF8\xF7\xF6\xF5\xF4\xF3\xF2\xF1\xF0"
            "\xEF\xEE\xED\xEC\xEB\xEA\xE9\xE8\xE7\xE6\xE5\xE4\xE3\xE2\xE1\xE0",
            1,
            (uint8_t*)
            "\xD9\xF3\x81\xEA\xAD\xA5\x7D\x8F\x40\x7A\x01\xD8\x76\xE6\xB3\xC2"
            "\x09\x34\x18\xA4\x86\x04\x5F\x7C\xE2\x3A\x90\x15\x0D\x93\x16\x01"
            "\x3B\xB5\x4E\x56\x38\xB3\x72\xE3\x75\x59\x72\x89\xCF\x74\x50\xEB"
            "\x47\x89\xB5\x55\x3E\x2B\x29\x47\xD2\xAA\x81\x09\x7F\x4A\x8E\x84"
            "\xD3\x9E\x0F\xCA\x2F\x30\xB5\xEE\x7A\x8E\xD7\x3C\x31\xF7\x8B\x58"
            "\x04\xB6\xEF\x79\xF5\x7F\xE4\x43\xAF\xBA\x11\x51\xCC\x48\xE0\x19"
            "\x1A\xC6\x25\xE9\xD5\xF7\x2B\x84\x3D\x77\x10\xB2\x9E\x7F\x98\x9D"
            "\x8D\x3F\xC2\x1B\xBA\x49\xD4\x6B\x9F\x75\xA0\x7B\x22\x08\x67\x3C",
            128
        },
        {
            kHASH_Algorithm_SKEIN1024,
            "Multi",
            "\xFF\xFE\xFD\xFC\xFB\xFA\xF9\xF8\xF7\xF6\xF5\xF4\xF3\xF2\xF1\xF0"
            "\xEF\xEE\xED\xEC\xEB\xEA\xE9\xE8\xE7\xE6\xE5\xE4\xE3\xE2\xE1\xE0"
            "\xDF\xDE\xDD\xDC\xDB\xDA\xD9\xD8\xD7\xD6\xD5\xD4\xD3\xD2\xD1\xD0"
            "\xCF\xCE\xCD\xCC\xCB\xCA\xC9\xC8\xC7\xC6\xC5\xC4\xC3\xC2\xC1\xC0"
            "\xBF\xBE\xBD\xBC\xBB\xBA\xB9\xB8\xB7\xB6\xB5\xB4\xB3\xB2\xB1\xB0"
            "\xAF\xAE\xAD\xAC\xAB\xAA\xA9\xA8\xA7\xA6\xA5\xA4\xA3\xA2\xA1\xA0"
            "\x9F\x9E\x9D\x9C\x9B\x9A\x99\x98\x97\x96\x95\x94\x93\x92\x91\x90"
            "\x8F\x8E\x8D\x8C\x8B\x8A\x89\x88\x87\x86\x85\x84\x83\x82\x81\x80",
            1,
            (uint8_t*)
            "\x1F\x3E\x02\xC4\x6F\xB8\x0A\x3F\xCD\x2D\xFB\xBC\x7C\x17\x38\x00"
            "\xB4\x0C\x60\xC2\x35\x4A\xF5\x51\x18\x9E\xBF\x43\x3C\x3D\x85\xF9"
            "\xFF\x18\x03\xE6\xD9\x20\x49\x31\x79\xED\x7A\xE7\xFC\xE6\x9C\x35"
            "\x81\xA5\xA2\xF8\x2D\x3E\x0C\x7A\x29\x55\x74\xD0\xCD\x7D\x21\x7C"
            "\x48\x4D\x2F\x63\x13\xD5\x9A\x77\x18\xEA\xD0\x7D\x07\x29\xC2\x48"
            "\x51\xD7\xE7\xD2\x49\x1B\x90\x2D\x48\x91\x94\xE6\xB7\xD3\x69\xDB"
            "\x0A\xB7\xAA\x10\x6F\x0E\xE0\xA3\x9A\x42\xEF\xC5\x4F\x18\xD9\x37"
            "\x76\x08\x09\x85\xF9\x07\x57\x4F\x99\x5E\xC6\xA3\x71\x53\xA5\x78",
            128
        },
        {
            kHASH_Algorithm_SKEIN1024,
            "Long",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            15625,
            (uint8_t*)
            "\x6C\xB2\x95\x4A\xED\xDE\xDC\x50\x67\x00\x8D\xB1\x94\x53\x33\xBD"
            "\x9C\xFE\x9E\x46\xC1\x07\x57\x23\xCB\x57\xC5\x6C\x1C\xC3\x76\xA8"
            "\xCE\x4D\xA1\x95\xF4\x2E\x01\xBC\x00\x98\xB4\x81\x7A\xB6\x86\xB9"
            "\x81\xC9\x37\xE0\x53\x7E\x11\xED\xDF\x62\xF0\x44\x22\xAE\x6F\x1E"
            "\xDA\x87\xEF\x09\x29\x9D\x87\x85\x2F\xAF\x89\xB6\xCA\xE3\xD3\x6E"
            "\x63\x91\x40\x69\x80\x6F\x82\x3B\xBC\xE4\x36\xB3\xC5\xF4\xAB\x6C"
            "\x8A\xA6\x93\x16\x46\x7C\x59\xA1\xCB\xDB\xC3\x5D\xD9\x74\x40\x32"
            "\x3C\x4C\x70\xEF\x17\xFB\x1B\xFC\xFC\x4E\xF6\xE3\xE1\xD7\xA2\x60",
            128
        },
        
        
    };
    
    OPTESTLogInfo("\nTesting Secure Hash Algorithms:\n");
    
     /* run test array */
    for (i = 0; i < sizeof(kat_vector_array)/ sizeof(katvector) ; i++)
    {
        katvector* kat = &kat_vector_array[i];
        unsigned long length = strlen( (const char *) kat->msg) *  kat->passes ;
        
        if(last_algor != kat->algor)
        {
            OPTESTLogInfo("%s\t%10s\t",
                          (last_algor != -1 ?"\n":""),
                          hash_algor_table(kat->algor));
            last_algor = kat->algor;
        }

        OPTESTLogInfo("%6lu%s",  length > 999?length/1000:length, length > 999?"K":"");
        
        err = TestHashKAT(kat_vector_array[i].algor,
                          kat_vector_array[i].name,
                          (uint8_t*) kat_vector_array[i].msg,
                          strlen( (const char *) kat_vector_array[i].msg), 
                          kat_vector_array[i].passes,
                          kat_vector_array[i].kat,
                          kat_vector_array[i].kat_len);  CKERR;
    }
    
    OPTESTLogInfo("\n\n");
    
done:
    
    return( err );
}
