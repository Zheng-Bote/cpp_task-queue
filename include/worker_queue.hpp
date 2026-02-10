/**
 * SPDX-FileComment: Thread-safe worker queue
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file worker_queue.hpp
 * @brief Defines a thread-safe priority queue for tasks.
 * @version 0.1.0
 * @date 2026-02-10
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once
#include "task.hpp"
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

/**
 * @brief Comparator for prioritizing tasks based on order.
 */
struct TaskComparator {
  /**
   * @brief Compares two tasks.
   *
   * @param a First task.
   * @param b Second task.
   * @return true if a.order > b.order (higher order value means lower
   * priority/later execution).
   * @return false otherwise.
   */
  bool operator()(const Task &a, const Task &b) const noexcept {
    return a.order > b.order;
  }
};

/**
 * @brief Thread-safe queue for managing tasks.
 */
class WorkerQueue {
public:
  /**
   * @brief Pushes a task into the queue.
   *
   * @param t The task to push.
   */
  void push(Task t) {
    {
      std::lock_guard lock(m_);
      q_.push(std::move(t));
    }
    cv_.notify_one();
  }

  /**
   * @brief Pops a task from the queue, blocking if empty until a task is
   * available or the queue is done.
   *
   * @return std::optional<Task> The popped task, or std::nullopt if the queue
   * is done and empty.
   */
  std::optional<Task> pop() {
    std::unique_lock lock(m_);
    cv_.wait(lock, [&] { return done_ || !q_.empty(); });
    if (q_.empty())
      return std::nullopt;
    Task t = q_.top();
    q_.pop();
    return t;
  }

  /**
   * @brief Tries to pop a task from the queue without blocking.
   *
   * @return std::optional<Task> The popped task, or std::nullopt if the queue
   * is empty.
   */
  std::optional<Task> try_pop() {
    std::lock_guard lock(m_);
    if (q_.empty())
      return std::nullopt;
    Task t = q_.top();
    q_.pop();
    return t;
  }

  /**
   * @brief Signals that no more tasks will be pushed to the queue.
   *
   * Wakes up all waiting threads.
   */
  void set_done() {
    {
      std::lock_guard lock(m_);
      done_ = true;
    }
    cv_.notify_all();
  }

  /**
   * @brief Returns the number of tasks in the queue.
   *
   * @return size_t The number of tasks.
   */
  size_t size() const {
    std::lock_guard lock(m_);
    return q_.size();
  }

private:
  std::priority_queue<Task, std::vector<Task>, TaskComparator> q_;
  mutable std::mutex m_;
  std::condition_variable cv_;
  bool done_ = false;
};
