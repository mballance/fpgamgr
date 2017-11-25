/*
 * FPGAMgrMsgE.h
 *
 *  Created on: Nov 20, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_FPGAMGRMSGE_H_
#define SRC_COMMON_FPGAMGRMSGE_H_

typedef enum {
	MSG_PROGRAM=1,
	MSG_SHUTDOWN,
	MSG_DISCONNECT,
	MSG_CONFIG_SIDEBAND, // Enable/disable sideband
	MSG_PING

} fpgamgr_msg_e;



#endif /* SRC_COMMON_FPGAMGRMSGE_H_ */
