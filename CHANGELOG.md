# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.2.0] - 2026-02-11

### Added

- **Multi-Format Configuration**: Load tasks seamlessly from **JSON** or **INI** files.
- **Three-Stage Workflow**: Independent queues for `pre`, `middle`, and `post` processing stages.
- **Thread-Safety**: Built-in thread-safe priority queues for concurrent consumer access.
- **Conflict Resolution**: Automatic order adjustment for conflicting priorities.
- **Uniqueness**: Enforces unique task names (first declaration wins).
- **Flexible Logging**: Customizable logging callbacks for integration with existing logging frameworks.
- **Dual API**: Full C++23 API and a C interop layer (FFI-ready).
- **Modern Build System**: CMake 3.28+ with `pkg-config` and `find_package` support.
