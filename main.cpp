#include <iostream>
#include <fstream>
#include <string>
#include <QtCore/QCoreApplication>
#include <QtGui/QImage>
#include <direct.h>



using namespace std;

int imgNum = 10;

int main(int argc, char *argv[])
{
	//QCoreApplication a(argc, argv);
	string folder, True, False;
	
	folder += "./imageData";
	True = folder;
	True += "/1/";
	False = folder;
	False += "/0/";
	_mkdir(folder.c_str());
	_chdir(folder.c_str());
	_mkdir("0");
	_mkdir("1");
	_chdir("../");
	ofstream outputFile("train.txt");

	for (int m = 1; m <= imgNum; m++)
	{

		//Read img
		string groundTruth, test;
		groundTruth += "./image/groundTruth";
		groundTruth += to_string(m);
		groundTruth += ".PNG";
		test += "./image/test";
		test += to_string(m);
		test += ".PNG";
		
		QImage imgGroundTruth(QString::fromStdString(groundTruth));
		QImage imgTest(QString::fromStdString(test));

		for (int i = 16; i < imgTest.height() - 17; i++)
		{
			for (int j = 16; j < imgTest.width() - 17; j++)
			{
				QColor clrCurrent(imgGroundTruth.pixel(j, i));
				if (clrCurrent.red() == 255)
				{
					
					string filename;
					//QRect rect(QPoint(j - 16, i - 16), QPoint(j + 16, i + 16));
					filename = "\\imageData\\1\\";
					filename += to_string(m);
					filename += "_";
					filename += to_string(i);
					filename += "_";
					filename += to_string(j);
					filename += ".PNG";
					/*
					QImage copy = imgTest.copy(rect);
					copy.save(QString::fromStdString(filename));
					*/
					outputFile << filename << " 1" << endl;
				}
				else
				{
					string filename;
					//QRect rect(QPoint(j - 16, i - 16), QPoint(j + 16, i + 16));
					filename = "\\imageData\\0\\";
					filename += to_string(m);
					filename += "_";
					filename += to_string(i);
					filename += "_";
					filename += to_string(j);
					filename += ".PNG";
					/*
					QImage copy = imgTest.copy(rect);
					copy.save(QString::fromStdString(filename));
					*/
					outputFile << filename << " 0" <<endl;
				}
			}
		}
		
	}
	
	//return a.exec();
	return 0;
}
