#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <opencv2/highgui/highgui.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer= new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(DisplayImage()));
    //timer->start();
    vid=new cv::VideoCapture();
    termcrit = cv::TermCriteria(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);
    subPixWinSize = cv::Size(10,10);
    winSize = cv::Size(31,31);

}

MainWindow::~MainWindow()
{
    timer->stop();
        delete ui;
        delete vid;
}

void MainWindow::on_actionFile_triggered()
{
    QString fname=QFileDialog::getOpenFileName(this,"Open Video File");
        vid->open(fname.toStdString());
        cv::Mat temp;
        vid->read(temp);
        Grid=cv::Mat::zeros(temp.size(),CV_8UC3);
        timer->start();
}

void MainWindow::on_actionCamera_triggered()
{
    vid->open(0);
        cv::Mat temp;
        vid->read(temp);
        Grid=cv::Mat::zeros(temp.size(),CV_8UC3);
        timer->start();
}

void MainWindow::DisplayImage()
{
    if(vid->read(frame))
                {
                    frame.copyTo(image);
                    cv::cvtColor(image,gray,cv::COLOR_BGR2GRAY);
                    if( needToInit )
                    {
                        // automatic initialization
                        cv::goodFeaturesToTrack(gray, points[0], MAX_COUNT, 0.1, 10, cv::Mat());
                        //cv::cornerSubPix(gray, points[1], subPixWinSize, cv::Size(-1,-1), termcrit);
                        needToInit = false;
                    }
                    else if( !points[1].empty() )
                    {
                        std::vector<uchar> status;
                        std::vector<float> err;
                        if(prevGray.empty())
                            gray.copyTo(prevGray);
                        cv::calcOpticalFlowPyrLK(prevGray, gray, points[1], points[0], status, err, winSize,
                                             3, termcrit, 0, 0.001);

                        for( i = k = 0; i < points[1].size(); i++ )
                        {
                            if( (points[0][i].x -points[1][i].x )>0 )
                            {
                                cv::line(image,points[0][i],points[1][i],cv::Scalar(0,0,255),1,1,0);
                                cv::circle(image,points[0][i],2,cv::Scalar(255,0,0),1,1,0);

                                cv::line(Grid,points[0][i],points[1][i],cv::Scalar(0,0,255),1,1,0);
                                cv::circle(Grid,points[0][i],2,cv::Scalar(255,0,0),1,1,0);


                            }
                            else
                            {
                                cv::line(image,points[0][i],points[1][i],cv::Scalar(0,255,0),1,1,0);
                                cv::circle(image,points[0][i],2,cv::Scalar(255,0,0),1,1,0);

                                cv::line(Grid,points[0][i],points[1][i],cv::Scalar(0,255,0),1,1,0);
                                cv::circle(Grid,points[0][i],2,cv::Scalar(255,0,0),1,1,0);
                            }
                            points[0][k++] = points[0][i];

                        }
                        cv::goodFeaturesToTrack(gray, points[0], MAX_COUNT, 0.3, 7, cv::Mat(), 7);
                    }
                    std::swap(points[1], points[0]);
                    points[0].clear();
                    gray.copyTo(prevGray);
                    cv::cvtColor(image,image,CV_BGR2RGB);
                DImage=QImage((uchar*)image.data,image.cols,image.rows,image.step,QImage::Format_RGB888);
                ui->label->setPixmap(QPixmap::fromImage(DImage.scaled(640,480,Qt::KeepAspectRatio)));
                //ThreshFrame.copyTo(TempFrame);
                //     cv::cvtColor(ThreshFrame,TempFrame,cv::COLOR_GRAY2BGR);
                  cv::cvtColor(gray,gray,CV_GRAY2RGB);
                HSVImage=QImage((uchar*)Grid.data,Grid.cols,Grid.rows,Grid.step,QImage::Format_RGB888);
                ui->label_2->setPixmap(QPixmap::fromImage(HSVImage.scaled(640,480,Qt::KeepAspectRatio)));
                char filename[100];
                char trailfilename[100];
                sprintf(filename,"C:/Users/araj/Pictures/capture_%d.png",imcount);
                sprintf(trailfilename,"C:/Users/araj/Pictures/trails_%d.png",imcount);
                imcount++;
                  cv::imwrite(filename,image);
                  cv::imwrite(trailfilename,Grid);
                }
}

void MainWindow::on_actionStop_triggered()
{
    timer->stop();
        vid->release();
        Grid.release();
        points[0].clear();
        points[1].clear();
        needToInit=true;
        HSVImage = QImage();
}

void MainWindow::on_actionSnapshot_triggered()
{
    char filename[100];
     char trailfilename[100];
     sprintf(filename,"/home/arpitha/Pictures/capture_%d.png",imcount);
     sprintf(trailfilename,"/home/arpitha/Pictures/trails_%d.png",imcount);
     imcount++;
       cv::imwrite(filename,image);
       cv::imwrite(trailfilename,Grid);
}

void MainWindow::on_pushButton_clicked()
{
    timer->stop();
     Grid.release();
     points[0].clear();
     points[1].clear();
     needToInit=true;
     HSVImage = QImage();
     cv::Mat temp;
     vid->read(temp);
     Grid=cv::Mat::zeros(temp.size(),CV_8UC3);
     timer->start();
}
