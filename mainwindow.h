#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionFile_triggered();

    void on_actionCamera_triggered();
    void DisplayImage();

    void on_actionStop_triggered();

    void on_actionSnapshot_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QImage DImage;
    QImage HSVImage;
    cv::VideoCapture *vid;

    cv::Mat gray, prevGray, image, frame;
    cv::Mat Grid;
    std::vector<cv::Point2f> points[2];
    cv::TermCriteria termcrit;
    cv::Size subPixWinSize, winSize;
    bool needToInit = true;
    cv::Point2f point;
    bool addRemovePt = false;
    const int MAX_COUNT = 100;
    size_t i, k;
    int imcount=0;


};

#endif // MAINWINDOW_H
