/*
The MNUI code is licensed under the terms of the GPL v3 license.
Copyright (C) 2024 Nixy·Gan

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "./mnui.h"

#if MN_USE_LOG

void _mn_log_add(int level, const char * file, uint32_t line, const char * func, const char * format, ...)
{
  if(level >= _MN_LOG_LEVEL_NUM) 
    return;

  static uint32_t last_log_time = 0;

  if(level >= MN_LOG_LEVEL)
  {
    va_list args;
    va_start(args, format);
    char msg[256];
    vsnprintf(msg, sizeof(msg), format, args);
    va_end(args);

    size_t p;
    for(p = strlen(file); p > 0; p--) {
      if(file[p] == '/' || file[p] == '\\') {
        p++;
        break;
      }
    }
    uint32_t t = mn_get_tick();
    static const char * prefix[] = {"Trace", "Info", "Warn", "Error", "User"};
    mn_log("[%s]\t(%d.%03d, +%d)\t %-30s: %-30s \t(in %-30s line #%d)\n", 
      prefix[level], t / 1000, t % 1000, t - last_log_time, func, msg, &file[p], line); 
  }
}

#endif //!MN_USE_LOG

