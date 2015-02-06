#include "errorlog.h"
#include <stdint.h>

int getDebugLevel()
{
  static int logLevel = 0;
  if (logLevel == 0) {
    if (getenv(ENV_DEBUG_LEVEL) == NULL) {
      logLevel = INFO_LEVEL;
    }
    else {
      char *level = getenv(ENV_DEBUG_LEVEL);
      if (strncmp(level, "FATAL", 5) == 0)
        logLevel = FATAL_LEVEL;
      else if (strncmp(level, "ERROR", 5) == 0)
        logLevel = ERROR_LEVEL;
      else if (strncmp(level, "WARN", 4) == 0)
        logLevel = WARN_LEVEL;
      else if (strncmp(level, "INFO", 4) == 0)
        logLevel = INFO_LEVEL;
      else
        logLevel = INFO_LEVEL;
    }
  }
  return logLevel;
}

