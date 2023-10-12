Tango device server for [Aerotech](https://www.aerotech.com/) [automation1](https://www.aerotech.com/product/software/automation1-software-based-machine-controller/) based axes.

The automation1 controller, drives, devices and the underlying API are developed and distributed by Aerotech Inc.
This project wraps this library for the usage in [tango](https://www.tango-controls.org/).

# How to build
Put the automation1 c-api header files (.h) into the folder *external/automation1/include*
and the shared libraries (*libautomation1c.so* and *libautomation1compiler.so*) to *external/automation1/lib*.
These files are distributed by Aerotech Inc.

For compilation and installation do:

    mkdir build && cd  build
    cmake ..
    make
    sudo make install

# Tango Classes

## Controller
### Device Parameters
* ip_address (str): The IP address of the controller

### Functions

### Attributes

## Axis
### Device Parameters
* axisName (str): The name of the axis.

### Functions

* enable(): Enables the axis
* disable(): Disables the axis
* home(): Starts a homing sequence
* fault_ack(): Acknowledge the current axis faults. In case of a gantry the command is sent to all gantry members.
* free_run(double velocity): Starts a continuous motion. Check the *accelerating* attribute to get the end of the acceleration phase.


### Attributes

* position(double), rw: Reads current position (in user units) or starts an absolute motion when set.
* motion_velocity(double), rw: Velocity, that is used when *position* is set (in user units per seconds).
* actual_velocity(double), r: Measured current velocity in user units per second
* accelerating (bool): Checks if the axis is currently accelerating

## BissEncoder

In the 2.9 version of the automation1 there was an issue to read out the *Biss* absolute encoders.
Therefore, this class reads out the *Automation1DriveItem_PrimaryBissAbsolutePosition* signal.
This has been fixed in the 2.10 version and the `Axis.position` can just bean used instead.

### Device Parameters
* axisName (str): The name of the axis.
* scale (double): scales the encoder resolution
* offset (double): Offset in mm

### Attributes
* position (double): Read out position

