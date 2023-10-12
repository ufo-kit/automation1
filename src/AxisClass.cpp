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

#include "AxisClass.h"

extern "C" {
Tango::DeviceClass* _create_Axis_class(const char* name)
{
    return Axis_ns::AxisClass::init(name);
}
}

namespace Axis_ns
{
    AxisClass* AxisClass::_instance = nullptr;

    AxisClass::AxisClass(const std::string& s) : Tango::DeviceClass(s)
    {
        TANGO_LOG_INFO << "Entering AxisClass constructor" << std::endl;
        set_default_property();
        write_class_property();
        TANGO_LOG_INFO << "Leaving AxisClass constructor" << std::endl;
    }

    AxisClass::~AxisClass()
    {
        _instance = nullptr;
    }

    AxisClass* AxisClass::init(const char* name)
    {
        if (_instance == nullptr)
        {
            try
            {
                const std::string s(name);
                _instance = new AxisClass(s);
            }
            catch (std::bad_alloc&)
            {
                throw;
            }
        }
        return _instance;
    }


    AxisClass* AxisClass::instance()
    {
        if (_instance == nullptr)
        {
            std::cerr << "Class is not initialised !!" << std::endl;
            exit(-1);
        }
        return _instance;
    }


    CORBA::Any* EnableCommand::execute(Tango::DeviceImpl* dev, TANGO_UNUSED(const CORBA::Any &any))
    {
        TANGO_LOG_DEBUG << "EnableCommand::execute(): arrived" << std::endl;
        ((dynamic_cast<Axis*>(dev))->enable());
        return new CORBA::Any();
    }

    CORBA::Any* FaultAckCommand::execute(Tango::DeviceImpl* dev, TANGO_UNUSED(const CORBA::Any &any))
    {
        TANGO_LOG_DEBUG << "FaultAckCommand::execute(): arrived" << std::endl;
        ((dynamic_cast<Axis*>(dev))->fault_ack());
        return new CORBA::Any();
    }

    CORBA::Any* FaultAckAllCommand::execute(Tango::DeviceImpl* dev, TANGO_UNUSED(const CORBA::Any &any))
    {
        TANGO_LOG_DEBUG << "FaultAckAllCommand::execute(): arrived" << std::endl;
        ((dynamic_cast<Axis*>(dev))->fault_ack_all());
        return new CORBA::Any();
    }

    CORBA::Any* StopCommand::execute(Tango::DeviceImpl* dev, TANGO_UNUSED(const CORBA::Any &any))
    {
        TANGO_LOG_DEBUG << "StopCommand::execute(): arrived" << std::endl;
        ((dynamic_cast<Axis*>(dev))->stop());
        return new CORBA::Any();
    }

    CORBA::Any* HomeCommand::execute(Tango::DeviceImpl* dev, TANGO_UNUSED(const CORBA::Any &any))
    {
        TANGO_LOG_DEBUG << "HomeCommand::execute(): arrived" << std::endl;
        ((dynamic_cast<Axis*>(dev))->home());
        return new CORBA::Any();
    }

    CORBA::Any* DisableCommand::execute(Tango::DeviceImpl* dev, TANGO_UNUSED(const CORBA::Any &any))
    {
        TANGO_LOG_DEBUG << "DisableCommand::execute(): arrived" << std::endl;
        ((dynamic_cast<Axis*>(dev))->disable());
        return new CORBA::Any();
    }

    CORBA::Any* FreerunCommand::execute(Tango::DeviceImpl* dev, TANGO_UNUSED(const CORBA::Any &any))
    {
        TANGO_LOG_DEBUG << "ForwardCommand::execute(): arrived" << std::endl;
        Tango::DevDouble arg_in;
        extract(any, arg_in);

        dynamic_cast<Axis*>(dev)->freerun(arg_in);
        return new CORBA::Any();
    }

    Tango::DbDatum AxisClass::get_class_property(std::string& prop_name)
    {
        for (auto& i : cl_prop)
            if (i.name == prop_name)
                return i;
        //	if not found, returns  an empty DbDatum
        return {prop_name};
    }


    Tango::DbDatum AxisClass::get_default_device_property(std::string& prop_name)
    {
        for (auto& i : dev_def_prop)
            if (i.name == prop_name)
                return i;
        return {prop_name};
    }


    Tango::DbDatum AxisClass::get_default_class_property(std::string& prop_name)
    {
        for (auto& i : cl_def_prop)
            if (i.name == prop_name)
                return i;
        return {prop_name};
    }

    void AxisClass::set_default_property()
    {
        std::string prop_name;
        std::string prop_desc;
        std::string prop_def;
        std::vector<std::string> vect_data;
    }

    void AxisClass::write_class_property()
    {
        if (!Tango::Util::_UseDb)
            return;

        Tango::DbData data;
        std::string classname = get_name();
        std::string header;

        Tango::DbDatum title("Automation1");
        std::string str_title;
        title << str_title;
        data.push_back(title);

        Tango::DbDatum description("Tango device server for aerotech automation1.");
        std::vector<std::string> str_desc;
        str_desc.emplace_back("");
        description << str_desc;
        data.push_back(description);


        Tango::DbDatum inher_datum("");
        std::vector<std::string> inheritance;
        inheritance.emplace_back("TANGO_BASE_CLASS");
        inher_datum << inheritance;
        data.push_back(inher_datum);


        get_db_class()->put_property(data);
    }


    void AxisClass::device_factory(const Tango::DevVarStringArray* devlist_ptr)
    {
        for (unsigned long i = 0; i < devlist_ptr->length(); i++)
        {
            TANGO_LOG_DEBUG << "Device name : " << (*devlist_ptr)[i].in() << std::endl;
            device_list.push_back(new Axis(this, (*devlist_ptr)[i]));
        }
        erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());
        for (unsigned long i = 1; i <= devlist_ptr->length(); i++)
        {
            const auto dev = dynamic_cast<Axis*>(device_list[device_list.size() - i]);
            Axis::add_dynamic_attributes();
            if (Tango::Util::_UseDb && !Tango::Util::_FileDb)
                export_device(dev);
            else
                export_device(dev, dev->get_name().c_str());
        }
    }

    void AxisClass::attribute_factory(std::vector<Tango::Attr*>& att_list)
    {
        auto* position = new positionAttrib();
        Tango::UserDefaultAttrProp position_prop;
        position->set_default_properties(position_prop);
        position->set_disp_level(Tango::OPERATOR);
        att_list.push_back(position);

        auto* accelerating = new acceleratingAttrib();
        Tango::UserDefaultAttrProp accelerating_prop;
        accelerating->set_default_properties(accelerating_prop);
        accelerating->set_disp_level(Tango::OPERATOR);
        att_list.push_back(accelerating);

        auto* positiveHardLimit = new positiveHardLimitAttrib();
        Tango::UserDefaultAttrProp positiveHardLimit_prop;
        positiveHardLimit->set_default_properties(positiveHardLimit_prop);
        positiveHardLimit->set_disp_level(Tango::OPERATOR);
        att_list.push_back(positiveHardLimit);

        auto* negativeHardLimit = new negativeHardLimitAttrib();
        Tango::UserDefaultAttrProp negativeHardLimit_prop;
        negativeHardLimit->set_default_properties(negativeHardLimit_prop);
        negativeHardLimit->set_disp_level(Tango::OPERATOR);
        att_list.push_back(negativeHardLimit);

        auto* faults = new faultsAttrib();
        Tango::UserDefaultAttrProp faults_prop;
        faults->set_default_properties(faults_prop);
        faults->set_disp_level(Tango::OPERATOR);
        att_list.push_back(faults);

        auto* positionTarget = new positionTargetAttrib();
        Tango::UserDefaultAttrProp position_target_prop;
        positionTarget->set_default_properties(position_target_prop);
        positionTarget->set_disp_level(Tango::OPERATOR);
        att_list.push_back(positionTarget);

        auto* negativeSoftlimit = new negativeSoftlimitAttrib();
        Tango::UserDefaultAttrProp negative_softlimit_prop;
        negativeSoftlimit->set_default_properties(negative_softlimit_prop);
        negativeSoftlimit->set_disp_level(Tango::OPERATOR);
        att_list.push_back(negativeSoftlimit);

        auto* positiveSoftlimit = new positiveSoftlimitAttrib();
        Tango::UserDefaultAttrProp positive_softlimit_prop;
        positiveSoftlimit->set_default_properties(positive_softlimit_prop);
        positiveSoftlimit->set_disp_level(Tango::OPERATOR);
        att_list.push_back(positiveSoftlimit);

        auto* motion_velocity = new motionVelocityAttrib();
        Tango::UserDefaultAttrProp motion_velocity_prop;
        motion_velocity->set_default_properties(motion_velocity_prop);
        motion_velocity->set_disp_level(Tango::OPERATOR);
        att_list.push_back(motion_velocity);

        create_static_attribute_list(get_class_attr()->get_attr_list());
    }

    void AxisClass::pipe_factory()
    {
    }

    void AxisClass::command_factory()
    {
        auto* pEnableCmd =
            new EnableCommand("enable",
                              Tango::DEV_VOID, Tango::DEV_VOID,
                              "",
                              "",
                              Tango::OPERATOR);
        command_list.push_back(pEnableCmd);

        auto* pFaultAckCmd =
            new FaultAckCommand("faultAck",
                                Tango::DEV_VOID, Tango::DEV_VOID,
                                "",
                                "",
                                Tango::OPERATOR);
        command_list.push_back(pFaultAckCmd);


        auto* pFaultAckAllCmd =
            new FaultAckAllCommand("faultAckAll",
                                   Tango::DEV_VOID, Tango::DEV_VOID,
                                   "",
                                   "",
                                   Tango::OPERATOR);
        command_list.push_back(pFaultAckAllCmd);

        auto* pForwardCmd =
            new FreerunCommand("freeRun",
                               Tango::DEV_DOUBLE, Tango::DEV_VOID,
                               "",
                               "",
                               Tango::OPERATOR);
        command_list.push_back(pForwardCmd);

        auto* pStopCmd =
            new StopCommand("stop",
                            Tango::DEV_VOID, Tango::DEV_VOID,
                            "",
                            "",
                            Tango::OPERATOR);
        command_list.push_back(pStopCmd);

        auto* pHomeCmd =
            new HomeCommand("home",
                            Tango::DEV_VOID, Tango::DEV_VOID,
                            "",
                            "",
                            Tango::OPERATOR);
        command_list.push_back(pHomeCmd);

        auto* pDisableCmd =
            new DisableCommand("disable",
                               Tango::DEV_VOID, Tango::DEV_VOID,
                               "",
                               "",
                               Tango::OPERATOR);
        command_list.push_back(pDisableCmd);
    }

    void AxisClass::create_static_attribute_list(std::vector<Tango::Attr*>& att_list)
    {
        for (const auto& i : att_list)
        {
            std::string att_name(i->get_name());
            std::ranges::transform(att_name, att_name.begin(), ::tolower);
            defaultAttList.push_back(att_name);
        }

        TANGO_LOG_DEBUG << defaultAttList.size() << " attributes in default list" << std::endl;
    }

    void AxisClass::erase_dynamic_attributes(const Tango::DevVarStringArray* devlist_ptr,
                                             const std::vector<Tango::Attr*>& att_list)
    {
        Tango::Util* tg = Tango::Util::instance();

        for (unsigned long i = 0; i < devlist_ptr->length(); i++)
        {
            Tango::DeviceImpl* dev_impl = tg->get_device_by_name(static_cast<std::string>((*devlist_ptr)[i]).c_str());
            const auto dev = dynamic_cast<Axis*>(dev_impl);

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

    Tango::Attr* AxisClass::get_attr_object_by_name(std::vector<Tango::Attr*>& att_list, const std::string& attname)
    {
        for (auto it = att_list.begin(); it < att_list.end(); ++it)
            if ((*it)->get_name() == attname)
                return (*it);
        //	Attr does not exist
        return nullptr;
    }
} //	namespace
