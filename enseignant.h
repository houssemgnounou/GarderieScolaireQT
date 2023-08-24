#ifndef ENSEIGNANT_H
#define ENSEIGNANT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

class Enseignant
{
public:
    Enseignant();
    Enseignant(int id,QString nom, QString prenom, QDate dateNaissance, QString adresse, QString email,
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
    bool modifier();
    bool supprimer();

    QSqlQueryModel* rechercher(QString nom, QString prenom, QString matiereEnseignee);
    QSqlQueryModel* trierParNomDateEmbaucheMatiere();

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
