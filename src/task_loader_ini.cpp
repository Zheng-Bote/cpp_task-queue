/**
 * SPDX-FileComment: INI Task Loader implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file task_loader_ini.cpp
 * @brief Implements loading of tasks from INI files.
 * @version 0.1.0
 * @date 2026-02-10
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

// src/task_loader_ini.cpp
#include "task_loader.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

/**
 * @brief Trims leading and trailing whitespace from a string.
 *
 * @param s The string to trim.
 * @return std::string The trimmed string.
 */
static std::string trim(const std::string &s) {
  size_t l = 0;
  while (l < s.size() && std::isspace(static_cast<unsigned char>(s[l])))
    ++l;
  if (l == s.size())
    return "";
  size_t r = s.size() - 1;
  while (r > l && std::isspace(static_cast<unsigned char>(s[r])))
    --r;
  return s.substr(l, r - l + 1);
}

/**
 * @brief Parses an attribute line (key=value,key=value).
 *
 * @param line The line containing attributes.
 * @return std::unordered_map<std::string, std::string> Map of key-value pairs.
 */
static std::unordered_map<std::string, std::string>
parse_attr_line(const std::string &line) {
  std::unordered_map<std::string, std::string> out;
  std::string token;
  std::istringstream ss(line);
  while (std::getline(ss, token, ',')) {
    auto pos = token.find('=');
    if (pos == std::string::npos)
      continue;
    std::string k = trim(token.substr(0, pos));
    std::string v = trim(token.substr(pos + 1));
    if (!k.empty())
      out.emplace(std::move(k), std::move(v));
  }
  return out;
}

/**
 * @brief Loads task queues from an INI file.
 * @see load_task_queues_from_ini
 */
TQ_Error load_task_queues_from_ini(const std::string &path, TaskQueues &out,
                                   const TQ_Logger &logger) {
  std::ifstream ifs(path);
  if (!ifs) {
    logger.error("INI file not found or unreadable: " + path);
    return TQ_Error::FileNotFound;
  }

  std::string line;
  std::string current_section;
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
      sections;

  size_t lineno = 0;
  while (std::getline(ifs, line)) {
    ++lineno;
    std::string s = trim(line);
    if (s.empty())
      continue;
    if (s[0] == ';' || s[0] == '#')
      continue;

    if (s.front() == '[' && s.back() == ']') {
      current_section = trim(s.substr(1, s.size() - 2));
      continue;
    }

    auto eq = s.find('=');
    if (eq == std::string::npos) {
      logger.warn("Skipping malformed line " + std::to_string(lineno) + " in " +
                  path);
      continue;
    }

    std::string key = trim(s.substr(0, eq));
    std::string value = trim(s.substr(eq + 1));
    if (current_section.empty()) {
      logger.warn("Key outside section at line " + std::to_string(lineno) +
                  ": " + key);
      continue;
    }
    sections[current_section][key] = value;
  }

  std::unordered_set<std::string> seen_names;
  std::unordered_set<int> used_pre, used_mid, used_post;

  auto assign_order = [&](int desired, auto &used) {
    while (used.find(desired) != used.end())
      ++desired;
    used.insert(desired);
    return desired;
  };

  const std::string prefix = "task:";
  for (const auto &sec_pair : sections) {
    const std::string &sec = sec_pair.first;
    if (sec.rfind(prefix, 0) != 0) {
      logger.warn("Skipping non-task section: " + sec);
      continue;
    }
    std::string name = trim(sec.substr(prefix.size()));
    if (name.empty()) {
      logger.warn("Empty task name in section: " + sec);
      continue;
    }
    if (!seen_names.insert(name).second) {
      logger.warn("Duplicate task name ignored: " + name);
      continue;
    }

    const auto &kv = sec_pair.second;
    Task t;
    t.name = name;
    auto it = kv.find("type");
    t.type = (it != kv.end()) ? trim(it->second) : std::string();
    it = kv.find("input");
    t.input = (it != kv.end()) ? trim(it->second) : std::string();
    it = kv.find("output");
    t.output = (it != kv.end()) ? trim(it->second) : std::string();
    it = kv.find("order");
    if (it != kv.end()) {
      try {
        t.order = std::stoi(trim(it->second));
      } catch (...) {
        t.order = 0;
      }
    } else {
      t.order = 0;
    }

    it = kv.find("attr");
    if (it != kv.end() && !trim(it->second).empty()) {
      t.attributes = parse_attr_line(it->second);
    }

    if (t.type == "pre") {
      t.order = assign_order(t.order, used_pre);
      out.pre.push(std::move(t));
    } else if (t.type == "middle") {
      t.order = assign_order(t.order, used_mid);
      out.middle.push(std::move(t));
    } else if (t.type == "post") {
      t.order = assign_order(t.order, used_post);
      out.post.push(std::move(t));
    } else {
      logger.warn("Unknown type for task " + t.name + ": " + t.type +
                  " — skipping task");
      continue;
    }
  }

  return TQ_Error::Ok;
}
