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

#include "config/ConfigFile.hpp"

#include <fstream>
#include <streambuf>
#include <sstream>

#include <rickycorte/Logging.hpp>

using namespace RickyCorte;


ConfigFile::ConfigFile(const std::string &cfg_file)
{
    _file_name = cfg_file;

    RC_INFO("[Config Load] Loading config: ", cfg_file);
    _json = loadJsonFromFile(cfg_file);
    RC_INFO("[Config Load] Done");
}


std::string ConfigFile::Get(const std::string &key)
{
    return Get(key, "");
}


std::string ConfigFile::Get(const std::string &key, const std::string default_value)
{
    try
    {
        return _json[nlohmann::json::json_pointer("/"+key)];
    }
    catch (...)
    {
        RC_DEBUG("Unable to read json key: ", key, " of ", _file_name);
    }

    Set(key,default_value);
    return default_value;
}


bool ConfigFile::Set(const std::string &key, const std::string &value)
{

    _json[nlohmann::json::json_pointer("/"+key)] = value;

    return saveJsonToFile(_file_name, _json);
}


nlohmann::json ConfigFile::loadJsonFromFile(const std::string &file_name)
{
    nlohmann::json json;

    std::ifstream is(file_name);
    if(is)
    {
        std::string content( (std::istreambuf_iterator<char>(is)),std::istreambuf_iterator<char>());
        json = nlohmann::json::parse(content);
    }
    else
    {
        RC_INFO("[Config Load] Unable to load file: ", file_name);
        return json;
    }

    is.close();

    return json;
}


bool ConfigFile::saveJsonToFile(const std::string& file_name, const nlohmann::json& json)
{
    std::ofstream os(file_name);
    if(os)
    {
        os<<json.dump();
    }
    else
    {
        RC_ERROR("[Config Save] Unable to open file: ", file_name);
        return false;
    }

    if(!os)
    {
        RC_ERROR("[Config Save] Unable to write file: ", file_name);
        return false;
    }

    os.close();

    return true;
}