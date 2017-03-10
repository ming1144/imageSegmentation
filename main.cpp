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
bool padding = true;

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

	string testImageFolder = "trainingData/all/";

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
		QImage imgTestGray(imgTest.width(), imgTest.height(), QImage::Format_Grayscale8);
		imgTestGray = imgTest.convertToFormat(QImage::Format_Grayscale8);

		uchar* p_imgGroundTruth, *p_imgTestGray, *p_ROI;

		int start = maskHeight / 2;
		if (padding)
		{
			start = 0;
		}

		p_imgGroundTruth = imgGroundTruth.bits();
		p_imgTestGray = imgTestGray.bits();


		for (int i = start; i < imgTest.height() - start; i++, p_imgGroundTruth += imgGroundTruth.bytesPerLine(), p_imgTestGray += imgTestGray.bytesPerLine())
		{
			for (int j = start; j < imgTest.width() - start; j++)
			{
				bool groundTruthIsRed;
				if (p_imgGroundTruth[j * 4 + 2] == 255)
				{
					groundTruthIsRed = true;
				}
				else
				{
					groundTruthIsRed = false;
				}

				QImage ROI(maskHeight, maskHeight,QImage::Format_Grayscale8);
				ROI.fill(QColor(0, 0, 0));
				uchar* temp_p_imgTestGray;
				int x_start, x_end, y_start, y_end;

				if (i < maskHeight / 2)
				{
					y_start = maskHeight / 2 - i;
				}
				else
				{
					y_start = 0;
				}

				if (i > imgGroundTruth.height() - maskHeight / 2)
				{
					y_end = maskHeight - imgGroundTruth.height() - i;
				}
				else
				{
					y_end = maskHeight;
				}

				if (j < maskHeight / 2)
				{
					x_start = maskHeight / 2 - j;
				}
				else
				{
					x_start = 0;
				}

				if (j > imgGroundTruth.width() - maskHeight / 2)
				{
					x_end = maskHeight - imgGroundTruth.width() - j;
				}
				else
				{
					x_end = maskHeight;
				}

				for (int y = y_start; y < y_end; y++)
				{
					p_ROI = ROI.scanLine(y);
					temp_p_imgTestGray = imgTestGray.scanLine(i + (y - maskHeight / 2));
					for (int x = x_start; x < x_end; x++)
					{
						p_ROI[x] = temp_p_imgTestGray[j + (x - maskHeight / 2)];
					}
				}

				string filename;
				if (groundTruthIsRed)
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
				ROI.save(QString::fromStdString(filename));
			}
		}
	}

	return 0;
}
