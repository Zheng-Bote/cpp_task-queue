/**
 * SPDX-FileComment: C-API implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file c_api.cpp
 * @brief Implements the C-compatible API for the Task Queue library.
 * @version 0.1.0
 * @date 2026-02-10
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "c_api.h"
#include "task_loader.hpp"

/**
 * @brief Internal wrapper for TaskQueues to expose as an opaque pointer.
 */
struct tq_queues {
  TaskQueues q; /**< The actual C++ TaskQueues object. */
};

/**
 * @brief Helper to call a logging callback if it is set.
 *
 * @param fn The logging callback function pointer.
 * @param s The message string.
 */
static void call_if_set(tq_log_fn fn, const std::string &s) {
  if (fn)
    fn(s.c_str());
}

/**
 * @brief Loads task queues from a JSON file.
 * @see tq_load_json
 */
int tq_load_json(const char *path, tq_logger logger, tq_queues **out) {
  TQ_Logger cpp_logger{
      [&](const std::string &s) { call_if_set(logger.info, s); },
      [&](const std::string &s) { call_if_set(logger.warn, s); },
      [&](const std::string &s) { call_if_set(logger.error, s); }};

  auto *obj = new tq_queues();
  TQ_Error err = load_task_queues_from_json(path, obj->q, cpp_logger);

  if (err != TQ_Error::Ok) {
    delete obj;
    return static_cast<int>(err);
  }

  *out = obj;
  return 0;
}

/**
 * @brief Loads task queues from an INI file.
 * @see tq_load_ini
 */
int tq_load_ini(const char *path, tq_logger logger, tq_queues **out) {
  TQ_Logger cpp_logger{
      [&](const std::string &s) { call_if_set(logger.info, s); },
      [&](const std::string &s) { call_if_set(logger.warn, s); },
      [&](const std::string &s) { call_if_set(logger.error, s); }};

  auto *obj = new tq_queues();
  TQ_Error err = load_task_queues_from_ini(path, obj->q, cpp_logger);

  if (err != TQ_Error::Ok) {
    delete obj;
    return static_cast<int>(err);
  }

  *out = obj;
  return 0;
}

/**
 * @brief Frees the task queues object.
 * @see tq_free
 */
void tq_free(tq_queues *q) { delete q; }
