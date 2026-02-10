/**
 * SPDX-FileComment: Task loading functions
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file task_loader.hpp
 * @brief Defines functions for loading tasks from various file formats.
 * @version 0.1.0
 * @date 2026-02-10
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once
#include "error_codes.hpp"
#include "logging.hpp"
#include "worker_queue.hpp"
#include <string>

/**
 * @brief Container for the three processing queues.
 */
struct TaskQueues {
  WorkerQueue pre;    /**< Queue for pre-processing tasks. */
  WorkerQueue middle; /**< Queue for main processing tasks. */
  WorkerQueue post;   /**< Queue for post-processing tasks. */
};

/**
 * @brief Loads task queues from a JSON file.
 *
 * @param path Path to the JSON file.
 * @param out Reference to the TaskQueues structure to populate.
 * @param logger Logger instance for reporting errors and info.
 * @return TQ_Error Error code indicating success or failure.
 */
TQ_Error load_task_queues_from_json(const std::string &path, TaskQueues &out,
                                    const TQ_Logger &logger);

/**
 * @brief Loads task queues from an INI file.
 *
 * @param path Path to the INI file.
 * @param out Reference to the TaskQueues structure to populate.
 * @param logger Logger instance for reporting errors and info.
 * @return TQ_Error Error code indicating success or failure.
 */
TQ_Error load_task_queues_from_ini(const std::string &path, TaskQueues &out,
                                   const TQ_Logger &logger);
