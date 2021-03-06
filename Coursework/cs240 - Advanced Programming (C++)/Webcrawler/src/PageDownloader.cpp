#include "PageDownloader.h"
#include "URLInputStream.h"
#include <iostream>
#include "UnitTest.h"
using namespace std;

std::string PageDownloader::DownloadPage(const std::string& url){
	URLInputStream stream(url);
	string html;
	while (!stream.IsDone()){
		html += stream.Read();
	}
	
	return html;
}

bool PageDownloader::Test(ostream& os){
	os << "Testing PageDownloader..." << endl;
	
	PageDownloader download;
	bool success = true;
	try{
		download.DownloadPage("http://www.mormon.org");
	}
	catch(...){
		success = false;
	}
	return success;
}
