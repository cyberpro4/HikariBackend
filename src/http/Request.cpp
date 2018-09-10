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
        _request_string[len] = '\0';                        // add null terminator to string


        int line_number = 0;
        char *previous_line_start = _request_string;
        int last_two_dots = -1;

        for(size_t i = 0; i < _request_size; i++)
        {
            if(_request_string[i] == '\0')                  // unexpected end of line in the middle of the request
            {
                _header_options.clear();
                RC_ERROR("[HTTP] Found unexpected \\0");
                return;
            }

            // remember last ':', we need this to keep the options parse fast
            // we take a bit more time to prevent false positives like "Host: localhost::8080"
            if(_request_string[i] == ':' && _request_size >  i+1 && _request_string[i+1] == ' ')
                last_two_dots = i;

            // we found a new line that could be followed by another header or by header terminators

            if(_request_string[i] == '\n')
            {
                // we check that the previous item is a \r because we replaced it earlier
                if(i + 2 > _request_size || i < 1 || _request_string[i - 1] != '\r')
                {
                    _header_options.clear();                // ops, out of range in headers
                    RC_ERROR("[HTTP] Broken end of line formatting on line ", line_number);
                    return;
                }

                _request_string[i-1] = '\0';                // replace \r with \0

                // now we have two possibilities
                if(_request_string[i+1] != '\r')            // new header line because next character is not a terminator
                {
                    // chose parser based on line number
                    if(line_number == 0)
                    {
                        // check that the first line of header is valid
                        if (!parse_first_header_line(previous_line_start))
                        {
                            RC_ERROR("[HTTP] Broken top line");
                            return;
                        }
                    }
                    else
                    {
                        //check if ':' is in the current line
                        if(last_two_dots >= 0 && previous_line_start - _request_string < last_two_dots)
                        {
                            _request_string[last_two_dots] = '\0'; // add terminator to split option name and value
                            // add header option to map
                            _header_options[std::string(previous_line_start)] = _request_string + last_two_dots + 2; // skip space after :
                        }
                        else
                        {
                            _header_options.clear();
                            RC_ERROR("[HTTP] Broken header option");
                            return;                        // broken header option
                        }
                    }

                    line_number++;
                    previous_line_start = _request_string + i + 1;

                }
                else if(_request_string[i+2] == '\n')       // we found body because we a double new line terminator!
                {
                    if(i + 3 < _request_size)               // range check before doing a dangerous operation
                        _body = _request_string + i + 3;    // (+3 -> 2 terminators and 1 body element)
                    break;                                  // stop parse, we found everything
                }

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
        char *type = nullptr, *protocol = nullptr;
        type = line_start;
        size_t len = strlen(line_start);

        size_t space_count = 0;
        for(size_t i = 0; i < len; i++)
        {
            if(line_start[i] == ' ')
            {
                space_count++;
                line_start[i] = '\0';       // replace space with terminator so we can grab individual parts
                if(space_count == 1)
                    _path = line_start + i + 1;
                if(space_count == 2)
                    protocol = line_start + i + 1;
                // the first line can't contain more than 3 spaces
                if(space_count > 2 )
                {
                    _path = nullptr;
                    return false;
                }
            }
        }

        if(strcmp(type, "GET") == 0) _req_type = RequestType::GET;
        else if(strcmp(type, "POST") == 0) _req_type = RequestType::POST;
        else if(strcmp(type, "PUT") == 0) _req_type = RequestType::PUT;
        else if(strcmp(type, "DELETE") == 0) _req_type = RequestType::DELETE;
        else return false; // broken protocol

        // we only support http 1.1
        return type && _path && protocol && (strcmp(protocol, "HTTP/1.1") == 0);
    }


    const Request::HeaderOptions &Request::GetHeaderOptions()
    {
        return _header_options;
    }


}