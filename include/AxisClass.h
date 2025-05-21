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

#ifndef AUTOMATION1_AXIS_CLASS_H
#define AUTOMATION1_AXIS_CLASS_H

#include <tango/tango.h>
#include "Axis.h"


namespace Axis_ns
{
    /**
     * Position attribute
     */
    class positionAttrib final : public Tango::Attr
    {
    public:
        positionAttrib() : Attr("position",
                                Tango::DEV_DOUBLE, Tango::READ_WRITE)
        {
        };

        ~positionAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Axis*>(dev))->read_position(att); }

        void write(Tango::DeviceImpl* dev,
                   Tango::WAttribute& att) override { (dynamic_cast<Axis*>(dev))->write_position(att); }

        bool is_allowed(Tango::DeviceImpl* dev, const Tango::AttReqType ty) override
        {
            return (dynamic_cast<Axis*>(dev))->is_position_allowed(ty);
        }
    };


    /**
     * Attribute for the positive soft limit
     */
    class positiveSoftlimitAttrib final : public Tango::Attr
    {
    public:
        positiveSoftlimitAttrib() : Attr("positive_softlimit",
                                         Tango::DEV_DOUBLE, Tango::READ_WRITE)
        {
        };

        ~positiveSoftlimitAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Axis*>(dev))->read_positive_softlimit(att); }

        void write(Tango::DeviceImpl* dev,
                   Tango::WAttribute& att) override { (dynamic_cast<Axis*>(dev))->write_positive_softlimit(att); }

        bool is_allowed(Tango::DeviceImpl* dev, const Tango::AttReqType ty) override
        {
            return (dynamic_cast<Axis*>(dev))->is_positive_softlimit_allowed(ty);
        }
    };

    /**
     * Attribute for the negative soft limit
     */
    class negativeSoftlimitAttrib final : public Tango::Attr
    {
    public:
        negativeSoftlimitAttrib() : Attr("negative_softlimit",
                                         Tango::DEV_DOUBLE, Tango::READ_WRITE)
        {
        };

        ~negativeSoftlimitAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Axis*>(dev))->read_negative_softlimit(att); }

        void write(Tango::DeviceImpl* dev,
                   Tango::WAttribute& att) override { (dynamic_cast<Axis*>(dev))->write_negative_softlimit(att); }

        bool is_allowed(Tango::DeviceImpl* dev, const Tango::AttReqType ty) override
        {
            return (dynamic_cast<Axis*>(dev))->is_negative_softlimit_allowed(ty);
        }
    };

    /**
     * Attribute for the accelerating state
     */
    class acceleratingAttrib final : public Tango::Attr
    {
    public:
        acceleratingAttrib() : Attr("accelerating",
                                    Tango::DEV_BOOLEAN, Tango::READ)
        {
        };

        ~acceleratingAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Axis*>(dev))->read_accelerating(att); }


        bool is_allowed(Tango::DeviceImpl* dev, const Tango::AttReqType ty) override
        {
            return (dynamic_cast<Axis*>(dev))->is_accelerating_allowed(ty);
        }
    };

    /**
     * Attribute to read the faults
     */
    class faultsAttrib final : public Tango::Attr
    {
    public:
        faultsAttrib() : Attr("faults",
                              Tango::DEV_STRING, Tango::READ)
        {
        };

        ~faultsAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Axis*>(dev))->read_faults(att); }

        bool is_allowed(Tango::DeviceImpl* dev, const Tango::AttReqType ty) override
        {
            return (dynamic_cast<Axis*>(dev))->is_fault_allowed(ty);
        }
    };

    /**
     * Attribute to read the current position target
     */
    class positionTargetAttrib final : public Tango::Attr
    {
    public:
        positionTargetAttrib() : Attr("position_command",
                                      Tango::DEV_DOUBLE, Tango::READ)
        {
        };

        ~positionTargetAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Axis*>(dev))->read_position_target(att); }


        bool is_allowed(Tango::DeviceImpl* dev, const Tango::AttReqType ty) override
        {
            return (dynamic_cast<Axis*>(dev))->is_position_target_allowed(ty);
        }
    };

    /**
     * Attribute to read the current motion velocity
     *
     * The motion velocity is the target velocity when a position change is requested.
     */
    class motionVelocityAttrib final : public Tango::Attr
    {
    public:
        motionVelocityAttrib() : Attr("motion_velocity",
                                      Tango::DEV_DOUBLE, Tango::READ_WRITE)
        {
        };

        ~motionVelocityAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Axis*>(dev))->read_motion_velocity(att); }

        void write(Tango::DeviceImpl* dev,
                   Tango::WAttribute& att) override { (dynamic_cast<Axis*>(dev))->write_motion_velocity(att); }

        bool is_allowed(Tango::DeviceImpl* dev, const Tango::AttReqType ty) override
        {
            return Axis_ns::Axis::is_motion_velocity_allowed(ty);
        }
    };

    /**
     * Attribute to read the positive hard limit
     */
    class positiveHardLimitAttrib final : public Tango::Attr
    {
    public:
        positiveHardLimitAttrib() : Attr("positive_hardlimit",
                                         Tango::DEV_BOOLEAN, Tango::READ)
        {
        };

        ~positiveHardLimitAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Axis*>(dev))->read_positive_hard_limit(att); }

        bool is_allowed(Tango::DeviceImpl* dev, const Tango::AttReqType ty) override
        {
            return (dynamic_cast<Axis*>(dev))->is_positive_hard_limit_allowed(ty);
        }
    };

    /**
     * Attribute to read the negative hard limit
     */
    class negativeHardLimitAttrib final : public Tango::Attr
    {
    public:
        negativeHardLimitAttrib() : Attr("negative_hardlimit",
                                         Tango::DEV_BOOLEAN, Tango::READ)
        {
        };

        ~negativeHardLimitAttrib() override = default;

        void read(Tango::DeviceImpl* dev,
                  Tango::Attribute& att) override { (dynamic_cast<Axis*>(dev))->read_negative_hard_limit(att); }

        bool is_allowed(Tango::DeviceImpl* dev, const Tango::AttReqType ty) override
        {
            return (dynamic_cast<Axis*>(dev))->is_negative_hard_limit_allowed(ty);
        }
    };

    /**
     * Command to enable the axis
     */
    class EnableCommand final : public Tango::Command
    {
    public:
        EnableCommand(const char* cmd_name,
                      const Tango::CmdArgType in,
                      const Tango::CmdArgType out,
                      const char* in_desc,
                      const char* out_desc,
                      const Tango::DispLevel level)
            : Command(cmd_name, in, out, in_desc, out_desc, level)
        {
        };

        EnableCommand(const char* cmd_name,
                      const Tango::CmdArgType in,
                      const Tango::CmdArgType out)
            : Command(cmd_name, in, out)
        {
        };

        ~EnableCommand() override = default;

        CORBA::Any* execute(Tango::DeviceImpl* dev, const CORBA::Any& any) override;

        bool is_allowed(Tango::DeviceImpl* dev, const CORBA::Any& any) override
        {
            return (dynamic_cast<Axis*>(dev))->is_enable_allowed(any);
        }
    };

    /**
     * Command to acknowledge all faults
     */
    class FaultAckAllCommand final : public Tango::Command
    {
    public:
        FaultAckAllCommand(const char* cmd_name,
                           const Tango::CmdArgType in,
                           const Tango::CmdArgType out,
                           const char* in_desc,
                           const char* out_desc,
                           const Tango::DispLevel level)
            : Command(cmd_name, in, out, in_desc, out_desc, level)
        {
        };

        FaultAckAllCommand(const char* cmd_name,
                           const Tango::CmdArgType in,
                           const Tango::CmdArgType out)
            : Command(cmd_name, in, out)
        {
        };

        ~FaultAckAllCommand() override = default;

        CORBA::Any* execute(Tango::DeviceImpl* dev, const CORBA::Any& any) override;

        bool is_allowed(Tango::DeviceImpl* dev, const CORBA::Any& any) override
        {
            return (dynamic_cast<Axis*>(dev))->is_faultack_all_allowed(any);
        }
    };

    /**
     * Command to start the axis in freerun mode
     *
     * This command is used to run an axis with a constant velocity.
     * Check the accelerating attribute to see if the axis has finished the acceleration phase.
     */
    class FreerunCommand final : public Tango::Command
    {
    public:
        FreerunCommand(const char* cmd_name,
                       const Tango::CmdArgType in,
                       const Tango::CmdArgType out,
                       const char* in_desc,
                       const char* out_desc,
                       const Tango::DispLevel level)
            : Command(cmd_name, in, out, in_desc, out_desc, level)
        {
        };

        FreerunCommand(const char* cmd_name,
                       const Tango::CmdArgType in,
                       const Tango::CmdArgType out)
            : Command(cmd_name, in, out)
        {
        };

        ~FreerunCommand() override = default;

        CORBA::Any* execute(Tango::DeviceImpl* dev, const CORBA::Any& any) override;

        bool is_allowed(Tango::DeviceImpl* dev, const CORBA::Any& any) override
        {
            return (dynamic_cast<Axis*>(dev))->is_freerun_allowed(any);
        }
    };

    /**
     * Command to stop the axis
     */
    class StopCommand final : public Tango::Command
    {
    public:
        StopCommand(const char* cmd_name,
                    const Tango::CmdArgType in,
                    const Tango::CmdArgType out,
                    const char* in_desc,
                    const char* out_desc,
                    const Tango::DispLevel level)
            : Command(cmd_name, in, out, in_desc, out_desc, level)
        {
        };

        StopCommand(const char* cmd_name,
                    const Tango::CmdArgType in,
                    const Tango::CmdArgType out)
            : Command(cmd_name, in, out)
        {
        };

        ~StopCommand() override = default;

        CORBA::Any* execute(Tango::DeviceImpl* dev, const CORBA::Any& any) override;

        bool is_allowed(Tango::DeviceImpl* dev, const CORBA::Any& any) override
        {
            return (dynamic_cast<Axis*>(dev))->is_stop_allowed(any);
        }
    };

    /**
     * Command to home the axis
     */
    class HomeCommand final : public Tango::Command
    {
    public:
        HomeCommand(const char* cmd_name,
                    const Tango::CmdArgType in,
                    const Tango::CmdArgType out,
                    const char* in_desc,
                    const char* out_desc,
                    const Tango::DispLevel level)
            : Command(cmd_name, in, out, in_desc, out_desc, level)
        {
        };

        HomeCommand(const char* cmd_name,
                    const Tango::CmdArgType in,
                    const Tango::CmdArgType out)
            : Command(cmd_name, in, out)
        {
        };

        ~HomeCommand() override = default;

        CORBA::Any* execute(Tango::DeviceImpl* dev, const CORBA::Any& any) override;

        bool is_allowed(Tango::DeviceImpl* dev, const CORBA::Any& any) override
        {
            return (dynamic_cast<Axis*>(dev))->is_home_allowed(any);
        }
    };

    /**
     * Command to disable the axis
     */
    class DisableCommand final : public Tango::Command
    {
    public:
        DisableCommand(const char* cmd_name,
                       const Tango::CmdArgType in,
                       const Tango::CmdArgType out,
                       const char* in_desc,
                       const char* out_desc,
                       const Tango::DispLevel level)
            : Command(cmd_name, in, out, in_desc, out_desc, level)
        {
        };

        DisableCommand(const char* cmd_name,
                       const Tango::CmdArgType in,
                       const Tango::CmdArgType out)
            : Command(cmd_name, in, out)
        {
        };

        ~DisableCommand() override = default;

        CORBA::Any* execute(Tango::DeviceImpl* dev, const CORBA::Any& any) override;

        bool is_allowed(Tango::DeviceImpl* dev, const CORBA::Any& any) override
        {
            return (dynamic_cast<Axis*>(dev))->is_disable_allowed(any);
        }
    };

    /**
     * Command to acknowledge a fault
     */
    class FaultAckCommand final : public Tango::Command
    {
    public:
        FaultAckCommand(const char* cmd_name,
                        const Tango::CmdArgType in,
                        const Tango::CmdArgType out,
                        const char* in_desc,
                        const char* out_desc,
                        const Tango::DispLevel level)
            : Command(cmd_name, in, out, in_desc, out_desc, level)
        {
        };

        FaultAckCommand(const char* cmd_name,
                        const Tango::CmdArgType in,
                        const Tango::CmdArgType out)
            : Command(cmd_name, in, out)
        {
        };

        ~FaultAckCommand() override = default;

        CORBA::Any* execute(Tango::DeviceImpl* dev, const CORBA::Any& any) override;

        bool is_allowed(Tango::DeviceImpl* dev, const CORBA::Any& any) override
        {
            return (dynamic_cast<Axis*>(dev))->is_fault_ack_allowed(any);
        }
    };


    class AxisClass final : public Tango::DeviceClass
    {
    public:
        Tango::DbData cl_prop;
        Tango::DbData cl_def_prop;
        Tango::DbData dev_def_prop;

        static AxisClass* init(const char*);

        static AxisClass* instance();

        ~AxisClass() override;

        Tango::DbDatum get_class_property(std::string&);

        Tango::DbDatum get_default_device_property(std::string&);

        Tango::DbDatum get_default_class_property(std::string&);

    protected:
        explicit AxisClass(const std::string&);

        static AxisClass* _instance;

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
