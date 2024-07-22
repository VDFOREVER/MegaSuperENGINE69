#pragma once

#include <tinyformat.h>
#include <iostream>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BOLDRED "\033[1m\033[31m"

#define LOG_INFO(...)       std::cout << GREEN << "[INFO] " << RESET << tfm::format(__VA_ARGS__) << std::endl;
#define LOG_WARN(...)       std::cout << YELLOW << "[WARN] " << RESET << tfm::format(__VA_ARGS__) << std::endl;
#define LOG_ERROR(...)      std::cout << RED << "[ERROR] " << RESET << tfm::format(__VA_ARGS__) << std::endl;
#define LOG_CRITICAL(...)   std::cout << BOLDRED << "[CRITICAL] " << RESET << tfm::format(__VA_ARGS__) << std::endl;
#define LOG_FATAL(...)   std::cout << BOLDRED << "[FATAL] " << RESET << tfm::format(__VA_ARGS__) << std::endl;