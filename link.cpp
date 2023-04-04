#include "link.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQuery>

bool link_started()
{
    //Vérifier la présence d'une connexion existante
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        if (db.isValid() && db.isOpen()) {
            return true;
        }
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
    QString dbPath = "./database.db"; // Chemin relatif à partir de l'emplacement de l'application
    db.setDatabaseName(dbPath);
    if(db.open()) {
        // Vérifier si les tables existent dans la base de données
        QStringList tableNames = db.tables();
        if (tableNames.contains("Content")) {
            return true;
        } else {
            // Si les tables n'existent pas, les créer
            QSqlQuery query;
            query.exec("CREATE TABLE Content (id INTEGER PRIMARY KEY AUTOINCREMENT, password VARCHAR(255), force VARCHAR(255), instructions TEXT)");
            return true;
        }
    } else {
        return false;
    }

}




