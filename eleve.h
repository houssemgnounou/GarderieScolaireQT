#ifndef ELEVE_H
#define ELEVE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

class Eleve
{
public:
    Eleve();
    Eleve(int id, QString nom, QString prenom, QDate dateNaissance, QString adresse, QString nomParentTuteur,
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
    bool modifier();
    bool supprimer();

    QSqlQueryModel* rechercher(QString nom, QString prenom, int classe);
    QSqlQueryModel* trierParNomDateInscriptionClasse();

private:
    int eleveId ;
    QString nom;
    QString prenom;
    QDate dateNaissance;
    QString adresse;
    QString nomParentTuteur;
    QString numeroTelephoneParent;
    QDate dateInscription;
    int classe;
};

#endif // ELEVE_H
