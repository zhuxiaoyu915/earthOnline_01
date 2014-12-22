#ifndef _MAC_H_
#define _MAC_H_




#include "phy.h"
#include "generic.h"

//--------------------------------------------------//
//-----------帧控制字段设置的一些预定义常数---------//
//--------------------------------------------------//
#define aMaxPHYPacketSize 127
//帧类型字段bit[2:0]
#define MAC_FRAME_BEACON                (0x00)
#define MAC_FRAME_DATA                  (0x01)
#define MAC_FRAME_ACK                   (0x02)
#define MAC_FRAME_CMD                   (0x03)
//该帧的传送是否采用MAC层的安全策略bit[3]
#define MAC_SECURITY_YES            (0x08)
#define MAC_SECURITY_NO             (0x00)
//用在间接通信中，表示还有数据没传完,需要继续发data request命令帧bit[4]
#define MAC_FRAME_PENDING_YES       (0x10)
#define MAC_FRAME_PENDING_NO        (0x00)
//对当前帧的传送，是否需要收方的应答bit[5]
#define MAC_ACK_YES                 (0x20)
#define MAC_ACK_NO                  (0x00)
//是否为同一个PAN内的传送刘bit[6]
#define MAC_INTRA_PAN_YES           (0x40)
#define MAC_INTRA_PAN_NO            (0x00)
//源地址模式定义字段bit[11:10]
#define MAC_DST_NO_ADDR             (0x00)
#define MAC_DST_SHORT_ADDR          (0x08)
#define MAC_DST_LONG_ADDR           (0x0c)
#define MAC_DST_ADDR_RESERVED       (0x04)
//目标地址模式定义字段bit[15:14]
#define MAC_SRC_NO_ADDR             (0x00)
#define MAC_SRC_SHORT_ADDR          (0x80)
#define MAC_SRC_LONG_ADDR           (0xc0)
#define MAC_SRC_ADDR_RESERVED       (0x40)
//九种命令帧类型信息字段
#define MAC_CMD_ASSOCIATE_REQ           (0x01)
#define MAC_CMD_ASSOCIATE_RPLY          (0x02)
#define MAC_CMD_DISASSOCIATE_NOTICE     (0x03)
#define MAC_CMD_DATA_REQ                (0x04)
#define MAC_CMD_PAN_ID_CONFLICT         (0x05)
#define MAC_CMD_ORPHAN_NOTICE           (0x06)
#define MAC_CMD_BEACON_REQ              (0x07)
#define MAC_CMD_COORD_REALIGNMENT       (0x08)
#define MAC_CMD_GTS_REQ                 (0x09)

#define ED_SCAN             (0x00)
#define ACTIVE_PASSIVE_SCAN     (0x01)
//#define PASSIVE_SCAN   (0x02)
#define ORPHAN_SCAN    (0x03)

//通过条件编译确定本设备是否为协调者
#ifdef I_AM_COORDINATOR
    #define IS_COORDINATOR      (0x01)      // 1 is yes, 0 if no.
    //#define IS_FFD                       (0x02)
#else
    #define IS_COORDINATOR      (0x00)
#endif
//通过条件编译确定是否采用MAC层的安全策略
#ifdef I_AM_SECURITY_CAPABLE
#define MAC_OPT_SECURITY            MAC_SECURITY_YES
#define MAC_OPT_ACL_SIZE         0x10
#else
#define MAC_OPT_SECURITY            MAC_SECURITY_NO
#define MAC_OPT_ACL_SIZE         0x00
#endif
//通过条件编译确定本设备是否为FFD设备
#ifdef I_AM_RFD
   #define IS_FFD              (0x00)      // 0x02 if yes, 0x00 if RFD
   #define IS_RFD              TRUE
#else
    #define IS_FFD              (0x02)  
   #define  IS_RFD              FALSE	
#endif

#ifdef  I_AM_FFD
   #define  IS_FFD              (0x02)
#else
   #define  IS_FFD              (0x00)   
#endif

//通过条件编译确定本设备的供电模式
#ifdef I_AM_MAINS_POWERED
   #define IS_MAINS_POWERED    (0x04)     
#else
#define IS_MAINS_POWERED    (0x00)
#endif 
   //通过条件编译确定本设备射频空闲时候，接收是否打开
   #ifdef MY_RX_IS_ALWAYS_ON_WHEN_IDLE
   #define IS_RX_ALWAYS_ON     (0x08)
   #else
   #define IS_RX_ALWAYS_ON     (0x00)
   #endif
   //通过条件编译确定本设备的MAC安全是否使能
   #ifdef I_AM_SECURITY_CAPABLE
   #define IS_SECURITY_ENABLED (0x40)
#else
   #define IS_SECURITY_ENABLED (0x00)
#endif
   //通过条件编译确定本设备是否希望协调设备给他分配短地址(pg125)
   #ifdef MAC_USE_SHORT_ADDR
   #define IS_SHORT_ADDRESS    (0x80)
#else
   #define IS_SHORT_ADDRESS    (0x00)
#endif
   //关联网络时候使用的本设备的有用信息
#define MAC_CAP_INFO      (IS_COORDINATOR        |     \  
                             IS_FFD                 |       \
                             IS_MAINS_POWERED       |       \
                             IS_RX_ALWAYS_ON        |       \
                             IS_SECURITY_ENABLED    |       \
IS_SHORT_ADDRESS);
//MAC层的枚举常数，反映MAC层各原语的状态值.(pg110)
#define SUCCESS                 0
#define BEACON_LOSS             0xE0
#define CHANNEL_ACCESS_FAILURE  0xE1
#define DENIED                  0xE2
#define DISABLE_TRX_FAILURE     0xE3
#define FAILED_SECURITY_CHECK   0xE4
#define FRAME_TOO_LONG          0xE5
#define INVALID_GTS             0xE6
#define INVALID_HANDLE          0xE7
#define INVALID_PARAMETER       0xE8
#define NO_ACK                  0xE9
#define NO_BEACON               0xEA
#define NO_DATA                 0xEB
#define NO_SHORT_ADDRESS        0xEC
#define OUT_OF_CAP              0xED
#define PAN_ID_CONFLICT         0xEE
#define REALIGNMENT             0xEF
#define TRANSACTION_EXPIRED     0xF0
#define TRANSACTION_OVERFLOW    0xF1
#define TX_ACTIVE               0xF2
#define UNAVAILABLE_KEY         0xF3
#define UNSUPPORTED_ATTRIBUTE   0xF4
#define RX_DEFERRED             0xF5

// 反映关联的状态值(pg126)
#define PAN_AT_CAPACITY         0x01
#define PAN_ACCESS_DENIED       0x02

//反映取消关联的原因状态值  (pg127)
#define COORD_WISH_TO_LEAVE     0x01
#define DIVICE_TO_LEAVE         0x02

//反映MAC层特征的一些常数
#define aBaseSlotDuration       20
#define aNumSuperframeSlots     8
#define aBaseSuperframeDuration (aBaseSlotDuration * aNumSuperframeSlots)
#define aMaxBE                  5
#define aMaxBeaconOverhead      75
#define aMaxBeaconPayloadLength (aMaxPHYPacketSize - aMaxBeaconOverhead)
#define aGTSDescPersistenceTime 4
#define aMaxFrameOverhead       25
#define aMaxFrameResponseTime   1220
#define aMaxFrameRetries        2
#define aMaxLostBeacons         4
#define aMaxMACFrameSize        (aMaxPHYPacketSize - aMaxFrameOverhead)
#define aMaxSIFSFrameSize       18
#define aMinCAPLength           440
#define aMinLIFSPeriod          40
#define aMinSIFSPeriod          12
#define aResponseWaitTime       (10 * aBaseSuperframeDuration)
//#define aUnitBackoffPeriod
//其他的涉及MAC层的常数定义
#define MAX_PENDING_LIST_SIZE   2



//反映安全策略的一些参数，包括MAC层的和PHY层的
// MAC ACL and security information bases
#ifdef I_AM_SECURITY_CAPABLE
#define MAC_SECURITY_NONE               0x00
#define MAC_SECURITY_AES_CTR            0x01
#define MAC_SECURITY_AES_CCM128         0x02
#define MAC_SECURITY_AES_CCM64          0x03
#define MAC_SECURITY_AES_CCM32          0x04
#define MAC_SECURITY_AES_CBC_MAC128     0x05
#define MAC_SECURITY_AES_CBC_MAC64      0x06
#define MAC_SECURITY_AES_CBC_MAC32      0x07
#define MAC_HIGHEST_SECURITY_MODE       MAC_SECURITY_AES_CBC_MAC32

#define MAC_UNSECURED_MODE              0x00
#define MAC_ACL_MODE                    0x01
#define MAC_SECURED_MODE                0x02

#define TX_OPTION_ACK_NEED        0x01;
#define TX_OPTION_GTS_TRANSMIT  0x02;
#define TX_OPTION_INDIRECT            0x04;
#define TX_OPTION_SECURITY           0x08;

// MAC Security flags definitions.
// Note that the bits are shifted compared to the actual security flags //defined by IEEE 802.15.4,
// please see the CC2420 datasheet for details.
#define MAC_CC2420_CTR_FLAGS            0x42
#define MAC_CC2420_CCM_FLAGS            0x09
#endif


//--------------------------------------------------//
//---下面为Zigbee中MAC层用到的专用的数据类型定义----//
//--------------------------------------------------//

//MAC层安全策略相关的类型定义
#if  ((MAC_OPT_SECURITY) || (MAC_OPT_ACL_SIZE>0))
typedef BYTE KEY[16];
typedef struct _SECURITY_MATERIAL {
    KEY pSymmetricKey;
    DWORD frameCounter;
    BYTE keySequenceCounter;
#if MAC_OPT_SEQUENTIAL_FRESHNESS
    DWORD externalFrameCounter;
    BYTE externalKeySequenceCounter;
#endif
} SECURITY_MATERIAL;

// Access Control List
typedef struct _ACL_ENTRY {
    QWORD extendedAddress;
    WORD shortAddress;
    WORD panId;
    BYTE securityMaterialLength;
    SECURITY_MATERIAL* pSecurityMaterial;
    BYTE securitySuite;
} ACL_ENTRY;
typedef ACL_ENTRY ACL_ENTRY_SET[MAC_OPT_ACL_SIZE];
   #endif
  
   //8字节长地址类型定义

//PAN地址类型定义
typedef SHORT_ADDR  PAN_ADDR;
//MAC层的PIB存储结构定义
typedef struct  _MAC_PIB{
    BYTE macAckWaitDuration;
    bool macAssociationPermit;
    bool macAutoRequest;
    bool macBattLifeExt;
    BYTE macBattLifeExtPeriods;
    BYTE* pMacBeaconPayload;
    BYTE macBeaconPayloadLength;
    uint8_t macBeaconOrder;
    DWORD macBeaconTxTime;
    BYTE macBSN;
    LONG_ADDR macCoordExtendedAddress;
    SHORT_ADDR macCoordShortAddress;
    BYTE macDSN;
    bool macGTSPermit;
    BYTE macMaxCsmaBackoffs;
    BYTE macMinBE;
    SHORT_ADDR macPANId;
    bool macPromiscuousMode;
    bool macRxOnWhenIdle;
    SHORT_ADDR macShortAddress;
    LONG_ADDR   macExtAddress;
    uint8_t macSuperframeOrder;
    WORD macTransactionPersistenceTime;

#if MAC_OPT_ACL_SIZE>0
    // ACL attributes
    ACL_ENTRY_SET* ppMacACLEntryDescriptorSet;
    BYTE macACLEntryDescriptorSetSize;
#endif

#if MAC_OPT_SECURITY
    // Security attributes
    bool macDefaultSecurity;
    BYTE macDefaultSecurityMaterialLength;
    SECURITY_MATERIAL* pMacDefaultSecurityMaterial;
    BYTE macDefaultSecuritySuite;
#endif

#if ((MAC_OPT_SECURITY) || (MAC_OPT_ACL_SIZE>0))
    BYTE macSecurityMode;
#endif
} MAC_PIB;

//给MAC PIB中的属性一个ID编号，从而用于访问
typedef enum {
    MAC_ACK_WAIT_DURATION = 0x40,
    MAC_ASSOCIATION_PERMIT,
    MAC_AUTO_REQUEST,
    MAC_BATT_LIFE_EXT,
    MAC_BATT_LIFE_EXT_PERIODS,
    MAC_BEACON_PAYLOAD,
    MAC_BEACON_PAYLOAD_LENGTH,
    MAC_BEACON_ORDER,
    MAC_BEACON_TX_TIME,
    MAC_BSN,
    MAC_COORD_EXTENDED_ADDRESS,
    MAC_COORD_SHORT_ADDRESS,
    MAC_DSN,
    MAC_GTS_PERMIT,
    MAC_MAX_CSMA_BACKOFFS,
    MAC_MIN_BE,
    MAC_PAN_ID,
    MAC_PROMISCUOUS_MODE,
    MAC_RX_ON_WHEN_IDLE,
    MAC_SHORT_ADDRESS,
    MAC_SUPERFRAME_ORDER,
    MAC_TRANSACTION_PERSISTENCE_TIME,
    MAC_ACL_ENTRY_DESCRIPTOR_SET = 0x70,
    MAC_ACL_ENTRY_DESCRIPTOR_SETSIZE,
    MAC_DEFAULT_SECURITY,
    MAC_DEFAULT_SECURITY_MATERIAL_LENGTH,
    MAC_DEFAULT_SECURITY_MATERIAL,
    MAC_DEFAULT_SECURITY_SUITE,
    MAC_SECURITY_MODE
} MAC_PIB_ATTR;




// MAC 命令帧对应状态编码
//?????????????????????????????????????????/不够区分系统MAC层的状态，还需要扩展。
typedef enum {
    MAC_STATE_UNSTARTED,
    MAC_STATE_ASSOC_STARTED,
    MAC_STATE_ASSOC_REQ_SENT,
    MAC_STATE_ASSOC_WAIT_FOR_RESPONSE,
    MAC_STATE_ASSOC_FINISHED,
  
   //5	
    MAC_STATE_ENERGY_SCAN,  
    MAC_STATE_ENERGY_SCAN_FINISHED,

	//7
    MAC_STATE_ACTIVE_PASSIVE_SCAN,
    MAC_STATE_ORPHAN_SCAN,
    MAC_STATE_ORPHAN_REALIGNED,
   //10
    MAC_STATE_ACTIVE_PASSIVE_SCAN_FINISHED,	
   //11
    MAC_STATE_ID_CONFLICT,
    MAC_STATE_REALIGNMENT,
    MAC_STATE_DISASSOCIATED,
//14
    MAC_STATE_SYNING,
    MAC_STATE_SYNING_END,
    //16
    MAC_STATE_IDLE,
    MAC_STATE_INACTIVE,

    MAC_STATE_START_TO_SLEEP,	
    //MAC_STATE_POLLING
} MAC_STATE_TYPE;


//--------------------------------------------------//
//-------下面为Zigbee中MAC层帧的数据结构定义--------//
//--------------------------------------------------//


//IEEE802.15.4帧的控制字段格式
typedef union _Frame_Control
{
    struct {
              BYTE  FrameType  : 3;
              BYTE  SecurityEnable  :1;
              BYTE  FramePending   :1;
              BYTE  AckReq         :1;
              BYTE  IntraPAN       :1;
              BYTE                 :3;
              BYTE  DestAddrMode   :2;
              BYTE                  :3;
              BYTE  SrcAddrMode    :2;
              }bits;
    BYTE v[2];
    WORD w;
}FRAME_CONTROL;

//IEEE802.15.4信标帧中的负载域(payload)格式(zigbee规范pg250，251)
typedef struct _BEACON_PAYLOAD
{
BYTE  ProtocolID;
BYTE  StackProfile   : 4;
BYTE  nwkcProtocolVersion  :4;
BYTE  Reserved_beacon_payload   :2;
BYTE  RouterCapacity   :1;
BYTE  Devicedepth ;
BYTE  EndDeviceCapacity     :1;
BYTE  TxOffSetbyte[3];

}BEACON_PAYLOAD;

//IEEE802.15.4信标帧中的pending address规范域
typedef  union _PendingAddress
{
   struct{
BYTE  NumOfShortAddress   :3;
BYTE                     :1;
BYTE  NumOfExtAddress    :3;
BYTE                      :1;
   	}bits;
   BYTE  bytes;
}PendingAddress;

   // IEEE 802.15.4 信标帧中的超帧规范域格式
typedef struct _SUPERFRAME_SPECIFICATION
{
    union
    {
        struct
        {
            BYTE    beaconOrder             : 4;
            BYTE    superframeOrder         : 4;
            BYTE    finalCAPSlot            : 4;
            BYTE    bBatteryLifeExtension   : 1;
            BYTE                            : 1;
            BYTE    bIsPANCoordinator       : 1;
            BYTE    bIsAssociationPermitted : 1;
        } fields;
        struct
        {
            BYTE    LSB;
            BYTE    MSB;
        } bytes;
        WORD    Val;
    } SF;
} SUPERFRAME_SPECIFICATION;

// IEEE 802.15.4 Beacon Frame GTS information
typedef struct _GTS_SPECIFICATION
{
    union
    {
        struct
        {
            BYTE    GTSDescriptorCount  : 3;
            BYTE                        : 3;
            BYTE    bGTSPermit          : 1;
        } fields;
        BYTE    Val;
    } GTS;
} GTS_SPECIFICATION;
//IEEE802.15.4中信标帧中GTS方向域
BYTE_VAL  GTSDirectionMask ;
//IEEE802.15.4中GTS descriptor定义
typedef struct _GTS_DESCRIPTOR
{
    SHORT_ADDR  DeviceAddr;
    BYTE          GTSStartingSlot  : 4;
    BYTE          GTSLength      :4;
}GTS_DESCRIPTOR;

//发送association request命令帧时候的节点capability信息
typedef union _Node_Capability
{
   struct{
   BYTE  Alt_PAN_Cord  :1;
   BYTE  Device_Type    :1;
   BYTE  PowerSource    :1;
   BYTE  Recv_On_Idle   :1;
   BYTE                 :2;
   BYTE  Security_Cap    :1;
   BYTE  Alloc_Addr      :1;
   	}bits;
   BYTE bytes;
}Node_Capability;

//GTS请求帧的characteristic域
typedef union  _GTS_CHARACTERISTIC
{
   struct{
   BYTE   GTS_length     : 4;
   BYTE   GTS_direction   :1;
   BYTE   GTS_characteristic_byte  : 1;
   BYTE                         : 2;
   	}bits;
   BYTE   bytes;
}GTS_CHARACTERISTIC;


// A 802.15.4 节点地址信息
typedef struct _NODE_INFO
{
    BYTE            addrMode;
    LONG_ADDR       longAddr;
    SHORT_ADDR      shortAddr;
    PAN_ADDR        panID;
} NODE_INFO;

//MAC层协议的当前帧存储的信息类型定义，这些信息要被用于转换成PSDU和计算PSDU长度或反过来从psdu转换成msdu。
//从而调用pd_data_request函数发送. 该结构主要用于MAC层对MAC帧的处理过程.
//MAC_FRAME_FORMAT是一种通用的定义,对于具体的帧,需要根据帧类型字段和长度等字段确定使用部分字段.
typedef struct _MAC_FRAME_FORMAT
{
    NODE_INFO dst;    //关于目标设备地址和目标网络的信息.
    NODE_INFO src;    //源设备地址和网络的信息.
    BYTE type;        //帧类型字段,可以用于快速访问.
    BYTE cmd;         //如果是命令帧的话,该域存储命令帧的类型信息
    union{
    BYTE macDSN;      //帧序列号
    BYTE macBSN;
    	};
    BYTE frameLength; //MAC层的帧长度,不计算FCS的总长度
    //BYTE txOptions;   //传送的策略
    BYTE framehandle;
    BYTE lqi;
    uint32_t  timestamp;
    //帧控制字段,共两个字节
    FRAME_CONTROL    frame_control_field;
    
    //由于地址域和PANID域都是以字节为单位进行,可以在形成psdu的过程中填写.
    //
    //
    
    //网络层传递过来的数据payload或信标payload指针
    uint8_t  nwk_data_length;  //即是msdulength
    uint8_t  beacon_payload_length;
    union
    {
      BYTE   nwk_data_payload[aMaxMACFrameSize];
      BYTE   beacon_payload[aMaxBeaconPayloadLength]; 
    }; 
    //capinfo主要用在association request帧中
    Node_Capability  node_capinfo_field;
    
    //如果是信标帧中用到的超帧规范域和地址pending域
    SUPERFRAME_SPECIFICATION  superframe_field;
    PendingAddress            pendaddress_field;

    uint8_t      shortpendingadd_length;
    uint8_t      longpendingadd_length;
    SHORT_ADDR                shortaddrpending_list[MAX_PENDING_LIST_SIZE];
    LONG_ADDR                 longaddrpending_list[MAX_PENDING_LIST_SIZE]; 
    
    //如果是关联响应帧中用到的分配的短地址信息和关联状态值
    SHORT_ADDR                shortaddralloc;
    BYTE                      association_status;
    
    //如果是撤销关联通知帧中可能用到的取消关联的原因
    BYTE                      disassociation_reason;
    
    //如果是协调者发出的realignment帧中用到的一些域
    SHORT_ADDR                PAN_ID_field;
    SHORT_ADDR                coordshortaddr_field;
    BYTE                      logicalchannel_field;
    SHORT_ADDR                shortaddrallocted;
    
    //如果是GTS请求帧中用到的GTS characteristic域
    GTS_CHARACTERISTIC        gts_char_field;
    
    //可能需要记录一些与当前帧相关的标志域
    /*union
    {
        struct
        {
            unsigned int bIsGetReady:1;
            unsigned int bToBeQueued:1;
        } bits;
        BYTE Val;
    } Flags;*/

} MAC_FRAME_FORMAT;

//只用于发送时候
typedef  struct _beacon_frame_format
{
    SUPERFRAME_SPECIFICATION  superframe_field;
    PendingAddress            pendaddress_field;

    uint8_t      shortpendingadd_length;
    uint8_t      longpendingadd_length;
    SHORT_ADDR                shortaddrpending_list[MAX_PENDING_LIST_SIZE];
    LONG_ADDR                 longaddrpending_list[MAX_PENDING_LIST_SIZE];   

    uint8_t  beacon_payload_length;
    BYTE   beacon_payload[aMaxBeaconPayloadLength];    	
	 
}BEACON_FRAME_FORMAT;


//--------------------------------------------------//
//-------下面为Zigbee中MAC层原语的数据类型定义------//
//--------------------------------------------------//


typedef union {
    SHORT_ADDR Short;
    LONG_ADDR Extended;
} ADDRESS;

typedef struct {
    NODE_INFO  src;
    NODE_INFO  dst;
    UINT8   mpduLinkQuality;
    bool    securityUse;
    UINT8   aclEntry;
    UINT8    msduLength;
    BYTE    pMsdu[aMaxMACFrameSize];
} MCPS_DATA_INDICATION;

typedef struct {
    //UINT8 coordAddrMode;
    //WORD coordPanId;
    UINT8 logicalChannel;
    //ADDRESS coordAddress;
    NODE_INFO  coordinator;
    SUPERFRAME_SPECIFICATION superframeSpec;
    bool gtsPermit;
    UINT8 linkQuality;
    DWORD timeStamp;
    bool securityUse;
    UINT8 aclEntry;
    bool securityFailure;
} PAN_DESCRIPTOR;

typedef struct {
    UINT8 bsn;
    PAN_DESCRIPTOR panDescriptor;
    BYTE pendAddrSpec;
    //ADDRESS pAddrList[7];
    uint8_t      shortpendingadd_length;
    uint8_t      longpendingadd_length;
    SHORT_ADDR                shortaddrpending_list[MAX_PENDING_LIST_SIZE];
    LONG_ADDR                 longaddrpending_list[MAX_PENDING_LIST_SIZE]; 
    UINT8 sduLength;
    BYTE pSdu[aMaxBeaconPayloadLength];
} MLME_BEACON_NOTIFY_INDICATION;

typedef struct {
    ADDRESS orphanAddress;
    bool securityUse;
    UINT8 aclEntry;
} MLME_ORPHAN_INDICATION;

typedef struct {
    WORD panId;
    UINT8 srcAddrMode;
    ADDRESS srcAddr;
    UINT8 dstAddrMode;
    ADDRESS dstAddr;
    MAC_ENUM status;
} MLME_COMM_STATUS_INDICATION;


typedef struct{
	LONG_ADDR  Deviceaddr;
	Node_Capability  cap_info;
	bool              security_use;
	uint8_t          aclentry;
}MLME_ASSOCIATE_INDICATION;

//做扫描时候容忍最多保存记录多少个PAN 描述符和 信道ED值，与实现相关的参数，可由自己确定
#define  MAC_MAX_PAN_DESCRIPTORS     5
#define  MAC_MAX_ED_MEASUREMENT      16

typedef struct {
    BYTE status;
    BYTE scanType;
    DWORD unscannedChannels;
    UINT8 resultListSize;
    union 
		{
    int8_t pEnergyDetectList[MAC_MAX_ED_MEASUREMENT];
    PAN_DESCRIPTOR pPANDescriptorList[MAC_MAX_PAN_DESCRIPTORS];
    };
} MLME_SCAN_CONFIRM;



//--------------------------------------------------//
//---------下面为Zigbee中MAC层原语的函数声明--------//
//--------------------------------------------------//

//原语函数声明
//对于某些比较简单的只返回状态成功与否的confirm原语，可以放在request原语的返回值里面返回。
//对于GTS，zigbee协议里面没有使用，故本版本的协议实现没有实现GTS相关的原语。
//----------------------------------------------------------------------------------------------------------
// MCPS and MLME prototypes (ordered alphabetically)
void mcpsDataRequest(NODE_INFO dst, NODE_INFO src,UINT8 msduLength, BYTE *pMsdu, BYTE msduHandle, BYTE txOptions);
void mcpsDataConfirm(MAC_ENUM status, BYTE msduHandle);

//参数作为一个全局的数据结构体来访问
void mcpsDataIndication(MCPS_DATA_INDICATION  mcps_data_indication_info);

//没有实现该原语
MAC_ENUM mcpsPurgeRequest(BYTE msduHandle);

void mlmeAssociateRequest(UINT8 logicalChannel, BYTE coordAddrMode, SHORT_ADDR coordPANId, ADDRESS pCoordAddress, Node_Capability capabilityInformation, bool securityEnable);

//void mlmeAssociateRequest(UINT8 logicalChannel, BYTE coordAddrMode, WORD coordPANId, ADDRESS *pCoordAddress, Node_Capability capabilityInformation, bool securityEnable);
void mlmeAssociateIndication(MLME_ASSOCIATE_INDICATION   mlme_associate_indication_info);
void mlmeAssociateResponse(LONG_ADDR deviceAddress, SHORT_ADDR assocShortAddress, BYTE status, bool securityEnable);
void mlmeAssociateConfirm(SHORT_ADDR AssocShortAddress, BYTE status);

//参数作为一个全局的数据结构体来访问
void mlmeBeaconNotifyIndication(MLME_BEACON_NOTIFY_INDICATION  beacon_payload_info);

void mlmeCommStatusIndication(WORD panId, BYTE srcAddrMode, ADDRESS *pSrcAddr, BYTE dstAddrMode, ADDRESS *pDstAddr, BYTE status);

void mlmeDisassociateRequest(LONG_ADDR pDeviceAddress, BYTE disassociateReason, bool securityEnable);
void mlmeDisassociateIndication(LONG_ADDR deviceAddress, BYTE disassociateReason, bool securityUse, BYTE aclEntry);
void mlmeDisassociateConfirm(MAC_ENUM status);

MAC_ENUM mlmeGetRequest(MAC_PIB_ATTR pibAttribute, void *pPibAttributeValue);

void mlmeOrphanIndication(LONG_ADDR orphanAddress, bool securityUse, BYTE aclEntry);
void mlmeOrphanResponse(LONG_ADDR orphanAddress, SHORT_ADDR shortAddress, bool associatedMember, bool securityEnable);

//本代码不实现这两个原语
void mlmePollRequest(BYTE coordAddrMode, WORD coordPANId, ADDRESS *coordAddress, bool securityEnable);
void mlmePollConfirm(MAC_ENUM status);

MAC_ENUM mlmeResetRequest(bool setDefaultPIB);


//本版本代码不实现下面两个原语
void mlmeRxEnableRequest(bool deferPermit, uint32_t  rxOnTime, uint32_t rxOnDuration);
void mlmeRxEnableConfirm(MAC_ENUM status);

MAC_ENUM mlmeScanRequest(BYTE scanType, DWORD scanChannels, UINT8 scanDuration);
//没有必要定义mlmescanconfirm函数,scan request得到的结果：
//状态由scan request返回，其他的结果由全局结构MLME_SCAN_CONFIRM返回;
void mlmeScanConfirm(MLME_SCAN_CONFIRM  scan_result_info);

MAC_ENUM mlmeSetRequest(MAC_PIB_ATTR pibAttribute, void *pPibAttributeValue);

//void mlmeStartRequest(SHORT_ADDR  panId, UINT8 logicalChannel, UINT8 beaconOrder, UINT8 superframeOrder, bool panCoordinator, bool batteryLifeExtension, bool coordRealignment, bool securityEnable, QWORD StartTime);
void mlmeStartRequest(SHORT_ADDR panId, uint8_t  logicalChannel, uint8_t beaconOrder, uint8_t superframeOrder, bool panCoordinator, bool batteryLifeExtension, bool coordRealignment, bool securityEnable , WORD StartTime);

void mlmeStartConfirm(void);

void mlmeSyncRequest(UINT8 logicalChannel, bool trackBeacon);

void mlmeSyncLossIndication(MAC_ENUM lossReason);




//--------------------------------------------------//
//----下面为Zigbee中MAC层内部需要的一些数据结构-----//
//--------------------------------------------------//


//射频发送buffer。已经打包好，准备向物理层发送的MAC帧，
//由于信标帧具有最高的发送优先级，故本队列里面不包括信标帧
//采用间接通信方式的帧由于在收到
typedef struct _TX_BUFFER
{
	//队列中该结构元素是否可用，如果发送完一个不需要ack的帧
	//或者发送一个需要ack的帧，并且已经收到ack帧，则发送完成后可以置IsValid为无效。
	bool IsNeedAcked;
	//区分各个帧的句柄
  BYTE FrameHandle;
  //是否发送过，表明该帧在等待ack中
  bool IsTxed;
  //该帧序列号，在处理应答帧的时候有用。
  BYTE FrameSeq; 
  //该帧已经发送过的次数，初始化为aMaxFrameRetries，每发送一次，减1。为0时候，不需要发送，直接向上层报告出错。
  BYTE FrameRetriesCounter;
  //发送的MAC帧长度
  BYTE frameLength;
  //命令帧类型字段
  BYTE  cmd_type;
  //待发送的帧负载
  BYTE * payload;
  struct _TX_BUFFER   *next;
} TX_BUFFER;

//初始化发送队列函数声明,将IsValid置为无效，payload指针置为空.
void initial_tx_queue(void);

//射频发送队列的操作函数声明,寻找发送buffer中的一个空位置，如果没有，则返回发送忙，把须发送的包放到队列中
TX_BUFFER * add_tx_queue(BYTE framelength, BYTE * payload, BYTE framehandle, bool is_ack, BYTE frameseq,BYTE cmd_type);

//射频发送队列的删除一个元素函数声明。对于需要应答的帧，在收到对应的ack后，即可从队列中删除，释放payload域,置isvalid为无效。
//如果为一个不需要应答的帧，则发送完成收到正确的confirm后即可直接删除掉
void remove_tx_queue(BYTE frameseq);

TX_BUFFER * get_frame_pointer(BYTE frameseq);

//下面的结构用于发送任务的参数传递作用
#define   MAX_TX_BUFFER_SIZE    5
typedef struct _parameter_tx_task
{
     TX_BUFFER * parameter_tx_task[MAX_TX_BUFFER_SIZE];
     uint8_t top;
     uint8_t tail;
}parameter_tx_task;


/*
//
//标志用于存储发送时候需要的信息，不用再到MAC帧里面去查找
typedef struct _MAC_FRAME_FLAG
{
	BYTE IsValid     :1;   //表示该帧是否处理完毕，那么该存储空间可以重新放入一个新的待发送的帧
	BYTE IsAcked     :1;   //
} MAC_FRAME_FLAG;
*/
//pending transaction队列，协调者用于存储采用间接通信的数据或命令帧
//注意:pending队列里面的帧，在发送时，还需要打包进入TX_BUFFERS队列，然后才能发送。在pending队列时候还没有分配帧序列号
typedef struct _INDIRECDT_TX_PENDING_QUEUE
{
	//该元素是否有效,在将该元素添加到发送队列中去以后，可以置该域为无效。同时置pending_payload指针为空
	bool IsValid;
	//可以表明该间接通信的帧已经经历过多少个超帧，如果计数到零，则需要通过原语向上层报告超时。
	//对于数据帧或协调者发给设备的disassociation notification帧，可以采用相应的confirm原语向高层报告
	//对于association response帧,采用comu_status原语向高层报告
	//该值的更新在每次发送信标帧时候，递减1，减到0时候，向上层报告，初始值设为macTransactionPersistenceTime
	uint16_t MacTranPersistCounter;
	//用于区分是哪个帧的句柄
	BYTE FrameHandle;
	//帧控制域,两字节
	FRAME_CONTROL framectl_field;
	bool   associate_or_disassociate;
	SHORT_ADDR shortaddralloc;
	union{
	 BYTE associate_status;
	 BYTE disassociation_reason;
		};
	union{
	 LONG_ADDR associate_device_addr;
	 LONG_ADDR end_device_addr;
		};
	//目标地址和源地址信息，包括PANID
	NODE_INFO  dst;
	NODE_INFO  src;
	//pending帧的长度                
	BYTE frameLength;
	//该帧的负载域
	BYTE * high_level_payload;

	struct _INDIRECDT_TX_PENDING_QUEUE  *next;
	//?????需要扩展？         
} INDIRECT_TX_PENDING_QUEUE;

//操作函数声明

//pending队列的初始化函数
void initial_pending_queue(void);

//添加一个数据包到pending队列中，如果队列满，需要采用对应的原语通知高层。
void add_pending_queue(BYTE framehandle, FRAME_CONTROL framectl, NODE_INFO dst, NODE_INFO src, BYTE framelength, BYTE *payload,BYTE associate_status_or_disassociate_reason,bool associate_or_disassociate,LONG_ADDR addr,SHORT_ADDR shortaddralloc);

//从pending队列中删除一个元素
void remove_pending_queue(INDIRECT_TX_PENDING_QUEUE  *current);
INDIRECT_TX_PENDING_QUEUE * get_pending_frame(NODE_INFO dst);

bool  Is_data_pending(NODE_INFO dst);





void transmit_with_csmaca(void);
uint32_t direct_transmit(uint8_t psdulength,uint8_t * psdu);






//MAC层在收到一个帧后如何处理该帧的函数(或任务)
void mac_frame_rcvd(void);




//定时器中断服务程序post的一个任务
void mac_timer_task(void);

uint16_t mac_timer3queue_getnextinterval(void);


//物理信道状态
//物理层将根据该状态进行接收的帧的预过滤，减轻高层负担


typedef enum  {

	PHY_IDLE=0,
       DOINGCSMA_CA,
       TRANSMITTING_FRAME,
       WAITING_FOR_ACK,
       WAITING_FOR_BEACON,
       INACTIVE_PERIOD
  
}PHY_CHANNEL_STATE ;
PHY_CHANNEL_STATE phy_channel_state;


#define    MAX_RX_BUFFER_SIZE    3
typedef  struct _RX_BUFFER
{
     BYTE    framelength;
     BYTE    * payload;
     BYTE     lqi;
     uint32_t  timestamp;
     struct  _RX_BUFFER  *next;
}RX_BUFFER;

RX_BUFFER * add_to_rx_buffer(BYTE framelength, BYTE *payload,BYTE lqi,uint32_t timestamp);
void  remove_from_rx_buffer(RX_BUFFER  * framepointer);


//该任务负责是从物理层取一个帧并进行解析
void  mac_get_received_frame(void);


//void beacon_received(void);



void data_request_frame_send(void);

void pending_data_send(NODE_INFO dst);

void  data_frame_send(BYTE * nwkpayload, BYTE nwkpayloadlength, NODE_INFO dst,NODE_INFO src,BYTE frameHandle);

void  associate_request_frame_send(SHORT_ADDR panid,NODE_INFO dst, Node_Capability node_capinfo);



void disassociate_notification_frame_send(BYTE disassociation_reason,LONG_ADDR end_device_addr);

void panid_confilct_frame_send(void);

void orphan_notification_frame_send(void);


void beacon_request_frame_send(void);

//#ifdef I_AM_COORDINATOR
void coordinator_realignment_frame_send(bool Isunicast,  NODE_INFO  dst, SHORT_ADDR allocated_addr);

void associate_response_frame_send(SHORT_ADDR coor_short_addr, BYTE associate_status,LONG_ADDR associate_device_addr,SHORT_ADDR shortaddralloc);

void  beacon_frame_send(bool short_or_long);


void mac_frame_send(void);
void pending_frame_handle(void);
//endif
void beacon_frame_tx_timing(void);
void mlmesyn_finished(void);
void  synrequest_task(void);

uint8_t  random_generate(void);
//uint8_t wait_for_backoff_boundary(void);
uint8_t  do_backoff(void);
uint8_t  csma_initial(void);
void  failure_to_tx_with_csma(void);
void CSMA_CA_DOING(void);
void scan_task(void);
#endif



