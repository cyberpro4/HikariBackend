/*
 * Hikari Backend
 *
 * Copyright (C) 2018 RickyCorte
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef HIKARIBACKEND_LOGGING_H
#define HIKARIBACKEND_LOGGING_H

#include <iostream>

namespace RickyCorte
{
    /**
    * Write base overload, writes to stdout the parameter t
    */
    template <typename T>
    static inline void write(const T& t) {
        std::cout << t << std::endl;
    }

    /**
    * Recursively writes to stdout the parameter list
    * No formatting is applied to parameters, they are placed in the submit order without separators
    */
    template <typename T, typename... Targs>
    static inline void write(const T& el, const Targs&... args)
    {
        std::cout << el;
        write(args...);
    }

    /**
    * Recursively writes to stdout the parameter list with prefix and newline (1 per call)
    * No formatting is applied to parameters, they are placed in the submit order without separators
    */
    template <typename... Targs>
    static inline void console_message(const std::string& prefix, Targs... args)
    {
        std::cout << prefix << " ";
        write(args...);
    }
}

template <typename... Targs>
inline void RC_DEBUG(Targs... args)
{
#ifndef SHOW_DEBUG
    RickyCorte::console_message("[DEBUG]", args...);
#endif // !SHOW_DEBUG
}

template <typename... Targs>
inline void RC_INFO(Targs... args)
{
#ifndef SHOW_INFO
    RickyCorte::console_message("[INFO]", args...);
#endif // !SHOW_INFO
}

template <typename... Targs>
inline void RC_WARNING(Targs... args)
{
    RickyCorte::console_message("[WARNING]", args...);
}

template <typename... Targs>
inline void RC_ERROR(Targs... args)
{
    RickyCorte::console_message("[ERROR]", args...);
}

template <typename... Targs>
inline void RC_CRITICAL(Targs... args)
{
    RickyCorte::console_message("[CRITICAL]", args...);
}

#endif //HIKARIBACKEND_LOGGING_H
