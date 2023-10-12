/*
* Tango-Device-Server for Automation1 Aerotech Controller
 * Copyright (C) 2025  Marcus Zuber
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <tango/tango.h>

#if defined(ENABLE_CRASH_REPORT)
#  include <crashreporting/crash_report.h>
#else
#  define DECLARE_CRASH_HANDLER
#  define INSTALL_CRASH_HANDLER
#endif

DECLARE_CRASH_HANDLER

int main(const int argc, char *argv[]) {
    INSTALL_CRASH_HANDLER
    Tango::Util *tg = nullptr;
    try {
        tg = Tango::Util::init(argc, argv);
        tg->server_init(false);
        std::cout << "Ready to accept request" << std::endl;
        tg->server_run();
    } catch (std::bad_alloc &) {
        std::cout << "Can't allocate memory to store device object." << std::endl;
        std::cout << "Exiting" << std::endl;
    }
    catch (CORBA::Exception &e) {
        Tango::Except::print_exception(e);

        std::cout << "Received a CORBA_Exception" << std::endl;
        std::cout << "Exiting" << std::endl;
    }
    std::cout << "Received a CORBA_Exception." << std::endl;
    std::cout << "Exiting" << std::endl;


    if (tg) {
        tg->server_cleanup();
    }
    return 0;
}
