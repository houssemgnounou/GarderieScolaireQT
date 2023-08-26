#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "enseignant.h"
#include "eleve.h"
#include <QMessageBox>
#include <QRegExp>
#include <QValidator>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->stackedWidget->setCurrentIndex(0);


    ui->table_enseignant->setModel(enseignant.afficherEnseignants());

    // Connect the double-click signal to the slot
    connect(ui->table_enseignant, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onEnseignantDoubleClicked(const QModelIndex&)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_openHomeEnseignant_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_btn_openAjoutEnseignant_clicked()
{
    ui->txt_nom_enseignant->clear();
    ui->txt_prenom_enseignant->clear();
    ui->txt_addresse_enseignant->clear();
    ui->dateEdit_date_naissance_enseignant->setDate(QDate::currentDate().addYears(-18));
    ui->txt_email_enseignant->clear();
    ui->txt_telephone_enseignant->clear();
    ui->dateEdit_date_embauche->setDate(QDate::currentDate());
    ui->txt_matiere_enseignee->clear();

    ui->btn_ModifierEnseignant->hide();
    ui->btn_AjouterEnseignant->show();
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_btn_AjouterEnseignant_clicked()
{
    QString nom = ui->txt_nom_enseignant->text();
    QString prenom = ui->txt_prenom_enseignant->text();
    QString adresse = ui->txt_addresse_enseignant->text();
    QDate dateNaissance = ui->dateEdit_date_naissance_enseignant->date();
    QString email = ui->txt_email_enseignant->text();
    QString phoneNumber = ui->txt_telephone_enseignant->text();
    QDate dateEmbauche = ui->dateEdit_date_embauche->date();
    QString matiereEnseignee = ui->txt_matiere_enseignee->text();

    // Controle de saisie
    if (nom.isEmpty() || prenom.isEmpty() || dateNaissance.isNull() || email.isEmpty() || dateEmbauche.isNull() || matiereEnseignee.isEmpty()) {
        QMessageBox::critical(this, "Input Error", "Please fill in all the required fields.");
        return;
    }

    // Email format validation
    QRegExp emailRegex(R"(\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,4}\b)");
    if (!email.contains(emailRegex)) {
        QMessageBox::critical(this, "Input Error", "Please enter a valid email address.");
        return;
    }

    // Phone number validation
    QRegExp numericOnly(R"(\d*)");
    QValidator *phoneValidator = new QRegExpValidator(numericOnly, this);
    int pos = 0; // This variable will receive the position of the invalid character if needed
    if (phoneValidator->validate(phoneNumber, pos) != QValidator::Acceptable) {
        QMessageBox::critical(this, "Input Error", "Please enter a valid phone number.");
        return;
    }

    if (phoneNumber.length() != 8) {
        QMessageBox::critical(this, "Input Error", "Phone number must have exactly 8 digits.");
        return;
    }


    QDate currentDate = QDate::currentDate();
    QDate eighteenYearsAgo = currentDate.addYears(-18);

    if (dateNaissance >= currentDate) {
        QMessageBox::critical(this, "Input Error", "Date of birth cannot be today or a future date.");
        return;
    }

    if (dateNaissance > eighteenYearsAgo) {
        QMessageBox::critical(this, "Input Error", "The person must be 18 years old or older.");
        return;
    }

    if (dateEmbauche < dateNaissance) {
        QMessageBox::critical(this, "Input Error", "Date of employment cannot be before date of birth.");
        return;
    }

    Enseignant enseignant(nom, prenom, dateNaissance, adresse, email, phoneNumber, dateEmbauche, matiereEnseignee);

    if (enseignant.ajouter()) {
        QMessageBox::information(this, "Success", "Enseignant added successfully.");
        ui->txt_nom_enseignant->clear();
        ui->txt_prenom_enseignant->clear();
        ui->txt_addresse_enseignant->clear();
        ui->dateEdit_date_naissance_enseignant->setDate(QDate::currentDate().addYears(-18));
        ui->txt_email_enseignant->clear();
        ui->txt_telephone_enseignant->clear();
        ui->dateEdit_date_embauche->setDate(QDate::currentDate());
        ui->txt_matiere_enseignee->clear();
        ui->table_enseignant->setModel(enseignant.afficherEnseignants()); // refresh
        ui->stackedWidget->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, "Error", "Failed to add enseignant to the database.");
    }
}


void MainWindow::on_Search_enseignant_textChanged(const QString &searchText)
{
    QString nom = searchText.toLower();
    QString prenom = searchText.toLower();
    QString matiereEnseignee = searchText.toLower();


    ui->table_enseignant->setModel(enseignant.rechercher(nom, prenom, matiereEnseignee));

    //ui->table_enseignant->resizeColumnsToContents();
}



void MainWindow::on_Supprimer_enseignant_clicked()
{
        QModelIndex index = ui->table_enseignant->currentIndex();
        if (index.isValid()) {
            QString email = index.sibling(index.row(), 4).data().toString();
            int choice = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete this teacher?", QMessageBox::Yes | QMessageBox::No);
            if (choice == QMessageBox::Yes) {
                Enseignant teacher;
                if (teacher.supprimer(email)) {
                   // QMessageBox::information(this, "Success", "Teacher deleted successfully.");
                    ui->table_enseignant->setModel(teacher.afficherEnseignants()); // Refresh TableView
                } else {
                    QMessageBox::critical(this, "Error", "Failed to delete teacher.");
                }
            }
        }
}

void MainWindow::onEnseignantDoubleClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        ui->stackedWidget->setCurrentIndex(1);
        ui->btn_AjouterEnseignant->hide();
        ui->btn_ModifierEnseignant->show();

        QString nom = index.sibling(index.row(), 0).data().toString();
        QString prenom = index.sibling(index.row(), 1).data().toString();
        QDate dateNaissance = index.sibling(index.row(), 2).data().toDate();
        QString adresse = index.sibling(index.row(), 3).data().toString();
        QString email = index.sibling(index.row(), 4).data().toString();
        QString phoneNumber = index.sibling(index.row(), 5).data().toString();
        QDate dateEmbauche = index.sibling(index.row(), 6).data().toDate();
        QString matiereEnseignee = index.sibling(index.row(), 7).data().toString();

        ui->txt_nom_enseignant->setText(nom);
        ui->txt_prenom_enseignant->setText(prenom);
        ui->dateEdit_date_naissance_enseignant->setDate(dateNaissance);
        ui->txt_addresse_enseignant->setText(adresse);
        ui->txt_email_enseignant->setText(email);
        ui->txt_telephone_enseignant->setText(phoneNumber);
        ui->dateEdit_date_embauche->setDate(dateEmbauche);
        ui->txt_matiere_enseignee->setText(matiereEnseignee);
        //
        QString id = enseignant.GetID(email);
        currentEnseignantId = id;
    }
}

void MainWindow::on_btn_ModifierEnseignant_clicked()
{
    QString nom = ui->txt_nom_enseignant->text();
    QString prenom = ui->txt_prenom_enseignant->text();
    QDate dateNaissance = ui->dateEdit_date_naissance_enseignant->date();
    QString adresse = ui->txt_addresse_enseignant->text();
    QString email = ui->txt_email_enseignant->text(); // Updated email
    QString phoneNumber = ui->txt_telephone_enseignant->text();
    QDate dateEmbauche = ui->dateEdit_date_embauche->date();
    QString matiereEnseignee = ui->txt_matiere_enseignee->text(); // Updated matiereEnseignee

    //Controle de saisie
    if (nom.isEmpty() || prenom.isEmpty() || dateNaissance.isNull() || email.isEmpty() || dateEmbauche.isNull() || matiereEnseignee.isEmpty()) {
        QMessageBox::critical(this, "Input Error", "Please fill in all the required fields.");
        return;
    }

    // Email format validation
    QRegExp emailRegex(R"(\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,4}\b)");
    if (!email.contains(emailRegex)) {
        QMessageBox::critical(this, "Input Error", "Please enter a valid email address.");
        return;
    }

    // Phone number validation
    QRegExp numericOnly(R"(\d*)");
    QValidator *phoneValidator = new QRegExpValidator(numericOnly, this);
    int pos = 0; // This variable will receive the position of the invalid character if needed
    if (phoneValidator->validate(phoneNumber, pos) != QValidator::Acceptable) {
        QMessageBox::critical(this, "Input Error", "Please enter a valid phone number.");
        return;
    }

    if (phoneNumber.length() != 8) {
        QMessageBox::critical(this, "Input Error", "Phone number must have exactly 8 digits.");
        return;
    }


    QDate currentDate = QDate::currentDate();
    QDate eighteenYearsAgo = currentDate.addYears(-18);

    if (dateNaissance >= currentDate) {
        QMessageBox::critical(this, "Input Error", "Date of birth cannot be today or a future date.");
        return;
    }

    if (dateNaissance > eighteenYearsAgo) {
        QMessageBox::critical(this, "Input Error", "The person must be 18 years old or older.");
        return;
    }

    if (dateEmbauche < dateNaissance) {
        QMessageBox::critical(this, "Input Error", "Date of employment cannot be before date of birth.");
        return;
    }

    Enseignant enseignant(nom, prenom, dateNaissance, adresse, email, phoneNumber, dateEmbauche, matiereEnseignee);

    if (enseignant.modifier(currentEnseignantId)) {
        QMessageBox::information(this, "Success", "Enseignant updated successfully.");
        ui->stackedWidget->setCurrentIndex(0);
        ui->table_enseignant->setModel(enseignant.afficherEnseignants());// refresh
    } else {
        QMessageBox::critical(this, "Error", "Failed to update enseignant in the database.");
    }
}




void MainWindow::on_exporter_enseignant_clicked()
{
    QTableView *table;
    table = ui->table_enseignant;

    QString filters("CSV files (*.csv);;All files (*.*)");
    QString defaultFilter("CSV files (*.csv)");
    QString fileName = QFileDialog::getSaveFileName(0, "Save file", QCoreApplication::applicationDirPath(),
                       filters, &defaultFilter);
    QFile file(fileName);

    QAbstractItemModel *model =  table->model();
    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream data(&file);
        QStringList strList;
        for (int i = 0; i < model->columnCount(); i++) {
            if (model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString().length() > 0)
                strList.append("\"" + model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() + "\"");
            else
                strList.append("");
        }
        data << strList.join(";") << "\n";
        for (int i = 0; i < model->rowCount(); i++) {
            strList.clear();
            for (int j = 0; j < model->columnCount(); j++) {

                if (model->data(model->index(i, j)).toString().length() > 0)
                    strList.append("\"" + model->data(model->index(i, j)).toString() + "\"");
                else
                    strList.append("");
            }
            data << strList.join(";") + "\n";
        }
        file.close();
        QMessageBox::information(this,"Exporter To Excel","Exporter En Excel Avec Succées ");
    }
}


void MainWindow::on_btn_OpenStat_enseignant_clicked()
{

        Enseignant enseignant;
        QtCharts::QChartView *chartView = enseignant.generateStatisticsChart();
        ui->stackedWidget->addWidget(chartView);
        ui->stackedWidget->setCurrentWidget(chartView);
}


void MainWindow::on_Trie_enseignantDESC_clicked()
{
    ui->table_enseignant->setModel(enseignant.trierParNomDateEmbaucheMatiereDESC());
}


void MainWindow::on_Trie_enseignantASC_clicked()
{
    ui->table_enseignant->setModel(enseignant.trierParNomDateEmbaucheMatiereASC());
}

