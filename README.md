# uawperf
UA Server for Windows Performance Monitor
## Pre-Req Software
1. Visual Studio 2022 with C++ workload
1. Python 3.10.5
1. Git for Windows
### Pre-Req
Open Developer Powershell for VS 2022  
```
cd ~
mkdir ~/prj
cd ~/prj
git clone https://github.com/open62541/open62541.git
cd open62541
git checkout v1.3
git submodule update --init --recursive
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo -DUA_NAMESPACE_ZERO=FULL -DUA_ENABLE_AMALGAMATION=ON .. -G "Visual Studio 17 2022" -A x64
```
### Initial setup
```
cd ~
mkdir prj
cd prj
git clone https://github.com/UACentral/uawperf.git
```