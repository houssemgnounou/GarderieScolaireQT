#include "eleve.h"
#include <QDebug>
#include <QSqlError>

Eleve::Eleve()
{
    eleveId  = 0;
    nom = "";
    prenom = "";
    dateNaissance = QDate();
    adresse = "";
    nomParentTuteur = "";
    numeroTelephoneParent = "";
    dateInscription = QDate();
    classe = 0;
}

Eleve::Eleve(int id, QString nom, QString prenom, QDate dateNaissance, QString adresse, QString nomParentTuteur,
             QString numeroTelephoneParent, QDate dateInscription, int classe)
{
    this->eleveId  = id;
    this->nom = nom;
    this->prenom = prenom;
    this->dateNaissance = dateNaissance;
    this->adresse = adresse;
    this->nomParentTuteur = nomParentTuteur;
    this->numeroTelephoneParent = numeroTelephoneParent;
    this->dateInscription = dateInscription;
    this->classe = classe;
}

QString Eleve::getNom()
{
    return nom;
}

QString Eleve::getPrenom()
{
    return prenom;
}


bool Eleve::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO Eleves (Nom, Prenom, Date_Naissance, Adresse, Nom_Parent_Tuteur, Numero_Telephone_Parent, Date_Inscription, Classe) "
                  "VALUES (:nom, :prenom, :dateNaissance, :adresse, :nomParentTuteur, :numeroTelephoneParent, :dateInscription, :classe)");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaissance", dateNaissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":nomParentTuteur", nomParentTuteur);
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

bool Eleve::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE Eleves SET Nom = :nom, Prenom = :prenom, Date_Naissance = :dateNaissance, "
                  "Adresse = :adresse, Nom_Parent_Tuteur = :nomParentTuteur, Numero_Telephone_Parent = :numeroTelephoneParent, "
                  "Date_Inscription = :dateInscription, Classe = :classe WHERE ID_Eleve = :eleveId");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaissance", dateNaissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":nomParentTuteur", nomParentTuteur);
    query.bindValue(":numeroTelephoneParent", numeroTelephoneParent);
    query.bindValue(":dateInscription", dateInscription);
    query.bindValue(":classe", classe);
    query.bindValue(":eleveId", eleveId);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error in modifier(): " << query.lastError().text();
        return false;
    }
}

bool Eleve::supprimer()
{
    QSqlQuery query;
    query.prepare("DELETE FROM Eleves WHERE ID_Eleve = :eleveId");
    query.bindValue(":eleveId", eleveId);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error in supprimer(): " << query.lastError().text();
        return false;
    }
}

QSqlQueryModel* Eleve::rechercher(QString nom, QString prenom, int classe)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * FROM Eleves WHERE Nom LIKE :nom AND Prenom LIKE :prenom AND Classe = :classe");
    query.bindValue(":nom", "%" + nom + "%");
    query.bindValue(":prenom", "%" + prenom + "%");
    query.bindValue(":classe", classe);

    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Error in rechercher(): " << query.lastError().text();
    }

    return model;
}

QSqlQueryModel* Eleve::trierParNomDateInscriptionClasse()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM Eleves ORDER BY Nom ASC, Date_Inscription ASC, Classe ASC");
    return model;
}
