#include "eleve.h"
#include <QDebug>
#include <QSqlError>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QSqlRecord>
#include <QSqlField>

Eleve::Eleve()
{
    eleveId  = 0;
    nom = "";
    prenom = "";
    dateNaissance = QDate();
    adresse = "";
    nomParent = "";
    numeroTelephoneParent = "";
    dateInscription = QDate();
    classe = 0;
    age = 0;
}

void Eleve::setNom(const QString& nom)
{
    this->nom = nom;
}

void Eleve::setPrenom(const QString& prenom)
{
    this->prenom = prenom;
}

void Eleve::setDateNaissance(const QDate& dateNaissance)
{
    this->dateNaissance = dateNaissance;
}

QString Eleve::getNom() const
{
    return nom;
}

QString Eleve::getPrenom() const
{
    return prenom;
}

QDate Eleve::getDateNaissance() const
{
    return dateNaissance;
}

int Eleve::getAge() const
{
    return age;
}

void Eleve::setAge(const int& age)
{
    this->age = age;
}


Eleve::Eleve(QString nom, QString prenom, QDate dateNaissance, QString adresse, QString nomParent,
             QString numeroTelephoneParent, QDate dateInscription, int classe)
{
    this->nom = nom;
    this->prenom = prenom;
    this->dateNaissance = dateNaissance;
    this->adresse = adresse;
    this->nomParent = nomParent;
    this->numeroTelephoneParent = numeroTelephoneParent;
    this->dateInscription = dateInscription;
    this->classe = classe;
}

Eleve::Eleve(QString nom, QString prenom, QDate dateNaissance)
{
    this->nom = nom;
    this->prenom = prenom;
    this->dateNaissance = dateNaissance;
}


QSqlQueryModel* Eleve::afficherEleves()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT Nom, Prenom, TO_CHAR(Date_Naissance, 'DD/MM/YYYY') AS Date_Naissance, Adresse, Nom_Parent, Numero_Telephone_Parent, TO_CHAR(Date_Inscription, 'DD/MM/YYYY') AS Date_Inscription, Classe FROM Eleves");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Naissance"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Parent"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Numéro du Parent"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Inscription"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Classe"));

    return model;
}

bool Eleve::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO Eleves (Nom, Prenom, Date_Naissance, Adresse, Nom_Parent, Numero_Telephone_Parent, Date_Inscription, Classe) "
                  "VALUES (:nom, :prenom, :dateNaissance, :adresse, :nomParent, :numeroTelephoneParent, :dateInscription, :classe)");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaissance", dateNaissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":nomParent", nomParent);
    query.bindValue(":numeroTelephoneParent", numeroTelephoneParent);
    query.bindValue(":dateInscription", dateInscription);
    query.bindValue(":classe", classe);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error in ajouter(): " << query.lastError().text();
        return false;
    }
}

bool Eleve::modifier(QString id)
{
    QSqlQuery query;
    query.prepare("UPDATE Eleves SET Nom = :nom, Prenom = :prenom, Date_Naissance = :dateNaissance, "
                  "Adresse = :adresse, Nom_Parent = :nomParent, Numero_Telephone_Parent = :numeroTelephoneParent, "
                  "Date_Inscription = :dateInscription, Classe = :classe WHERE ID_Eleve = :eleveId");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaissance", dateNaissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":nomParent", nomParent);
    query.bindValue(":numeroTelephoneParent", numeroTelephoneParent);
    query.bindValue(":dateInscription", dateInscription);
    query.bindValue(":classe", classe);
    query.bindValue(":eleveId", id);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error in modifier(): " << query.lastError().text();
        return false;
    }
}

bool Eleve::supprimer(QString tl)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Eleves WHERE Numero_Telephone_Parent = :tl");
    query.bindValue(":tl", tl);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error in supprimer(): " << query.lastError().text();
        return false;
    }
}

QString Eleve::GetID(QString numero)
{
    QString id;

    QSqlQuery query;
    query.prepare("SELECT ID_Eleve FROM Eleves WHERE Numero_Telephone_Parent = :numeroo");
    query.bindValue(":numeroo", numero);

    if (query.exec() && query.next()) {
        id = query.value(0).toString();
    }

    return id;
}


bool Eleve::telephoneExists( QString &telephone)  {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Eleves WHERE Numero_Telephone_Parent = ?");
    query.addBindValue(telephone);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }

    return false;
}

QSqlQueryModel* Eleve::rechercher(QString nom, QString prenom, int classe)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString queryString = "SELECT Nom, Prenom, TO_CHAR(Date_Naissance, 'DD/MM/YYYY') AS Date_Naissance, "
                          "Adresse, Nom_Parent, Numero_Telephone_Parent, "
                          "TO_CHAR(Date_Inscription, 'DD/MM/YYYY') AS Date_Inscription, Classe "
                          "FROM Eleves WHERE LOWER(Nom) LIKE '%" + nom.toLower() + "%' "
                          "OR LOWER(Prenom) LIKE '%" + prenom.toLower() + "%' OR (Classe) LIKE '%" + QString::number(classe) + "%'";


    model->setQuery(queryString);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Naissance"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Parent"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Numéro du Parent"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Inscription"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Classe"));

    return model;
}



QSqlQueryModel* Eleve::trierParNomDateInscriptionClasse(QString sortOrder)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString selectColumns = "Nom, Prenom, TO_CHAR(Date_Naissance, 'DD/MM/YYYY') AS Date_Naissance, "
                            "Adresse, Nom_Parent, Numero_Telephone_Parent, "
                            "TO_CHAR(Date_Inscription, 'DD/MM/YYYY') AS Date_Inscription, Classe";
    QString orderByClause = "ORDER BY Nom " + sortOrder + ", Date_Inscription " + sortOrder + ", Classe " + sortOrder;

    model->setQuery("SELECT " + selectColumns + " FROM Eleves " + orderByClause);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Naissance"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Parent"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Numéro du Parent"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Inscription"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Classe"));

    return model;
}


QtCharts::QChartView* Eleve::generateStatisticsChart()
{
    QSqlQuery query("SELECT Classe, COUNT(*) AS Count FROM Eleves GROUP BY Classe");

    QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
    QtCharts::QBarCategoryAxis *axisX = new QtCharts::QBarCategoryAxis();

    while (query.next()) {
        QString classe = query.value("Classe").toString();
        int count = query.value("Count").toInt();

        qDebug() << "Classe: " << classe << ", Count: " << count;

        QtCharts::QBarSet *set = new QtCharts::QBarSet(classe);
        *set << count;
        series->append(set);

        axisX->append(classe);
    }

    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des élèves par classe");
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

    axisX->setTitleText("Classes");
    chart->setAxisX(axisX);

    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setTitleText("Nombre d'élèves");
    chart->setAxisY(axisY, series);

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

int Eleve::calculerAge() const
{
    QDate currentDate = QDate::currentDate();
    int age = currentDate.year() - dateNaissance.year();

    if (currentDate < dateNaissance.addYears(age)) {
        age--;
    }

    return age;
}


QList<Eleve> Eleve::getUpcomingBirthdays()
{
    QList<Eleve> upcomingBirthdays;

    QSqlQuery query;
    query.prepare("SELECT ID_ELEVE, Nom, Prenom, Date_Naissance FROM Eleves");
    if (!query.exec()) {
        qDebug() << "Error executing query: " << query.lastError().text();
        return upcomingBirthdays;
    }

    QDate currentDate = QDate::currentDate();
    QDate endDate = currentDate.addDays(7);

    while (query.next()) {
        QString nom = query.value("Nom").toString();
        QString prenom = query.value("Prenom").toString();
        QDate dateNaissance = query.value("Date_Naissance").toDate();

        QDate birthdateThisYear(currentDate.year(), dateNaissance.month(), dateNaissance.day());

        if (birthdateThisYear >= currentDate && birthdateThisYear <= endDate) {
            Eleve student;
            student.setNom(nom);
            student.setPrenom(prenom);
            student.setDateNaissance(dateNaissance);
         //   student.setAge(student.calculerAge());  // Calculer age
            upcomingBirthdays.append(student);

          //  qDebug() << "age : " << student.calculerAge();

        }
    }

    std::sort(upcomingBirthdays.begin(), upcomingBirthdays.end(), [currentDate](const Eleve& a, const Eleve& b) {
        QDate birthdateA(currentDate.year(), a.getDateNaissance().month(), a.getDateNaissance().day());
        QDate birthdateB(currentDate.year(), b.getDateNaissance().month(), b.getDateNaissance().day());
        int daysToA = currentDate.daysTo(birthdateA);
        int daysToB = currentDate.daysTo(birthdateB);
        return daysToA < daysToB;
    });

    return upcomingBirthdays;
}





