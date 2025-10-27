/*
    LibSys, Library Management System Software
    Copyright (C) 2025 Habil Eren Türker

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/



#include "headers/Input.h"

class InputCPP : Input

{

    UserInfo User;
    

    protected:

        void set_Username (std::string username_Entered) {
            User.username = username_Entered;
        }


        void set_SchoolNo (std::string schoolNo_Entered) {
            User.schoolNo = schoolNo_Entered;
        }


        void set_Password (std::string password_Entered) {
            User.password = password_Entered;
        }


        void set_AccountType (std::string accountType_Entered) {
            User.accountType = accountType_Entered;
        }


        bool is_Matched (std::string username, std::string schoolNo, std::string password, std::string accountType) {

        }



};