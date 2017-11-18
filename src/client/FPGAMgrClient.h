/*
 * FPGAMgrClient.h
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#ifndef SRC_CLIENT_FPGAMGRCLIENT_H_
#define SRC_CLIENT_FPGAMGRCLIENT_H_
#include <string>
#include <stdint.h>

class FPGAMgrMsgStream;
class FPGAMgrClient {
public:
	FPGAMgrClient();

	virtual ~FPGAMgrClient();

	int connect(const std::string &host, uint16_t port);

	int program(const std::string &path);

	int shutdown_server();

	int close();

private:
	int32_t					m_client_sock;
	FPGAMgrMsgStream		*m_stream;

};

#endif /* SRC_CLIENT_FPGAMGRCLIENT_H_ */
