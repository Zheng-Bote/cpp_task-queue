/**
 * SPDX-FileComment: C-compatible API for Task Queue
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file c_api.h
 * @brief C-compatible API for interacting with the Task Queue library.
 * @version 0.1.0
 * @date 2026-02-10
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function pointer type for logging callbacks.
 *
 * @param msg The log message string.
 */
typedef void (*tq_log_fn)(const char *msg);

/**
 * @brief Structure holding logging callbacks.
 */
typedef struct {
  tq_log_fn info;  /**< Callback for info level logs. */
  tq_log_fn warn;  /**< Callback for warning level logs. */
  tq_log_fn error; /**< Callback for error level logs. */
} tq_logger;

/**
 * @brief Opaque structure representing the task queues.
 */
typedef struct tq_queues tq_queues;

/**
 * @brief Loads task queues from a JSON file.
 *
 * @param path Path to the JSON file.
 * @param logger Logger instance for reporting errors and info.
 * @param out Pointer to a pointer that will be set to the created tq_queues
 * object.
 * @return int 0 on success, non-zero error code on failure.
 */
int tq_load_json(const char *path, tq_logger logger, tq_queues **out);

/**
 * @brief Loads task queues from an INI file.
 *
 * @param path Path to the INI file.
 * @param logger Logger instance for reporting errors and info.
 * @param out Pointer to a pointer that will be set to the created tq_queues
 * object.
 * @return int 0 on success, non-zero error code on failure.
 */
int tq_load_ini(const char *path, tq_logger logger, tq_queues **out);

/**
 * @brief Frees the task queues object.
 *
 * @param q Pointer to the tq_queues object to free.
 */
void tq_free(tq_queues *q);

#ifdef __cplusplus
}
#endif
