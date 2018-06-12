@echo off
pushd %~dp0
if not exist build\ mkdir build
cd build
cmake ../sources
popd