#include "mainscreen.h"
#include "link.h"
#include "ui_mainscreen.h"

MainScreen::MainScreen(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainScreen)
{
    ui->setupUi(this);
    ui->check_pass_input->setPlaceholderText(tr("Ex: Password@7"));
    QString style = "QPushButton {"
                    "padding: 5px;"
                    "border: none;"
                    "background-color: #f0f0f0;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: #d0d0d0;"
                    "}";
    ui->checkBtn->setStyleSheet(style);
    ui->quit_btn->setStyleSheet(style);
    ui->access_database_btn->setStyleSheet(style);
    ui->reset->setStyleSheet(style);
    ui->testing->setStyleSheet(style);
    ui->settings->setStyleSheet(style);

    QString input_style = "QLineEdit {"
                          "border: 1px;"
                          "padding: 3px"
                          "}";
    ui->check_pass_input->setStyleSheet(input_style);
    ui->app_screen->setStyleSheet(input_style);
}

MainScreen::~MainScreen()
{
    delete ui;
}

QString MainScreen::force(QString val)
{
    if (check_password(val) == QValidator::Acceptable) {
        return tr("Fort");
    }
    else if(check_password(val) == QValidator::Intermediate && val.length() > 6)
    {
        return tr("Force Moyenne");
    }
    else if(check_password(val) == QValidator::Invalid)
    {
        return tr("Faible");
    }
    else{
        return tr("Error");
    }
}

QValidator::State MainScreen::check_password(QString value)
{
    int pos = 0;
    QRegExp rx("^(?=.*[A-Z])(?=.*[a-z])(?=.*[0-9])(?=.*[!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?]).{8,}$");
    QRegExpValidator v(rx, this);
    return v.validate(value, pos);
}

void MainScreen::on_quit_btn_clicked()
{
    qApp->exit();
}


void MainScreen::on_checkBtn_clicked()
{
    if(ui->check_pass_input->text().isEmpty())
    {
        ui->app_screen->setText(tr("Veuillez entrer un mot de passe pour pouvoir en tester la force"));
    }
    else if(link_started())
    {
        ui->app_screen->setText(tr("Mot de pass ")+force(ui->check_pass_input->text()));
        // Récupérer l'id courant de la table Profil
        QSqlQuery queryId;
        queryId.exec("SELECT MAX(id) FROM Content");
        queryId.next();
        int currentId = queryId.value(0).toInt();
        int newId = currentId + 1;
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM Content WHERE password = :password");
        query.bindValue(":password", ui->check_pass_input->text());
        query.exec();
        query.next();
        int count = query.value(0).toInt();
        if (!(count > 0)) {
            query.prepare("INSERT INTO Content (id, password, force, instructions) VALUES (:id, :password, :force, :instructions)");
            query.bindValue(":id", newId);
            query.bindValue(":password", ui->check_pass_input->text());
            query.bindValue(":force", force(ui->check_pass_input->text()));
            if(force(ui->check_pass_input->text()) == "Fort")
                query.bindValue(":instructions", tr("Mot de pass fort contenant au moins 8 caractère, 1 chiffre, une lettre minuscule et une lettre majuscule"));
            else if(force(ui->check_pass_input->text()) == "Force Moyenne")
                query.bindValue(":instructions", tr("Mot de pass moyen contenant au moins 6 caractère, 1 chiffre, une lettre minuscule et une lettre majuscule. Veuillez augmenter sa tailler et rajouter des symbole pour augmenter sa force"));
            else if(force(ui->check_pass_input->text()) == "Faible")
                query.bindValue(":instructions", tr("Mot de pass faible invalide et ne respectant aucune des fonctionalités d'un mot de pass sécurisé. Veuillez le changer."));
            else if(force(ui->check_pass_input->text()) == "Error")
                query.bindValue(":instructions", tr("Mauvaise valeur: Le mot de pass que vous essayez de tester ne peut être évalué pour certaines raisons. Veuillez le changer pour vous assurer de protéger efficacement vos données."));

            if(!query.exec())
                QMessageBox::critical(this, tr("Erreur"), tr("Le mot de pass n'a pas été enregistré."));
        }
    }
}
void MainScreen::on_access_database_btn_clicked()
{
    // Créer la boîte de dialogue
    if(link_started())
    {
        //vider le contenu de la QLineEdit
        ui->check_pass_input->clear();

        QDialog dialog(this);
        dialog.setWindowTitle(tr("Liste des Mots de pass"));
        dialog.setFixedSize(600, 400);

        // Créer le modèle de table
        QSqlTableModel model;
        model.setTable("Content");
        model.select();

        // Créer le QTableView et lui assigner le modèle
        QTableView tableView(&dialog);
        tableView.setModel(&model);

        // Configurer les options d'affichage du QTableView
        tableView.setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView.setEditTriggers(QAbstractItemView::NoEditTriggers);

        // Ajouter le QTableView à la boîte de dialogue
        QVBoxLayout layout(&dialog);
        layout.addWidget(&tableView);

        // Afficher la boîte de dialogue
        dialog.exec();

    }
    else
    {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'accéder à la database"));
    }
}


void MainScreen::on_testing_clicked()
{
    QMessageBox::information(this, tr("Test"), tr("Bébut de la session de débugage"));
}

void MainScreen::on_reset_clicked()
{
    if(link_started())
    {
        QSqlQuery requete;
        requete.exec("DROP TABLE IF EXISTS Content");
        if(requete.exec("CREATE TABLE Content (id INTEGER PRIMARY KEY AUTOINCREMENT,password VARCHAR(255), force VARCHAR(255), instructions TEXT)"))
            QMessageBox::information(this, tr("Notification"), tr("Base de donnée réinitialisée"));

        //vider le contenu de la QLineEdit
        ui->check_pass_input->clear();
    }

}

