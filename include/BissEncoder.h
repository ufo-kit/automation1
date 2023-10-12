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

#ifndef AUTOMATION1_BISS_ENCODER_H
#define AUTOMATION1_BISS_ENCODER_H

#include <tango/tango.h>


namespace BissEncoder_ns
{
    class BissEncoder final : public TANGO_BASE_CLASS
    {
    public:
        BissEncoder(Tango::DeviceClass* cl, const std::string& s);

        BissEncoder(Tango::DeviceClass* cl, const char* s);

        BissEncoder(Tango::DeviceClass* cl, const char* s, const char* d);

        ~BissEncoder() override;

        Tango::DevState dev_state() override;

        const char* dev_status() override;

        Tango::DevDouble* attr_position_read{};

        void delete_device() override;

        void init_device() override;

        void get_device_property();

        void always_executed_hook() override;

        void read_attr_hardware(std::vector<long>& attr_list) override;

        static void add_dynamic_attributes();

        static void add_dynamic_commands();

        void read_position(Tango::Attribute& attribute);

        bool is_position_allowed(Tango::AttReqType type);

    private:
        [[nodiscard]] double counts_to_user_unit(double counts) const;

        [[nodiscard]] double user_unit_to_counts(double user_unit) const;

        int axisID{};

        std::string axisName{};

        double scale{};

        double offset{};

        std::string status{};
    };
}
#endif   //	AUTOMATION1_BISS_ENCODER_H
