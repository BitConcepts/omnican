# SPDX-License-Identifier: Apache-2.0
# OmniCAN — Unified multi-protocol CAN stack for Zephyr RTOS
# Copyright (c) 2026 BitConcepts, LLC

cmake_minimum_required(VERSION 3.20)

if(CONFIG_OMNICAN)

zephyr_include_directories(include)

# ---------------------------------------------------------------------------
# Core frame router (always present when any protocol enabled)
# ---------------------------------------------------------------------------
if(CONFIG_OMNICAN_FRAME_ROUTER)
  add_subdirectory(src/core)
endif()

# ---------------------------------------------------------------------------
# ISOTP workaround for Zephyr issue #86025
# ---------------------------------------------------------------------------
if(CONFIG_OMNICAN_ISOTP_PATCH)
  add_subdirectory(src/isotp_patch)
endif()

# ---------------------------------------------------------------------------
# Protocol modules
# ---------------------------------------------------------------------------
if(CONFIG_OMNICAN_CANOPEN)
  add_subdirectory(src/canopen)
endif()

if(CONFIG_OMNICAN_CANOPEN_FD)
  add_subdirectory(src/canopen_fd)
endif()

if(CONFIG_OMNICAN_J1939)
  add_subdirectory(src/j1939)
endif()

if(CONFIG_OMNICAN_UDS)
  add_subdirectory(src/uds)
endif()

if(CONFIG_OMNICAN_OBD2)
  add_subdirectory(src/obd2)
endif()

endif() # CONFIG_OMNICAN
