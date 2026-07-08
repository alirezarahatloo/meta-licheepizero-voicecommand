#include <stdio.h>
#include <sybfront.h>
#include <sybdb.h>

#define SERVER "192.168.2.101"
#define USERNAME "sa"
#define PASSWORD "Alilair69"
#define DATABASE "TestDB"

int main() {
    DBPROCESS *dbproc;
    LOGINREC *login;
    RETCODE retcode;

    // Initialize DB Library
    dbinit();

    // Create login record
    login = dblogin();
    DBSETLUSER(login, USERNAME);
    DBSETLPWD(login, PASSWORD);

    // Connect to the SQL Server
    dbproc = dbopen(login, SERVER);
    if (!dbproc) {
        printf("Failed to connect to database.\n");
        return 1;
    }

    // Select the database
    dbuse(dbproc, DATABASE);

    // Execute SQL query
    if (dbcmd(dbproc, "SELECT ID, Name, Age FROM Employees") == FAIL) {
        printf("Failed to set command.\n");
        dbclose(dbproc);
        return 1;
    }

    if (dbsqlexec(dbproc) == FAIL) {
        printf("Failed to execute query.\n");
        dbclose(dbproc);
        return 1;
    }

    // Process query results
    while ((retcode = dbresults(dbproc)) != NO_MORE_RESULTS) {
        if (retcode == SUCCEED) {
            int id, age;
            char name[100];

            while (dbnextrow(dbproc) != NO_MORE_ROWS) {
                id = dbdata(dbproc, 1) ? *(int *)dbdata(dbproc, 1) : 0;
                strcpy(name, dbdata(dbproc, 2) ? (char *)dbdata(dbproc, 2) : "NULL");
                age = dbdata(dbproc, 3) ? *(int *)dbdata(dbproc, 3) : 0;

                printf("ID: %d, Name: %s, Age: %d\n", id, name, age);
            }
        }
    }

    // Clean up
    dbclose(dbproc);
    dbexit();
    return 0;
}

