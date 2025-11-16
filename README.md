# LibSys 

Library Management System Software, LibSys Installer, LibSys Updater
© 2025 – Habil Eren Türker

## About the Project

LibSys is an open-source library management system written in C++ that handles book borrow operations, user management, and catalog operations.

> ⚠️ **Attention:** This project is still under test. You may encounter bugs and crashes. I, Habil Eren Türker, created this open-source project for educational purposes.
> My main goal during the development of this project was creating a software which my school ([Çemberlitaş Anatolian High School](https:/cemberlitasanadolu.meb.k12.tr/)) can use in its library.
---

## Features

* Book management (add, remove, update)
* User management (add member, remove member, update member info)
* Borrow and return operations
* Basic search and filtering functionality
* Modular and extensible architecture
* User-friendly GUI
* RFID support

## Technology & Structure

* Programming Language: C++
* Build system: CMake + Makefile
* License: GNU AGPL v3.0 ([License](https://www.gnu.org/licenses/agpl-3.0.en.html))

## Installation

If you are using Windows, you must install mingw-w64/MSYS2 and tools G++, CMake and Makefile.
Also you must install Qt Framework (Version at least 6.9.2).

You can find more information here:
([Qt](https://www.qt.io/product/framework))
([MSYS2](https://www.msys2.org/))
([mingw-w64](https://www.mingw-w64.org/))

Also you must change all CMakeLists.txt files' 
```bash
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.2/mingw_64")
```
part with the path you have Qt installed.

Follow the steps below to run the project on your machine:

```bash
git clone https://github.com/herenturker/LibSys.git  
cd LibSys  
mkdir project && cd project  
make build # Create CMake build files. If you want to build files a seperate, run make <program_name>. Example: make LibSys
./LibSys   # or the generated executable name 
```

## Project Directory Structure

```
/LibSys
│  
├─ LibSys/            # Main source code of LibSys
├─ LibSysInstaller/   # LibSys Installer Source Codes 
├─ LibSysUpdater/     # LibSys Updater Source Codes 
├─ .vscode/           # VS Code settings  
├─ .github/           # GitHub Workflow .yml Files
├─ Makefile           # Build script
├─ docs/images        # Documents and images for this project
|  └─ USAGE.md        # Usage document for this project.
|  └─ ARDUINO.md      # Arduino RFID reader connections document for this project.
├─ LICENSE            # License of the project
├─ Arduino/           # Arduino code for RFID Data reading
├─ README.md          # README file for this project
└─ version.json       # Current version of LibSys
```

## Usage

See ([Usage](docs/USAGE.md))
When you run LibSys, program would create databases directory with databases/users.db and databases/library.db and a log.log in program directory.
> Do not delete these files.

## Contributing

Contributions are welcome! Please follow these steps:

* Fork the repository and create your branch.
* Commit and push your changes.
* Open a pull request.
* Ensure your code is tested and readable.

## Testing

Manual testing scenarios:

* Add a new book → should appear in the database
* Add a new member → should be able to borrow books
* Lend a book → member and book status updated
* Return a book → lending status reset

## License

This project is licensed under GNU AGPL v3.0. See the `LICENSE` file for details. ([GitHub link](LICENSE))

This project uses Qt 6.9.2 which is licensed under GNU LGPL v3.0. See ([Qt License](https://doc.qt.io/qt-6/lgpl.html)) for more information.

This project uses serialib library which is licensed under MIT License. See serialib project ([serialib link](https://github.com/imabot2/serialib)).

This project uses AES library which is licensed under MIT License. See AES project ([AES link](https://github.com/SergeyBel/AES)).

See ([MIT License](https://mit-license.org/))

## Contact

For questions or suggestions, reach out via habilerenturker@hotmail.com .
