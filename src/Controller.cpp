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

#include "Controller.h"
#include "ControllerClass.h"
#include "Automation1.h"
#include <format>


namespace Controller_ns
{
    Controller::Controller(Tango::DeviceClass* cl, const std::string& s) : TANGO_BASE_CLASS(
        cl, s.c_str())
    {
        Controller::init_device();
    }

    Controller::Controller(Tango::DeviceClass* cl, const char* s) : TANGO_BASE_CLASS(cl, s)
    {
        Controller::init_device();
    }

    Controller::Controller(Tango::DeviceClass* cl, const char* s, const char* d) : TANGO_BASE_CLASS(cl, s, d)
    {
        Controller::init_device();
    }

    Controller::~Controller()
    {
        Controller::delete_device();
    }

    void Controller::delete_device()
    {
        DEBUG_STREAM << "Controller::delete_device() " << device_name << std::endl;
        delete attr_api_version_read;
        delete attr_available_axis_count_read;
        delete attr_available_task_count_read;
        delete attr_is_running_read;

        Automation1_Disconnect(ControllerClass::instance()->controller);
    }

    void Controller::init_device()
    {
        DEBUG_STREAM << "Controller::init_device() create device " << device_name << std::endl;
        get_device_property();
        set_state(Tango::INIT);
        attr_api_version_read = new Tango::DevString();
        attr_available_axis_count_read = new Tango::DevShort();
        attr_available_task_count_read = new Tango::DevShort();
        attr_is_running_read = new Tango::DevBoolean();

        connect();
        set_state(Tango::STANDBY);
    }

    void Controller::get_device_property()
    {
        Tango::DbData dev_prop;
        dev_prop.emplace_back("ip_address");

        if (!dev_prop.empty())
        {
            if (Tango::Util::_UseDb)
                get_db_device()->get_property(dev_prop);

            const auto ds_class =
                (dynamic_cast<ControllerClass*>(get_device_class()));
            int i = -1;

            if (Tango::DbDatum cl_prop = ds_class->get_class_property(dev_prop[++i].name); !cl_prop.is_empty()) cl_prop
                >> ip_address;
            else
            {
                if (Tango::DbDatum def_prop = ds_class->get_default_device_property(dev_prop[i].name); !def_prop.
                    is_empty()) def_prop >> ip_address;
            }
            if (!dev_prop[i].is_empty()) dev_prop[i] >> ip_address;
        }
    }

    void Controller::always_executed_hook()
    {
        DEBUG_STREAM << "Controller::always_executed_hook()  " << device_name << std::endl;
    }

    void Controller::read_attr_hardware(TANGO_UNUSED(std::vector<long> &attr_list))
    {
        DEBUG_STREAM << "Controller::read_attr_hardware(std::vector<long> &attr_list) entering... " << std::endl;
    }

    void Controller::write_attr_hardware(TANGO_UNUSED(std::vector<long> &attr_list))
    {
        DEBUG_STREAM << "Controller::write_attr_hardware(std::vector<long> &attr_list) entering... " << std::endl;
    }


    void Controller::add_dynamic_attributes()
    {
    }

    void Controller::add_dynamic_commands()
    {
    }

    void Controller::available_axis_count(Tango::Attribute& att)
    {
        if (!ControllerClass::instance()->controller)
        {
            Tango::Except::throw_exception("ConnectionError", "Controller not connected", "read_is_running");
        }

        *attr_available_axis_count_read = static_cast<short>(Automation1_Controller_AvailableAxisCount(
            ControllerClass::instance()->controller));
        att.set_value(attr_available_axis_count_read);
    }

    void Controller::read_api_version(Tango::Attribute& att) const
    {
        int version[3];
        Automation1_GetApiVersion(&version[0], &version[1], &version[2]);
        *attr_api_version_read = Tango::string_dup(std::format("{}.{}.{}", version[0], version[1], version[2]).c_str());
        att.set_value(attr_api_version_read);
    }

    void Controller::available_task_count(Tango::Attribute& att) const
    {
        if (!ControllerClass::instance()->controller)
        {
            Tango::Except::throw_exception("ConnectionError", "Controller not connected", "read_is_running");
        }

        *attr_available_task_count_read = 0;
        att.set_value(attr_available_task_count_read);
    }


    void Controller::connect()
    {
        DEBUG_STREAM << "Controller::connect entering... " << std::endl;
        DEBUG_STREAM << "Controller::ip_address: " << ip_address << std::endl;
        if (const auto result = Automation1_ConnectWithHost(ip_address.c_str(),
                                                            &ControllerClass::instance()->controller)
            ; result == false)
        {
            char error_msg[100];
            Automation1_GetLastErrorMessage(error_msg, 100);
            DEBUG_STREAM << "Controller::error_msg: " << error_msg << std::endl;
            Tango::Except::throw_exception("ConnectionError", "Could not connect to controller", "connect");
        }
    }

    void Controller::read_is_running(Tango::Attribute& attr)
    {
        DEBUG_STREAM << "Controller::read_is_running(Tango::Attribute &attr) entering... " << std::endl;
        if (!ControllerClass::instance()->controller)
        {
            Tango::Except::throw_exception("ConnectionError", "Controller not connected", "read_is_running");
        }
        Automation1_Controller_IsRunning(ControllerClass::instance()->controller, attr_is_running_read);
        attr.set_value(attr_is_running_read);
    }
}
