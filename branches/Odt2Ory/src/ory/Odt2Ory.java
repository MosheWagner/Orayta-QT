package ory;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.InputStreamReader;
import java.net.ConnectException;
import java.util.ArrayList;

import ory.fileTypes.*;

import com.artofsolving.jodconverter.DocumentConverter;
import com.artofsolving.jodconverter.openoffice.connection.OpenOfficeConnection;
import com.artofsolving.jodconverter.openoffice.connection.SocketOpenOfficeConnection;
import com.artofsolving.jodconverter.openoffice.converter.OpenOfficeDocumentConverter;

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



@SuppressWarnings("unused")
public class Odt2Ory {
	private Filename inputFilename; 
//	private String fileType;
	private final ArrayList<String> SUPPORTED_TYPES = new ArrayList<String>();
	

	Odt2Ory()    {
		SUPPORTED_TYPES.add("doc");
		SUPPORTED_TYPES.add("docx");
		SUPPORTED_TYPES.add("html");
		SUPPORTED_TYPES.add("sxw");
		SUPPORTED_TYPES.add("rtf");
		SUPPORTED_TYPES.add("wpd");
//		SUPPORTED_TYPES.add("txt");
//		SUPPORTED_TYPES.add(""); //more supported types?
//		SUPPORTED_TYPES.add("");
		
		process (Main.ui.getInputFile());
	}
	
	/**
	 * Decides what to do with the input file / directory.
	 * @param input - a file or path.
	 * @throws Exception
	 */
	private void process(String input) {


		if (input == null) {
			Main.ui.message("no file selected");
			return;
		}

		inputFilename = new Filename(input); 
		
		//recursively process files.
		if (inputFilename.isDirectory()){
			final File[] childs = inputFilename.listFiles();
			for (File child: childs){
				process(child.getAbsolutePath());
			}
			return;
		}
		
		if (! inputFilename.canRead()) {
			Main.ui.errorMessage("cant read file:\n" + inputFilename + "\nהקובץ לא נמצא" );
			return;
		}
		
		Main.ui.log("processing file: " + input);
		
		FileType file = selectType(inputFilename);
		
		
		try {
			// use the extractor to generate the needed files.
			String[] fileNames = ((FileType) file).convertToOrayta();
			if (fileNames != null)
				success(fileNames);

		} catch (FileNotFoundException e) {
			
			Main.ui.errorMessage("File not found" + "\n" + "הקובץ לא נמצא" + "\n", e);
			return;

		} catch (Exception e) {
			Main.ui.errorMessage("an error occured" + "\n" + "אירעה שגיאה" + "\n", e);
			return;
		}

		

	}

	private FileType selectType(Filename input) {

		String fileType = inputFilename.getExtension();
		//if this files is a known filetype but not odt, convert it to odt and then continue.
		
		FileType file;
		
		if(fileType.equalsIgnoreCase("odt"))
			file = new OdtFile(input);
		else if (fileType.equalsIgnoreCase("doc"))
			file = new DocFile(input);
		else if (fileType.equalsIgnoreCase("docx"))
			file = new DocxFile(input);
		else if (fileType.equalsIgnoreCase("html") || fileType.equalsIgnoreCase("htm"))
			file = new HtmlFile(input);
		else if (fileType.equalsIgnoreCase("rtf"))
			file = new RtfFile(input);
		else if (fileType.equalsIgnoreCase("swx"))
			file = new SwxFile(input);
		else if (fileType.equalsIgnoreCase("wpd"))
			file = new WpdFile(input);
		else
			file = new UnknownFile(input);
		
		return file;
	}
	

	
	

	private void success(String[] fileNames) {
		
		String files = "";
		for (String name : fileNames){
			files += "-\t" + name + "\n";
		}
		
		Main.ui.message("opporation completed successfully", "הפעולה הסתיימה בהצלחה",
				"input: " + inputFilename.getPath(), "output:\n" + files);
	
	}

//	void message (String str){
//		Utils.message(str);
//	}
//	
//	void message (String ... strings){
//		Utils.message(strings);
//	}
//
//	void errorMessage (String str, Exception e){
//		Utils.errorMessage(str, e);
//	}
//	
//	void errorMessage (String str) {
//		Utils.errorMessage(str);
//	}
//	
//	void errorMessage (String ... strings ) {
//		Utils.errorMessage(strings);		
//	}
//
//	
//	public static void log(String string) {
//		Utils.log(string);
//	}
//	
//	public static void dbgLog(String string) {
//		Utils.dbgLog(string);
//	}

}