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

#ifndef HIKARIBACKEND_REQUEST_HPP
#define HIKARIBACKEND_REQUEST_HPP

#include <string>
#include <map>

namespace RickyCorte
{
namespace Http
{
    /**
     * An incoming http request
     */
    class Request
    {
    public:
        Request(const char *req_string, size_t len);

        /**
         * Request types supported
         */
        enum RequestType {GET, POST, PUT, DELETE }; /* we don't support all methods right now */

        /**
         * Returns true if the request is valid
         * @return
         */
        bool IsValid();

        /**
         * Returns request type
         * eg: POST
         * @return
         */
        RequestType GetType();

        /**
         * Return url path
         * example: www.test.com/home returns /home
         * @return
         */
        std::string GetPath();

        /**
         * Return header option value if present
         * If missing return ""
         *
         * @param option option name
         * @return
         */
        std::string GetHeader(const std::string& option);


        /**
         * Return request body if any
         *
         * @return
         */
        std::string GetBody();

        virtual ~Request();

    private:
        /**
         * Parse first line of http header
         *
         * @param line_start
         * @return
         */
        inline bool parse_first_header_line(char *line_start);

        /**
         * Parse a header option line
         * @param line_start
         * @return
         */
        inline bool parse_header_option(char *line_start);

        char *_request_string;
        size_t _request_size;

        char *_path;
        RequestType _req_type;
        std::map<std::string, char *> _header_options;
        char *_body;

        bool _is_valid;

    };

}
}

#endif //HIKARIBACKEND_REQUEST_HPP
