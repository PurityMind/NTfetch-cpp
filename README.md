# NTfetch-cpp

> \[!WARNING]
> This project is barely held together by duct tape and prayers.
> Expect weird bugs, undocumented Win32 quirks, and potential crashes.

## Displayed Metrics

- Username & Hostname
- OS Distribution, Build & Architecture
- Kernel/NT Version

## Building
```cmd
cl /EHsc /std:c++17 /O2 main.cpp ntinfo.cpp /Fe:<any name that u want idk>.exe advapi32.lib
```
