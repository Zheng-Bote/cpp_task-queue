/**
 * SPDX-FileComment: JSON Task Loader implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file task_loader.cpp
 * @brief Implements loading of tasks from JSON files.
 * @version 0.1.0
 * @date 2026-02-10
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "task_loader.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_set>

using json = nlohmann::json;

/**
 * @brief Parses a single task from a JSON object.
 *
 * @param j The JSON object representing a task.
 * @param t Reference to the Task object to populate.
 * @return true If parsing was successful.
 * @return false If the JSON object is invalid (e.g., missing name).
 */
static bool parse_task_json(const json &j, Task &t) {
  if (!j.contains("name") || !j["name"].is_string())
    return false;

  t.name = j.value("name", "");
  t.type = j.value("type", "");
  t.input = j.value("input", "");
  t.output = j.value("output", "");
  t.order = j.value("order", 0);

  if (j.contains("attr") && j["attr"].is_array()) {
    for (const auto &a : j["attr"]) {
      std::string key = a.value("key", "");
      std::string value = a.value("value", "");
      if (!key.empty())
        t.attributes[key] = value;
    }
  }
  return true;
}

/**
 * @brief Loads task queues from a JSON file.
 * @see load_task_queues_from_json
 */
TQ_Error load_task_queues_from_json(const std::string &path, TaskQueues &out,
                                    const TQ_Logger &logger) {
  std::ifstream file(path);
  if (!file) {
    logger.error("File not found: " + path);
    return TQ_Error::FileNotFound;
  }

  json j;
  try {
    file >> j;
  } catch (...) {
    logger.error("JSON parse error: " + path);
    return TQ_Error::ParseError;
  }

  std::unordered_set<std::string> seen_names;
  std::unordered_set<int> used_pre, used_mid, used_post;

  auto assign_order = [&](int desired, auto &used) {
    while (used.find(desired) != used.end())
      ++desired;
    used.insert(desired);
    return desired;
  };

  if (!j.is_array()) {
    logger.error("JSON root is not an array: " + path);
    return TQ_Error::InvalidFormat;
  }

  for (const auto &item : j) {
    Task t;
    if (!parse_task_json(item, t)) {
      logger.warn("Invalid task object in JSON");
      continue;
    }

    if (t.name.empty()) {
      logger.warn("Task with empty name ignored");
      continue;
    }

    if (!seen_names.insert(t.name).second) {
      logger.warn("Duplicate task ignored: " + t.name);
      continue;
    }

    if (t.type == "pre") {
      t.order = assign_order(t.order, used_pre);
      out.pre.push(t);
    } else if (t.type == "middle") {
      t.order = assign_order(t.order, used_mid);
      out.middle.push(t);
    } else if (t.type == "post") {
      t.order = assign_order(t.order, used_post);
      out.post.push(t);
    } else {
      logger.warn("Unknown type: " + t.type);
      return TQ_Error::UnknownType;
    }
  }

  return TQ_Error::Ok;
}
