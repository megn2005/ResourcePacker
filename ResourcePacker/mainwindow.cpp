#include <QDockWidget>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	// 设置窗口标题
	setWindowTitle("资源打包工具");

	// 初始化界面布局
	initLayout();
}

MainWindow::~MainWindow()
{

}


// 初始化界面布局
bool MainWindow::initLayout()
{
	QDockWidget* pFolderDock = new QDockWidget("文件夹浏览", this);
	//addDockWidget(Qt::LeftDockWidgetArea, pFolderDock);

	return true;
}







