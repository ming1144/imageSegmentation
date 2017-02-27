#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <QtCore/QCoreApplication>
#include <QtGui/QImage>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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

int maskHeight = 33;
bool padding = false;

int main(int argc, char *argv[])
{
	string folder, True, False;
	
	folder += "./imageData-";
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

	
	
	string testImageFolder = "trainingData/male/";


	string testfile_filename = testImageFolder;
	testfile_filename += "origin/testfile.txt";
	ifstream input(testfile_filename);
	vector<string> testfileList;
	string temp1;
	while (getline(input, temp1))
	{
		int temp = temp1.find_last_of('.');
		string temp2 = temp1.substr(0, temp);
		testfileList.push_back(temp2);
	}


	for (int m = 0; m < testfileList.size(); m++)
	{
		//Read img
		string groundTruth, test;
		groundTruth = testImageFolder;
		groundTruth += "groundTruth/";
		groundTruth += testfileList[m].c_str();
		groundTruth += "(R).bmp";
		test = testImageFolder;
		test += "origin/";
		test += testfileList[m].c_str();
		test += ".bmp";
		
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
				filename += testfileList[m].c_str();
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
