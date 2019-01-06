#include"SimpleXmlParserTest.h"
#include"SimpleXMLParser.h"
#include"Node.h"
#include"Utility.h"
using namespace xml;
using namespace std;
TEST_F(SimpleXmlParserTest, ParseHrefTest) {
	wstring content = L"<a href=\"www.google.com\">Test</a>";
	unique_ptr<xml::SimpleXMLParser> xmlParser = make_unique<xml::SimpleXMLParser>();
	//cout << "before parsing" << endl;
	xmlParser->parse(content);
	//cout << "after parsing" << endl;
	auto _root = xmlParser->getRoot();
	EXPECT_EQ(1, _root->getChildrenCount());
	auto node = _root->getChild(0);
	EXPECT_EQ(L"Test", node->getValue());
	EXPECT_EQ(0, node->getChildrenCount());
	EXPECT_EQ(L"a", node->getName());
	EXPECT_EQ(L"www.google.com", node->getAttribute(L"href"));
}

TEST_F(SimpleXmlParserTest, ParseCommnetTest) {
	wstring content = L"<!--this is a comment-->";
	unique_ptr<xml::SimpleXMLParser> xmlParser = make_unique<xml::SimpleXMLParser>();
	//cout << "before parsing" << endl;
	xmlParser->parse(content);
	//cout << "after parsing" << endl;
	auto _root = xmlParser->getRoot();
	EXPECT_EQ(0, _root->getChildrenCount());
}
TEST_F(SimpleXmlParserTest, ParseCommnetTest1) {
	wstring content = L"<a href=<!--some nonsense here-->\"www.google.com\">Test</a>";
	unique_ptr<xml::SimpleXMLParser> xmlParser = make_unique<xml::SimpleXMLParser>();
	//cout << "before parsing" << endl;
	xmlParser->parse(content);
	//cout << "after parsing" << endl;
	auto _root = xmlParser->getRoot();
	EXPECT_EQ(1, _root->getChildrenCount());
	auto node = _root->getChild(0);
	EXPECT_EQ(L"Test", node->getValue());
	EXPECT_EQ(0, node->getChildrenCount());
	EXPECT_EQ(L"a", node->getName());
	EXPECT_EQ(L"www.google.com", node->getAttribute(L"href"));
}

TEST_F(SimpleXmlParserTest, ParseCommnetTest2) {
	wstring content = L"<a href<!--some nonsense here-->=<!--some nonsense here-->\"www.google.com\">Test</a>";
	unique_ptr<xml::SimpleXMLParser> xmlParser = make_unique<xml::SimpleXMLParser>();
	//cout << "before parsing" << endl;
	xmlParser->parse(content);
	//cout << "after parsing" << endl;
	auto _root = xmlParser->getRoot();
	EXPECT_EQ(1, _root->getChildrenCount());
	auto node = _root->getChild(0);
	EXPECT_EQ(L"Test", node->getValue());
	EXPECT_EQ(0, node->getChildrenCount());
	EXPECT_EQ(L"a", node->getName());
	EXPECT_EQ(L"www.google.com", node->getAttribute(L"href"));
}

TEST_F(SimpleXmlParserTest, ParseCommnetTest3) {
	wstring content = L"<a <!--some nonsense here-->href=<!--some nonsense here-->\"www.google.com\">Test</a>";
	unique_ptr<xml::SimpleXMLParser> xmlParser = make_unique<xml::SimpleXMLParser>();
	//cout << "before parsing" << endl;
	xmlParser->parse(content);
	//cout << "after parsing" << endl;
	auto _root = xmlParser->getRoot();
	EXPECT_EQ(1, _root->getChildrenCount());
	auto node = _root->getChild(0);
	EXPECT_EQ(L"Test", node->getValue());
	EXPECT_EQ(0, node->getChildrenCount());
	EXPECT_EQ(L"a", node->getName());
	EXPECT_EQ(L"www.google.com", node->getAttribute(L"href"));
}

TEST_F(SimpleXmlParserTest, ParseCommnetTest4) {
	wstring content = L"<a <!--some nonsense here-->href=<!--some nonsense here-->\"www.google.com\"><!--Test--></a>";
	unique_ptr<xml::SimpleXMLParser> xmlParser = make_unique<xml::SimpleXMLParser>();
	//cout << "before parsing" << endl;
	xmlParser->parse(content);
	//cout << "after parsing" << endl;
	auto _root = xmlParser->getRoot();
	EXPECT_EQ(1, _root->getChildrenCount());
	auto node = _root->getChild(0);
	EXPECT_EQ(L"", node->getValue());
	EXPECT_EQ(0, node->getChildrenCount());
	EXPECT_EQ(L"a", node->getName());
	EXPECT_EQ(L"www.google.com", node->getAttribute(L"href"));
}

TEST_F(SimpleXmlParserTest, ParseCommnetTest5) {
	wstring content = L"<a <!--some nonsense here-->href=<!--some nonsense here-->\"www.google.com\"><!--Test-->Test<!--Test--></a>";
	unique_ptr<xml::SimpleXMLParser> xmlParser = make_unique<xml::SimpleXMLParser>();
	//cout << "before parsing" << endl;
	xmlParser->parse(content);
	//cout << "after parsing" << endl;
	auto _root = xmlParser->getRoot();
	EXPECT_EQ(1, _root->getChildrenCount());
	auto node = _root->getChild(0);
	EXPECT_EQ(L"Test", node->getValue());
	EXPECT_EQ(0, node->getChildrenCount());
	EXPECT_EQ(L"a", node->getName());
	EXPECT_EQ(L"www.google.com", node->getAttribute(L"href"));
}

TEST_F(SimpleXmlParserTest, ParseCommnetTest6) {
	wstring content = L"<a <!--some nonsense here-->href=<!--some nonsense here-->\"www.google.com\"><!--Test-->Test<!--Test-->Hello</a>";
	unique_ptr<xml::SimpleXMLParser> xmlParser = make_unique<xml::SimpleXMLParser>();
	//cout << "before parsing" << endl;
	xmlParser->parse(content);
	//cout << "after parsing" << endl;
	auto _root = xmlParser->getRoot();
	EXPECT_EQ(1, _root->getChildrenCount());
	auto node = _root->getChild(0);
	EXPECT_EQ(L"TestHello", node->getValue());
	EXPECT_EQ(0, node->getChildrenCount());
	EXPECT_EQ(L"a", node->getName());
	EXPECT_EQ(L"www.google.com", node->getAttribute(L"href"));
}

TEST_F(SimpleXmlParserTest, ParseCommnetTest7) {
	wstring content = L"<a <!--some nonsense here-->href=<!--some nonsense here-->\"www.google.com\"><!--Test-->Test <!--Test-->Hello</a>";
	unique_ptr<xml::SimpleXMLParser> xmlParser = make_unique<xml::SimpleXMLParser>();
	//cout << "before parsing" << endl;
	xmlParser->parse(content);
	//cout << "after parsing" << endl;
	auto _root = xmlParser->getRoot();
	EXPECT_EQ(1, _root->getChildrenCount());
	auto node = _root->getChild(0);
	EXPECT_EQ(L"Test Hello", node->getValue());
	EXPECT_EQ(0, node->getChildrenCount());
	EXPECT_EQ(L"a", node->getName());
	EXPECT_EQ(L"www.google.com", node->getAttribute(L"href"));
}

TEST_F(SimpleXmlParserTest, ParseScript) {
	using namespace Utility;
	wstring content = L"<Script>{print(123+456);}</Script>";
	unique_ptr<xml::SimpleXMLParser> xmlParser = make_unique<xml::SimpleXMLParser>();
	//cout << "before parsing" << endl;
	xmlParser->parse(content);
	//cout << "after parsing" << endl;
	auto _root = xmlParser->getRoot();
	auto scriptNodes = _root->getNodesByName(L"Script");
	EXPECT_EQ(1, scriptNodes.size());
	std::wstring scriptContent = scriptNodes[0]->getValue(); 
	EXPECT_EQ(L"{print(123+456);}", scriptContent);
}

//TEST_F(SimpleXmlParserTest, parseScriptLessThan) {
//	using namespace Utility;
//	wstring content = L"<Script>{print(123<456);}</Script>";
//	unique_ptr<xml::SimpleXMLParser> xmlParser = make_unique<xml::SimpleXMLParser>();
//	//cout << "before parsing" << endl;
//	xmlParser->parse(content);
//	//cout << "after parsing" << endl;
//	auto _root = xmlParser->getRoot();
//	auto scriptNodes = _root->getNodesByName(L"Script");
//	EXPECT_EQ(1, scriptNodes.size());
//	std::wstring scriptContent = scriptNodes[0]->getValue();
//	EXPECT_EQ(L"{print(123<456);}", scriptContent);
//}