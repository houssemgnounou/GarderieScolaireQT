#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "enseignant.h"
#include "eleve.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btn_openHomeEnseignant_clicked();

    void on_btn_openAjoutEnseignant_clicked();

    void on_btn_AjouterEnseignant_clicked();

    void on_Search_enseignant_textChanged(const QString &arg1);

    void on_Supprimer_enseignant_clicked();

    void onEnseignantDoubleClicked(const QModelIndex&);

    void on_btn_ModifierEnseignant_clicked();

    void on_exporter_enseignant_clicked();

    void on_btn_OpenStat_enseignant_clicked();

    void on_Trie_enseignantDESC_clicked();

    void on_Trie_enseignantASC_clicked();

    ///////////////////////////////////////////////////////////////// partie éleves

    void on_btn_openHomeEleve_clicked();

    void on_btn_openAjoutEleve_clicked();

    void on_btn_AjouterEleve_clicked();

    void on_Search_eleve_textChanged(const QString &arg1);

    void on_Trie_eleveASC_clicked();

    void on_Trie_eleveDESC_clicked();

    void on_Supprimer_eleve_clicked();

    void onEleveDoubleClicked(const QModelIndex&);

    void on_btn_ModifierEleve_clicked();

    void on_btn_OpenStat_eleve_clicked();

private:
    Ui::MainWindow *ui;
    Enseignant enseignant;
    Eleve eleve;
    QString currentEnseignantId;
    QString currentEleveId;
};
#endif // MAINWINDOW_H
