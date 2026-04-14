# CMake Build Script for ToTheMoonPuzzleGame (Windows)
# Usage: .\build_cmake.ps1 [Debug|Release] [Generator]

param(
    [string]$BuildType = "Debug",
    [string]$Generator = ""
)

$ErrorActionPreference = "Stop"

if ($BuildType -notmatch "^(Debug|Release|RelWithDebInfo|MinSizeRel)$") {
    Write-Error "Invalid build type. Use: Debug, Release, RelWithDebInfo, or MinSizeRel"
    exit 1
}

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$BuildDir = Join-Path $ScriptDir "build"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "ToTheMoonPuzzleGame CMake Build" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Build Type: $BuildType" -ForegroundColor Yellow
Write-Host "Build Directory: $BuildDir" -ForegroundColor Yellow
Write-Host ""

if (-not (Test-Path $BuildDir)) {
    Write-Host "Creating build directory..." -ForegroundColor Green
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Set-Location $BuildDir

$CMakeArgs = @("-S", $ScriptDir, "-B", ".")

if ($Generator -ne "") {
    $CMakeArgs += @("-G", $Generator)
} else {
    $CMakeArgs += @("-G", "MinGW Makefiles")
}

Write-Host "Configuring with CMake..." -ForegroundColor Green
cmake @CMakeArgs -DCMAKE_BUILD_TYPE=$BuildType

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed!"
    exit 1
}

Write-Host "Building..." -ForegroundColor Green
cmake --build . --config $BuildType --parallel

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed!"
    exit 1
}

Write-Host ""
Write-Host "Build completed successfully!" -ForegroundColor Green
Write-Host "Executable: $BuildDir\ToTheMoonPuzzleGame.exe" -ForegroundColor Cyan
