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

#ifndef HIKARIBACKEND_CONFIGFILE_H
#define HIKARIBACKEND_CONFIGFILE_H

#include <string>

#include <nlohmann/json.hpp>

namespace RickyCorte
{
/**
 * Represents a json config file
 * This is a helper class the enables fast and easy config load/save
 *
 * NO THREAD SAFE
 */
    class ConfigFile
    {
    public:

        /**
         * Create a config file instance that loads/saves the specified file
         *
         * @param cfg_file file to open
         */
        ConfigFile(const std::string &cfg_file);


        /**
         * Get the value of the specified key
         *
         * @param key key to search
         * @return key value if found or ""
         */
        std::string Get(const std::string &key);


        /**
         * Get the value of the specified key, if missing creates a new one with the default value
         *
         * @param key key to search
         * @param default_value default key value
         * @return key value if found, default_value if not found
         */
        std::string Get(const std::string &key, const std::string default_value);


        /**
         * Edit key value and save changes to file
         *
         * @param key key to edit
         * @param value new key value
         * @return false on error
         */
        bool Set(const std::string &key, const std::string &value);

    private:

        /**
         * Loads json data from file
         * In case of errors returns a empty json
         *
         * @param file_name file to load
         * @return json loaded from file, empty object on errors
         */
        nlohmann::json loadJsonFromFile(const std::string& file_name);


        /**
         * Saves json to a file
         *
         * @param file_name file to save
         * @param json json to save
         * @return false on error
         */
        bool saveJsonToFile(const std::string& file_name, const nlohmann::json& json);

        std::string _file_name;
        nlohmann::json _json;

    };
}


#endif //HIKARIBACKEND_CONFIGFILE_H
