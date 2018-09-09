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

#include "Request.hpp"

#include <cstring>

#include <rickycorte/Logging.hpp>

namespace RickyCorte::Http
{


    Request::Request(const char *req_string, size_t len)
            : _request_string{nullptr}, _path{nullptr},_body{nullptr}, _is_valid{false}, _request_size{len + 1}
    {
        _request_string = new char[_request_size];
        if(!_request_string)
        {
            RC_ERROR("Unable to allocate http request internal data");
            return;
        }
        memcpy(_request_string, req_string, len);
        _request_string[len] = '\0';                // add null terminator to string


        int line_numeber = 0;
        char *previous_line_start = _request_string;

        for(size_t i = 0; i < _request_size; i++)
        {
            if(_request_string[i] == '\0')                  // unexpected end of line in the middle of the request
            {
                _header_options.clear();
                return;
            }

            if(_request_string[i] == '\r')                  // we don't care about \r's
            {
                _request_string[i] = '\0';                  // we use them to understand where a line terminates
                continue;
            }


            // we found a new line that could be followed by another header or by header terminators
            if(_request_string[i] == '\n' && i + 2 > _request_size)
            {

                // now we have two possibilities
                if(_request_string[i+1] != '\r')            // new header line because next character is not a terminator
                {
                    // chose parser based on line number
                    if(line_numeber == 0)
                    {
                        // check that the first line of header is valid
                        if (!parse_first_header_line(previous_line_start))
                        {
                            return;
                        }
                    }
                    else
                        parse_header_option(previous_line_start);

                    line_numeber++;
                    previous_line_start = _request_string + i + 1;

                }
                else if(_request_string[i+2] == '\n')       // we found body because we a double new line terminator!
                {
                    if(i + 3 < _request_size)               // range check before doing a dangerous operation
                        _body = _request_string + i + 3;    // (+3 -> 2 terminators and 1 body element)
                    break;                                  // stop parse, we found everything
                }

            }
            else
            {
                _header_options.clear();                    // ops, out of range in headers
                return;
            }


        }

        _is_valid = true;
    }

    bool Request::IsValid()
    {
        return _is_valid;
    }

    Request::RequestType Request::GetType()
    {
        return _req_type;
    }

    std::string Request::GetPath()
    {
        if(!_path) return "";

        return std::string(_path);
    }

    std::string Request::GetHeader(const std::string &option)
    {
        if(_header_options.find(option) != _header_options.end())
        {
            return std::string(_header_options[option]);
        }

        return "";
    }


    std::string Request::GetBody()
    {
        if(!_body) return "";

        return std::string(_body);
    }

    Request::~Request()
    {
        if(_request_string) delete[] _request_string;
    }

    bool Request::parse_first_header_line(char *line_start)
    {
        RC_DEBUG("Line 1: ", line_start);
        return true;
    }

    bool Request::parse_header_option(char *line_start)
    {
        RC_DEBUG("Option: ", line_start);
        return false;
    }


}