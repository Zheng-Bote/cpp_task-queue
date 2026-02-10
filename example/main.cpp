/**
 * SPDX-FileComment: Example application for Task Queue
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file main.cpp
 * @brief Example usage of the Task Queue library.
 * @version 0.1.0
 * @date 2026-02-10
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "task_loader.hpp"
#include <algorithm>
#include <iostream>
#include <string>

/**
 * @brief Converts a string to lowercase.
 *
 * @param s The string to convert.
 * @return std::string The converted string.
 */
static std::string to_lower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return s;
}

/**
 * @brief Main entry point for the example application.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return int 0 on success, non-zero on failure.
 */
int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <json|ini|auto> <path>\n";
    return 1;
  }

  std::string mode = to_lower(argv[1]);
  std::string path;
  if (mode == "auto") {
    if (argc < 3) {
      std::cout << "Usage: " << argv[0] << " auto <path>\n";
      return 1;
    }
    path = argv[2];
    auto pos = path.find_last_of('.');
    if (pos == std::string::npos) {
      std::cerr << "Cannot autodetect format: no file extension\n";
      return 2;
    }
    std::string ext = to_lower(path.substr(pos + 1));
    if (ext == "ini")
      mode = "ini";
    else if (ext == "json")
      mode = "json";
    else {
      std::cerr << "Unknown extension: " << ext << "\n";
      return 3;
    }
  } else {
    if (argc < 3) {
      std::cout << "Usage: " << argv[0] << " <json|ini|auto> <path>\n";
      return 1;
    }
    path = argv[2];
  }

  TQ_Logger logger{
      [](const std::string &s) { std::cout << "[INFO] " << s << "\n"; },
      [](const std::string &s) { std::cout << "[WARN] " << s << "\n"; },
      [](const std::string &s) { std::cout << "[ERROR] " << s << "\n"; }};

  TaskQueues queues;
  TQ_Error err = TQ_Error::Ok;

  if (mode == "json") {
    err = load_task_queues_from_json(path, queues, logger);
  } else if (mode == "ini") {
    err = load_task_queues_from_ini(path, queues, logger);
  } else {
    std::cout << "Unknown mode: " << mode << "\n";
    return 1;
  }

  if (err != TQ_Error::Ok) {
    std::cout << "Error loading tasks: " << static_cast<int>(err) << "\n";
    return 1;
  }

  queues.pre.set_done();
  queues.middle.set_done();
  queues.post.set_done();

  auto dump = [](auto &q, const std::string &name) {
    std::cout << "--- " << name << " ---\n";
    while (auto t = q.pop()) {
      std::cout << t->order << " | " << t->name << " | " << t->input << " -> "
                << t->output << "\n";
    }
  };

  dump(queues.pre, "PRE");
  dump(queues.middle, "MIDDLE");
  dump(queues.post, "POST");

  return 0;
}
