/**
 * SPDX-FileComment: definition of error codes
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file error_codes.hpp
 * @brief Defines error codes used throughout the Task Queue library.
 * @version 0.1.0
 * @date 2026-02-10
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once

/**
 * @brief Enumeration of possible error codes.
 */
enum class TQ_Error {
  Ok = 0,        /**< No error. */
  FileNotFound,  /**< The specified file was not found. */
  ParseError,    /**< Error parsing the file content. */
  InvalidFormat, /**< The file format is invalid. */
  EmptyName,     /**< A task has an empty name. */
  UnknownType    /**< A task has an unknown type. */
};
