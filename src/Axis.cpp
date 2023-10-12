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

#include "Axis.h"
#include "AxisClass.h"
#include "ControllerClass.h"
#include <Automation1.h>
#include <complex>
#include <stdexcept>


namespace Axis_ns
{
    Axis::Axis(Tango::DeviceClass* cl, const std::string& s) : TANGO_BASE_CLASS(cl, s.c_str())
    {
        Axis::init_device();
    }

    Axis::Axis(Tango::DeviceClass* cl, const char* s) : TANGO_BASE_CLASS(cl, s)
    {
        Axis::init_device();
    }

    Axis::Axis(Tango::DeviceClass* cl, const char* s, const char* d) : TANGO_BASE_CLASS(cl, s, d)
    {
        Axis::init_device();
    }

    Axis::~Axis()
    {
        Axis::delete_device();
    }

    void Axis::delete_device()
    {
        DEBUG_STREAM << "Axis::delete_device() " << device_name << std::endl;
        Automation1_StatusConfig_Destroy(statusConfig);
        delete attr_motion_velocity;
        delete attr_position_read;
        delete attr_faults_read;
        delete attr_position_target_read;
        delete attr_accelerating_read;
        delete attr_negative_softlimit_read;
        delete attr_positive_softlimit_read;
    }

    void Axis::init_device()
    {
        DEBUG_STREAM << "Axis::init_device() create device " << device_name << std::endl;
        get_device_property();

        Automation1_Controller_GetAxisIndexFromAxisName(Controller_ns::ControllerClass::instance()->controller,
                                                        axisName.c_str(), &axisID);
        Automation1_StatusConfig_Create(&statusConfig);


        auto getKeyByValue = [&](const auto& map, const auto& value)
        {
            auto it = std::find_if(map.begin(), map.end(), [&](const auto& pair)
            {
                return pair.second == value;
            });
            if (it != map.end())
            {
                return it->first;
            }
            else
            {
                throw std::runtime_error("Value not found in the map");
            }
        };

        for (int i = 0; i < axisStates.size(); i++)
        {
            auto fst = getKeyByValue(axisStates, i);
            Automation1_StatusConfig_AddAxisStatusItem(statusConfig, axisID, fst, 0);
        }

        attr_motion_velocity = new Tango::DevDouble();
        *attr_motion_velocity = 5.f;
        attr_position_read = new Tango::DevDouble();
        attr_position_target_read = new Tango::DevDouble();
        attr_faults_read = new Tango::DevString();
        attr_accelerating_read = new Tango::DevBoolean();
        attr_negative_softlimit_read = new Tango::DevDouble();
        attr_positive_softlimit_read = new Tango::DevDouble();
        attr_positive_hard_limit_read = new Tango::DevBoolean();
        attr_negative_hard_limit_read = new Tango::DevBoolean();


        DEBUG_STREAM << "axis " << axisName << " with id " << axisID << " found." << std::endl;
    }

    void Axis::get_device_property()
    {
        Tango::DbData dev_prop;
        dev_prop.emplace_back("axisName");

        if (!dev_prop.empty())
        {
            if (Tango::Util::_UseDb)
                get_db_device()->get_property(dev_prop);

            const auto ds_class =
                (dynamic_cast<AxisClass*>(get_device_class()));
            int i = -1;

            if (Tango::DbDatum cl_prop = ds_class->get_class_property(dev_prop[++i].name); !cl_prop.is_empty())
                cl_prop >> axisName;
            else
            {
                if (Tango::DbDatum def_prop = ds_class->get_default_device_property(dev_prop[i].name); !def_prop.
                    is_empty())
                    def_prop >> axisName;
            }
            if (!dev_prop[i].is_empty()) dev_prop[i] >> axisName;
        }
    }


    void Axis::always_executed_hook()
    {
    }


    void Axis::read_attr_hardware(TANGO_UNUSED(std::vector<long> &attr_list))
    {
    }


    void Axis::add_dynamic_attributes()
    {
    }

    void Axis::enable()
    {
        DEBUG_STREAM << "Axis::enable()  - " << device_name << std::endl;
        std::lock_guard<std::mutex> lk(Controller_ns::ControllerClass::instance()->mutex);
        if (const auto response = Automation1_Command_Enable(Controller_ns::ControllerClass::instance()->controller,
                                                             1, &axisID, 1); !response)
        {
            char msg[100];
            Automation1_GetLastErrorMessage(msg, 100);
            ERROR_STREAM << msg << std::endl;
        }
    }

    void Axis::stop()
    {
        DEBUG_STREAM << "Axis::stop()  - " << device_name << std::endl;
        std::lock_guard<std::mutex> lk(Controller_ns::ControllerClass::instance()->mutex);
        if (const auto response = Automation1_Command_MoveFreerunStop(
            Controller_ns::ControllerClass::instance()->controller, 1, &axisID, 1); !response)
        {
            char msg[100];
            Automation1_GetLastErrorMessage(msg, 100);
            ERROR_STREAM << msg << std::endl;
        }
    }

    void Axis::home()
    {
        DEBUG_STREAM << "Axis::home()  - " << device_name << std::endl;
        std::lock_guard<std::mutex> lk(Controller_ns::ControllerClass::instance()->mutex);
        if (const auto response = Automation1_Command_HomeAsync(
            Controller_ns::ControllerClass::instance()->controller, 1, &axisID, 1); !response)
        {
            char msg[100];
            Automation1_GetLastErrorMessage(msg, 100);
            ERROR_STREAM << msg << std::endl;
        }
    }

    void Axis::disable()
    {
        DEBUG_STREAM << "Axis::home()  - " << device_name << std::endl;
        std::lock_guard<std::mutex> lk(Controller_ns::ControllerClass::instance()->mutex);
        if (const auto response = Automation1_Command_Disable(
            Controller_ns::ControllerClass::instance()->controller, &axisID, 1); !response)
        {
            char msg[100];
            Automation1_GetLastErrorMessage(msg, 100);
            ERROR_STREAM << msg << std::endl;
        }
    }

    void Axis::fault_ack()
    {
        DEBUG_STREAM << "Axis::fault_ack()  - " << device_name << std::endl;
        std::lock_guard<std::mutex> lk(Controller_ns::ControllerClass::instance()->mutex);
        if (const auto response = Automation1_Command_FaultAcknowledge(
            Controller_ns::ControllerClass::instance()->controller,
            1, &axisID, 1); !response)
        {
            char msg[100];
            Automation1_GetLastErrorMessage(msg, 100);
            ERROR_STREAM << msg << std::endl;
        }
        auto sub_axes = get_gantry_sub_axes();
        for (auto sub_axis : sub_axes)
        {
            if (const auto response = Automation1_Command_FaultAcknowledge(
                Controller_ns::ControllerClass::instance()->controller,
                1, &sub_axis, 1); !response)
            {
                char msg[100];
                Automation1_GetLastErrorMessage(msg, 100);
                ERROR_STREAM << msg << std::endl;
            }
        }
    }

    void Axis::add_dynamic_commands()
    {
    }

    void Axis::read_position(Tango::Attribute& attribute) const
    {
        std::lock_guard<std::mutex> lk(Controller_ns::ControllerClass::instance()->mutex);
        if (Controller_ns::ControllerClass::instance()->controller == nullptr)
            Tango::Except::throw_exception("Controller not connected", "The conctroller is not connected",
                                           "read_position()");

        double results[axisStates.size()];
        Automation1_Status_GetResults(Controller_ns::ControllerClass::instance()->controller,
                                      statusConfig,
                                      results,
                                      static_cast<int>(sizeof(results) / sizeof(double)));

        *attr_position_read = results[axisStates.at(Automation1AxisStatusItem_ProgramPositionFeedback)];
        attribute.set_value(attr_position_read);
    }

    void Axis::read_position_target(Tango::Attribute& attribute) const
    {
        std::lock_guard<std::mutex> lk(Controller_ns::ControllerClass::instance()->mutex);
        if (Controller_ns::ControllerClass::instance()->controller == nullptr)
            Tango::Except::throw_exception("Controller not connected", "The conctroller is not connected",
                                           "read_position_target()");

        double results[axisStates.size()];
        Automation1_Status_GetResults(Controller_ns::ControllerClass::instance()->controller,
                                      statusConfig,
                                      results,
                                      static_cast<int>(sizeof(results) / sizeof(double)));

        *attr_position_target_read = results[axisStates.at(Automation1AxisStatusItem_ProgramPositionCommand)];
        attribute.set_value(attr_position_target_read);
    }

    void Axis::write_position(Tango::WAttribute& attribute)
    {
        std::lock_guard<std::mutex> lk(Controller_ns::ControllerClass::instance()->mutex);
        Tango::DevDouble w_val;
        attribute.get_write_value(w_val);
        if (const auto response = Automation1_Command_MoveAbsolute(
            Controller_ns::ControllerClass::instance()->controller, 1, &axisID, 1, &w_val,
            1, attr_motion_velocity, 1); !response)
        {
            ERROR_STREAM << "Motion not successful." << std::endl;
        }
    }

    bool Axis::is_enable_allowed(const CORBA::Any& type)
    {
        return true;
        return dev_state() == Tango::DISABLE;
    }

    bool Axis::is_home_allowed(const CORBA::Any& type)
    {
        return dev_state() == Tango::STANDBY;
    }

    bool Axis::is_disable_allowed(const CORBA::Any& type)
    {
        return dev_state() == Tango::STANDBY;
    }

    bool Axis::is_stop_allowed(const CORBA::Any& type)
    {
        return true;
    }

    bool Axis::is_fault_ack_allowed(const CORBA::Any& type)
    {
        return dev_state() == Tango::FAULT;
    }

    bool Axis::is_position_allowed(const Tango::AttReqType type)
    {
        if (type == Tango::READ_REQ)
        {
            return true;
        }
        else
        {
            return dev_state() == Tango::STANDBY;
        }
    }

    void Axis::read_positive_hard_limit(Tango::Attribute& att)
    {
        std::lock_guard lk(Controller_ns::ControllerClass::instance()->mutex);
        double motor_direction;
        bool limit_switch;
        Automation1_Parameter_GetAxisValue(Controller_ns::ControllerClass::instance()->controller, axisID,
                                           Automation1AxisParameterId_ReverseMotionDirection, &motor_direction);
        auto [enabled, cw_end_of_travel_limit_input, ccw_end_of_travel_limit_input, emergency_stop_input, accelerating,
            decelerating, move_active] = get_drive_status();
        if (int(motor_direction) == 0)
        {
            *attr_positive_hard_limit_read = ccw_end_of_travel_limit_input;
        }
        else
        {
            *attr_positive_hard_limit_read = cw_end_of_travel_limit_input;
        }

        att.set_value(attr_positive_hard_limit_read);
    }


    void Axis::read_negative_hard_limit(Tango::Attribute& att)
    {
        std::lock_guard lk(Controller_ns::ControllerClass::instance()->mutex);
        double motor_direction;
        Automation1_Parameter_GetAxisValue(Controller_ns::ControllerClass::instance()->controller, axisID,
                                           Automation1AxisParameterId_ReverseMotionDirection, &motor_direction);
        auto [enabled, cw_end_of_travel_limit_input, ccw_end_of_travel_limit_input, emergency_stop_input, accelerating,
            decelerating, move_active] = get_drive_status();
        if (static_cast<int>(motor_direction) == 0)
        {
            *attr_positive_hard_limit_read = cw_end_of_travel_limit_input;
        }
        else
        {
            *attr_positive_hard_limit_read = ccw_end_of_travel_limit_input;
        }

        att.set_value(attr_positive_hard_limit_read);
    }


    bool Axis::is_positive_hard_limit_allowed(Tango::AttReqType ty)
    {
        return true;
    }

    bool Axis::is_negative_hard_limit_allowed(Tango::AttReqType ty)
    {
        return true;
    }

    bool Axis::is_position_target_allowed(const Tango::AttReqType type)
    {
        if (type == Tango::READ_REQ)
        {
            return true;
        }
        else
        {
            return dev_state() == Tango::STANDBY;
        }
    }

    void Axis::read_positive_softlimit(Tango::Attribute& attr)
    {
        std::lock_guard lk(Controller_ns::ControllerClass::instance()->mutex);
        double softlimit;
        double softlimitSetup;
        Automation1_Parameter_GetAxisValue(Controller_ns::ControllerClass::instance()->controller, axisID,
                                           Automation1AxisParameterId_SoftwareLimitSetup, &softlimitSetup);
        if (static_cast<int>(softlimitSetup) == 0)
        {
            *attr_positive_softlimit_read = NAN;
        }
        else
        {
            Automation1_Parameter_GetAxisValue(Controller_ns::ControllerClass::instance()->controller, axisID,
                                               Automation1AxisParameterId_SoftwareLimitHigh, &softlimit);
            *attr_positive_softlimit_read = softlimit;
        }
        attr.set_value(attr_positive_softlimit_read);
    }

    void Axis::read_negative_softlimit(Tango::Attribute& attr)
    {
        std::lock_guard lk(Controller_ns::ControllerClass::instance()->mutex);
        double softlimit;
        double softlimitSetup;
        Automation1_Parameter_GetAxisValue(Controller_ns::ControllerClass::instance()->controller, axisID,
                                           Automation1AxisParameterId_SoftwareLimitSetup, &softlimitSetup);
        if (static_cast<int>(softlimitSetup) == 0)
        {
            *attr_negative_softlimit_read = NAN;
        }
        else
        {
            Automation1_Parameter_GetAxisValue(Controller_ns::ControllerClass::instance()->controller, axisID,
                                               Automation1AxisParameterId_SoftwareLimitLow, &softlimit);
            *attr_negative_softlimit_read = softlimit;
        }
        attr.set_value(attr_negative_softlimit_read);
    }

    void Axis::write_negative_softlimit(Tango::WAttribute& attr)
    {
        std::lock_guard lk(Controller_ns::ControllerClass::instance()->mutex);
        Tango::DevDouble w_val;
        attr.get_write_value(w_val);
        Automation1_Parameter_SetAxisValue(Controller_ns::ControllerClass::instance()->controller, axisID,
                                           Automation1AxisParameterId_SoftwareLimitLow, w_val);
    }

    void Axis::write_positive_softlimit(Tango::WAttribute& attr)
    {
        std::lock_guard lk(Controller_ns::ControllerClass::instance()->mutex);
        Tango::DevDouble w_val;
        attr.get_write_value(w_val);
        Automation1_Parameter_SetAxisValue(Controller_ns::ControllerClass::instance()->controller, axisID,
                                           Automation1AxisParameterId_SoftwareLimitHigh, w_val);
        char msg[100];
        Automation1_GetLastErrorMessage(msg, 100);
        ERROR_STREAM << msg << std::endl;
    }

    bool Axis::is_motion_velocity_allowed(Tango::AttReqType type)
    {
        return true;
    }

    bool Axis::is_positive_softlimit_allowed(const Tango::AttReqType type)
    {
        return true;
    }

    bool Axis::is_negative_softlimit_allowed(const Tango::AttReqType type)
    {
        return true;
    }


    void Axis::write_motion_velocity(Tango::WAttribute& attribute)
    {
        attribute.get_write_value(*attr_motion_velocity);
    }

    void Axis::read_motion_velocity(Tango::Attribute& attribute) const
    {
        attribute.set_value(attr_motion_velocity);
    }

    bool Axis::is_freerun_allowed(const CORBA::Any& type)
    {
        return dev_state() == Tango::STANDBY;
    }

    void Axis::freerun(Tango::DevDouble arg_in)
    {
        std::lock_guard lk(Controller_ns::ControllerClass::instance()->mutex);
        if (const auto response = Automation1_Command_MoveFreerun(
            Controller_ns::ControllerClass::instance()->controller,
            1, &axisID, 1, &arg_in, 1); !response)
        {
            char msg[100];
            Automation1_GetLastErrorMessage(msg, 100);
            ERROR_STREAM << msg << std::endl;
        }
    }

    double Axis::counts_to_user_unit(const double counts) const
    {
        std::lock_guard lk(Controller_ns::ControllerClass::instance()->mutex);
        double countsPerUnitParam;

        Automation1_Parameter_GetAxisValue(Controller_ns::ControllerClass::instance()->controller,
                                           axisID,
                                           Automation1AxisParameterId_CountsPerUnit,
                                           &countsPerUnitParam);
        return counts / countsPerUnitParam;
    }

    double Axis::user_unit_to_counts(const double user_unit) const
    {
        std::lock_guard lk(Controller_ns::ControllerClass::instance()->mutex);
        double countsPerUnitParam;
        Automation1_Parameter_GetAxisValue(Controller_ns::ControllerClass::instance()->controller,
                                           axisID,
                                           Automation1AxisParameterId_CountsPerUnit,
                                           &countsPerUnitParam);
        return user_unit * countsPerUnitParam;
    }

    [[maybe_unused]] Tango::DevState Axis::dev_state()
    {
        std::lock_guard lk(Controller_ns::ControllerClass::instance()->mutex);
        auto [enabled, cw_end_of_travel_limit_input, ccw_end_of_travel_limit_input, emergency_stop_input, accelerating,
            decelerating, move_active] = get_drive_status();
        auto [homed, profiling, homing, jogging, not_virtual, motion_done, motion_clamped, gantry_aligned,
            calibration_enabled_1d, calibration_enabled_2d] = get_axis_status();
        auto [anyFault, positionErrorFault, OverCurrentFault, CwEndOfTravelLimitFault, CcwEndOfTravelLimitFault,
            CwSoftwareLimitFault, CcwSoftwareLimitFault, AmplifierFault, FeedbackInput0Fault, FeedbackInput1Fault,
            HallSensorFault, MaxVelocityCommandFault, EmergencyStopFault, VelocityErrorFault, CommutationFault,
            ExternalFault, MotorTemperatureFault, AmplifierTemperatureFault, EncoderFault, GantryMisalignmentFault,
            FeedbackScalingFault, MarkerSearchFault, SafeZoneFault, InPositionTimeoutFault, VoltageClampFault,
            MotorSupplyFault, InternalFault] = get_axis_faults();
        if (anyFault)
            return Tango::DevState::FAULT;
        if (!enabled)
            return Tango::DevState::DISABLE;
        if (homing)
            return Tango::DevState::MOVING;
        if (motion_done)
            return Tango::DevState::STANDBY;
        return Tango::MOVING;
    }

    const char* Axis::dev_status()
    {
        std::lock_guard lk(Controller_ns::ControllerClass::instance()->mutex);
        auto [enabled, cw_end_of_travel_limit_input, ccw_end_of_travel_limit_input, emergency_stop_input, accelerating,
            decelerating, move_active] = get_drive_status();
        auto [homed, profiling, homing, jogging, not_virtual, motion_done, motion_clamped, gantry_aligned,
            calibration_enabled_1d, calibration_enabled_2d] = get_axis_status();
        status = std::format("enable: {}\ncw_end_of_travel_limit_input: {}\nccw_end_of_travel_limit_input: {}\n"
                             "emergency_stop_input: {}\naccelerating: {}\ndecelerating: {}\nhomed: {}\n"
                             "profiling: {}\nhoming: {}\njogging: {}\nnot_virtual: {}\nmotion_done: {}\n"
                             "motion_clamped: {}\ngantry_aligned: {}\ncalibration_enabled_1d: {}\n"
                             "calibration_enabled_2d: {}\nmove_active: {}\n",
                             enabled, cw_end_of_travel_limit_input, ccw_end_of_travel_limit_input,
                             emergency_stop_input, accelerating, decelerating, homed, profiling, homing,
                             jogging, not_virtual, motion_done, motion_clamped, gantry_aligned,
                             calibration_enabled_1d, calibration_enabled_2d, move_active);
        return status.c_str();
    }

    bool Axis::is_fault_allowed(Tango::AttReqType ty)
    {
        return true;
    }

    void Axis::read_faults(Tango::Attribute& att)
    {
        std::lock_guard<std::mutex> lk(Controller_ns::ControllerClass::instance()->mutex);
        auto [anyFault, positionErrorFault, OverCurrentFault, CwEndOfTravelLimitFault, CcwEndOfTravelLimitFault,
            CwSoftwareLimitFault, CcwSoftwareLimitFault, AmplifierFault, FeedbackInput0Fault, FeedbackInput1Fault,
            HallSensorFault, MaxVelocityCommandFault, EmergencyStopFault, VelocityErrorFault, CommutationFault,
            ExternalFault, MotorTemperatureFault, AmplifierTemperatureFault, EncoderFault, GantryMisalignmentFault,
            FeedbackScalingFault, MarkerSearchFault, SafeZoneFault, InPositionTimeoutFault, VoltageClampFault,
            MotorSupplyFault, InternalFault] = get_axis_faults();

        std::string faults{};
        if (positionErrorFault) faults += "Position Error Fault\n";
        if (OverCurrentFault) faults += "Over Current Fault\n";
        if (CwEndOfTravelLimitFault) faults += "CW End Of Travel Limit Fault\n";
        if (CcwEndOfTravelLimitFault) faults += "CCW End Of Travel Limit Fault\n";
        if (CwSoftwareLimitFault) faults += "CW Software Limit Fault\n";
        if (CcwSoftwareLimitFault) faults += "CCW Software Limit Fault\n";
        if (AmplifierFault) faults += "Amplifier Fault\n";
        if (FeedbackInput0Fault) faults += "Feedback Input 0 Fault\n";
        if (FeedbackInput1Fault) faults += "Feedback Input 1 Fault\n";
        if (HallSensorFault) faults += "Hall Sensor Fault\n";
        if (MaxVelocityCommandFault) faults += "Max Velocity Command Fault\n";
        if (EmergencyStopFault) faults += "Emergency Stop Fault\n";
        if (VelocityErrorFault) faults += "Velocity Error Fault\n";
        if (CommutationFault) faults += "Commutation Fault\n";
        if (ExternalFault) faults += "External Fault\n";
        if (MotorTemperatureFault) faults += "Motor Temperature Fault\n";
        if (AmplifierTemperatureFault) faults += "Amplifier Temperature Fault\n";
        if (EncoderFault) faults += "Encoder Fault\n";
        if (GantryMisalignmentFault) faults += "Gantry Misalignment Fault\n";
        if (FeedbackScalingFault) faults += "Feedback Scaling Fault\n";
        if (MarkerSearchFault) faults += "Marker Search Fault\n";
        if (SafeZoneFault) faults += "Safe Zone Fault\n";
        if (InPositionTimeoutFault) faults += "In Position Timeout Fault\n";
        if (VoltageClampFault) faults += "Voltage Clamp Fault\n";
        if (MotorSupplyFault) faults += "Motor Supply Fault\n";
        if (InternalFault) faults += "Internal Fault\n";

        *attr_faults_read = Tango::string_dup(faults.c_str());
        att.set_value(attr_faults_read);
    }

    void Axis::read_accelerating(Tango::Attribute& att)
    {
        std::lock_guard<std::mutex> lk(Controller_ns::ControllerClass::instance()->mutex);
        auto [enabled, cw_end_of_travel_limit_input, ccw_end_of_travel_limit_input, emergency_stop_input, accelerating,
            decelerating, move_active] = get_drive_status();
        *attr_accelerating_read = accelerating;
        att.set_value(attr_accelerating_read);
    }

    bool Axis::is_accelerating_allowed(Tango::AttReqType ty)
    {
        return true;
    }

    void Axis::fault_ack_all()
    {
        std::lock_guard lk(Controller_ns::ControllerClass::instance()->mutex);
        DEBUG_STREAM << "Axis::fault_ack()  - " << device_name << std::endl;
        if (const auto response = Automation1_Command_AcknowledgeAll(
            Controller_ns::ControllerClass::instance()->controller,
            1); !response)
        {
            char msg[100];
            Automation1_GetLastErrorMessage(msg, 100);
            ERROR_STREAM << msg << std::endl;
        }
    }

    bool Axis::is_faultack_all_allowed(const CORBA::Any& any)
    {
        return true;
    }

    std::list<int> Axis::get_gantry_sub_axes()
    {
        double mask;
        Automation1_Parameter_GetAxisValue(Controller_ns::ControllerClass::instance()->controller, axisID,
                                           Automation1AxisParameterId_GantryAxisMask, &mask);
        const int axisMask = mask;
        std::cout << axisName << '\t' << axisID << '\t' << axisMask << std::endl;

        std::list<int> axes;
        if (axisMask == 0)
            return axes;
        const auto nAxes = static_cast<short>(Automation1_Controller_AvailableAxisCount(
            Controller_ns::ControllerClass::instance()->controller));
        for (short i = 0; i < nAxes; i++)
        {
            if (axisMask & (1 << i) and (axisID != i))
                axes.emplace_back(i);
        }
        return axes;
    }


    AxisStatus Axis::get_axis_status() const
    {
        double results[axisStates.size()];
        if (Controller_ns::ControllerClass::instance()->controller == nullptr)
            Tango::Except::throw_exception("Controller not connected", "The conctroller is not connected",
                                           "get_axis_status()");
        Automation1_Status_GetResults(Controller_ns::ControllerClass::instance()->controller,
                                      statusConfig,
                                      results,
                                      static_cast<int>(axisStates.size()));


        const auto axis_status = static_cast<int>(results[axisStates.at(Automation1AxisStatusItem_AxisStatus)]);

        return {
            .homed = static_cast<bool>(axis_status & Automation1AxisStatus::Automation1AxisStatus_Homed),
            .profiling = static_cast<bool>(axis_status & Automation1AxisStatus::Automation1AxisStatus_Profiling),
            .homing = static_cast<bool>(axis_status & Automation1AxisStatus::Automation1AxisStatus_Homing),
            .jogging = static_cast<bool>(axis_status & Automation1AxisStatus::Automation1AxisStatus_Jogging),
            .not_virtual = static_cast<bool>(axis_status & Automation1AxisStatus::Automation1AxisStatus_NotVirtual),
            .motion_done = static_cast<bool>(axis_status & Automation1AxisStatus::Automation1AxisStatus_MotionDone),
            .motion_clamped = static_cast<bool>(axis_status &
                Automation1AxisStatus::Automation1AxisStatus_MotionClamped),
            .gantry_aligned = static_cast<bool>(axis_status &
                Automation1AxisStatus::Automation1AxisStatus_GantryAligned),
            .calibration_enabled_1d = static_cast<bool>(axis_status &
                Automation1AxisStatus::Automation1AxisStatus_CalibrationEnabled1D),
            .calibration_enabled_2d = static_cast<bool>(axis_status &
                Automation1AxisStatus::Automation1AxisStatus_CalibrationEnabled2D)
        };;
    }

    AxisFaults Axis::get_axis_faults() const
    {
        double results[axisStates.size()];
        if (Controller_ns::ControllerClass::instance()->controller == nullptr)
            Tango::Except::throw_exception("Controller not connected", "The conctroller is not connected",
                                           "get_axis_status()");
        Automation1_Status_GetResults(Controller_ns::ControllerClass::instance()->controller,
                                      statusConfig,
                                      results,
                                      static_cast<int>(axisStates.size()));


        const auto axis_faults = static_cast<int>(results[axisStates.at(Automation1AxisStatusItem_AxisFault)]);
        return {
            .anyFault = static_cast<bool>(axis_faults),
            .positionErrorFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_PositionErrorFault),
            .OverCurrentFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_OverCurrentFault),
            .CwEndOfTravelLimitFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_CwEndOfTravelLimitFault),
            .CcwEndOfTravelLimitFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_CcwEndOfTravelLimitFault),
            .CwSoftwareLimitFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_CwSoftwareLimitFault),
            .CcwSoftwareLimitFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_CcwSoftwareLimitFault),
            .AmplifierFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_AmplifierFault),
            .FeedbackInput0Fault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_FeedbackInput0Fault),
            .FeedbackInput1Fault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_FeedbackInput1Fault),
            .HallSensorFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_HallSensorFault),
            .MaxVelocityCommandFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_MaxVelocityCommandFault),
            .EmergencyStopFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_EmergencyStopFault),
            .VelocityErrorFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_VelocityErrorFault),
            .CommutationFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_CommutationFault),
            .ExternalFault = static_cast<bool>(axis_faults & Automation1AxisFault::Automation1AxisFault_ExternalFault),
            .MotorTemperatureFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_MotorTemperatureFault),
            .AmplifierTemperatureFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_AmplifierFault),
            .EncoderFault = static_cast<bool>(axis_faults & Automation1AxisFault::Automation1AxisFault_EncoderFault),
            .GantryMisalignmentFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_GantryMisalignmentFault),
            .FeedbackScalingFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_FeedbackScalingFault),
            .MarkerSearchFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_MarkerSearchFault),
            .SafeZoneFault = static_cast<bool>(axis_faults & Automation1AxisFault::Automation1AxisFault_SafeZoneFault),
            .InPositionTimeoutFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_InPositionTimeoutFault),
            .VoltageClampFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_VoltageClampFault),
            .MotorSupplyFault = static_cast<bool>(axis_faults &
                Automation1AxisFault::Automation1AxisFault_MotorSupplyFault),
            .InternalFault = static_cast<bool>(axis_faults & Automation1AxisFault::Automation1AxisFault_InternalFault)
        };
    }

    DriveStatus Axis::get_drive_status() const
    {
        double results[axisStates.size()];
        if (Controller_ns::ControllerClass::instance()->controller == nullptr)
            Tango::Except::throw_exception("Controller not connected", "The conctroller is not connected",
                                           "get_drive_status()");
        Automation1_Status_GetResults(Controller_ns::ControllerClass::instance()->controller,
                                      statusConfig,
                                      results,
                                      static_cast<int>(sizeof(results) / sizeof(double)));

        const auto drive_status = static_cast<int>(results[axisStates.at(Automation1AxisStatusItem_DriveStatus)]);

        return {
            .enabled = static_cast<bool>(drive_status & Automation1DriveStatus::Automation1DriveStatus_Enabled),
            .cw_end_of_travel_limit_input = static_cast<bool>(drive_status &
                Automation1DriveStatus::Automation1DriveStatus_CwEndOfTravelLimitInput),
            .ccw_end_of_travel_limit_input = static_cast<bool>(drive_status &
                Automation1DriveStatus::Automation1DriveStatus_CcwEndOfTravelLimitInput),
            .emergency_stop_input = static_cast<bool>(drive_status &
                Automation1DriveStatus::Automation1DriveStatus_EmergencyStopInput),
            .accelerating = static_cast<bool>(drive_status &
                Automation1DriveStatus::Automation1DriveStatus_AccelerationPhase),
            .decelerating = static_cast<bool>(drive_status &
                Automation1DriveStatus::Automation1DriveStatus_DecelerationPhase),
            .move_active = static_cast<bool>(drive_status &
                Automation1DriveStatus::Automation1DriveStatus_MoveActive)
        };
    }
}
