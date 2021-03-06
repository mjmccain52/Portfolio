#include "URL.h"
#include "UnitTest.h"
#include <iostream>
#include "StringUtil.h"
#include <sstream>
#include "URLInputStream.h"
using namespace std;

URL::URL(std::string url, std::string b) : original(url), baseString(b){
	ResolveURL();
	MakePrefix();
}
//for test purposes only
URL::URL(std::string url) : resolved(url){
	Parse(resolved);
	string temp = schemeString + "//" + domain + "/";
	if (pathSize > 0){
		resolved = temp + resolved.substr(temp.length());
	}else{
		resolved = temp;
	}
	MakePrefix();
}

void URL::Redirect(){
	URLInputStream source (resolved); //opens a connection
	string newResolved = source.GetLocation();
	if (newResolved != resolved){
		string temp = DoubleCheck(newResolved);
		resolved = temp + newResolved.substr(temp.length());
	}
}

string URL::DoubleCheck(string& toParse){
	domain = "";
	stringstream tokenizer(toParse);
	getline(tokenizer, schemeString, '/');
	StringUtil::ToLower(schemeString);
	/*These two getlines advance the buffer two slashes, 
	picking up the web domain if there is one*/
	getline(tokenizer, domain, '/');
	getline(tokenizer, domain, '/');
	StringUtil::ToLower(domain);
	
	return schemeString + "//" + domain;
}
void URL::MakePrefix(){
	int lastSlash = resolved.find_last_of('/') + 1;
	if (lastSlash > 8){
		prefix = resolved.substr(0,lastSlash);
	}else{
		prefix = resolved;
	}
	if (prefix[prefix.length() - 1] != '/'){
		prefix += '/';
	}
}

std::string* URL::GetPath(){
	return pathTokens;
}

string URL::GetScheme() const {
	return schemeString;
}

string URL::GetDomain() const {
	return domain;
}

URL::~URL(){}
void URL::Parse(string& toParse){
	pathSize = 0;
	domain = "";
	stringstream tokenizer(toParse);
	getline(tokenizer, schemeString, '/');
	StringUtil::ToLower(schemeString);
	/*These two getlines advance the buffer two slashes, 
	picking up the web domain if there is one*/
	getline(tokenizer, domain, '/');
	getline(tokenizer, domain, '/');
	StringUtil::ToLower(domain);

	while (getline(tokenizer, pathTokens[pathSize], '/')){
		pathSize++;		
	}

}
std::string URL::GetResolvedURL() const{
	return resolved;
}

int URL::GetPathSize(){
	return pathSize;
}

string URL::GetPrefix() const{
	return prefix;
}

void URL::ResolveURL(){
	
	string tempCopy = original;
	StringUtil::ToLower(tempCopy);
	//Page Anchors
	int anchor = original.find('#');
	if (anchor > 0){
		original = original.substr(0, anchor);
	}
	
	if (original[0] == '#' || original == baseString){
		resolved = baseString;
		return;
	}
	else if (tempCopy.substr(0,7) == "http://" || tempCopy.substr(0,8) == "file:///"){
		Parse(original);
		string temp = schemeString + "//" + domain;
		resolved = temp + original.substr(temp.length());
		return;		
	}

	Parse(baseString);
	if (original[0] == '/'){
		resolved = schemeString + "//" + domain + original;
		return;
	}
	
	resolved = schemeString + "//" + domain + "/";
	climbBack(original, pathSize);
	
	for (int j = 0; j < pathSize - back - 1; j++){
		resolved += pathTokens[j] + "/";
	}
	
	string moreParsing = original.substr(resumeIndex);
	resolved += moreParsing;
	stringstream parse(moreParsing);
	if (pathSize > 0){
		pathSize = pathSize - back - 1;
	}
	while(getline(parse, pathTokens[pathSize], '/')){
		pathSize++;
	}
}

int URL::climbBack(const string& link, int index){
	back = 0;
	resumeIndex = 0;
	for (unsigned int i = 0, dots = 0; i < link.length(); i++){
		if (link[i] == '.'){
			dots++;
		}else if (link[i] == '/'){
			if (dots == 2){
				back++;
			}
			dots = 0;
		}else{
			resumeIndex = i;
			break;
		}
	}
	return 0;
}


bool URL::Test(ostream& os){
	bool success = true;
	
	os << "Testing URL..." << endl;
	os << "\tTesting Constructors..." << endl;
	URL u("http://www.a.com", "http://www.b.com");
	TEST(u.original == "http://www.a.com");
	TEST(u.baseString == "http://www.b.com");
	URL ui("file:///a/b");
	TEST(ui.resolved == "file:///a/b");
	
	//----------------------------------
	os << "\tTesting Accessors..." << endl;
	URL acc("http://www.a.com", "http://www.b.com");
	acc.pathTokens[2] = "5";
	acc.pathSize = 5;
	acc.resolved = "r";
	acc.prefix = "p";
	acc.domain = "d";
	acc.schemeString = "s";
	TEST (acc.GetPath()[2] == "5");
	TEST(acc.GetDomain() == "d");
	TEST(acc.GetScheme() == "s");
	TEST(acc.GetPathSize() == 5);
	TEST(acc.GetResolvedURL() == "r");
	TEST(acc.GetPrefix() == "p");
	
	//----------------------------------
	os << "\tTesting ClimbBack..." << endl;
	acc.climbBack(".././../a/b", 0);
	TEST( acc.back == 2 && acc.resumeIndex == 8);
	acc.climbBack("index", 0);
	TEST( acc.back == 0 && acc.resumeIndex == 0);
	acc.climbBack("./index", 0);
	TEST( acc.back == 0 && acc.resumeIndex == 2);
	
	//----------------------------------
	
	os << "\tTesting Parse..." << endl;
	string str = "HTTP://WWW.A.COM/a/b";
	acc.Parse(str);
	TEST(acc.schemeString == "http:" && acc.domain == "www.a.com" && acc.pathSize == 2);	
	
	//----------------------------------
	os << "\tTesting ResolveURL..." << endl;
	
	os << "\t\t'#' links..." << endl;
	URL anchored("#index", "www.a.com");
	TEST(anchored.resolved == "www.a.com");
	URL anchored2("http://www.b.com/pages#toys", "http://www.a.com");
	TEST(anchored2.resolved == "http://www.b.com/pages");

	os << "\t\tabsolute links..." << endl;
	TEST(u.resolved == "http://www.a.com");
	TEST(u.pathSize == 0);
	
	URL u2("http://www.a.com/a/b/c", "http://www.b.com");
	TEST(u2.resolved == "http://www.a.com/a/b/c");
	TEST(u2.pathSize == 3);
	
	os << "\t\t'/' links..." << endl;
	URL slash("/images/index","HTTP://www.a.com");
	TEST(slash.resolved == "http://www.a.com/images/index");
	
	URL slash2("/images/index","HTTP://www.a.com/b/c");
	TEST(slash2.resolved == "http://www.a.com/images/index");
	
	URL slash3("/images/index","file:///");
	TEST(slash3.resolved == "file:///images/index");
	
	os << "\t\trelative links..." << endl;
	URL r("images/index","file:///a/b/c/d");
	TEST(r.resolved == "file:///a/b/c/images/index");
	
	URL r2("./images/index","file:///a/b/c/d");
	TEST(r2.resolved == "file:///a/b/c/images/index");
	TEST(r2.schemeString == "file:");
	
	URL r3("./././images/index","file:///a/b/c/d");
	TEST(r3.resolved == "file:///a/b/c/images/index");
	
	URL r4(".././../images/index","file:///a/b/c/d");
	TEST(r4.resolved == "file:///a/images/index");
	
	URL r5("../images/index","htTp://www.a.com/a/b/c/d");
	TEST(r5.resolved == "http://www.a.com/a/b/images/index");
	TEST(r5.domain == "www.a.com");
	TEST(r5.pathTokens[0] == "a" && r5.pathTokens[3] == "index");
	
	URL r6("./images/index","htTp://www.a.com");
	TEST(r6.resolved == "http://www.a.com/images/index");
	TEST(r6.domain == "www.a.com");
	
	//----------------------------------
	os << "\tTesting Redirect..." << endl;
	URL ur( "crawler/TestOne.html","HTTP://STUDENTS.cs.byu.edu/~cs240ta/crawler_tests/crawlindex.html");
	ur.Redirect();
	TEST(ur.resolved == "http://students.cs.byu.edu/~cs240ta/crawler_tests/crawler/TestOne.html");
	URL lds("http://www.mormon.com");
	lds.Redirect();
	TEST(lds.resolved == "http://www.mormon.org");
	
	//----------------------------------
	os << "\tTesting DoubleCheck..." << endl;
	string string1 = "file:///a/b";
	TEST (lds.DoubleCheck(string1) == "file://");
	string1 = "HTTP://WWW.A.COM/a/b";
	TEST (lds.DoubleCheck(string1) == "http://www.a.com");
	
	//----------------------------------
	os << "\tTesting MakePrefix..." << endl;
	URL p1("http://www.a.com/a");
	TEST (p1.prefix == "http://www.a.com/");
	
	URL p2("file:///a/b/c/");
	TEST(p2.prefix == "file:///a/b/c/");
	
	URL p3("images/index", "http://www.a.com");
	TEST(p3.prefix == "http://www.a.com/images/");
	
	URL p4("http://www.a.com");
	TEST(p4.prefix == "http://www.a.com/");
	
	URL p5("http://a");
	TEST(p5.prefix == "http://a/");
	
	URL p6("file:///");
	TEST(p6.prefix == "file:///");
	
	return success;
}
