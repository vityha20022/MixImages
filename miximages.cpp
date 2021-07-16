#include "miximages.h"
#include "ui_miximages.h"
#include <QMessageBox>
#include <QInputDialog>


static const QSize resultSize(270, 250);

MixImages::MixImages(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MixImages)
{
    ui->resultLabel = new QLabel;
    ui->firstImage = new QToolButton;
    ui->firstImage->setIconSize(resultSize);
    ui->secondImage = new QToolButton;
    ui->secondImage->setIconSize(resultSize);
    resultImage = QImage(resultSize, QImage::Format_ARGB32_Premultiplied);
    ui->setupUi(this);
    connect(ui->firstImage, &QAbstractButton::clicked, this, &MixImages::chooseSource);
    connect(ui->secondImage, &QAbstractButton::clicked, this, &MixImages::chooseDestination);
    connect(ui->mixButton, &QAbstractButton::clicked, this, &MixImages::recalculateResult);

}


void MixImages::chooseSource()
{
    chooseImage(tr("Choose Source Image"), &sourceImage, &sourceImageOriginal, ui->firstImage);
}

void MixImages::chooseDestination()
{
    chooseImage(tr("Choose Destination Image"), &destinationImage, &destinationImageOriginal, ui->secondImage);
}

void MixImages::chooseImage(const QString &title, QImage *image, QImage *originalImage, QToolButton *button)
{
    QString fileName = QFileDialog::getOpenFileName(this, title);
    QFileInfo fileInfo(fileName);
    if (!fileName.isEmpty()){
        if (fileInfo.completeSuffix() != "bmp" && fileInfo.completeSuffix() != "jpeg" && fileInfo.completeSuffix() != "png" && fileInfo.completeSuffix() != "jpg"){
            QMessageBox::information(this, "Invalid format", "The file must be in BMP/JPEG/PNG/JPG format!");
            return;
        }
        QImage checkedSizeImage;
        checkedSizeImage.load(fileName);

        if (checkedSizeImage.width() > 1024 || checkedSizeImage.height() > 1024){
            QMessageBox::information(this, "Invalid size", "The side of the image must be no more than 1024 pixels!");
            return;
        }
        loadImage(fileName, image, originalImage, button);
    }

}


void MixImages::loadImage(const QString &fileName, QImage *image, QImage *originalImage, QToolButton *button)
{
    image->load(fileName);
    originalImage->load(fileName);

    *image = image->scaled(resultSize, Qt::KeepAspectRatio);

    QImage fixedImage(resultSize, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&fixedImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(fixedImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(imagePos(*image), *image);
    painter.end();
    button->setIcon(QPixmap::fromImage(fixedImage));

    *image = fixedImage;
}

QPoint MixImages::imagePos(const QImage &image) const
{
    return QPoint((resultSize.width() - image.width()) / 2,
                  (resultSize.height() - image.height()) / 2);
}

void MixImages::recalculateResult()
{
    QPainter::CompositionMode mode = QPainter::CompositionMode_DestinationOver;
    resultImageOriginal = QImage(QSize(qMax(destinationImageOriginal.width(), sourceImageOriginal.width()), qMax(destinationImageOriginal.height(), sourceImageOriginal.height())), QImage::Format_ARGB32_Premultiplied);
    QPainter originalPainter(&resultImageOriginal);
    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, destinationImage);
    painter.setCompositionMode(mode);
    painter.drawImage(0, 0, sourceImage);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.fillRect(resultImage.rect(), Qt::white);
    painter.end();

    originalPainter.setCompositionMode(QPainter::CompositionMode_Source);
    originalPainter.fillRect(resultImageOriginal.rect(), Qt::transparent);
    originalPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    originalPainter.drawImage(0, 0, destinationImageOriginal);
    originalPainter.setCompositionMode(mode);
    originalPainter.drawImage(0, 0, sourceImageOriginal);
    originalPainter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    originalPainter.fillRect(resultImageOriginal.rect(), Qt::white);
    originalPainter.end();

    ui->resultLabel->setPixmap(QPixmap::fromImage(resultImage));
}


MixImages::~MixImages()
{
    delete ui;
}


void MixImages::on_actionSave_triggered()
{
    if (!resultImageOriginal.isNull()){
        bool ok;
        QString text = QInputDialog::getText(this, tr("File name"), tr("Enter file name:"), QLineEdit::Normal, QDir::home().dirName(), &ok);
        if (ok && !text.isEmpty()){
            resultImageOriginal.save(text, "PNG");
        }
    } else {
        QMessageBox::information(this, "Invalid save", "There is no file to save!");
    }

}

void MixImages::on_actionAbout_the_program_triggered()
{
     QMessageBox::information(this, "Promt", "This program is\ndesigned to mix\ntwo images. You can\nsave the result of this\nprogram by clicking\nFile -> Save.");
}
