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

#include <iostream>

#include <rickycorte/Logging.hpp>

#include "global/GlobalStaticConfig.hpp"
#include "Server.hpp"


void printStartHeader()
{
    std::cout << HEADER_DISPLAY_NAME << std::endl << " Release "
        << HIKARI_VERSION_MAJOR << "."
        << HIKARI_VERSION_MINOR << "."
        << HIKARI_VERSION_PATCH
        << std::endl << std::endl;
}

int main()
{
    using namespace RickyCorte;

    printStartHeader();


    Server server;
    server.Run();


    return 0;
}