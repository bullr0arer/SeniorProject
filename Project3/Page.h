#pragma once
#ifndef Later_H
#define Later_H

#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>;

#include <vector>;

using namespace std;


class Page
{
public:
	Page::Page()
	{
	}
	Page::~Page() {

	}

	void Page::EnumMeasures(std::vector<std::vector<unsigned short int>> &data, int w, int h) {
		width = w;
		height = h;

		double barThreshold = width / 12;

		measures.push_back(0);

		int prevLevel = 0;
		totalMeasures = 0;
		int currRowMeasures = 0;
		int rowCount = 1;

		std::vector<int> prevMeasures;
		bool checker;


		for (int u = 0; u < height - 1; u++) {
			for (int y = 0; y < width - 1; y++) {
				if (data[y][u] == 0) {
					int tmpu = u;
					int count = 1;
					checker = true;
					if (prevMeasures.size() > 0 && abs(prevLevel - u) < barThreshold * .5) {
						for (int pr = 0; pr < prevMeasures.size(); pr++) {
							if (abs(prevMeasures[pr] - y) < width / 20 || abs(y-width)<(width/100)) { checker = false; } // preempt if prev measure within 1/20 of width or edge within 1/100 (for scans with shadows on edge)
						} 
					}
					while ((data[y][tmpu] == 0) && checker) {
						count++;
						++tmpu;
						if (count > barThreshold) {barThreshold=count;}
					}
					if (count >= barThreshold *.95) {
						if (prevLevel == 0 || abs(prevLevel - u) < barThreshold/2) {
							prevMeasures.push_back(y);
							measures[rowCount-1] = ++currRowMeasures;
							if (prevLevel == 0) { prevLevel = u; newRows.push_back((tmpu+u)/2);}
							totalMeasures++;
						}
						else {
							newRows.push_back(u);
							prevMeasures.clear();
							currRowMeasures = 0;
							prevMeasures.push_back(y);
							measures[rowCount - 1] = measures[rowCount - 1] - 1;
							measures.push_back(++currRowMeasures);
							rowCount++;
							prevLevel = u;
							totalMeasures++;
						}
					}
				}
			}
		}
		measures[rowCount - 1] = measures[rowCount - 1] - 1;
		lines = rowCount;



	}

	double getLines() {
		return (double)lines;
	}
	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	std::vector<int> getMeasures() {
		return measures;
	}

	int getTmeasures() {
		return totalMeasures;
	}
	std::vector<int> getnewRows() {
		return newRows;
	}

private:
	int lines;
	int height;
	int width;
	int totalMeasures;
	std::vector<int> newRows;
	std::vector<int> measures;

};

#endif
