/**
 * SPDX-FileComment: Test loader implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file test_loader.cpp
 * @brief Tests for the Task Queue loader functionality.
 * @version 0.1.0
 * @date 2026-02-10
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "../include/task_loader.hpp"
#include <cassert>
#include <iostream>

/**
 * @brief Converts TQ_Error enum values to string representation.
 *
 * @param e The error code.
 * @return const char* String representation of the error.
 */
static const char *err_to_str(TQ_Error e) {
  switch (e) {
  case TQ_Error::Ok:
    return "Ok";
  case TQ_Error::FileNotFound:
    return "FileNotFound";
  case TQ_Error::ParseError:
    return "ParseError";
  case TQ_Error::InvalidFormat:
    return "InvalidFormat";
  case TQ_Error::EmptyName:
    return "EmptyName";
  case TQ_Error::UnknownType:
    return "UnknownType";
  default:
    return "Unknown";
  }
}

/**
 * @brief Main entry point for the test runner.
 *
 * @return int 0 on success, non-zero on failure.
 */
int main() {
  TQ_Logger logger{[](const std::string &) {}, [](const std::string &) {},
                   [](const std::string &) {}};

  TaskQueues q;
  auto err = load_task_queues_from_ini("../data/data.ini", q, logger);
  if (err != TQ_Error::Ok) {
    std::cerr << "load_task_queues_from_ini failed: " << err_to_str(err) << " ("
              << static_cast<int>(err) << ")\n";
    return 1;
  }

  // basic checks: sizes > 0
  if (q.pre.size() == 0 || q.middle.size() == 0 || q.post.size() == 0) {
    std::cerr << "One of the queues is empty: pre=" << q.pre.size()
              << " middle=" << q.middle.size() << " post=" << q.post.size()
              << "\n";
    return 2;
  }

  return 0;
}
