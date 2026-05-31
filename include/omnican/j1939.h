/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — SAE J1939 protocol module
 * Copyright (c) 2026 BitConcepts, LLC
 */

#ifndef OMNICAN_J1939_H
#define OMNICAN_J1939_H

#include <omnican/core.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup omnican_j1939 OmniCAN SAE J1939
 * @brief SAE J1939 — address claiming, PGN routing, TP/ETP.
 *
 * Uses 29-bit extended CAN identifiers. Implements:
 *  - J1939/21: Transport Protocol (TP/ETP for multi-packet)
 *  - J1939/81: Network Management (address claiming/commanded)
 *  - PGN (Parameter Group Number) routing and callbacks
 *
 * @{
 */

/** J1939 address constants */
#define OMNICAN_J1939_ADDR_NULL    0xFEU  /**< NULL (unclaimed) address */
#define OMNICAN_J1939_ADDR_GLOBAL  0xFFU  /**< Global (broadcast) address */

/** J1939 source address type */
typedef uint8_t omnican_j1939_addr_t;

/** J1939 PGN type (18-bit value) */
typedef uint32_t omnican_j1939_pgn_t;

/**
 * @brief J1939 node context.
 */
struct omnican_j1939_node {
	struct omnican_node    *base;       /**< OmniCAN base node */
	omnican_j1939_addr_t    addr;       /**< Claimed source address */
	uint8_t                 name[8];    /**< 64-bit NAME for address claiming */
};

/**
 * @brief Initialize a J1939 node.
 *
 * @param j1939  J1939 node context.
 * @param base   OmniCAN base node.
 * @param name   64-bit J1939 NAME (8 bytes, big-endian).
 *
 * @retval OMNICAN_OK on success.
 */
int omnican_j1939_init(struct omnican_j1939_node *j1939,
		       struct omnican_node *base,
		       const uint8_t name[8]);

/**
 * @brief Claim a J1939 address (J1939/81 address claiming procedure).
 *
 * @param j1939        J1939 node.
 * @param desired_addr Preferred source address (0x00–0xFD).
 * @param timeout_ms   Max time to wait for address claim result.
 *
 * @retval OMNICAN_OK  Address claimed successfully.
 * @retval OMNICAN_ERR_BUSY  Address in use by higher-priority node.
 */
int omnican_j1939_claim_address(struct omnican_j1939_node *j1939,
				omnican_j1939_addr_t desired_addr,
				uint32_t timeout_ms);

/**
 * @brief Send a J1939 PGN message.
 *
 * @param j1939  J1939 node.
 * @param pgn    Parameter Group Number.
 * @param dst    Destination address (OMNICAN_J1939_ADDR_GLOBAL for broadcast).
 * @param data   Payload data.
 * @param len    Payload length (1–1785 bytes; >8 uses TP).
 *
 * @retval OMNICAN_OK on success.
 */
int omnican_j1939_send(struct omnican_j1939_node *j1939,
		       omnican_j1939_pgn_t pgn,
		       omnican_j1939_addr_t dst,
		       const uint8_t *data, size_t len);

/**
 * @brief PGN receive callback type.
 */
typedef void (*omnican_j1939_rx_cb_t)(omnican_j1939_pgn_t pgn,
				      omnican_j1939_addr_t src,
				      const uint8_t *data, size_t len,
				      void *user_data);

/**
 * @brief Register a receive callback for a specific PGN.
 */
int omnican_j1939_register_pgn(struct omnican_j1939_node *j1939,
			       omnican_j1939_pgn_t pgn,
			       omnican_j1939_rx_cb_t cb,
			       void *user_data);

/** @} */ /* omnican_j1939 */

#ifdef __cplusplus
}
#endif

#endif /* OMNICAN_J1939_H */
