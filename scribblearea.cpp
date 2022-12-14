#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "scribblearea.h"

ScribbleArea::ScribbleArea(QWidget *parent) : QWidget(parent){


    // places widgets in the top left
    setAttribute(Qt::WA_StaticContents);

    // default values for variables
    modified = false;
    scribbling = false;
    myPenWidth = 1;
    myPenColour = Qt::blue;
}


// load existing image into ScribbleArea
bool ScribbleArea::openImage(const QString &fileName){
    QImage loadedImage;
    if(!loadedImage.load(fileName)){
        return false;
    }
    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}


// Save image function
bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat){
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());
    if(visibleImage.save(fileName, fileFormat)){
        modified = false;
        return true;
    } else {
        return false;
    }
}

void ScribbleArea::setPenColour(const QColor &newColour){
    myPenColour = newColour;
}

void ScribbleArea::setPenWidth(int newWidth){
    myPenWidth = newWidth;
}

void ScribbleArea::clearImage(){
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}


// Triggers if mouse button is left click
void ScribbleArea::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
        scribbling = true;
    }
}


// for as long as scribbling = true, draw where the mouse pointer is
void ScribbleArea::mouseMoveEvent(QMouseEvent *event){
    if((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
}


// when left click is released, stop drawing
void ScribbleArea::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
    }
}

void ScribbleArea::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}


void ScribbleArea::resizeEvent(QResizeEvent *event){
    if(width() > image.width() || height() > image.height()){
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

// used to draw on the widgets
void ScribbleArea::drawLineTo(const QPoint &endPoint){

    QPainter painter(&image);
    painter.setPen(QPen(myPenColour, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;
}


// triggers when app is resized
void ScribbleArea::resizeImage(QImage *image, const QSize &newSize){

    if(image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));


    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

// print the image
void ScribbleArea::print()
{
    // Check for print dialog availability
#if QT_CONFIG(printdialog)

    // Can be used to print
    QPrinter printer(QPrinter::HighResolution);

    // Open printer dialog and print if asked
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}
