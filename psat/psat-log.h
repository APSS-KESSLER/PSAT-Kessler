#ifndef PSAT_LOG_H
#define PSAT_LOG_H

#define LOG_ERROR(module, log) Serial.println("[Error] " module ": " log)
#define LOG_INFO(module, log) Serial.println("[Info] " module ": " log)

#define LOG_ERROR_F(module, log, ...) Serial.printf("[Error] " module ": " log "\n", __VA_ARGS__)
#define LOG_INFO_F(module, log, ...) Serial.printf("[Info] " module ": " log "\n", __VA_ARGS__)

#define LOG_ERROR_P(module) Serial.print("[Error] " module ": ")
#define LOG_INFO_P(module) Serial.print("[Info] " module ": ")

#endif