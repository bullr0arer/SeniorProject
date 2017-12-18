#pragma once
#ifndef PathName_H
#define PathName_H

#include <string>;
#include <windows.h>;
#include <iostream>;


class PathName {

private:


public:

	std::string getPathName() {
		OPENFILENAME ofn;
		char szFile[260];
		HWND hwnd = 0;
		HANDLE hf;

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;

		ofn.lpstrFile = szFile;

		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);

		ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn) == TRUE) {

			if (ofn.lpstrFile != NULL) {
				std::cout << ofn.lpstrFile;
				std::string fp = ofn.lpstrFile;
				return fp;
			}
			else
				return "na";
		}
		else
			return "na";
	}
};

#endif