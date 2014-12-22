#ifndef _GENERIC_H_
#define _GENERIC_H_


#define I_AM_COORDINATOR   
#define I_AM_FFD
#define COOR_SHORT_ADDRESS   0x0088  
#define COOR_PAN_ID                  0x0066

//#define I_AM_RFD 
//#define RFD_SHORT_ADDRESS     0x0044


#define MY_IEEE_ADDR          0xffff000000000002     





typedef unsigned char BYTE;
typedef unsigned char UINT8;
typedef unsigned char MAC_ENUM;
typedef unsigned int  WORD;
typedef unsigned long DWORD;
//-----------------------------------------------------------
//------------系统中数据最基本的定义-----------------------
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef int int16_t;
typedef unsigned int uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef int16_t intptr_t;
typedef uint16_t uintptr_t;
typedef unsigned int size_t;
typedef int wchar_t;
typedef unsigned char bool;
//--------------------------------------------------------------------
typedef void __attribute((__progmem__)) prog_void;
typedef char __attribute((__progmem__)) prog_char;
typedef unsigned char __attribute((__progmem__)) prog_uchar;
typedef int __attribute((__progmem__)) prog_int;
typedef long __attribute((__progmem__)) prog_long;
typedef long long __attribute((__progmem__)) prog_long_long;
//-----------------------------------------------------------
typedef union _BYTE_VAL
{
    struct
    {
        unsigned int b0:1;
        unsigned int b1:1;
        unsigned int b2:1;
        unsigned int b3:1;
        unsigned int b4:1;
        unsigned int b5:1;
        unsigned int b6:1;
        unsigned int b7:1;
    } bits;
    BYTE Val;
} BYTE_VAL;

typedef union _WORD_VAL
{
    BYTE v[2];
    WORD Val;
    struct
    {
        BYTE LSB;
        BYTE MSB;
    } byte;
} WORD_VAL;

#define LSB(a)          ((a).v[0])
#define MSB(a)          ((a).v[1])

typedef union _DWORD_VAL
{
    DWORD Val;
    struct
    {
        BYTE LOLSB;
        BYTE LOMSB;
        BYTE HILSB;
        BYTE HIMSB;
    } byte;
    struct
    {
        WORD LSW;
        WORD MSW;
    } word;
    BYTE v[4];
} DWORD_VAL;
#define LOWER_LSB(a)    ((a).v[0])
#define LOWER_MSB(a)    ((a).v[1])
#define UPPER_LSB(a)    ((a).v[2])
#define UPPER_MSB(a)    ((a).v[3])
   typedef struct _QWORD
  {
     BYTE v[8];
}QWORD;


   typedef struct _LONG_ADDR
{
      union{
      BYTE v[8];
      	
      uint64_t   longaddr;
      	};
} LONG_ADDR;
typedef union _SHORT_ADDR
{
    struct
    {
        BYTE LSB;
        BYTE MSB;
    } byte;
    WORD Val;
    BYTE v[2];
} SHORT_ADDR;

//------------------------------------------------------------------------------




#endif
