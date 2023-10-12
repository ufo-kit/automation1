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

#ifndef AUTOMATION1_CONTROLLER_CLASS_H
#define AUTOMATION1_CONTROLLER_CLASS_H


#include <tango/tango.h>
#include "Controller.h"
#include <memory>
#include "Automation1.h"


namespace Controller_ns
{
    class api_versionAttrib final : public Tango::Attr
    {
    public:
        api_versionAttrib() : Attr("api_version",
                                   Tango::DEV_STRING, Tango::READ)
        {
        };

        ~api_versionAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Controller*>(dev))->read_api_version(att); }
    };

    class is_runningAttrib final : public Tango::Attr
    {
    public:
        is_runningAttrib() : Attr("is_running",
                                  Tango::DEV_BOOLEAN, Tango::READ)
        {
        };

        ~is_runningAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Controller*>(dev))->read_is_running(att); }
    };

    class available_axis_countAttrib final : public Tango::Attr
    {
    public:
        available_axis_countAttrib() : Attr("available_axis_count",
                                            Tango::DEV_SHORT, Tango::READ)
        {
        };

        ~available_axis_countAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Controller*>(dev))->available_axis_count(att); }
    };

    class available_task_countAttrib final : public Tango::Attr
    {
    public:
        available_task_countAttrib() : Attr("available_task_count",
                                            Tango::DEV_SHORT, Tango::READ)
        {
        };

        ~available_task_countAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Controller*>(dev))->available_task_count(att); }
    };

#ifdef _TG_WINDOWS_
    class __declspec(dllexport)  ControllerClass : public Tango::DeviceClass
#else

    class ControllerClass final : public Tango::DeviceClass
#endif
    {
    public:
        Tango::DbData cl_prop;
        Tango::DbData cl_def_prop;
        Tango::DbData dev_def_prop;

        static ControllerClass* init(const char*);

        static ControllerClass* instance();

        ~ControllerClass() override;

        Tango::DbDatum get_class_property(std::string&);

        Tango::DbDatum get_default_device_property(std::string&);

        Tango::DbDatum get_default_class_property(std::string&);

        Automation1Controller controller{};

        std::mutex mutex;

    protected:
        explicit ControllerClass(const std::string&);

        static ControllerClass* _instance;

        void command_factory() override;

        void attribute_factory(std::vector<Tango::Attr*>&) override;

        void pipe_factory() override;

        void write_class_property();

        void set_default_property();

        void get_class_property();

        std::string get_cvstag();

        std::string get_cvsroot();

    private:
        void device_factory(TANGO_UNUSED(const Tango::DevVarStringArray *)) override;

        void create_static_attribute_list(std::vector<Tango::Attr*>&);

        void erase_dynamic_attributes(const Tango::DevVarStringArray*, const std::vector<Tango::Attr*>&);

        std::vector<std::string> defaultAttList;

        static Tango::Attr* get_attr_object_by_name(std::vector<Tango::Attr*>& att_list, const std::string& attname);
    };
}
#endif
