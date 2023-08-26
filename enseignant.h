#ifndef ENSEIGNANT_H
#define ENSEIGNANT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QtCharts/QChartView>

class Enseignant
{
public:
    Enseignant();
    Enseignant(QString nom, QString prenom, QDate dateNaissance, QString adresse, QString email,
               QString numeroTelephone, QDate dateEmbauche, QString matiereEnseignee);

    QString getNom();
    QString getPrenom();
    QDate getDateNaissance();
    QString getAdresse();
    QString getEmail();
    QString getNumeroTelephone();
    QDate getDateEmbauche();
    QString getMatiereEnseignee();

    bool ajouter();
    bool modifier(QString email);
    bool supprimer(QString email);

    QSqlQueryModel* afficherEnseignants();
    QSqlQueryModel* rechercher(QString nom, QString prenom, QString matiereEnseignee);
    QString GetID(QString emailll);
    QtCharts::QChartView* generateStatisticsChart();
    QSqlQueryModel* trierParNomDateEmbaucheMatiereASC();
    QSqlQueryModel* trierParNomDateEmbaucheMatiereDESC();


private:
    int enseignantId;
    QString nom;
    QString prenom;
    QDate dateNaissance;
    QString adresse;
    QString email;
    QString numeroTelephone;
    QDate dateEmbauche;
    QString matiereEnseignee;
};

#endif // ENSEIGNANT_H
