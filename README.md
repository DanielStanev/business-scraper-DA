# Business Scraper

A professional desktop application for searching and collecting business information using the Google Maps API.

## Background

I originally built this as a web application in an afternoon by combining parts from an old Perl project and an old Python project, then vibe-coding a front-end using HTML, CSS, and Node.js. The original project was created to help my friend Rhoan Egemo with his unpaid business internship, making it easier for him to gather business data.

The original web app is currently in a private GitHub repository because I naively exposed API keys in the first few commits. I'll make it public when I stop being lazy and retire those keys.

This C++ version is a complete rewrite using Qt for the desktop interface. I built it to practice making desktop applications and to create a more professional, standalone version of the tool.

## Building on Linux

Just run:
```
./build_gui.sh
```
Then run the program:
```
./build/bin/business_scraper_gui
```

## Building on Windows

1. Install prerequisites:
	- Visual Studio 2022 (Desktop C++ workload)
	- Qt 6 (e.g. 6.9.x) for MSVC (e.g. msvc2022_64)
	- vcpkg with curl, jsoncpp, zlib installed for x64-windows

2. Build the GUI app and deploy Qt runtime:
	```powershell
	powershell -ExecutionPolicy Bypass -File .\build_gui.ps1 -QtDir "C:\path\to\Qt\6.9.2\msvc2022_64" -VcpkgRoot "C:\path\to\vcpkg"
	```

3. The built program will be in:
	```
	.\build-windows\bin\Release\business_scraper_gui.exe
	```

### Creating the Windows Installer (MSI)

To create the .msi installer for Windows:

1. Open `installer/BusinessScraper.aip` with Advanced Installer 23.0.
2. Make sure your built files (from `build-windows/bin/Release`) are included in the project.
3. Press the **Build** button in Advanced Installer.
4. The resulting `.msi` installer will be in the output folder specified in the project.

## License

This project is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. See the [LICENSE](LICENSE) file for details.

Third-party components are licensed under their respective licenses. See [LICENSE_THIRD_PARTY](LICENSE_THIRD_PARTY) for details.
