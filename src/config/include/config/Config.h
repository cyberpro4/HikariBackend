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

#ifndef HIKARIBACKEND_CONFIG_H
#define HIKARIBACKEND_CONFIG_H

#define GLOBAL_CONFIG_FILE "hikari.conf"

#include <string>

#include <rickycorte/Logging.h>

#include "ConfigFile.h"

namespace RickyCorte
{
    namespace Config
    {

        namespace Global
        {
            /**
             * Initialize global configuration file
             */
            void Init();


            /**
             * Get key value from global config file
             *
             * @param key key to find
             * @param default_value default key value
             * @return key value or default_value if key is missing
             */
            std::string Get(const std::string& key, const std::string& default_value);


            /**
             * Get key value from global config file
             *
             * @param key key to find
             * @return key value or "" if key is missing
             */
            std::string Get(const std::string& key);


            /**
             * Set key valeu of global config
             *
             * @param key key to set
             * @param value value to set
             * @return false on errors
             */
            bool Set(const std::string& key, const std::string& value);


            /**
             * Clean up global save instance
             */
            void Dispose();

        }
    }
}

#endif //HIKARIBACKEND_CONFIG_H
