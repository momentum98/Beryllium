#include "logger.h"

#include <stdio.h>
#include <stdarg.h>

void L_VLog(LogType type, const char* const message, va_list vaList)
{
    const char* prefix = "";

    switch (type) 
    {
        case LOG_INFO:
            prefix = "INFO";
            break;

        case LOG_ERROR:
            prefix = "ERROR";
            break;

        case LOG_WARNING:
            prefix = "WARNING";
            break;

        default:
            break;
    }
    
    char buffer[256];

    snprintf(buffer, sizeof(buffer), "[%s] %s\n", prefix, message);
    vprintf(buffer, vaList);
}

void L_Log(LogType type, const char* const message, ...)
{
    va_list vaList;
    va_start(vaList, message);

    L_VLog(type, message, vaList);

    va_end(vaList);
}

void L_LogInfo(const char* const message, ...)
{
    va_list vaList;
    va_start(vaList, message);

    L_VLog(LOG_INFO, message, vaList);

    va_end(vaList);
}

void L_LogError(const char* const message, ...)
{
    va_list vaList;
    va_start(vaList, message);

    L_VLog(LOG_ERROR, message, vaList);

    va_end(vaList);
}

void L_LogWarning(const char* const message, ...)
{
    va_list vaList;
    va_start(vaList, message);

    L_VLog(LOG_WARNING, message, vaList);

    va_end(vaList);
}