#include "pch.h"
#include <iostream>
#include <ctime>
#include "Interpreter.h"
#include "Compiler.h";
#include <stdarg.h>

const char* code = "int coolness = 0;\n\n  int computeCoolness(int a)\n  {\n    return a + joystick.left.x;\n  }\n\n  while(true)\n  {\n    coolness = computeCoolness(coolness);\n\n    if(coolness<=100)\n    {\n      coolness=coolness+1;\n    }\n    else if(coolness <= 200)\n    {\n      coolness = coolness + 2;\n    }\n	else\n    {\n      coolness = coolness + 3;\n    }\n  }\0";
//const char* code = "import java.io.File;\nimport java.io.FileNotFoundException;\nimport java.util.ArrayList;\nimport java.util.HashMap;\nimport java.util.Iterator;\nimport java.util.Map;\nimport java.util.Scanner;\n import javax.swing.JFileChooser;\nimport javax.swing.UIManager;\n public class WordCount {\n public static void main(String[] args) {\ncountWordsViaGUI();\n}\n // allow user to pick file to exam via GUI.\n// allow multiple picks\npublic static void countWordsViaGUI() {\nsetLookAndFeel();\ntry {\nScanner key = new Scanner(System.in);\ndo {\nSystem.out.println(\"Opening GUI to choose file.\");\nScanner fileScanner = new Scanner(getFile());\nStopwatch st = new Stopwatch();\nst.start();\nArrayList<String> words = countWordsWithArrayList(fileScanner);\nst.stop();\nSystem.out.println(\"time to count: \" + st);\nSystem.out.print(\"Enter number of words to display: \");\nint numWordsToShow = Integer.parseInt(key.nextLine());\nshowWords(words, numWordsToShow);\nfileScanner.close();\nSystem.out.print(\"Perform another count? \");\n} while(key.nextLine().toLowerCase().charAt(0) == 'y');\nkey.close();\n}\ncatch(FileNotFoundException e) {\nSystem.out.println(\"Problem reading the data file. Exiting the program.\" + e);\n}\n}\n // determine distinct words in a file using an array list\nprivate static ArrayList<String> countWordsWithArrayList(Scanner fileScanner) {\n System.out.println(\"Total number of words: \" + numWords);\nSystem.out.println(\"number of distincy words: \" + result.size());\nreturn result;\n}\n // determine distinct words in a file and frequency of each word with a Map\nprivate static Map<String, Integer> countWordsWithMap(Scanner fileScanner) {\n System.out.println(\"Total number of words: \" + numWords);\nSystem.out.println(\"number of distincy words: \" + result.size());\nreturn result;\n}\n private static void showWords(ArrayList<String> words, int numWordsToShow) {\nfor(int i = 0; i < words.size() && i < numWordsToShow; i++)\nSystem.out.println(words.get(i));\n}\n private static void showWords(Map<String, Integer> words, int numWordsToShow) {\n }\n // perform a series of experiments on files. Determine average time to\n// count words in files of various sizes\nprivate static void performExp() {\nString[] smallerWorks = {\"smallWords.txt\", \"2BR02B.txt\", \"Alice.txt\", \"SherlockHolmes.txt\"};;\nString[] bigFile = {\"ciaFactBook2008.txt\"};\ntimingExpWithArrayList(smallerWorks, 50);\ntimingExpWithArrayList(bigFile, 3);\ntimingExpWithMap(smallerWorks, 50);\ntimingExpWithMap(bigFile, 3);\n}\n // pre: titles != null, elements of titles refer to files in the\n// same path as this program, numExp >= 0\n// read words from files and print average time to cound words.\nprivate static void timingExpWithMap(String[] titles, int numExp) {\ntry {\ndouble[] times = new double[titles.length];\nfinal int NUM_EXP = 50;\nfor(int i = 0; i < NUM_EXP; i++) {\nfor(int j = 0; j < titles.length; j++) {\nScanner fileScanner = new Scanner(new File(titles[j]));\nStopwatch st = new Stopwatch();\nst.start();\nMap<String, Integer> words = countWordsWithMap(fileScanner);\nst.stop();\nSystem.out.println(words.size());\ntimes[j] += st.time();\nfileScanner.close();\n}\n}\nfor(double a : times)\nSystem.out.println(a / NUM_EXP);\n}\ncatch(FileNotFoundException e) {\nSystem.out.println(\"Problem reading the data file. Exiting the program.\" + e);\n}\n}\n // pre: titles != null, elements of titles refer to files in the\n// same path as this program, numExp >= 0\n// read words from files and print average time to cound words.\nprivate static void timingExpWithArrayList(String[] titles, int numExp) {\ntry {\ndouble[] times = new double[titles.length];\nfor(int i = 0; i < numExp; i++) {\nfor(int j = 0; j < titles.length; j++) {\nScanner fileScanner = new Scanner(new File(titles[j]));\nStopwatch st = new Stopwatch();\nst.start();\nArrayList<String> words = countWordsWithArrayList(fileScanner);\nst.stop();\ntimes[j] += st.time();\nfileScanner.close();\n}\n}\nfor(int i = 0; i < titles.length; i++)\nSystem.out.println(\"Average time for \" + titles[i] + \": \" + (times[i] / numExp));\n}\ncatch(FileNotFoundException e) {\nSystem.out.println(\"Problem reading the data file. Exiting the program.\" + e);\n}\n}\n // try to set look and feel to same as system\nprivate static void setLookAndFeel() {\ntry {\nUIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());\n}\ncatch(Exception e) {\nSystem.out.println(\"Unable to set look at feel to local settings. \" +\n\"Continuing with default Java look and feel.\");\n}\n}\n /** Method to choose a file using a window.\n* @return the file chosen by the user. Returns null if no file picked.\n*/\nprivate static File getFile() {\n// create a GUI window to pick the text to evaluate\nJFileChooser chooser = new JFileChooser(\".\");\nchooser.setDialogTitle(\"Select File To Count Words:\");\nint retval = chooser.showOpenDialog(null);\nFile f =null;\nchooser.grabFocus();\nif (retval == JFileChooser.APPROVE_OPTION)\nf = chooser.getSelectedFile();\nreturn f;\n}\n}import java.io.File;\nimport java.io.FileNotFoundException;\nimport java.util.ArrayList;\nimport java.util.HashMap;\nimport java.util.Iterator;\nimport java.util.Map;\nimport java.util.Scanner;\n import javax.swing.JFileChooser;\nimport javax.swing.UIManager;\n public class WordCount {\n public static void main(String[] args) {\ncountWordsViaGUI();\n}\n // allow user to pick file to exam via GUI.\n// allow multiple picks\npublic static void countWordsViaGUI() {\nsetLookAndFeel();\ntry {\nScanner key = new Scanner(System.in);\ndo {\nSystem.out.println(\"Opening GUI to choose file.\");\nScanner fileScanner = new Scanner(getFile());\nStopwatch st = new Stopwatch();\nst.start();\nArrayList<String> words = countWordsWithArrayList(fileScanner);\nst.stop();\nSystem.out.println(\"time to count: \" + st);\nSystem.out.print(\"Enter number of words to display: \");\nint numWordsToShow = Integer.parseInt(key.nextLine());\nshowWords(words, numWordsToShow);\nfileScanner.close();\nSystem.out.print(\"Perform another count? \");\n} while(key.nextLine().toLowerCase().charAt(0) == 'y');\nkey.close();\n}\ncatch(FileNotFoundException e) {\nSystem.out.println(\"Problem reading the data file. Exiting the program.\" + e);\n}\n}\n // determine distinct words in a file using an array list\nprivate static ArrayList<String> countWordsWithArrayList(Scanner fileScanner) {\n System.out.println(\"Total number of words: \" + numWords);\nSystem.out.println(\"number of distincy words: \" + result.size());\nreturn result;\n}\n // determine distinct words in a file and frequency of each word with a Map\nprivate static Map<String, Integer> countWordsWithMap(Scanner fileScanner) {\n System.out.println(\"Total number of words: \" + numWords);\nSystem.out.println(\"number of distincy words: \" + result.size());\nreturn result;\n}\n private static void showWords(ArrayList<String> words, int numWordsToShow) {\nfor(int i = 0; i < words.size() && i < numWordsToShow; i++)\nSystem.out.println(words.get(i));\n}\n private static void showWords(Map<String, Integer> words, int numWordsToShow) {\n }\n // perform a series of experiments on files. Determine average time to\n// count words in files of various sizes\nprivate static void performExp() {\nString[] smallerWorks = {\"smallWords.txt\", \"2BR02B.txt\", \"Alice.txt\", \"SherlockHolmes.txt\"};;\nString[] bigFile = {\"ciaFactBook2008.txt\"};\ntimingExpWithArrayList(smallerWorks, 50);\ntimingExpWithArrayList(bigFile, 3);\ntimingExpWithMap(smallerWorks, 50);\ntimingExpWithMap(bigFile, 3);\n}\n // pre: titles != null, elements of titles refer to files in the\n// same path as this program, numExp >= 0\n// read words from files and print average time to cound words.\nprivate static void timingExpWithMap(String[] titles, int numExp) {\ntry {\ndouble[] times = new double[titles.length];\nfinal int NUM_EXP = 50;\nfor(int i = 0; i < NUM_EXP; i++) {\nfor(int j = 0; j < titles.length; j++) {\nScanner fileScanner = new Scanner(new File(titles[j]));\nStopwatch st = new Stopwatch();\nst.start();\nMap<String, Integer> words = countWordsWithMap(fileScanner);\nst.stop();\nSystem.out.println(words.size());\ntimes[j] += st.time();\nfileScanner.close();\n}\n}\nfor(double a : times)\nSystem.out.println(a / NUM_EXP);\n}\ncatch(FileNotFoundException e) {\nSystem.out.println(\"Problem reading the data file. Exiting the program.\" + e);\n}\n}\n // pre: titles != null, elements of titles refer to files in the\n// same path as this program, numExp >= 0\n// read words from files and print average time to cound words.\nprivate static void timingExpWithArrayList(String[] titles, int numExp) {\ntry {\ndouble[] times = new double[titles.length];\nfor(int i = 0; i < numExp; i++) {\nfor(int j = 0; j < titles.length; j++) {\nScanner fileScanner = new Scanner(new File(titles[j]));\nStopwatch st = new Stopwatch();\nst.start();\nArrayList<String> words = countWordsWithArrayList(fileScanner);\nst.stop();\ntimes[j] += st.time();\nfileScanner.close();\n}\n}\nfor(int i = 0; i < titles.length; i++)\nSystem.out.println(\"Average time for \" + titles[i] + \": \" + (times[i] / numExp));\n}\ncatch(FileNotFoundException e) {\nSystem.out.println(\"Problem reading the data file. Exiting the program.\" + e);\n}\n}\n // try to set look and feel to same as system\nprivate static void setLookAndFeel() {\ntry {\nUIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());\n}\ncatch(Exception e) {\nSystem.out.println(\"Unable to set look at feel to local settings. \" +\n\"Continuing with default Java look and feel.\");\n}\n}\n /** Method to choose a file using a window.\n* @return the file chosen by the user. Returns null if no file picked.\n*/\nprivate static File getFile() {\n// create a GUI window to pick the text to evaluate\nJFileChooser chooser = new JFileChooser(\".\");\nchooser.setDialogTitle(\"Select File To Count Words:\");\nint retval = chooser.showOpenDialog(null);\nFile f =null;\nchooser.grabFocus();\nif (retval == JFileChooser.APPROVE_OPTION)\nf = chooser.getSelectedFile();\nreturn f;\n}\n}";

Compiler compiler;
Interpreter interpreter;

void gg(std::initializer_list<char>)
{
}

void gg2(char a, ...)
{
	va_list vl;
	va_start(vl, a);

	std::cout << vl[0] << std::endl;

	va_end(vl);
}

int main()
{
	clock_t begin = clock();

	/*
	lookFor (if not found exit)
	setRange (set found to false)
	range (if found set found to true)
	checkRangeExit (if found is false exit)
	*/

	/*
	0:lookFor or require
	1:range or singleCharacter
	2:
	3:
	4:
	5:
	6:
	7:
	*/

	/*const char* rules[] = {
		"azAZ__\0\2azAZ__09\1",
		"/\0*\0\2*\1/\1\0",
		0/ 0* 2 1* 1/
	};*/

	//lookFor azAZ__ then loopWhile azAZ09__ goBackOne addToken
	//lookFor / fallowedBy * then loopUntil * fallowedBy /

	//rangeOr, then (previous was true)

	enum compilerCodes
	{
		then,
		loopWhile,
		loopUntil,
		loopEnd,
		goBack,
		addToken,
		discardToken,
		endOfRule,
		range,
		single,
		notRange,
		notSingle
	};

	//gg2(0, "a", 'z', 'h', 'i', "az", "AZ", '_', then, loopWhile, "az", "AZ", '_', "09", endLoop, goBack, 1, addToken);

	//0: then
	//1: loopWhile
	//2: loopUntil
	//3: loopEnd
	//4: goBack
	//5: addToken
	//6: discardToken
	//7: endOfRule
	//8: range
	//9: single
	//10: !range
	//11: !single

	//compiler.rule.push_back((char*)"\x08az\x08AZ\x09_\x00\x01\x08az\x08AZ\x0809\x09_\x03\x041\x05\x07");
	compiler.addRule(range, 'a', 'z', range, 'A', 'Z', single, '_', then, loopWhile, range, 'a', 'z', range, 'A', 'Z', range, '0', '9', single, '_', loopEnd, goBack, 1, addToken, endOfRule);

	//gg2('a', goBack, "43trfgd");

	//

	/*interpreter.compiler.addRuleEmpty((char*)" \n\t\r");
	interpreter.compiler.addRule((char*)"{}()[];", 1, true);
	interpreter.compiler.addRule((char*)"azAZ09__", -1);
	interpreter.compiler.addRule((char*)"", -1);
	interpreter.compiler.addRuleComment((char*)"//", (char*)"\n");
	*///interpreter.compiler.addRuleComment((char*)"/*", (char*)"*/");
	//interpreter.compiler.addRuleString((char*)"\"", (char*)"\"", (char*)"\\\"", -1);
	//interpreter.compiler.addRuleString((char*)"'", (char*)"'", (char*)"\\'", -1);
	//interpreter.compiler.parseCode2((char*)code);

	/*compiler.addStandardRule({ "azAZ__", "azAZ__09" });
	compiler.addStandardRule({ "09" });
	compiler.addStandardRule({ "{}[]()..;;" }, 1);
	compiler.addStandardRule({ "!/::<@\\\\^`||~~" });
	*///compiler.addWrapRule("/*", "*/", true);
	//compiler.addWrapRule("//", "\n", true);
	//compiler.addWrapRule("\"", "\"", false, { "\\\"" });
	//compiler.addWrapRule("'", "'", false, { "\\'" });

	double elapsed_secs = double(clock() - begin) / CLOCKS_PER_SEC;
	std::cout << "Time to set rules: " << elapsed_secs * 1000.0 << "m" << std::endl;
	begin = clock();

	//compiler.parseCode1(code);
	compiler.parseCode(code);

	elapsed_secs = double(clock() - begin) / CLOCKS_PER_SEC;
	std::cout << "Time to parse: " << elapsed_secs * 1000.0 << "m" << std::endl;

	std::cout << "Parsed code: " << std::endl;
	for (unsigned int i = 0; i < compiler.getSize(); i++)
	{
		char* a = compiler.getToken(i, (char*)code);
		std::cout << a << std::endl;
		free(a);
	}
}
