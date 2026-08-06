#ifndef LOGGER_H
#define LOGGER_H

#include <types.h>

typedef enum
{
    LOG_WARNING,
    LOG_ERROR  ,
    LOG_INFO   ,
} LogType;

void L_VLog(LogType type, const char* const message, va_list vaList);
void L_Log(LogType type, const char* const message, ...);
void L_LogWarning(const char* const message, ...);
void L_LogError(const char* const message, ...);
void L_LogInfo(const char* const message, ...);

#endif