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

#ifndef AUTOMATION1_CONTROLLER_H
#define AUTOMATION1_CONTROLLER_H

#include <tango/tango.h>


namespace Controller_ns {

    class Controller final : public TANGO_BASE_CLASS {
    public:
        std::string ip_address {"127.0.0.1"};
        Tango::DevString *attr_api_version_read{};
        Tango::DevShort *attr_available_axis_count_read{};
        Tango::DevShort *attr_available_task_count_read{};
        Tango::DevBoolean *attr_is_running_read{};

        Controller(Tango::DeviceClass *cl, const std::string &s);

        Controller(Tango::DeviceClass *cl, const char *s);

        Controller(Tango::DeviceClass *cl, const char *s, const char *d);

        ~Controller() override;

        void delete_device() override;

        void init_device() override;

        void get_device_property();

        void always_executed_hook() override;

        void read_attr_hardware(std::vector<long> &attr_list) override;

        void write_attr_hardware(std::vector<long> &attr_list) override;

        void read_is_running(Tango::Attribute &attr);

        static void add_dynamic_attributes();

        void connect() ;

        static void add_dynamic_commands();

        void available_axis_count( Tango::Attribute & att);

        void read_api_version( Tango::Attribute & att) const;

        void available_task_count( Tango::Attribute & att) const;

    };

}
#endif