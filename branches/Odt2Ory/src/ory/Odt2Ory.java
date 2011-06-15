package ory;

import java.io.FileNotFoundException;

/*
 * Odt2Ory01.java
 *
 * Created on September 12, 2007, 4:24 PM
 *
 * this program is meant to convert text from an .odt file
 * to a text format that is used by Oryata.
 * 
 *  it is based on a program i found here:
 *  http://blogs.oracle.com/prasanna/entry/openoffice_parser_extracting_text_from
 *  
 *  IMPORTENT: the program currently handles footnotes vary badly, so dont use it on
 *  files with footnotes.
 *  also it will work only with full linux paths. 
 * 
 */

/* TODO :
 * write output to file. DONE.
 * compile to a single jar. DONE.
 * replace Filename so the program can work with windows. DONE.
 * create a gui. DONE.
 * get license. DONE.
 * loop option for many files.
 * create conf files. DONE.
 * Footnote class. DONE.
 * run tests.
 * upload. DONE.
 * print usage.
 */


/**
 * this program is a helper for 'orayta' project.
 * it is designed to except an open office file (.odt) 
 * and transform to the format orayta understands.
 * @author izar00@gmail.com
 */



public class Odt2Ory {

	Odt2Ory()    {
	}
	
	/**
	 * this is where all the work is done.
	 * @param inputFilename - an odt file path.
	 * @throws Exception
	 */
	public void process(String inputFilename)  throws Exception {


		if (inputFilename == null) {
			message("no file selected");
			return;
		}

		launchInfo(inputFilename);

		try {
			OryFile oryFile = new OryFile(inputFilename);

			Filename oryFilename = new Filename(oryFile.saveToOryDir());


			//    	System.out.println("Extracted Text:");
			//    	System.out.println(oryFile.getFileText());


			OryConfFile oryConf = new OryConfFile (oryFilename);
			oryConf.setBookName(oryFile.getBookTitle());

			oryConf.save();

		} catch (FileNotFoundException e) {
			
			errorMessage("File not found" + "\n" + "הקובץ לא נמצא" + "\n", e);
			return;

		} catch (Exception e) {
			errorMessage("an error occured" + "\n" + "אירעה שגיאה" + "\n", e);
			return;
		}

		success();

	}

	void message (String str){
		System.out.println(str);
	}

	void errorMessage (String str, Exception e){
		System.err.println(str);
		System.err.println(e);
	}

	void success() {
		message("oporation completed successfully" + "\n" + "הפעולה הסתיימה בהצלחה" + "\n");

	}

	void launchInfo(String inputFilename){
		message("runing odt2ory ver 0.2");
		message("processing file: " + inputFilename );
	}

}