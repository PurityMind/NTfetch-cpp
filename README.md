# NTfetch-cpp

> \[!WARNING]
> This project is barely held together by duct tape and prayers.
> Expect weird bugs, undocumented Win32 quirks, and potential crashes.

## Displayed Metrics

- Username & Hostname
- OS Distribution, Build & Architecture
- Kernel/NT Version
- Uptime

## Building
```cmd
cl /EHsc /std:c++17 /O2 ConsoleApplication1\main.cpp ConsoleApplication1\ntinfo.cpp /Fe:ntfetch.exe advapi32.lib
```
