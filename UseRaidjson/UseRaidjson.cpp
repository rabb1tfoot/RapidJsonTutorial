// Hello World example
// This example shows basic usage of DOM-style API.

#pragma warning(disable:4996)

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <iostream>
#include <direct.h>
#include <cstdio>
#include <fstream>
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

using namespace rapidjson;
using namespace std;

//도큐먼트를 콘솔에 프린트합니다.
void printDoc(const Document *d)
{
	rapidjson::StringBuffer strbuf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
	d->Accept(writer);
	puts(strbuf.GetString());
}
//파일로 부터 읽습니다.(절대좌표)
void StreamFileReaderExample()
{
	char curDir[255];
	_getcwd(curDir, 255);
	string dir = curDir;

	string::iterator iter = dir.end();
	int Maxlength = dir.length();
	int sublength = 0;
	for (int i = Maxlength - 1; i > 0; --i)
	{
		sublength++;
		if (dir[i] == '\\')
		{
			break;
		}
	}
	dir = dir.substr(0, Maxlength - sublength);
	dir += "\\Debug\\surfFinder.djob";

	FILE* fp = fopen(dir.c_str(), "rb"); // non-Windows use "r"

	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);

	fclose(fp);

	printDoc(&d);
}

//파일을 저장합니다.
void StreamFileWriterExample()
{
	Document d;
	d.SetObject();

	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

	size_t sz = allocator.Size();

	d.AddMember("version2", 1.5, allocator);
	d.AddMember("testId2", 2, allocator);
	d.AddMember("group2", 3, allocator);
	d.AddMember("order2", 4, allocator);

	Value str(kObjectType);
	string textstr = "common string";
	str.SetString(textstr.c_str(), static_cast<SizeType>(textstr.length()), allocator);

	d.AddMember("string1", str, allocator);

	//암묵적으로 변환됨
	d.AddMember("string2", "common string2", allocator);

	//하부 맴버 만들어서 넣기
	Value obj(kObjectType);
	Value val(kObjectType);

	obj.AddMember("id", 1, allocator);

	string description = "a description";
	val.SetString(description.c_str(), static_cast<SizeType>(description.length()), allocator);
	obj.AddMember("description", val, allocator);

	string help = "some help";
	val.SetString(help.c_str(), static_cast<SizeType>(help.length()), allocator);
	obj.AddMember("help", val, allocator);

	string workgroup = "a workgroup";
	val.SetString(workgroup.c_str(), static_cast<SizeType>(workgroup.length()), allocator);
	obj.AddMember("workgroup", val, allocator);

	val.SetBool(true);
	obj.AddMember("online", val, allocator);

	d.AddMember("test", obj, allocator);

	FILE* fp = fopen("output2.json", "wb");

	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	Writer<FileWriteStream> writer(os);
	d.Accept(writer);

	fclose(fp);
}
//파일로 부터 읽습니다. (상대좌표)
Document StreamFileReader(string dir)
{
	FILE* fp = fopen(dir.c_str(), "rb"); // non-Windows use "r"

	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);
	
	fclose(fp);
	return d;
}
//도큐먼트를 이어 붙이거나 도쿠먼트 하부에 이어 붙입니다.
void AddDoc()
{
	Document Doc = StreamFileReader("output.json");
	Document subDoc = StreamFileReader("output2.json");
	Document subDoc2 = StreamFileReader("output2.json");

	//하부에 이어 붙이는경우
	//도큐먼트변수["찾으려는 오브젝트"].getObject().AddMembet("오브젝트명", 밸류값, 도큐먼트 할당자)
	Doc["test"].GetObject().AddMember("subDoc", subDoc, Doc.GetAllocator());

	//도큐먼트끝에 이어 붙이는경우
	Doc.AddMember("AddDoc", subDoc2, Doc.GetAllocator());

	printDoc(&Doc);
}

//특정 벨류를 추가합니다.
void AddValue()
{
	Document Doc = StreamFileReader("output.json");
	Doc["test"].GetObject().AddMember("Add_value", "here is value", Doc.GetAllocator());
	printDoc(&Doc);

}

//오브젝트 전체를 삭제하거나 특정 밸류를 삭제합니다.
void DeleteValue()
{
	Document Doc = StreamFileReader("output.json");

	if (!Doc["test"].IsObject())
	{
		cout << "잘못된 키값을 입력했습니다.";
		return;
	}

	//키값에 있는 벨류 전부 삭제
	Doc.EraseMember("test");
	printDoc(&Doc);
	cout << endl << "========================================================" << endl;

	Document Doc2 = StreamFileReader("output.json");

	//특정 밸류만 삭제
	Doc2["test"].GetObject().EraseMember("online");
	printDoc(&Doc2);
}

//하위 문서를 프린트합니다
void printObject()
{
	Document Doc = StreamFileReader("output.json");

	rapidjson::StringBuffer strbuf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
	Value & sub = Doc["test"];
	sub.Accept(writer);
	puts(strbuf.GetString());

	//벨류를 프린트하는경우

	cout << endl << "========================================================" << endl;

	printf("string1 = %s\n", Doc["string1"].GetString());
}

void Substitution()
{
	Document Doc = StreamFileReader("output.json");
	Document Doc2 = StreamFileReader("output2.json");

	Doc.GetObject() = Doc2.GetObject();

	printDoc(&Doc);
}

int main()
{
	//StreamFileReaderExample();
	//StreamFileWriterExample();
	//AddDoc();
	//AddValue();
	//DeleteValue();
	//printObject();

	Substitution();
}
