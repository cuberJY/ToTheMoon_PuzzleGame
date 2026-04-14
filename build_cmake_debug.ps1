# CMake Build Script for ToTheMoonPuzzleGame (Debug)
# Usage: .\build_cmake_debug.ps1

$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$BuildDir = Join-Path $ScriptDir "build"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "ToTheMoonPuzzleGame Debug Build" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

if (-not (Test-Path $BuildDir)) {
    Write-Host "Creating build directory..." -ForegroundColor Green
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Set-Location $BuildDir

Write-Host "Configuring..." -ForegroundColor Green
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -S $ScriptDir -B .

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed!"
    exit 1
}

Write-Host "Building..." -ForegroundColor Green
cmake --build . --parallel

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed!"
    exit 1
}

Write-Host ""
Write-Host "Debug build completed successfully!" -ForegroundColor Green
Write-Host "Executable: $BuildDir\ToTheMoonPuzzleGame.exe" -ForegroundColor Cyan
