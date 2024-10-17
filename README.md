# Library Management System with Self-Service Kiosk

## Overview
The **Library Management System with Self-Service Kiosk** is a software designed to manage the operations of a library, including book issues, returns, and fines. It incorporates an admin panel for managing books, users, and system activities, as well as a self-service kiosk for students to issue and return books. This system enhances efficiency by automating common library tasks and allowing for real-time tracking of book circulation.

## Features

### Admin Functionality:
1. **New Student Registration**: Admins can register new students by adding their details, ensuring only authorized users can borrow books.
2. **Show Library Activities**: View and monitor all library activities, including book issues, returns, and student interactions at kiosks.
3. **Delete Students**: Remove students who are no longer part of the system.
4. **Manage Books**: Admins can add or remove books from the library collection.
5. **View Issued Books**: Track all issued books, including borrower details and due dates.
6. **Issue Book**: Manually issue books to students when necessary.
7. **Check Defaulters / Unlock Student ID**: Manage students with overdue books or unpaid fines, and unlock their account after fine payment.

### Student Functionality:
1. **Search Book**: Search for books by title, author, or genre.
2. **Issue Book**: Use the self-service kiosk to issue books (up to 2 books per student).
3. **Return Book**: Return books via the kiosk, with real-time updates to the system.
4. **Change Password**: Securely change login credentials.
5. **Show My Activity**: Review personal library activities, including issued and returned books.
6. **Check My Due**: Check for any outstanding dues or fines.

## Database Structure

### Tables Required:
1. **Users**: Stores student and admin information.
   - `user_id (PK)`, `name`, `email`, `password`, `role`, `phone`
   
2. **Books**: Stores details of available books.
   - `book_id (PK)`, `title`, `author`, `genre`, `available_copies`
   
3. **IssuedBooks**: Tracks the issue and return of books.
   - `issue_id (PK)`, `user_id (FK)`, `book_id (FK)`, `issue_date`, `return_date`
   
4. **Defaulters**: Lists users with overdue books and pending fines.
   - `defaulter_id (PK)`, `user_id (FK)`, `fine_due`
   
5. **ActivityLog**: Logs all activities, including book issues and returns.
   - `activity_id (PK)`, `user_id (FK)`, `book_id (FK)`, `activity`, `timestamp`
   
6. **CurrentSession**: Tracks the current session of the user interacting with the kiosk.
   - `session_id (PK)`, `user_id (FK)`, `login_time`, `logout_time`

## Setup and Installation

### Prerequisites
- **C++**: The system is implemented in C++.
- **SQLite**: The database used to store library data.
- **Qt Framework**: For building the graphical interface of the system.

### Steps to Run the Project
1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/library-management-system.git
   ```
2. Open the project in your preferred **C++** development environment.
3. Ensure that **SQLite** is installed and properly configured in your environment.
4. Compile the project.
5. Run the system and start using the admin or student functionalities.

### Admin Credentials:
- **Username**: **
- **Password**: **
