#include "Webcrawler.h"
#include "UnitTest.h"
#include <iostream>
#include "CS240Exception.h"

Webcrawler::Webcrawler(const std::string& start, const std::string& output, 
					const std::string& stops){
	outfile  = output;
	base = new URL(start);
	theGrandIndex = new WordIndex;
	pages = new Pages;
	queue = new PageQueue;
	outputGenerator = new OutputGenerator();
	filter = new URLFilter(base, pages, queue);
	parser = new HTMLParser(stops, filter, queue, pages, theGrandIndex);					

}
Webcrawler::~Webcrawler(){
	delete theGrandIndex;
	delete pages;
	delete queue;
	delete outputGenerator;
	delete filter;
	delete parser;
}
bool Webcrawler::Crawl(){
	if (!filter->IsHTML(base)){
		std::cout << "Start file is not HTML." << std::endl;
		return false;
	}
	std::string html = downloader.DownloadPage(base->GetResolvedURL());
	parser->ParseHTML(html, base);
	while (!queue->IsEmpty()){
		URL* current = queue->Serve();
		try{
			html = downloader.DownloadPage(current->GetResolvedURL());
			parser->ParseHTML(html, current);
		}
		catch(CS240Exception& e){
			std::cout << e.GetMessage() << std::endl;
		}
	}
	
	return true;
}

	//! uses the output generator to create an xml file
void Webcrawler::GetXML(){
	outputGenerator->generateXMLFile(outfile, base->GetResolvedURL(), *pages, *theGrandIndex);
}
bool Webcrawler::Test(std::ostream& os){
	bool success = true;
	Webcrawler crawler("http://students.cs.byu.edu/~cs240ta/crawler_tests/crawlindex.html", "test/xmlTest.xml", "test/stopword.txt");
	crawler.Crawl();
	crawler.GetXML();
	return success;
}

