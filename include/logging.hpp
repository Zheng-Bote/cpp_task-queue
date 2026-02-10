/**
 * SPDX-FileComment: Logging utilities
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file logging.hpp
 * @brief Defines logging structures and types.
 * @version 0.1.0
 * @date 2026-02-10
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once
#include <functional>
#include <string>

/**
 * @brief Alias for a logging function that takes a string message.
 */
using TQ_LogFn = std::function<void(const std::string &)>;

/**
 * @brief Structure holding C++ logging callbacks.
 */
struct TQ_Logger {
  TQ_LogFn info;  /**< Callback for info level logs. */
  TQ_LogFn warn;  /**< Callback for warning level logs. */
  TQ_LogFn error; /**< Callback for error level logs. */
};
