/**
 * SPDX-FileComment: Task structure definition
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file task.hpp
 * @brief Defines the Task structure and its members.
 * @version 0.1.0
 * @date 2026-02-10
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once
#include <string>
#include <unordered_map>

/**
 * @brief Represents a single task in the queue.
 */
struct Task {
  std::string name;   /**< Unique name of the task. */
  std::string type;   /**< Type of the task (e.g., pre, middle, post). */
  std::string input;  /**< Input identifier for the task. */
  std::string output; /**< Output identifier for the task. */
  int order = 0;      /**< execution order priority. */
  std::unordered_map<std::string, std::string>
      attributes; /**< Additional key-value attributes. */
};
