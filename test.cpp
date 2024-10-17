#include <iostream>
#include <sqlite3.h>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unistd.h>

using namespace std;

void menuforadmin(sqlite3 *db, const string &role);
void menuforstudent(sqlite3 *db, const string &role);
void createstudent(sqlite3 *db);
void libraryactivity(sqlite3 *db);
void deletestudent(sqlite3 *db);
void seeborrowedbooks(sqlite3 *db);
void borrowbooks(sqlite3 *db, const string &role);
void returnbooks(sqlite3 *db, const string &role);
bool authentication(sqlite3 *db, const string &role);
void admincreation(sqlite3 *db);
bool isadminpresent(sqlite3 *db);
void menuforadmin();
void menuforstudent();
void findbooks(sqlite3 *db);
void defaulter(sqlite3 *db);
void unlock(sqlite3 *db);
void updatepassword(sqlite3 *db, const string &role);
void myactivity(sqlite3 *db);
void changephonenumber(sqlite3 *db);
void managebook(sqlite3 *db);
void studentdefaulterlist(sqlite3 *db);
void allstudentdefaulter(sqlite3 *db);
void managestudentdefaulter(sqlite3 *db, int studentID);
void myfines(sqlite3 *db);
void getstudentdetail(sqlite3 *db);
void addmanualbook(sqlite3 *db);

int main()
{
    sqlite3 *db;
    int exit = sqlite3_open("library.db", &db);

    if (exit)
    {
        cerr << "Error  database is not responding " << sqlite3_errmsg(db) << endl;
        return -1;
    }

    if (!isadminpresent(db))
    {
        system("CLS");
        cout << "No admin found. thus create new admin\n";
        system("pause");
        admincreation(db);
    }
    defaulter(db);
    int choice1, choice2;
    cout << "                              LOG IN                             \n";
    cout << "************************ 1. Admin Login   ************************\n";
    cout << "************************ 2. Student Login ************************\n";
    cout << "************************ 3. Exit          ************************\n";
    cout << " select -> ";
    cin >> choice1;

    switch (choice1)
    {
    case 1:
        system("CLS");
        cout << "xxxxxxxxxx ADMIN LOG IN xxxxxxxxxx";
        sleep(1);
        if (authentication(db, "admin"))
        {
            system("CLS");
            menuforadmin(db, "admin");
        }
        else
        {
            system("CLS");
            cerr << "Check your id and password\n";
            sleep(2);
            system("CLS");
            system("pause");
            system("CLS");
            cout << "You can try again or exit\n";
            cout << "1. Try again\n";
            cout << "2. Exit\n";
            cout << "select ->";
            cin >> choice2;
            switch (choice2)
            {
            case 1:
                system("CLS");
                main();
                break;
            case 2:
                cout << "Exiting the program...";
                sleep(2);
                sqlite3_close(db);
                return 0;
                break;
            default:
                system("CLS");
                cout << "Option not available.make a vaild choice.\n";
                cout << "Exiting the program...";
                sleep(2);
                sqlite3_close(db);
                return -1;
            }
        }
        break;
    case 2:
        system("CLS");
        cout << "xxxxxxxxxx STUDENT LOG IN xxxxxxxxxx";
        sleep(1);
        if (authentication(db, "student"))
        {
            system("CLS");
            cout << "xxxxxxxxxx Self service kiosk xxxxxxxxxxx\n";
            system("pause");
            system("CLS");
            menuforstudent(db, "student");
        }
        else
        {
            system("CLS");
            cerr << "Check your id and password\n";
            cout << "If you forgot your id and password you may contact admin.\n";
            sleep(2);
            system("CLS");
            system("pause");
            system("CLS");
            cout << "You can try again or exit\n";
            cout << "1. Try again\n";
            cout << "2. Exit\n";
            cout << "select ->";
            cin >> choice2;
            switch (choice2)
            {
            case 1:
                system("CLS");
                main();
                break;
            case 2:
                cout << "Exiting the program...";
                sleep(2);
                sqlite3_close(db);
                return 0;
                break;
            default:
                system("CLS");
                cout << "option not available.make a vaild choice.\n";
                cout << "Exiting the program...";
                sleep(2);

                sqlite3_close(db);
                return -1;
            }
        }
        break;
    case 3:
        sqlite3_close(db);
        return 0;
    default:

        system("CLS");
        cout << "option not available.make a vaild choice.\n";
        cout << "Exiting the program...";
        sleep(2);
        sqlite3_close(db);
        return -1;
    }
}

bool isadminpresent(sqlite3 *db)
{
    string sql = "select COUNT(*) from Users where role = 'admin';";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    sqlite3_step(stmt);
    int adminCount = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return adminCount > 0;
}

void admincreation(sqlite3 *db)
{
    system("CLS");
    string name, email, password, libraryPhone;

    cout << "Enter admin  name ->";
    cin >> name;
    cout << "Enter admin  email -> ";
    cin >> email;
    cout << "Enter admin  password -> ";
    cin >> password;

    cout << "Enter library phone number -> ";
    cin >> libraryPhone;

    string sql = "insert into Users (name, email, password, role, phone) values (?, ?, ?, 'admin', ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, libraryPhone.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            cout << "Error in creation of admin .thus admin is not created . check in program for any further detail about error.thank you. " << sqlite3_errmsg(db) << endl;
        }
        else
        {
            system("CLS");
            cout << "Admin is successfully created. you can carry forward with your work .good luck\n";
            system("pause");
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        cout << "Error while creating admin. try again later.for any further detail check the program.\n";
    }
}

bool authentication(sqlite3 *db, const string &role)
{
    system("CLS");
    string email, password;
    cout << "xxxxxxxxxxx Enter your details xxxxxxxxxx\n";
    cout << "Enter email -> ";
    cin >> email;
    cout << "Enter password -> ";
    cin >> password;

    string sql = "select id from Users where email = '" + email + "' and password = '" + password + "' and role = '" + role + "';";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int userID = sqlite3_column_int(stmt, 0);

            if (sqlite3_exec(db, "delete from CurrentSession;", nullptr, nullptr, nullptr) != SQLITE_OK)
            {
                system("CLS");
                cout << "Error while deliting id from currentsession" << sqlite3_errmsg(db) << ".\n";
                system("pause");
                sqlite3_finalize(stmt);
                return false;
            }

            string insertSessionSQL = "insert into CurrentSession (user_id, role) values (" + to_string(userID) + ", '" + role + "');";
            if (sqlite3_exec(db, insertSessionSQL.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK)
            {
                system("CLS");
                cout << "Error while inserting detail in currentsession" << sqlite3_errmsg(db) << ".\n";
                system("pause");
                sqlite3_finalize(stmt);
                return false;
            }

            sqlite3_finalize(stmt);
            return true;
        }
        else
        {
            cout << "User is not authenticated \n";
            sqlite3_finalize(stmt);
            return false;
        }
    }
    else
    {
        cout << "Error while vaildating id and password detail from user table." << ".\n";
        return false;
    }
}

void menuforadmin(sqlite3 *db, const string &role)
{
    int choice;
    do
    {
        system("CLS");
        menuforadmin();
        cin >> choice;
        switch (choice)
        {
        case 1:
            system("CLS");
            createstudent(db);
            system("pause");
            break;
        case 2:
            system("CLS");
            libraryactivity(db);
            system("pause");
            break;
        case 3:
            system("CLS");
            deletestudent(db);
            system("pause");
            break;
        case 4:
            system("CLS");
            seeborrowedbooks(db);
            system("pause");
            break;
        case 5:
            system("CLS");
            borrowbooks(db, role);
            system("pause");
            break;
        case 6:
            system("CLS");
            studentdefaulterlist(db);
            system("pause");
            break;
        case 7:
            system("CLS");
            updatepassword(db, role);
            system("pause");
            break;
        case 8:
            system("CLS");
            managebook(db);
            system("pause");
            break;
        case 9:
            system("CLS");
            getstudentdetail(db);
            system("pause");
            break;
        case 10:
            system("CLS");
            findbooks(db);
            system("pause");
            break;
        case 0:
            cout << "leaving admin menu . going back\n";
            sleep(1);
            break;
        default:
            system("CLS");
            cout << "Your choice is not valid . thus make a valid choice\n";
            system("pause");
        }
    } while (choice != 0);
    system("CLS");
    main();
}

void menuforstudent(sqlite3 *db, const string &role)
{
    int choice;
    do
    {
        system("CLS");
        menuforstudent();
        cin >> choice;

        switch (choice)
        {
        case 1:
            system("CLS");
            findbooks(db);
            system("pause");
            break;
        case 2:
            system("CLS");
            borrowbooks(db, role);
            system("pause");
            break;
        case 3:
            system("CLS");
            returnbooks(db, role);
            system("pause");
            break;
        case 4:
            system("CLS");
            updatepassword(db, role);
            system("pause");
            break;
        case 5:
            system("CLS");
            myactivity(db);
            system("pause");
            break;
        case 6:
            system("CLS");
            changephonenumber(db);
            system("pause");
            break;
        case 7:
            system("CLS");
            myfines(db);
            system("pause");
            break;
        case 0:
            cout << "leaving student menu . going back...\n";
            sleep(1);
            break;
        default:
            system("CLS");
            cout << "your choice is not valid . thus make a valid choice\n";
            system("pause");
        }
    } while (choice != 0);
    system("CLS");
    main();
}

void menuforadmin()
{
    system("CLS");
    cout << "xxxxxxxxxxxxxxxxxxxx  Admin Menu xxxxxxxxxxxxxxxxxx\n";
    cout << "1. New Student Registration\n";
    cout << "2. Show Library Activities\n";
    cout << "3. Delete Students\n";
    cout << "4. View Issued Books\n";
    cout << "5. Issue Book\n";
    cout << "6. Check Defaulter / Unlock Student ID\n";
    cout << "7. Change  Student password\n";
    cout << "8. Manage Books\n";
    cout << "9. Give Student ID And Password\n";
    cout << "10. Search Book\n";
    cout << "0. Leave Admin Menu\n";
    cout << "Select -> ";
}

void menuforstudent()
{
    system("CLS");
    cout << "xxxxxxxxxxxxxxxxxx   Student Menu   xxxxxxxxxxxxxxxx\n";
    cout << "1. Search Book\n";
    cout << "2. Issue Book\n";
    cout << "3. Return Book\n";
    cout << "4. Change  Student password\n";
    cout << "5. Show My Activity\n";
    cout << "6. Update My Phone Number\n";
    cout << "7. Check My Due\n";
    cout << "0. Leaving Student Menu\n";
    cout << "Select -> ";
}

void createstudent(sqlite3 *db)
{
    system("CLS");
    string name, email, password, phone;
    int studentID;
    string role = "student";
    cout << "xxxxxxxxxxx Enter student details xxxxxxxxxxx";
    cout << "Enter student id -> ";
    cin >> studentID;
    cout << "Enter student name -> ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter student email -> ";
    getline(cin, email);
    cout << "Enter student password -> ";
    getline(cin, password);
    cout << "Enter student phone number -> ";
    getline(cin, phone);

    string sql = "insert into Users (id, name, email, password, role, phone) values (" +
                 to_string(studentID) + ", '" + name + "', '" + email + "', '" + password + "', '" + role + "', '" + phone + "');";

    char *errorMessage;
    if (sqlite3_exec(db, sql.c_str(), nullptr, 0, &errorMessage) != SQLITE_OK)
    {
        system("CLS");
        cout << "Error occur during creation of student. check the program " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
    else
    {
        system("CLS");
        cout << "Student creation got successfull . Now this student can borrow books.\n";
    }
}

void libraryactivity(sqlite3 *db)
{

    cout << "Make your choice to get data by \n";
    cout << "1. All Student\n";
    cout << "2. Specific Student id\n";
    cout << "3. using Student id and Book id\n";
    cout << "select -> ";

    int option;
    cin >> option;

    if (option == 1)
    {
        system("CLS");
        cout << "xxxxxxxxxxxxx Activity Log Of All Student xxxxxxxxxxxxx\n";

        string sql = "select student_id, activity, timestamp from ActivityLog;";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            bool found = false;

            cout << left << setw(15) << "Student id" << setw(50) << "Activity" << setw(25) << "Time" << endl;
            cout << string(90, '-') << endl;

            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                found = true;

                int id = sqlite3_column_int(stmt, 0);
                string activity = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
                string timestamp = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));

                cout << left << setw(15) << id << setw(50) << activity << setw(25) << timestamp << endl;
            }
            if (!found)
            {
                cout << "Activity log is found to be empty.There is no activity\n";
            }
            sqlite3_finalize(stmt);
        }
        else
        {
            cout << "Error while finding activity of all student. try again later.thank you. " << sqlite3_errmsg(db) << "\n";
        }
    }
    else if (option == 2)
    {
        system("CLS");
        int studentID;
        cout << "Enter student id -> ";
        cin >> studentID;
        system("CLS");

        cout << "xxxxxxxxxx Activity Log For Student -> " << studentID << " xxxxxxxxxxx\n";

        string sql = "select activity, timestamp from ActivityLog where student_id = ?;";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 1, studentID);

            bool found = false;

            cout << left << setw(50) << "Activity" << setw(25) << "Time" << endl;
            cout << string(75, '-') << endl;

            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                found = true;

                string activity = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
                string timestamp = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

                cout << left << setw(50) << activity << setw(25) << timestamp << endl;
            }
            if (!found)
            {
                cout << "No activity is founded for student id -> " << studentID << ".\n";
            }
            sqlite3_finalize(stmt);
        }
        else
        {
            cout << "Error in founding activity of student -> " << studentID << ": " << sqlite3_errmsg(db) << "\n";
        }
    }
    else if (option == 3)
    {
        system("CLS");
        int studentID, bookID;
        cout << "Enter student id -> ";
        cin >> studentID;
        cout << "Enter book id -> ";
        cin >> bookID;

        system("CLS");
        cout << "xxxxxxxxxx Activity of student -> " << studentID << " for book -> " << bookID << " xxxxxxxxxx\n";

        string sql = "select activity, timestamp from ActivityLog where student_id = ? and book_id = ?;";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 1, studentID);
            sqlite3_bind_int(stmt, 2, bookID);

            bool found = false;

            cout << left << setw(50) << "Activity" << setw(25) << "Time" << endl;
            cout << string(75, '-') << endl;

            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                found = true;

                string activity = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
                string timestamp = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

                cout << left << setw(50) << activity << setw(25) << timestamp << endl;
            }
            if (!found)
            {
                cout << "No activity was found for this student with this book. this student not have done any issue or return for this book\n";
            }
            sqlite3_finalize(stmt);
        }
        else
        {
            cout << "Error in finding activity of student with specific book " << sqlite3_errmsg(db) << "\n";
        }
    }

    else
    {
        system("CLS");
        cout << "Please click for some valid option given by system. thank you\n";
    }
}

void deletestudent(sqlite3 *db)
{
    string studentIdentifier;
    int choice;

    cout << "xxxxxxxxxx Delete Student xxxxxxxxxx\n";
    cout << "1. Delete by Student id \n";
    cout << "2. Delete by Student email \n";
    cout << "Select -> ";
    cin >> choice;

    string sql;
    if (choice == 1)
    {
        system("CLS");
        int studentID;
        cout << "Enter Student id -> ";
        cin >> studentID;

        sql = "delete from Users where id = " + to_string(studentID) + " and role = 'student';";
    }
    else if (choice == 2)
    {
        system("CLS");
        string studentEmail;
        cout << "Enter Student email -> ";
        cin >> studentEmail;

        sql = "delete from Users where email = '" + studentEmail + "' and role = 'student';";
    }
    else
    {
        system("CLS");
        cout << "Plese choose a valid option. thank you \n";
        return;
    }

    char *errorMessage;
    int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage);

    if (result != SQLITE_OK)
    {
        cerr << "Error  program not able to delete student " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
    else
    {
        cout << "Successfully delete the student . thank you" << endl;
    }
}

void seeborrowedbooks(sqlite3 *db)
{
    cout << "xxxxxxxxxxx View Issued Books xxxxxxxxxx\n";

    string sql = "select Users.id, Users.name, Users.email, Books.id, Books.title, IssuedBooks.issue_date, IssuedBooks.return_by "
                 "from IssuedBooks "
                 "inner join Users ON IssuedBooks.user_id = Users.id "
                 "inner join Books ON IssuedBooks.book_id = Books.id;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK)
    {
        cerr << "Program failed to get detail of borrowed books " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << left << setw(10) << "User ID"
         << setw(20) << "Name"
         << setw(30) << "Email"
         << setw(10) << "Book ID"
         << setw(30) << "Book Title"
         << setw(15) << "Issue Date"
         << setw(15) << "Return By"
         << endl;

    // Print a separator line
    cout << string(130, '-') << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int userID = sqlite3_column_int(stmt, 0);
        string userName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        string userEmail = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        int bookID = sqlite3_column_int(stmt, 3);
        string bookTitle = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        string issueDate = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        string returnBy = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));

        cout << left << setw(10) << userID
             << setw(15) << userName
             << setw(30) << userEmail
             << setw(10) << bookID
             << setw(30) << bookTitle
             << setw(15) << issueDate
             << setw(15) << returnBy
             << endl;
    }

    sqlite3_finalize(stmt);
}

void borrowbooks(sqlite3 *db, const string &role)
{
    int studentID = 0, bookID;

    cout << "xxxxxxxxxx Borrow Book xxxxxxxxxx\n";

    auto now = chrono::system_clock::now();
    auto issueTime = chrono::system_clock::to_time_t(now);

    if (role == "student")
    {
        string getStudentIDSql = "select user_id from CurrentSession where role = 'student';";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, getStudentIDSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                studentID = sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }

        if (studentID == 0)
        {
            cout << "Error  not able to get student id fron current session . check your program.\n";
            return;
        }
    }
    else if (role == "admin")
    {
        system("CLS");
        cout << "Enter student id -> ";
        cin >> studentID;

        string checkStudentSql = "select role from Users WHERE id = ?;";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, checkStudentSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 1, studentID);
            if (sqlite3_step(stmt) == SQLITE_ROW) // verify if this user id is valid or not.
            {
                // check if this id is of student or not.
                string studentRole = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
                if (studentRole != "student")
                {
                    cout << "This id did not belong to any student . thank you .\n";
                    sqlite3_finalize(stmt);
                    return;
                }
            }
            else
            {
                cout << "User id was not found in database. thank you.\n";
                sqlite3_finalize(stmt);
                return;
            }
            sqlite3_finalize(stmt);
        }
    }

    // check if student is defaulter or not
    string checkDefaulterSQL = "select SUM(fine) from defaulter where user_id = ?;";
    sqlite3_stmt *checkStmt;
    if (sqlite3_prepare_v2(db, checkDefaulterSQL.c_str(), -1, &checkStmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(checkStmt, 1, studentID); // if no detail found it will not go in this loop
        if (sqlite3_step(checkStmt) == SQLITE_ROW)
        {
            int totalFine = sqlite3_column_int(checkStmt, 0);
            if (totalFine > 0)
            {
                cout << "You have a pending fine of " << totalFine << " rupees. You cannot issue any more books until you pay the fine .\n";
                sqlite3_finalize(checkStmt);
                return;
            }
        }
        sqlite3_finalize(checkStmt);
    }
    else
    {
        cout << "Error in sql querry for  defaulter list  \n";
        return;
    }
    sqlite3_stmt *stmt;
    // here we are limiting the number of book a student can issue
    string checkIssuedBooksSql = "SELECT COUNT(*) FROM IssuedBooks WHERE user_id = ?;";
    if (sqlite3_prepare_v2(db, checkIssuedBooksSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, studentID);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int issuedBooksCount = sqlite3_column_int(stmt, 0);
            if (issuedBooksCount >= 2)
            {
                cout << "The student has already issued the maximum allowed number of 2 books.\n";
                sqlite3_finalize(stmt);
                return;
            }
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        cout << "Error while validating number of book issued by student. \n";
        return;
    }

    cout << "Enter book id -> ";
    cin >> bookID;

    string checkSameBookIssuedSql = "select COUNT(*) from IssuedBooks where user_id = ? and book_id = ?;";
    if (sqlite3_prepare_v2(db, checkSameBookIssuedSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, studentID);
        sqlite3_bind_int(stmt, 2, bookID);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int alreadyIssuedSameBook = sqlite3_column_int(stmt, 0);
            if (alreadyIssuedSameBook > 0)
            {
                cout << "This student already issued this book.\n";
                sqlite3_finalize(stmt);
                return;
            }
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        cout << "Error while checking if this book is already borrowed or not.\n";
        return;
    }

    string checkAvailabilitySql = "select available_copies from Books where id = ?;";
    if (sqlite3_prepare_v2(db, checkAvailabilitySql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, bookID);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int availableCopies = sqlite3_column_int(stmt, 0);
            if (availableCopies <= 0)
            {
                cout << "This book is not available.\n";
                sqlite3_finalize(stmt);
                return;
            }
        }
        else
        {

            cout << "Book with id " << bookID << " is not found.\n";
            sqlite3_finalize(stmt);
            return;
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        cout << "Error checking if book is available or not.\n";
        return;
    }

    string updateSql = "update Books set available_copies = available_copies - 1 where id = ?;";
    char *errorMessage;
    if (sqlite3_prepare_v2(db, updateSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, bookID);
        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            cout << "Error while updating the book availability.\n";
        }
        sqlite3_finalize(stmt);
    }

    auto returnTime = now + chrono::hours(24 * 30);
    auto returnTime_t = chrono::system_clock::to_time_t(returnTime);

    stringstream issueDateStream, returnDateStream;
    issueDateStream << put_time(localtime(&issueTime), "%Y-%m-%d %H:%M:%S");
    returnDateStream << put_time(localtime(&returnTime_t), "%Y-%m-%d %H:%M:%S");

    string issueSql = "insert into IssuedBooks (user_id, book_id, issue_date, return_by) VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, issueSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, studentID);
        sqlite3_bind_int(stmt, 2, bookID);
        sqlite3_bind_text(stmt, 3, issueDateStream.str().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, returnDateStream.str().c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            cout << "Error while filling the detail of borrowed books.\n";
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        cout << "Error in the filling detail of book statement.\n";
        return;
    }

    // filling activity log
    string activity = "borrowed book id " + to_string(bookID) + " to student id " + to_string(studentID);
    string logSql = "insert  into ActivityLog (activity, student_id, book_id, timestamp) values (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, logSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, activity.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, studentID);
        sqlite3_bind_int(stmt, 3, bookID);
        sqlite3_bind_text(stmt, 4, issueDateStream.str().c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            cout << "Error to log the activity.\n";
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        cout << "Error in using activity log statement.\n";
    }

    cout << "Book borrowed successfully\n";
    cout << "Borrow time: " << issueDateStream.str() << "\n";
    cout << "You must return the book by: " << returnDateStream.str() << endl;
}

void returnbooks(sqlite3 *db, const string &role)
{
    int studentID = 0, bookID;

    cout << "xxxxxxxxxx Return Book xxxxxxxxxx\n";

    if (role == "student")
    {
        string getStudentIDSql = "select user_id from CurrentSession where role = 'student';";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, getStudentIDSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                studentID = sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }

        if (studentID == 0)
        {
            cout << "Error  not able to get student id fron current session . check your program.\n";
            return;
        }
    }
    else if (role == "admin")
    {
        system("CLS");
        cout << "Enter student id -> ";
        cin >> studentID;
    }

    string checkStudentSql = "select COUNT(*) from Users where id = ? and role = 'student';";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, checkStudentSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, studentID);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int studentExists = sqlite3_column_int(stmt, 0);
            if (studentExists == 0)
            {
                cout << "This id did not belong to any student . thank you .\n";
                sqlite3_finalize(stmt);
                return;
            }
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        cout << "Error while checking if student exist.\n";
        return;
    }

    cout << "Enter book id -> ";
    cin >> bookID;

    string checkIssueSql = "select COUNT(*) from IssuedBooks where user_id = ? and book_id = ?;";
    if (sqlite3_prepare_v2(db, checkIssueSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, studentID);
        sqlite3_bind_int(stmt, 2, bookID);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int bookIssued = sqlite3_column_int(stmt, 0);
            if (bookIssued == 0)
            {
                cout << "This book was not issued to this student.\n";
                sqlite3_finalize(stmt);
                return;
            }
        }
        sqlite3_finalize(stmt);
    }

    string checkDefaulterSQL = "SELECT fine FROM defaulter WHERE user_id = ? AND book_id = ?;";
    if (sqlite3_prepare_v2(db, checkDefaulterSQL.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, studentID);
        sqlite3_bind_int(stmt, 2, bookID);
        int fine = 0;

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            fine = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);

        if (fine > 0)
        {
            auto now = chrono::system_clock::now();
            auto returnTime = chrono::system_clock::to_time_t(now);
            stringstream returnDateStream;
            returnDateStream << put_time(localtime(&returnTime), "%Y-%m-%d %H:%M:%S");

            string updateReturnDateSQL = "update defaulter set actual_return_date = ? where user_id = ? and book_id = ?;";
            char *errorMessage;
            if (sqlite3_prepare_v2(db, updateReturnDateSQL.c_str(), -1, &stmt, 0) == SQLITE_OK)
            {
                sqlite3_bind_text(stmt, 1, returnDateStream.str().c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(stmt, 2, studentID);
                sqlite3_bind_int(stmt, 3, bookID);
                if (sqlite3_step(stmt) != SQLITE_DONE)
                {
                    cout << "Error while updating actual return date. " << errorMessage << endl;
                    sqlite3_finalize(stmt);
                    return;
                }
                sqlite3_finalize(stmt);
            }
            cout << "Thank you for returning the book. You still have to pay " << fine << " rupees for this book.\n";
        }
    }

    string updateSql = "update Books set available_copies = available_copies + 1 where id = ?;";
    char *errorMessage;
    if (sqlite3_prepare_v2(db, updateSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, bookID);
        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            cout << "Error while updating book availability " << errorMessage << endl;
            sqlite3_finalize(stmt);
            return;
        }
        sqlite3_finalize(stmt);
    }

    string deleteSql = "delete from IssuedBooks where user_id = ? and book_id = ?;";
    if (sqlite3_prepare_v2(db, deleteSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, studentID);
        sqlite3_bind_int(stmt, 2, bookID);
        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            cout << "Error while deleting book from borrowed book detail. " << errorMessage << endl;
            sqlite3_finalize(stmt);
            return;
        }
        sqlite3_finalize(stmt);
    }

    // log return activity
    auto now1 = chrono::system_clock::now();
    auto time_t_now = chrono::system_clock::to_time_t(now1);
    tm *ltm = localtime(&time_t_now);

    stringstream timestampStream;
    timestampStream << put_time(ltm, "%Y-%m-%d %H:%M:%S");
    string formattedTimestamp = timestampStream.str();

    string activity = "returned book id " + to_string(bookID) + " from student id " + to_string(studentID);
    string logSql = "insert into ActivityLog (activity, student_id, book_id, timestamp) values (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, logSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, activity.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, studentID);
        sqlite3_bind_int(stmt, 3, bookID); // Add book ID to the log
        sqlite3_bind_text(stmt, 4, formattedTimestamp.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            cout << "Error while insert return detail in activity log. " << errorMessage << endl;
            sqlite3_free(errorMessage);
            sqlite3_finalize(stmt);
            return;
        }
        sqlite3_finalize(stmt);
    }

    cout << "Book returned successfully\n";
}

void findbooks(sqlite3 *db)
{
    int searchOption;
    string searchInput;
    int bookID;

    cout << "xxxxxxxxxx Search Book xxxxxxxxxxx\n";
    cout << "Search by \n";
    cout << "1. Book id\n";
    cout << "2. Title\n";
    cout << "3. Author\n";
    cout << "4. Genre\n";
    cout << "Select -> ";
    cin >> searchOption;
    cin.ignore();

    string sql;

    switch (searchOption)
    {
    case 1:
        system("CLS");
        cout << "Enter book id -> ";
        cin >> bookID;
        sql = "select id, title, author, genre, available_copies from Books where id = " + to_string(bookID) + ";";
        break;
    case 2:
        system("CLS");
        cout << "Enter book title -> ";
        getline(cin, searchInput);
        sql = "select id, title, author, genre, available_copies from Books where title LIKE '%" + searchInput + "%';";
        break;
    case 3:
        system("CLS");
        cout << "Enter book author -> ";
        getline(cin, searchInput);
        sql = "select id, title, author, genre, available_copies from Books where author LIKE '%" + searchInput + "%';";
        break;
    case 4:
        system("CLS");
        cout << "Enter book genre -> ";
        getline(cin, searchInput);
        sql = "select id, title, author, genre, available_copies from Books where genre LIKE '%" + searchInput + "%';";
        break;
    default:
        cout << "You select wrong option . going back .\n";
        return;
    }

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK)
    {
        cerr << "Error in search quarry. " << sqlite3_errmsg(db) << endl;
        return;
    }
    system("CLS");
    cout << left << setw(10) << "Book ID"
         << setw(30) << "Title"
         << setw(20) << "Author"
         << setw(15) << "Genre"
         << setw(15) << "Available Copies" << endl;
    cout << string(90, '-') << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int bookID = sqlite3_column_int(stmt, 0);
        string title = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        string author = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        string genre = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        int availableCopies = sqlite3_column_int(stmt, 4);

        cout << left << setw(10) << bookID
             << setw(30) << title
             << setw(20) << author
             << setw(15) << genre
             << setw(15) << availableCopies
             << endl;
    }

    sqlite3_finalize(stmt);
}

void defaulter(sqlite3 *db)
{

    string query = "select id, user_id, book_id, issue_date, return_by, actual_return_date from IssuedBooks;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK)
    {
        cerr << "Failed to book detail.  " << sqlite3_errmsg(db) << endl;
        return;
    }

    auto now = chrono::system_clock::now();

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int issuedBookID = sqlite3_column_int(stmt, 0);
        int userID = sqlite3_column_int(stmt, 1);
        int bookID = sqlite3_column_int(stmt, 2);
        string issueDateStr = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        string returnDateStr = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        const char *actualReturnDateStr = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));

        tm issue_tm = {};
        stringstream issueDateStream(issueDateStr);
        issueDateStream >> get_time(&issue_tm, "%Y-%m-%d %H:%M:%S");
        time_t issueDate = mktime(&issue_tm);

        int fine = 0;

        if (actualReturnDateStr == nullptr)
        {

            tm return_tm = {};
            stringstream returnDateStream(returnDateStr);
            returnDateStream >> get_time(&return_tm, "%Y-%m-%d %H:%M:%S");
            time_t returnDate = mktime(&return_tm);

            auto returnTime = chrono::system_clock::from_time_t(returnDate);
            auto overdueTime = now - returnTime;
            auto overdueDays = chrono::duration_cast<chrono::hours>(overdueTime).count() / 24;

            if (overdueDays > 0)
            {
                fine = overdueDays * 100;
            }
        }
        else
        {

            tm actualReturn_tm = {};
            stringstream actualReturnDateStream(actualReturnDateStr);
            actualReturnDateStream >> get_time(&actualReturn_tm, "%Y-%m-%d %H:%M:%S");
            time_t actualReturnDate = mktime(&actualReturn_tm);

            tm return_tm = {};
            stringstream returnDateStream(returnDateStr);
            returnDateStream >> get_time(&return_tm, "%Y-%m-%d %H:%M:%S");
            time_t returnDate = mktime(&return_tm);

            auto returnTime = chrono::system_clock::from_time_t(returnDate);
            auto actualReturnTime = chrono::system_clock::from_time_t(actualReturnDate);
            auto overdueTime = actualReturnTime - returnTime;
            auto overdueDays = chrono::duration_cast<chrono::hours>(overdueTime).count() / 24;

            if (overdueDays > 0)
            {
                fine = overdueDays * 100;
            }
        }

        if (fine > 0)
        {

            string defaulterCheckSQL = "select COUNT(*) from defaulter where user_id = ? and book_id = ?;";
            sqlite3_stmt *defaulterCheckStmt;
            sqlite3_prepare_v2(db, defaulterCheckSQL.c_str(), -1, &defaulterCheckStmt, 0);
            sqlite3_bind_int(defaulterCheckStmt, 1, userID);
            sqlite3_bind_int(defaulterCheckStmt, 2, bookID);
            sqlite3_step(defaulterCheckStmt);
            int exists = sqlite3_column_int(defaulterCheckStmt, 0);
            sqlite3_finalize(defaulterCheckStmt);

            if (exists == 0)
            {
                string insertDefaulterSQL = "insert into defaulter (user_id, book_id, issue_date, return_by, actual_return_date, fine) "
                                            "values (?, ?, ?, ?, ?, ?);";
                sqlite3_stmt *insertStmt;
                sqlite3_prepare_v2(db, insertDefaulterSQL.c_str(), -1, &insertStmt, 0);
                sqlite3_bind_int(insertStmt, 1, userID);
                sqlite3_bind_int(insertStmt, 2, bookID);
                sqlite3_bind_text(insertStmt, 3, issueDateStr.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(insertStmt, 4, returnDateStr.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(insertStmt, 5, actualReturnDateStr == nullptr ? "-" : actualReturnDateStr, -1, SQLITE_STATIC);
                sqlite3_bind_int(insertStmt, 6, fine);
                sqlite3_step(insertStmt);
                sqlite3_finalize(insertStmt);

                string lockSQL = "update Users set islock = 1 WHERE id = ?;";
                sqlite3_stmt *lockStmt;
                sqlite3_prepare_v2(db, lockSQL.c_str(), -1, &lockStmt, 0);
                sqlite3_bind_int(lockStmt, 1, userID);
                sqlite3_step(lockStmt);
                sqlite3_finalize(lockStmt);

                cout << "Student with id " << userID << " added to defaulter list with a fine of " << fine << " rupees.\n";
            }
            else
            {

                string updateDefaulterSQL = "update defaulter set fine = ? where user_id = ? and book_id = ?;";
                sqlite3_stmt *updateStmt;
                sqlite3_prepare_v2(db, updateDefaulterSQL.c_str(), -1, &updateStmt, 0);
                sqlite3_bind_int(updateStmt, 1, fine);
                sqlite3_bind_int(updateStmt, 2, userID);
                sqlite3_bind_int(updateStmt, 3, bookID);
                sqlite3_step(updateStmt);
                sqlite3_finalize(updateStmt);

                cout << "Fine for student with id  " << userID << " is " << fine << " rupees.\n";
            }
        }
    }

    sqlite3_finalize(stmt);
}

void unlock(sqlite3 *db)
{

    int studentID;
    cout << "xxxxxxxxxxx Unlock student xxxxxxxxxx";
    cout << " Enter student id to unlock -> ";
    cin >> studentID;

    string unlockSql = "update Users set islock = 0 where id = ?;";
    sqlite3_stmt *unlockStmt;
    if (sqlite3_prepare_v2(db, unlockSql.c_str(), -1, &unlockStmt, 0) != SQLITE_OK)
    {
        cerr << " Error in unlock statement ." << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(unlockStmt, 1, studentID);

    if (sqlite3_step(unlockStmt) != SQLITE_DONE)
    {
        cerr << "Error student did not get unlocked." << sqlite3_errmsg(db) << endl;
    }
    else
    {
        cout << "Student with id " << studentID << " got successfully unlocked.\n";
    }
    sqlite3_finalize(unlockStmt);

    string deleteSql = "delete from Defaulter where user_id = ?;";
    sqlite3_stmt *deleteStmt;
    if (sqlite3_prepare_v2(db, deleteSql.c_str(), -1, &deleteStmt, 0) != SQLITE_OK)
    {
        cerr << "Error in delete statement ." << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(deleteStmt, 1, studentID);

    if (sqlite3_step(deleteStmt) != SQLITE_DONE)
    {
        cerr << "Error while deleting student from defaulter list. " << sqlite3_errmsg(db) << endl;
    }
    else
    {
        cout << "Student with id " << studentID << " got successfully removed from defaulter list.\n";
    }
    sqlite3_finalize(deleteStmt);
}

void updatepassword(sqlite3 *db, const string &role)
{
    int studentID = 0;
    string newPassword;
    cout << "xxxxxxxxxx Update your password xxxxxxxxxx";
    if (role == "student")
    {

        string getStudentIDSql = "select user_id from CurrentSession where role = 'student';";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, getStudentIDSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                studentID = sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }

        if (studentID == 0)
        {
            cout << "Error while getting student id.\n";
            return;
        }
    }
    else if (role == "admin")
    {
        system("CLS");
        cout << "Enter student id to change password -> ";
        cin >> studentID;

        string checkStudentSql = "select role from Users WHERE id = ?;";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, checkStudentSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 1, studentID);
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                string studentRole = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
                if (studentRole != "student")
                {
                    cout << "Entered id does not belong to any student.\n";
                    sqlite3_finalize(stmt);
                    return;
                }
            }
            else
            {
                cout << "Error student id is not found.\n";
                sqlite3_finalize(stmt);
                return;
            }
            sqlite3_finalize(stmt);
        }
    }

    cout << "Enter new password -> ";
    cin >> newPassword;

    string updatePasswordSql = "update Users set password = ? where id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, updatePasswordSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, studentID);

        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            cout << "Password updated successfully.\n";
        }
        else
        {
            cout << "Error while updating password..\n";
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        cout << "Error in update password atatement.\n";
    }
}

void myactivity(sqlite3 *db)
{
    int studentID = 0;

    string getStudentIDSql = "select user_id from CurrentSession where role = 'student';";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, getStudentIDSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            studentID = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    if (studentID == 0)
    {
        cout << "Error while getting student id.\n";
        return;
    }
    system("CLS");
    cout << "xxxxxxxxxx Check My Activity xxxxxxxxxx\n";
    cout << "1. View All Activities\n";
    cout << "2. View Activities for a Specific Book\n";
    cout << "select -> ";

    int choice;
    cin >> choice;

    if (choice == 1)
    {
        system("CLS");
        string allActivitiesSql = "select activity, timestamp from ActivityLog where student_id = ?;";
        if (sqlite3_prepare_v2(db, allActivitiesSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 1, studentID);
            cout << "xxxxxxxxxx Your Activities xxxxxxxxxx\n";
            cout << left << setw(50) << "Activity" << setw(25) << "Time" << endl;
            cout << string(75, '-') << endl;

            while (sqlite3_step(stmt) == SQLITE_ROW)
            {

                string activity = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
                string timestamp = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

                cout << left << setw(50) << activity << setw(25) << timestamp << endl;
            }
            sqlite3_finalize(stmt);
        }
        else
        {
            cout << "Error while retrieving my activities.\n";
        }
    }
    else if (choice == 2)
    {
        system("CLS");
        int bookID;
        cout << "Enter the book id -> ";
        cin >> bookID;

        string specificBookSql = "select activity, timestamp from ActivityLog where student_id = ? and book_id = ?;";
        if (sqlite3_prepare_v2(db, specificBookSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 1, studentID);
            sqlite3_bind_int(stmt, 2, bookID);
            system("CLS");
            cout << "Activities for Book id " << bookID << ":\n";

            cout << left << setw(50) << "Activity" << setw(25) << "Time" << endl;
            cout << string(75, '-') << endl;

            while (sqlite3_step(stmt) == SQLITE_ROW)
            {

                string activity = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
                string timestamp = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

                cout << left << setw(50) << activity << setw(25) << timestamp << endl;
            }
            sqlite3_finalize(stmt);
        }
        else
        {
            cout << "Error while retrieving activities for the specified book\n";
        }
    }
    else
    {
        cout << "You select wrong choice . please make a valid choice. thank you.\n";
    }
}

void changephonenumber(sqlite3 *db)
{

    string newPhone;
    cout << "xxxxxxxxxxxx Update you phone number xxxxxxxxxx";
    int studentID = 0;
    string getStudentIDSql = "select user_id from CurrentSession where role = 'student';";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, getStudentIDSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            studentID = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    if (studentID == 0)
    {
        cout << "Error while retriving student id from current session.\n";
        return;
    }

    cout << "Enter your new phone number -> ";
    cin >> newPhone;

    string changephonenumberSql = "update Users set phone = ? where id = ?;";
    char *errorMessage;

    if (sqlite3_prepare_v2(db, changephonenumberSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, newPhone.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, studentID);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            cout << "Error while updating phone number. " << errorMessage << endl;
            sqlite3_finalize(stmt);
            return;
        }
        sqlite3_finalize(stmt);
        cout << "Number got added successfully.\n";
    }
    else
    {
        cout << "Error while preparing number change statement.\n";
    }
}

void managebook(sqlite3 *db)
{
    int option;
    cout << "xxxxxxxxxx Manage Books xxxxxxxxxx\n";
    cout << "1. Add a New Book\n";
    cout << "2. Delete an Existing Book\n";
    cout << "select -> ";
    cin >> option;

    if (option == 1)
    {
        sqlite3 *testb;
        int exit = sqlite3_open("books.db", &testb);
        string bname;

        if (exit)
        {
            cerr << "Error database is not responding " << sqlite3_errmsg(testb) << endl;
            return;
        }

        system("CLS");
        cout << "Enter name of book -> ";
        cin.ignore();
        getline(cin, bname);

        string sql1 = "select name, author, genre from books where name = ?;";
        sqlite3_stmt *stmt1;

        if (sqlite3_prepare_v2(testb, sql1.c_str(), -1, &stmt1, 0) == SQLITE_OK)
        {
            sqlite3_bind_text(stmt1, 1, bname.c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(stmt1) == SQLITE_ROW)
            {
                string bookname = reinterpret_cast<const char *>(sqlite3_column_text(stmt1, 0));
                string authorname = reinterpret_cast<const char *>(sqlite3_column_text(stmt1, 1));
                string genre = reinterpret_cast<const char *>(sqlite3_column_text(stmt1, 2));
                int book_id, available_copies;

                cout << "Enter book ID -> ";
                cin >> book_id;
                cout << "Enter available copies -> ";
                cin >> available_copies;

                string sql2 = "insert into books (id, title, author, genre, available_copies) values (?, ?, ?, ?, ?);";
                sqlite3_stmt *stmt2;

                if (sqlite3_prepare_v2(db, sql2.c_str(), -1, &stmt2, 0) == SQLITE_OK)
                {
                    sqlite3_bind_int(stmt2, 1, book_id);
                    sqlite3_bind_text(stmt2, 2, bookname.c_str(), -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt2, 3, authorname.c_str(), -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt2, 4, genre.c_str(), -1, SQLITE_STATIC);
                    sqlite3_bind_int(stmt2, 5, available_copies);

                    if (sqlite3_step(stmt2) == SQLITE_DONE)
                    {
                        cout << "Book added successfully\n";
                    }
                    else
                    {
                        cerr << "There was a problem " << sqlite3_errmsg(db) << endl;
                    }

                    sqlite3_finalize(stmt2);
                }
                else
                {
                    cerr << "Error in statement preparation " << sqlite3_errmsg(db) << endl;
                }

                sqlite3_close(testb);
            }
            else
            {
                cout << "Data is not present about the book\n";
                sqlite3_close(testb);
                int k;
                cout << "Do you want to fill data manually?\n";
                cout << "1. Yes\n";
                cout << "2. No\n";
                cin >> k;

                if (k == 1)
                {
                    system("CLS");
                    addmanualbook(db);
                }
                else
                {
                    cout << "Goodbye.\n";
                }
            }

            sqlite3_finalize(stmt1);
        }
        else
        {
            cerr << "Error while executing statement " << sqlite3_errmsg(testb) << endl;
            sqlite3_close(testb);
        }
    }

    else if (option == 2)
    {
        system("CLS");
        int bookID;
        cout << "xxxxxxxxxx Delete Book xxxxxxxxxx\n";

        cout << "Enter the book id to delete -> ";
        cin >> bookID;

        string checkBookSql = "select COUNT(*) from Books where id = ?;";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, checkBookSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 1, bookID);
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                int bookExists = sqlite3_column_int(stmt, 0);
                if (bookExists == 0)
                {
                    cout << "The book id does not match any existing book. Please check the id and try again.\n";
                    sqlite3_finalize(stmt);
                    return;
                }
            }
            sqlite3_finalize(stmt);
        }
        else
        {
            cout << "Error while checking book existence.\n";
            return;
        }

        string deleteBookSql = "delete from Books WHERE id = ?;";
        char *errorMessage;

        if (sqlite3_prepare_v2(db, deleteBookSql.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 1, bookID);

            if (sqlite3_step(stmt) != SQLITE_DONE)
            {
                cout << "error while deleting book. " << errorMessage << endl;
                sqlite3_finalize(stmt);
                return;
            }
            sqlite3_finalize(stmt);
        }
        else
        {
            cout << "error in delete book statement.\n";
        }

        cout << "book with id " << bookID << " has been deleted successfully.\n";
    }
    else
    {
        cout << "You select wrong choice . please make a valid choice. thank you.\n";
    }
}

void studentdefaulterlist(sqlite3 *db)
{

    int option;
    cout << "xxxxxxxxxx Manage Defaulter List xxxxxxxxxx\n";
    cout << "1. Show All Defaulters\n";
    cout << "2. Check Fine And Unlock Student ID\n";
    cout << "Select -> ";
    cin >> option;

    if (option == 1)
    {
        system("CLS");
        allstudentdefaulter(db);
    }
    else if (option == 2)
    {
        system("CLS");
        int studentID;
        cout << "Enter the Student id: ";
        cin >> studentID;
        system("CLS");
        managestudentdefaulter(db, studentID);
    }
    else
    {
        cout << "You select wrong choice . please make a valid choice. thank you..\n";
    }
}

void allstudentdefaulter(sqlite3 *db)
{
    string sql = "select Users.id, Users.name, Books.title, Defaulter.fine FROM Defaulter "
                 "inner join Users ON Users.id = Defaulter.user_id "
                 "inner join Books ON Books.id = Defaulter.book_id;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        system("CLS");
        cout << "xxxxxxxxxx List of Defaulters xxxxxxxxxxx\n";
        cout << "ID\tName\tBook Title\tFine Amount\n";
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int studentID = sqlite3_column_int(stmt, 0);
            string studentName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
            string bookTitle = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
            double fineAmount = sqlite3_column_double(stmt, 3);
            cout << studentID << "\t" << studentName << "\t" << bookTitle << "\t" << fineAmount << "\n";
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        cout << "Error while retrieving defaulters list.\n";
    }
}

void managestudentdefaulter(sqlite3 *db, int studentID)
{

    string sql = "select Books.title, Defaulter.fine FROM Defaulter "
                 "inner join Books ON Books.id = Defaulter.book_id WHERE Defaulter.user_id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, studentID);

        double totalFine = 0.0;
        bool hasFines = false;

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            hasFines = true;
            string bookTitle = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
            double fineAmount = sqlite3_column_double(stmt, 1);
            totalFine += fineAmount;

            cout << "Book  '" << bookTitle << "' has a fine of " << fineAmount << " rupess" << ".\n";
        }
        sqlite3_finalize(stmt);

        if (hasFines)
        {
            //  total fine
            cout << "Total Fine for Student with " << studentID << "  " << totalFine << " rupess" << "\n";
            system("pause");
            system("CLS");
            int payOption;
            cout << "Do you want to pay the total fine now?\n1. Yes\n2. No\n";
            cin >> payOption;

            if (payOption == 1)
            {

                unlock(db);
            }
            else
            {
                cout << "Going back to admin menu\n";
            }
        }
        else
        {

            cout << "Student with id " << studentID << " is not a defaulter.\n";
        }
    }
    else
    {
        cout << "error while checking defaulter list " << sqlite3_errmsg(db) << endl;
    }
}

void myfines(sqlite3 *db)
{
    cout << "xxxxxxxxxx Your fine xxxxxxxxxx \n";
    string sqlSession = "select user_id from CurrentSession where role = 'student';";
    sqlite3_stmt *stmt;
    int studentID = -1;

    if (sqlite3_prepare_v2(db, sqlSession.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            studentID = sqlite3_column_int(stmt, 0); // Get student ID from session
        }
        sqlite3_finalize(stmt);
    }

    if (studentID == -1)
    {
        cout << "Error not able to retrive student id from current session.\n";
        return;
    }

    string sqlQuery = "select Books.title, defaulter.fine, defaulter.issue_date, defaulter.return_by, defaulter.actual_return_date "
                      "from defaulter "
                      "inner join Books ON defaulter.book_id = Books.id "
                      "where defaulter.user_id = ?;";

    if (sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, studentID);

        bool hasDefaults = false;
        double totalFine = 0.0;

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            hasDefaults = true;
            string bookTitle = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
            double fine = sqlite3_column_double(stmt, 1);
            string issueDate = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
            string returnBy = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
            string actualReturnDate = sqlite3_column_text(stmt, 4) ? reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)) : "Not Returned";

            totalFine += fine;

            cout << "Book: " << bookTitle << "\n"
                 << "Fine: " << fine << "rupess" << "\n"
                 << "Issue Date: " << issueDate << "\n"
                 << "Return By: " << returnBy << "\n"
                 << "Actual Return Date: " << actualReturnDate << "\n"
                 << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
        }
        sqlite3_finalize(stmt);

        if (!hasDefaults)
        {
            cout << "You have no dues.\n";
        }
        else
        {

            cout << "Total Fine is of " << totalFine << "rupess" << "\n";
        }
    }
    else
    {
        cerr << "Error while getting detail of defaulter from defaulter list." << sqlite3_errmsg(db) << endl;
    }
}

void getstudentdetail(sqlite3 *db)
{
    int studentID;
    cout << "xxxxxxxxxxxx Get Student Details xxxxxxxxxxxx" << "\n";
    cout << "Enter student id ->";
    cin >> studentID;
    string sql = "select email , password from Users where id= ? ;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, studentID);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            system("CLS");
            const char *email = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
            const char *password = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
            cout << "email ->" << email << "\n";
            cout << "password ->" << password << "\n";
        }
        else
        {
            cerr << "No records found for the provided student id ->" << studentID << ".\n";
        }
    }
    else
    {
        cerr << "Error executing statement while retrieving student details." << ".\n";
    }
}

void addmanualbook(sqlite3 *db)
{
    system("CLS");
    int bookID;
    string title, author, genre;
    int available_copies;

    cout << "xxxxxxxxxx Add New Book xxxxxxxxxx\n";

    cout << "Enter book id -> ";
    cin >> bookID;

    cout << "Enter book title -> ";
    cin.ignore();
    getline(cin, title);

    cout << "Enter book author -> ";
    getline(cin, author);

    cout << "Enter book genre -> ";
    getline(cin, genre);

    cout << "Enter number of available copies -> ";
    cin >> available_copies;

    string sql = "insert into Books (id, title, author, genre, available_copies) values (" +
                 to_string(bookID) + ", '" + title + "', '" + author + "', '" + genre + "', " +
                 to_string(available_copies) + ");";

    char *errorMessage;
    int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage);

    if (result != SQLITE_OK)
    {
        cerr << "Error while adding the book " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
    else
    {
        cout << "Book added successfully" << endl;
    }
}