/* ReaPack: Package manager for REAPER
 * Copyright (C) 2015-2016  Christian Fillion
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "encoding.hpp"

#ifdef _WIN32
#include <windows.h>

auto_string make_autostring(const std::string &input)
{
  const char *cstr = input.c_str();
  const int size = MultiByteToWideChar(CP_UTF8, 0, cstr, -1, nullptr, 0);

  wchar_t *output = new wchar_t[size];
  MultiByteToWideChar(CP_UTF8, 0, cstr, -1, output, size);

  auto_string string(output);
  delete[] output;

  return string;
}
#endif
