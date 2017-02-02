#include <iostream>
#include <fstream>
#include <string>
#include <QtCore/QCoreApplication>
#include <QtGui/QImage>
#include <direct.h>

using namespace std;

int startImgNum = 1;
int imgNum =  22;
int maskHeight = 47;

int main(int argc, char *argv[])
{
	string folder, True, False;
	
	folder += "./imageData-";
	folder += to_string(maskHeight);
	True = folder;
	True += "/1/";
	False = folder;
	False += "/0/";
	_mkdir(folder.c_str());
	_chdir(folder.c_str());
	_mkdir("0");
	_mkdir("1");
	_chdir("../");
	//ofstream outputFile("train.txt");

	for (int m = startImgNum; m <= imgNum; m++)
	{

		//Read img
		string groundTruth, test;
		groundTruth += "./image/groundTruth";
		groundTruth += to_string(m);
		groundTruth += ".bmp";
		test += "./image/test";
		test += to_string(m);
		test += ".bmp";
		
		QImage imgGroundTruth(QString::fromStdString(groundTruth));
		QImage imgTest(QString::fromStdString(test));

		for (int i = 0; i < imgTest.height() ; i++)
		{
			for (int j = 0; j < imgTest.width(); j++)
			{
				QColor clrCurrent(imgGroundTruth.pixel(j, i));
				string filename;
				QRect rect(QPoint(j - maskHeight/2, i - maskHeight/2), QPoint(j + maskHeight/2, i + maskHeight/2));
				if (clrCurrent.red() == 255)
				{
					filename = "imageData/1/";
				}
				else
				{
					filename = "imageData/0/";
				}
				filename += to_string(m);
				filename += "_";
				filename += to_string(i);
				filename += "_";
				filename += to_string(j);
				filename += ".PNG";
				//outputFile << filename << " 0" <<endl;
				QImage copy = imgTest.copy(rect);
				copy.save(QString::fromStdString(filename));
			}
		}
		
	}
	
	//return a.exec();
	return 0;
}
