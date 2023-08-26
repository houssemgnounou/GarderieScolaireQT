#include "enseignant.h"
#include <QDebug>
#include <QSqlError>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

Enseignant::Enseignant()
{
    enseignantId =0;
    nom = "";
    prenom = "";
    dateNaissance = QDate();
    adresse = "";
    email = "";
    numeroTelephone = "";
    dateEmbauche = QDate();
    matiereEnseignee = "";
}



Enseignant::Enseignant( QString nom, QString prenom, QDate dateNaissance, QString adresse, QString email,
                       QString numeroTelephone, QDate dateEmbauche, QString matiereEnseignee)
{
    this->nom = nom;
    this->prenom = prenom;
    this->dateNaissance = dateNaissance;
    this->adresse = adresse;
    this->email = email;
    this->numeroTelephone = numeroTelephone;
    this->dateEmbauche = dateEmbauche;
    this->matiereEnseignee = matiereEnseignee;
}



QSqlQueryModel* Enseignant::afficherEnseignants()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT Nom, Prenom, Date_Naissance, Adresse, Email, Numero_Telephone, Date_Embauche, Matiere_Enseignee FROM Enseignants");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Naissance"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Numero"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date d'Embauche"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Matiere"));

    return model;
}


bool Enseignant::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO Enseignants (Nom, Prenom, Date_Naissance, Adresse, Email, Numero_Telephone, Date_Embauche, Matiere_Enseignee) "
                  "VALUES (:nom, :prenom, :dateNaissance, :adresse, :email, :numeroTelephone, :dateEmbauche, :matiereEnseignee)");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaissance", dateNaissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":numeroTelephone", numeroTelephone);
    query.bindValue(":dateEmbauche", dateEmbauche);
    query.bindValue(":matiereEnseignee", matiereEnseignee);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error in ajouter(): " << query.lastError().text();
        return false;
    }
}

QString Enseignant::GetID(QString emailll)
{
    QString id;

    QSqlQuery query;
    query.prepare("SELECT ID_ENSEIGNANT FROM Enseignants WHERE Email = :emailll");
    query.bindValue(":emailll", emailll);

    if (query.exec() && query.next()) {
        id = query.value(0).toString();
    }

    return id;
}

bool Enseignant::modifier(QString id)
{
    QSqlQuery query;
    query.prepare("UPDATE Enseignants SET Nom = :nom, Prenom = :prenom, Date_Naissance = :dateNaissance, "
                  "Adresse = :adresse, Email = :email, Numero_Telephone = :numeroTelephone, Date_Embauche = :dateEmbauche, "
                  "Matiere_Enseignee = :matiereEnseignee WHERE ID_ENSEIGNANT = :enseignantId");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaissance", dateNaissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":numeroTelephone", numeroTelephone);
    query.bindValue(":dateEmbauche", dateEmbauche);
    query.bindValue(":matiereEnseignee", matiereEnseignee);
    query.bindValue(":enseignantId", id);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error in modifier(): " << query.lastError().text();
        return false;
    }
}


bool Enseignant::supprimer(QString email)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Enseignants WHERE Email = :email");
    query.bindValue(":email", email);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error in supprimer(): " << query.lastError().text();
        return false;
    }
}


QSqlQueryModel* Enseignant::rechercher(QString nom, QString prenom, QString matiereEnseignee)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryString = "SELECT Nom, Prenom, Date_Naissance, Adresse, Email, Numero_Telephone, Date_Embauche, Matiere_Enseignee FROM Enseignants WHERE LOWER(Nom) LIKE '%" + nom + "%' OR LOWER(Prenom) LIKE '%" + prenom + "%' OR LOWER(Matiere_Enseignee) LIKE '%" + matiereEnseignee + "%'";

    model->setQuery(queryString);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Naissance"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Numero"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date d'Embauche"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Matiere"));

    return model;
}


QSqlQueryModel* Enseignant::trierParNomDateEmbaucheMatiereASC()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryString = "SELECT Nom, Prenom, Date_Naissance, Adresse, Email, Numero_Telephone, Date_Embauche, Matiere_Enseignee FROM Enseignants ORDER BY Nom ASC, Date_Embauche ASC, Matiere_Enseignee ASC";

    model->setQuery(queryString);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Naissance"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date d'Embauche"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Matière"));

    return model;
}

QSqlQueryModel* Enseignant::trierParNomDateEmbaucheMatiereDESC()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryString = "SELECT Nom, Prenom, Date_Naissance, Adresse, Email, Numero_Telephone, Date_Embauche, Matiere_Enseignee FROM Enseignants ORDER BY Nom DESC, Date_Embauche DESC, Matiere_Enseignee DESC";

    model->setQuery(queryString);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Naissance"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date d'Embauche"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Matière"));

    return model;
}




QtCharts::QChartView* Enseignant::generateStatisticsChart()
{
    QSqlQuery query("SELECT Matiere_Enseignee, COUNT(*) AS Count FROM Enseignants GROUP BY Matiere_Enseignee");

    QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
    QtCharts::QBarCategoryAxis *axisX = new QtCharts::QBarCategoryAxis();

    while (query.next()) {
        QString matiere = query.value(0).toString();
        int count = query.value(1).toInt();

        qDebug() << "Matiere: " << matiere << ", Count: " << count;

        QtCharts::QBarSet *set = new QtCharts::QBarSet(matiere);
        *set << count;
        series->append(set);

        axisX->append(matiere);
    }

    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Nombre d'Enseignants par Matière");
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

    axisX->setTitleText("Matières");
    chart->setAxisX(axisX);

    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setTitleText("Nombre d'Enseignants");
    chart->setAxisY(axisY, series);

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}


bool Enseignant::emailExists( QString &email)  {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Enseignants WHERE Email = ?");
    query.addBindValue(email);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }

    // Handle database error here (return false, show error message, etc.)
    return false;
}


bool Enseignant::telephoneExists( QString &telephone)  {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Enseignants WHERE Numero_Telephone = ?");
    query.addBindValue(telephone);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }

    // Handle database error here (return false, show error message, etc.)
    return false;
}
