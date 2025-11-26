## Introduction

The LLM-generated C code and unit tests are based on the proposed dataset for evaluating LLM-generated code, developed for the research paper *"Security and Quality in LLM-Generated Code: A Multi-Language, Multi-Model Analysis"*.

---

## Prerequisites

Before running the compiler and SonarQube scan, make sure the following are installed:

- [Python 3.12+](https://www.python.org/downloads/)
- [CMake](https://cmake.org/)
- [SonarQube Scanner](https://docs.sonarqube.org/latest/analysis/scan/sonarscanner/)
- [SonarSource Build Wrapper](https://docs.sonarsource.com/cpp-analysis/latest/)

---

## Configure Compiler Settings

Update the following in your `compiler.py` file:

```python
logfile = "claude_syntax_and_compliation_cpp_log.log"  # Replace with the path to your log file
source_directory = "./source/output/CLAUDE_claude-3-5-sonnet-20240620/cpp"  # Replace with your source directory
output_directory = "./out"  # Replace with your output directory

## Run sonarqube scanning:

build-wrapper-linux-x86-64 --out-dir bw-output cmake --build cmake-build-debug
cd bw-output
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cd ..
sonar-scanner
