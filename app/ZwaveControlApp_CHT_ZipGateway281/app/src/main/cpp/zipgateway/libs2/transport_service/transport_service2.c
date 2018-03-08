/* © 2014 Sigma Designs, Inc. This is an unpublished work protected by Sigma
 * Designs, Inc. as a trade secret, and is not to be used or disclosed except as
 * provided Z-Wave Controller Development Kit Limited License Agreement. All
 * rights reserved.
 *
 * Notice: All information contained herein is confidential and/or proprietary to
 * Sigma Designs and may be covered by U.S. and Foreign Patents, patents in
 * process, and are protected by trade secret or copyright law. Dissemination or
 * reproduction of the source code contained herein is expressly forbidden to
 * anyone except Licensees of Sigma Designs  who have executed a Sigma Designs’
 * Z-WAVE CONTROLLER DEVELOPMENT KIT LIMITED LICENSE AGREEMENT. The copyright
 * notice above is not evidence of any actual or intended publication of the
 * source code. THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED
 * INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS  TO REPRODUCE, DISCLOSE OR
 * DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL A PRODUCT THAT IT  MAY
 * DESCRIBE.
 *
 * THE SIGMA PROGRAM AND ANY RELATED DOCUMENTATION OR TOOLS IS PROVIDED TO COMPANY
 * "AS IS" AND "WITH ALL FAULTS", WITHOUT WARRANTY OF ANY KIND FROM SIGMA. COMPANY
 * ASSUMES ALL RISKS THAT LICENSED MATERIALS ARE SUITABLE OR ACCURATE FOR
 * COMPANY’S NEEDS AND COMPANY’S USE OF THE SIGMA PROGRAM IS AT COMPANY’S
 * OWN DISCRETION AND RISK. SIGMA DOES NOT GUARANTEE THAT THE USE OF THE SIGMA
 * PROGRAM IN A THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT WILL
 * BE: (A) PERFORMED ERROR-FREE OR UNINTERRUPTED; (B) THAT SIGMA WILL CORRECT ANY
 * THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT ERRORS; (C) THE
 * THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT WILL OPERATE IN
 * COMBINATION WITH COMPANY’S CONTENT OR COMPANY APPLICATIONS THAT UTILIZE THE
 * SIGMA PROGRAM; (D) OR WITH ANY OTHER HARDWARE, SOFTWARE, SYSTEMS, SERVICES OR
 * DATA NOT PROVIDED BY SIGMA. COMPANY ACKNOWLEDGES THAT SIGMA DOES NOT CONTROL
 * THE TRANSFER OF DATA OVER COMMUNICATIONS FACILITIES, INCLUDING THE INTERNET,
 * AND THAT THE SERVICES MAY BE SUBJECT TO LIMITATIONS, DELAYS, AND OTHER PROBLEMS
 * INHERENT IN THE USE OF SUCH COMMUNICATIONS FACILITIES. SIGMA IS NOT RESPONSIBLE
 * FOR ANY DELAYS, DELIVERY FAILURES, OR OTHER DAMAGE RESULTING FROM SUCH ISSUES.
 * SIGMA IS NOT RESPONSIBLE FOR ANY ISSUES RELATED TO THE PERFORMANCE, OPERATION
 * OR SECURITY OF THE THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES
 * ENVIRONMENT THAT ARISE FROM COMPANY CONTENT, COMPANY APPLICATIONS OR THIRD
 * PARTY CONTENT. SIGMA DOES NOT MAKE ANY REPRESENTATION OR WARRANTY REGARDING THE
 * RELIABILITY, ACCURACY, COMPLETENESS, CORRECTNESS, OR USEFULNESS OF THIRD PARTY
 * CONTENT OR SERVICE OR THE SIGMA PROGRAM, AND DISCLAIMS ALL LIABILITIES ARISING
 * FROM OR RELATED TO THE SIGMA PROGRAM OR THIRD PARTY CONTENT OR SERVICES. TO THE
 * EXTENT NOT PROHIBITED BY LAW, THESE WARRANTIES ARE EXCLUSIVE. SIGMA OFFERS NO
 * WARRANTY OF NON-INFRINGEMENT, TITLE, OR QUIET ENJOYMENT. NEITHER SIGMA NOR ITS
 * SUPPLIERS OR LICENSORS SHALL BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES OR LOSS (INCLUDING DAMAGES FOR LOSS OF BUSINESS, LOSS OF
 * PROFITS, OR THE LIKE), ARISING OUT OF THIS AGREEMENT WHETHER BASED ON BREACH OF
 * CONTRACT, INTELLECTUAL PROPERTY INFRINGEMENT, TORT (INCLUDING NEGLIGENCE),
 * STRICT LIABILITY, PRODUCT LIABILITY OR OTHERWISE, EVEN IF SIGMA OR ITS
 * REPRESENTATIVES HAVE BEEN ADVISED OF OR OTHERWISE SHOULD KNOW ABOUT THE
 * POSSIBILITY OF SUCH DAMAGES. THERE ARE NO OTHER EXPRESS OR IMPLIED WARRANTIES
 * OR CONDITIONS INCLUDING FOR SOFTWARE, HARDWARE, SYSTEMS, NETWORKS OR
 * ENVIRONMENTS OR FOR MERCHANTABILITY, NONINFRINGEMENT, SATISFACTORY QUALITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * The Sigma Program  is not fault-tolerant and is not designed, manufactured or
 * intended for use or resale as on-line control equipment in hazardous
 * environments requiring fail-safe performance, such as in the operation of
 * nuclear facilities, aircraft navigation or communication systems, air traffic
 * control, direct life support machines, or weapons systems, in which the failure
 * of the Sigma Program, or Company Applications created using the Sigma Program,
 * could lead directly to death, personal injury, or severe physical or
 * environmental damage ("High Risk Activities").  Sigma and its suppliers
 * specifically disclaim any express or implied warranty of fitness for High Risk
 * Activities.Without limiting Sigma’s obligation of confidentiality as further
 * described in the Z-Wave Controller Development Kit Limited License Agreement,
 * Sigma has no obligation to establish and maintain a data privacy and
 * information security program with regard to Company’s use of any Third Party
 * Service Environment or Cloud Service Environment. For the avoidance of doubt,
 * Sigma shall not be responsible for physical, technical, security,
 * administrative, and/or organizational safeguards that are designed to ensure
 * the security and confidentiality of the Company Content or Company Application
 * in any Third Party Service Environment or Cloud Service Environment that
 * Company chooses to utilize.

 * Author: mdumbare */

//#include <ZW_classcmd.h>

#include "transport_service2.h"
#include "transport2_fsm.h"
#ifdef ZIPGW
#include <ZW_transport_api.h>
#include <ZIP_Router.h>
#include "CC_FirmwareUpdate.h"
#endif
#include <S2_external.h>
//#include <sys/clock.h>
//#include <unistd.h>
//#include <stdlib.h>

static uint8_t flag_tie_broken = 0;
void fc_timer_expired(void *);
#ifdef __C51__
void ZCB_send_subseq_frag(void *);
#else
static void send_subseq_frag(void *);
#endif
static void find_missing();
struct rx_timer_expired_data {
    uint8_t state; /* Rx timer expired after sending SEG_REQ or after sending */
};

static void rx_timer_expired(void *);
#if defined(NEW_TEST_T2)
typedef uint16_t clock_time_t;

static void ZCB_ts_senddata_cb(unsigned char status_send, void* user);
struct ctimer {
    struct ctimer *next;
    clock_time_t tickCounts;
    clock_time_t startValue;
    VOID_CALLBACKFUNC(f)(void *);
    void *ptr;
};

void fire_timer_btwn_2_frags(int test_type)
{
//    send_subseq_frag(NULL);
    ZCB_ts_senddata_cb(0, NULL);
}
void fire_fc_timer()
{
    fc_timer_expired(NULL);
}
void fire_rx_timer(){
    struct rx_timer_expired_data rdata;
    rdata.state = 0;
    rx_timer_expired(&rdata);
}
void ctimer_set(struct ctimer *c, clock_time_t t,
                void (*f)(void *), void *ptr)
{
}

int fc_timer_counter = 0;
void ctimer_stop(struct ctimer *c)
{
    fc_timer_counter++;
}

uint8_t MyNodeID = 0;

extern unsigned char output[];
uint8_t TS_SEND_RAW(uint8_t snode, uint8_t dnode, uint8_t *cmd, uint8_t len, uint8_t flags, VOID_CALLBACKFUNC(completedFunc)(uint8_t, void*))
{
    memcpy(output, cmd, len);

#if 0
    int i;
    for(i = 0; i < len; i++)
        printf("%x ", cmd[i]);

    printf("\n");
#endif
/*
    if (completedFunc)
        completedFunc(S2_TRANSMIT_COMPLETE_OK, NULL);
*/
    return 1;
}

int check_flag_tie_broken()
{
    return flag_tie_broken;
}

#else
#ifdef ZIPGW
typedef enum SECURITY_KEY
{
  SECURITY_KEY_NONE = 0x00,
  SECURITY_KEY_S2_UNAUTHENTICATED = 0x01,
  SECURITY_KEY_S2_AUTHENTICATED = 0x02,
  SECURITY_KEY_S2_ACCESS = 0x03,
  SECURITY_KEY_S0 = 0x08
} security_key_t;

typedef void (*ZW_CommandHandler_Callback_t)(ts_param_t* p, ZW_APPLICATION_TX_BUFFER *pCmd, WORD cmdLength);
#define ZIPCommandHandler(srcNode, count) \
    if(TSApplicationCommandHandler) {\
      ts_param_t p;     \
      p.dendpoint = 0; \
      p.sendpoint = 0; \
      p.snode = srcNode; \
      p.dnode = rcb.cmn.p.dnode; \
      p.rx_flags =0; \
      p.tx_flags = TRANSMIT_OPTION_ACK | TRANSMIT_OPTION_AUTO_ROUTE | TRANSMIT_OPTION_EXPLORE;\
      p.scheme = NO_SCHEME; \
      TSApplicationCommandHandler(&p,(ZW_APPLICATION_TX_BUFFER*) rcb.datagramData, count); \
    }

#define TS_SEND_RAW(src, dst, buf, buflen, txopt, cb) ZW_SendData_Bridge(src, dst, buf, buflen, txopt, cb)
#include <ctimer.h>
#else
#include "ZW_ctimer.h"
#endif
#ifndef __C51__
extern uint8_t ZW_SendData_Bridge(uint8_t, uint8_t, uint8_t *, uint8_t, uint8_t, VOID_CALLBACKFUNC(completedFunc)(uint8_t, void*));
#endif /* ifndef __C51__ */
#endif

static uint8_t flag_initialize_once = 1; // to initialize the receive session stuff once

extern const char *T2_EVENTS_STRING[];
extern const char *T2_STATES_STRING[];

#if !defined(__C51__) && !defined(ZIPGW)
#define CRC_POLY        0x1021
WORD
ZW_CheckCrc16(WORD crc, BYTE *pDataAddr, WORD bDataLen)
{
  BYTE WorkData;
  BYTE bitMask;
  BYTE NewBit;
  //printf("ZW_CheckCrc16: bDataLen = %u\r\n", bDataLen);
  while (bDataLen--)
  {
    WorkData = *pDataAddr++;
    for (bitMask = 0x80; bitMask != 0; bitMask >>= 1)
    {
      /* Align test bit with next bit of the message byte, starting with msb. */
      NewBit = ((WorkData & bitMask) != 0) ^ ((crc & 0x8000) != 0);
      crc <<= 1;
      if (NewBit)
      {
        crc ^= CRC_POLY;
      }
    } /* for (bitMask = 0x80; bitMask != 0; bitMask >>= 1) */
  }
  return crc;
}
#endif
/*
 * Max frame size is 64
 * Protocol need 10
 * Transport service header needs 7
 *
 * #define FRAGMENTMAXPAYLOAD  (64 - 10 - 7);
 */
#define FRAGMENTMAXPAYLOAD 47
static uint8_t FragmentMaxPayload = 0;

/* ZW_COMMAND_SUBSEQUENT_FRAGMENT_1BYTE_FRAME has the max size in transport service header
 * subtracting 1 for payload field inside ZW_COMMAND_SUBSEQUENT_FRAGMENT_1BYTE_FRAME */
uint8_t t2_txBuf[FRAGMENTMAXPAYLOAD + sizeof(ZW_COMMAND_SUBSEQUENT_FRAGMENT_1BYTE_FRAME) - 1];

ZW_COMMAND_FIRST_FRAGMENT_1BYTE_FRAME *first_frag = (ZW_COMMAND_FIRST_FRAGMENT_1BYTE_FRAME *)t2_txBuf;
ZW_COMMAND_SUBSEQUENT_FRAGMENT_1BYTE_FRAME *subseq_frag = (ZW_COMMAND_SUBSEQUENT_FRAGMENT_1BYTE_FRAME *)t2_txBuf;


static void send_last_frag(void);
static void reply_frag_req(void *);
static void send_first_frag();
static uint8_t send_frag_wait_cmd();
static uint8_t discard_all_received_fragments(void);

/* Structure desribing the inflight fragment. */
typedef struct cb {
    ts_param_t p;
#ifdef ZIPGW
    void (*completedFunc)(uint8_t txStatus, TX_STATUS_TYPE *t);
#else
    ZW_TransportService_SendData_Callback_t completedFunc;
#endif
#if defined(ZIPGW) || defined(__C51__)
    TX_STATUS_TYPE tx_status;
#endif
    uint8_t session_id;
    uint8_t pending_segments;
}control_block_t;

struct sending_cntrl_blk {
    uint16_t datalen_to_send; // this is set to FragmentMaxPayload or remaining data less than FragmentMaxPayload
    uint16_t missing_offset; // this is set to missing offset received in FRAMENT_REQUEST command and used to resend that fragment
    uint16_t offset; // this is used in sending side
    uint16_t remaining_data_len; // this records the len of remaining data to be sent
    control_block_t cmn; /* Common fields, necessary for both sending and receiving */
    const uint8_t *datagram;
    uint16_t datagram_len;
    uint8_t sending;
    uint8_t flag_replied_frag_req;
    uint8_t transmission_aborted; /*Initializing to out of range session id */

#define RESET_TIME 5000 /* ms */
    struct ctimer reset_timer;

    /* When fragment wait is received with pending segments this timer is used to halt the restart of datagram sending
    by 100ms * no_of_pending_segments */
    struct ctimer wait_restart_timer;

    /*Timer used to keep delay between two fragments sent typically 15ms */
    struct ctimer timer_btwn_2_frags;

    /* Array to mark the receival of Fragment Completion command for each sending session */
    /* As there could be max 16 sessions (4 bits for the session id) */
    uint8_t frag_compl_list[15];

    /* If the session is sending one then this flag is set to TRUE to mark that next send is going to be
    fragment wait. If the session is receiving send_frag_wait_cmd() is called directly */
    /*FIXME this and sending flags can be merged into one */
    uint8_t flag_send_frag_wait;
    uint8_t flag_reply_frag_req;
    ts_param_t frag_wait_p;

    uint16_t round_trip_first_frag;

    /* This flag is used to mark that there have been mutliple fc_timer expiration
     * for this fragment complete
     * So that transport service can report error to the application layer */
    uint8_t flag_fc_timer_expired_once;

    /* this stores the current destination node where the send session is going on. If a fragment
    request from different node is received this helps identifying it */
    uint8_t current_dnode;

    /* Used to determine if the send_last_frag() function is called from send_first_frag()
     * or ZCB_send_subseq_frag()/reply_frag_req */
    uint8_t does_not_fit_in_first_frag;

} scb;

#if !defined(ZIPGW) && !defined(NEW_TEST_T2)
/* Used for ZW_SendDataEx calls throughout this module */
TRANSMIT_OPTIONS_TYPE ts_txo;
#endif



struct receiving_cntrl_blk {
    control_block_t cmn; /* Common fields, necessary for both sending and receiving */
    uint8_t *fragment;
    uint8_t fragment_len;
    uint8_t flag_retry_frag_req_once;
    /* Fragment Completion timer */
    struct ctimer fc_timer;
    /*RX timer */
    struct ctimer rx_timer;

    struct rx_timer_expired_data rx_data;

    /* Array to mark the receival of whole Fragment (sending of fragment complete) command for each receiving session */
    /* As there could be max 16 sessions (4 bits for the session id) */
    uint8_t recv_frag_compl_list[16];

    /* Buffer for incoming re-assembled datagrams */
    //#define datagramData       ((BYTE*) uip_buf)
    uint8_t datagramData[DATAGRAM_SIZE_MAX];

    /* To copy the data to destination buffer */
    uint8_t cur_recvd_data_size;

    /* this is calculated on first fragment and is used for marking the fragments
      received Should be equal to FragmentMaxPayload/FRAGMENTMAXPAYLOAD. */
    uint8_t max_data_in_frag;

    /* this is set to the source node id on receiving FIRST_FRAG, and used in checking
    all subseq fragments and (first fragments if any before the transmissin of current
    datagram is done). This variable is set back to 0 on sending FRAG_COMPL or when the
    datagram is discarded */
    /*Used on receiving side */
    uint8_t current_snode;

    /* used to mark the fragments received in a datagram. Then used to find missing
     * fragments if any */
    uint8_t list_recvd_frag[((DATAGRAM_SIZE_MAX / META_DATA_MAX_DATA_SIZE)+1) * 2];
    uint8_t last_offset;
    uint16_t datagram_size;

} rcb;

#ifdef NEW_TEST_T2
int call_with_large_value = 0;
int get_current_scb_cmnd_session_id()
{
    return scb.cmn.session_id;
}
int check_scb_current_dnode()
{
    return scb.current_dnode;
}

int check_flag_fc_timer_expired_once()
{
    return scb.flag_fc_timer_expired_once;
}
#endif
/* IF there is no reception of sending for 1000ms we go back to IDLE state */
/* Helps if transprot service is stuck somewhere */
#ifdef __C51__
VOID_CALLBACKFUNC_PVOID(ZCB_reset_transport_service, puser)
#else
static void reset_transport_service(void *ss)
#endif
{
#ifdef __C51__
  UNUSED(puser);
#endif
  T2_ERR("reset_timer expired going back to ST_IDLE state ");
  ctimer_stop(&scb.reset_timer);
  current_state = ST_IDLE;
  discard_all_received_fragments();
}
#ifdef __C51__
#define FUNC(STR) ZCB##_##STR
#else
#define FUNC(STR) STR
#endif
static uint8_t recv_or_send()
{
    T2_DBG("sending 1: %s", scb.sending? "TRUE": "FALSE");
    switch (current_state) {
    case ST_IDLE:
        return 2; /*Neither sending nor receiving */

    /* Receive state machine states */
    case ST_RECEIVING:
    case ST_SEND_FRAG_COMPLETE:
    case ST_SEND_FRAG_REQ:
    case ST_SEND_FRAG_WAIT:
    case ST_FIND_MISS_FRAG:
        T2_DBG("Sending 2: FALSE");
            return 1;

    /* Send state machine states */
    case ST_SEND_FRAG:
    case ST_SEND_LAST_FRAG:
    case ST_WAIT_ACK:
        T2_DBG("Sending 2: TRUE")
            return 0;
    default:
        T2_ERR("Unkonwn current_state: %s\n", T2_STATES_STRING[current_state]);
        break;
    }
    return -1;
}
BOOL ZW_TransportService_Is_Receving()
{
    return (recv_or_send() == 1)? TRUE: FALSE;
}

BOOL ZW_TransportService_Is_Sending()
{
    return (recv_or_send() == 0)? TRUE: FALSE;
}

#if !defined(ZIPGW) && !defined(NEW_TEST_T2)
BOOL TS_SendRaw(BYTE dst, BYTE *buf, BYTE buflen, BYTE txopt, VOID_CALLBACKFUNC(cb)(unsigned char status, void* user))
{
  memset((BYTE*)&ts_txo, 0, sizeof(ts_txo));
  ts_txo.destNode = dst;
  ts_txo.txOptions = txopt;
  return ZW_SendDataEx(buf, (BYTE)buflen, &ts_txo, cb);
}
#endif


#ifdef ZIPGW
BOOL ZW_TransportService_SendData(ts_param_t* p, const BYTE *pData,
                                  uint16_t dataLength,
                                  void (*completedFunc)(BYTE txStatus, TX_STATUS_TYPE *t))
#else
BOOL ZW_TransportService_SendData(ts_param_t* p, uint8_t *pData,
                                  uint16_t dataLength,
                                  ZW_TransportService_SendData_Callback_t completedFunc)
#endif
{
#if defined(ZIPGW) || defined(__C51__)
    TX_STATUS_TYPE t;
    memset(&t, 0, sizeof(TX_STATUS_TYPE));
#endif
    ctimer_set(&scb.reset_timer, RESET_TIME, FUNC(reset_transport_service), 0);
    T2_DBG("Request for Sending data: dataLength: %d, MyNodeid: %d Source node:%d, Destination node: %d", dataLength, (int)MyNodeID, (int)p->snode, (int)p->dnode);
    if (ZW_TransportService_Is_Sending()) {
        T2_ERR("Another TX session is in progress. session id: %d", scb.cmn.session_id);
        T2_ERR("Sending buffer %p, while new request to send of buffer: %p", scb.datagram, pData);
#if defined(ZIPGW) || defined(__C51__)
        completedFunc(S2_TRANSMIT_COMPLETE_FAIL, &t);
#else
        completedFunc(S2_TRANSMIT_COMPLETE_FAIL, 0);
#endif
        return FALSE;
    }
    if (ZW_TransportService_Is_Receving()) {
        T2_ERR("Another RX session is in progress. session id: %d", rcb.cmn.session_id);
        T2_ERR("Sending buffer %p, while new request to send of buffer: %p", scb.datagram, pData);
#if defined(ZIPGW) || defined(__C51__)
        completedFunc(S2_TRANSMIT_COMPLETE_FAIL, &t);
#else
        completedFunc(S2_TRANSMIT_COMPLETE_FAIL, 0);
#endif
        return FALSE;
    }

    scb.datagram = pData;
    memcpy((uint8_t*)&scb.cmn.p, (uint8_t*)p, sizeof(ts_param_t));
    scb.datagram_len = dataLength;
    scb.cmn.completedFunc = completedFunc;
#if defined(ZIPGW) || defined(__C51__)
    memset((uint8_t*)&scb.cmn.tx_status, 0, sizeof(TX_STATUS_TYPE));
#endif
    scb.sending = FALSE;
    scb.flag_replied_frag_req = 0;
    scb.transmission_aborted = 0x11; /*Initializing to out of range session id */
    scb.flag_send_frag_wait = FALSE;
    scb.flag_reply_frag_req = FALSE;
    scb.round_trip_first_frag = 0;
    scb.flag_fc_timer_expired_once = 0;
    scb.remaining_data_len = 0;
    scb.current_dnode = 0;

    T2_DBG("MyNodeid: %d Source node:%d, Destination node: %d", (int)MyNodeID, (int)p->snode, (int)p->dnode);
    scb.current_dnode = p->dnode;
    switch (current_state) {
        case ST_IDLE:
            T2_DBG("Current state: ST_IDLE");
            t2_sm_post_event(EV_START_SEND); /* send() */
            break;

        case ST_SEND_FRAG:
            T2_DBG("Current state: ST_SEND_FRAG");
            t2_sm_post_event(EV_SEND_NEW_FRAG); /* send() */
            break;

        default:
            T2_ERR("Trying to send fragment from wrong state: %d", current_state);
#if 0
            scb.cmn.completedFunc(S2_TRANSMIT_COMPLETE_FAIL, 0); /*FIXME: Need to decide what to do if we are trying to send while receiving */
            return FALSE;
#endif
            break;
        }
    send_first_frag();
    return TRUE;
}

static void add_crc(uint8_t *buf, uint8_t len)
{
    uint8_t *tmp_buf = buf;
    WORD crc = ZW_CheckCrc16(0x1D0F, tmp_buf, len);
    tmp_buf+=len;
    *tmp_buf++ =  (crc>>8)&0xff;
    *tmp_buf=(crc)&0xff;
}


/*callback frunction when fc timer expires */
#ifdef __C51__
VOID_CALLBACKFUNC_PVOID(ZCB_fc_timer_expired, puser)
#else
void fc_timer_expired(void *nthing)
#endif
{
#ifdef __C51__
  UNUSED(puser);
#endif
    if (scb.flag_replied_frag_req) {
        scb.transmission_aborted = scb.cmn.session_id;
        scb.flag_replied_frag_req = 0;
        scb.current_dnode = 0;
        T2_ERR("FC timer expired after reply_frag_req()");
        T2_ERR("Sending failure to application");
        t2_sm_post_event(EV_FRAG_COMPL_TIMER_REQ);
        if (scb.cmn.completedFunc) {
#if defined(ZIPGW) || defined(__C51__)
            scb.cmn.completedFunc(S2_TRANSMIT_COMPLETE_FAIL, &scb.cmn.tx_status);
#else
            scb.cmn.completedFunc(S2_TRANSMIT_COMPLETE_FAIL, 0);
#endif
        }
        return;
    }

    /* This happens when in Tie break we abort a transmission and then FC completion timer
        event happens. Need to ignore it to make sure we dont send the last fragment again
        and make the state machine end up in weird state */
    /* Tested in test_fc_timer_after_frag_compl_of_aborted_transmission() */
    if (scb.transmission_aborted == scb.cmn.session_id) {
        T2_ERR("FC timer expired for aborted transmission. Ignoring the timer event");
        T2_ERR("Sending failure to application");
        if (scb.cmn.completedFunc) {
#if defined(ZIPGW) || defined(__C51__)
            scb.cmn.completedFunc(S2_TRANSMIT_COMPLETE_FAIL, &scb.cmn.tx_status);
#else
            scb.cmn.completedFunc(S2_TRANSMIT_COMPLETE_FAIL, 0);
#endif
        }
        return;
    }

    /* Tested in test_fc_timer_after_last_frag_twice() */
    if (scb.flag_fc_timer_expired_once) {
        T2_ERR("Frag completion timer event happened twice \n");
        T2_ERR("Sending failure to application");
        scb.flag_fc_timer_expired_once = 0;
        scb.current_dnode = 0;
        if (scb.cmn.completedFunc) {
#if defined(ZIPGW) || defined(__C51__)
            scb.cmn.completedFunc(S2_TRANSMIT_COMPLETE_FAIL, &scb.cmn.tx_status);
#else
            scb.cmn.completedFunc(S2_TRANSMIT_COMPLETE_FAIL, 0);
#endif
        }
        t2_sm_post_event(EV_FRAG_COMPL_TIMER2);
        return;
    }
    T2_DBG("fc_timer_expired once. Sending last fragment again")
    t2_sm_post_event(EV_FRAG_COMPL_TIMER); /*send_last_frag() */
    scb.flag_fc_timer_expired_once++;
    send_last_frag();
}

#ifdef __C51__
void ZCB_temp_callback_last_frag(unsigned char status, void* user);
code const void (code * ZCB_temp_callback_last_frag_p)(unsigned char status, void* user) = &ZCB_temp_callback_last_frag;
void ZCB_temp_callback_last_frag(unsigned char status, void* user)
#else
static void ZCB_temp_callback_last_frag(unsigned char status, void *user)
#endif
{
    UNUSED(user);
#if defined(ZIPGW) || defined(__C51__)
    memcpy((uint8_t*)&scb.cmn.tx_status, user, sizeof(TX_STATUS_TYPE));
#endif
    if (status != S2_TRANSMIT_COMPLETE_OK) {
            T2_ERR("Transmission status is not TRANSMIT_COMPLETE_OK for last_frag");
    }
}

static void send_last_frag(void)
{

    uint8_t ret = 0;
#ifndef __C51__
retry:
#endif
    ctimer_stop(&rcb.fc_timer); /* FIXME this is called twice. First in send_subseq_frag() ? */
    ctimer_set(&scb.reset_timer, RESET_TIME, FUNC(reset_transport_service), 0);
    scb.sending = FALSE;
    /* this is last fragment being sent, so pending_segments are 0 now */
    scb.cmn.pending_segments = 0 ;
    if (scb.does_not_fit_in_first_frag) {
        ret = TS_SEND_RAW(scb.cmn.p.snode,scb.cmn.p.dnode, t2_txBuf, sizeof(*subseq_frag) + scb.datalen_to_send - 1,
                          scb.cmn.p.tx_flags | TRANSMIT_OPTION_ACK, ZCB_temp_callback_last_frag);
    } else {
        ret = TS_SEND_RAW(scb.cmn.p.snode,scb.cmn.p.dnode, t2_txBuf, sizeof(*first_frag) + scb.datalen_to_send - 1,
                          scb.cmn.p.tx_flags | TRANSMIT_OPTION_ACK, ZCB_temp_callback_last_frag);
    }

    if (ret == 0) {
#ifndef __C51__
        goto retry;
        T2_ERR("ZW_SendData failed\n")
        if (scb.flag_fc_timer_expired_once) {
#endif
            if (scb.cmn.completedFunc) {
#if defined(ZIPGW) || defined(__C51__)
                scb.cmn.completedFunc(S2_TRANSMIT_COMPLETE_FAIL, &scb.cmn.tx_status);
#else
                scb.cmn.completedFunc(S2_TRANSMIT_COMPLETE_FAIL, 0);
#endif
            }
            t2_sm_post_event(EV_FAILURE_LAST_FRAG2);
            return;
#ifndef __C51__
        } else {
            scb.flag_fc_timer_expired_once++; /* FIXME: Assuming transmit queue overflow as expired timer */
            goto retry;
        }
#endif
    }
#ifdef TIMER
    if ((scb.cmn.p.tx_flags == RECEIVE_STATUS_TYPE_BROAD) ||
        (scb.cmn.p.dnode == 0xff)) {
        T2_ERR("Fragments being sent were broadcast. Not waiting for fragment complete");
        t2_sm_post_event(EV_MISSING_FRAG_BCAST);
    } else {
        ctimer_set(&rcb.fc_timer, FRAGMENT_FC_TIMEOUT, FUNC(fc_timer_expired), 0);
        t2_sm_post_event(EV_SUCCESS); /* Go to ST_WAIT_ACK state */
    }
#endif
    return;
}

#ifdef __C51__
void ZCB_ts_senddata_cb(unsigned char status_send, TX_STATUS_TYPE* txStatus);
code const void (code * ZCB_ts_senddata_cb_p)(void* puser) = &ZCB_ts_senddata_cb;
void ZCB_ts_senddata_cb(unsigned char status_send, TX_STATUS_TYPE* txStatus)
#else
static void ZCB_ts_senddata_cb(unsigned char status_send, void* user)
#endif
{
    /* FIXME: May be, this should be part of the specs

    Find out how long it took for the callback of FIRST_FRAG,
    wait that much before sending second FRAG. if the receiving
    node wants to send FRAG_WAIT, this will give the receiving node little
    time to breath - Anders Esbensen*/
#ifdef __C51__
  memcpy((uint8_t*)&scb.cmn.tx_status, (uint8_t*)txStatus, sizeof(TX_STATUS_TYPE));
#elif ZIPGW
  memcpy((uint8_t*)&scb.cmn.tx_status, (uint8_t*)user, sizeof(TX_STATUS_TYPE));
#endif
#ifndef ZIPGW
  ZW_DEBUG_SEND_STR("1!\r\n");
#endif
  if (scb.round_trip_first_frag) {
      scb.round_trip_first_frag = clock_time() - scb.round_trip_first_frag;

      /* FIXME 500 below is added to ease the receiving side to send fragment wait if it wants to */
      scb.round_trip_first_frag += 300;
      T2_DBG("Adding delay of scb.round_trip_first_frag: %d ms before sending second fragment", scb.round_trip_first_frag);
    }

#ifndef ZIPGW
    ZW_DEBUG_SEND_STR("2!\r\n");
#endif
    if (status_send != S2_TRANSMIT_COMPLETE_OK) {
            T2_ERR("Transmission status is not TRANSMIT_COMPLETE_OK");
    }
    if (scb.flag_reply_frag_req) {
        scb.flag_reply_frag_req = FALSE;
        reply_frag_req(NULL);
        return;
    }

    if(scb.flag_send_frag_wait) {
        scb.flag_send_frag_wait = FALSE;
        T2_DBG("Send Frag_wait now");
        send_frag_wait_cmd();
        return;
    }
    /*In order not to congest the Z-Wave network,
      large data transfers MUST leave transmit opportunities for her
      nodes in the network. If sending a command longer than two frames,
      a node MUST implement a delay between every transmitted frame. The
      mninimum required time delay and number of frames before delay MUST
      be inserted depends on the actual bit rate.

     40 kbit/s: At least 35 ms if sending more than 2 frames back-to-back
     100 kbit/s: At least 15 ms if sending more than 2 frames back-to-back
    */
    if (scb.transmission_aborted == scb.cmn.session_id) {
        T2_DBG("stopping tranmission for session: %d", scb.transmission_aborted)
        ctimer_stop(&scb.timer_btwn_2_frags);
    } else {
        T2_DBG("Calling ZCB_send_subseq_frag");
        ctimer_set(&scb.timer_btwn_2_frags, 15 + (scb.round_trip_first_frag), FUNC(send_subseq_frag), 0);
#ifdef NEW_TEST_T2
    send_subseq_frag(NULL);
#endif

    }
    scb.round_trip_first_frag = 0;
#ifndef ZIPGW
    ZW_DEBUG_SEND_STR("3!\r\n");
#endif
}

#ifdef __C51__
VOID_CALLBACKFUNC_PVOID(ZCB_send_subseq_frag, puser)
#else
static void send_subseq_frag(void *nthing)
#endif
{
    uint8_t ret = 0;

#ifdef __C51__
  UNUSED(puser);
#endif
    ctimer_stop(&rcb.fc_timer);
    if (scb.remaining_data_len == 0)
        scb.remaining_data_len = scb.datagram_len;

    if (scb.remaining_data_len >= FragmentMaxPayload) {
        scb.datalen_to_send = FragmentMaxPayload;
    } else {
        scb.datalen_to_send = scb.remaining_data_len;
    }

    if (scb.frag_compl_list[scb.cmn.session_id] == TRUE)
    {
        T2_ERR("Already received frag complete command for this session. Aborting any more fragment sending");
        return; /*FIXME just return?*/
    }
    scb.offset = scb.datagram_len - scb.remaining_data_len;
    T2_DBG("Sending Subsequent Fragment scb.offset: %d", scb.offset);
    subseq_frag->cmdClass = COMMAND_CLASS_TRANSPORT_SERVICE;
    subseq_frag->cmd_datagramSize1 = (COMMAND_SUBSEQUENT_FRAGMENT)|((scb.datagram_len>>8)&0x07);
    subseq_frag->datagramSize2 = scb.datagram_len & 0xff;
#if 0//t

    if (frag_wait_once) {
        subseq_frag->properties2  = ((scb.cmn.session_id+1) << 3) | ((scb.offset>>8)&0x07);
            frag_wait_once = 0;
    } else
#endif
    /* properties2 4 MSBs are session id 4th LSB is reserved and 3 LSBs are 3 MSBs of scb.offset */
    subseq_frag->properties2  = (scb.cmn.session_id << 4) | ((scb.offset>>8) & 0x07);
    /* datagramOffset2 is 8 LSBs of scb.offset */
    subseq_frag->datagramOffset2 = scb.offset & 0xff;

    memcpy((uint8_t *)&subseq_frag->payload1, (scb.datagram + scb.offset), scb.datalen_to_send);

    /*5 is size of ZW_COMMAND_SUBSEQUENT_FRAGMENT_1BYTE_FRAME till payload field */
    add_crc((uint8_t *)&subseq_frag->cmdClass, scb.datalen_to_send + 5);

    if (scb.remaining_data_len <= FragmentMaxPayload) {
        scb.does_not_fit_in_first_frag = TRUE;
        t2_sm_post_event(EV_SEND_LAST_FRAG); /* send_last_frag() */
        send_last_frag();
        return;
    }
//        ret = send_data(&scb.cmn.p, t2_txBuf, sizeof(*subseq_frag) + scb.datalen_to_send - 1, ZCB_ts_senddata_cb, NULL);
    ctimer_set(&scb.reset_timer, RESET_TIME, FUNC(reset_transport_service), 0);
    ret = TS_SEND_RAW(scb.cmn.p.snode,scb.cmn.p.dnode, t2_txBuf, sizeof(*subseq_frag) + scb.datalen_to_send - 1,
                              scb.cmn.p.tx_flags | TRANSMIT_OPTION_ACK, ZCB_ts_senddata_cb);
    if (ret == 0) {
        T2_ERR("ZW_SendData failed\n");
    }
#ifndef __C51__
    else
    {
#endif
      if (scb.remaining_data_len >= FragmentMaxPayload) {
          scb.remaining_data_len = scb.remaining_data_len - FragmentMaxPayload;
      }

      scb.cmn.pending_segments = scb.remaining_data_len / scb.datalen_to_send;
      if (scb.remaining_data_len % scb.datalen_to_send) {
          scb.cmn.pending_segments++;
      }
#ifndef __C51__
    }
#endif

    t2_sm_post_event(EV_SEND_NEW_FRAG); /*ZCB_send_subseq_frag*/
}

/* Incase fragment wait command is received */
#ifdef __C51__
VOID_CALLBACKFUNC_PVOID(ZCB_wait_restart_from_first, puser)
#else
static void wait_restart_from_first(void *nthing)
#endif
{
#ifdef __C51__
  UNUSED(puser);
#endif
    send_first_frag();
    return;
}

#if 0 //for debugging. Do not remove.
static void print_data(unsigned char *buf, uint8_t len)
{
    unsigned char *tmp_buf = buf;
    uint8_t i = 0;
        printf("len: %d data:", len);
    for (i = 0; i < len; i++) {
        printf("%x ", (int)*tmp_buf);
        tmp_buf++;
    }
        printf("\n");
}
#endif

static void send_first_frag()
{

    uint8_t ret = 0;

#if 0
    unsigned int iseed = (unsigned int)time(NULL);

    /*FIXME. New specs dont mention about starting the session id from random number?. */
    srand(iseed);
#endif
#ifdef NEW_TEST_T2 /* session id is fixed: 0 in tests */
        scb.cmn.session_id = 0;
#else
    if (!scb.cmn.session_id) /* session id begins with random number and then keeps incrementing */
        scb.cmn.session_id = (rand() % 0xf); /* only 8 bits for session id so max session id can be 0xf*/
    else
        scb.cmn.session_id++;

    T2_DBG("Sending First Fragment");
    if (scb.cmn.session_id > 0xf) /*scb.cmn.session_id has only 4 bits for it */
        scb.cmn.session_id = 0; /* Being back from 0 */
#endif

    scb.frag_compl_list[scb.cmn.session_id] = FALSE;
    FragmentMaxPayload = FRAGMENTMAXPAYLOAD;
#ifndef NEW_TEST_T2
    if(scb.cmn.p.tx_flags & TRANSMIT_OPTION_EXPLORE) {
        T2_DBG("TRANSMIT_OPTION_EXPLORE is on");
        FragmentMaxPayload-=8;
    } else if(!(scb.cmn.p.tx_flags & TRANSMIT_OPTION_NO_ROUTE)) {
        T2_DBG("TRANSMIT_OPTION_NO_ROUTE is on");
        FragmentMaxPayload-=8;
    }
#endif
    T2_DBG("FragmentMaxPayload: %d", FragmentMaxPayload);

    if (scb.remaining_data_len == 0)
        scb.remaining_data_len = scb.datagram_len;

    if (scb.datagram_len > FragmentMaxPayload) {
        scb.remaining_data_len = scb.datagram_len - FragmentMaxPayload;
        scb.datalen_to_send = FragmentMaxPayload;
    } else {
        scb.remaining_data_len = scb.datagram_len;
        scb.datalen_to_send = scb.datagram_len;
    }

    first_frag->cmdClass = COMMAND_CLASS_TRANSPORT_SERVICE;

    /* Take 8th, 9th and 10th bit of scb.datagram_len */
    first_frag->cmd_datagramSize1 = (COMMAND_FIRST_FRAGMENT) | ((scb.datagram_len >> 8) & 0x07);

    /* Take 0th-7th bit of scb.datagram_len */
    first_frag->datagramSize2 = scb.datagram_len & 0xff;
    first_frag->properties2 = scb.cmn.session_id << 4; /*FIXME need to check EXT and Reserved section */
    T2_DBG("packing session id %d", first_frag->properties2 >> 4);

    memcpy((uint8_t*)&first_frag->payload1, scb.datagram, scb.datalen_to_send);
    //printf("Copied following data:");
    //print_data((uint8_t*)&first_frag->payload1, scb.datalen_to_send);

     /*4 is size of ZW_COMMAND_FIRST_FRAGMENT_1BYTE_FRAME till payload field */
    add_crc((uint8_t *)&subseq_frag->cmdClass, scb.datalen_to_send + 4);

    scb.cmn.pending_segments = scb.remaining_data_len / scb.datalen_to_send;
    if (scb.remaining_data_len % scb.datalen_to_send)
        scb.cmn.pending_segments++;

    if (scb.datagram_len <= FragmentMaxPayload) { /* If it was only one fragment */
        scb.does_not_fit_in_first_frag = FALSE;
        t2_sm_post_event(EV_SEND_LAST_FRAG); /* send_last_frag() */
        send_last_frag();
        return;
    }

    //print_data(t2_txBuf, sizeof(*first_frag) + scb.datalen_to_send - 1);
//        ret = send_data(&scb.cmn.p, t2_txBuf, sizeof(*first_frag) + scb.datalen_to_send - 1, ZCB_ts_senddata_cb, NULL);
    ctimer_set(&scb.reset_timer, RESET_TIME, FUNC(reset_transport_service), 0);
    ret = TS_SEND_RAW(scb.cmn.p.snode, scb.cmn.p.dnode, t2_txBuf, sizeof(*first_frag) + scb.datalen_to_send - 1,
                              scb.cmn.p.tx_flags | TRANSMIT_OPTION_ACK, ZCB_ts_senddata_cb);
    if (ret == 0) {
        T2_ERR("send_data failed\n");
        return;
    }

    scb.round_trip_first_frag = clock_time();

    scb.sending = TRUE;
    t2_sm_post_event(EV_SEND_NEW_FRAG); /*ZCB_send_subseq_frag*/
}

#ifdef __C51__
void ZCB_temp_callback_reply_frag_req(unsigned char status, void* user);
code const void (code * ZCB_temp_callback_reply_frag_req_p)(unsigned char status, void* user) = &ZCB_temp_callback_reply_frag_req;
void ZCB_temp_callback_reply_frag_req(unsigned char status, void* user)
#else
static void ZCB_temp_callback_reply_frag_req(unsigned char status, void* user)
#endif
{
    UNUSED(user);
#if defined(ZIPGW) || defined(__C51__)
    memcpy((uint8_t*)&scb.cmn.tx_status, user, sizeof(TX_STATUS_TYPE));
#endif
    if (status != S2_TRANSMIT_COMPLETE_OK) {
        if (scb.cmn.completedFunc) {
#if defined(ZIPGW) || defined(__C51__)
            scb.cmn.completedFunc(status, user);
#else
            scb.cmn.completedFunc(status, 0);
#endif

        }
    }
    t2_sm_post_event(EV_SENT_MISS_FRAG);

}

/*TODO this has to be aligned in sending session similarly to send_frag_wait_cmd() */
static void reply_frag_req(void* nthing)
{
    uint8_t ret;
#ifdef __C51__
    UNUSED(nthing);
#endif
    if (scb.frag_compl_list[scb.cmn.session_id] == TRUE) {
        T2_ERR("Already received frag complete command for this session. Aborting any more fragment sending");
        return;
    }
    scb.datalen_to_send = FragmentMaxPayload;
    T2_DBG("Resending offset: %d", scb.missing_offset);

    subseq_frag->cmdClass = COMMAND_CLASS_TRANSPORT_SERVICE;

    if ((scb.missing_offset + scb.datalen_to_send) > scb.datagram_len) { /*last fragment */
        scb.datalen_to_send = scb.datagram_len - scb.missing_offset;
    }

    subseq_frag->cmd_datagramSize1 = (COMMAND_SUBSEQUENT_FRAGMENT) |
                                            ((scb.datagram_len>>8)&0x07);
    subseq_frag->datagramSize2 = scb.datagram_len & 0xff;
    subseq_frag->properties2  = (scb.cmn.session_id << 4) | ((scb.missing_offset>>8)&0x07);
    subseq_frag->datagramOffset2 = scb.missing_offset & 0xff;
    memcpy((uint8_t *)&subseq_frag->payload1, (scb.datagram + scb.missing_offset),
           scb.datalen_to_send);

    /* 5 is size of ZW_COMMAND_SUBSEQUENT_FRAGMENT_1BYTE_FRAME till payload
     * field */
    add_crc((uint8_t *)&subseq_frag->cmdClass, scb.datalen_to_send + 5);
    if ((scb.missing_offset + scb.datalen_to_send) == scb.datagram_len) { /*last fragment */
        T2_DBG("Resending last fragmnet");
        scb.does_not_fit_in_first_frag = TRUE;
        t2_sm_post_event(EV_SEND_LAST_MISS_FRAG); /* send_last_frag() */
        scb.flag_replied_frag_req = 1;
        send_last_frag();
        return;
    }

//  ret = send_data(&scb.cmn.p, t2_txBuf, sizeof(*subseq_frag) + scb.datalen_to_send - 1, ZCB_temp_callback_reply_frag_req, NULL);
    ctimer_set(&scb.reset_timer, RESET_TIME, FUNC(reset_transport_service), 0);
    if (scb.sending) {
        ret = TS_SEND_RAW(scb.cmn.p.snode, scb.cmn.p.dnode, t2_txBuf, sizeof(*subseq_frag) + scb.datalen_to_send - 1,
                              scb.cmn.p.tx_flags | TRANSMIT_OPTION_ACK, ZCB_ts_senddata_cb);
    } else {
        ret = TS_SEND_RAW(scb.cmn.p.snode, scb.cmn.p.dnode, t2_txBuf, sizeof(*subseq_frag) + scb.datalen_to_send - 1,
                              scb.cmn.p.tx_flags | TRANSMIT_OPTION_ACK, ZCB_temp_callback_reply_frag_req);
    }
    if (ret == 0) {
        T2_ERR("send_data failed\n");
    }
    scb.flag_replied_frag_req = 1;
    /*FIXME: After replying to fragment request, the code wait for fragment complete or another fragment request.
        But on receive side decision of another fragment request or fragment complete is taken when rx timer expires after 800ms
        this makes the FC timer here on sending side expire so adding 500ms more here */
    ctimer_set(&rcb.fc_timer, (FRAGMENT_FC_TIMEOUT + 500), FUNC(fc_timer_expired), 0);

}

/* ----------------------------------------------------------------------------------
 * receive part
 *----------------------------------------------------------------------------------*/


#ifdef ZIPGW
typedef void (*ZW_CommandHandler_Callback_t)(ts_param_t*, ZW_APPLICATION_TX_BUFFER*, WORD);
#endif
ZW_CommandHandler_Callback_t TSApplicationCommandHandler;

static void receive(void);
static uint8_t send_frag_complete_cmd();
static uint8_t send_frag_req_cmd();

void ZW_TransportService_Init(ZW_CommandHandler_Callback_t commandHandler)
{
    /* Set the callback function which will be called by this layer to notify
     upper (application) layer */
    TSApplicationCommandHandler = commandHandler;
}

#ifdef __C51__
void TransportService_ApplicationCommandHandler(ts_CommandHandler_t *pCmdHandlerStruct)
{
    ts_param_t* p;
    uint8_t *pCmd;
    uint8_t cmdLength;
    uint8_t cmd_type;
    uint8_t datagram_size_tmp;

    p = pCmdHandlerStruct->pParam;
    pCmd = (uint8_t*)pCmdHandlerStruct->pCmd;
    cmdLength = pCmdHandlerStruct->wCmdLength;
#else
void TransportService_ApplicationCommandHandler(ts_param_t* p,
                                                uint8_t *pCmd,
                                                uint8_t cmdLength)
{
    uint8_t cmd_type;
    uint16_t datagram_size_tmp;
#endif

    ctimer_set(&scb.reset_timer, RESET_TIME, FUNC(reset_transport_service), 0);
    T2_DBG("Received data: Source node:%d, Destination node: %d", (int)p->snode, (int)p->dnode);
    /* Tie break check */
    /* 1. The receiving node is currently transmitting a datagram.
     * 2. The recipient of the datagram being transmitted is also the
            originator of the received fragment
     * 3. The receiving node has a lower NodeID than the originator */
    if (ZW_TransportService_Is_Sending() && /* 1st condition */
       (scb.cmn.p.dnode == p->snode) && /* 2nd condition */
       (MyNodeID < p->snode)) { /* 3rd condition */
        T2_ERR("Tie breaking. Failing the send session. Ready to receive");
        T2_DBG("ZW_TransportService_Is_Sending() is true. scb.cmn.p.dnode: %d, p->snode: %d, MyNodeID: %d", scb.cmn.p.dnode, p->snode, MyNodeID);
        t2_sm_post_event(EV_TIE_BREAK);
        flag_tie_broken = 1;
        /*FIXME: Can not FAIL the transmission because of following reason:
            When GW is sending to some node. On receiving frag compl for a
            transmision from that node, if we have following line GW will fail the
            transmission just because of tie break logic
        scb.cmn.completedFunc(TRANSMIT_COMPLETE_FAIL, 0);
        */
    }

    T2_DBG("Mynodeid: %d, Source node:%d, Destination node: %d", MyNodeID, (int)p->snode, (int)p->dnode);

    /* There are some garbage retranmissions where the source and destination ids are messed up */
    if (p->snode == p->dnode) {
        T2_ERR("source and destination is same node id? Ignoring the frame")
        return;
    }

    T2_ERR("cmdLength: %d\n",cmdLength);
    if (cmdLength > DATAGRAM_SIZE_MAX) {
        T2_ERR("Length of command received is more than DATAGRAM_SIZE_MAX. Ignorning the frame")
#ifdef NEW_TEST_T2
        call_with_large_value = 1;
#endif
        return;
    }

    /* incase FRAG_WAIT has to be sent backup the ts_param_t received */
    memcpy((uint8_t*)&scb.frag_wait_p, (uint8_t*)p, sizeof(ts_param_t));

    if (((p->snode != rcb.current_snode) && (rcb.current_snode)) || /* received frame from third node while receiving from second node */
            ((p->snode != scb.current_dnode) && (scb.current_dnode))) { /* received frame from third node while sending to second node */
        if (p->rx_flags == RECEIVE_STATUS_TYPE_SINGLE) {
            T2_DBG("Current source node is %d but received source node id is %d, session_id: %d", rcb.current_snode, p->snode,  ((*((uint8_t *)(pCmd + 3))& 0xf0) >> 4));
            T2_DBG("Current dest node is %d but received source node id is %d, session_id : %d", scb.current_dnode, p->snode, ((*((uint8_t *)(pCmd + 3))& 0xf0) >> 4));

            /*FIXME workaround to ignore further singlecast frames from different node */
            if (current_state == ST_SEND_FRAG_WAIT) {
                return;
            }
            t2_sm_post_event(EV_SCAST_DIFF_NODE); /*send_frag_wait_cmd */
            if (scb.sending) {
                T2_DBG("Next fragment sent will be FRAG_WAIT to %d", p->snode)
                scb.flag_send_frag_wait = TRUE;
            } else {
                send_frag_wait_cmd();
            }
            return;
        }

        if (p->rx_flags == RECEIVE_STATUS_TYPE_BROAD) {
            t2_sm_post_event(EV_BCAST_DIFF_NODE); /*drop_fragment */
            return;
        }
    }

    cmd_type = *((uint8_t *)pCmd + 1);
    cmd_type = cmd_type & 0xf8;

    if ((rcb.cmn.session_id == 0x10) && ( cmd_type == COMMAND_SUBSEQUENT_FRAGMENT)) {
        datagram_size_tmp  = (*((uint8_t *)pCmd + 1)) & 0x07;
        datagram_size_tmp = (datagram_size_tmp << 8) + (*((uint8_t *)pCmd + 2));

        if (datagram_size_tmp > DATAGRAM_SIZE_MAX) {
            T2_ERR("datagram size is more than DATAGRAM_SIZE_MAX. Ignoring the fragment\n");
            datagram_size_tmp = 0;
            t2_sm_post_event(EV_DIFF_SESSION);
            return;
        }
        T2_ERR("Received subseq fragment without first fragment. session_id:%d",  ((*((uint8_t *)(pCmd + 3))& 0xf0) >> 4));
        t2_sm_post_event(EV_SUBSEQ_DIFF_SESSION);
        if (scb.sending) {
            scb.flag_send_frag_wait = TRUE;
        } else {
            send_frag_wait_cmd();
        }
        return;
    }

    switch (current_state)
    {
        case ST_IDLE:
            t2_sm_post_event(EV_START_RECV);
            break;
        case ST_RECEIVING:
            t2_sm_post_event(EV_RECV_NEW_FRAG);
            break;
        case ST_WAIT_ACK:
            /*Next state is decided in receive() */
            /*Only fragment complete, fragment request or fragment wait are expected */
            break;
        default:
            T2_ERR("Received a fragment in unexpected state %s. Processing it, incase we need to send FRAG_WAIT.", T2_STATES_STRING[current_state]);
            /*no break no return*/
            break;
    }

    if (flag_initialize_once) {
        T2_DBG("Initializing rcb.cmn.session_id to 0x10");
        flag_initialize_once = 0;
        memset((uint8_t*)&rcb.cmn, 0, sizeof(control_block_t));
        rcb.cmn.session_id = 0x10;
        rcb.flag_retry_frag_req_once = 1;
        rcb.cur_recvd_data_size = 0;
        rcb.max_data_in_frag = 0;
        rcb.current_snode = 0;
        rcb.last_offset = 0;
        memset(rcb.datagramData, 0, DATAGRAM_SIZE_MAX);
    }
    rcb.fragment = pCmd;
    /*need to memcpy because the (ts_param_t*)p pointer is not valid when
     the rx_timer_expired is called by the timer*/
    memcpy((uint8_t*)&rcb.cmn.p, (uint8_t*)p, sizeof(ts_param_t));
    rcb.fragment_len = cmdLength;

    receive();
    return;
}

static uint8_t mark_frag_received(WORD offset)
{
    /* rcb.cur_recvd_data_size = frag.len - FIRST_FRAG_NONPAYLOAD_LENGTH; */
    uint8_t frag_no = 0;

    if (!rcb.max_data_in_frag)
        return 1;

    if (offset != 0)
        frag_no= offset / rcb.max_data_in_frag;

    if ((offset != 0) && (rcb.list_recvd_frag[0] == FALSE)) {
        T2_ERR("Received subseq fragment without first fragment.");
        t2_sm_post_event(EV_SUBSEQ_DIFF_SESSION);
        if (scb.sending) {
            scb.flag_send_frag_wait = TRUE;
        } else {
            send_frag_wait_cmd();
        }
        return 1;
    }

    T2_DBG("marking %dth fragment of offset: %d received", (int)frag_no, (int)offset);
    if (rcb.list_recvd_frag[frag_no] == TRUE) {
        T2_ERR("Error. duplicate fragemnt of offset %d received\n", offset);
        return 1; /* need to discard the duplicate fragment */
    } else {
        rcb.list_recvd_frag[frag_no] = TRUE;
        return 0;
    }
}

/* Get number of missing fragments in the datagram */
static uint8_t get_num_missing_frag()
{
    uint8_t missing_frag = 0;
    uint8_t max_frag_no;

    /* To avoid divide by zero in next step */
    if (rcb.max_data_in_frag == 0)
        return 0;

    max_frag_no = rcb.datagram_size / rcb.max_data_in_frag;

    if (rcb.datagram_size % rcb.max_data_in_frag)
        max_frag_no++;

    while (max_frag_no--) {
        if (rcb.list_recvd_frag[max_frag_no] == FALSE) {
                missing_frag++;
        }
    }

    T2_DBG("missing_frag: %d", missing_frag);
    return missing_frag;
}

#ifdef TIMER

#ifdef __C51__
VOID_CALLBACKFUNC_PVOID(ZCB_rx_timer_expired, puser)
#else
static void rx_timer_expired(void *ss)
#endif
{
#ifdef __C51__
    struct rx_timer_expired_data *rdata = puser;
#else
    struct rx_timer_expired_data *rdata = ss;
#endif
    uint8_t state = rdata->state;

#ifdef TIMER
    ctimer_stop(&rcb.rx_timer);
#endif

#if 0 /* Following code is just for information purpose */
    /* If the rx_timer event happened while sending frag request */
    if (current_state == ST_SEND_FRA_REQ) {
            t2_sm_post_event(EV_FRAG_RX_TIMER); /* discard_all_received_fragments() */
    } else {
            t2_sm_post_event(EV_FRAG_RX_TIMER); /* send_frag_req_cmd */
    }
#endif
    /* There could be two functions called after this depending on current
     * state. See code above */
    t2_sm_post_event(EV_FRAG_RX_TIMER);
    if (state && (get_num_missing_frag(/*rcb.datagram_size*/))) {
        T2_ERR("rx timer expired after sending Fragment Request");
        T2_ERR("Discarding all fragments");
        discard_all_received_fragments();
    } else {
        find_missing();
    }
/*
    T2_DBG("ctimer_set rcb.rx_timer");
    ctimer_set(&rcb.rx_timer, FRAGMENT_RX_TIMEOUT, ZCB_rx_timer_expired, 0);
*/
}
#endif

static void find_missing()
{
    uint8_t missing_frag;

    missing_frag = get_num_missing_frag(/*rcb.datagram_size*/);

    if (missing_frag) {
        if (rcb.cmn.p.rx_flags == RECEIVE_STATUS_TYPE_BROAD) {
            T2_DBG("There are missing fragments, but in broadcast datagram. Not sending fragment request command to sender");
            discard_all_received_fragments();
            return;
        }
        t2_sm_post_event(EV_MISSING_FRAG); /* send_frag_req_cmd */
        send_frag_req_cmd();
    } else {
        /* No need to send Fragment complete in case of Broadcast */
        if (rcb.cmn.p.rx_flags == RECEIVE_STATUS_TYPE_BROAD) {
            T2_DBG("Fragment transfer has compoleted, but in broadcast datagram. Not sending fragment complete command to sender");
            return;
        }
        t2_sm_post_event(EV_SEND_FRAG_COMPLETE);
        send_frag_complete_cmd();
    }
}

/* this is only used on receiving side. i.e. to send fragment wait when in the middle of receiving session
    a fragment is received from different node */
//void calculate_pending_segments(uint16_t datagram_size, uint16_t offset, uint8_t cur_recvd_data_size)
void calculate_pending_segments()
{
    /*FIXME: Handle the case of last fragment with size less than 41bytes.
    Not critical just calculates pending segments as 2 for last segment instead of 1 */
#if 0
    rcb.cmn.pending_segments = (datagram_size - offset) / cur_recvd_data_size;
    if (datagram_size % cur_recvd_data_size)
        rcb.cmn.pending_segments++;
#endif
    uint8_t max_frag_no;

    rcb.cmn.pending_segments = 0;
    /* To avoid divide by zero in next step */
    if (rcb.max_data_in_frag == 0) {
        return;
    }

    max_frag_no = rcb.datagram_size / rcb.max_data_in_frag;
    T2_DBG("max_frag_no: %d", max_frag_no);

    if (rcb.datagram_size % rcb.max_data_in_frag) {
        max_frag_no++;
    }

    while (max_frag_no--) {
        if (rcb.list_recvd_frag[max_frag_no] == FALSE) {
            rcb.cmn.pending_segments++;
        }
    }
}

static void receive(void)
{
#if defined(ZIPGW) || defined(__C51__)
    TX_STATUS_TYPE t;
#endif

    uint8_t byte1 = *((uint8_t *)rcb.fragment + 1);
    uint8_t byte2 = *((uint8_t *)rcb.fragment + 2);
    uint8_t byte3 = *((uint8_t *)rcb.fragment + 3);
    uint8_t byte4 = *((uint8_t *)rcb.fragment + 4);

    uint16_t datagram_offset = 0; /*It has to fit 11 bits so need to be two BYTE */
    uint8_t *curr_datagramData;
    uint8_t recvd_session_id = 0;
    uint16_t datagram_size_tmp;

    if (*((uint8_t *)rcb.fragment) != COMMAND_CLASS_TRANSPORT_SERVICE) {
        T2_ERR("Command class is not COMMAND_CLASS_TRANSPORT_SERVICE");
        return;
    }

    switch (byte1 & 0xf8) {
    case COMMAND_FIRST_FRAGMENT:
        T2_DBG("Received First Fragment");
        if (flag_tie_broken) {
            scb.transmission_aborted = scb.cmn.session_id;
        }
        //print_data((BYTE*)rcb.fragment, rcb.fragment_len);
#define FIRST_FRAG_NONPAYLOAD_LENGTH (sizeof(ZW_COMMAND_FIRST_FRAGMENT_1BYTE_FRAME) - 1)
#define SUBSEQ_FRAG_NONPAYLOAD_LENGTH (sizeof(ZW_COMMAND_SUBSEQUENT_FRAGMENT_1BYTE_FRAME) - 1)

        if (rcb.fragment_len <= FIRST_FRAG_NONPAYLOAD_LENGTH) {
#ifdef __C51__
            T2_ERR("Length of received fragment is less than %d", FIRST_FRAG_NONPAYLOAD_LENGTH)
#else
            T2_ERR("Length of received fragment is less than %i", (int)FIRST_FRAG_NONPAYLOAD_LENGTH)
#endif
            t2_sm_post_event(EV_RECV_NEW_FRAG);
            return;
        }

        /* If first fragment received is corrupt send fragment wait command */
        if (ZW_CheckCrc16(0x1D0F, (uint8_t*) rcb.fragment, rcb.fragment_len) != 0) {
            T2_ERR("CRC error. Discarding fragment");
            /*FIXME: Do we need to send FRAG_WAIT here? */
            t2_sm_post_event(EV_RECV_NEW_FRAG);
            return;
        }

        recvd_session_id = (byte3 & 0xf0) >> 4;
        T2_DBG("recvd_sesion_id is %d", recvd_session_id);
        if ((recvd_session_id != rcb.cmn.session_id) && (rcb.cmn.session_id != 0x10)) { /*Refer 10.1.3.1.5 */
        T2_DBG("Current session is %d but received session id is %d. Ignoring the fragment", rcb.cmn.session_id, recvd_session_id);
            t2_sm_post_event(EV_DIFF_SESSION);
            return;
        }
        datagram_size_tmp = (byte1) & 0x07;
        datagram_size_tmp = (datagram_size_tmp << 8) + byte2;

        if (datagram_size_tmp > DATAGRAM_SIZE_MAX) {
            T2_ERR("datagram size is more than DATAGRAM_SIZE_MAX. Ignoring the fragment\n");
            t2_sm_post_event(EV_DIFF_SESSION);
            return;
        }
#ifdef TIMER
        ctimer_set(&rcb.rx_timer, FRAGMENT_RX_TIMEOUT, FUNC(rx_timer_expired), &rcb.rx_data);
#endif
        rcb.cmn.session_id = recvd_session_id;
        rcb.recv_frag_compl_list[recvd_session_id] = FALSE; /* Setting this session id as "havent received FRAG_COMPLETE for it"*/


        rcb.cur_recvd_data_size = rcb.fragment_len - FIRST_FRAG_NONPAYLOAD_LENGTH;

        rcb.datagram_size = datagram_size_tmp;

        rcb.current_snode = rcb.cmn.p.snode;

        rcb.max_data_in_frag = rcb.cur_recvd_data_size;
#define FIRST_HDR_LEN 4 /* Cmd class, cmd, size, seqno */
#define SUBSEQ_HDR_LEN 5 /* Cmd class, cmd, size, seqno + offset 1, offset 2*/
        memcpy(rcb.datagramData, rcb.fragment + FIRST_HDR_LEN,
               rcb.cur_recvd_data_size);
        if(mark_frag_received(0))
            return;

        /* The current fragment had all the data needed for the datagram */
        if (rcb.cur_recvd_data_size == rcb.datagram_size) {
            t2_sm_post_event(EV_SEND_FRAG_COMPLETE); /* send_frag_complete_cmd */
            send_frag_complete_cmd();
            return;
        }

        /* The current fragment had more data than the size of
           whole datagram. TODO: Something wrong?*/
        if (rcb.cur_recvd_data_size > rcb.datagram_size) {
            T2_ERR("Something went wrong. Current fragment has more data than needed in this datagram");
            //t2_sm_post_event(EV_ERROR);
        }
        rcb.rx_data.state = 0; /*not after sending req cmd */
        break;

    case COMMAND_SUBSEQUENT_FRAGMENT:
        /* Stay in the same function and handle fragment */
        T2_DBG("Received Subsequent Fragment");
        if (flag_tie_broken) {
            scb.transmission_aborted = scb.cmn.session_id;
        }

        if (rcb.fragment_len <= SUBSEQ_FRAG_NONPAYLOAD_LENGTH) {
#ifdef __C51__
            T2_ERR("Length of received subseq fragment is less than %d. Ignoring the fragment", SUBSEQ_FRAG_NONPAYLOAD_LENGTH)
#else
            T2_ERR("Length of received subseq fragment is less than %i. Ignoring the fragment", (int)SUBSEQ_FRAG_NONPAYLOAD_LENGTH)
#endif
            /*FIXME: Do we need to send FRAG_WAIT here? */
            t2_sm_post_event(EV_RECV_NEW_FRAG);
            return;
        }
        /* If subseq fragment received is corrupt just ignore it */
        if (ZW_CheckCrc16(0x1D0F, (uint8_t*) rcb.fragment, rcb.fragment_len) != 0) {
            T2_ERR("CRC error. Ignoring");
            /*FIXME: Do we need to send FRAG_WAIT here? */
            t2_sm_post_event(EV_RECV_NEW_FRAG);
            return;
        }

        recvd_session_id = (byte3 & 0xf0) >> 4;

        if (rcb.recv_frag_compl_list[recvd_session_id] == TRUE) {
            T2_ERR("Already received Fragment Complete command for this session: %d. Looks like duplicate frame", recvd_session_id);
            if (current_state == ST_RECEIVING) {
                t2_sm_post_event(EV_DUPL_FRAME);
            } else {
                T2_ERR("Strange current state: %s(%d)", T2_STATES_STRING[current_state], current_state);
            }
            return;
        }
        /* session ID of new received fragment is different from the one being assembled */
        if ((recvd_session_id != rcb.cmn.session_id) && (rcb.cmn.session_id != 0x10)) {
            T2_DBG("Current session is %d but recived session id is %d. Ignoring fragment", rcb.cmn.session_id, recvd_session_id);
            t2_sm_post_event(EV_DIFF_SESSION);
            return;
        }

        datagram_offset = byte3 & 0x07; /* last three bits of 2nd byte */
        datagram_offset = (datagram_offset << 8) + byte4;

        // Also checks if first fragment for this datagram is missing and we received subsequent fragment.
        // Sends FRAG_WAIT as well

        T2_DBG("offset: %d", datagram_offset);
        rcb.cur_recvd_data_size = rcb.fragment_len - SUBSEQ_FRAG_NONPAYLOAD_LENGTH;

        if ((datagram_offset + rcb.cur_recvd_data_size) > DATAGRAM_SIZE_MAX) {
            T2_ERR("Offset of fragment received is more than DATAGRAM_SIZE_MAX. Ignoring fragment");
            if (current_state == ST_RECEIVING) {
                t2_sm_post_event(EV_DUPL_FRAME);
            }
            return;
        }
        datagram_size_tmp  = (byte1) & 0x07;
        datagram_size_tmp = (datagram_size_tmp << 8) + byte2;

        if (datagram_size_tmp > DATAGRAM_SIZE_MAX) {
            T2_ERR("datagram size is more than DATAGRAM_SIZE_MAX. Ignoring the fragment\n");
            t2_sm_post_event(EV_DIFF_SESSION);
            return;
        }
#ifdef TIMER
        ctimer_set(&rcb.rx_timer, FRAGMENT_RX_TIMEOUT, FUNC(rx_timer_expired), &rcb.rx_data);
#endif
        if (mark_frag_received(datagram_offset))
                break;

        T2_DBG("Pending Segments: %d", rcb.cmn.pending_segments);
        rcb.datagram_size = datagram_size_tmp;
        curr_datagramData = rcb.datagramData; /* Should not change the global buffer address */
        curr_datagramData = curr_datagramData + datagram_offset;

        memcpy(curr_datagramData, rcb.fragment + SUBSEQ_HDR_LEN, rcb.cur_recvd_data_size);

        if (((datagram_offset + rcb.cur_recvd_data_size) >= rcb.datagram_size)) { /*last fragment? */
            t2_sm_post_event(EV_RECV_LAST_FRAG); /*find_missing()*/
            find_missing();
            return;
        }
        rcb.rx_data.state = 0; /*not after sending req cmd */
        break;

   case COMMAND_SEGMENT_REQUEST_V2:
        t2_sm_post_event(EV_FRAG_REQ_OR_COMPL);
        T2_DBG("Received Fragment Request Command");
        //T2_DBG("byte2: %x", byte2)
        recvd_session_id = (byte2 & 0xf0) >> 4;
        /*Fragment request is not from the same session in which we were sending */

        if (recvd_session_id == scb.transmission_aborted) {
            T2_DBG("COMMAND_FRAGMENT_REQUEST: for aborted transmionss session:%d. Igoring... ", recvd_session_id);
            t2_sm_post_event(EV_FRAG_REQ_COMPL_WAIT_DIFF_SESSION);
            return;
        }
        if (recvd_session_id != scb.cmn.session_id) {
            T2_DBG("Current session is %d but recived session id is %d. Ignoring...", scb.cmn.session_id, recvd_session_id);
            t2_sm_post_event(EV_FRAG_REQ_COMPL_WAIT_DIFF_SESSION);
            return;
        }
        if ((rcb.cmn.p.snode != scb.current_dnode) && (rcb.current_snode != 0)) { /* Check if the FRAG REQ is from the destination node where we were sending data to */
            T2_ERR("Session id of Fragment request received is not same as session_id of fragment being sent, recvd_session_id: %d, scb.cmn.session_id: %d. Ignoring the Frag request command", recvd_session_id, scb.cmn.session_id);
            t2_sm_post_event(EV_FRAG_REQ_COMPL_WAIT_DIFF_NODE);
            return;
        }

#ifdef TIMER
        ctimer_stop(&rcb.fc_timer);
#endif
        scb.missing_offset = ((byte2 & 0x7) << 8);
        scb.missing_offset |= byte3;
        //T2_DBG("Frag req cmd for %d missing fragment", (int)scb.missing_offset);

        t2_sm_post_event(EV_RECV_FRAG_REQ); /* reply_frag_req(); */
        if (scb.sending) {
            scb.flag_reply_frag_req = TRUE;
        } else {
            reply_frag_req(NULL);
        }
        break;

   case COMMAND_SEGMENT_COMPLETE_V2:
        t2_sm_post_event(EV_FRAG_REQ_OR_COMPL);
        T2_DBG("Received Fragment Complete Command");
        recvd_session_id = (byte2 & 0xf0) >> 4;
        if (recvd_session_id == scb.transmission_aborted) {
            T2_DBG("COMMAND_FRAGMENT_COMPLETE: for aborted transmionss session:%d. Igoring... ", recvd_session_id);
            t2_sm_post_event(EV_FRAG_REQ_COMPL_WAIT_DIFF_SESSION);
            return;
        }
        /*Fragment complete is not from the same session in which we were sending */
        if (recvd_session_id != scb.cmn.session_id) {
            T2_ERR("Current session is %d but recived session id is %d", recvd_session_id, rcb.cmn.session_id);
            t2_sm_post_event(EV_FRAG_REQ_COMPL_WAIT_DIFF_SESSION);
            return;
        }
        if ((rcb.cmn.p.snode != scb.current_dnode) && (rcb.current_snode != 0)) { /* Check if the FRAG complete is from the destination node where we were sending data to */
            T2_ERR("Session id of Fragment request received is not same as session_id of fragment being sent, recvd_session_id: %d, scb.cmn.session_id: %d. Ignoring the Frag request command", recvd_session_id, scb.cmn.session_id);
            t2_sm_post_event(EV_FRAG_REQ_COMPL_WAIT_DIFF_NODE);
            return;
        }
#ifdef TIMER
        ctimer_stop(&rcb.fc_timer);
#endif
        T2_DBG("recvd_session_id : %d, scb.cmn.completedFunc: %p", recvd_session_id, scb.cmn.completedFunc);
        if (scb.cmn.session_id == recvd_session_id) {
            scb.frag_compl_list[recvd_session_id] = TRUE;
            scb.current_dnode = 0;
            if (scb.cmn.completedFunc) {
                T2_DBG("Sending back TRANSMIT_COMPLETE_OK to client");
#if defined(ZIPGW) || defined(__C51__)
                scb.cmn.completedFunc(S2_TRANSMIT_COMPLETE_OK, &scb.cmn.tx_status);
#else
                scb.cmn.completedFunc(S2_TRANSMIT_COMPLETE_OK, 0);
#endif
            }
        } else {
            T2_ERR("Fragment complete session id is %d while current session id is %d", recvd_session_id, scb.cmn.session_id);
        }

        t2_sm_post_event(EV_RECV_FRAG_COMPL); /* Go back to ST_IDLE state */
        break;

   case COMMAND_SEGMENT_WAIT_V2:
       /* Though the code flow is in receive() function. Current state is still be ST_SEND_FRAG */
       T2_DBG("Received Fragment wait Command");
        if (scb.frag_compl_list[scb.cmn.session_id] == TRUE) {
            T2_ERR("Already received Fragment Complete command for this session: %d", scb.cmn.session_id);
            t2_sm_post_event(EV_DUPL_FRAME);
            return;
        }
        t2_sm_post_event(EV_RECV_FRAG_WAIT);
        scb.transmission_aborted = scb.cmn.session_id;
        /* call ZCB_ts_senddata_cb() here that will halt the next fragment send function called from ZCB_ts_senddata_cb() */
#if defined(ZIPGW) || defined(__C51__)
        memset(&t, 0, sizeof(TX_STATUS_TYPE));
        ZCB_ts_senddata_cb(S2_TRANSMIT_COMPLETE_FAIL, &t);
#else
        ZCB_ts_senddata_cb(S2_TRANSMIT_COMPLETE_FAIL, 0);
#endif
        /* Refer 10.1.3.5.3 */
        rcb.cmn.pending_segments = byte2;
        T2_DBG("Pending fragments: %d", rcb.cmn.pending_segments);
        /*FIXME: Shall we increment the scb.sending session id here or should we send it in same session id */
        /* If the pending segments are 0 then the sending side is going to bombard the receiving side with new fragments
            so added a delay of 100ms regardless of number of pending segments */
        ctimer_set(&scb.wait_restart_timer, (100 + 100 * rcb.cmn.pending_segments), FUNC(wait_restart_from_first), NULL);
        break;
    default:
        T2_ERR("Unknown command type: %d", *((uint8_t *)rcb.fragment + 1));
        break;
    }
    return;
}

/* FIXME: FRAGMENT_WAIT should have session id field inside it. Delayed FRAGMENT_WAIT frames end up stopping the wrong session.
Then the sending side starts new session which is ignored and then receiving side sends fragment request which are ignored on
sending side as the sending side has ended the session. Which makes rx_timer expire and then receiving side figures out that
receiving side has been sending subseq fragments without first fragment then (as it ignored few fragments).
Then receiving side sends frag wait which finally makes the transfer happen as sending side restarts the third session */
static uint8_t send_frag_wait_cmd()
{
    uint8_t ret = 0;
    ZW_COMMAND_SEGMENT_WAIT_V2_FRAME frag_wait;

    frag_wait.cmdClass = COMMAND_CLASS_TRANSPORT_SERVICE;
    frag_wait.cmd_reserved = (COMMAND_SEGMENT_WAIT_V2 & 0xf8);
    ctimer_set(&scb.reset_timer, RESET_TIME, FUNC(reset_transport_service), 0);
    if (scb.sending) { /* If there is a sending session going on FRAG_WAIT will be queed for next callback*/
        T2_DBG("Sending fragment wait command. Pending segments: %d", scb.cmn.pending_segments);
        frag_wait.pendingFragments = scb.cmn.pending_segments;
        T2_DBG("Sending FRAG_WAIT from sending session snode: %d dnode: %d", scb.frag_wait_p.dnode,  scb.frag_wait_p.snode);
        ret = TS_SEND_RAW(scb.frag_wait_p.dnode, scb.frag_wait_p.snode, (uint8_t *)&frag_wait, sizeof(frag_wait),
                                 scb.frag_wait_p.tx_flags | TRANSMIT_OPTION_ACK, ZCB_ts_senddata_cb);
        t2_sm_post_event(EV_SUCCESS2); /* Go back to ST_SEND_FRAG state in */
    } else {
        if (rcb.cmn.session_id == 0x10) {
            rcb.cmn.pending_segments = 0;
        } else {
            calculate_pending_segments();
        }
        T2_DBG("Sending fragment wait command. Pending segments: %d", rcb.cmn.pending_segments);
        frag_wait.pendingFragments = rcb.cmn.pending_segments;
        T2_DBG("Sending FRAG_WAIT from receiving session snode: %d dnode: %d", scb.frag_wait_p.dnode,  scb.frag_wait_p.snode);
        ret = TS_SEND_RAW(scb.frag_wait_p.dnode, scb.frag_wait_p.snode, (uint8_t *)&frag_wait, sizeof(frag_wait),
                                 scb.frag_wait_p.tx_flags | TRANSMIT_OPTION_ACK, NULL);
        t2_sm_post_event(EV_SUCCESS); /* Go back to ST_RECEIVING state in receive() funciton */
    }

    /*TODO SPEC: What to do if sending frag wait fails*/
    if (ret == 0) {
        T2_ERR("send_data failed\n");
    }
    return 0;
}

static uint8_t send_frag_complete_cmd()
{
    uint8_t ret = 0;
    //uint8_t i;

    T2_DBG("Sending COMMAND_FRAGMENT_COMPLETE\n");
    ZW_COMMAND_SEGMENT_COMPLETE_V2_FRAME frag_compl;

    if (rcb.cmn.session_id > 0x0f) { /* Session ID has only 4 bits for it.*/
        T2_ERR("Session id is more than 15");
        return 0;
    }

    frag_compl.cmdClass = COMMAND_CLASS_TRANSPORT_SERVICE;
    frag_compl.cmd_reserved = (COMMAND_SEGMENT_COMPLETE_V2 & 0xf8);

    frag_compl.properties2 = (rcb.cmn.session_id << 4);
    ctimer_set(&scb.reset_timer, RESET_TIME, FUNC(reset_transport_service), 0);
    ret = TS_SEND_RAW(rcb.cmn.p.dnode, rcb.cmn.p.snode, (uint8_t *)&frag_compl, sizeof(frag_compl),
                              rcb.cmn.p.tx_flags | TRANSMIT_OPTION_ACK, NULL);
    if (ret == 0) {
        T2_ERR("send_data failed\n"); /* TODO What to do of sending Frag Compl fails */
    }

#if 0
    printf("test_pData: \n");
    for (i = test_pData_len -1 ; i > 0; --i) {
         printf("%x ", test_pData[i]);
    }
    printf("\nrcb.datagramData: \n");
    for (i = test_pData_len -1 ; i > 0; --i) {
         printf("%x ", rcb.datagramData[i]);
    }
#endif
#ifndef __C51__
    ZIPCommandHandler(rcb.cmn.p.snode, rcb.datagram_size); /**/
#endif /* ifndef __C51__ */

    /* Resetting for next session */
    rcb.recv_frag_compl_list[rcb.cmn.session_id] = TRUE;
    rcb.cmn.session_id = 0x10;
    rcb.current_snode = 0;
    /* Set all fragments as missing */
    memset(rcb.list_recvd_frag, 0, sizeof(rcb.list_recvd_frag));

    /* Need to set back the index to zero so that next get_next_missing_frag() will
       start finding the missing fragments from beginnning of the rcb.list_recvd_frag[] array */
    rcb.last_offset = 0;

#ifdef TIMER
    ctimer_stop(&rcb.rx_timer);
#endif
    /* FIXME: should this be in the call back? */
    t2_sm_post_event(EV_SUCCESS); /* just change the state to ST_RECEIVING */
#ifdef __C51__
#if DATAGRAM_SIZE_MAX > 250
#error Datagram size does not fit in uin8_t.
#endif
    TransportService_msg_received_event((BYTE*) rcb.datagramData, (uint8_t)rcb.datagram_size,  rcb.cmn.p.snode);
#endif /* __C51 __*/
    return 0;
}

static uint16_t get_next_missing_frag()
{
    uint8_t max_offset = rcb.datagram_size / rcb.max_data_in_frag;
    uint8_t i = 0;

    if (rcb.datagram_size % rcb.max_data_in_frag)
         max_offset++;

    for (i = rcb.last_offset; i < max_offset; i++) {
        if (rcb.list_recvd_frag[i] == FALSE) {
            rcb.last_offset = i;
            T2_DBG("Offset missing: %d", i * rcb.max_data_in_frag);
            return (i * rcb.max_data_in_frag);
        }
    }
    return 0;
}

static uint8_t send_frag_req_cmd()
{
  ZW_COMMAND_SEGMENT_REQUEST_V2_FRAME *frag_req =
                    (ZW_COMMAND_SEGMENT_REQUEST_V2_FRAME *) t2_txBuf;
    uint16_t offset_to_request;
    uint8_t ret1 = 0;


    offset_to_request = get_next_missing_frag();
    if (!offset_to_request) {
        T2_ERR("No offset_to_request is missing");
        t2_sm_post_event(EV_SUCCESS); /* Just change the state to ST_RECEIVING */
        return 0;
    }

    if (rcb.cmn.session_id > 0x0f) {/* Session ID has only 4 bits for it.*/
        T2_ERR("Session id is more than %d", 0x0f);
        return 0;
    }

    frag_req->cmdClass = COMMAND_CLASS_TRANSPORT_SERVICE;
    frag_req->cmd_reserved =  (COMMAND_SEGMENT_REQUEST_V2) & 0xf8;
    frag_req->properties2 = rcb.cmn.session_id << 4;
    frag_req->properties2 |= ((offset_to_request & 0x700) >> 8); /* Get 9th, 10th and 11th MSB */
    frag_req->datagramOffset2 = (offset_to_request & 0xff);

#ifndef __C51__
retry:
#endif
    T2_DBG("Sending fragment request command for offset: %d in session id: %d", offset_to_request, rcb.cmn.session_id);

    /* At receiver t2_txBuf is only needed in sending frag request.
     * max size = 21 _to_reqoffsets * 2bytes + size of frag req cmd header
     * BYTE t2_txBuf[(21 * 2) + sizeof(ZW_COMMAND_FRAGMENT_REQUEST_1BYTE_FRAME)]; */
    //ret1 = send_data(&rcb.cmn.p, t2_txBuf, sizeof(*frag_req), NULL, NULL);
    ctimer_set(&scb.reset_timer, RESET_TIME, FUNC(reset_transport_service), 0);
    ret1 = TS_SEND_RAW(rcb.cmn.p.dnode, rcb.cmn.p.snode, t2_txBuf, sizeof(*frag_req),
                              rcb.cmn.p.tx_flags | TRANSMIT_OPTION_ACK, NULL);
    if (ret1 == FALSE) {
        /* TODO SPEC: what to do if frag req cmd fails */
        T2_ERR("send_data failed ");
        if (rcb.flag_retry_frag_req_once) {
            rcb.flag_retry_frag_req_once--;
#ifndef __C51__
            goto retry;
#endif
        }
    }

    /*TODO Got to wait here some time or wait for ACK */
    rcb.rx_data.state = 1; /*after sending frag req, as we need to discard fragments in rx_timer_expired */
#ifdef TIMER

    t2_sm_post_event(EV_SUCCESS); /* FIXME: should this be in the call back? Just change the state to ST_RECEIVING */
    ctimer_set(&rcb.rx_timer, FRAGMENT_RX_TIMEOUT, FUNC(rx_timer_expired), &rcb.rx_data);
#endif
    return 0;
}

static uint8_t discard_all_received_fragments(void)
{
    memset(rcb.datagramData, 0, sizeof(rcb.datagramData));

    memset((uint8_t*)&rcb.cmn, 0, sizeof(control_block_t));
    rcb.cmn.session_id = 0x10;
    memset(rcb.list_recvd_frag, 0, sizeof(rcb.list_recvd_frag));
    rcb.current_snode = 0;
    return 0;
}




