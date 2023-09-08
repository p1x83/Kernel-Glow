# KDMP - memory and process hook in a kernel driver
This Driver is able to open a handle to CS:GO and later on modify it's memory to show enemys on the radar and also show glow for enemys

## Starting the driver
Windows doesn't allow to load an unsigned driver into the kernel, so just use [KDMapper](https://github.com/TheCruZ/kdmapper)
Compile the project and drag the .sys file onto the KDMapper.exe

## Troubleshooting

### Driver makes the PC bluescreen (system exception)
This driver is specifically built for the Windows 10 21H1 eProcess offsets
Basically you just have to update the offsets in Process::FindProcessByName
Updated offsets can be found [here](https://www.vergiliusproject.com/kernels/x64)

### Glow doesn't appear
Update the offsets in offsets.h using [hazedumper](https://github.com/frk1/hazedumper/blob/master/csgo.hpp)
