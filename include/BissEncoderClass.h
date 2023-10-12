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

#ifndef AUTOMATION1_BISS_ENCODER_CLASS_H
#define AUTOMATION1_BISS_ENCODER_CLASS_H

#include <tango/tango.h>
#include "BissEncoder.h"


namespace BissEncoder_ns
{
    class positionAttrib final : public Tango::Attr
    {
    public:
        positionAttrib() : Attr("position",
                                Tango::DEV_DOUBLE, Tango::READ)
        {
        };

        ~positionAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<BissEncoder*>(dev))->read_position(att); }


        bool is_allowed(Tango::DeviceImpl* dev, const Tango::AttReqType ty) override
        {
            return (dynamic_cast<BissEncoder*>(dev))->is_position_allowed(ty);
        }
    };

    class BissEncoderClass final : public Tango::DeviceClass
    {
    public:
        Tango::DbData cl_prop;
        Tango::DbData cl_def_prop;
        Tango::DbData dev_def_prop;

        static BissEncoderClass* init(const char*);

        static BissEncoderClass* instance();

        ~BissEncoderClass() override;

        Tango::DbDatum get_class_property(std::string&);

        Tango::DbDatum get_default_device_property(std::string&);

        Tango::DbDatum get_default_class_property(std::string&);

    protected:
        explicit BissEncoderClass(const std::string&);

        static BissEncoderClass* _instance;

        void command_factory() override;

        void attribute_factory(std::vector<Tango::Attr*>&) override;

        void pipe_factory() override;

        void write_class_property();

        static void set_default_property();

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
