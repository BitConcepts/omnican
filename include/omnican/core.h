/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — Unified multi-protocol CAN stack for Zephyr RTOS
 * Copyright (c) 2026 BitConcepts, LLC
 */

#ifndef OMNICAN_CORE_H
#define OMNICAN_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include <zephyr/drivers/can.h>
#include <zephyr/sys/util.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup omnican_core OmniCAN Core
 * @brief Shared types, return codes, and CAN frame abstraction.
 * @{
 */

/** OmniCAN return codes */
typedef enum {
	OMNICAN_OK              =  0,  /**< Success */
	OMNICAN_ERR_INVAL       = -1,  /**< Invalid argument */
	OMNICAN_ERR_NODEV       = -2,  /**< CAN device not ready */
	OMNICAN_ERR_NOMEM       = -3,  /**< Out of memory */
	OMNICAN_ERR_TIMEOUT     = -4,  /**< Operation timed out */
	OMNICAN_ERR_BUSY        = -5,  /**< Resource busy */
	OMNICAN_ERR_PROTO       = -6,  /**< Protocol error */
	OMNICAN_ERR_UNSUPPORTED = -7,  /**< Unsupported operation */
	OMNICAN_ERR_IO          = -8,  /**< Hardware I/O error */
} omnican_err_t;

/**
 * @brief CAN frame type enumeration.
 *
 * Distinguishes classical CAN from CAN FD frames at the OmniCAN layer.
 */
typedef enum {
	OMNICAN_FRAME_CLASSIC = 0, /**< Classical CAN (max 8 data bytes) */
	OMNICAN_FRAME_FD      = 1, /**< CAN FD (max 64 data bytes) */
} omnican_frame_type_t;

/**
 * @brief OmniCAN protocol identifiers.
 *
 * Used by the frame router to identify which protocol owns a received frame.
 */
typedef enum {
	OMNICAN_PROTO_NONE     = 0x00,
	OMNICAN_PROTO_CANOPEN  = 0x01,
	OMNICAN_PROTO_J1939    = 0x02,
	OMNICAN_PROTO_UDS      = 0x04,
	OMNICAN_PROTO_OBD2     = 0x08,
} omnican_proto_t;

/**
 * @brief Common CAN node context, shared by all protocol instances.
 *
 * Every OmniCAN protocol node holds one of these, providing a unified
 * reference to the underlying Zephyr CAN device and runtime state.
 */
struct omnican_node {
	const struct device *can_dev; /**< Zephyr CAN device */
	uint32_t             bitrate; /**< Nominal bitrate in kbps */
	bool                 fd_mode; /**< True if CAN FD is active */
	omnican_proto_t      protos;  /**< Bitmask of enabled protocols */
};

/**
 * @brief Initialize an OmniCAN node context.
 *
 * @param node     Node context to initialise.
 * @param can_dev  Zephyr CAN device pointer.
 * @param bitrate  Nominal bitrate in kbps (e.g. 500).
 * @param fd_mode  True to enable CAN FD mode.
 *
 * @retval OMNICAN_OK        Success.
 * @retval OMNICAN_ERR_NODEV Device not ready.
 */
int omnican_node_init(struct omnican_node *node,
		      const struct device *can_dev,
		      uint32_t bitrate,
		      bool fd_mode);

/** @} */ /* omnican_core */

#ifdef __cplusplus
}
#endif

#endif /* OMNICAN_CORE_H */
