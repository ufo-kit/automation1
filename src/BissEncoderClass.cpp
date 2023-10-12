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

#include "BissEncoderClass.h"

extern "C" {
Tango::DeviceClass* _create_BissEncoder_class(const char* name)
{
    return BissEncoder_ns::BissEncoderClass::init(name);
}
}

namespace BissEncoder_ns
{
    BissEncoderClass* BissEncoderClass::_instance = nullptr;

    BissEncoderClass::BissEncoderClass(const std::string& s) : Tango::DeviceClass(s)
    {
        TANGO_LOG_INFO << "Entering AxisClass constructor" << std::endl;
        set_default_property();
        write_class_property();
        TANGO_LOG_INFO << "Leaving AxisClass constructor" << std::endl;
    }

    BissEncoderClass::~BissEncoderClass()
    {
        _instance = nullptr;
    }

    BissEncoderClass* BissEncoderClass::init(const char* name)
    {
        if (_instance == nullptr)
        {
            try
            {
                const std::string s(name);
                _instance = new BissEncoderClass(s);
            }
            catch (std::bad_alloc&)
            {
                throw;
            }
        }
        return _instance;
    }


    BissEncoderClass* BissEncoderClass::instance()
    {
        if (_instance == nullptr)
        {
            std::cerr << "Class is not initialised !!" << std::endl;
            exit(-1);
        }
        return _instance;
    }


    Tango::DbDatum BissEncoderClass::get_class_property(std::string& prop_name)
    {
        for (auto& i : cl_prop)
            if (i.name == prop_name)
                return i;
        //	if not found, returns  an empty DbDatum
        return {prop_name};
    }


    Tango::DbDatum BissEncoderClass::get_default_device_property(std::string& prop_name)
    {
        for (auto& i : dev_def_prop)
            if (i.name == prop_name)
                return i;
        return {prop_name};
    }


    Tango::DbDatum BissEncoderClass::get_default_class_property(std::string& prop_name)
    {
        for (auto& i : cl_def_prop)
            if (i.name == prop_name)
                return i;
        return {prop_name};
    }

    void BissEncoderClass::set_default_property()
    {
        std::string prop_name;
        std::string prop_desc;
        std::string prop_def;
        std::vector<std::string> vect_data;
    }

    void BissEncoderClass::write_class_property()
    {
        if (!Tango::Util::_UseDb)
            return;

        Tango::DbData data;
        std::string classname = get_name();
        std::string header;

        Tango::DbDatum title("ProjectTitle");
        std::string str_title;
        title << str_title;
        data.push_back(title);

        Tango::DbDatum description("Description");
        std::vector<std::string> str_desc;
        str_desc.emplace_back("");
        description << str_desc;
        data.push_back(description);


        Tango::DbDatum inher_datum("InheritedFrom");
        std::vector<std::string> inheritance;
        inheritance.emplace_back("TANGO_BASE_CLASS");
        inher_datum << inheritance;
        data.push_back(inher_datum);


        get_db_class()->put_property(data);
    }


    void BissEncoderClass::device_factory(const Tango::DevVarStringArray* devlist_ptr)
    {
        for (unsigned long i = 0; i < devlist_ptr->length(); i++)
        {
            TANGO_LOG_DEBUG << "Device name : " << (*devlist_ptr)[i].in() << std::endl;
            device_list.push_back(new BissEncoder(this, (*devlist_ptr)[i]));
        }
        erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());
        for (unsigned long i = 1; i <= devlist_ptr->length(); i++)
        {
            const auto dev = dynamic_cast<BissEncoder*>(device_list[device_list.size() - i]);
            BissEncoder_ns::BissEncoder::add_dynamic_attributes();
            if (Tango::Util::_UseDb && !Tango::Util::_FileDb)
                export_device(dev);
            else
                export_device(dev, dev->get_name().c_str());
        }
    }

    void BissEncoderClass::attribute_factory(std::vector<Tango::Attr*>& att_list)
    {
        auto* position = new positionAttrib();
        Tango::UserDefaultAttrProp position_prop;
        position->set_default_properties(position_prop);
        position->set_disp_level(Tango::OPERATOR);
        att_list.push_back(position);


        create_static_attribute_list(get_class_attr()->get_attr_list());
    }

    void BissEncoderClass::pipe_factory()
    {
    }

    void BissEncoderClass::command_factory()
    {
    }

    void BissEncoderClass::create_static_attribute_list(std::vector<Tango::Attr*>& att_list)
    {
        for (const auto& i : att_list)
        {
            std::string att_name(i->get_name());
            std::ranges::transform(att_name, att_name.begin(), ::tolower);
            defaultAttList.push_back(att_name);
        }

        TANGO_LOG_INFO << defaultAttList.size() << " attributes in default list" << std::endl;
    }

    void BissEncoderClass::erase_dynamic_attributes(const Tango::DevVarStringArray* devlist_ptr,
                                                    const std::vector<Tango::Attr*>& att_list)
    {
        Tango::Util* tg = Tango::Util::instance();

        for (unsigned long i = 0; i < devlist_ptr->length(); i++)
        {
            Tango::DeviceImpl* dev_impl = tg->get_device_by_name(static_cast<std::string>((*devlist_ptr)[i]).c_str());
            const auto dev = dynamic_cast<BissEncoder*>(dev_impl);

            std::vector<Tango::Attribute*>& dev_att_list = dev->get_device_attr()->get_attribute_list();
            std::vector<Tango::Attribute*>::iterator ite_att;
            for (ite_att = dev_att_list.begin(); ite_att != dev_att_list.end(); ++ite_att)
            {
                std::string att_name((*ite_att)->get_name_lower());
                if ((att_name == "state") || (att_name == "status"))
                    continue;
                if (auto ite_str = std::ranges::find(defaultAttList, att_name); ite_str == defaultAttList.end())
                {
                    TANGO_LOG_INFO << att_name << " is a UNWANTED dynamic attribute for device " << (*devlist_ptr)[i]
                                   << std::endl;
                    Tango::Attribute& att = dev->get_device_attr()->get_attr_by_name(att_name.c_str());
                    dev->remove_attribute(att_list[att.get_attr_idx()], true, false);
                    --ite_att;
                }
            }
        }
    }

    Tango::Attr* BissEncoderClass::get_attr_object_by_name(std::vector<Tango::Attr*>& att_list,
                                                           const std::string& attname)
    {
        for (auto it = att_list.begin(); it < att_list.end(); ++it)
            if ((*it)->get_name() == attname)
                return (*it);
        //	Attr does not exist
        return nullptr;
    }
} //	namespace
