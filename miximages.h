#ifndef MIXIMAGES_H
#define MIXIMAGES_H

#include <QMainWindow>
#include <QWidget>
#include <QToolButton>
#include <QComboBox>
#include <QLabel>
#include <QPainter>
#include <QGridLayout>
#include <QFileDialog>
#include <QMenuBar>

namespace Ui {
class MixImages;
}

class MixImages : public QMainWindow
{
    Q_OBJECT

public:
    explicit MixImages(QWidget *parent = 0);
    ~MixImages();

private:
    Ui::MixImages *ui;
    void chooseImage(const QString &title, QImage *image, QImage *originalImage, QToolButton *button);
    void loadImage(const QString &fileName, QImage *image, QImage *originalImage, QToolButton *button);
    QPainter::CompositionMode currentMode() const;
    QPoint imagePos(const QImage &image) const;



    QImage sourceImage;
    QImage sourceImageOriginal;
    QImage destinationImage;
    QImage destinationImageOriginal;
    QImage resultImage;
    QImage resultImageOriginal;


private slots:
    void chooseSource();
    void chooseDestination();
    void recalculateResult();
    void on_actionSave_triggered();
    void on_actionAbout_the_program_triggered();
};

#endif // MIXIMAGES_H
