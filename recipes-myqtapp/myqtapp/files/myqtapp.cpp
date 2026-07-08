#include <QCoreApplication>
#include <QtSql>
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    if (argc < 2) {
        qDebug() << "Usage: myqtapp <server_ip>";
        return -1;
    }

    QString serverIp = argv[1];

    // Database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QString("Driver={ODBC Driver 17 for SQL Server};Server=%1;Database=TestDB;PORT=1433;").arg(serverIp));
    db.setUserName("sa");
    db.setPassword("Alilair69");

    if (!db.open()) {
        qDebug() << "Database connection failed: " << db.lastError().text();
        return -1;
    }
    qDebug() << "Database connected successfully!";

    // Execute query
    QSqlQuery query("SELECT id, name FROM Employees");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        qDebug() << "ID:" << id << "Name:" << name;
    }

    db.close();
    return a.exec();
}