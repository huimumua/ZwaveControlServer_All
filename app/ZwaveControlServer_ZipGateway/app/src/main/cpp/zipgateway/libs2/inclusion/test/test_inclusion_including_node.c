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
 */
/*
 * test_inclusion.c
 *
 *  Created on: Aug 25, 2015
 *      Author: trasmussen
 */

#include "s2_inclusion.h"
#include <mock_control.h>
#include <stdint.h>
#include <string.h>
#include "platform.h"
#include "unity.h"
#include "s2_protocol.h"

typedef uint8_t BYTE; // TODO: Is this the right place for defining these?
typedef uint16_t WORD;
#include "ZW_classcmd.h"
#include "s2_classcmd.h"

#define ELEM_COUNT(ARRAY)  (sizeof(ARRAY)/(sizeof(ARRAY[0])))

/** Overview of behavior which should be implemented:
 *  General inclusion:
 *  - Implement SM according to designed flow.
 *  - Identify all events and actions which must be handled and executed during node inclusion.
 *  - It is expected that a common S2 layer resides above the inclusion module (and other S2
 *    related modules) which is responsible for dispatching packet/events based on command.
 *    In the tests it is expected that a learn request will initiate the inclusion flow.
 *  - Early discussions on future direction has been initiated between Torsten, Jakob, and Anders on event generation
 *    Early ideas is to have an event notification system which could be simplified version of SKB in Linux.
 *    This work must progress but in S2_inclusion.c a simple notification system will be taken into use with refactoring possibilities.
 *
 *
 *  Error handling:
 *  - Timeout: it must be ensured that a proper timer API is available for the inclusion state machine is available.
 *             In case a timeout occurs, it must be ensured that the inclusion state machine returns to idle and
 *             appropriate error is notified to callback provided by protocol layer.
 *             Error handler provided by protocol layer is expected to be implemented by application..
 *             The SM will not be responsible for further actions due to timeouts.
 *
 */

/** Current working assumptions:
 *  Lib S2 - message encapsulation/inclusion mode and context handling
 *  -	Currently context is updated with peer, buffer, and length in libs2 protocol (message
 *     encapsulation handling), this should be moved to common place. s2 dispatch layer ?
 *  -	Libs2 S2.c contains the entry point for frames receined, S2_application_command_handler(.),
 *     this should ideally be refactored to upper dispatcher layer which should be common for encapsulation handling and inclusion.
 */

/** Currently identyfied test cases not yet implemented.
 *
 *  - Key Verify failure.
 *
 */
#define UNIT_TEST_BUF_SIZE  256

static uint8_t  m_test_mem_pool[4][64];  // Statically allocated chunks of memory that can be freely used during the test.

// Public key A as used in ECDH curve test cases.
static uint8_t  m_test_public_key_a[] = {0x85, 0x20, 0xf0, 0x09, 0x89, 0x30, 0xa7, 0x54,
                                         0x74, 0x8b, 0x7d, 0xdc, 0xb4, 0x3e, 0xf7, 0x5a,
                                         0x0d, 0xbf, 0x3a, 0x0d, 0x26, 0x38, 0x1a, 0xf4,
                                         0xeb, 0xa4, 0xa9, 0x8e, 0xaa, 0x9b, 0x4e, 0x6a};

// Public key B as used in ECDH curve test cases.
static uint8_t  m_test_public_key_b[] = {0xde, 0x9e, 0xdb, 0x7d, 0x7b, 0x7d, 0xc1, 0xb4,
                                         0xd3, 0x5b, 0x61, 0xc2, 0xec, 0xe4, 0x35, 0x37,
                                         0x3f, 0x83, 0x43, 0xc8, 0x5b, 0x78, 0x67, 0x4d,
                                         0xad, 0xfc, 0x7e, 0x14, 0x6f, 0x88, 0x2b, 0x4f};



//static uint8_t  m_temp_key[] = {0x7E, 0xFE, 0x12, 0x32, 0x45, 0x65, 0x78, 0x98,
//                                0x7E, 0xFE, 0x12, 0x32, 0x45, 0x65, 0x78, 0x98};

static uint8_t  m_test_network_key_s2_class_0[] = {0x11, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
                                                   0x00, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99};

static uint8_t  m_test_network_key_s2_class_1[] = {0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
                                                   0x00, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99};

static uint8_t  m_test_network_key_s2_class_2[] = {0x22, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
                                                   0x00, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99};

static uint8_t  m_test_network_key_s0[] = {0xCA, 0xFE, 0xBA, 0xBE, 0x44, 0x33, 0x22, 0x11,
                                           0xCA, 0xFE, 0xBA, 0xBE, 0xCC, 0xBB, 0xAA, 0x99};

s2_connection_t s2_conn;
/** Event listener for S2 inclusion.
 *
 *  S2 will use an event listening scheme for posting notifications upwards.
 *  How the notification is stored/handled is outside scope of the state machine, but it is
 *  imagined that such system would implement a queing system to allow inclusion operations
 *  to progress as fast as possible.
 *
 *  This forward declares the mock funtion used, because the callback is a function pointer with no named declaration.
 */
void s2_event_handler(zwave_event_t * p_zwave_evt);

/** Forward declaration of helper classes.
 *  All helper classes are implemented in buttom of file to easily keep them seperated from the test cases.
 */
void helper_func_kex_report_frame(struct S2 *p_context);
void helper_func_echo_kex_report_frame_expect(uint8_t expected_scheme, uint8_t expected_curves, uint8_t expected_keys);
void helper_func_kex_set_frame_expect(uint8_t expected_scheme, uint8_t expected_curves, uint8_t expected_keys);
void helper_func_pub_key_frame(struct S2 *p_context);
void helper_func_echo_kex_set_frame(struct S2 *p_context);
void helper_func_net_key_get_frame(struct S2 *p_context, uint8_t key);
void helper_func_net_key_verify_frame(struct S2 *p_context);
void helper_func_verify_idle_state(struct S2 *p_context);
void helper_func_kex_get_frame_expect(void);
void helper_func_restore_keys_expect(void);

/** This is not a real test case but a test to ensure that the including node
 *  code in inclusion is correctly working when building for a controller.
 */
void test_controller_build()
{
#ifndef ZW_CONTROLLER
  TEST_FAIL_MESSAGE("ZW_CONTROLLER is not defined but including node (ZW Controller) test cases are being executed.");
#endif
}

/** Verification that the normal flow succeeds in inclusion of a new node.
 *
 * It verifies the behavior as seen from an including node (Controller node) as described in SDS11274.
 *
 * When a node is to be included securely it is expected that a ZW_SendData is send.
 * For this the common S2_send_frame(...) defined in s2.h is used, which will be implemented elsewhere.
 */

void kex_including_node_state_machine(uint8_t csa) {
  uint8_t public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  uint8_t private_key_a[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, // Private key.
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  uint8_t s2_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT,
                                   0x00, // bit 0 is echo field, rest are reserved
                                   0x02, // Supported schemes. Scheme 1.
                                   0x01, // Supported ECDH Profiles, bit0=1 is curve 25519 value.
                                   0x82};// Requested keys bits. Security 2 class 1, Security 0 network key.
  if(csa) {
    s2_kex_report_frame[2] = 0x2;
  }

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));

  // Stubbed as it is about to be removed.
  mock_call_use_as_stub(TO_STR(S2_network_key_update));

  /**
   *  Test expectation setup.
   * This section set up the expectations in the system when the inclusion state machine progresses:
   * 1) Starting in idle it is expected that an S2 frame KEX1 will be transmitted based on external event when a node id has been assigned.
   * 2) After S2 KEX Get has been transmitted, the FSM awaits a S2 KEX Report as response.
   * 3) After S2 KEX Report is received then S2 KEX Set shall be sent.
   * 4) After S2 KEX Set  has been transmitted, the FSM awaits a Public KeyB from joining node.
   * 5) Exchange of public keys.
   *    a) Public KeyB is received from joining node A.
   *       - Public KeyB must be pushed upwards as event to be presented to user for further verification.
   *         If node is rejected, then node is part of network insecurely. Should this be changed ? Maybe force a node exclude.
   *         Currently that use case is outside the scope of libs2.
   *       - When Public KeyB is received, the controller must send its public KeyA.
   *         Public KeyB shall be provided by the application.
   *       - Using Pub KeyB and Private KeyA then a temporary key, KeyT is derived from the x coordinate of the shared secret.
   *         This is done in security sub module (currently under development)
   *       - The security context must be re-initialized with temporary derived key.
   *    b) Public KeyA is transmitted from including node A, the state machine will await 'Echo (KEX Set)'.
   * 6) Echoing of KEX frames.
   *    a) A 'Echo(KEX Set)' is expected to be received from the joining node, and it must be
   *       verified that 'Echo(KEX Set)' is identical to the KEX Set sent at step 3).
   *    b) 'Echo(KEX Report)' with content identical to packet received in 3) shall be replied to the joining node.
   * x) Nonce exchange: When joining node wants to send 'Echo(KEX Set)' the S2 protocol layer will exchange a nonce prior to communication.
   * 7) Network key exchange:
   *    a) Joining node will request the network key, by sending a 'Network Key Get'.
   *    b) Including node shall reply with a 'Network Key Set'.
   *    c) Joining node shall confirm with a 'Network Key Verify' which is encrypted using the the key obtained by 'NetworkKey-expand'
   *       - Including node must verify that it can succesfully decrypt the message using the key.
   *    d) Including node shall reply with a 'Transfer End'.
   *    e) Joining node shall reply with a 'Transfer End' if no more keys shall be exchanged.
   *
   */
  mock_t * p_mock;

  // Expect a S2 KEX Get to be sent.
  uint8_t S2_kex_get_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame);

  // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
  // The operator (or including app automatically) can then respond with granted keys.
  zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
  p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x82;
  p_expected_report_event->evt.s2_event.s2_data.kex_report.csa = csa;

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_report_event;

  // Expect a S2 KEX Set to be sent.
  uint8_t S2_kex_set_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET,
                                0x00,  // bit0: echo field, bit1-7: Reserved.
                                0x02,  // Selected schemes: scheme 1.
                                0x01,  // Selected curve25519
                                0x82   // Keys to exchange, Security2, class 1 - Security0, network key.
  };
  if(csa) {
    S2_kex_set_frame[2] = 0x2;
  }
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1] = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_set_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_set_frame);

  if(!csa) {
    // When the public key is received, we expect that the event from libs2 contains the key in order to present it for the operator.
    // Therefore we copy the key minus header frame into expected data.
    zwave_event_t  * p_expected_challenge_event = (zwave_event_t *)m_test_mem_pool[1];
    p_expected_challenge_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
    p_expected_challenge_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
    memcpy(p_expected_challenge_event->evt.s2_event.s2_data.challenge_req.public_key, m_test_public_key_b, sizeof(m_test_public_key_b));

    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_challenge_event;
  }

  // When the public key is received, we expect a call to the keystore in order to obtain our public key.
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;

  // Expect Public KeyA to be sent.
  uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};
  if(csa) {
    S2_pub_key_A_frame[3]=0;
    S2_pub_key_A_frame[4]=0;
    S2_pub_key_A_frame[5]=0;
    S2_pub_key_A_frame[6]=0;
  }

  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_pub_key_A_frame);

  // After our public key has been sent, we expect user acceptance of the inclusion, which will result in key updates and key derivation.
  mock_call_expect(TO_STR(keystore_private_key_read), &p_mock);
  p_mock->output_arg[0].pointer = private_key_a;
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;


  // Expect Echo(KEX Report) to be sent.
  uint8_t S2_echo_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x01, 0x02, 0x01, 0x82}; // Note: Echo flag set.
  if(csa) {
    S2_echo_kex_report_frame[0x2] = 0x3;
  }

  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1] = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_echo_kex_report_frame; // Ideally, this should be updated to be identically to replayed received KEX Set.
  p_mock->expect_arg[3].value    = sizeof(S2_echo_kex_report_frame);

  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x02;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_1;

  // Expect Net Key Report to be sent.
  uint8_t S2_net_key_report_2_frame[19] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_REPORT, 0x02, };
  memcpy(&S2_net_key_report_2_frame[3], m_test_network_key_s2_class_1, sizeof(m_test_network_key_s2_class_1));
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1] = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_net_key_report_2_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_net_key_report_2_frame);

  // Expect S2 Transfer End to be sent.
  uint8_t S2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x02};
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1] = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_transfer_end_frame);

  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x80;
  p_mock->output_arg[1].pointer = m_test_network_key_s0;

  // Expect Net Key Set to be sent.
  uint8_t S2_net_key_report_0_frame[19] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_REPORT, 0x80, };
  memcpy(&S2_net_key_report_0_frame[3], m_test_network_key_s0, sizeof(m_test_network_key_s0));
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1] = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_net_key_report_0_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_net_key_report_0_frame);

  // Expect S2 Transfer End to be sent.
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1] = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_transfer_end_frame);

  // When S2 Transfer End is received, we expect keys to be restored and a corresponding Node inclusion complete event from libs2.
  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x01;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_0;

  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x02;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_1;

  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x04;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_2;

  zwave_event_t  * p_expected_complete_event = (zwave_event_t *)m_test_mem_pool[2];
  p_expected_complete_event->event_type = S2_NODE_INCLUSION_COMPLETE_EVENT;
  p_expected_complete_event->evt.s2_event.s2_data.inclusion_complete.exchanged_keys = 0x82;

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_complete_event ;

  /**
   *  Test execution.
   */
  struct S2     s2_context;

  /*FIXME S2_init_ctx() bomb placed */

  s2_context.inclusion_state = S2_INC_IDLE;
  s2_inclusion_set_event_handler(s2_event_handler);

  // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  // KEX Report frame received.
  s2_context.buf    = s2_kex_report_frame;
  s2_context.length = sizeof(s2_kex_report_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_key_grant(&s2_context, 1, 0x82,csa);

  uint8_t public_key_frame[3 + sizeof(m_test_public_key_b)] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x00}; // Key exchange received from slave - public key for secure exchange of LTK.
  memcpy(&public_key_frame[3], m_test_public_key_b, sizeof(m_test_public_key_b));
  s2_context.buf    = public_key_frame;
  s2_context.length = sizeof(public_key_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  if(!csa) {
    s2_inclusion_challenge_response(&s2_context, 1, m_test_public_key_a);
  }
  // Echo(KEX Set) frame received.
  // bit0: echo field set, bit1-7: Reserved.
  // Selected schemes: scheme 0 and scheme 2.
  // Selected curve25519
  // Keys to exchange, Security2, class 2 - Security0, network key.
  uint8_t s2_echo_kex_set_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x01, 0x02, 0x01, 0x82};
  if(csa) {
    s2_echo_kex_set_frame[0x2] = 0x3;
  }

  s2_context.buf    = s2_echo_kex_set_frame;
  s2_context.length = sizeof(s2_echo_kex_set_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);

  // Network Key Get frame received.
  uint8_t s2_net_key_get_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_GET, 0x02};   // Keys requested, Security2, class 2 - Security0, network key.
  s2_context.buf    = s2_net_key_get_frame;
  s2_context.length = sizeof(s2_net_key_get_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);

  // Network Key Verify frame received.
  uint8_t s2_net_key_verify_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_VERIFY};
  s2_context.buf    = s2_net_key_verify_frame;
  s2_context.length = sizeof(s2_net_key_verify_frame);
  s2_context.peer.class_id = 0x01;
  s2_inclusion_post_event(&s2_context);

  // Network Key Get frame received.
  uint8_t s2_net_key_get_0_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_GET, 0x80};   // Keys requested, Security2, class 2 - Security0, network key.
  s2_context.buf    = s2_net_key_get_0_frame;
  s2_context.length = sizeof(s2_net_key_get_0_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);

  // Network Key Verify frame received.
  uint8_t s2_net_key_verify_0_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_VERIFY};
  s2_context.buf    = s2_net_key_verify_0_frame;
  s2_context.length = sizeof(s2_net_key_verify_0_frame);
  s2_context.peer.class_id = 0x01;
  s2_inclusion_post_event(&s2_context);

  // S2 Transfer end frame received.
  // bit0: Key request complete set,
  // bit1: Key verified not set,
  // bit2-7: Reserved.
  uint8_t s2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x01};
  s2_context.buf    = s2_transfer_end_frame;
  s2_context.length = sizeof(s2_transfer_end_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);

  mock_calls_verify();
}

void test_kex_including_node_state_machine_ssa(uint8_t csa) {
  kex_including_node_state_machine(0);
}
void test_kex_including_node_state_machine_csa(uint8_t csa) {
  kex_including_node_state_machine(1);
}


/** Verification that the normal flow succeeds when send done is succesful.
 *
 */
void test_kex_including_node_sond_complete() {
//  uint8_t public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
//                            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
//                            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
//                            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};
//
//  uint8_t private_key_a[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, // Private key.
//                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
//                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
//                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
//
  uint8_t s2_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT,
                                   0x00, // bit 0 is echo field, rest are reserved
                                   0x02, // Supported schemes. Scheme 1.
                                   0x01, // Supported ECDH Profiles, bit0=1 is curve 25519 value.
                                   0x82};// Requested keys bits. Security 2 class 1, Security 0 network key.

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));
  mock_call_use_as_stub(TO_STR(s2_event_handler));
  mock_call_use_as_stub(TO_STR(S2_send_data));
  mock_call_use_as_stub(TO_STR(S2_send_frame));
  mock_call_use_as_stub(TO_STR(s2_keystore_mock.c));

  // Stubbed as it is about to be removed.
  mock_call_use_as_stub(TO_STR(S2_network_key_update));

  /**
   *  Test execution.
   */
  struct S2     s2_context;

  /*FIXME S2_init_ctx() bomb placed */

  s2_context.inclusion_state = S2_INC_IDLE;
  s2_inclusion_set_event_handler(s2_event_handler);

  // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);
  TEST_ASSERT_EQUAL_UINT8(S2_AWAITING_KEX_REPORT, s2_context.inclusion_state);

  // KEX Report frame received.
  s2_context.buf    = s2_kex_report_frame;
  s2_context.length = sizeof(s2_kex_report_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);
  TEST_ASSERT_EQUAL_UINT8(S2_AWAITING_KEY_USER_ACCEPT, s2_context.inclusion_state);

  s2_inclusion_key_grant(&s2_context, 1, 0x82,0);
  TEST_ASSERT_EQUAL_UINT8(S2_AWAITING_PUB_KEY_B, s2_context.inclusion_state);

  uint8_t public_key_frame[3 + sizeof(m_test_public_key_b)] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x00}; // Key exchange received from slave - public key for secure exchange of LTK.
  memcpy(&public_key_frame[3], m_test_public_key_b, sizeof(m_test_public_key_b));
  s2_context.buf    = public_key_frame;
  s2_context.length = sizeof(public_key_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);
  TEST_ASSERT_EQUAL_UINT8(S2_AWAITING_USER_ACCEPT, s2_context.inclusion_state);

  s2_inclusion_challenge_response(&s2_context, 1, m_test_public_key_a);
  TEST_ASSERT_EQUAL_UINT8(S2_AWAITING_ECHO_KEX_SET, s2_context.inclusion_state);

  // Echo(KEX Set) frame received.
  // bit0: echo field set, bit1-7: Reserved.
  // Selected schemes: scheme 0 and scheme 2.
  // Selected curve25519
  // Keys to exchange, Security2, class 2 - Security0, network key.
  uint8_t s2_echo_kex_set_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x01, 0x02, 0x01, 0x82};
  s2_context.buf    = s2_echo_kex_set_frame;
  s2_context.length = sizeof(s2_echo_kex_set_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);
  TEST_ASSERT_EQUAL_UINT8(S2_AWAITING_NET_KEY_GET, s2_context.inclusion_state);

  // Network Key Get frame received.
  uint8_t s2_net_key_get_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_GET, 0x02};   // Keys requested, Security2, class 2 - Security0, network key.
  s2_context.buf    = s2_net_key_get_frame;
  s2_context.length = sizeof(s2_net_key_get_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);
  TEST_ASSERT_EQUAL_UINT8(S2_AWAITING_NET_KEY_VERIFY, s2_context.inclusion_state);

  // Network Key Verify frame received.
  uint8_t s2_net_key_verify_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_VERIFY};
  s2_context.buf    = s2_net_key_verify_frame;
  s2_context.length = sizeof(s2_net_key_verify_frame);
  s2_context.peer.class_id = 0x01;
  s2_inclusion_post_event(&s2_context);
  TEST_ASSERT_EQUAL_UINT8(S2_AWAITING_NET_KEY_GET, s2_context.inclusion_state);

  // Network Key Get frame received.
  uint8_t s2_net_key_get_0_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_GET, 0x80};   // Keys requested, Security2, class 2 - Security0, network key.
  s2_context.buf    = s2_net_key_get_0_frame;
  s2_context.length = sizeof(s2_net_key_get_0_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);
  TEST_ASSERT_EQUAL_UINT8(S2_AWAITING_NET_KEY_VERIFY, s2_context.inclusion_state);

  // Network Key Verify frame received.
  uint8_t s2_net_key_verify_0_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_VERIFY};
  s2_context.buf    = s2_net_key_verify_0_frame;
  s2_context.length = sizeof(s2_net_key_verify_0_frame);
  s2_context.peer.class_id = 0x01;
  s2_inclusion_post_event(&s2_context);
  TEST_ASSERT_EQUAL_UINT8(S2_AWAITING_TRANSFER_END, s2_context.inclusion_state);

  // S2 Transfer end frame received.
  // bit0: Key request complete set,
  // bit1: Key verified not set,
  // bit2-7: Reserved.
  uint8_t s2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x01};
  s2_context.buf    = s2_transfer_end_frame;
  s2_context.length = sizeof(s2_transfer_end_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);
  TEST_ASSERT_EQUAL_UINT8(S2_INC_IDLE, s2_context.inclusion_state);

  mock_calls_verify();
}

/** Test case for ensuring correct timer handling during inclusion.
 *
 *  Purpose: In order to ensure correct inclusion flow and avoid deadlock during inclusion it is
 *           important that in case messages are never received during inclusion, then the state
 *           machine must return to idle a push a corresponding event.
 *
 *  Not to test: This test case will only verify expectations related to the timer handling.
 *               Thus content of frames sent between the nodes during inclusion are not
 *               relevant for this test as they are verified in: test_kex_including_node_state_machine()
 *               NOTE: messages related to Timeout will be verified in this test.
 */
void test_kex_inclusion_timer_handling() {
  mock_t *    p_mock;
  uint8_t     frame_buffer[UNIT_TEST_BUF_SIZE];
  struct S2   s2_context;

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(S2_send_frame));
  mock_call_use_as_stub(TO_STR(S2_send_data));
  mock_call_use_as_stub(TO_STR(s2_event_handler));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(s2_keystore_mock.c));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  /** Timer start expected on every transmitted frame.
    * For flow chart, see SDS11274.
    * Timer values:
    * - TA1, TA2, TA4, TA5, TA6, TA7, TA : 10s
    * - TA3                              : 20s
    * - TAI1, TAI2                       : 240s
    */

  // When initiating the secure inclusion of node B, then we expect a Timer Start call.
  mock_call_expect(TO_STR(s2_inclusion_set_timeout), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;
  p_mock->expect_arg[1].value = 1000;
  p_mock->return_code.value   = 1;

  // After pushing up the event with requested keys, then we expect a reset of the timer with 240s for user input.
  mock_call_expect(TO_STR(s2_inclusion_set_timeout), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;
  p_mock->expect_arg[1].value = 24000;
  p_mock->return_code.value   = 1;

  // After transmitting KEX Set we expect a reset of the timer.
  mock_call_expect(TO_STR(s2_inclusion_set_timeout), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;
  p_mock->expect_arg[1].value = 1000;
  p_mock->return_code.value   = 1;

  // After transmitting Public Key and pushing event upwards with received public key we expect a reset of the timer.
  // Timeour is 240s for the user to provide the missing part of the DSK.
  mock_call_expect(TO_STR(s2_inclusion_set_timeout), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;
  p_mock->expect_arg[1].value = 24000;
  p_mock->return_code.value   = 1;

  // After transmitting echo(KEX Report) we expect a reset of the timer.
  mock_call_expect(TO_STR(s2_inclusion_set_timeout), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;
  p_mock->expect_arg[1].value = 1000;
  p_mock->return_code.value   = 1;

  // After transmitting Network Key Set we expect a reset of the timer.
  mock_call_expect(TO_STR(s2_inclusion_set_timeout), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;
  p_mock->expect_arg[1].value = 1000;
  p_mock->return_code.value   = 1;

  // After transmitting Transfer End we expect a reset of the timer.
  mock_call_expect(TO_STR(s2_inclusion_set_timeout), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;
  p_mock->expect_arg[1].value = 1000;
  p_mock->return_code.value   = 1;

  // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context, &s2_conn);

  helper_func_kex_report_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_key_grant(&s2_context, 1, 0x82,0);

  helper_func_pub_key_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_challenge_response(&s2_context, 1, m_test_public_key_a);

  helper_func_echo_kex_set_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);

  helper_func_net_key_get_frame(&s2_context, 0x02);
  s2_inclusion_post_event(&s2_context);

  helper_func_net_key_verify_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);

  mock_calls_verify();
}


/** Test case for ensuring correct timer handling on timeout during inclusion in case a timeout
 *  occurs after sending KEX Get.
 *
 *  Purpose: In order to ensure correct inclusion flow and avoid deadlock during inclusion it is
 *           important that in case messages are never received during inclusion, then the state
 *           machine must return to idle and push a corresponding event.
 *
 *  Test: This test case will verify expectations related to the timer handling.
 *        If a joining node does not respond within the timeout to a S2 inclusion frame then the
 *        state machine must return to idle and push the corresponding event upwards to application.
 *        After timeout, we expect the system to retry node inclusion.
 *
 */
void test_kex_inclusion_timeout_kex_get_with_retry() {
  mock_t    * p_mock;
  uint8_t     frame_buffer[UNIT_TEST_BUF_SIZE];
  struct S2   s2_context;

  mock_calls_clear();
  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(keystore_network_key_read));

  /** Timer start expected on every transmitted frame.
    * For flow chart, see SDS11274.
    * Timer values:
    * - TA1, TA2, TA4, TA5, TA6, TA7, TA : 10s
    * - TA3                              : 20s
    * - TI1                              : 240s
    */

  // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
  uint8_t S2_kex_get_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame);

  // When initiating the secure inclusion of node B, then we expect a Timer Start call.
  mock_call_expect(TO_STR(s2_inclusion_set_timeout), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;
  p_mock->expect_arg[1].value = 1000;
  p_mock->return_code.value         = 1;

  // This test will trigger a timeout to the inclusion module after which we expect to receive a
  // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
  zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_event;

  // After receiving a timeout it is expected that the inclusion state machine is back in idle and
  // that we can initiate a new inclusion which will also trigger a new set timeout call.
  uint8_t S2_kex_get_frame_retry[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame_retry;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame_retry);

  mock_call_expect(TO_STR(s2_inclusion_set_timeout), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;
  p_mock->expect_arg[1].value = 1000;
  p_mock->return_code.value   = 1;

  // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  s2_inclusion_notify_timeout(&s2_context);

  // Retry node inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  mock_calls_verify();
}


/** Test case for ensuring correct timer handling on timeout during inclusion in case a timeout
 *  occurs after sending KEX Set.
 *
 *  Purpose: In order to ensure correct inclusion flow and avoid deadlock during inclusion it is
 *           important that in case messages are never received during inclusion, then the state
 *           machine must return to idle and push a corresponding event.
 *
 *  Test: This test case will verify expectations related to the timer handling.
 *        If a joining node does not respond within the timeout to a S2 inclusion frame then the
 *        state machine must return to idle and push the corresponding event upwards to application.
 *        After timeout, we expect the system to retry node inclusion.
 *
 */
void test_kex_inclusion_timeout_kex_set_with_retry() {
  mock_t    * p_mock;
  uint8_t     frame_buffer[UNIT_TEST_BUF_SIZE];
  struct S2   s2_context;

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(keystore_network_key_read));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
  uint8_t S2_kex_get_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame);

  // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
  // The operator (or including app automatically) can then respond with granted keys.
  zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
  p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x82;

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_report_event;

  // Expect a S2 KEX Set to be sent.
  uint8_t S2_kex_set_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x00, 0x02, 0x01, 0x82};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_set_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_set_frame);

  // This test will trigger a timeout to the inclusion module after which we expect to receive a
  // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
  zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[1];
  p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_event;

  // After receiving a timeout it is expected that the inclusion state machine is back in idle and
  // that we can initiate a new inclusion.
  uint8_t S2_kex_get_frame_retry[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame_retry;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame_retry);

  // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  // KEX Report frame received.
  uint8_t s2_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x00, 0x02, 0x01, 0x82}; // Scheme 1, curve25519, S2, key 1, and S0 network key.
  s2_context.buf    = s2_kex_report_frame;
  s2_context.length = sizeof(s2_kex_report_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_key_grant(&s2_context, 1, 0x82,0);

  // Trigger a timeout.
  s2_inclusion_notify_timeout(&s2_context);

  // Retry node inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  mock_calls_verify();
}

/** Test case for ensuring correct timer handling on timeout during inclusion in case a timeout
 *  occurs after any state.
 *
 *  Purpose: In order to ensure correct inclusion flow and avoid deadlock during inclusion it is
 *           important that in case messages are never received during inclusion, then the state
 *           machine must return to idle and push a corresponding event.
 *
 *  Test: This test case will verify expectations related to the timer handling.
 *        If a joining node does not respond within the timeout to a S2 inclusion frame then the
 *        state machine must return to idle and push the corresponding event upwards to application.
 *        After timeout, we expect the system to retry node inclusion.
 *        As this test case must covers timeout in ANY state, it will loop over the expectations and
 *        decrement a counter to go one step further on each loop.
 *
 */
 #define NO_OF_STATES 6
void test_kex_inclusion_timeout_all_states() {
  mock_t    * p_mock;
  uint32_t    i;
  uint8_t     frame_buffer[UNIT_TEST_BUF_SIZE];
  struct S2   s2_context;
  uint8_t     public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                                0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  uint8_t private_key_a[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, // Private key.
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  /** The loop which will set up the expectation based on the round. */
  for (i = 0; i < NO_OF_STATES; i++)
  {
    // Expect a S2 KEX Get to be sent.
    uint8_t S2_kex_get_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_kex_get_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame);

    // Node id (first step in inclusion) has been assigned.
    // Continue with secure inclusion.
    s2_inclusion_including_start(&s2_context,&s2_conn);
    if (i == 0){
      helper_func_restore_keys_expect();

      // This test will trigger a timeout to the inclusion module after which we expect to receive a
      // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
      zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[0];
      p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
      mock_call_expect(TO_STR(s2_event_handler), &p_mock);
      p_mock->expect_arg[0].pointer = p_expected_event;

      // Trigger a timeout.
      s2_inclusion_notify_timeout(&s2_context);
      continue; // if first iteration, continue here to test timeout for timeout.
    }

    // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
    // The operator (or including app automatically) can then respond with granted keys.
    zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
    p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
    p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x82;
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_report_event;

    // Expect a S2 KEX Set to be sent.
    uint8_t S2_kex_set_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x00, 0x02, 0x01, 0x82};
    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_kex_set_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_kex_set_frame);

    helper_func_kex_report_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_key_grant(&s2_context, 1, 0x82,0);

    if (i == 1)
    {
      // This test will trigger a timeout to the inclusion module after which we expect to receive a
      // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
      helper_func_restore_keys_expect();

      zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[1];
      p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
      mock_call_expect(TO_STR(s2_event_handler), &p_mock);
      p_mock->expect_arg[0].pointer = p_expected_event;

      // Trigger a timeout.
      s2_inclusion_notify_timeout(&s2_context);
      continue; // if second iteration, break here to test timeout for timeout.
    }

    // When the public key is received, we expect that the event from libs2 contains the key in order to present it for the operator.
    // Therefore we copy the key minus header frame into expected data.
    zwave_event_t  * p_expected_inc_req_event = (zwave_event_t *)m_test_mem_pool[1];
    p_expected_inc_req_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
    p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
    memcpy(p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.public_key, m_test_public_key_b, sizeof(m_test_public_key_b));
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_inc_req_event;

    // When the public key is received, we expect a call to the keystore in order to obtain our public key.
    mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
    p_mock->output_arg[0].pointer = public_key_a;

    // Expect Public KeyA to be sent.
    uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_pub_key_A_frame);

    helper_func_pub_key_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);

    // After our public key has been sent, we expect user acceptance of the inclusion, which will result in key updates and key derivation.
    mock_call_expect(TO_STR(keystore_private_key_read), &p_mock);
    p_mock->output_arg[0].pointer = private_key_a;
    mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
    p_mock->output_arg[0].pointer = public_key_a;

    s2_inclusion_challenge_response(&s2_context, 1, m_test_public_key_a);

    if (i == 2){
      helper_func_restore_keys_expect();

      // This test will trigger a timeout to the inclusion module after which we expect to receive a
      // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
      zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
      p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
      mock_call_expect(TO_STR(s2_event_handler), &p_mock);
      p_mock->expect_arg[0].pointer = p_expected_event;

      // Trigger a timeout.
      s2_inclusion_notify_timeout(&s2_context);
      continue; // if third iteration, break here to test timeout for timeout.
    }

    // Expect Echo(KEX Report) to be sent.
    uint8_t S2_echo_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x01, 0x02, 0x01, 0x82};
    mock_call_expect(TO_STR(S2_send_data), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_echo_kex_report_frame; // Ideally, this should be updated to be identically to replayed received KEX Set.
    p_mock->expect_arg[3].value   = sizeof(S2_echo_kex_report_frame);

    helper_func_echo_kex_set_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);
    if (i == 3){
      helper_func_restore_keys_expect();

      // This test will trigger a timeout to the inclusion module after which we expect to receive a
      // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
      zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
      p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
      mock_call_expect(TO_STR(s2_event_handler), &p_mock);
      p_mock->expect_arg[0].pointer = p_expected_event;

      // Trigger a timeout.
      s2_inclusion_notify_timeout(&s2_context);
      continue; // if fourth iteration, break here to test timeout for timeout.
    }

    mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
    p_mock->expect_arg[0].value   = 0x02;
    p_mock->output_arg[1].pointer = m_test_network_key_s2_class_1;

    // Expect Net Key Set to be sent.
    static uint8_t S2_net_key_set_frame[19] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_REPORT, 0x02};
    memcpy(&S2_net_key_set_frame[3], m_test_network_key_s2_class_1, sizeof(m_test_network_key_s2_class_1));
    mock_call_expect(TO_STR(S2_send_data), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_net_key_set_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_net_key_set_frame);

    helper_func_net_key_get_frame(&s2_context, 0x02);
    s2_inclusion_post_event(&s2_context);
    if (i == 4){
      helper_func_restore_keys_expect();

      // This test will trigger a timeout to the inclusion module after which we expect to receive a
      // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
      zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
      p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
      mock_call_expect(TO_STR(s2_event_handler), &p_mock);
      p_mock->expect_arg[0].pointer = p_expected_event;

      // Trigger a timeout.
      s2_inclusion_notify_timeout(&s2_context);
      continue; // if fifth iteration, break here to test timeout for timeout.
    }

    // Expect S2 Transfer End to be sent.
    static uint8_t S2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x02};
    mock_call_expect(TO_STR(S2_send_data), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_transfer_end_frame);

    helper_func_net_key_verify_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);
    if (i == 5){
      helper_func_restore_keys_expect();

      // This test will trigger a timeout to the inclusion module after which we expect to receive a
      // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
      zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
      p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
      mock_call_expect(TO_STR(s2_event_handler), &p_mock);
      p_mock->expect_arg[0].pointer = p_expected_event;

      // Trigger a timeout.
      s2_inclusion_notify_timeout(&s2_context);
      continue; // if sixth iteration, break here to test timeout for timeout.
    }
  }

  // After receiving a timeout it is expected that the inclusion state machine is back in idle and
  // that we can initiate a new inclusion which will also trigger a new set timeout call.
  static uint8_t S2_kex_get_frame_retry[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame_retry;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame_retry);

  // At final stage retry node inclusion to ensure we are in idle stage.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  mock_calls_verify();
}

/** Test case for ensuring correct handling when frames are received in wrong order during inclusion.
 *  This could either be implementation error or more critical, someone trying to find weakness in
 *  the system. Therefore the system must silently return to idle.
 *
 *  An error should still be returned upwards to the application.
 */
  void test_kex_inclusion_invalid_frame_order_all_states() {
  mock_t       * p_mock;
  uint32_t       i;
  static uint8_t frame_buffer[UNIT_TEST_BUF_SIZE];
  struct S2      s2_context;
  uint8_t        public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                                   0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                   0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                   0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  uint8_t private_key_a[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, // Private key.
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  /** The loop which will set up the expectation based on the round. */
  for (i = 0; i < NO_OF_STATES; i++)
  {
    // Expect a S2 KEX Get to be sent.
    static uint8_t S2_kex_get_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_kex_get_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame);

    // Node id (first step in inclusion) has been assigned.
    // Continue with secure inclusion.
    s2_inclusion_including_start(&s2_context,&s2_conn);
    if (i == 0){
      // The invalid frame.
      uint8_t s2_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_GET, 0x02};

      helper_func_restore_keys_expect();

      // This test will trigger a timeout to the inclusion module after which we expect to receive a
      // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
      zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[0];
      p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
      mock_call_expect(TO_STR(s2_event_handler), &p_mock);
      p_mock->expect_arg[0].pointer = p_expected_event;

      // Let's try to directly fetch a network key.
      memcpy((uint8_t *)s2_context.buf, s2_frame, sizeof(s2_frame));
      s2_context.length = sizeof(s2_frame);
      s2_context.peer.class_id = 0x00;
      s2_inclusion_post_event(&s2_context);
      //s2_context.buf = frame_buffer; // Reset buffer back.
      continue; // if first iteration, continue here to test timeout for timeout.
    }

    // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
    // The operator (or including app automatically) can then respond with granted keys.
    zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
    p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
    p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x82;
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_report_event;

    // Expect a S2 KEX Set to be sent.
    static uint8_t S2_kex_set_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x00, 0x02, 0x01, 0x82};
    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_kex_set_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_kex_set_frame);

    helper_func_kex_report_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_key_grant(&s2_context, 1, 0x82,0);

    if (i == 1)
    {
      // The invalid frame - Let's send a KEX Get the opposite direction.
      uint8_t s2_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};

      helper_func_restore_keys_expect();
      // This test will trigger a timeout to the inclusion module after which we expect to receive a
      // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
      zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[1];
      p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
      mock_call_expect(TO_STR(s2_event_handler), &p_mock);
      p_mock->expect_arg[0].pointer = p_expected_event;

      // Let's try to directly fetch a network key.
      memcpy((uint8_t *)s2_context.buf, s2_frame, sizeof(s2_frame));
      s2_context.length = sizeof(s2_frame);
      s2_inclusion_post_event(&s2_context);
      s2_context.peer.class_id = 0x00;
//      s2_context.buf = frame_buffer; // Reset buffer back.
      continue; // if first iteration, continue here to test timeout for timeout.
    }

    // When the public key is received, we expect that the event from libs2 contains the key in order to present it for the operator.
    // Therefore we copy the key minus header frame into expected data.
    zwave_event_t  * p_expected_inc_req_event = (zwave_event_t *)m_test_mem_pool[1];
    p_expected_inc_req_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
    p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
    memcpy(p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.public_key, m_test_public_key_b, sizeof(m_test_public_key_b));

    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_inc_req_event;

    // When the public key is received, we expect a call to the keystore in order to obtain our public key.
    mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
    p_mock->output_arg[0].pointer = public_key_a;

    // Expect Public KeyA to be sent.
    uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_pub_key_A_frame);

    helper_func_pub_key_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);

    // After our public key has been sent, we expect user acceptance of the inclusion, which will result in key updates and key derivation.
    mock_call_expect(TO_STR(keystore_private_key_read), &p_mock);
    p_mock->output_arg[0].pointer = private_key_a;
    mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
    p_mock->output_arg[0].pointer = public_key_a;

    s2_inclusion_challenge_response(&s2_context, 1, m_test_public_key_b);

    if (i == 2){
      uint8_t S2_pub_key_A_return[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                       0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                       0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                       0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                       0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

      helper_func_restore_keys_expect();
      // This test will trigger a timeout to the inclusion module after which we expect to receive a
      // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
      zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
      p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
      mock_call_expect(TO_STR(s2_event_handler), &p_mock);
      p_mock->expect_arg[0].pointer = p_expected_event;

      // Let's try to reply by sending back public key frame to examine behavior.
      memcpy((uint8_t *)s2_context.buf, S2_pub_key_A_return, sizeof(S2_pub_key_A_return));
      s2_context.length = sizeof(S2_pub_key_A_return);
      s2_inclusion_post_event(&s2_context);
      s2_context.buf = frame_buffer; // Reset buffer back.
      continue; // if first iteration, continue here to test timeout for timeout.
    }

    // Expect Echo(KEX Report) to be sent.
    static uint8_t S2_echo_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x01, 0x02, 0x01, 0x82};
    mock_call_expect(TO_STR(S2_send_data), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_echo_kex_report_frame; // Ideally, this should be updated to be identically to replayed received KEX Set.
    p_mock->expect_arg[3].value   = sizeof(S2_echo_kex_report_frame);

    helper_func_echo_kex_set_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);
    if (i == 3){
      // The invalid frame.
      uint8_t s2_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x02, 0x01, 0x82};

      helper_func_restore_keys_expect();
      // This test will trigger a timeout to the inclusion module after which we expect to receive a
      // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
      zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
      p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
      mock_call_expect(TO_STR(s2_event_handler), &p_mock);
      p_mock->expect_arg[0].pointer = p_expected_event;

      // Let's try to check what happens if we intercept and try to restart secure inclusion by
      // sending a KEX Report. Will we be accepted ?
      memcpy((uint8_t *)s2_context.buf, s2_frame, sizeof(s2_frame));
      s2_context.length = sizeof(s2_frame);
      s2_context.peer.class_id = 0xFF;
      s2_inclusion_post_event(&s2_context);
      //s2_context.buf = frame_buffer; // Reset buffer back.
      continue; // if first iteration, continue here to test timeout for timeout.
    }

    // Expect Net Key Set to be sent.
    mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
    p_mock->expect_arg[0].value   = 0x02;
    p_mock->output_arg[1].pointer = m_test_network_key_s2_class_1;

    static uint8_t S2_net_key_set_frame[19] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_REPORT, 0x02};
    memcpy(&S2_net_key_set_frame[3], m_test_network_key_s2_class_1, sizeof(m_test_network_key_s2_class_1));
    mock_call_expect(TO_STR(S2_send_data), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_net_key_set_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_net_key_set_frame);

    helper_func_net_key_get_frame(&s2_context, 0x02);
    s2_inclusion_post_event(&s2_context);
    if (i == 4){
      // Go to transfer end without verifying the key.
      uint8_t s2_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x01};

      helper_func_restore_keys_expect();
      // This test will trigger a timeout to the inclusion module after which we expect to receive a
      // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
      zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
      p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
      mock_call_expect(TO_STR(s2_event_handler), &p_mock);
      p_mock->expect_arg[0].pointer = p_expected_event;

      memcpy((uint8_t *)s2_context.buf, s2_frame, sizeof(s2_frame));
      s2_context.length = sizeof(s2_frame);
      s2_context.peer.class_id = 0x00;
      s2_inclusion_post_event(&s2_context);
      continue; // if fifth iteration, break here to test timeout for timeout.
    }

    // Expect S2 Transfer End to be sent.
    static uint8_t S2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x02};
    mock_call_expect(TO_STR(S2_send_data), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_transfer_end_frame);

    helper_func_net_key_verify_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);
    if (i == 5){
      // Retry the echo to see if we can get further back in the loop than officially allowed.
      uint8_t s2_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x01, 0x02, 0x01, 0x82};

      helper_func_restore_keys_expect();
      // This test will trigger a timeout to the inclusion module after which we expect to receive a
      // callback event with a S2_NODE_INCLUSION_FAILED_EVENT.
      zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
      p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
      mock_call_expect(TO_STR(s2_event_handler), &p_mock);
      p_mock->expect_arg[0].pointer = p_expected_event;

      memcpy((uint8_t *)s2_context.buf, s2_frame, sizeof(s2_frame));
      s2_context.length = sizeof(s2_frame);
      s2_inclusion_post_event(&s2_context);
      s2_context.buf = frame_buffer; // Reset buffer back.
      continue; // if fifth iteration, break here to test timeout for timeout.
    }
  }

  // After receiving a timeout it is expected that the inclusion state machine is back in idle and
  // that we can initiate a new inclusion which will also trigger a new set timeout call.
  static uint8_t S2_kex_get_frame_retry[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame_retry;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame_retry);

  // At final stage retry node inclusion to ensure we are in idle stage.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  mock_calls_verify();
}

void test_kex_inclusion_duplicate_frame_order_all_states() {
  mock_t       * p_mock;
  uint32_t       i;
  static uint8_t frame_buffer[UNIT_TEST_BUF_SIZE];
  struct S2      s2_context;
  uint8_t        public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                                   0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                   0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                   0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  uint8_t private_key_a[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, // Private key.
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

    // Expect a S2 KEX Get to be sent.
  static uint8_t S2_kex_get_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame);
  // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
  // The operator (or including app automatically) can then respond with granted keys.
  zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
  p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x82;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_report_event;

  // Expect a S2 KEX Set to be sent.
  static uint8_t S2_kex_set_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x00, 0x02, 0x01, 0x82};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_set_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_set_frame);

  // When the public key is received, we expect that the event from libs2 contains the key in order to present it for the operator.
  // Therefore we copy the key minus header frame into expected data.
  zwave_event_t  * p_expected_inc_req_event = (zwave_event_t *)m_test_mem_pool[1];
  p_expected_inc_req_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
  p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
  memcpy(p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.public_key, m_test_public_key_b, sizeof(m_test_public_key_b));

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_inc_req_event;

  // When the public key is received, we expect a call to the keystore in order to obtain our public key.
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;

  // Expect Public KeyA to be sent.
  uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_pub_key_A_frame);

  // After our public key has been sent, we expect user acceptance of the inclusion, which will result in key updates and key derivation.
  mock_call_expect(TO_STR(keystore_private_key_read), &p_mock);
  p_mock->output_arg[0].pointer = private_key_a;
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;

  // Expect Echo(KEX Report) to be sent.
  static uint8_t S2_echo_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x01, 0x02, 0x01, 0x82};
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_echo_kex_report_frame; // Ideally, this should be updated to be identically to replayed received KEX Set.
  p_mock->expect_arg[3].value   = sizeof(S2_echo_kex_report_frame);

  // Expect Net Key Set to be sent.
  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x02;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_1;

  static uint8_t S2_net_key_set_frame[19] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_REPORT, 0x02};
  memcpy(&S2_net_key_set_frame[3], m_test_network_key_s2_class_1, sizeof(m_test_network_key_s2_class_1));
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_net_key_set_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_net_key_set_frame);

  // Expect S2 Transfer End to be sent.
  static uint8_t S2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x02};
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_transfer_end_frame);

  // Expect Net Key Set to be sent.
  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x80;
  p_mock->output_arg[1].pointer = m_test_network_key_s0;

  static uint8_t S2_net_key_set_frame_s0[19] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_REPORT, 0x80};
  memcpy(&S2_net_key_set_frame_s0[3], m_test_network_key_s0, sizeof(m_test_network_key_s0));
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_net_key_set_frame_s0;
  p_mock->expect_arg[3].value   = sizeof(S2_net_key_set_frame_s0);

  // Expect S2 Transfer End to be sent.
  //static uint8_t S2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x02};
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_transfer_end_frame);

  helper_func_restore_keys_expect();
  zwave_event_t  * p_expected_complete_event = (zwave_event_t *)m_test_mem_pool[2];
  p_expected_complete_event->event_type = S2_NODE_INCLUSION_COMPLETE_EVENT;
  p_expected_complete_event->evt.s2_event.s2_data.inclusion_complete.exchanged_keys = 0x82;

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_complete_event ;

  /** The loop which will set up the expectation based on the round. */
  for (i = 0; i < 7; i++)
  {
    // Node id (first step in inclusion) has been assigned.
    // Continue with secure inclusion.
    s2_inclusion_including_start(&s2_context,&s2_conn);
    if (i == 0)
      continue;
    helper_func_kex_report_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);

    if (i == 1)
    {
      s2_inclusion_key_grant(&s2_context, 1, 0x82,0);
      continue; // if first iteration, continue here to test timeout for timeout.
    }

    helper_func_pub_key_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);

    if (i == 2){
      s2_inclusion_challenge_response(&s2_context, 1, m_test_public_key_a);
      continue;
    }

    helper_func_echo_kex_set_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);
    if (i == 3)
      continue;

    helper_func_net_key_get_frame(&s2_context, 0x02);
    s2_inclusion_post_event(&s2_context);
    if (i == 4)
      continue;

    helper_func_net_key_verify_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);

    helper_func_net_key_get_frame(&s2_context, 0x80);
    s2_inclusion_post_event(&s2_context);

    helper_func_net_key_verify_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);

    if (i == 5)
      continue;

    uint8_t s2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x01};
    s2_context.buf    = s2_transfer_end_frame;
    s2_context.length = sizeof(s2_transfer_end_frame);
    s2_context.peer.class_id = 0x00;
    s2_inclusion_post_event(&s2_context);
  }

  mock_calls_verify();
}

/** Test case for ensuring correct handling of node inclusion in case the node is not supporting
 *  mandatory schemes, or the schemes supported are not supported by the controller.
 *
 *  Purpose: In order to ensure a node can be correctly included there must be a match between the
 *           schemmes supported by both inclusion and joining node. If this cannot be established
 *           between the node, then it must bew ensured that appropriate failure is returned.
 *
 *  Test: This test case will in case the following schemes are received from joining node as
 *        supported then a KEX_FAIL_SCHEME is returned.
 */
void test_kex_inclusion_error_schemes() {
  mock_t    * p_mock;
  uint32_t    i;
  uint8_t     frame_buffer[UNIT_TEST_BUF_SIZE];
  uint8_t     s2_kex_report_frame[6];
  struct S2   s2_context;

  uint8_t          scheme_scheme_fail_test_vector[][2] = {
    {0x00, 0x00}, // Scheme 2 = false, scheme 0 = false, key request don't care. This should return FAIL_KEY_REPORT.
    {0x01, 0x80}, // Scheme 2 = false, scheme 0 = true,  key request don't care. This should return FAIL_KEY_REPORT.
  };

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(keystore_network_key_read));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  for (i = 0; i < ELEM_COUNT(scheme_scheme_fail_test_vector); i++)
  {

    /****************************
     * Mock expectation section *
     ****************************/

    // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
    helper_func_kex_get_frame_expect();

    // Expect a S2 KEX Fail as the including node received an unsupported combination of security schemes.
    uint8_t S2_kex_set_error_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_FAIL, 0x02}; // KEX_FAIL_KEX_SCHEME = 0x02
    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_kex_set_error_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_kex_set_error_frame);

    zwave_event_t  * p_expected_event = (zwave_event_t *)m_test_mem_pool[0];
    p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_event;

    /*******************
     * Testing section *
     *******************/

    // Node id (first step in inclusion) has been assigned.
    // Continue with secure inclusion.
    s2_inclusion_including_start(&s2_context,&s2_conn);

    // KEX Report frame received - With an unsupported scheme set.
    s2_kex_report_frame[0] = COMMAND_CLASS_SECURITY_2;
    s2_kex_report_frame[1] = KEX_REPORT;
    s2_kex_report_frame[2] = 0x00, // bit 0 is echo field, rest are reserved
    s2_kex_report_frame[3] = scheme_scheme_fail_test_vector[i][0]; // Supported schemes.
    s2_kex_report_frame[4] = 0x01, // Supported ECDH Profiles, bit0=1 is curve 25519 value.
    s2_kex_report_frame[5] = scheme_scheme_fail_test_vector[i][1]; // Requested keys bit.
    s2_context.buf    = s2_kex_report_frame;
    s2_context.length = sizeof(s2_kex_report_frame);
    s2_context.peer.class_id = 0xFF;
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_send_done(&s2_context, 1);
  }

  // Retry node inclusion.
  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

/** Test case for ensuring correct handling of node inclusion in case the node is not supporting
 *  curve 25519, or other curves requested are not (yet) supported by the controller.
 *
 *  Purpose: In order to ensure a node can be correctly included there must be a match between the
 *           curves supported by both inclusion and joining node. If the curve supported by joining
 *           node is not matching a curve on inclusion node a failure is returned.
 *
 *  Test: This test case will a KEX_FAIL_CURVE is returned when invalid set of schemes is requested.
 */
void test_kex_inclusion_error_curves() {
  mock_t         * p_mock;
  zwave_event_t  * p_expected_event;
  uint8_t          frame_buffer[UNIT_TEST_BUF_SIZE];
  struct S2        s2_context;

  uint8_t          S2_kex_set_error_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_FAIL, 0x03};

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(keystore_network_key_read));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  /****************************
   * Mock expectation section *
   ****************************/

  // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
  helper_func_kex_get_frame_expect();

  // Expect a S2 KEX Fail as the including node received an unsupported combination of security schemes.
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_set_error_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_set_error_frame);

  p_expected_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_event;

  /*******************
   * Testing section *
   *******************/

    // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  // KEX Report frame received - Unknown curve set.
  uint8_t s2_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT,
                                   0x00, // bit 0 is echo field, rest are reserved
                                   0x02, // Supported schemes. Scheme 1.
                                   0x02, // Supported ECDH Profiles, bit1=1 is unknown cureve value.
                                   0x01}; // Requested keys bit0=Security2 Class 0 key.
  s2_context.buf    = s2_kex_report_frame;
  s2_context.length = sizeof(s2_kex_report_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_send_done(&s2_context, 1);

  // Retry node inclusion.
  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

/** Test case for ensuring correct handling of node inclusion in case there is an invalid combination
 *  of scheme support and key request.
 *
 *  Purpose: In order to ensure a node can be correctly included there must be a match between the
 *           schemes supported by both inclusion and joining node and the request for keys to exchange.
 *
 *  Test: This test case will a KEX_FAIL_KEY is returned when invalid set of schemes/keys is requested.
 */
void test_kex_inclusion_error_keys() {
  uint32_t         i;
  mock_t         * p_mock;
  zwave_event_t  * p_expected_event;
  uint8_t          frame_buffer[UNIT_TEST_BUF_SIZE];
  uint8_t          s2_kex_report_frame[6];
  struct S2        s2_context;

  uint8_t          S2_kex_error_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_FAIL, 0x01}; // KEX_FAIL_KEX_KEY = 0x01
  uint8_t          scheme_key_request_fail_test_vector[] = {
    0x00,   // key request S2 = no, key request S0 = no.  This should return FAIL_KEY_REPORT.
  };

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(keystore_network_key_read));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  for (i = 0; i < ELEM_COUNT(scheme_key_request_fail_test_vector); i++)
  {
    /****************************
     * Mock expectation section *
     ****************************/

    // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
    helper_func_kex_get_frame_expect();

    // Expect a S2 KEX Fail as the including node received an unsupported combination of security schemes.
    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_kex_error_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_kex_error_frame);

    p_expected_event = (zwave_event_t *)m_test_mem_pool[0];
    p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_event;

    /*******************
     * Testing section *
     *******************/

      // Node id (first step in inclusion) has been assigned.
    // Continue with secure inclusion.
    s2_inclusion_including_start(&s2_context,&s2_conn);

    // KEX Report frame received - Invalid mix of schemes and keys.
    s2_kex_report_frame[0] = COMMAND_CLASS_SECURITY_2;
    s2_kex_report_frame[1] = KEX_REPORT;
    s2_kex_report_frame[2] = 0x00, // bit 0 is echo field, rest are reserved
    s2_kex_report_frame[3] = 0x02; // Supported schemes.
    s2_kex_report_frame[4] = 0x01, // Supported ECDH Profiles, bit0=1 is curve 25519 value.
    s2_kex_report_frame[5] = scheme_key_request_fail_test_vector[i]; // Requested keys bit.
    s2_context.buf    = s2_kex_report_frame;
    s2_context.length = sizeof(s2_kex_report_frame);
    s2_context.peer.class_id = 0xFF;
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_send_done(&s2_context, 1);
  }

  // Retry node inclusion.
  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

/** Test case for ensuring correct handling of node inclusion in all cases when there is a valid combination
 *  of scheme support and key request.
 *
 *  Purpose: In order to ensure a node can be correctly included there must be a match between the
 *           schemes supported by both inclusion and joining node and the request for keys to exchange.
 *
 *  Test: This test case will ensure a KEX Set is transmitted when a valid set of schemes/keys is requested.
 */
void test_kex_inclusion_valid_keys() {
  uint32_t         i;
  mock_t         * p_mock;
  uint8_t          frame_buffer[UNIT_TEST_BUF_SIZE];
  uint8_t          s2_kex_report_frame[6];
  struct S2        s2_context;

  uint8_t S2_kex_set_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x00, 0x02, 0x01, 0x82}; // Scheme 0 and 2, curve25519, S2, key 2, and S0 network key.

  // Double array, each row is a test, first two bytes in column is received frame, second two bytes are the expected output.
  uint8_t          scheme_key_request_pass_test_vector[][4] = {
    {0x02, 0x01, 0x02, 0x01},   // Scheme 1 = true, key request S2 = yes, key request S0 = no.  Valid request.
    {0x02, 0x02, 0x02, 0x02},   // Scheme 1 = true, key request S2 = yes, key request S0 = no.  Valid request.
    {0x02, 0x03, 0x02, 0x03},   // Scheme 1 = true, key request S2 = yes, key request S0 = no.  Valid request.
    {0x02, 0x04, 0x02, 0x04},   // Scheme 1 = true, key request S2 = yes, key request S0 = no.  Valid request.
    {0x02, 0x05, 0x02, 0x05},   // Scheme 1 = true, key request S2 = yes, key request S0 = no.  Valid request.
    {0x02, 0x06, 0x02, 0x06},   // Scheme 1 = true, key request S2 = yes, key request S0 = no.  Valid request.
    {0x02, 0x07, 0x02, 0x07},   // Scheme 1 = true, key request S2 = yes, key request S0 = no.  Valid request.
    {0x02, 0x80, 0x02, 0x80},   // Scheme 1 = true, key request S2 = no,  key request S0 = yes. Valid request.
    {0x02, 0x81, 0x02, 0x81},   // Scheme 1 = true, key request S2 = yes, key request S0 = yes. Valid request.
    {0x02, 0x82, 0x02, 0x82},   // Scheme 1 = true, key request S2 = yes, key request S0 = yes. Valid request.
    {0x02, 0x83, 0x02, 0x83},   // Scheme 1 = true, key request S2 = yes, key request S0 = yes. Valid request.
    {0x02, 0x84, 0x02, 0x84},   // Scheme 1 = true, key request S2 = yes, key request S0 = yes. Valid request.
    {0x02, 0x85, 0x02, 0x85},   // Scheme 1 = true, key request S2 = yes, key request S0 = yes. Valid request.
    {0x02, 0x86, 0x02, 0x86},   // Scheme 1 = true, key request S2 = yes, key request S0 = yes. Valid request.
    {0x02, 0x87, 0x02, 0x87},   // Scheme 1 = true, key request S2 = yes, key request S0 = yes. Valid request.
  };

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(s2_event_handler));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(keystore_public_key_read));
  mock_call_use_as_stub(TO_STR(keystore_network_key_read));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  for (i = 0; i < ELEM_COUNT(scheme_key_request_pass_test_vector); i++)
  {
    /****************************
     * Mock expectation section *
     ****************************/

    // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
    helper_func_kex_get_frame_expect();

    S2_kex_set_frame[3] = scheme_key_request_pass_test_vector[i][2];
    S2_kex_set_frame[5] = scheme_key_request_pass_test_vector[i][3];
    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_kex_set_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_kex_set_frame);

    /*******************
     * Testing section *
     *******************/

    // Node id (first step in inclusion) has been assigned.
    // Continue with secure inclusion.
    s2_inclusion_including_start(&s2_context,&s2_conn);

    // KEX Report frame received - Valid mix of schemes and keys.
    s2_kex_report_frame[0] = COMMAND_CLASS_SECURITY_2;
    s2_kex_report_frame[1] = KEX_REPORT;
    s2_kex_report_frame[2] = 0x00, // bit 0 is echo field, rest are reserved
    s2_kex_report_frame[3] = scheme_key_request_pass_test_vector[i][0]; // Supported schemes.
    s2_kex_report_frame[4] = 0x01, // Supported ECDH Profiles, bit0=1 is curve 25519 value.
    s2_kex_report_frame[5] = scheme_key_request_pass_test_vector[i][1]; // Requested keys bit.
    s2_context.buf    = s2_kex_report_frame;
    s2_context.length = sizeof(s2_kex_report_frame);
    s2_context.peer.class_id = 0xFF;
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_key_grant(&s2_context, 1, scheme_key_request_pass_test_vector[i][3],0); // Granted keys.

    // Inject a timeout to ensure state goes back to idle so we can test next combination.
    s2_inclusion_notify_timeout(&s2_context);
  }
  // Retry node inclusion.
  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

/** Test case for ensuring correct handling of node inclusion in case the joining node is
 *  requesting more keys than what was granted in KEX Set
 *
 *  Purpose: In order to ensure a node can be correctly included there must be a match between the
 *           Key granted in KEX Set and the keys requested during key exchange.
 *
 *  Test: This test case will ensure a KEX_FAIL_KEY_GET is returned when non granted key is requested.
 */
void test_kex_inclusion_error_key_get_request() {
  uint32_t         i;
  mock_t         * p_mock;
  zwave_event_t  * p_expected_event;
  uint8_t          frame_buffer[UNIT_TEST_BUF_SIZE];
  uint8_t          s2_kex_report_frame[6] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x00, 0x00, 0x01, 0x00};
  uint8_t          s2_echo_kex_set_frame[6] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x01, 0x00, 0x01, 0x00};
  uint8_t          s2_net_key_get_frame[6] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_GET, 0x00};
  uint8_t          public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  uint8_t          private_key_a[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, // Private key.
                                      0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                                      0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                                      0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  struct S2        s2_context;

  uint8_t          S2_kex_error_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_FAIL, 0x08}; // KEX_FAIL_KEY_GET = 0x08

  // Double array, each row is a test, first two bytes in column is reported/granted keys, third byte is the requested key.
  uint8_t          key_get_request_fail_test_vector[][3] = {
    {0x02, 0x01, 0x02},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class 1.
    {0x02, 0x01, 0x04},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 2.
    {0x02, 0x01, 0x80},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S0 Network key.
    {0x02, 0x02, 0x01},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class 0.
    {0x02, 0x02, 0x04},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class 2.
    {0x02, 0x02, 0x08},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class x.
    {0x02, 0x02, 0x80},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S0 Network key.
    {0x02, 0x03, 0x04},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class 2.
    {0x02, 0x03, 0x08},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class x.
    {0x02, 0x03, 0x80},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S0 Network key.
    {0x02, 0x04, 0x01},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class 0.
    {0x02, 0x04, 0x02},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class 1.
    {0x02, 0x04, 0x08},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class x.
    {0x02, 0x04, 0x80},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S0 Network key.
    {0x02, 0x05, 0x02},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class 1.
    {0x02, 0x05, 0x08},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class x.
    {0x02, 0x05, 0x80},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S0 Network key.
    {0x02, 0x06, 0x01},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class 1.
    {0x02, 0x06, 0x08},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class x.
    {0x02, 0x06, 0x80},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S0 Network key.
    {0x02, 0x07, 0x08},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S2 class x.
    {0x02, 0x07, 0x80},   // Scheme 2 = true,  scheme 0 = false, key request S2 = yes - Request S0 Network key.
    {0x02, 0x81, 0x02},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 1.
    {0x02, 0x81, 0x04},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 2.
    {0x02, 0x81, 0x08},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request unknown/unsupported key.
    {0x02, 0x82, 0x01},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 0.
    {0x02, 0x82, 0x04},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 2.
    {0x02, 0x82, 0x08},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request unknown/unsupported key.
    {0x02, 0x84, 0x01},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 0.
    {0x02, 0x84, 0x02},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 1.
    {0x02, 0x84, 0x08},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request unknown/unsupported key.
    {0x02, 0x85, 0x02},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 1.
    {0x02, 0x85, 0x08},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request unknown/unsupported key.
    {0x02, 0x86, 0x01},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 0.
    {0x02, 0x86, 0x08},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request unknown/unsupported key.
    {0x02, 0x81, 0x02},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 1.
    {0x02, 0x81, 0x04},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 2.
    {0x02, 0x82, 0x01},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 0.
    {0x02, 0x82, 0x04},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 2.
    {0x02, 0x83, 0x04},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request S2 class 2.
    {0x02, 0x83, 0x08},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request unknown/unsupported key.
    {0x02, 0x87, 0x08},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request unknown/unsupported key.



    {0x02, 0x83, 0x10},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request unknown/unsupported key.
    {0x02, 0x83, 0x20},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request unknown/unsupported key.
    {0x02, 0x83, 0x40},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request unknown/unsupported key.
  };

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(keystore_network_key_read));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  for (i = 0; i < ELEM_COUNT(key_get_request_fail_test_vector); i++)
  {
    /****************************
     * Mock expectation section *
     ****************************/

    // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
    helper_func_kex_get_frame_expect();

    // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
    // The operator (or including app automatically) can then respond with granted keys.
    zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
    p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
    p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = key_get_request_fail_test_vector[i][1];
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_report_event;

    helper_func_kex_set_frame_expect(key_get_request_fail_test_vector[i][0], 0x01,
                                     key_get_request_fail_test_vector[i][1]);

    // When the public key is received, we expect a call to the keystore in order to obtain our public key.
    mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
    p_mock->output_arg[0].pointer = public_key_a;

    zwave_event_t  * p_expected_inc_req_event = (zwave_event_t *)m_test_mem_pool[1];
    p_expected_inc_req_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
//    p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.security_keys = key_get_request_fail_test_vector[i][1];
    p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
    memcpy(p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.public_key, m_test_public_key_b, sizeof(m_test_public_key_b));
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_inc_req_event;

    // Expect Public KeyA to be sent.
    uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};
    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
    p_mock->expect_arg[3].value    = sizeof(S2_pub_key_A_frame);

    // After our public key has been sent, we expect user acceptance of the inclusion, which will result in key updates and key derivation.
    mock_call_expect(TO_STR(keystore_private_key_read), &p_mock);
    p_mock->output_arg[0].pointer = private_key_a;
    mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
    p_mock->output_arg[0].pointer = public_key_a;

    helper_func_echo_kex_report_frame_expect(key_get_request_fail_test_vector[i][0], 0x01,
                                             key_get_request_fail_test_vector[i][1]);

    // Expect a S2 KEX Fail as the including node received an unsupported combination of security schemes.
    mock_call_expect(TO_STR(S2_send_data), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_kex_error_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_kex_error_frame);

    p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
    p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_event;

    /*******************
     * Testing section *
     *******************/

      // Node id (first step in inclusion) has been assigned.
    // Continue with secure inclusion.
    s2_inclusion_including_start(&s2_context,&s2_conn);

    // KEX Report frame received - Valid mix of schemes and keys.
    s2_kex_report_frame[3] = key_get_request_fail_test_vector[i][0]; // Supported schemes.
    s2_kex_report_frame[5] = key_get_request_fail_test_vector[i][1]; // Requested keys bit.
    s2_context.buf    = s2_kex_report_frame;
    s2_context.length = sizeof(s2_kex_report_frame);
    s2_context.peer.class_id = 0xFF;
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_key_grant(&s2_context, 1, key_get_request_fail_test_vector[i][1],0); // Granted keys.

    s2_context.buf = frame_buffer;
    helper_func_pub_key_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_challenge_response(&s2_context, 1, m_test_public_key_b);

    // KEX Echo Set frame received - Valid mix of schemes and keys.
    s2_echo_kex_set_frame[3] = key_get_request_fail_test_vector[i][0]; // Supported schemes.
    s2_echo_kex_set_frame[5] = key_get_request_fail_test_vector[i][1]; // Requested keys bit.
    s2_context.buf    = s2_echo_kex_set_frame;
    s2_context.length = sizeof(s2_echo_kex_set_frame);
    s2_context.peer.class_id = 0x00;
    s2_inclusion_post_event(&s2_context);

    // Network key get frame received - Invalid key requested.
    s2_net_key_get_frame[2] = key_get_request_fail_test_vector[i][2]; // Requested key.
    s2_context.buf    = s2_net_key_get_frame;
    s2_context.length = sizeof(s2_net_key_get_frame);
    s2_context.peer.class_id = 0x00;
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_send_done(&s2_context, 1);
  }

  // Retry node inclusion.
  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

/** Test case for ensuring correct handling of node inclusion in case the user rejects the node
 *  from inclusion after receiving KEX Report containing requested keys.
 *
 *  Test: This test case will ensure a KEX_FAIL_USER_CANCEL is returned when user rejects inclusion.
 */
void test_kex_inclusion_key_grant_user_reject() {
  mock_t         * p_mock;
  zwave_event_t  * p_expected_event;
  uint8_t          frame_buffer[UNIT_TEST_BUF_SIZE];
  uint8_t          s2_kex_report_frame[6] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x00, 0x02, 0x01, 0x83};

  struct S2        s2_context;
  s2_connection_t   s2_conn;
  uint8_t          S2_kex_error_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_FAIL, 0x06}; // KEX_FAIL_CANCEL = 0x06

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(keystore_network_key_read));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  /****************************
   * Mock expectation section *
   ****************************/

  // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
  helper_func_kex_get_frame_expect();

  // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
  // The operator (or including app automatically) can then respond with granted keys.
  zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
  p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x83;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_report_event;

  // Expect a S2 KEX Fail as the user cancelled inclusion.
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_error_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_error_frame);

  p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
  p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_event;

  /*******************
   * Testing section *
   *******************/

    // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  // KEX Report frame received - Valid mix of schemes and keys.
  memcpy((uint8_t *)s2_context.buf, s2_kex_report_frame, sizeof(s2_kex_report_frame));
  s2_context.length = sizeof(s2_kex_report_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_key_grant(&s2_context, 0, 0x00,0); // Granted keys.

//  helper_func_pub_key_frame(&s2_context);
//  s2_inclusion_post_event(&s2_context);

//  s2_inclusion_challenge_response(&s2_context, 0, 0xAABB);

  s2_inclusion_send_done(&s2_context, 1);

  // Retry node inclusion.
  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

/** Test case for ensuring correct handling of node inclusion in case the user rejects the node
 *  from inclusion during DSK input.
 *
 *  Test: This test case will ensure a KEX_FAIL_USER_CANCEL is returned when user rejects inclusion.
 */
void test_kex_inclusion_user_reject() {
  mock_t         * p_mock;
  zwave_event_t  * p_expected_event;
  uint8_t          frame_buffer[UNIT_TEST_BUF_SIZE];
  uint8_t          s2_kex_report_frame[6] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x00, 0x02, 0x01, 0x81};
  //uint8_t          s2_echo_kex_set_frame[6] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x01, 0x02, 0x01, 0x81};
  //uint8_t          s2_net_key_get_frame[6] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_GET, 0x01};
  uint8_t          public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};
  struct S2        s2_context;
  s2_connection_t   s2_conn;
  uint8_t          S2_kex_error_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_FAIL, 0x06}; // KEX_FAIL_CANCEL = 0x06

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(keystore_network_key_read));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  /****************************
   * Mock expectation section *
   ****************************/

  // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
  helper_func_kex_get_frame_expect();

  // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
  // The operator (or including app automatically) can then respond with granted keys.
  zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
  p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x81;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_report_event;

  helper_func_kex_set_frame_expect(0x02, 0x01, 0x81);

  // When the public key is received, we expect a call to the keystore in order to obtain our public key.
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;

  zwave_event_t  * p_expected_inc_req_event = (zwave_event_t *)m_test_mem_pool[1];
  p_expected_inc_req_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
  p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
  memcpy(p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.public_key, m_test_public_key_b, sizeof(m_test_public_key_b));
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_inc_req_event;

  // Expect Public KeyA to be sent.
  uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_pub_key_A_frame);

  // Expect a S2 KEX Fail as the including node received an unsupported combination of security schemes.
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_error_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_error_frame);

  p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
  p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_event;

  /*******************
   * Testing section *
   *******************/

    // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  // KEX Report frame received - Valid mix of schemes and keys.
  memcpy((uint8_t *)s2_context.buf, s2_kex_report_frame, sizeof(s2_kex_report_frame));
  s2_context.length = sizeof(s2_kex_report_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_key_grant(&s2_context, 1, 0x81,0); // Granted keys.

  helper_func_pub_key_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_challenge_response(&s2_context, 0, m_test_public_key_b);

  s2_inclusion_send_done(&s2_context, 1);

  // Retry node inclusion.
  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

/** Test case for ensuring correct handling of node inclusion in case an echo kex set is received.
 *  As user has not accepted the inclusion, then the Temporary Shared Secret has not been
 *  established, thus receiving such a frame is an error.
 */
void test_kex_inclusion_pending_user_accept_echo_kex_set_received() {
  mock_t         * p_mock;
  zwave_event_t  * p_expected_event;
  uint8_t          frame_buffer[UNIT_TEST_BUF_SIZE];
  uint8_t          s2_kex_report_frame[6] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x00, 0x02, 0x01, 0x81};
  uint8_t          s2_echo_kex_set_frame[6] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x01, 0x02, 0x01, 0x81};
  uint8_t          public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};
  struct S2        s2_context;

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(keystore_network_key_read));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  /****************************
   * Mock expectation section *
   ****************************/

  // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
  helper_func_kex_get_frame_expect();

  // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
  // The operator (or including app automatically) can then respond with granted keys.
  zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
  p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x81;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_report_event;

  helper_func_kex_set_frame_expect(0x02, 0x01, 0x81);

  // When the public key is received, we expect a call to the keystore in order to obtain our public key.
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;

  zwave_event_t  * p_expected_inc_req_event = (zwave_event_t *)m_test_mem_pool[1];
  p_expected_inc_req_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
  p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
  memcpy(p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.public_key, m_test_public_key_b, sizeof(m_test_public_key_b));
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_inc_req_event;

  // Expect Public KeyA to be sent.
  uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_pub_key_A_frame);

  // Expect an inclusion error to be pushed upwards.
  p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
  p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_event;

  /*******************
   * Testing section *
   *******************/

    // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  // KEX Report frame received - Valid mix of schemes and keys.
  memcpy((uint8_t *)s2_context.buf, s2_kex_report_frame, sizeof(s2_kex_report_frame));
  s2_context.length = sizeof(s2_kex_report_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_key_grant(&s2_context, 1, 0x81,0); // Granted keys.

  helper_func_pub_key_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);

  // No challenge response received, anyway we receive Key get frame, which is invalid.
  //  s2_inclusion_challenge_response(&s2_context, 0, 0xAABB);

  // KEX Echo Set frame received - Valid mix of schemes and keys.
  memcpy((uint8_t *)s2_context.buf, s2_echo_kex_set_frame, sizeof(s2_echo_kex_set_frame));
  s2_context.length = sizeof(s2_echo_kex_set_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);

  // Retry node inclusion.
  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

/** Test case for ensuring correct handling of node inclusion in case the joining node is
 *  requesting more than a single key in Network Key Get request.
 *
 *  Test: This test case will ensure a KEX_FAIL_KEY_GET is returned when more than one key is requested.
 */
void test_kex_inclusion_error_multi_key_get_request() {
  uint32_t         i;
  mock_t         * p_mock;
  zwave_event_t  * p_expected_event;
  uint8_t          frame_buffer[UNIT_TEST_BUF_SIZE];
  uint8_t          s2_kex_report_frame[6]   = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x00, 0x00, 0x01, 0x00};
  uint8_t          s2_echo_kex_set_frame[6] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x01, 0x00, 0x01, 0x00};
  uint8_t          s2_net_key_get_frame[6]  = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_GET, 0x00};
  uint8_t          public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  uint8_t          private_key_a[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, // Private key.
                                      0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                                      0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                                      0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  struct S2        s2_context;

  uint8_t          S2_kex_error_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_FAIL, 0x08}; // KEX_FAIL_KEY_GET = 0x08

  // Double array, each row is a test, first two bytes in column is reported/granted keys, third byte is the requested key.
  uint8_t          key_get_request_fail_test_vector[][3] = {
    {0x02, 0x83, 0x83},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request all keys.
    {0x02, 0x83, 0x03},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - Request all S2 keys.
    {0x02, 0x83, 0x81},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - two keys.
    {0x02, 0x83, 0x82},   // Scheme 2 = true,  scheme 0 = true,  key request S2 = yes - two keys.
  };

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(keystore_network_key_read));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));
  //mock_call_use_as_stub(TO_STR(s2_event_handler));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  for (i = 0; i < ELEM_COUNT(key_get_request_fail_test_vector); i++)
  {
    /****************************
     * Mock expectation section *
     ****************************/

    // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
    helper_func_kex_get_frame_expect();

    // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
    // The operator (or including app automatically) can then respond with granted keys.
    zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
    p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
    p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = key_get_request_fail_test_vector[i][1];
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_report_event;

    helper_func_kex_set_frame_expect(key_get_request_fail_test_vector[i][0], 0x01,
                                     key_get_request_fail_test_vector[i][1]);

    // When the public key is received, we expect a call to the keystore in order to obtain our public key.
    mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
    p_mock->output_arg[0].pointer = public_key_a;

    zwave_event_t  * p_expected_inc_req_event = (zwave_event_t *)m_test_mem_pool[1];
    p_expected_inc_req_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
//    p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.security_keys = key_get_request_fail_test_vector[i][1];
    p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
    memcpy(p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.public_key, m_test_public_key_b, sizeof(m_test_public_key_b));
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_inc_req_event;

    // Expect Public KeyA to be sent.
    uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};
    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
    p_mock->expect_arg[3].value    = sizeof(S2_pub_key_A_frame);

    // After our public key has been sent, we expect user acceptance of the inclusion, which will result in key updates and key derivation.
    mock_call_expect(TO_STR(keystore_private_key_read), &p_mock);
    p_mock->output_arg[0].pointer = private_key_a;
    mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
    p_mock->output_arg[0].pointer = public_key_a;

    helper_func_echo_kex_report_frame_expect(key_get_request_fail_test_vector[i][0], 0x01,
                                             key_get_request_fail_test_vector[i][1]);

    // Expect a S2 KEX Fail as the including node received an unsupported combination of security schemes.
    mock_call_expect(TO_STR(S2_send_data), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_kex_error_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_kex_error_frame);

    p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
    p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_event;

    /*******************
     * Testing section *
     *******************/

      // Node id (first step in inclusion) has been assigned.
    // Continue with secure inclusion.
    s2_inclusion_including_start(&s2_context,&s2_conn);

    // KEX Report frame received - Valid mix of schemes and keys.
    s2_kex_report_frame[3] = key_get_request_fail_test_vector[i][0]; // Supported schemes.
    s2_kex_report_frame[5] = key_get_request_fail_test_vector[i][1]; // Requested keys bit.
    s2_context.buf    = s2_kex_report_frame;
    s2_context.length = sizeof(s2_kex_report_frame);
    s2_context.peer.class_id = 0xFF;
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_key_grant(&s2_context, 1, key_get_request_fail_test_vector[i][1],0); // Granted keys.

    s2_context.buf = frame_buffer;
    helper_func_pub_key_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_challenge_response(&s2_context, 1, m_test_public_key_b);

    // KEX Echo Set frame received - Valid mix of schemes and keys.
    s2_echo_kex_set_frame[3] = key_get_request_fail_test_vector[i][0]; // Supported schemes.
    s2_echo_kex_set_frame[5] = key_get_request_fail_test_vector[i][1]; // Requested keys bit.
    s2_context.buf    = s2_echo_kex_set_frame;
    s2_context.length = sizeof(s2_echo_kex_set_frame);
    s2_context.peer.class_id = 0x00;
    s2_inclusion_post_event(&s2_context);

    // Network key get frame received - Invalid key requested.
    s2_net_key_get_frame[2] = key_get_request_fail_test_vector[i][2]; // Requested key.
    s2_context.buf    = s2_net_key_get_frame;
    s2_context.length = sizeof(s2_net_key_get_frame);
    s2_context.peer.class_id = 0x00;
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_send_done(&s2_context, 1);
  }

  // Retry node inclusion.
  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

/** Test case for ensuring correct handling of node inclusion in case the joining node is
 *  transmitting its public key with including node flag set, TO#7554.
 *
 *  Purpose: In order to ensure correct key exchange it is important that the nodes agrees on who
 *  is including/joining node.
 *
 *  Test: This test case will ensure that the state machine returns to idle as the wrong including
 *  bit is identical to a unexpected frame at this stages in the state machine.
 */
void test_kex_inclusion_error_public_key_including_bit() {
  mock_t         * p_mock;
  zwave_event_t  * p_expected_event;
  uint8_t          frame_buffer[UNIT_TEST_BUF_SIZE];
  uint8_t          s2_kex_report_frame[6] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x00, 0x02, 0x01, 0x02};
  struct S2        s2_context;

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  /****************************
   * Mock expectation section *
   ****************************/

  // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
  helper_func_kex_get_frame_expect();

  // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
  // The operator (or including app automatically) can then respond with granted keys.
  zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
  p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x02;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_report_event;

  helper_func_kex_set_frame_expect(0x02, 0x01, 0x02);

  // When including fails we expect at call to keystore in order to restore keys.
  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x01;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_0;

  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x02;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_1;

  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x04;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_2;

  p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
  p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_event;

  /*******************
   * Testing section *
   *******************/

  // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  // KEX Report frame received - Valid mix of schemes and keys.
  memcpy((uint8_t *)s2_context.buf, s2_kex_report_frame, sizeof(s2_kex_report_frame));
  s2_context.length = sizeof(s2_kex_report_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_key_grant(&s2_context, 1, 0x02, 0); // Granted keys.

  frame_buffer[0] = COMMAND_CLASS_SECURITY_2;
  frame_buffer[1] = PUBLIC_KEY_REPORT;
  frame_buffer[2] = 0x01; // Including bit set (error)
  memcpy(&frame_buffer[3], m_test_public_key_b, sizeof(m_test_public_key_b));
  s2_context.buf = frame_buffer;
  s2_context.peer.class_id = 0xFF;
  s2_context.length = 3  + sizeof(m_test_public_key_b);
  s2_inclusion_post_event(&s2_context);

  // Retry node inclusion to ensure we are in idle state.
  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

/** Test case for ensuring correct handling of node inclusion in case the joining node is
 *  transmitting frames with reserved bits set (which should be ignored).
 */
void test_kex_inclusion_public_key_reserved_bits() {
  mock_t       * p_mock;
  static uint8_t frame_buffer[UNIT_TEST_BUF_SIZE];
  struct S2      s2_context;
  uint8_t        public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                                   0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                   0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                   0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  uint8_t private_key_a[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, // Private key.
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

    // Expect a S2 KEX Get to be sent.
  static uint8_t S2_kex_get_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame);
  // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
  // The operator (or including app automatically) can then respond with granted keys.
  zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
  p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x82;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_report_event;

  // Expect a S2 KEX Set to be sent.
  static uint8_t S2_kex_set_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x00, 0x02, 0x01, 0x82};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_set_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_set_frame);

  // When the public key is received, we expect that the event from libs2 contains the key in order to present it for the operator.
  // Therefore we copy the key minus header frame into expected data.
  zwave_event_t  * p_expected_inc_req_event = (zwave_event_t *)m_test_mem_pool[1];
  p_expected_inc_req_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
  p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
  memcpy(p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.public_key, m_test_public_key_b, sizeof(m_test_public_key_b));

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_inc_req_event;

  // When the public key is received, we expect a call to the keystore in order to obtain our public key.
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;

  // Expect Public KeyA to be sent.
  uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_pub_key_A_frame);

  // After our public key has been sent, we expect user acceptance of the inclusion, which will result in key updates and key derivation.
  mock_call_expect(TO_STR(keystore_private_key_read), &p_mock);
  p_mock->output_arg[0].pointer = private_key_a;
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;

  // Expect Echo(KEX Report) to be sent.
  static uint8_t S2_echo_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x01, 0x02, 0x01, 0x82};
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_echo_kex_report_frame; // Ideally, this should be updated to be identically to replayed received KEX Set.
  p_mock->expect_arg[3].value   = sizeof(S2_echo_kex_report_frame);

  // Expect Net Key Set to be sent.
  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x02;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_1;

  static uint8_t S2_net_key_set_frame[19] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_REPORT, 0x02};
  memcpy(&S2_net_key_set_frame[3], m_test_network_key_s2_class_1, sizeof(m_test_network_key_s2_class_1));
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_net_key_set_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_net_key_set_frame);

  // Expect S2 Transfer End to be sent.
  static uint8_t S2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x02};
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_transfer_end_frame);

  // Expect Net Key Set to be sent.
  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x80;
  p_mock->output_arg[1].pointer = m_test_network_key_s0;

  static uint8_t S2_net_key_set_frame_s0[19] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_REPORT, 0x80};
  memcpy(&S2_net_key_set_frame_s0[3], m_test_network_key_s0, sizeof(m_test_network_key_s0));
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_net_key_set_frame_s0;
  p_mock->expect_arg[3].value   = sizeof(S2_net_key_set_frame_s0);

  // Expect S2 Transfer End to be sent.
  //static uint8_t S2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x02};
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_transfer_end_frame);

  helper_func_restore_keys_expect();
  zwave_event_t  * p_expected_complete_event = (zwave_event_t *)m_test_mem_pool[2];
  p_expected_complete_event->event_type = S2_NODE_INCLUSION_COMPLETE_EVENT;
  p_expected_complete_event->evt.s2_event.s2_data.inclusion_complete.exchanged_keys = 0x82;

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_complete_event ;

  // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  helper_func_kex_report_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_key_grant(&s2_context, 1, 0x82,0);

  frame_buffer[0] = COMMAND_CLASS_SECURITY_2;
  frame_buffer[1] = PUBLIC_KEY_REPORT;
  frame_buffer[2] = 0xA2; // Setting a couple of reserved bits which should be ignored on remote side.
  memcpy(&frame_buffer[3], m_test_public_key_b, sizeof(m_test_public_key_b));
  s2_context.buf = frame_buffer;
  s2_context.length = 3  + sizeof(m_test_public_key_b);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_challenge_response(&s2_context, 1, m_test_public_key_a);

  helper_func_echo_kex_set_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);

  helper_func_net_key_get_frame(&s2_context, 0x02);
  s2_inclusion_post_event(&s2_context);

  helper_func_net_key_verify_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);

  helper_func_net_key_get_frame(&s2_context, 0x80);
  s2_inclusion_post_event(&s2_context);

  helper_func_net_key_verify_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);

  uint8_t s2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x01};
  s2_context.buf    = s2_transfer_end_frame;
  s2_context.length = sizeof(s2_transfer_end_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);

  mock_calls_verify();
}

/** Test case for ensuring correct handling of node inclusion in case the joining node is
 *  not echo'ing KEX Set with identical fields.
 *
 *  Purpose: In order to ensure a node can be correctly included the KEX Set and the echo KEX Set
 *           must be identical except for the echo flag.
 *
 *  Test: This test case will ensure a KEX_FAIL_AUTH is returned when the echo KEX Set does not
 *        match the KEX Set frame.
 */
void test_kex_inclusion_error_echo_kex_set() {
  uint32_t         i;
  mock_t         * p_mock;
  zwave_event_t  * p_expected_event;
  uint8_t          frame_buffer[UNIT_TEST_BUF_SIZE];
  uint8_t          s2_kex_report_frame[6] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x00, 0x00, 0x01, 0x00};
  uint8_t          s2_echo_kex_set_frame[6] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x01, 0x00, 0x01, 0x00};
  uint8_t          public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  uint8_t          private_key_a[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, // Private key.
                                      0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                                      0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                                      0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  struct S2        s2_context;

  uint8_t          S2_kex_error_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_FAIL, 0x07}; // KEX_FAIL_AUTH = 0x07

  // Double array, each row is a test, first two bytes in column is reported/granted keys, third and fourth bytes are hte bytes in echo KEX Set.
  uint8_t          key_echo_fail_test_vector[][4] = {
    {0x02, 0x01, 0x02, 0x02},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x01, 0x02, 0x03},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x01, 0x02, 0x04},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x01, 0x02, 0x80},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x02, 0x02, 0x01},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x02, 0x02, 0x04},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x02, 0x02, 0x03},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x02, 0x02, 0x80},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.

    {0x02, 0x01, 0x03, 0x01},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x02, 0x03, 0x02},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x03, 0x03, 0x03},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x03, 0x72, 0x03},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x03, 0xF2, 0x03},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x03, 0xFA, 0x03},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.

    {0x02, 0x81, 0x02, 0x82},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x81, 0x02, 0x83},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x81, 0x02, 0x84},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x81, 0x02, 0x80},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x82, 0x02, 0x81},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x82, 0x02, 0x83},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x82, 0x02, 0x84},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x82, 0x02, 0x80},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x83, 0x02, 0x81},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x83, 0x02, 0x82},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x83, 0x02, 0x84},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x83, 0x02, 0x80},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x83, 0x02, 0x07},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x83, 0x02, 0x78},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x83, 0x02, 0xFF},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x83, 0x02, 0x00},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.

    {0x02, 0x83, 0x7C, 0x83},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x83, 0xF3, 0x83},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
    {0x02, 0x83, 0xFC, 0x83},   // Scheme 1 = true, key request S2 = yes - Wrong echo frame.
  };

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

  for (i = 0; i < ELEM_COUNT(key_echo_fail_test_vector); i++)
  {
    /****************************
     * Mock expectation section *
     ****************************/

    // When initiating the secure inclusion of node B, then we expect a S2 KEX Get to be sent.
    helper_func_kex_get_frame_expect();

    // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
    // The operator (or including app automatically) can then respond with granted keys.
    zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
    p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
    p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = key_echo_fail_test_vector[i][1];
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_report_event;

    helper_func_kex_set_frame_expect(key_echo_fail_test_vector[i][0], 0x01,
                                     key_echo_fail_test_vector[i][1]);

    // When the public key is received, we expect a call to the keystore in order to obtain our public key.
    mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
    p_mock->output_arg[0].pointer = public_key_a;

    zwave_event_t  * p_expected_inc_req_event = (zwave_event_t *)m_test_mem_pool[1];
    p_expected_inc_req_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
//    p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.security_keys = key_echo_fail_test_vector[i][1];
    p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
    memcpy(p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.public_key, m_test_public_key_b, sizeof(m_test_public_key_b));
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_inc_req_event;

    // Expect Public KeyA to be sent.
    uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};
    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
    p_mock->expect_arg[3].value    = sizeof(S2_pub_key_A_frame);

    // After our public key has been sent, we expect user acceptance of the inclusion, which will result in key updates and key derivation.
    mock_call_expect(TO_STR(keystore_private_key_read), &p_mock);
    p_mock->output_arg[0].pointer = private_key_a;
    mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
    p_mock->output_arg[0].pointer = public_key_a;

    // Expect a S2 KEX Fail as the including node received an unsupported combination of security schemes.
    mock_call_expect(TO_STR(S2_send_data), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_kex_error_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_kex_error_frame);

    mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
    p_mock->expect_arg[0].value   = 0x01;
    p_mock->output_arg[1].pointer = m_test_network_key_s2_class_0;

    mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
    p_mock->expect_arg[0].value   = 0x02;
    p_mock->output_arg[1].pointer = m_test_network_key_s2_class_1;

    mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
    p_mock->expect_arg[0].value   = 0x04;
    p_mock->output_arg[1].pointer = m_test_network_key_s2_class_2;

    p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
    p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
    mock_call_expect(TO_STR(s2_event_handler), &p_mock);
    p_mock->expect_arg[0].pointer = p_expected_event;

    /*******************
     * Testing section *
     *******************/

      // Node id (first step in inclusion) has been assigned.
    // Continue with secure inclusion.
    s2_inclusion_including_start(&s2_context,&s2_conn);

    // KEX Report frame received - Valid mix of schemes and keys.
    s2_kex_report_frame[3] = key_echo_fail_test_vector[i][0]; // Supported schemes.
    s2_kex_report_frame[5] = key_echo_fail_test_vector[i][1]; // Requested keys bit.
    s2_context.buf    = s2_kex_report_frame;
    s2_context.length = sizeof(s2_kex_report_frame);
    s2_context.peer.class_id = 0xFF;
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_key_grant(&s2_context, 1, key_echo_fail_test_vector[i][1],0); // Granted keys.

    s2_context.buf = frame_buffer;
    helper_func_pub_key_frame(&s2_context);
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_challenge_response(&s2_context, 1, m_test_public_key_b);

    // KEX Echo Set frame received - Valid mix of schemes and keys.
    s2_echo_kex_set_frame[3] = key_echo_fail_test_vector[i][2]; // Wrong match supported schemes.
    s2_echo_kex_set_frame[5] = key_echo_fail_test_vector[i][3]; // Wrong match requested keys bit.
    s2_context.buf    = s2_echo_kex_set_frame;
    s2_context.length = sizeof(s2_echo_kex_set_frame);
    s2_context.peer.class_id = 0x00;
    s2_inclusion_post_event(&s2_context);

    s2_inclusion_send_done(&s2_context, 1);
  }

  // Retry node inclusion.
  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

/** Test case for ensuring correct timer handling on timeout while in idle.
 *
 *  Purpose: In order to ensure correct inclusion flow and avoid deadlock during inclusion it is
 *           important that in case messages are never received during inclusion, then the state
 *           machine must return to idle and push a corresponding event.
 *
 *  Test: This test case will verify expectations related to the timer handling.
 *        If a joining node does not respond within the timeout to a S2 inclusion frame then the
 *        state machine must return to idle and push the corresponding event upwards to application.
 *        After timeout, we expect nothing to happen as we are already in idle.
 *
 */
void test_kex_inclusion_timeout_when_idle() {
  mock_t  * p_mock;
  uint8_t   frame_buffer[UNIT_TEST_BUF_SIZE];
  struct S2 s2_context;

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(keystore_network_key_read));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);


  // No timeout event/node inclusion failure is expected as we are already in idle.

  // After timeout we will issue a node inclusion just to insure we were in idle and that a node
  // may be included after timeout.
  uint8_t S2_kex_get_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame);

  // Trigger a timeout.
  s2_inclusion_notify_timeout(&s2_context);

  // Node id (first step in inclusion) has been assigned.
  // Let's initiate a secure inclusion after the timeout to verify the state of the system.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  mock_calls_verify();
}

/** Verification that the ECHO Kex report match the received frame even if only a subset of keys are granted.
 */
void test_kex_including_node_key_grant_subset() {
  uint8_t public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  uint8_t private_key_a[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, // Private key.
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  uint8_t s2_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT,
                                   0x00, // bit 0 is echo field, rest are reserved
                                   0x02, // Supported schemes. Scheme 1.
                                   0x01, // Supported ECDH Profiles, bit0=1 is curve 25519 value.
                                   0x87};// Requested keys bits. Security 2 class 1, Security 0 network key.

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));

  // Stubbed as it is about to be removed.
  mock_call_use_as_stub(TO_STR(S2_network_key_update));

  /**
   *  Test expectation setup.
   */
  mock_t * p_mock;

  // Expect a S2 KEX Get to be sent.
  helper_func_kex_get_frame_expect();

  // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
  // The operator (or including app automatically) can then respond with granted keys.
  zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
  p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x87;

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_report_event;

  // Expect a S2 KEX Set to be sent.
  helper_func_kex_set_frame_expect(0x02, 0x01, 0x82);

  // When the public key is received, we expect that the event from libs2 contains the key in order to present it for the operator.
  // Therefore we copy the key minus header frame into expected data.
  zwave_event_t  * p_expected_challenge_event = (zwave_event_t *)m_test_mem_pool[1];
  p_expected_challenge_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
  p_expected_challenge_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
  memcpy(p_expected_challenge_event->evt.s2_event.s2_data.challenge_req.public_key, m_test_public_key_b, sizeof(m_test_public_key_b));

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_challenge_event;

  // When the public key is received, we expect a call to the keystore in order to obtain our public key.
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;

  // Expect Public KeyA to be sent.
  uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_pub_key_A_frame);

  // After our public key has been sent, we expect user acceptance of the inclusion, which will result in key updates and key derivation.
  mock_call_expect(TO_STR(keystore_private_key_read), &p_mock);
  p_mock->output_arg[0].pointer = private_key_a;
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;


  // Expect Echo(KEX Report) to be sent.
  uint8_t S2_echo_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x01, 0x02, 0x01, 0x87}; // Note: Echo flag set.
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1] = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_echo_kex_report_frame; // Ideally, this should be updated to be identically to replayed received KEX Set.
  p_mock->expect_arg[3].value    = sizeof(S2_echo_kex_report_frame);

  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x02;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_1;

  // Expect Net Key Report to be sent.
  uint8_t S2_net_key_report_2_frame[19] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_REPORT, 0x02, };
  memcpy(&S2_net_key_report_2_frame[3], m_test_network_key_s2_class_1, sizeof(m_test_network_key_s2_class_1));
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1] = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_net_key_report_2_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_net_key_report_2_frame);

  // Expect S2 Transfer End to be sent.
  uint8_t S2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x02};
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1] = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_transfer_end_frame);

  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x80;
  p_mock->output_arg[1].pointer = m_test_network_key_s0;

  // Expect Net Key Set to be sent.
  uint8_t S2_net_key_report_0_frame[19] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_REPORT, 0x80, };
  memcpy(&S2_net_key_report_0_frame[3], m_test_network_key_s0, sizeof(m_test_network_key_s0));
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1] = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_net_key_report_0_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_net_key_report_0_frame);

  // Expect S2 Transfer End to be sent.
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1] = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_transfer_end_frame);

  // When S2 Transfer End is received, we expect keys to be restored and a corresponding Node inclusion complete event from libs2.
  helper_func_restore_keys_expect();

  zwave_event_t  * p_expected_complete_event = (zwave_event_t *)m_test_mem_pool[2];
  p_expected_complete_event->event_type = S2_NODE_INCLUSION_COMPLETE_EVENT;
  p_expected_complete_event->evt.s2_event.s2_data.inclusion_complete.exchanged_keys = 0x82;

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_complete_event ;

  /**
   *  Test execution.
   */
  struct S2     s2_context;

  /*FIXME S2_init_ctx() bomb placed */

  s2_context.inclusion_state = S2_INC_IDLE;
  s2_inclusion_set_event_handler(s2_event_handler);

  // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  // KEX Report frame received.
  s2_context.buf    = s2_kex_report_frame;
  s2_context.length = sizeof(s2_kex_report_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_key_grant(&s2_context, 1, 0x82,0);

  uint8_t public_key_frame[3 + sizeof(m_test_public_key_b)] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x00}; // Key exchange received from slave - public key for secure exchange of LTK.
  memcpy(&public_key_frame[3], m_test_public_key_b, sizeof(m_test_public_key_b));
  s2_context.buf    = public_key_frame;
  s2_context.length = sizeof(public_key_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_challenge_response(&s2_context, 1, m_test_public_key_b);

  // Echo(KEX Set) frame received.
  // bit0: echo field set, bit1-7: Reserved.
  // Selected schemes: scheme 0 and scheme 2.
  // Selected curve25519
  // Keys to exchange, Security2, class 2 - Security0, network key.
  uint8_t s2_echo_kex_set_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x01, 0x02, 0x01, 0x82};
  s2_context.buf    = s2_echo_kex_set_frame;
  s2_context.length = sizeof(s2_echo_kex_set_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);

  // Network Key Get frame received.
  uint8_t s2_net_key_get_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_GET, 0x02};   // Keys requested, Security2, class 2 - Security0, network key.
  s2_context.buf    = s2_net_key_get_frame;
  s2_context.length = sizeof(s2_net_key_get_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);

  // Network Key Verify frame received.
  uint8_t s2_net_key_verify_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_VERIFY};
  s2_context.buf    = s2_net_key_verify_frame;
  s2_context.length = sizeof(s2_net_key_verify_frame);
  s2_context.peer.class_id = 0x01;
  s2_inclusion_post_event(&s2_context);

  // Network Key Get frame received.
  uint8_t s2_net_key_get_0_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_GET, 0x80};   // Keys requested, Security2, class 2 - Security0, network key.
  s2_context.buf    = s2_net_key_get_0_frame;
  s2_context.length = sizeof(s2_net_key_get_0_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);

  // Network Key Verify frame received.
  uint8_t s2_net_key_verify_0_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_VERIFY};
  s2_context.buf    = s2_net_key_verify_0_frame;
  s2_context.length = sizeof(s2_net_key_verify_0_frame);
  s2_context.peer.class_id = 0x01;
  s2_inclusion_post_event(&s2_context);

  // S2 Transfer end frame received.
  // bit0: Key request complete set,
  // bit1: Key verified not set,
  // bit2-7: Reserved.
  uint8_t s2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x01};
  s2_context.buf    = s2_transfer_end_frame;
  s2_context.length = sizeof(s2_transfer_end_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);

  mock_calls_verify();
}

/** Verification that the in case an decryption failure is reported from the S2 protocol layer,
 *  that the S2 inclusion state machine transmits the corresponding error to the peer.
 */
void test_kex_including_node_decrypt_failed() {
  uint8_t public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  uint8_t private_key_a[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, // Private key.
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  uint8_t s2_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT,
                                   0x00, // bit 0 is echo field, rest are reserved
                                   0x02, // Supported schemes. Scheme 1.
                                   0x01, // Supported ECDH Profiles, bit0=1 is curve 25519 value.
                                   0x82};// Requested keys bits. Security 2 class 1, Security 0 network key.
  zwave_event_t * p_expected_event;

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));

  // Stubbed as it is about to be removed.
  mock_call_use_as_stub(TO_STR(S2_network_key_update));

  /**
   *  Test expectation setup.
   */
  mock_t * p_mock;

  // Expect a S2 KEX Get to be sent.
  helper_func_kex_get_frame_expect();

  // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
  // The operator (or including app automatically) can then respond with granted keys.
  zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
  p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x82;

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_report_event;

  // Expect a S2 KEX Set to be sent.
  helper_func_kex_set_frame_expect(0x02, 0x01, 0x82);

  // When the public key is received, we expect that the event from libs2 contains the key in order to present it for the operator.
  // Therefore we copy the key minus header frame into expected data.
  zwave_event_t  * p_expected_challenge_event = (zwave_event_t *)m_test_mem_pool[1];
  p_expected_challenge_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
  p_expected_challenge_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
  p_expected_challenge_event->evt.s2_event.s2_data.challenge_req.public_key[0] = 0x00;
  p_expected_challenge_event->evt.s2_event.s2_data.challenge_req.public_key[1] = 0x00;
  memcpy(&p_expected_challenge_event->evt.s2_event.s2_data.challenge_req.public_key[2], &m_test_public_key_b[2], sizeof(m_test_public_key_b) - 2);

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_challenge_event;

  // When the public key is received, we expect a call to the keystore in order to obtain our public key.
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;

  // Expect Public KeyA to be sent.
  uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
  p_mock->expect_arg[3].value    = sizeof(S2_pub_key_A_frame);

  // After our public key has been sent, we expect user acceptance of the inclusion, which will result in key updates and key derivation.
  mock_call_expect(TO_STR(keystore_private_key_read), &p_mock);
  p_mock->output_arg[0].pointer = private_key_a;
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;

  // When inclusion fails, we expect a KEX Fail to be sent to peer.
  uint8_t S2_kex_error_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_FAIL, 0x05}; // KEX_FAIL_DECRYPT = 0x05

    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_kex_error_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_kex_error_frame);

  // When inclusion fails, we expect keys to be restored and a corresponding failed inclusion event from libs2.
  helper_func_restore_keys_expect();

  p_expected_event = (zwave_event_t *)m_test_mem_pool[2];
  p_expected_event->event_type = S2_NODE_INCLUSION_FAILED_EVENT;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_event;

  /**
   *  Test execution.
   */
  struct S2     s2_context;

  /*FIXME S2_init_ctx() bomb placed */

  s2_context.inclusion_state = S2_INC_IDLE;
  s2_inclusion_set_event_handler(s2_event_handler);

  // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);

  // KEX Report frame received.
  s2_context.buf    = s2_kex_report_frame;
  s2_context.length = sizeof(s2_kex_report_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  s2_inclusion_key_grant(&s2_context, 1, 0x82,0);

  uint8_t public_key_frame[3 + sizeof(m_test_public_key_b)] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x00, 0x00, 0x00}; // Key exchange received from slave - public key for secure exchange of LTK.
  memcpy(&public_key_frame[5], &m_test_public_key_b[2], sizeof(m_test_public_key_b) - 2);
  s2_context.buf    = public_key_frame;
  s2_context.length = sizeof(public_key_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  // A ECHO KEX Set was received, unfortunately the missing part of the DSK has not yet been
  // provided by the end user, resulting in s2_inclusion_decryption_failure(...) from S2
  // protocol layer.
  // This is supposed to be ignored by the state machine.
  s2_inclusion_decryption_failure(&s2_context);

  s2_inclusion_decryption_failure(&s2_context);

  // User provides wrong key.

  const uint8_t wrong_key[] = {0xBB,0xCC};
  s2_inclusion_challenge_response(&s2_context, 1, wrong_key);

  //The joing node's retry of A ECHO KEX Set was received, public key is set but with wrong value.
  // This result in s2_inclusion_decryption_failure(...) from S2 protocol layer which should
  // trigger transmission of a KEX Fail.
  s2_inclusion_decryption_failure(&s2_context);

  s2_inclusion_send_done(&s2_context, 1);

  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

/** Verification that in case the inclusion state machine is in idle, then AUTH Failed should not
 *  be returned for an invalid encrypted frame. TO#06510
 */
void test_kex_invalid_crypt_frame_idle() {

  mock_calls_clear();

  /**
   *  Test expectation setup.
   */

  /**
   *  Test execution.
   */
  struct S2     s2_context;

  s2_context.inclusion_state = S2_INC_IDLE;
  s2_inclusion_set_event_handler(s2_event_handler);

  // S2 Transfer end frame received.
  // bit0: Key request complete set,
  // bit1: Key verified not set,
  // bit2-7: Reserved.
  uint8_t s2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x01};
  s2_context.buf    = s2_transfer_end_frame;
  s2_context.length = sizeof(s2_transfer_end_frame);
  s2_context.peer.class_id = 0xFF;
  s2_inclusion_post_event(&s2_context);

  helper_func_verify_idle_state(&s2_context);

  mock_calls_verify();
}

void test_kex_inclusion_tx_queue_full() {
  mock_t       * p_mock;
  static uint8_t frame_buffer[UNIT_TEST_BUF_SIZE];
  struct S2      s2_context;
  uint8_t        public_key_a[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, // Public key as being set by upper layer.
                                   0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                   0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                   0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  uint8_t private_key_a[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, // Private key.
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  mock_calls_clear();
  mock_call_use_as_stub(TO_STR(s2_inclusion_set_timeout));
  mock_call_use_as_stub(TO_STR(S2_network_key_update));
  mock_call_use_as_stub(TO_STR(curve25519_mock.c));
  mock_call_use_as_stub(TO_STR(kderiv_mock.c));

  memset(&s2_context, 0, sizeof(s2_context));
  s2_context.inclusion_state = S2_INC_IDLE;
  s2_context.buf             = frame_buffer;
  s2_inclusion_set_event_handler(s2_event_handler);

    // Expect a S2 KEX Get to be sent.
  static uint8_t S2_kex_get_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame);
  p_mock->return_code.value     = 0;

  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame);
  p_mock->return_code.value     = 1;

  // When the KEX Report is received, we expect that the event from libs2 contains the key requested in order to present it for the operator.
  // The operator (or including app automatically) can then respond with granted keys.
  zwave_event_t  * p_expected_report_event = (zwave_event_t *)m_test_mem_pool[0];
  p_expected_report_event->event_type = S2_NODE_INCLUSION_KEX_REPORT_EVENT;
  p_expected_report_event->evt.s2_event.s2_data.kex_report.security_keys = 0x82;
  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_report_event;

  // Expect a S2 KEX Set to be sent.
  static uint8_t S2_kex_set_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x00, 0x02, 0x01, 0x82};
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_set_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_set_frame);
  p_mock->return_code.value     = 0;

  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_set_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_set_frame);
  p_mock->return_code.value     = 1;

  // When the public key is received, we expect that the event from libs2 contains the key in order to present it for the operator.
  // Therefore we copy the key minus header frame into expected data.
  zwave_event_t  * p_expected_inc_req_event = (zwave_event_t *)m_test_mem_pool[1];
  p_expected_inc_req_event->event_type = S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT;
  p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.length = sizeof(m_test_public_key_b);
  memcpy(p_expected_inc_req_event->evt.s2_event.s2_data.challenge_req.public_key, m_test_public_key_b, sizeof(m_test_public_key_b));

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_inc_req_event;

  // When the public key is received, we expect a call to the keystore in order to obtain our public key.
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;

  // Expect Public KeyA to be sent.
  uint8_t S2_pub_key_A_frame[] = {COMMAND_CLASS_SECURITY_2, PUBLIC_KEY_REPORT, 0x01, // Including node bit set
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,   // Public key as being set by upper layer.
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_pub_key_A_frame);
  p_mock->return_code.value     = 0;

  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_pub_key_A_frame);
  p_mock->return_code.value     = 0;

  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_pub_key_A_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_pub_key_A_frame);
  p_mock->return_code.value     = 1;

  // After our public key has been sent, we expect user acceptance of the inclusion, which will result in key updates and key derivation.
  mock_call_expect(TO_STR(keystore_private_key_read), &p_mock);
  p_mock->output_arg[0].pointer = private_key_a;
  mock_call_expect(TO_STR(keystore_public_key_read), &p_mock);
  p_mock->output_arg[0].pointer = public_key_a;

  // Expect Echo(KEX Report) to be sent.
  static uint8_t S2_echo_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x01, 0x02, 0x01, 0x82};
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_echo_kex_report_frame; // Ideally, this should be updated to be identically to replayed received KEX Set.
  p_mock->expect_arg[3].value   = sizeof(S2_echo_kex_report_frame);
  p_mock->return_code.value     = 0;

  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_echo_kex_report_frame; // Ideally, this should be updated to be identically to replayed received KEX Set.
  p_mock->expect_arg[3].value   = sizeof(S2_echo_kex_report_frame);
  p_mock->return_code.value     = 1;

  // Expect Net Key Set to be sent.
  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x02;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_1;

  static uint8_t S2_net_key_set_frame[19] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_REPORT, 0x02};
  memcpy(&S2_net_key_set_frame[3], m_test_network_key_s2_class_1, sizeof(m_test_network_key_s2_class_1));
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_net_key_set_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_net_key_set_frame);
  p_mock->return_code.value     = 0;

  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_net_key_set_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_net_key_set_frame);
  p_mock->return_code.value     = 1;

  // Expect S2 Transfer End to be sent.
  static uint8_t S2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x02};
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_transfer_end_frame);
  p_mock->return_code.value     = 0;

  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_transfer_end_frame);
  p_mock->return_code.value     = 1;

  // Expect Net Key Set to be sent.
  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x80;
  p_mock->output_arg[1].pointer = m_test_network_key_s0;

  static uint8_t S2_net_key_set_frame_s0[19] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_REPORT, 0x80};
  memcpy(&S2_net_key_set_frame_s0[3], m_test_network_key_s0, sizeof(m_test_network_key_s0));
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_net_key_set_frame_s0;
  p_mock->expect_arg[3].value   = sizeof(S2_net_key_set_frame_s0);
  p_mock->return_code.value     = 1;

  // Expect S2 Transfer End to be sent.
  //static uint8_t S2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x02};
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_transfer_end_frame);
  p_mock->return_code.value     = 0;

  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_transfer_end_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_transfer_end_frame);
  p_mock->return_code.value     = 1;

  helper_func_restore_keys_expect();
  zwave_event_t  * p_expected_complete_event = (zwave_event_t *)m_test_mem_pool[2];
  p_expected_complete_event->event_type = S2_NODE_INCLUSION_COMPLETE_EVENT;
  p_expected_complete_event->evt.s2_event.s2_data.inclusion_complete.exchanged_keys = 0x82;

  mock_call_expect(TO_STR(s2_event_handler), &p_mock);
  p_mock->expect_arg[0].pointer = p_expected_complete_event;

  // Node id (first step in inclusion) has been assigned.
  // Continue with secure inclusion.
  s2_inclusion_including_start(&s2_context,&s2_conn);
  s2_inclusion_send_done(&s2_context, 1);

  helper_func_kex_report_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);
  s2_inclusion_key_grant(&s2_context, 1, 0x82,0);
  s2_inclusion_send_done(&s2_context, 1);

  helper_func_pub_key_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);
  s2_inclusion_send_done(&s2_context, 0);
  s2_inclusion_challenge_response(&s2_context, 1, m_test_public_key_b);
  s2_inclusion_send_done(&s2_context, 1);

  helper_func_echo_kex_set_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);
  s2_inclusion_send_done(&s2_context, 1);

  helper_func_net_key_get_frame(&s2_context, 0x02);
  s2_inclusion_post_event(&s2_context);
  s2_inclusion_send_done(&s2_context, 1);

  helper_func_net_key_verify_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);
  s2_inclusion_send_done(&s2_context, 1);

  helper_func_net_key_get_frame(&s2_context, 0x80);
  s2_inclusion_post_event(&s2_context);
  s2_inclusion_send_done(&s2_context, 1);

  helper_func_net_key_verify_frame(&s2_context);
  s2_inclusion_post_event(&s2_context);
  s2_inclusion_send_done(&s2_context, 1);

  uint8_t s2_transfer_end_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_TRANSFER_END, 0x01};
  s2_context.buf    = s2_transfer_end_frame;
  s2_context.length = sizeof(s2_transfer_end_frame);
  s2_context.peer.class_id = 0x00;
  s2_inclusion_post_event(&s2_context);

  mock_calls_verify();
}


/** Helper function sections - Start */

/** This helper function set up an expected KEX get frame to be send (through the mock). */
void helper_func_kex_get_frame_expect(void)
{
  mock_t         * p_mock;
  // Static to keep in scope for complete test.
  static uint8_t   S2_kex_get_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};

  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_kex_get_frame;
  p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame);
}

/** This helper function set up the expectation of restoring all keys, either upon completion or error. */
void helper_func_restore_keys_expect(void)
{
  mock_t * p_mock;

  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x01;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_0;

  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x02;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_1;

  mock_call_expect(TO_STR(keystore_network_key_read), &p_mock);
  p_mock->expect_arg[0].value   = 0x04;
  p_mock->output_arg[1].pointer = m_test_network_key_s2_class_2;
}

/** This helper function set up an expected KEX set frame to be send (through the mock).
 *  param[in] expected_scheme  Expected value for scheme field.
 *  param[in] expected_curves  Expected value for curves field.
 *  param[in] expected_keys    Expected value for keys field.
 */
void helper_func_kex_set_frame_expect(uint8_t expected_scheme, uint8_t expected_curves, uint8_t expected_keys)
{
  mock_t * p_mock;
  static uint8_t s2_kex_set_frame[6] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x00, 0x00, 0x00, 0x00};
  // Expect a S2 KEX Set when the scheme/key request combination was accepted.
  s2_kex_set_frame[3] = expected_scheme;
  s2_kex_set_frame[4] = expected_curves;
  s2_kex_set_frame[5] = expected_keys;
  mock_call_expect(TO_STR(S2_send_frame), &p_mock);
  p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = s2_kex_set_frame;
  p_mock->expect_arg[3].value   = sizeof(s2_kex_set_frame);
}

/** This helper function set up an expected Echo KEX report frame to be send (through the mock).
 *  param[in] expected_scheme  Expected value for scheme field.
 *  param[in] expected_curves  Expected value for curves field.
 *  param[in] expected_keys    Expected value for keys field.
 */
void helper_func_echo_kex_report_frame_expect(uint8_t expected_scheme, uint8_t expected_curves, uint8_t expected_keys)
{
  mock_t * p_mock;
  // Expect Echo(KEX Report) to be sent.
  static uint8_t S2_echo_kex_report_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT, 0x01, 0x00, 0x00, 0x00};
  S2_echo_kex_report_frame[3] = expected_scheme;
  S2_echo_kex_report_frame[4] = expected_curves;
  S2_echo_kex_report_frame[5] = expected_keys;
  mock_call_expect(TO_STR(S2_send_data), &p_mock);
  p_mock->compare_rule_arg[0] = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
  p_mock->compare_rule_arg[1] = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
  p_mock->expect_arg[2].pointer = S2_echo_kex_report_frame; // Ideally, this should be updated to be identically to replayed received KEX Set.
  p_mock->expect_arg[3].value    = sizeof(S2_echo_kex_report_frame);
}

/** This helper function constructs a KEX Report frame and update p_context */
void helper_func_kex_report_frame(struct S2 *p_context)
{
  uint8_t s2_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_REPORT,
                        0x00,  // Reserved, echo off                                     => 0x00
                        0x03,  // Supported schemes, Scheme 1 = 1                        => 0x02
                        0x01,  // Curve25519 support                                     => 0x01
                        0x82}; // Security2, Class 1 key = 1, Security0, Network key = 1 => 0x82
  memcpy((uint8_t *)p_context->buf, s2_frame, sizeof(s2_frame));
  p_context->length = sizeof(s2_frame);
  p_context->peer.class_id = 0xFF;
}

void helper_func_pub_key_frame(struct S2 *p_context)
{
  uint8_t * p_tmp = (uint8_t *)p_context->buf;
  p_tmp[0] = COMMAND_CLASS_SECURITY_2;
  p_tmp[1] = PUBLIC_KEY_REPORT;
  p_tmp[2] = 0x00;
  memcpy(&p_tmp[3], m_test_public_key_b, sizeof(m_test_public_key_b));
  p_context->length = 3  + sizeof(m_test_public_key_b);
  p_context->peer.class_id = 0xFF;
}

void helper_func_echo_kex_set_frame(struct S2 *p_context)
{
  uint8_t s2_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_SET, 0x01, 0x02, 0x01, 0x82}; // This data must be compared to earlier sent data. As long as frames are not defined, this data is unknown.
  memcpy((uint8_t *)p_context->buf, s2_frame, sizeof(s2_frame));
  p_context->length = sizeof(s2_frame);
  p_context->peer.class_id = 0x00;
}

void helper_func_net_key_get_frame(struct S2 *p_context, uint8_t key)
{
  uint8_t s2_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_GET, key};
  memcpy((uint8_t *)p_context->buf, s2_frame, sizeof(s2_frame));
  p_context->length = sizeof(s2_frame);
  p_context->peer.class_id = 0x00;
}

  // The context must be updated to net key must be updated when S2 Network Key Set has been tranmitted.

void helper_func_net_key_verify_frame(struct S2 *p_context)
{
  uint8_t s2_frame[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_NETWORK_KEY_VERIFY};
  memcpy((uint8_t *)p_context->buf, s2_frame, sizeof(s2_frame));
  p_context->length = sizeof(s2_frame);
  p_context->peer.class_id = 0x01;
}

void helper_func_verify_idle_state(struct S2 *p_context)
{
  mock_t * p_mock;

  // No timeout event/node inclusion failure is expected as we are verifying that the system is in idle.

  // After timeout we will issue a node inclusion just to insure we were in idle and that a node
  // may be included after timeout.

  if (!mock_call_used_as_stub(TO_STR(S2_send_frame), "s2_extern_mock.c"))
  {
    uint8_t S2_kex_get_frame[] = {COMMAND_CLASS_SECURITY_2, KEX_GET};
    mock_call_expect(TO_STR(S2_send_frame), &p_mock);
    p_mock->compare_rule_arg[0]   = COMPARE_ANY;  // For the outline, we just expect any/null pointers now.
    p_mock->compare_rule_arg[1]   = COMPARE_NOT_NULL; // This shall be updated once excact frame is defined for S2 frames.
    p_mock->expect_arg[2].pointer = S2_kex_get_frame;
    p_mock->expect_arg[3].value   = sizeof(S2_kex_get_frame);
  }

  if (!mock_call_used_as_stub(TO_STR(s2_inclusion_set_timeout), "s2_inclusion_extern_mock.c"))
  {
    mock_call_expect(TO_STR(s2_inclusion_set_timeout), &p_mock);
    p_mock->compare_rule_arg[0] = COMPARE_ANY;
    p_mock->expect_arg[1].value = 1000;
    p_mock->return_code.value   = 1;
  }

  // Node id (first step in inclusion) has been assigned.
  // Let's initiate a secure inclusion after the timeout to verify the state of the system.
  s2_inclusion_including_start(p_context,&s2_conn);
}