/*
 * Hikari Backend BENCHMARK
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

#include <chrono>
#include <iostream>
#include <cstring>

#include "http/Request.hpp"

#define REQUEST_TO_DO 1000000

const char *TEST_REQUEST =     "POST /joyent/http-parser HTTP/1.1\r\n"
                               "Host: github.com\r\n"
                               "DNT: 1\r\n"
                               "Accept-Encoding: gzip, deflate, sdch\r\n"
                               "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4\r\n"
                               "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) "
                               "AppleWebKit/537.36 (KHTML, like Gecko) "
                               "Chrome/39.0.2171.65 Safari/537.36\r\n"
                               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,"
                               "image/webp,*/*;q=0.8\r\n"
                               "Referer: https://github.com/rickycorte/HikariBackend\r\n"
                               "Connection: keep-alive\r\n"
                               "Transfer-Encoding: chunked\r\n"
                               "Cache-Control: max-age=0\r\n\r\nb\r\nhello world\r\n0\r\n";

int main()
{
    using namespace std::chrono;
    long total_time = 0;

    size_t data_len = strlen(TEST_REQUEST);

    std::cout<<"Performing " << REQUEST_TO_DO << " requests of " << data_len << " bytes\n";

    for(int i =0; i< REQUEST_TO_DO; i++)
    {
        high_resolution_clock::time_point start_tm = high_resolution_clock::now();

        RickyCorte::Http::Request(TEST_REQUEST, data_len);

        auto duration = duration_cast<microseconds>( high_resolution_clock::now() - start_tm ).count();
        total_time += duration;
    }
    std::cout << "RESULTS "<<"( "<< total_time/(double)1000000 <<"s ):\n";
    std::cout << "Avg: " << total_time/(double)REQUEST_TO_DO
              << "us | " << REQUEST_TO_DO/(double)total_time * 1000000 << " req/sec";



    return 0;
}