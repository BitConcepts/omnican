/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — ISO 14229 / UDS protocol module
 * Copyright (c) 2026 BitConcepts, LLC
 */

#ifndef OMNICAN_UDS_H
#define OMNICAN_UDS_H

#include <omnican/core.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup omnican_uds OmniCAN UDS (ISO 14229)
 * @brief ISO 14229 Unified Diagnostic Services — server and client.
 *
 * Transport: Zephyr ISO-TP (ISO 15765-2), with OmniCAN's patch for
 * Zephyr issue #86025 (same-ID bind/send limitation).
 *
 * Supported services (server):
 *  0x10  DiagnosticSessionControl
 *  0x11  ECUReset
 *  0x22  ReadDataByIdentifier
 *  0x27  SecurityAccess
 *  0x28  CommunicationControl
 *  0x2E  WriteDataByIdentifier
 *  0x31  RoutineControl
 *  0x34  RequestDownload
 *  0x36  TransferData
 *  0x37  RequestTransferExit
 *  0x3E  TesterPresent
 *  0x85  ControlDTCSetting
 *
 * @{
 */

/** UDS service IDs */
#define OMNICAN_UDS_SID_SESSION_CTRL 0x10U
#define OMNICAN_UDS_SID_ECU_RESET 0x11U
#define OMNICAN_UDS_SID_READ_DID 0x22U
#define OMNICAN_UDS_SID_SEC_ACCESS 0x27U
#define OMNICAN_UDS_SID_COMM_CTRL 0x28U
#define OMNICAN_UDS_SID_WRITE_DID 0x2EU
#define OMNICAN_UDS_SID_ROUTINE_CTRL 0x31U
#define OMNICAN_UDS_SID_REQ_DOWNLOAD 0x34U
#define OMNICAN_UDS_SID_TRANSFER_DATA 0x36U
#define OMNICAN_UDS_SID_REQ_XFER_EXIT 0x37U
#define OMNICAN_UDS_SID_TESTER_PRESENT 0x3EU
#define OMNICAN_UDS_SID_CTRL_DTC 0x85U

/** UDS session types */
typedef enum {
	OMNICAN_UDS_SESSION_DEFAULT = 0x01,
	OMNICAN_UDS_SESSION_PROG = 0x02,
	OMNICAN_UDS_SESSION_EXTENDED = 0x03,
} omnican_uds_session_t;

/** UDS negative response codes */
#define OMNICAN_UDS_NRC_GENERAL_REJECT 0x10U
#define OMNICAN_UDS_NRC_SERVICE_NOT_SUPPORTED 0x11U
#define OMNICAN_UDS_NRC_CONDITIONS_NOT_MET 0x22U
#define OMNICAN_UDS_NRC_REQUEST_OUT_OF_RANGE 0x31U
#define OMNICAN_UDS_NRC_SECURITY_ACCESS_DENIED 0x33U
#define OMNICAN_UDS_NRC_RESPONSE_PENDING 0x78U

/**
 * @brief UDS server configuration.
 */
struct omnican_uds_server_cfg {
	uint32_t rx_can_id; /**< Request CAN ID  (e.g. 0x7E0) */
	uint32_t tx_can_id; /**< Response CAN ID (e.g. 0x7E8) */
	uint8_t ecu_addr; /**< ECU logical address          */
};

/**
 * @brief UDS server context.
 */
struct omnican_uds_server;

/**
 * @brief Initialize a UDS server.
 *
 * @param srv   Server context (caller allocates).
 * @param base  OmniCAN base node.
 * @param cfg   Server configuration.
 *
 * @retval OMNICAN_OK on success.
 */
int omnican_uds_server_init(struct omnican_uds_server *srv, struct omnican_node *base,
			    const struct omnican_uds_server_cfg *cfg);

/**
 * @brief UDS service request callback.
 *
 * Application registers callbacks for each SID it wants to handle.
 *
 * @param sid        Service ID.
 * @param req_data   Request payload (after SID byte).
 * @param req_len    Request payload length.
 * @param resp_data  Response buffer (caller fills, up to resp_maxlen).
 * @param resp_len   Output: number of bytes written to resp_data.
 * @param user_data  User context.
 *
 * @return 0 on positive response; negative NRC on failure.
 */
typedef int (*omnican_uds_service_cb_t)(uint8_t sid, const uint8_t *req_data, size_t req_len,
					uint8_t *resp_data, size_t *resp_len, void *user_data);

/**
 * @brief Register a handler for a UDS service ID.
 */
int omnican_uds_register_service(struct omnican_uds_server *srv, uint8_t sid,
				 omnican_uds_service_cb_t cb, void *user_data);

/** @} */ /* omnican_uds */

#ifdef __cplusplus
}
#endif

#endif /* OMNICAN_UDS_H */
