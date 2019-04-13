# Gamepad-F710

A simple class for rapid prototyping robotics applications on Linux with the F710 Logitech Gamepad. 

## Usage

```
Gamepad gamepad;

// Pass in true to have the class try to automatically recover from errors
// by closing and reopening the input file
gamepad.Start(true);

Gamepad::GamepadEvent event;
bool success = gamepad.Read(event);
```

 Exception handeling is not used for speed and to avoid too many
 issues with ROS transfering between sim and real environments. On failiure, you
 need to re-call `Start()`, unless `auto_restart` is enabled, in which case it 
 will attempt to close and reopen the `/dev/input/js0` file automatically.