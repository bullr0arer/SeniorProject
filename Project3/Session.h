#pragma once
#ifndef Session_H
#define Session_H

#include <vector>
#include <string>

using namespace std;

class Session
{

private:
	int imageCount = 0;
	string folderPath;
	string fileNPath;
	double tempo;
	double bpm;
public:
	Session::Session( )
	{

	}
	void setimageCount(int imageCount1) {
		imageCount = imageCount1;
	}
	void setfolderPath(string folderPath1) {
		folderPath = folderPath1;
	}
	void setfileNPath(string fileNPath1) {
		fileNPath = fileNPath1;
	}
	void settempo(double tempo1) {
		tempo = tempo1;
	}
	void setbpm(double bpm1) {
		bpm = bpm1;
	}

	int getimageCount() {
		return imageCount;
	}
	string getfolderPath() {
		return folderPath;
	}
	string getfileNPath() {
		return fileNPath;
	}
	double gettempo() {
		return tempo;
	}
	double getbpm() {
		return bpm;
	}
};

#endif
