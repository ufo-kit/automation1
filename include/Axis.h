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

#ifndef AUTOMATION1_AXIS_H
#define AUTOMATION1_AXIS_H

#include <map>
#include <tango/tango.h>
#include <Automation1Status.h>


namespace Axis_ns
{
    const std::map<Automation1AxisStatusItem, int> axisStates = {
        {Automation1AxisStatusItem_AxisStatus, 0},
        {Automation1AxisStatusItem_DriveStatus, 1},
        {Automation1AxisStatusItem_ProgramPositionCommand, 2},
        {Automation1AxisStatusItem_ProgramPositionFeedback, 3},
        {Automation1AxisStatusItem_ProgramVelocityCommand, 4},
        {Automation1AxisStatusItem_ProgramVelocityFeedback, 5},
        {Automation1AxisStatusItem_AxisFault, 6}
    };

    struct DriveStatus
    {
        bool enabled;
        bool cw_end_of_travel_limit_input;
        bool ccw_end_of_travel_limit_input;
        bool emergency_stop_input;
        bool accelerating;
        bool decelerating;
        bool move_active;
    };

    struct AxisStatus
    {
        bool homed;
        bool profiling;
        bool homing;
        bool jogging;
        bool not_virtual;
        bool motion_done;
        bool motion_clamped;
        bool gantry_aligned;
        bool calibration_enabled_1d;
        bool calibration_enabled_2d;
    };

    struct AxisFaults
    {
        bool anyFault;
        bool positionErrorFault;
        bool OverCurrentFault;
        bool CwEndOfTravelLimitFault;
        bool CcwEndOfTravelLimitFault;
        bool CwSoftwareLimitFault;
        bool CcwSoftwareLimitFault;
        bool AmplifierFault;
        bool FeedbackInput0Fault;
        bool FeedbackInput1Fault;
        bool HallSensorFault;
        bool MaxVelocityCommandFault;
        bool EmergencyStopFault;
        bool VelocityErrorFault;
        bool CommutationFault;
        bool ExternalFault;
        bool MotorTemperatureFault;
        bool AmplifierTemperatureFault;
        bool EncoderFault;
        bool GantryMisalignmentFault;
        bool FeedbackScalingFault;
        bool MarkerSearchFault;
        bool SafeZoneFault;
        bool InPositionTimeoutFault;
        bool VoltageClampFault;
        bool MotorSupplyFault;
        bool InternalFault;
    };

    class Axis final : public TANGO_BASE_CLASS
    {
    public:
        Axis(Tango::DeviceClass* cl, const std::string& s);

        Axis(Tango::DeviceClass* cl, const char* s);

        Axis(Tango::DeviceClass* cl, const char* s, const char* d);

        ~Axis() override;

        Tango::DevState dev_state() override;

        const char* dev_status() override;

        bool is_fault_allowed(Tango::AttReqType ty);

        void read_faults(Tango::Attribute& att);

        void read_accelerating(Tango::Attribute& att);

        bool is_accelerating_allowed(Tango::AttReqType ty);

        void fault_ack_all();

        bool is_faultack_all_allowed(const CORBA::Any& any);

        std::list<int> get_gantry_sub_axes();

        Tango::DevDouble* attr_position_read{};
        Tango::DevDouble* attr_position_target_read{};

        Tango::DevDouble* attr_motion_velocity{};

        Tango::DevString* attr_faults_read{};

        Tango::DevBoolean* attr_accelerating_read{};

        Tango::DevDouble* attr_positive_softlimit_read{};
        Tango::DevDouble* attr_negative_softlimit_read{};

        Tango::DevBoolean* attr_positive_hard_limit_read{};
        Tango::DevBoolean* attr_negative_hard_limit_read{};


        void delete_device() override;

        void init_device() override;

        void get_device_property();

        void always_executed_hook() override;

        void read_attr_hardware(std::vector<long>& attr_list) override;

        static void add_dynamic_attributes();

        void enable();

        void disable();

        void stop();

        void home();

        bool is_home_allowed(const CORBA::Any& type);

        bool is_stop_allowed(const CORBA::Any& type);

        bool is_enable_allowed(const CORBA::Any& type);

        bool is_disable_allowed(const CORBA::Any& type);

        void fault_ack();

        bool is_fault_ack_allowed(const CORBA::Any& type);

        static void add_dynamic_commands();

        void read_position(Tango::Attribute& attribute) const;

        void read_position_target(Tango::Attribute& attribute) const;

        void write_position(Tango::WAttribute& attribute);

        bool is_position_allowed(Tango::AttReqType type);

        void read_positive_hard_limit(Tango::Attribute& att);

        bool is_positive_hard_limit_allowed(Tango::AttReqType ty);

        void read_negative_hard_limit(Tango::Attribute& att);

        bool is_negative_hard_limit_allowed(Tango::AttReqType ty);

        bool is_position_target_allowed(Tango::AttReqType type);

        bool is_negative_softlimit_allowed(Tango::AttReqType type);

        bool is_positive_softlimit_allowed(Tango::AttReqType type);

        void read_positive_softlimit(Tango::Attribute& attr);

        void write_positive_softlimit(Tango::WAttribute& attr);

        void read_negative_softlimit(Tango::Attribute& attr);

        void write_negative_softlimit(Tango::WAttribute& attr);

        void read_motion_velocity(Tango::Attribute& attribute) const;

        void write_motion_velocity(Tango::WAttribute& attribute);

        static bool is_motion_velocity_allowed(Tango::AttReqType type);

        void freerun(Tango::DevDouble arg_in);

        bool is_freerun_allowed(const CORBA::Any& type);

        [[nodiscard]] AxisStatus get_axis_status() const;

        [[nodiscard]] AxisFaults get_axis_faults() const;

        [[nodiscard]] DriveStatus get_drive_status() const;

    private:
        [[nodiscard]] double counts_to_user_unit(double counts) const;

        [[nodiscard]] double user_unit_to_counts(double user_unit) const;

        Automation1StatusConfig statusConfig{};

        int axisID{};

        std::string axisName{};

        std::string status{};
    };
}
#endif   //	AUTOMATION1_AXIS_H
