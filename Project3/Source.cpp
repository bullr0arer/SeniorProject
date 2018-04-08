#define _CRT_SECURE_NO_DEPRECATE
#define MINWHEELMOVEMENT 120
#include <windows.h>;
#include <list>;
#include <vector>;
#include <Magick++.h>;
#include <string>;
#include <iostream>;
#include <conio.h>;
#include "Page.h";
#include "Scroller.h";
#include "Session.h";
#include "PathName.h";

/*The MusicScroller relies on Magick++ (http://imagemagick.sourceforge.net/http/www/windows.html), its dependency: GhostScript, and google chrome 
being installed (in the default directories). 

Application MUST be executed in release mode

This program is for educational purposes only.
*/


using namespace std;
using namespace Magick;

static int handler(Session *thisSession, int selection);

int main(int argc, char *argv[]) {

	Session thisSession;
	int selection = 1;
	selection = handler(&thisSession, selection);

	while (selection != 0) {
		if (selection == 1) {
			selection = handler(&thisSession, selection);
		}
		else {
			selection = handler(&thisSession, 2);
		}
	}
	for (int i = 0; i < thisSession.getimageCount(); i++) {                                            
		string name = thisSession.getfolderPath() + string("fullfile7584923056784239017851-7584139-574891785491-5749-137589-41857-1") + std::to_string(i) + ".bmp";   
		std::remove(name.c_str());
	}
}



static int handler(Session *thisSession, int selection)
{

	InitializeMagick("");



	double tempo;
	double bpm;
	string location;



	if(selection!=2){
		PathName p;
		location = p.getPathName();
		if (location == "na") {
			return 0;
		}
		cout << "Please input the tempo";
		cin >> tempo;
		if (cin.fail()) {
			tempo = 120;
		}
		cout << "Please input the beats per measure";
		cin >> bpm;
		if (cin.fail()) {
			bpm = 4;
		}
	}
	else {
		location = thisSession->getfileNPath();
		tempo = thisSession->gettempo();
		bpm = thisSession->getbpm();
	}


	cout << location;

	TCHAR fPath[MAX_PATH];
	GetModuleFileName(NULL, fPath, sizeof(fPath));
	string path = fPath;
	path = path.substr(0, path.rfind("\\"));
	path = path.substr(0, path.rfind("\\"));
	path = path.substr(0, path.rfind("\\"));

	path = path + "\\PixP\\";
	int numImages;
			
	try {

		try{

			list<Image> imageList;

			readImages(&imageList, location);

			numImages = imageList.size();

			for (int g = 0; g < numImages; g++) {

				string tmpFile = "BMP3:" + path + string("fullfile7584923056784239017851-7584139-574891785491-5749-137589-41857-1") + std::to_string(g) + ".bmp";
				size_t dep = 24;
				imageList.front().depth(8);
				imageList.front().alpha(false);
				imageList.front().type(TrueColorType);
				imageList.front().defineSet("bmp:bit-depth", "8");
				imageList.front().type(Magick::GrayscaleType);
				imageList.front().write(tmpFile.c_str());
				imageList.erase(imageList.begin());
				
			}
		}
			
		catch (Magick::ErrorBlob p){
			std::cout << p.what() << std::endl;
			system("pause");
			exit(1);
		}

		
	}
			
	catch (Exception &error_){
		cout << "Caught exception: " << error_.what() << endl;
		system("pause");
		exit(1);
	}

	std::vector<Page> pStor;

	try {
		for (int length = 0; length < numImages; length++) {

			string filename = path + string("fullfile7584923056784239017851-7584139-574891785491-5749-137589-41857-1") + std::to_string(length) + ".bmp";

			int i;
			FILE* f = fopen((filename.c_str()), "rb");

			if (f == NULL) {
				throw "Argument Exception";
			}
			unsigned char info[54];
			fread(info, sizeof(unsigned char), 54, f);


			int width = *(int*)&info[18];
			int height = *(int*)&info[22];
			int pixelSize = *(int*)&info[28];
			int offset = (*(int*)&info[10]) - 54;

			unsigned char tmp;
			unsigned char *row = new unsigned char[(width * 3 + 3) & (~3)];
			std::vector<std::vector<unsigned short int>> data(width + 3, std::vector<unsigned short int>(height));
			unsigned char *off = new unsigned char[offset];
			fread(off, sizeof(unsigned char), offset, f);

			for (int i = height - 1; i >= 0; i--) {
				fread(row, sizeof(unsigned char), ((width * 3 + 3) & (~3)), f);

				for (int j = 0; j < width * 3; j = j + 3) {
					tmp = (row[j] + row[j + 1] + row[j + 2]) / 3;

					if (tmp > 200) {
						data[j / 3][i] = 1;
						//cout << 1;
					}
					else {
						data[j / 3][i] = 0;
						//cout << 2;
					}
					int out = data[j / 3][i];
				}
				printf("\n");
			}
			fclose(f);

			delete[] off;
			delete[] row;



			Page tester;
			tester.EnumMeasures(data, width, height);
			pStor.push_back(tester);
		}
	}
	catch (Exception &error_) {
		cout << "Caught exception: " << error_.what() << endl;
		system("pause");
		exit(1);
	}

	UINT userSetting;
	BOOL settingCheck = SystemParametersInfo(SPI_GETWHEELSCROLLLINES,
		0,
		&userSetting,
		0);
	if (userSetting != 3) {
		BOOL settingSet = SystemParametersInfo(SPI_SETWHEELSCROLLLINES,
			3,
			&userSetting,
			0
		);
		cout << "changed to 3" << endl << userSetting;
	}

	std::string paramStr = location + " --start-maximized --new-window"; //--force-device-scale-factor=3";
	LPCTSTR param = paramStr.c_str();
	std::string OpenWithStr = "C:\\Program Files (x86)\\Google\\Chrome\\Application\\Chrome.exe";  //Have zoom set to ~200% or adobe "fit to page"
	LPCTSTR OpenWith = OpenWithStr.c_str();
	LPCTSTR op = "Open";
	ShellExecute(0, op, OpenWith, param, 0, SW_SHOWMAXIMIZED);

	
	cout << "Press enter when you are ready to begin playing. Scrolling can be stopped at any time by press ESC";
	
	int confirm = 0;
	while (confirm == 0) {
		Scroller initialScroll(10, &timer);
		short state = GetAsyncKeyState(VK_RETURN);
		if (state & 0x8000){
			confirm = 1;
		}
	}

	HWND wind = GetFocus();

	/*RECT rect;
	GetWindowRect(wind, &rect);
	int winHeight = rect.bottom - rect.top;*/

	double spb = 60 / tempo;      //sbp= seconds per beat
	double spm = spb * bpm;       //spm= seconds per measure



	double mCount = pStor[0].getMeasures()[1] - 1;
	double initialMS = spm/(bpm*2);
	Scroller initialScrollWait(initialMS, &timer);
	double rowH;
	double nextmCount;

	for (int y = 0; y < pStor.size(); y++) {
		std::vector<int> rowSpots = pStor[y].getnewRows();
		std::vector<int> nextRowSpots;
		std::vector<int> prevRowSpots;

		for (int h = 0; h < rowSpots.size(); h++) {
			cout << endl << rowSpots[h];              // locations of measures on current line
		}

		if (y != 0) { 
			prevRowSpots = pStor[y - 1].getnewRows();  // locations of measures on previous line
		}
		if (y != pStor.size() - 1) { 
			nextRowSpots = pStor[y + 1].getnewRows();   // locations of measures on next line
		}

		double sizeFactor = .025;
		double ticksPerPage = pStor[y].getHeight()*sizeFactor;


		for (int line = 0; line < pStor[y].getMeasures().size(); line++) {
			mCount = pStor[y].getMeasures()[line];  // # of measures on current line
			cout << endl << mCount << endl;
			
			//
			if (line == pStor[y].getMeasures().size() - 1) {

				if (y != pStor.size() - 1) { 
					nextmCount = pStor[y + 1].getMeasures()[0]; 
				}
				else { 
					nextmCount = mCount; 
				}
			}
			else {
				nextmCount = pStor[y].getMeasures()[line + 1]; 
			}

			if (line == pStor[y].getMeasures().size() - 1) {
				if (y != pStor.size() - 1) { 
					rowH = ((rowSpots[line] - rowSpots[line - 1]) / 2) + (((pStor[y].getHeight() - rowSpots[line]) + (nextRowSpots[0])/2)); 
				}
					
				else { 
					rowH = pStor[y].getHeight() - rowSpots[line];
				}
			}

			else if (line == 0) {

				if (y == 0) { 
					rowH = (rowSpots[line] / 2) + ((rowSpots[line + 1] - rowSpots[line]) / 2); 
				}
				else { 
					rowH = ((rowSpots[line] + pStor[y - 1].getHeight() - prevRowSpots[pStor[y - 1].getMeasures().size() - 1]) / 2) + ((rowSpots[line + 1] - rowSpots[line]) / 2); 
				}
			}
			else { 
				rowH = ((rowSpots[line] - rowSpots[line - 1]) / 2) + ((rowSpots[line + 1] - rowSpots[line]) / 2); 
			}

			double thisFactor = rowH / pStor[y].getHeight();                      
			double spl = spm * mCount;											   

			double ticks = thisFactor * ticksPerPage * 30;						  
			int pace = int(spl * 1000);									  
			double msPerTick = pace / ticks;							  

			for (int r = 0; r < ticks; r++) {
				if (!GetAsyncKeyState(VK_ESCAPE))
				{
					Scroller scroll(msPerTick, &timer);
					mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -MINWHEELMOVEMENT / 30, 0);    
				}
				else {
					goto label;
				}
			}
		}
	}
	label:
	cout << "Please enter 0 to exit, 1 to restart the application with new inputs, or 2 to restart the application with the same inputs";
	cin >> selection; 
	if (selection == 2) {
		thisSession->setbpm(bpm);
		thisSession->settempo(tempo);
		thisSession->setfileNPath(location);
	}
	thisSession->setfolderPath(path);
	thisSession->setimageCount(numImages);

	return selection;
}

