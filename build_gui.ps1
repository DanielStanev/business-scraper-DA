<#
.SYNOPSIS
  Build Business Scraper GUI for Windows using CMake and MSVC.

.PARAMETER QtDir
  Root Qt directory containing bin/windeployqt.exe (e.g. C:\Qt\6.9.2\msvc2022_64)

.PARAMETER VcpkgRoot
  Path to vcpkg root (default: C:\vcpkg)

.PARAMETER Rebuild
  If set, deletes existing build folder before configuring.

.EXAMPLE
  powershell -ExecutionPolicy Bypass -File build_gui.ps1 -QtDir "C:\Qt\6.9.2\msvc2022_64"
#>
param(
  [Parameter(Mandatory=$true)][string]$QtDir,
  [string]$VcpkgRoot = "C:\vcpkg",
  [switch]$Rebuild
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Write-Info($m){ Write-Host "[INFO] $m" -ForegroundColor Cyan }
function Write-Step($m){ Write-Host "`n=== $m ===" -ForegroundColor Green }

$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $Root

if(!(Test-Path $QtDir)){ throw "QtDir not found: $QtDir" }
$WinDeployQt = Join-Path $QtDir 'bin/windeployqt.exe'
if(!(Test-Path $WinDeployQt)){ throw "windeployqt not found at $WinDeployQt" }

$BuildDir = Join-Path $Root "build-windows"
if($Rebuild -and (Test-Path $BuildDir)){
  Write-Step "Rebuild requested - deleting $BuildDir"
  Remove-Item -Recurse -Force $BuildDir
}
if(!(Test-Path $BuildDir)){ New-Item -ItemType Directory -Path $BuildDir | Out-Null }

# Set CMAKE_PREFIX_PATH to help CMake find Qt6Config.cmake
$CMakePrefixPath = $QtDir
if (!(Test-Path (Join-Path $CMakePrefixPath 'lib\cmake\Qt6\Qt6Config.cmake'))){
  Write-Info "Warning: Qt6Config.cmake not found in $CMakePrefixPath/lib/cmake/Qt6."
}
Write-Step "Configuring CMake (Release) with CMAKE_PREFIX_PATH=$CMakePrefixPath and vcpkg toolchain"
$VcpkgToolchain = Join-Path $VcpkgRoot 'scripts\buildsystems\vcpkg.cmake'
if (!(Test-Path $VcpkgToolchain)) {
  Write-Info "Warning: vcpkg toolchain file not found at $VcpkgToolchain. Continuing without it."
  cmake -G "Visual Studio 17 2022" -A x64 -S $Root -B $BuildDir -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$CMakePrefixPath"
} else {
  cmake -G "Visual Studio 17 2022" -A x64 -S $Root -B $BuildDir -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$CMakePrefixPath" -DCMAKE_TOOLCHAIN_FILE="$VcpkgToolchain"
}

Write-Step "Building GUI target"
cmake --build $BuildDir --config Release --target business_scraper_gui --parallel

$ExeDir = Join-Path $BuildDir "bin/Release"
$GuiExe = Join-Path $ExeDir 'business_scraper_gui.exe'
if(!(Test-Path $GuiExe)){ throw "GUI executable not found at $GuiExe" }

Write-Step "Deploying Qt runtime"
Push-Location $ExeDir
& $WinDeployQt --release --no-translations business_scraper_gui.exe
Pop-Location

Write-Info "Binary folder prepared: $ExeDir"
