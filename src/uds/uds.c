/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — ISO 14229 / UDS
 * Copyright (c) 2026 BitConcepts, LLC
 */

#include <omnican/uds.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(omnican_uds, CONFIG_OMNICAN_LOG_LEVEL);

int omnican_uds_server_init(struct omnican_uds_server *srv,
			    struct omnican_node *base,
			    const struct omnican_uds_server_cfg *cfg)
{
	if (!srv || !base || !cfg) {
		return OMNICAN_ERR_INVAL;
	}

	base->protos |= OMNICAN_PROTO_UDS;
	LOG_INF("UDS server init: rx=0x%03x tx=0x%03x ecu=0x%02x",
		cfg->rx_can_id, cfg->tx_can_id, cfg->ecu_addr);
	return OMNICAN_OK;
}

int omnican_uds_register_service(struct omnican_uds_server *srv,
				 uint8_t sid,
				 omnican_uds_service_cb_t cb,
				 void *user_data)
{
	ARG_UNUSED(sid);
	ARG_UNUSED(cb);
	ARG_UNUSED(user_data);

	if (!srv) {
		return OMNICAN_ERR_INVAL;
	}

	return OMNICAN_ERR_UNSUPPORTED; /* pending */
}
