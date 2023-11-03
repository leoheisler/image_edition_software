#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qmessagebox.h>
#include <QPixmap>
#include <QFileDialog>
#include <QColor>
#include <QVector>

bool cinza_clicado = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_quantizar_clicked()
{
    if(cinza_clicado == false){
        QMessageBox::warning(this,"cuidado","A imagem não está em tons de cinza");

    }else{
        QImage alterada = (ui->Photo_alterada->pixmap()).toImage();
        int canais = ui->spinBox->value();
        QColor menor_valor = QColor(255,255,255), maior_valor = QColor(0,0,0), cor_atual;
        int largura = alterada.width();
        int altura = alterada.height();

        for(int y = 0; y < altura; y++){
            for(int x = 0; x < largura; x++ ){
                cor_atual = alterada.pixelColor(x,y);
                if(cor_atual.red() > maior_valor.red()){
                    maior_valor = QColor(cor_atual.red(),cor_atual.green(),cor_atual.blue());
                }
                if(cor_atual.red() < menor_valor.red()){
                    menor_valor = QColor(cor_atual.red(),cor_atual.green(),cor_atual.blue());
                }
            }
        }

        int tamanho_intervalor = maior_valor.red() - menor_valor.red() + 1;


        if(tamanho_intervalor <= canais){
            QMessageBox::information(this,"aviso","A imagem já esta quantizada para esse valor");

        }else if(canais == 0){
            QMessageBox::information(this,"aviso","Não é possivel quantizar para 0 tons");
        }
        else{
            int tb = tamanho_intervalor/canais,value;
            std::vector<double> intervalos;
            for (float inter = menor_valor.red() - 0.5; inter < maior_valor.red() + tb; inter = inter + tb) {
                if(inter > 255){
                    intervalos.push_back(255);
                }else{
                    intervalos.push_back(inter);
                }

            }
            qDebug() << menor_valor.red() <<" [] "<<maior_valor.red() <<"\n"<<intervalos;

            for(int y = 0; y < altura; y++){
                for(int x = 0; x < largura; x++ ){
                    cor_atual = alterada.pixelColor(x,y);

                    for(int i = 0; i<intervalos.size();i++){
                        if(cor_atual.red() < intervalos[i]){
                            value = int((intervalos[i] + intervalos[i-1])/2);
                            cor_atual = QColor(value,value,value);
                            alterada.setPixelColor(x,y,cor_atual);
                            break;
                        }
                    }
                }
            }


            QPixmap altered = QPixmap::fromImage(alterada);
            ui->Photo_alterada->setPixmap(altered);
        }
    }


}


void MainWindow::on_saida_clicked()
{
    QMessageBox::StandardButton resposta = QMessageBox::question(this,"sair","Deseja mesmo Sair?",QMessageBox::Yes | QMessageBox::No);
    if(resposta == QMessageBox::Yes){
        close();
    }
}


void MainWindow::on_inserir_imagem_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Selecione uma imagem", "", "Imagens (*.png *.jpg *.jpeg *.bmp *.gif *.tif)");
    if (!filePath.isEmpty()) {
        QPixmap imagem(filePath);
        ui->Photo_Original->setPixmap(imagem);
        ui->Photo_alterada->setPixmap(imagem);
    }
    cinza_clicado = false;
}


void MainWindow::on_flip_vertical_clicked()
{
    QImage alterar = (ui->Photo_alterada->pixmap()).toImage();
    QImage virada = alterar;
    int largura = alterar.width();
    int altura = alterar.height();

    for(int y =0; y < altura; y++){
        for(int x = 0; x<largura;x++){
            virada.setPixelColor(x,altura - y - 1,alterar.pixelColor(x,y));

        }
    }


    QPixmap alterada = QPixmap::fromImage(virada);
    ui->Photo_alterada->setPixmap(alterada);
}


void MainWindow::on_flip_horizontal_clicked()
{
    QImage alterar = (ui->Photo_alterada->pixmap()).toImage();
    QImage virada = alterar;
    int largura = alterar.width();
    int altura = alterar.height();

    for(int y =0; y < altura; y++){
        for(int x = 0; x<largura;x++){
            virada.setPixelColor(largura - x - 1,y,alterar.pixelColor(x,y));

        }
    }


    QPixmap alterada = QPixmap::fromImage(virada);
    ui->Photo_alterada->setPixmap(alterada);
}


void MainWindow::on_tons_de_cinza_clicked()
{
    QImage torna_cinza = (ui->Photo_alterada->pixmap()).toImage();
    int largura = torna_cinza.width();
    int altura = torna_cinza.height();
    QColor cor;
    int cor2;
    for(int y = 0; y< altura;y++){
        for(int x = 0; x < largura; x++){
            cor = torna_cinza.pixelColor(x,y);
            cor2 = cor.red()*0.299 + cor.green()*0.587 + cor.blue()*0.114;
            cor = QColor(cor2,cor2,cor2);
            torna_cinza.setPixelColor(x,y,cor);
        }
    }
    QPixmap alterada = QPixmap::fromImage(torna_cinza);
    ui->Photo_alterada->setPixmap(alterada);
    cinza_clicado = true;
}


void MainWindow::on_salvar_clicked()
{
    QImage imagem = (ui->Photo_alterada->pixmap()).toImage();
    QString caminhoDoArquivo = QFileDialog::getSaveFileName(nullptr, "Salvar imagem", "", "Imagens (*.png *.jpg *.bmp);;Todos os arquivos (*.*)");
    imagem.save(caminhoDoArquivo);
}

