# LibSys 

Library Management System Software
© 2025 – Habil Eren Türker

## 🔍 About the Project

LibSys is an open-source library management system written in C++ that handles book lending, user management, and catalog operations.

⚠️ **Attention:** This project is still under test. You may encounter bugs and crashes. 

## Features

* Book management (add, remove, update)
* User management (add member, remove member, update member info)
* Lending and return operations
* Basic search and filtering functionality
* Modular and extensible architecture
* User-friendly GUI

## 🧱 Technology & Structure

* Programming Language: C++ ([GitHub link](https://github.com/herenturker/LibSys))
* Build system: CMake + Makefile
* License: GNU AGPL v3.0 ([GitHub link](https://github.com/herenturker/LibSys))

## 🚀 Installation

Follow the steps below to run the project on your machine:

```bash
git clone https://github.com/herenturker/LibSys.git  
cd LibSys  
mkdir build && cd build  
cmake ..  
make all   # or "make Updater" OR "make Installer" OR "make LibSys"
./LibSys   # or the generated executable name 
```

## 🧩 Directory Structure

```
/LibSys
│  
├─ libsys/            # Main source code  
├─ LibSysInstaller/   # Installation scripts  
├─ LibSysUpdater/     # Update module  
├─ .vscode/           # VS Code settings  
└─ Makefile           # Build script
```

## 📘 Usage



## 👥 Contributing

Contributions are welcome! Please follow these steps:

* Fork the repository and create your branch.
* Commit and push your changes.
* Open a pull request.
* Ensure your code is tested and readable.

## 🧪 Testing

Manual testing scenarios:

* Add a new book → should appear in the database
* Add a new member → should be able to borrow books
* Lend a book → member and book status updated
* Return a book → lending status reset

## 📄 License

This project is licensed under GNU AGPL v3.0. See the `LICENSE` file for details. ([GitHub link](https://github.com/herenturker/LibSys))

This project uses Qt 6.9.2 which is licensed under GNU LGPL v3.0. See ([Qt License] (https://doc.qt.io/qt-6/lgpl.html)) for more information.

## 📝 Contact

For questions or suggestions, reach out via habilerenturker@hotmail.com .
