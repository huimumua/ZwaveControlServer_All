/****************************************************************************
 *
 * Copyright (c) 2001-2014
 * Sigma Designs, Inc.
 * All Rights Reserved
 *
 *---------------------------------------------------------------------------
 *
 * Description: Application NVM variable declarations
 *
 *        All far variables (NVM offsets) should be defined in the application's eeprom.h module
 *        in the struct t_nvmApplDescriptor
 *
 * Author:   Peter Shorty
 *
 * Last Changed By:  $Author:  $
 * Revision:         $Revision:  $
 * Last Changed:     $Date:  $
 *
 ****************************************************************************/
#ifndef _EEPROM_H_
#define _EEPROM_H_

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
#include <ZW_stdint.h>
#include "config_app.h"
#include <association_plus.h>
#include <CommandClassUserCode.h>
#include <CommandClassManufacturerSpecific.h>
#include <ZW_TransportLayer.h>
#include <ZW_nvm_descriptor.h>

/****************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                       */
/****************************************************************************/

/* Default values */
#define APPL_MAGIC_VALUE                 0x42

/*
 * WARNING: The following definitions must not be changed across versions of a product since they
 * contribute to the structure of the NVM. If they are changed, NVM data will be corrupted.
 * - NUMBER_OF_ENDPOINTS_NVM_MAX
 * - ASSOCIATION_SIZE_NVM_MAX
 */
#define NUMBER_OF_ENDPOINTS_NVM_MAX (4)

// ASSOCIATION_SIZE_NVM_MAX is the product of MAX_ASSOCIATION_GROUPS and MAX_ASSOCIATION_IN_GROUP
#define ASSOCIATION_SIZE_NVM_MAX    (15)

#if NUMBER_OF_ENDPOINTS > NUMBER_OF_ENDPOINTS_NVM_MAX
#error "NUMBER_OF_ENDPOINTS cannot be larger than NUMBER_OF_ENDPOINTS_NVM_MAX"
#endif

#if ASSOCIATION_SIZE > ASSOCIATION_SIZE_NVM_MAX
#error "ASSOCIATION_SIZE cannot be larger than ASSOCIATION_SIZE_NVM_MAX"
#endif

/***********************************/
/*      Application NVM layout     */
/***********************************/

/* Note from Keil knowledgebase: http://www.keil.com/support/docs/901.htm   */
/* "The order is not necessarily taken from the variable declarations, but  */
/* the first use of the variables."                                         */
/* Therefore, when using #pragma ORDER to order variables, declare them in  */
/* the order they should be in a collection. And none of them may be        */
/* declared or known in any way from other header files.                    */

/* NVM layout WallController */
extern t_NvmModuleSize far nvmApplicationSize;

extern uint8_t far EEOFFSET_ASSOCIATION_START_far[(NUMBER_OF_ENDPOINTS_NVM_MAX + 1) * ASSOCIATION_SIZE_NVM_MAX];
extern uint8_t far EEOFFSET_ASSOCIATION_MAGIC_far;
extern uint8_t far EEOFFSET_ASSOCIATION_ENDPOINT_START_far[(NUMBER_OF_ENDPOINTS_NVM_MAX + 1) * ASSOCIATION_SIZE_NVM_MAX];
extern EEOFFSET_TRANSPORT_CAPABILITIES_STRUCT far EEOFFSET_TRANSPORT_CAPABILITIES_START_far[(NUMBER_OF_ENDPOINTS_NVM_MAX + 1) * ASSOCIATION_SIZE_NVM_MAX];
extern uint8_t far EEOFFSET_ASSOCIATION_ENDPOINT_MAGIC_far;
extern EEOFFS_NETWORK_SECURITY_STRUCT far EEOFFS_SECURITY_RESERVED; //This can be remove for new sample-applications, but is placed here for backward compatibility

/*
 * The following 2 EEPROM variables is no longer used and should not be used in a new application.
 * They're still here to avoid reset when doing firmware update.
 */
extern uint8_t far EEOFFSET_MAN_SPECIFIC_DEVICE_ID_far[MAN_DEVICE_ID_SIZE];
extern uint8_t far EEOFFSET_MAN_SPECIFIC_MAGIC_far;

extern uint8_t far EEOFFSET_TEST_NODE_ID_far;
extern uint8_t far EEOFFSET_TEST_POWER_LEVEL_far;
extern uint8_t far EEOFFSET_TEST_FRAME_COUNT_SUCCESS_far[2];
extern uint8_t far EEOFFSET_TEST_STATUS_far;
extern uint8_t far EEOFFSET_TEST_SOURCE_NODE_ID_far;
//@ [EEOFFSET_MAGIC_far]
extern uint8_t far EEOFFSET_MAGIC_far;
//@ [EEOFFSET_MAGIC_far]
extern uint8_t far EEOFFSET_CENTRAL_SCENE_SLOW_REFRESH_far;
extern uint8_t far EEOFFSET_MAGIC_SDK_6_70_ASSOCIATION_SECURE_far;

extern t_nvmModuleDescriptor far nvmApplicationDescriptor;

/* The starting address of the segment ?FD?EEPROM (to be used as a constant as (WORD)&_FD_EEPROM_S_) */
extern unsigned char _FD_EEPROM_S_;
/* The length of the segment ?FD?EEPROM in bytes (to be used as a constant as (WORD)&_FD_EEPROM_L_) */
extern unsigned char _FD_EEPROM_L_;

#endif /* _EEPROM_H_ */
