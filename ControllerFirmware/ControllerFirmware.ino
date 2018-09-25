#include "Interpreter.h"

char* code = "int coolness = 0;\n\n  int computeCoolness(int a)\n  {\n    return a + joystick.left.x;\n  }\n\n  while(true)\n  {\n    coolness = computeCoolness(coolness);\n\n    if(coolness<=100)\n    {\n      coolness=coolness+1;\n    }\n    else if(coolness <= 200)\n    {\n      coolness = coolness + 2;\n    }\n	else\n    {\n      coolness = coolness + 3;\n    }\n  }\0";
//char* code = "import java.io.File;\nimport java.io.FileNotFoundException;\nimport java.util.ArrayList;\nimport java.util.HashMap;\nimport java.util.Iterator;\nimport java.util.Map;\nimport java.util.Scanner;\n import javax.swing.JFileChooser;\nimport javax.swing.UIManager;\n public class WordCount {\n public static void main(String[] args) {\ncountWordsViaGUI();\n}\n // allow user to pick file to exam via GUI.\n// allow multiple picks\npublic static void countWordsViaGUI() {\nsetLookAndFeel();\ntry {\nScanner key = new Scanner(System.in);\ndo {\nSystem.out.println(\"Opening GUI to choose file.\");\nScanner fileScanner = new Scanner(getFile());\nStopwatch st = new Stopwatch();\nst.start();\nArrayList<String> words = countWordsWithArrayList(fileScanner);\nst.stop();\nSystem.out.println(\"time to count: \" + st);\nSystem.out.print(\"Enter number of words to display: \");\nint numWordsToShow = Integer.parseInt(key.nextLine());\nshowWords(words, numWordsToShow);\nfileScanner.close();\nSystem.out.print(\"Perform another count? \");\n} while(key.nextLine().toLowerCase().charAt(0) == 'y');\nkey.close();\n}\ncatch(FileNotFoundException e) {\nSystem.out.println(\"Problem reading the data file. Exiting the program.\" + e);\n}\n}\n // determine distinct words in a file using an array list\nprivate static ArrayList<String> countWordsWithArrayList(Scanner fileScanner) {\n System.out.println(\"Total number of words: \" + numWords);\nSystem.out.println(\"number of distincy words: \" + result.size());\nreturn result;\n}\n // determine distinct words in a file and frequency of each word with a Map\nprivate static Map<String, Integer> countWordsWithMap(Scanner fileScanner) {\n System.out.println(\"Total number of words: \" + numWords);\nSystem.out.println(\"number of distincy words: \" + result.size());\nreturn result;\n}\n private static void showWords(ArrayList<String> words, int numWordsToShow) {\nfor(int i = 0; i < words.size() && i < numWordsToShow; i++)\nSystem.out.println(words.get(i));\n}\n private static void showWords(Map<String, Integer> words, int numWordsToShow) {\n }\n // perform a series of experiments on files. Determine average time to\n// count words in files of various sizes\nprivate static void performExp() {\nString[] smallerWorks = {\"smallWords.txt\", \"2BR02B.txt\", \"Alice.txt\", \"SherlockHolmes.txt\"};;\nString[] bigFile = {\"ciaFactBook2008.txt\"};\ntimingExpWithArrayList(smallerWorks, 50);\ntimingExpWithArrayList(bigFile, 3);\ntimingExpWithMap(smallerWorks, 50);\ntimingExpWithMap(bigFile, 3);\n}\n // pre: titles != null, elements of titles refer to files in the\n// same path as this program, numExp >= 0\n// read words from files and print average time to cound words.\nprivate static void timingExpWithMap(String[] titles, int numExp) {\ntry {\ndouble[] times = new double[titles.length];\nfinal int NUM_EXP = 50;\nfor(int i = 0; i < NUM_EXP; i++) {\nfor(int j = 0; j < titles.length; j++) {\nScanner fileScanner = new Scanner(new File(titles[j]));\nStopwatch st = new Stopwatch();\nst.start();\nMap<String, Integer> words = countWordsWithMap(fileScanner);\nst.stop();\nSystem.out.println(words.size());\ntimes[j] += st.time();\nfileScanner.close();\n}\n}\nfor(double a : times)\nSystem.out.println(a / NUM_EXP);\n}\ncatch(FileNotFoundException e) {\nSystem.out.println(\"Problem reading the data file. Exiting the program.\" + e);\n}\n}\n // pre: titles != null, elements of titles refer to files in the\n// same path as this program, numExp >= 0\n// read words from files and print average time to cound words.\nprivate static void timingExpWithArrayList(String[] titles, int numExp) {\ntry {\ndouble[] times = new double[titles.length];\nfor(int i = 0; i < numExp; i++) {\nfor(int j = 0; j < titles.length; j++) {\nScanner fileScanner = new Scanner(new File(titles[j]));\nStopwatch st = new Stopwatch();\nst.start();\nArrayList<String> words = countWordsWithArrayList(fileScanner);\nst.stop();\ntimes[j] += st.time();\nfileScanner.close();\n}\n}\nfor(int i = 0; i < titles.length; i++)\nSystem.out.println(\"Average time for \" + titles[i] + \": \" + (times[i] / numExp));\n}\ncatch(FileNotFoundException e) {\nSystem.out.println(\"Problem reading the data file. Exiting the program.\" + e);\n}\n}\n // try to set look and feel to same as system\nprivate static void setLookAndFeel() {\ntry {\nUIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());\n}\ncatch(Exception e) {\nSystem.out.println(\"Unable to set look at feel to local settings. \" +\n\"Continuing with default Java look and feel.\");\n}\n}\n /** Method to choose a file using a window.\n* @return the file chosen by the user. Returns null if no file picked.\n*/\nprivate static File getFile() {\n// create a GUI window to pick the text to evaluate\nJFileChooser chooser = new JFileChooser(\".\");\nchooser.setDialogTitle(\"Select File To Count Words:\");\nint retval = chooser.showOpenDialog(null);\nFile f =null;\nchooser.grabFocus();\nif (retval == JFileChooser.APPROVE_OPTION)\nf = chooser.getSelectedFile();\nreturn f;\n}\n}import java.io.File;\nimport java.io.FileNotFoundException;\nimport java.util.ArrayList;\nimport java.util.HashMap;\nimport java.util.Iterator;\nimport java.util.Map;\nimport java.util.Scanner;\n import javax.swing.JFileChooser;\nimport javax.swing.UIManager;\n public class WordCount {\n public static void main(String[] args) {\ncountWordsViaGUI();\n}\n // allow user to pick file to exam via GUI.\n// allow multiple picks\npublic static void countWordsViaGUI() {\nsetLookAndFeel();\ntry {\nScanner key = new Scanner(System.in);\ndo {\nSystem.out.println(\"Opening GUI to choose file.\");\nScanner fileScanner = new Scanner(getFile());\nStopwatch st = new Stopwatch();\nst.start();\nArrayList<String> words = countWordsWithArrayList(fileScanner);\nst.stop();\nSystem.out.println(\"time to count: \" + st);\nSystem.out.print(\"Enter number of words to display: \");\nint numWordsToShow = Integer.parseInt(key.nextLine());\nshowWords(words, numWordsToShow);\nfileScanner.close();\nSystem.out.print(\"Perform another count? \");\n} while(key.nextLine().toLowerCase().charAt(0) == 'y');\nkey.close();\n}\ncatch(FileNotFoundException e) {\nSystem.out.println(\"Problem reading the data file. Exiting the program.\" + e);\n}\n}\n // determine distinct words in a file using an array list\nprivate static ArrayList<String> countWordsWithArrayList(Scanner fileScanner) {\n System.out.println(\"Total number of words: \" + numWords);\nSystem.out.println(\"number of distincy words: \" + result.size());\nreturn result;\n}\n // determine distinct words in a file and frequency of each word with a Map\nprivate static Map<String, Integer> countWordsWithMap(Scanner fileScanner) {\n System.out.println(\"Total number of words: \" + numWords);\nSystem.out.println(\"number of distincy words: \" + result.size());\nreturn result;\n}\n private static void showWords(ArrayList<String> words, int numWordsToShow) {\nfor(int i = 0; i < words.size() && i < numWordsToShow; i++)\nSystem.out.println(words.get(i));\n}\n private static void showWords(Map<String, Integer> words, int numWordsToShow) {\n }\n // perform a series of experiments on files. Determine average time to\n// count words in files of various sizes\nprivate static void performExp() {\nString[] smallerWorks = {\"smallWords.txt\", \"2BR02B.txt\", \"Alice.txt\", \"SherlockHolmes.txt\"};;\nString[] bigFile = {\"ciaFactBook2008.txt\"};\ntimingExpWithArrayList(smallerWorks, 50);\ntimingExpWithArrayList(bigFile, 3);\ntimingExpWithMap(smallerWorks, 50);\ntimingExpWithMap(bigFile, 3);\n}\n // pre: titles != null, elements of titles refer to files in the\n// same path as this program, numExp >= 0\n// read words from files and print average time to cound words.\nprivate static void timingExpWithMap(String[] titles, int numExp) {\ntry {\ndouble[] times = new double[titles.length];\nfinal int NUM_EXP = 50;\nfor(int i = 0; i < NUM_EXP; i++) {\nfor(int j = 0; j < titles.length; j++) {\nScanner fileScanner = new Scanner(new File(titles[j]));\nStopwatch st = new Stopwatch();\nst.start();\nMap<String, Integer> words = countWordsWithMap(fileScanner);\nst.stop();\nSystem.out.println(words.size());\ntimes[j] += st.time();\nfileScanner.close();\n}\n}\nfor(double a : times)\nSystem.out.println(a / NUM_EXP);\n}\ncatch(FileNotFoundException e) {\nSystem.out.println(\"Problem reading the data file. Exiting the program.\" + e);\n}\n}\n // pre: titles != null, elements of titles refer to files in the\n// same path as this program, numExp >= 0\n// read words from files and print average time to cound words.\nprivate static void timingExpWithArrayList(String[] titles, int numExp) {\ntry {\ndouble[] times = new double[titles.length];\nfor(int i = 0; i < numExp; i++) {\nfor(int j = 0; j < titles.length; j++) {\nScanner fileScanner = new Scanner(new File(titles[j]));\nStopwatch st = new Stopwatch();\nst.start();\nArrayList<String> words = countWordsWithArrayList(fileScanner);\nst.stop();\ntimes[j] += st.time();\nfileScanner.close();\n}\n}\nfor(int i = 0; i < titles.length; i++)\nSystem.out.println(\"Average time for \" + titles[i] + \": \" + (times[i] / numExp));\n}\ncatch(FileNotFoundException e) {\nSystem.out.println(\"Problem reading the data file. Exiting the program.\" + e);\n}\n}\n // try to set look and feel to same as system\nprivate static void setLookAndFeel() {\ntry {\nUIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());\n}\ncatch(Exception e) {\nSystem.out.println(\"Unable to set look at feel to local settings. \" +\n\"Continuing with default Java look and feel.\");\n}\n}\n /** Method to choose a file using a window.\n* @return the file chosen by the user. Returns null if no file picked.\n*/\nprivate static File getFile() {\n// create a GUI window to pick the text to evaluate\nJFileChooser chooser = new JFileChooser(\".\");\nchooser.setDialogTitle(\"Select File To Count Words:\");\nint retval = chooser.showOpenDialog(null);\nFile f =null;\nchooser.grabFocus();\nif (retval == JFileChooser.APPROVE_OPTION)\nf = chooser.getSelectedFile();\nreturn f;\n}\n}";

Interpreter interpreter;

void gg(void(*a)(int))
{
	(*a)(8);
}

void(*g)(int) = [](int a)
{
	
};

void setup()
{	
	Serial.begin(115200);
	long startTime = millis();

	interpreter.compiler.addRuleEmpty(" \n\t\r");
	interpreter.compiler.addRule("{}()[];", 1, true);
	interpreter.compiler.addRule("azAZ09__", -1);
	interpreter.compiler.addRule("", -1);
	interpreter.compiler.addRuleComment("//", "\n");
	interpreter.compiler.addRuleComment("/*", "*/");
	interpreter.compiler.addRuleString("\"", "\"", "\\\"", -1);
	interpreter.compiler.addRuleString("'", "'", "\\'", -1);

	/*char* q = {
		'a','z',38,1,
		'A','Z',38,1,
		'_','_',38,1,
		'0','9',43,0,
		'\0','\0',0,0,
		'a','z',38,0,//38
		'A','Z',38,0,
		'_','_',38,0,
		'0','9',38,0,
		'\0','\0',0,2,
		'0','9',43,0,//43
		'\0','\0',0,2,};*/
	
	Serial.print("time: ");
	Serial.println(millis() - startTime);
	
	//interpreter.compiler.addCompileFunction();

	/*
	 * need to know:
	 * -block start/end; blockStartString, blockEndString, bool singleCharacterOrCharacterAmount (ex: tabs), int increament amount, bool haveBlockStart (cannot create just block, other then that function, classes... start the block)
	 * -object structure, ex: "object", name, "{", "}";
	 * -function structure, ex: objectType, name, "(", parameters, ")", "{", "}";
	 * 
	 * look for these structure in every new line; on find execute function(it can be use added)
	 * 
	 * -object: "class", name, block
	 * -function: objectType, name, "(", parameters, ")" block
	 * -instance: objectType, name
	 * 
	 * -: "if", "(", statment, ")", block = add clear workingStack; eval(statment); add ifnot true skip block; add block;
	 * -: "else", "if", "(", statment, ")", block
	 * -: "else" "(", statment, ")", block
	 */
	
	gg([](int a)
	{
		
	});

	/*addCompileFunction([]()
	{
		int a = 0;
	}{Compiler_Type, Compiler_Ignore, Compiler_String}, 0, "");


	{Compiler_String}, "object");
	{Compiler_String}, "if");
	{Compiler_String, Compiler_String}, "else", "if");
	{Compiler_String}, "else");
	{Compiler_Type, Compiler_Ignore, Compiler_String}, 0, "(");
	{Compiler_Type}, 0);*/
}

void loop()
{
}
