#ifndef ELEVE_H
#define ELEVE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QtCharts/QChartView>

class Eleve
{
public:
    Eleve();
    Eleve(QString nom, QString prenom, QDate dateNaissance, QString adresse, QString nomParent,
          QString numeroTelephoneParent, QDate dateInscription, int classe);

    QString getNom();
    QString getPrenom();
    QDate getDateNaissance();
    QString getAdresse();
    QString getNomParentTuteur();
    QString getNumeroTelephoneParent();
    QDate getDateInscription();
    int getClasse();

    bool ajouter();
    bool modifier(QString id);
    bool supprimer(QString tl);

    QSqlQueryModel* rechercher(QString nom, QString prenom, int classe);
    QString GetID(QString emailll);
    QSqlQueryModel* afficherEleves();
    bool telephoneExists( QString &telephone);
    QSqlQueryModel* trierParNomDateInscriptionClasse(QString tri);
    QtCharts::QChartView* generateStatisticsChart();

private:
    int eleveId ;
    QString nom;
    QString prenom;
    QDate dateNaissance;
    QString adresse;
    QString nomParent;
    QString numeroTelephoneParent;
    QDate dateInscription;
    int classe;
};

#endif // ELEVE_H
