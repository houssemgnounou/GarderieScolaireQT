#include "enseignant.h"
#include <QDebug>
#include <QSqlError>

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



Enseignant::Enseignant(int id, QString nom, QString prenom, QDate dateNaissance, QString adresse, QString email,
                       QString numeroTelephone, QDate dateEmbauche, QString matiereEnseignee)
{
    this->enseignantId  = id;
    this->nom = nom;
    this->prenom = prenom;
    this->dateNaissance = dateNaissance;
    this->adresse = adresse;
    this->email = email;
    this->numeroTelephone = numeroTelephone;
    this->dateEmbauche = dateEmbauche;
    this->matiereEnseignee = matiereEnseignee;
}


QString Enseignant::getNom()
{
    return nom;
}

QString Enseignant::getPrenom()
{
    return prenom;
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

bool Enseignant::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE Enseignants SET Nom = :nom, Prenom = :prenom, Date_Naissance = :dateNaissance, "
                  "Adresse = :adresse, Email = :email, Numero_Telephone = :numeroTelephone, Date_Embauche = :dateEmbauche, "
                  "Matiere_Enseignee = :matiereEnseignee WHERE ID_Enseignant = :enseignantId");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaissance", dateNaissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":numeroTelephone", numeroTelephone);
    query.bindValue(":dateEmbauche", dateEmbauche);
    query.bindValue(":matiereEnseignee", matiereEnseignee);
    query.bindValue(":enseignantId", enseignantId);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error in modifier(): " << query.lastError().text();
        return false;
    }
}

bool Enseignant::supprimer()
{
    QSqlQuery query;
    query.prepare("DELETE FROM Enseignants WHERE ID_Enseignant = :enseignantId");
    query.bindValue(":enseignantId", enseignantId);

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
    QSqlQuery query;

    query.prepare("SELECT * FROM Enseignants WHERE Nom LIKE :nom AND Prenom LIKE :prenom AND Matiere_Enseignee LIKE :matiere");
    query.bindValue(":nom", "%" + nom + "%");
    query.bindValue(":prenom", "%" + prenom + "%");
    query.bindValue(":matiere", "%" + matiereEnseignee + "%");

    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Error in rechercher(): " << query.lastError().text();
    }

    return model;
}

QSqlQueryModel* Enseignant::trierParNomDateEmbaucheMatiere()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM Enseignants ORDER BY Nom ASC, Date_Embauche ASC, Matiere_Enseignee ASC");
    return model;
}
