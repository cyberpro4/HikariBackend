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

#include "Server.hpp"

#include <rickycorte/Logging.hpp>

#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

#include "http/Reply.hpp"


namespace RickyCorte
{

    Server::Server(int bind_port)
    :_server_port{bind_port}, _exit_loop{false}
    {
        _epoll_fd = epoll_create1(0);
        if(_epoll_fd == -1)
        {
            RC_CRITICAL("Can't create epoll file descriptor");
            exit(1);
        }
    }

    void Server::Run()
    {
        _server_fd = bind_server_port(_server_port);

        RC_INFO("Running http server on port: ", _server_port);

        struct epoll_event *events = (epoll_event *)calloc(EPOLL_MAX_EVENTS, sizeof(epoll_event));

        if(listen(_server_fd, MAX_CONNECTIONS))
        {
            RC_CRITICAL("Unable to listen to server socket");
            exit(4);
        }


        if(!add_to_epoll(_server_fd, _epoll_fd))
        {
            RC_CRITICAL("Unable to add server fd to epoll");
            exit(5);
        }


        while (!_exit_loop)
        {
            int ev_count = epoll_wait(_epoll_fd, events, EPOLL_MAX_EVENTS, 1000);
            for(int i = 0; i < ev_count; i++)
            {
                if(check_epoll_error( &events[i] )) continue; //skip iteration on error

                //check for new pending connections
                if(events[i].data.fd == _server_fd)
                {
                    //accept all new connections
                    int new_conn = -1;
                    do
                    {
                        new_conn = accept_connection(_server_fd);
                        if(new_conn > -1 && !add_to_epoll(new_conn, _epoll_fd))
                        {
                            RC_WARNING("Unable to add connection ", new_conn," to epoll, ignoring it");
                            close(new_conn);
                        }
                    }
                    while(new_conn != -1);
                }
                else
                {
                    handle_socket_event(&events[i]);
                }

            }
        }

        free(events);
    }


    void Server::Dispose()
    {
        close(_epoll_fd);
        close(_server_fd);
    }


    int Server::bind_server_port(int port)
    {
        struct sockaddr_in reciver;
        memset(&reciver, 0, sizeof(reciver));
        reciver.sin_family = AF_INET;
        reciver.sin_port = htons(port);

        //create listen socket
        int listenSocket = socket(reciver.sin_family, SOCK_STREAM , 0);

        int sockOtp = 1;
        //enable socket reuse before os timeout
        setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &sockOtp, sizeof(sockOtp));

        if( bind(listenSocket, (struct sockaddr *)&reciver , sizeof(reciver) ) < 0)
        {
            RC_CRITICAL("Unable to bind server on port: ", port);
            exit(2);
        }

        //set notblocking
        if(fcntl(listenSocket, F_SETFL, O_NONBLOCK))
        {
            RC_CRITICAL("Can't set listen socket to nonblock mode");
            exit(3);
        }

        return listenSocket;
    }


    int Server::accept_connection(int server_fd)
    {
        struct sockaddr in_addr;
        socklen_t in_len;

        int new_conn = accept4(server_fd, &in_addr, &in_len, SOCK_NONBLOCK);

        //print new connection informations
        if(new_conn != -1)
        {
            char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
            if(getnameinfo(&in_addr, in_len, hbuf, sizeof hbuf, sbuf, sizeof sbuf, NI_NUMERICHOST | NI_NUMERICSERV) == 0)
            {
                RC_INFO("Accepted connection ", new_conn, " from ", hbuf,":",sbuf);
            }
        }

        return new_conn;
    }


    bool Server::check_epoll_error(epoll_event *event)
    {
        if(!event) return true;

        if ((event->events & EPOLLERR) || (event->events & EPOLLHUP) ||
            (!(event->events & EPOLLIN)))
        {
            // An error has occured on this fd
            RC_ERROR("Error in socket fd: ", event->data.fd, ". Error type: ",event->events, " Error number: ", errno);
            fprintf(stderr, "epoll error. events=%u\n", event->events);
            close(event->data.fd);
            return true;
        }

        if(event->events & EPOLLRDHUP)
        {
            RC_WARNING("Detected socket close on fd: ", event->data.fd, ". Closing socket");
            return true;
        }

        return false;
    }


    void Server::handle_socket_event(epoll_event *event)
    {

        std::string req_string;
        char *buffer = new char[READ_BUFFER_SIZE];
        if(!buffer)
        {
            RC_CRITICAL("Unable to allocate read buffer!");
            return;
        }

        //read all available data
        int read_sz = -1, ready_to_read_sz = -1;
        do
        {
            memset(buffer, '\0', READ_BUFFER_SIZE);
            read_sz = read(event->data.fd, buffer, READ_BUFFER_SIZE -1);

            //check if socket has been closed
            if(read_sz == 0)
            {
                RC_INFO("Socket ", event->data.fd, " has been closed");
                close(event->data.fd);
                return;
            }

            ioctl(event->data.fd, FIONREAD, &ready_to_read_sz);

            if(read_sz > 0) req_string += buffer;

        }
        while(ready_to_read_sz > 0 && read_sz > 0);

        //RC_INFO("Read ", req_string.size(), " from ", event->data.fd);
        req_string = Http::Reply(200, req_string).Dump();


        write(event->data.fd, req_string.c_str(), req_string.size());
        //RC_INFO("Write ", req_string.size(), " to ", event->data.fd);

        //TODO: sto coso lo mandiamo in timeout al posto di chiuderlo eh :3
        //close(event->data.fd);

        delete[] buffer;
        //RC_INFO("Closed: ", event->data.fd);
    }


    bool Server::add_to_epoll(int target_socket, int epoll_fd)
    {
        epoll_event event;

        event.data.fd = target_socket;
        event.events = EPOLLIN | EPOLLET;

        return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, target_socket, &event) == 0;
    }

}