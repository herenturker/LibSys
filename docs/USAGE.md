# USAGE
See ([Arduino](ARDUINO.md)) for RFID reader connections.
## Normal Login
After building project or installing it from *releases* page, you can run LibSys. 
![LibSys Login Page](images/img1.png)

## Admin Account

By default, the only user is **Admin**. These are the credentials 👇

* **Username:** Admin  
* **School Number:** 0  
* **Password:** admin

![LibSys Admin Login](images/img2.png)

You must choose account type from left buttons. Students are "Student", Admins are "Admin". Also if you do not have RFID Data (UID) for an account, choose **Normal Login**. If you have UID for an account and want to log in using UID, choose **Quick Login**.

After entering login info, click **Login** button.

---
# Admin Dashboard

![LibSys Admin Dashboard](images/img3.png)

In admin dashboard, you can
1. Add books to database by clicking **Add Book** button,
2. Delete books from database by clicking **Delete Book** button,
3. Update book info by clicking **Update Book Info** button,
4. Add user to database, by clicking **Add User** button,
5. Delete user from database by clicking **Delete User** button,
6. Update user info by clicking **Update User Info** button,
7. You can report a book as "lost" by clicking **Report Lost Book** button,
8. Display users in the database by clicking **Users** button,
9. Display books in the database by clicking **Books** button,
10. Display logs in the log.log by clicking **Log History** button.
11. Set COM for RFID serial port by clicking **COM** button.

## 1. Add Book
![Add Book](images/img4.png)
You must enter book title, ISBN and author name to add a book, and other information as optional.

![Add Book](images/img13.png)
If you enter UID for a book, a student must use UID of the book to borrow and return. If you don't enter UID, entering UID for a book in order to borrow and return would be unnecessary.

## 2. Delete Book
You must enter book title, ISBN and author name to delete a book.

## 3. Update Book Info
You must enter ISBN in order to match the book in the database and then enter information which you want to update.

## 4. Add User
![Add User](images/img5.png)
To add user, you must enter *username*, *school number*, *password* and *UID*. If you
do not want to add UID, just enter it as '0'.

## 5. Delete User
![Delete User](images/img6.png)
You must enter the username of the user you want to delete from database.

## 6. Update User Info
![Update User Info](images/img7.png)
You must enter username in order to match the user in the database and then enter information which you want to update.

## 7. Report Lost Book
You must enter the isbn of the book to report it as lost. The book's *additional info* section will be changed as 'LOST'.

## 8. Display Users
![Display Users](images/img8.png)
You can display all users and their credentials.

## 9. Display Books
![Display Books](images/img11.png)
You can display all books and their information.

![Display Book](images/img12.png)

## 10. Display Logs
![Display Logs](images/img10.png)
You can display logs in log.log .

## 11. Set Serial Port (COM)
![Set Serial Port](images/img9.png)
You can enter serial port name for using RFID and reading data from RFID reader.

## Quick Login
![Quick Login](images/img14.png)
If you have UID data entered for an user, you can use RFID reader to get UID and choose *Quick Login*, then click login.

---
# Student Dashboard
You can see borrowed books, overdue books, search for books in the library and do borrow/return operations. 

By clicking button with magnifier icon, you can for search books. By clicking the button "**Open Book Search**", you can manually enter information for searching books. Also by writing to searching bar, you can find books with titles including your search.
![Book Search](images/img15.png)

## Borrow Book
![Borrow Book](images/img16.png)
You can click *Borrow* button to borrow a book from library. UID is optional but ISBN is essential. You can choose a book up to 3 and should return them in up to 1 month. 

![Borrow Book](images/img17.png)

You can see borrowed books as shown:
![Borrow Book](images/img18.png)

## Return Book
You can click *Return* button to return a book to library. UID is optional but ISBN is essential.
![Return Book](images/img19.png)


# LibSys Installer
You can install LibSys and LibSys Updater with latest versions.
![LibSys Installer](images/img20.png)

# LibSys Updater
You can update LibSys and LibSys Installer with latest versions.
![LibSys Updater](images/img21.png)
