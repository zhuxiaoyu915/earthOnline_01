#ifndef _MAC_H_
#define _MAC_H_




#include "phy.h"
#include "generic.h"

//--------------------------------------------------//
//-----------֡�����ֶ����õ�һЩԤ���峣��---------//
//--------------------------------------------------//
#define aMaxPHYPacketSize 127
//֡�����ֶ�bit[2:0]
#define MAC_FRAME_BEACON                (0x00)
#define MAC_FRAME_DATA                  (0x01)
#define MAC_FRAME_ACK                   (0x02)
#define MAC_FRAME_CMD                   (0x03)
//��֡�Ĵ����Ƿ����MAC��İ�ȫ����bit[3]
#define MAC_SECURITY_YES            (0x08)
#define MAC_SECURITY_NO             (0x00)
//���ڼ��ͨ���У���ʾ��������û����,��Ҫ������data request����֡bit[4]
#define MAC_FRAME_PENDING_YES       (0x10)
#define MAC_FRAME_PENDING_NO        (0x00)
//�Ե�ǰ֡�Ĵ��ͣ��Ƿ���Ҫ�շ���Ӧ��bit[5]
#define MAC_ACK_YES                 (0x20)
#define MAC_ACK_NO                  (0x00)
//�Ƿ�Ϊͬһ��PAN�ڵĴ�����bit[6]
#define MAC_INTRA_PAN_YES           (0x40)
#define MAC_INTRA_PAN_NO            (0x00)
//Դ��ַģʽ�����ֶ�bit[11:10]
#define MAC_DST_NO_ADDR             (0x00)
#define MAC_DST_SHORT_ADDR          (0x08)
#define MAC_DST_LONG_ADDR           (0x0c)
#define MAC_DST_ADDR_RESERVED       (0x04)
//Ŀ���ַģʽ�����ֶ�bit[15:14]
#define MAC_SRC_NO_ADDR             (0x00)
#define MAC_SRC_SHORT_ADDR          (0x80)
#define MAC_SRC_LONG_ADDR           (0xc0)
#define MAC_SRC_ADDR_RESERVED       (0x40)
//��������֡������Ϣ�ֶ�
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

//ͨ����������ȷ�����豸�Ƿ�ΪЭ����
#ifdef I_AM_COORDINATOR
    #define IS_COORDINATOR      (0x01)      // 1 is yes, 0 if no.
    //#define IS_FFD                       (0x02)
#else
    #define IS_COORDINATOR      (0x00)
#endif
//ͨ����������ȷ���Ƿ����MAC��İ�ȫ����
#ifdef I_AM_SECURITY_CAPABLE
#define MAC_OPT_SECURITY            MAC_SECURITY_YES
#define MAC_OPT_ACL_SIZE         0x10
#else
#define MAC_OPT_SECURITY            MAC_SECURITY_NO
#define MAC_OPT_ACL_SIZE         0x00
#endif
//ͨ����������ȷ�����豸�Ƿ�ΪFFD�豸
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

//ͨ����������ȷ�����豸�Ĺ���ģʽ
#ifdef I_AM_MAINS_POWERED
   #define IS_MAINS_POWERED    (0x04)     
#else
#define IS_MAINS_POWERED    (0x00)
#endif 
   //ͨ����������ȷ�����豸��Ƶ����ʱ�򣬽����Ƿ��
   #ifdef MY_RX_IS_ALWAYS_ON_WHEN_IDLE
   #define IS_RX_ALWAYS_ON     (0x08)
   #else
   #define IS_RX_ALWAYS_ON     (0x00)
   #endif
   //ͨ����������ȷ�����豸��MAC��ȫ�Ƿ�ʹ��
   #ifdef I_AM_SECURITY_CAPABLE
   #define IS_SECURITY_ENABLED (0x40)
#else
   #define IS_SECURITY_ENABLED (0x00)
#endif
   //ͨ����������ȷ�����豸�Ƿ�ϣ��Э���豸��������̵�ַ(pg125)
   #ifdef MAC_USE_SHORT_ADDR
   #define IS_SHORT_ADDRESS    (0x80)
#else
   #define IS_SHORT_ADDRESS    (0x00)
#endif
   //��������ʱ��ʹ�õı��豸��������Ϣ
#define MAC_CAP_INFO      (IS_COORDINATOR        |     \  
                             IS_FFD                 |       \
                             IS_MAINS_POWERED       |       \
                             IS_RX_ALWAYS_ON        |       \
                             IS_SECURITY_ENABLED    |       \
IS_SHORT_ADDRESS);
//MAC���ö�ٳ�������ӳMAC���ԭ���״ֵ̬.(pg110)
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

// ��ӳ������״ֵ̬(pg126)
#define PAN_AT_CAPACITY         0x01
#define PAN_ACCESS_DENIED       0x02

//��ӳȡ��������ԭ��״ֵ̬  (pg127)
#define COORD_WISH_TO_LEAVE     0x01
#define DIVICE_TO_LEAVE         0x02

//��ӳMAC��������һЩ����
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
//�������漰MAC��ĳ�������
#define MAX_PENDING_LIST_SIZE   2



//��ӳ��ȫ���Ե�һЩ����������MAC��ĺ�PHY���
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
//---����ΪZigbee��MAC���õ���ר�õ��������Ͷ���----//
//--------------------------------------------------//

//MAC�㰲ȫ������ص����Ͷ���
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
  
   //8�ֽڳ���ַ���Ͷ���

//PAN��ַ���Ͷ���
typedef SHORT_ADDR  PAN_ADDR;
//MAC���PIB�洢�ṹ����
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

//��MAC PIB�е�����һ��ID��ţ��Ӷ����ڷ���
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




// MAC ����֡��Ӧ״̬����
//?????????????????????????????????????????/��������ϵͳMAC���״̬������Ҫ��չ��
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
//-------����ΪZigbee��MAC��֡�����ݽṹ����--------//
//--------------------------------------------------//


//IEEE802.15.4֡�Ŀ����ֶθ�ʽ
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

//IEEE802.15.4�ű�֡�еĸ�����(payload)��ʽ(zigbee�淶pg250��251)
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

//IEEE802.15.4�ű�֡�е�pending address�淶��
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

   // IEEE 802.15.4 �ű�֡�еĳ�֡�淶���ʽ
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
//IEEE802.15.4���ű�֡��GTS������
BYTE_VAL  GTSDirectionMask ;
//IEEE802.15.4��GTS descriptor����
typedef struct _GTS_DESCRIPTOR
{
    SHORT_ADDR  DeviceAddr;
    BYTE          GTSStartingSlot  : 4;
    BYTE          GTSLength      :4;
}GTS_DESCRIPTOR;

//����association request����֡ʱ��Ľڵ�capability��Ϣ
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

//GTS����֡��characteristic��
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


// A 802.15.4 �ڵ��ַ��Ϣ
typedef struct _NODE_INFO
{
    BYTE            addrMode;
    LONG_ADDR       longAddr;
    SHORT_ADDR      shortAddr;
    PAN_ADDR        panID;
} NODE_INFO;

//MAC��Э��ĵ�ǰ֡�洢����Ϣ���Ͷ��壬��Щ��ϢҪ������ת����PSDU�ͼ���PSDU���Ȼ򷴹�����psduת����msdu��
//�Ӷ�����pd_data_request��������. �ýṹ��Ҫ����MAC���MAC֡�Ĵ������.
//MAC_FRAME_FORMAT��һ��ͨ�õĶ���,���ھ����֡,��Ҫ����֡�����ֶκͳ��ȵ��ֶ�ȷ��ʹ�ò����ֶ�.
typedef struct _MAC_FRAME_FORMAT
{
    NODE_INFO dst;    //����Ŀ���豸��ַ��Ŀ���������Ϣ.
    NODE_INFO src;    //Դ�豸��ַ���������Ϣ.
    BYTE type;        //֡�����ֶ�,�������ڿ��ٷ���.
    BYTE cmd;         //���������֡�Ļ�,����洢����֡��������Ϣ
    union{
    BYTE macDSN;      //֡���к�
    BYTE macBSN;
    	};
    BYTE frameLength; //MAC���֡����,������FCS���ܳ���
    //BYTE txOptions;   //���͵Ĳ���
    BYTE framehandle;
    BYTE lqi;
    uint32_t  timestamp;
    //֡�����ֶ�,�������ֽ�
    FRAME_CONTROL    frame_control_field;
    
    //���ڵ�ַ���PANID�������ֽ�Ϊ��λ����,�������γ�psdu�Ĺ�������д.
    //
    //
    
    //����㴫�ݹ���������payload���ű�payloadָ��
    uint8_t  nwk_data_length;  //����msdulength
    uint8_t  beacon_payload_length;
    union
    {
      BYTE   nwk_data_payload[aMaxMACFrameSize];
      BYTE   beacon_payload[aMaxBeaconPayloadLength]; 
    }; 
    //capinfo��Ҫ����association request֡��
    Node_Capability  node_capinfo_field;
    
    //������ű�֡���õ��ĳ�֡�淶��͵�ַpending��
    SUPERFRAME_SPECIFICATION  superframe_field;
    PendingAddress            pendaddress_field;

    uint8_t      shortpendingadd_length;
    uint8_t      longpendingadd_length;
    SHORT_ADDR                shortaddrpending_list[MAX_PENDING_LIST_SIZE];
    LONG_ADDR                 longaddrpending_list[MAX_PENDING_LIST_SIZE]; 
    
    //����ǹ�����Ӧ֡���õ��ķ���Ķ̵�ַ��Ϣ�͹���״ֵ̬
    SHORT_ADDR                shortaddralloc;
    BYTE                      association_status;
    
    //����ǳ�������֪ͨ֡�п����õ���ȡ��������ԭ��
    BYTE                      disassociation_reason;
    
    //�����Э���߷�����realignment֡���õ���һЩ��
    SHORT_ADDR                PAN_ID_field;
    SHORT_ADDR                coordshortaddr_field;
    BYTE                      logicalchannel_field;
    SHORT_ADDR                shortaddrallocted;
    
    //�����GTS����֡���õ���GTS characteristic��
    GTS_CHARACTERISTIC        gts_char_field;
    
    //������Ҫ��¼һЩ�뵱ǰ֡��صı�־��
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

//ֻ���ڷ���ʱ��
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
//-------����ΪZigbee��MAC��ԭ����������Ͷ���------//
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

//��ɨ��ʱ��������ౣ���¼���ٸ�PAN �������� �ŵ�EDֵ����ʵ����صĲ����������Լ�ȷ��
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
//---------����ΪZigbee��MAC��ԭ��ĺ�������--------//
//--------------------------------------------------//

//ԭ�ﺯ������
//����ĳЩ�Ƚϼ򵥵�ֻ����״̬�ɹ�����confirmԭ����Է���requestԭ��ķ���ֵ���淵�ء�
//����GTS��zigbeeЭ������û��ʹ�ã��ʱ��汾��Э��ʵ��û��ʵ��GTS��ص�ԭ�
//----------------------------------------------------------------------------------------------------------
// MCPS and MLME prototypes (ordered alphabetically)
void mcpsDataRequest(NODE_INFO dst, NODE_INFO src,UINT8 msduLength, BYTE *pMsdu, BYTE msduHandle, BYTE txOptions);
void mcpsDataConfirm(MAC_ENUM status, BYTE msduHandle);

//������Ϊһ��ȫ�ֵ����ݽṹ��������
void mcpsDataIndication(MCPS_DATA_INDICATION  mcps_data_indication_info);

//û��ʵ�ָ�ԭ��
MAC_ENUM mcpsPurgeRequest(BYTE msduHandle);

void mlmeAssociateRequest(UINT8 logicalChannel, BYTE coordAddrMode, SHORT_ADDR coordPANId, ADDRESS pCoordAddress, Node_Capability capabilityInformation, bool securityEnable);

//void mlmeAssociateRequest(UINT8 logicalChannel, BYTE coordAddrMode, WORD coordPANId, ADDRESS *pCoordAddress, Node_Capability capabilityInformation, bool securityEnable);
void mlmeAssociateIndication(MLME_ASSOCIATE_INDICATION   mlme_associate_indication_info);
void mlmeAssociateResponse(LONG_ADDR deviceAddress, SHORT_ADDR assocShortAddress, BYTE status, bool securityEnable);
void mlmeAssociateConfirm(SHORT_ADDR AssocShortAddress, BYTE status);

//������Ϊһ��ȫ�ֵ����ݽṹ��������
void mlmeBeaconNotifyIndication(MLME_BEACON_NOTIFY_INDICATION  beacon_payload_info);

void mlmeCommStatusIndication(WORD panId, BYTE srcAddrMode, ADDRESS *pSrcAddr, BYTE dstAddrMode, ADDRESS *pDstAddr, BYTE status);

void mlmeDisassociateRequest(LONG_ADDR pDeviceAddress, BYTE disassociateReason, bool securityEnable);
void mlmeDisassociateIndication(LONG_ADDR deviceAddress, BYTE disassociateReason, bool securityUse, BYTE aclEntry);
void mlmeDisassociateConfirm(MAC_ENUM status);

MAC_ENUM mlmeGetRequest(MAC_PIB_ATTR pibAttribute, void *pPibAttributeValue);

void mlmeOrphanIndication(LONG_ADDR orphanAddress, bool securityUse, BYTE aclEntry);
void mlmeOrphanResponse(LONG_ADDR orphanAddress, SHORT_ADDR shortAddress, bool associatedMember, bool securityEnable);

//�����벻ʵ��������ԭ��
void mlmePollRequest(BYTE coordAddrMode, WORD coordPANId, ADDRESS *coordAddress, bool securityEnable);
void mlmePollConfirm(MAC_ENUM status);

MAC_ENUM mlmeResetRequest(bool setDefaultPIB);


//���汾���벻ʵ����������ԭ��
void mlmeRxEnableRequest(bool deferPermit, uint32_t  rxOnTime, uint32_t rxOnDuration);
void mlmeRxEnableConfirm(MAC_ENUM status);

MAC_ENUM mlmeScanRequest(BYTE scanType, DWORD scanChannels, UINT8 scanDuration);
//û�б�Ҫ����mlmescanconfirm����,scan request�õ��Ľ����
//״̬��scan request���أ������Ľ����ȫ�ֽṹMLME_SCAN_CONFIRM����;
void mlmeScanConfirm(MLME_SCAN_CONFIRM  scan_result_info);

MAC_ENUM mlmeSetRequest(MAC_PIB_ATTR pibAttribute, void *pPibAttributeValue);

//void mlmeStartRequest(SHORT_ADDR  panId, UINT8 logicalChannel, UINT8 beaconOrder, UINT8 superframeOrder, bool panCoordinator, bool batteryLifeExtension, bool coordRealignment, bool securityEnable, QWORD StartTime);
void mlmeStartRequest(SHORT_ADDR panId, uint8_t  logicalChannel, uint8_t beaconOrder, uint8_t superframeOrder, bool panCoordinator, bool batteryLifeExtension, bool coordRealignment, bool securityEnable , WORD StartTime);

void mlmeStartConfirm(void);

void mlmeSyncRequest(UINT8 logicalChannel, bool trackBeacon);

void mlmeSyncLossIndication(MAC_ENUM lossReason);




//--------------------------------------------------//
//----����ΪZigbee��MAC���ڲ���Ҫ��һЩ���ݽṹ-----//
//--------------------------------------------------//


//��Ƶ����buffer���Ѿ�����ã�׼��������㷢�͵�MAC֡��
//�����ű�֡������ߵķ������ȼ����ʱ��������治�����ű�֡
//���ü��ͨ�ŷ�ʽ��֡�������յ�
typedef struct _TX_BUFFER
{
	//�����иýṹԪ���Ƿ���ã����������һ������Ҫack��֡
	//���߷���һ����Ҫack��֡�������Ѿ��յ�ack֡��������ɺ������IsValidΪ��Ч��
	bool IsNeedAcked;
	//���ָ���֡�ľ��
  BYTE FrameHandle;
  //�Ƿ��͹���������֡�ڵȴ�ack��
  bool IsTxed;
  //��֡���кţ��ڴ���Ӧ��֡��ʱ�����á�
  BYTE FrameSeq; 
  //��֡�Ѿ����͹��Ĵ�������ʼ��ΪaMaxFrameRetries��ÿ����һ�Σ���1��Ϊ0ʱ�򣬲���Ҫ���ͣ�ֱ�����ϲ㱨�����
  BYTE FrameRetriesCounter;
  //���͵�MAC֡����
  BYTE frameLength;
  //����֡�����ֶ�
  BYTE  cmd_type;
  //�����͵�֡����
  BYTE * payload;
  struct _TX_BUFFER   *next;
} TX_BUFFER;

//��ʼ�����Ͷ��к�������,��IsValid��Ϊ��Ч��payloadָ����Ϊ��.
void initial_tx_queue(void);

//��Ƶ���Ͷ��еĲ�����������,Ѱ�ҷ���buffer�е�һ����λ�ã����û�У��򷵻ط���æ�����뷢�͵İ��ŵ�������
TX_BUFFER * add_tx_queue(BYTE framelength, BYTE * payload, BYTE framehandle, bool is_ack, BYTE frameseq,BYTE cmd_type);

//��Ƶ���Ͷ��е�ɾ��һ��Ԫ�غ���������������ҪӦ���֡�����յ���Ӧ��ack�󣬼��ɴӶ�����ɾ�����ͷ�payload��,��isvalidΪ��Ч��
//���Ϊһ������ҪӦ���֡����������յ���ȷ��confirm�󼴿�ֱ��ɾ����
void remove_tx_queue(BYTE frameseq);

TX_BUFFER * get_frame_pointer(BYTE frameseq);

//����Ľṹ���ڷ�������Ĳ�����������
#define   MAX_TX_BUFFER_SIZE    5
typedef struct _parameter_tx_task
{
     TX_BUFFER * parameter_tx_task[MAX_TX_BUFFER_SIZE];
     uint8_t top;
     uint8_t tail;
}parameter_tx_task;


/*
//
//��־���ڴ洢����ʱ����Ҫ����Ϣ�������ٵ�MAC֡����ȥ����
typedef struct _MAC_FRAME_FLAG
{
	BYTE IsValid     :1;   //��ʾ��֡�Ƿ�����ϣ���ô�ô洢�ռ�������·���һ���µĴ����͵�֡
	BYTE IsAcked     :1;   //
} MAC_FRAME_FLAG;
*/
//pending transaction���У�Э�������ڴ洢���ü��ͨ�ŵ����ݻ�����֡
//ע��:pending���������֡���ڷ���ʱ������Ҫ�������TX_BUFFERS���У�Ȼ����ܷ��͡���pending����ʱ��û�з���֡���к�
typedef struct _INDIRECDT_TX_PENDING_QUEUE
{
	//��Ԫ���Ƿ���Ч,�ڽ���Ԫ����ӵ����Ͷ�����ȥ�Ժ󣬿����ø���Ϊ��Ч��ͬʱ��pending_payloadָ��Ϊ��
	bool IsValid;
	//���Ա����ü��ͨ�ŵ�֡�Ѿ����������ٸ���֡������������㣬����Ҫͨ��ԭ�����ϲ㱨�泬ʱ��
	//��������֡��Э���߷����豸��disassociation notification֡�����Բ�����Ӧ��confirmԭ����߲㱨��
	//����association response֡,����comu_statusԭ����߲㱨��
	//��ֵ�ĸ�����ÿ�η����ű�֡ʱ�򣬵ݼ�1������0ʱ�����ϲ㱨�棬��ʼֵ��ΪmacTransactionPersistenceTime
	uint16_t MacTranPersistCounter;
	//�����������ĸ�֡�ľ��
	BYTE FrameHandle;
	//֡������,���ֽ�
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
	//Ŀ���ַ��Դ��ַ��Ϣ������PANID
	NODE_INFO  dst;
	NODE_INFO  src;
	//pending֡�ĳ���                
	BYTE frameLength;
	//��֡�ĸ�����
	BYTE * high_level_payload;

	struct _INDIRECDT_TX_PENDING_QUEUE  *next;
	//?????��Ҫ��չ��         
} INDIRECT_TX_PENDING_QUEUE;

//������������

//pending���еĳ�ʼ������
void initial_pending_queue(void);

//���һ�����ݰ���pending�����У��������������Ҫ���ö�Ӧ��ԭ��֪ͨ�߲㡣
void add_pending_queue(BYTE framehandle, FRAME_CONTROL framectl, NODE_INFO dst, NODE_INFO src, BYTE framelength, BYTE *payload,BYTE associate_status_or_disassociate_reason,bool associate_or_disassociate,LONG_ADDR addr,SHORT_ADDR shortaddralloc);

//��pending������ɾ��һ��Ԫ��
void remove_pending_queue(INDIRECT_TX_PENDING_QUEUE  *current);
INDIRECT_TX_PENDING_QUEUE * get_pending_frame(NODE_INFO dst);

bool  Is_data_pending(NODE_INFO dst);





void transmit_with_csmaca(void);
uint32_t direct_transmit(uint8_t psdulength,uint8_t * psdu);






//MAC�����յ�һ��֡����δ����֡�ĺ���(������)
void mac_frame_rcvd(void);




//��ʱ���жϷ������post��һ������
void mac_timer_task(void);

uint16_t mac_timer3queue_getnextinterval(void);


//�����ŵ�״̬
//����㽫���ݸ�״̬���н��յ�֡��Ԥ���ˣ�����߲㸺��


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


//���������Ǵ������ȡһ��֡�����н���
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



