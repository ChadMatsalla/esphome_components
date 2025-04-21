#if defined(USE_HOST)
#include "monologger.h"

namespace esphome {
namespace monologger {

void HOT Monologger::write_msg_(const char *msg) {
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer, sizeof buffer, "[%H:%M:%S]", timeinfo);
  fputs(buffer, stdout);
  puts(msg);
}

void Monologger::pre_setup() { global_logger = this; }

}  // namespace logger
}  // namespace esphome

#endif
