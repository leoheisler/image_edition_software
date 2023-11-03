#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_quantizar_clicked();
    void on_saida_clicked();

    void on_inserir_imagem_clicked();

    void on_flip_vertical_clicked();

    void on_flip_horizontal_clicked();

    void on_tons_de_cinza_clicked();

    void on_salvar_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
