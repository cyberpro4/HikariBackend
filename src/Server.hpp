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

#ifndef HIKARIBACKEND_SERVER_HPP
#define HIKARIBACKEND_SERVER_HPP

#include "GlobalStaticConfig.hpp"

#include <sys/epoll.h>

namespace RickyCorte
{
        class Server
        {

        public:
            /**
             * Initialize server but NOT run it
             * Use Run() to start server loop
             * @param bind_port
             */
            Server(int bind_port = DEFAULT_SERVER_PORT);

            /**
             * Run server loop on this thread
             */
            void Run();


            /**
             * Stop server loop
             */
            void Dispose();

        private:

            /**
             * Bind server to specified port
             *
             * @param port server_port
             * @return listen socket fd (not blocking)
             */
            int bind_server_port(int port);

            /**
             * Accept a new connection pending at server_fd
             *
             * @param server_fd listen socket
             * @return new client descriptor
             */
            int accept_connection(int server_fd);

            /**
             * Check for error in event and close if any problem is found
             * @param event event to check
             * @return true if error occurred
             */
            bool check_epoll_error(epoll_event *event);


            /**
             * Handle a event on a socket
             * @param event
             */
            void handle_socket_event(epoll_event* event);


            /**
             * Add socket to epoll watcher
             * @param target_socket socket to add to epoll
             * @param epoll_fd epoll descriptor
             * @return false on error
             */
            bool add_to_epoll(int target_socket, int epoll_fd);

            int _server_port;
            bool _exit_loop;

            int _epoll_fd;
            int _server_fd;

        };
}


#endif //HIKARIBACKEND_SERVER_HPP
