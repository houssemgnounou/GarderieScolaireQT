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
    nomParent = "";
    numeroTelephoneParent = "";
    dateInscription = QDate();
    classe = 0;
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


QSqlQueryModel* Eleve::afficherEleves()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT Nom, Prenom, Date_Naissance, Adresse, Nom_Parent, Numero_Telephone_Parent, Date_Inscription, Classe FROM Eleves");

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
                  "VALUES (:nom, :prenom, :dateNaissance, :adresse, :nomParentTuteur, :numeroTelephoneParent, :dateInscription, :classe)");
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


QString Eleve::GetID(QString numero)
{
    QString id;

    QSqlQuery query;
    query.prepare("SELECT ID_ENSEIGNANT FROM Eleves WHERE Numero_Telephone_Parent = :numeroo");
    query.bindValue(":numeroo", numero);

    if (query.exec() && query.next()) {
        id = query.value(0).toString();
    }

    return id;
}
