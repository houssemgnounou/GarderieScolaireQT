#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "enseignant.h"
#include "eleve.h"
#include <QMessageBox>
#include <QRegExp>
#include <QValidator>
#include <QDebug>
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
    connect(ui->table_eleve, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onEleveDoubleClicked(const QModelIndex&)));

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
    QString numeroTelephone = ui->txt_telephone_enseignant->text();
    QDate dateEmbauche = ui->dateEdit_date_embauche->date();
    QString matiereEnseignee = ui->txt_matiere_enseignee->text();

    // Contrôle de saisie
    if (nom.isEmpty() || prenom.isEmpty() || dateNaissance.isNull() || email.isEmpty() || dateEmbauche.isNull() || matiereEnseignee.isEmpty()) {
        QMessageBox::critical(this, "Erreur de saisie", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Validation du format de l'adresse e-mail
    QRegExp regexEmail(R"(\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,4}\b)");
    if (!email.contains(regexEmail)) {
        QMessageBox::critical(this, "Erreur de saisie", "Veuillez entrer une adresse e-mail valide.");
        return;
    }

    // Validation du numéro de téléphone
    QRegExp numericOnly(R"(\d*)");
    QValidator *validateurTelephone = new QRegExpValidator(numericOnly, this);
    int pos = 0; // Cette variable recevra la position du caractère invalide si nécessaire
    if (validateurTelephone->validate(numeroTelephone, pos) != QValidator::Acceptable) {
        QMessageBox::critical(this, "Erreur de saisie", "Veuillez entrer un numéro de téléphone valide.");
        return;
    }

    if (numeroTelephone.length() != 8) {
        QMessageBox::critical(this, "Erreur de saisie", "Le numéro de téléphone doit comporter exactement 8 chiffres.");
        return;
    }

    QDate dateActuelle = QDate::currentDate();
    QDate ilYaDixHuitAns = dateActuelle.addYears(-18);

    if (dateNaissance >= dateActuelle) {
        QMessageBox::critical(this, "Erreur de saisie", "La date de naissance ne peut pas être aujourd'hui ni une date future.");
        return;
    }

    if (dateNaissance > ilYaDixHuitAns) {
        QMessageBox::critical(this, "Erreur de saisie", "La personne doit avoir 18 ans ou plus.");
        return;
    }

    if (dateEmbauche < dateNaissance) {
        QMessageBox::critical(this, "Erreur de saisie", "La date d'embauche ne peut pas être antérieure à la date de naissance.");
        return;
    }

    if (enseignant.emailExists(email)) {
        QMessageBox::critical(this, "Erreur", "L'adresse e-mail existe déjà dans la base de données.");
        return;
    }

    if (enseignant.telephoneExists(numeroTelephone)) {
        QMessageBox::critical(this, "Erreur", "Le numéro de téléphone existe déjà dans la base de données.");
        return;
    }

    Enseignant enseignant(nom, prenom, dateNaissance, adresse, email, numeroTelephone, dateEmbauche, matiereEnseignee);

    if (enseignant.ajouter()) {
        QMessageBox::information(this, "Succès", "Enseignant ajouté avec succès.");
        ui->txt_nom_enseignant->clear();
        ui->txt_prenom_enseignant->clear();
        ui->txt_addresse_enseignant->clear();
        ui->dateEdit_date_naissance_enseignant->setDate(QDate::currentDate().addYears(-18));
        ui->txt_email_enseignant->clear();
        ui->txt_telephone_enseignant->clear();
        ui->dateEdit_date_embauche->setDate(QDate::currentDate());
        ui->txt_matiere_enseignee->clear();
        ui->table_enseignant->setModel(enseignant.afficherEnseignants()); // rafraîchir
        ui->stackedWidget->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'enseignant dans la base de données.");
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
        int choix = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cet enseignant ?", QMessageBox::Yes | QMessageBox::No);
        if (choix == QMessageBox::Yes) {
            Enseignant enseignant;
            if (enseignant.supprimer(email)) {
                // QMessageBox::information(this, "Succès", "Enseignant supprimé avec succès.");
                ui->table_enseignant->setModel(enseignant.afficherEnseignants()); // Rafraîchir TableView
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'enseignant.");
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
    QString email = ui->txt_email_enseignant->text();
    QString numeroTelephone = ui->txt_telephone_enseignant->text();
    QDate dateEmbauche = ui->dateEdit_date_embauche->date();
    QString matiereEnseignee = ui->txt_matiere_enseignee->text();

    // Contrôle de saisie
    if (nom.isEmpty() || prenom.isEmpty() || dateNaissance.isNull() || email.isEmpty() || dateEmbauche.isNull() || matiereEnseignee.isEmpty()) {
        QMessageBox::critical(this, "Erreur de saisie", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Validation du format de l'adresse e-mail
    QRegExp regexEmail(R"(\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,4}\b)");
    if (!email.contains(regexEmail)) {
        QMessageBox::critical(this, "Erreur de saisie", "Veuillez entrer une adresse e-mail valide.");
        return;
    }

    // Validation du numéro de téléphone
    QRegExp numericOnly(R"(\d*)");
    QValidator *validateurTelephone = new QRegExpValidator(numericOnly, this);
    int pos = 0; // Cette variable recevra la position du caractère invalide si nécessaire
    if (validateurTelephone->validate(numeroTelephone, pos) != QValidator::Acceptable) {
        QMessageBox::critical(this, "Erreur de saisie", "Veuillez entrer un numéro de téléphone valide.");
        return;
    }

    if (numeroTelephone.length() != 8) {
        QMessageBox::critical(this, "Erreur de saisie", "Le numéro de téléphone doit comporter exactement 8 chiffres.");
        return;
    }

    QDate dateActuelle = QDate::currentDate();
    QDate ilYaDixHuitAns = dateActuelle.addYears(-18);

    if (dateNaissance >= dateActuelle) {
        QMessageBox::critical(this, "Erreur de saisie", "La date de naissance ne peut pas être aujourd'hui ni une date future.");
        return;
    }

    if (dateNaissance > ilYaDixHuitAns) {
        QMessageBox::critical(this, "Erreur de saisie", "La personne doit avoir 18 ans ou plus.");
        return;
    }

    if (dateEmbauche < dateNaissance) {
        QMessageBox::critical(this, "Erreur de saisie", "La date d'embauche ne peut pas être antérieure à la date de naissance.");
        return;
    }


    Enseignant enseignant(nom, prenom, dateNaissance, adresse, email, numeroTelephone, dateEmbauche, matiereEnseignee);

    if (enseignant.modifier(currentEnseignantId)) {
      //  QMessageBox::information(this, "Succès", "Enseignant mis à jour avec succès.");
        ui->stackedWidget->setCurrentIndex(0);
        ui->table_enseignant->setModel(enseignant.afficherEnseignants());// Rafraîchir
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la mise à jour de l'enseignant dans la base de données.");
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


////////////////////////////////////////////////////////////////////////////////////////////// Partie éleves


void MainWindow::on_btn_openHomeEleve_clicked()
{
    ui->table_eleve->setModel(eleve.afficherEleves());

    ui->stackedWidget->setCurrentIndex(2);

    ui->txt_nom_eleve->clear();
    ui->txt_prenom_eleve->clear();
    ui->dateEdit_date_naissance_eleve->setDate(QDate::currentDate());
    ui->txt_addresse_eleve->clear();
    ui->txt_NomParent_eleve->clear();
    ui->txt_NumeroParentEleve->clear();
    ui->dateEdit_date_inscription->setDate(QDate::currentDate());
    ui->txt_classe_eleve->clear();
}


void MainWindow::on_btn_openAjoutEleve_clicked()
{
    ui->dateEdit_date_inscription->setDate(QDate::currentDate());
    ui->btn_ModifierEleve->hide();
    ui->btn_AjouterEleve->show();
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_btn_AjouterEleve_clicked()
{
    QString nom = ui->txt_nom_eleve->text();
    QString prenom = ui->txt_prenom_eleve->text();
    QDate dateNaissance = ui->dateEdit_date_naissance_eleve->date();
    QString adresse = ui->txt_addresse_eleve->text();
    QString nomParent = ui->txt_NomParent_eleve->text();
    QString numeroTelephoneParent = ui->txt_NumeroParentEleve->text();
    QDate dateInscription = ui->dateEdit_date_inscription->date();
    int classe = ui->txt_classe_eleve->text().toInt();

    if (nom.isEmpty() || prenom.isEmpty() || adresse.isEmpty() || nomParent.isEmpty() || numeroTelephoneParent.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs doivent être remplis.");
        return;
    }

    if (numeroTelephoneParent.length() != 8 || !numeroTelephoneParent.toInt()) {
        QMessageBox::critical(this, "Erreur", "Le numéro de téléphone doit être composé de 8 chiffres.");
        return;
    }

    int classeMin = 1;
    int classeMax = 6;
    if (classe < classeMin || classe > classeMax) {
        QMessageBox::critical(this, "Erreur", "La classe doit être comprise entre 1 et 6.");
        return;
    }

    if (eleve.telephoneExists(numeroTelephoneParent)) {
        QMessageBox::critical(this, "Erreur", "Le numéro de téléphone existe déjà dans la base de données.");
        return;
    }

    if (dateInscription < dateNaissance) {
        QMessageBox::critical(this, "Erreur", "La date d'inscription ne peut pas être antérieure à la date de naissance.");
        return;
    }

    Eleve eleve(nom, prenom, dateNaissance, adresse, nomParent, numeroTelephoneParent, dateInscription, classe);

    if (eleve.ajouter()) {
        ui->txt_nom_eleve->clear();
        ui->txt_prenom_eleve->clear();
        ui->dateEdit_date_naissance_eleve->setDate(QDate::currentDate());
        ui->txt_addresse_eleve->clear();
        ui->txt_NomParent_eleve->clear();
        ui->txt_NumeroParentEleve->clear();
        ui->dateEdit_date_inscription->setDate(QDate::currentDate());
        ui->txt_classe_eleve->clear();

        ui->table_eleve->setModel(eleve.afficherEleves());
        ui->stackedWidget->setCurrentIndex(2);

        QMessageBox::information(this, "Succès", "Élève ajouté avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'élève.");
    }
}




void MainWindow::on_Search_eleve_textChanged(const QString &searchText)
{

    QString nom = searchText.toLower();
    QString prenom = searchText.toLower();
    int classe = searchText.toInt();

    ui->table_eleve->setModel(eleve.rechercher(nom, prenom, classe));
}

void MainWindow::on_Trie_eleveASC_clicked()
{
    ui->table_eleve->setModel(eleve.trierParNomDateInscriptionClasse("ASC"));

}


void MainWindow::on_Trie_eleveDESC_clicked()
{
    ui->table_eleve->setModel(eleve.trierParNomDateInscriptionClasse("DESC"));

}


void MainWindow::on_Supprimer_eleve_clicked()
{

    QModelIndex index = ui->table_eleve->currentIndex();
    if (index.isValid()) {
        QString numero = index.sibling(index.row(), 5).data().toString();
        int choix = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cet éleve ?", QMessageBox::Yes | QMessageBox::No);
        if (choix == QMessageBox::Yes) {
            if (eleve.supprimer(numero)) {
                ui->table_eleve->setModel(eleve.afficherEleves()); // Rafraîchir TableView
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'éleve.");
            }
        }
    }
}


void MainWindow::onEleveDoubleClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        ui->stackedWidget->setCurrentIndex(3);
        ui->btn_AjouterEleve->hide();
        ui->btn_ModifierEleve->show();

        QString nom = index.sibling(index.row(), 0).data().toString();
        QString prenom = index.sibling(index.row(), 1).data().toString();

        QString dateNaissance = index.sibling(index.row(), 2).data().toString();
        QString adresse = index.sibling(index.row(), 3).data().toString();
        QString nomParent = index.sibling(index.row(), 4).data().toString();
        QString numeroParent = index.sibling(index.row(), 5).data().toString();
        QString dateInscription = index.sibling(index.row(), 6).data().toString();
        QString classe = index.sibling(index.row(), 7).data().toString();

        ui->txt_nom_eleve->setText(nom);
        ui->txt_prenom_eleve->setText(prenom);
        ui->dateEdit_date_naissance_eleve->setDate(QDate::fromString(dateNaissance, "dd/MM/yyyy"));
        ui->txt_addresse_eleve->setText(adresse);
        ui->txt_NomParent_eleve->setText(nomParent);
        ui->txt_NumeroParentEleve->setText(numeroParent);
        ui->dateEdit_date_inscription->setDate(QDate::fromString(dateInscription, "dd/MM/yyyy"));
        ui->txt_classe_eleve->setText(classe);

        QString id = eleve.GetID(numeroParent);
        currentEleveId = id;
    }
}


void MainWindow::on_btn_ModifierEleve_clicked()
{
    QString nom = ui->txt_nom_eleve->text();
    QString prenom = ui->txt_prenom_eleve->text();
    QDate dateNaissance = ui->dateEdit_date_naissance_eleve->date();
    QString adresse = ui->txt_addresse_eleve->text();
    QString nomParent = ui->txt_NomParent_eleve->text();
    QString numeroTelephoneParent = ui->txt_NumeroParentEleve->text();
    QDate dateInscription = ui->dateEdit_date_inscription->date();
    int classe = ui->txt_classe_eleve->text().toInt();

    if (nom.isEmpty() || prenom.isEmpty() || adresse.isEmpty() || nomParent.isEmpty() || numeroTelephoneParent.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs doivent être remplis.");
        return;
    }

    if (numeroTelephoneParent.length() != 8 || !numeroTelephoneParent.toInt()) {
        QMessageBox::critical(this, "Erreur", "Le numéro de téléphone doit être composé de 8 chiffres.");
        return;
    }

    int classeMin = 1;
    int classeMax = 6;
    if (classe < classeMin || classe > classeMax) {
        QMessageBox::critical(this, "Erreur", "La classe doit être comprise entre 1 et 6.");
        return;
    }


    if (dateInscription < dateNaissance) {
        QMessageBox::critical(this, "Erreur", "La date d'inscription ne peut pas être antérieure à la date de naissance.");
        return;
    }

    Eleve eleve(nom, prenom, dateNaissance, adresse, nomParent, numeroTelephoneParent, dateInscription, classe);

    if (eleve.modifier(currentEleveId)) {
        ui->txt_nom_eleve->clear();
        ui->txt_prenom_eleve->clear();
        ui->dateEdit_date_naissance_eleve->setDate(QDate::currentDate());
        ui->txt_addresse_eleve->clear();
        ui->txt_NomParent_eleve->clear();
        ui->txt_NumeroParentEleve->clear();
        ui->dateEdit_date_inscription->setDate(QDate::currentDate());
        ui->txt_classe_eleve->clear();

        ui->table_eleve->setModel(eleve.afficherEleves());
        ui->stackedWidget->setCurrentIndex(2);
     //   QMessageBox::information(this, "Succès", "Élève mis à jour avec succès.");

    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la mise à jour de l'éléve dans la base de données.");
    }
}


void MainWindow::on_btn_OpenStat_eleve_clicked()
{
    QtCharts::QChartView *chartView = eleve.generateStatisticsChart();
    ui->stackedWidget->addWidget(chartView);
    ui->stackedWidget->setCurrentWidget(chartView);
}

