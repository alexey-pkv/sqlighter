# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build/Test Commands
- Build: `cmake -S . -B build/debug && cmake --build build/debug`
- Run tests: `cmake --build build/debug --target test -- -j10`
- Run single test: `cd build/debug/bin && ./sqlighter_tests --gtest_filter="TestName.TestCase"`
- Run with coverage: `./cover.sh`
- Check for memory leaks: `./leaks.sh`

## Code Style Guidelines
- Use C++17 standard
- Tab indentation; braces on new lines for function/class definitions
- Class members prefixed with `m_` (e.g., `m_path`)
- Class structure: private members first, then public methods
- Headers: Use classic #ifndef guards with format `SQLIGHTER_<PATH_TO_FILE_IN_UPPER_SNAKE_CASE>_<CLASS_NAME_IN_UPPERCASE>_H`
  Example:
  ```cpp
  #ifndef SQLIGHTER_CORE_BINDVALUE_H
  #define SQLIGHTER_CORE_BINDVALUE_H
  
  // Code here
  
  #endif
  ```
- Exceptions inherit from SQLighterException with specific error codes
- RAII pattern for resources (e.g., DB connections)
- Use `[[nodiscard]]` for methods returning values that shouldn't be ignored
- Default/delete appropriate constructors and assignment operators
- Use `std::string_view` for string parameters not requiring ownership
- Group includes: standard library first, then external, then project headers
- Test naming convention: `TEST(ClassName, method_name__scenario)`
