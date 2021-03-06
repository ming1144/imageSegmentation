#include <iostream>
#include <fstream>
#include <string>

#include <QtGui/QImage>


//MSVC
#ifdef _WIN32
#include <direct.h>
#define mkdir(x,y) _mkdir(x)
#define chdir(x) _chdir(x)
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

using namespace std;

int startImgNum = 2;
int imgNum =  24;
int maskHeight = 33;
bool padding = false;

int main(int argc, char *argv[])
{
	string folder, True, False;
	
	folder += "./imageDataCT-";
	folder += to_string(maskHeight);
	True = folder;
	True += "/1/";
	False = folder;
	False += "/0/";
	mkdir(folder.c_str(), 0777);
	chdir(folder.c_str());
	mkdir("0", 0777);
	mkdir("1", 0777);
	chdir("../");

	for (int m = startImgNum; m <= imgNum; m++)
	{

		//Read img
		string groundTruth, test;
		groundTruth = "./CTmerge/groundTruth";
		groundTruth += to_string(m);
		groundTruth += ".png";
		test = "./CTmerge/merge";
		test += to_string(m-1);
		test += "_";
		test += to_string(m);
		test += "_";
		test += to_string(m+1);
		test += ".png";
		
		QImage imgGroundTruth(QString::fromStdString(groundTruth));
		QImage imgTest(QString::fromStdString(test));

		int start = maskHeight/2;
		if (padding)
		{
			start = 0;
		}
		for (int i = start; i < imgTest.height() - start; i++)
		{
			for (int j = start; j < imgTest.width() - start; j++)
			{
				QColor clrCurrent(imgGroundTruth.pixel(j, i));
				string filename;
				QRect rect(QPoint(j - maskHeight/2, i - maskHeight/2), QPoint(j + maskHeight/2, i + maskHeight/2));
				if (clrCurrent.red() == 255 && clrCurrent.green() == 0 && clrCurrent.blue() == 0)
				{
					filename = True;
				}
				else
				{
					filename = False;
				}
				filename += to_string(m);
				filename += "_";
				filename += to_string(i);
				filename += "_";
				filename += to_string(j);
				filename += ".PNG";
				QImage copy = imgTest.copy(rect);
				copy.save(QString::fromStdString(filename));
			}
		}
		
	}
	
	//return a.exec();
	return 0;
}
