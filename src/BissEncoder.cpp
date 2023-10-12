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

#include "BissEncoder.h"
#include "BissEncoderClass.h"
#include "ControllerClass.h"
#include <Automation1.h>
#include <complex>
#include <stdexcept>


namespace BissEncoder_ns {
    BissEncoder::BissEncoder(Tango::DeviceClass *cl, const std::string &s) : TANGO_BASE_CLASS(cl, s.c_str()) {
        BissEncoder::init_device();
    }

    BissEncoder::BissEncoder(Tango::DeviceClass *cl, const char *s) : TANGO_BASE_CLASS(cl, s) {
        BissEncoder::init_device();
    }

    BissEncoder::BissEncoder(Tango::DeviceClass *cl, const char *s, const char *d) : TANGO_BASE_CLASS(cl, s, d) {
        BissEncoder::init_device();
    }

    BissEncoder::~BissEncoder() {
        BissEncoder::delete_device();
    }

    void BissEncoder::delete_device() {
        DEBUG_STREAM << "BissEncoder::delete_device() " << device_name << std::endl;

        delete attr_position_read;

    }

    void BissEncoder::init_device() {
        DEBUG_STREAM << "BissEncoder::init_device() create device " << device_name << std::endl;
        get_device_property();

        Automation1_Controller_GetAxisIndexFromAxisName(Controller_ns::ControllerClass::instance()->controller,
                                                        axisName.c_str(), &axisID);




        attr_position_read = new Tango::DevDouble();

        DEBUG_STREAM << "axis " << axisName << " with id " << axisID << " found." << std::endl;
    }

    void BissEncoder::get_device_property() {
        Tango::DbData dev_prop;
        dev_prop.emplace_back("axisName");
        dev_prop.emplace_back("scale");
        dev_prop.emplace_back("offset");


        if (!dev_prop.empty()) {
            if (Tango::Util::_UseDb)
                get_db_device()->get_property(dev_prop);

            const auto ds_class =
                    (dynamic_cast<BissEncoderClass *>(get_device_class()));
            int i = -1;

            if (Tango::DbDatum cl_prop = ds_class->get_class_property(dev_prop[++i].name); !cl_prop.is_empty())
                cl_prop >> axisName;
            else {
                if (Tango::DbDatum def_prop = ds_class->get_default_device_property(dev_prop[i].name); !def_prop.
                    is_empty())
                    def_prop >> axisName;
            }
            if (!dev_prop[i].is_empty()) dev_prop[i] >> axisName;

            if (Tango::DbDatum cl_prop = ds_class->get_class_property(dev_prop[++i].name); !cl_prop.is_empty())
                cl_prop >> scale;
            else {
                if (Tango::DbDatum def_prop = ds_class->get_default_device_property(dev_prop[i].name); !def_prop.
                    is_empty())
                    def_prop >> scale;
            }
            if (!dev_prop[i].is_empty()) dev_prop[i] >> scale;

            if (Tango::DbDatum cl_prop = ds_class->get_class_property(dev_prop[++i].name); !cl_prop.is_empty())
                cl_prop >> offset;
            else {
                if (Tango::DbDatum def_prop = ds_class->get_default_device_property(dev_prop[i].name); !def_prop.
                    is_empty())
                    def_prop >> offset;
            }
            if (!dev_prop[i].is_empty()) dev_prop[i] >> offset;
        }
    }


    void BissEncoder::always_executed_hook() {

    }


    void BissEncoder::read_attr_hardware(TANGO_UNUSED(std::vector<long> &attr_list)) {

    }


    void BissEncoder::add_dynamic_attributes() {
    }


    void BissEncoder::add_dynamic_commands() {
    }

    void BissEncoder::read_position(Tango::Attribute &attribute) {
        if (Controller_ns::ControllerClass::instance()->controller == nullptr)
            Tango::Except::throw_exception("Controller not connected", "The controller is not connected",
                                           "read_position()");

        double encoder_position = 0;
        DEBUG_STREAM << "scale: " << scale << std::endl;
        Automation1_Command_DriveGetItem(Controller_ns::ControllerClass::instance()->controller, 1, axisID, Automation1DriveItem_PrimaryBissAbsolutePosition, 0, &encoder_position);
        DEBUG_STREAM << "encoder value: " <<  static_cast<int>(encoder_position) << std::endl;
        *attr_position_read = counts_to_user_unit(encoder_position) * scale - offset;
        attribute.set_value(attr_position_read);
    }


    bool BissEncoder::is_position_allowed(const Tango::AttReqType type) {
        if (type == Tango::READ_REQ) {
            return true;
        } else {
            return dev_state() == Tango::STANDBY;
        }
    }



    double BissEncoder::counts_to_user_unit(const double counts) const {
        double countsPerUnitParam;

        Automation1_Parameter_GetAxisValue(Controller_ns::ControllerClass::instance()->controller,
                                           axisID,
                                           Automation1AxisParameterId_CountsPerUnit,
                                           &countsPerUnitParam);
        return counts / countsPerUnitParam;
    }

    double BissEncoder::user_unit_to_counts(const double user_unit) const {
        double countsPerUnitParam;
        Automation1_Parameter_GetAxisValue(Controller_ns::ControllerClass::instance()->controller,
                                           axisID,
                                           Automation1AxisParameterId_CountsPerUnit,
                                           &countsPerUnitParam);
        return user_unit * countsPerUnitParam;
    }

    [[maybe_unused]] Tango::DevState BissEncoder::dev_state() {
        return Tango::DevState::STANDBY;
    }

    const char *BissEncoder::dev_status() {
        return {"standby"};
    }


}
