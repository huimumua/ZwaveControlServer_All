 
/* AUTOGENERATED FILE. DO NOT EDIT. */


  #include "unity.h"
  #include "unity_print.h"
#ifndef __codasip__
  #include "string.h"
#endif /* __codasip__ */


int verbose;

void  test_slave_build();
void  test_kex_joining_node_state_machine_ssa();
void  test_kex_joining_node_state_machine_csa();
void  test_kex_joining_node_state_machine_unauthenticated();
void  test_kex_joining_timer_handling();
void  test_kex_inclusion_timeout_all_states();
void  test_kex_inclusion_invalid_frame_order_all_states();
void  test_kex_inclusion_duplicate_frame_all_states();
void  test_kex_inclusion_error_schemes();
void  test_kex_inclusion_init_errors();
void  test_kex_inclusion_error_curves();
void  test_kex_inclusion_error_keys();
void  test_kex_inclusion_error_public_key_including_bit();
void  test_kex_inclusion_public_key_reserved_bits();
void  test_kex_inclusion_error_echo_kex_report();
void  test_kex_inclusion_invalid_key_report_set();
void  test_kex_inclusion_valid_keys();
void  test_kex_inclusion_timeout_when_idle();
void  test_kex_inclusion_invalid_frame_idle();
void  test_kex_inclusion_echo_kex_set_retry();
void  test_inclusion_keystore_handling();
void  test_kex_inclusion_received_kex_fail_frame();
void  test_kex_inclusion_abort();
void  test_kex_joining_node_wrong_dsk_input();
void  test_keystore_doorlock();
void  test_kex_invalid_crypt_frame_idle();
void  test_kex_inclusion_tx_queue_full();


#ifdef __C51__
#include "reg51.h"

void setUp() {
#if 1
SCON  = 0x50;                   /* SCON: mode 1, 8-bit UART, enable rcvr    */
TMOD |= 0x20;                   /* TMOD: timer 1, mode 2, 8-bit reload      */
TH1   = 0xf3;                   /* TH1:  reload value for 2400 baud         */
TR1   = 1;                      /* TR1:  timer 1 run                        */
TI    = 1;                      /* TI:   set TI to send first char of UART  */
#else
  int bBaudRate;
  WATCHDOG_DISABLE;
  bBaudRate = 1152;
  bBaudRate = (80000/bBaudRate ) + (((80000 %bBaudRate ) >= (bBaudRate >> 1))  ? 1:0);
  UART0_SET_BAUD_RATE(68);
  UART0_TX_ENABLE;
  
  OPEN_IOS
  UART0BUF = '*';
#endif

}

void tearDown() {

}

extern int main(void) {
    int i;
    int ret;
    
    verbose=0;
    unity_print_init();
    UNITY_BEGIN();


    UnityDefaultTestRun(&test_slave_build, "test_slave_build",  185 );
    UnityDefaultTestRun(&test_kex_joining_node_state_machine_ssa, "test_kex_joining_node_state_machine_ssa",  514 );
    UnityDefaultTestRun(&test_kex_joining_node_state_machine_csa, "test_kex_joining_node_state_machine_csa",  518 );
    UnityDefaultTestRun(&test_kex_joining_node_state_machine_unauthenticated, "test_kex_joining_node_state_machine_unauthenticated",  532 );
    UnityDefaultTestRun(&test_kex_joining_timer_handling, "test_kex_joining_timer_handling",  776 );
    UnityDefaultTestRun(&test_kex_inclusion_timeout_all_states, "test_kex_inclusion_timeout_all_states",  920 );
    UnityDefaultTestRun(&test_kex_inclusion_invalid_frame_order_all_states, "test_kex_inclusion_invalid_frame_order_all_states",  1145 );
    UnityDefaultTestRun(&test_kex_inclusion_duplicate_frame_all_states, "test_kex_inclusion_duplicate_frame_all_states",  1365 );
    UnityDefaultTestRun(&test_kex_inclusion_error_schemes, "test_kex_inclusion_error_schemes",  1584 );
    UnityDefaultTestRun(&test_kex_inclusion_init_errors, "test_kex_inclusion_init_errors",  1687 );
    UnityDefaultTestRun(&test_kex_inclusion_error_curves, "test_kex_inclusion_error_curves",  1754 );
    UnityDefaultTestRun(&test_kex_inclusion_error_keys, "test_kex_inclusion_error_keys",  1863 );
    UnityDefaultTestRun(&test_kex_inclusion_error_public_key_including_bit, "test_kex_inclusion_error_public_key_including_bit",  1975 );
    UnityDefaultTestRun(&test_kex_inclusion_public_key_reserved_bits, "test_kex_inclusion_public_key_reserved_bits",  2069 );
    UnityDefaultTestRun(&test_kex_inclusion_error_echo_kex_report, "test_kex_inclusion_error_echo_kex_report",  2263 );
    UnityDefaultTestRun(&test_kex_inclusion_invalid_key_report_set, "test_kex_inclusion_invalid_key_report_set",  2509 );
    UnityDefaultTestRun(&test_kex_inclusion_valid_keys, "test_kex_inclusion_valid_keys",  2694 );
    UnityDefaultTestRun(&test_kex_inclusion_timeout_when_idle, "test_kex_inclusion_timeout_when_idle",  2814 );
    UnityDefaultTestRun(&test_kex_inclusion_invalid_frame_idle, "test_kex_inclusion_invalid_frame_idle",  2866 );
    UnityDefaultTestRun(&test_kex_inclusion_echo_kex_set_retry, "test_kex_inclusion_echo_kex_set_retry",  2897 );
    UnityDefaultTestRun(&test_inclusion_keystore_handling, "test_inclusion_keystore_handling",  3066 );
    UnityDefaultTestRun(&test_kex_inclusion_received_kex_fail_frame, "test_kex_inclusion_received_kex_fail_frame",  3240 );
    UnityDefaultTestRun(&test_kex_inclusion_abort, "test_kex_inclusion_abort",  3306 );
    UnityDefaultTestRun(&test_kex_joining_node_wrong_dsk_input, "test_kex_joining_node_wrong_dsk_input",  3352 );
    UnityDefaultTestRun(&test_keystore_doorlock, "test_keystore_doorlock",  3488 );
    UnityDefaultTestRun(&test_kex_invalid_crypt_frame_idle, "test_kex_invalid_crypt_frame_idle",  3656 );
    UnityDefaultTestRun(&test_kex_inclusion_tx_queue_full, "test_kex_inclusion_tx_queue_full",  3688 );

    ret = UNITY_END();
    unity_print_close();
    while(1);
}
#else
int main(int argc, char** argp) {
    int ret;
    unity_print_init();
    UNITY_BEGIN();


    UnityDefaultTestRun(&test_slave_build, "test_slave_build",  185 );
    UnityDefaultTestRun(&test_kex_joining_node_state_machine_ssa, "test_kex_joining_node_state_machine_ssa",  514 );
    UnityDefaultTestRun(&test_kex_joining_node_state_machine_csa, "test_kex_joining_node_state_machine_csa",  518 );
    UnityDefaultTestRun(&test_kex_joining_node_state_machine_unauthenticated, "test_kex_joining_node_state_machine_unauthenticated",  532 );
    UnityDefaultTestRun(&test_kex_joining_timer_handling, "test_kex_joining_timer_handling",  776 );
    UnityDefaultTestRun(&test_kex_inclusion_timeout_all_states, "test_kex_inclusion_timeout_all_states",  920 );
    UnityDefaultTestRun(&test_kex_inclusion_invalid_frame_order_all_states, "test_kex_inclusion_invalid_frame_order_all_states",  1145 );
    UnityDefaultTestRun(&test_kex_inclusion_duplicate_frame_all_states, "test_kex_inclusion_duplicate_frame_all_states",  1365 );
    UnityDefaultTestRun(&test_kex_inclusion_error_schemes, "test_kex_inclusion_error_schemes",  1584 );
    UnityDefaultTestRun(&test_kex_inclusion_init_errors, "test_kex_inclusion_init_errors",  1687 );
    UnityDefaultTestRun(&test_kex_inclusion_error_curves, "test_kex_inclusion_error_curves",  1754 );
    UnityDefaultTestRun(&test_kex_inclusion_error_keys, "test_kex_inclusion_error_keys",  1863 );
    UnityDefaultTestRun(&test_kex_inclusion_error_public_key_including_bit, "test_kex_inclusion_error_public_key_including_bit",  1975 );
    UnityDefaultTestRun(&test_kex_inclusion_public_key_reserved_bits, "test_kex_inclusion_public_key_reserved_bits",  2069 );
    UnityDefaultTestRun(&test_kex_inclusion_error_echo_kex_report, "test_kex_inclusion_error_echo_kex_report",  2263 );
    UnityDefaultTestRun(&test_kex_inclusion_invalid_key_report_set, "test_kex_inclusion_invalid_key_report_set",  2509 );
    UnityDefaultTestRun(&test_kex_inclusion_valid_keys, "test_kex_inclusion_valid_keys",  2694 );
    UnityDefaultTestRun(&test_kex_inclusion_timeout_when_idle, "test_kex_inclusion_timeout_when_idle",  2814 );
    UnityDefaultTestRun(&test_kex_inclusion_invalid_frame_idle, "test_kex_inclusion_invalid_frame_idle",  2866 );
    UnityDefaultTestRun(&test_kex_inclusion_echo_kex_set_retry, "test_kex_inclusion_echo_kex_set_retry",  2897 );
    UnityDefaultTestRun(&test_inclusion_keystore_handling, "test_inclusion_keystore_handling",  3066 );
    UnityDefaultTestRun(&test_kex_inclusion_received_kex_fail_frame, "test_kex_inclusion_received_kex_fail_frame",  3240 );
    UnityDefaultTestRun(&test_kex_inclusion_abort, "test_kex_inclusion_abort",  3306 );
    UnityDefaultTestRun(&test_kex_joining_node_wrong_dsk_input, "test_kex_joining_node_wrong_dsk_input",  3352 );
    UnityDefaultTestRun(&test_keystore_doorlock, "test_keystore_doorlock",  3488 );
    UnityDefaultTestRun(&test_kex_invalid_crypt_frame_idle, "test_kex_invalid_crypt_frame_idle",  3656 );
    UnityDefaultTestRun(&test_kex_inclusion_tx_queue_full, "test_kex_inclusion_tx_queue_full",  3688 );

    ret = UNITY_END();
    unity_print_close();
    return ret;
}
#endif


