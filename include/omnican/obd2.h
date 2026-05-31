/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — SAE J1979 / OBD-II protocol module
 * Copyright (c) 2026 BitConcepts, LLC
 */

#ifndef OMNICAN_OBD2_H
#define OMNICAN_OBD2_H

#include <omnican/core.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup omnican_obd2 OmniCAN OBD-II (SAE J1979)
 * @brief SAE J1979 OBD-II PID request/response over ISO 15765-2.
 *
 * Supports OBD-II service modes 0x01–0x09.
 * Transport: Zephyr ISO-TP (ISO 15765-4 for CAN OBD).
 *
 * Standard CAN IDs:
 *  0x7DF — broadcast request (all ECUs)
 *  0x7E8–0x7EF — ECU responses
 *
 * @{
 */

/** OBD-II service modes */
#define OMNICAN_OBD2_MODE_CURRENT_DATA    0x01U
#define OMNICAN_OBD2_MODE_FREEZE_FRAME    0x02U
#define OMNICAN_OBD2_MODE_STORED_DTC      0x03U
#define OMNICAN_OBD2_MODE_CLEAR_DTC       0x04U
#define OMNICAN_OBD2_MODE_O2_MONITOR      0x05U
#define OMNICAN_OBD2_MODE_TEST_RESULTS    0x06U
#define OMNICAN_OBD2_MODE_PENDING_DTC     0x07U
#define OMNICAN_OBD2_MODE_CTRL_OP         0x08U
#define OMNICAN_OBD2_MODE_VEHICLE_INFO    0x09U

/** OBD-II well-known PIDs (Mode 0x01) */
#define OMNICAN_OBD2_PID_SUPPORTED_0     0x00U /**< Supported PIDs [01-20] */
#define OMNICAN_OBD2_PID_MONITOR_STATUS  0x01U /**< Monitor status since DTC clear */
#define OMNICAN_OBD2_PID_ENGINE_COOLANT  0x05U /**< Engine coolant temp */
#define OMNICAN_OBD2_PID_ENGINE_LOAD     0x04U /**< Calculated engine load */
#define OMNICAN_OBD2_PID_RPM             0x0CU /**< Engine RPM */
#define OMNICAN_OBD2_PID_SPEED           0x0DU /**< Vehicle speed */
#define OMNICAN_OBD2_PID_THROTTLE        0x11U /**< Throttle position */
#define OMNICAN_OBD2_PID_VIN             0x02U /**< VIN (mode 0x09) */

/**
 * @brief OBD-II PID response callback.
 *
 * @param mode      OBD-II mode (0x01..0x09).
 * @param pid       PID requested.
 * @param data      Response data bytes.
 * @param len       Number of data bytes.
 * @param user_data User context.
 */
typedef void (*omnican_obd2_response_cb_t)(uint8_t mode, uint8_t pid,
					   const uint8_t *data, size_t len,
					   void *user_data);

/**
 * @brief OBD-II client context.
 */
struct omnican_obd2_client;

/**
 * @brief Initialize an OBD-II client (tester/scanner role).
 *
 * @param client   Client context.
 * @param base     OmniCAN base node.
 *
 * @retval OMNICAN_OK on success.
 */
int omnican_obd2_client_init(struct omnican_obd2_client *client,
			     struct omnican_node *base);

/**
 * @brief Request a PID value from the vehicle ECU.
 *
 * @param client    OBD-II client.
 * @param mode      Service mode (e.g. OMNICAN_OBD2_MODE_CURRENT_DATA).
 * @param pid       PID to request.
 * @param cb        Response callback.
 * @param user_data User context passed to callback.
 * @param timeout_ms Request timeout.
 *
 * @retval OMNICAN_OK request sent; callback fires on response.
 */
int omnican_obd2_request_pid(struct omnican_obd2_client *client,
			     uint8_t mode, uint8_t pid,
			     omnican_obd2_response_cb_t cb,
			     void *user_data,
			     uint32_t timeout_ms);

/** @} */ /* omnican_obd2 */

#ifdef __cplusplus
}
#endif

#endif /* OMNICAN_OBD2_H */
