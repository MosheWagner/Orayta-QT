package ory;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.InputStreamReader;
import java.net.ConnectException;
import java.util.ArrayList;

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
		
	}
	
	/**
	 * this is where all the work is done.
	 * @param input - an odt file path.
	 * @throws Exception
	 */
	public void process(String input)  throws Exception {


		if (input == null) {
			message("no file selected");
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
		
//		launchInfo(input);
		log("processing file: " + input);
		
		
		
		
		if (! inputFilename.canRead()) {
			errorMessage("cant read file:\n" + inputFilename + "\nהקובץ לא נמצא" );
			return;
		}
		
		String fileType = inputFilename.getExtension();
		if (SUPPORTED_TYPES.contains(fileType)){
			Filename odtFile = doc2odt(inputFilename);
			process(odtFile.getFilename());
			return;
		}
		else if (! fileType.equals("odt")){
			errorMessage("file type: " + fileType + " not supported\n" +
					"סוג הקובץ: " + fileType + " לא נתמך.");
			return;
		}
		
		Filename oryFilename;
		try {
			OryFile oryFile = new OryFile(inputFilename);
			oryFilename = oryFile.save();

//			 new Filename(oryFile.getFilename());


			//    	System.out.println("Extracted Text:");
			//    	System.out.println(oryFile.getFileText());


//			OryConfFile oryConf = new OryConfFile (oryFilename);
//			oryConf.setBookName(oryFile.getBookTitle());
//			
//			if (Main.parameters.getUid().isEmpty()){
//				oryConf.setAutoUid();
//			}
//			else {
//				oryConf.setUID(Main.parameters.getUid());
//			}
//			
//			if (Main.parameters.getSource() != null)
//				oryConf.setBookSource(Main.parameters.getSource());
//			
//			if (Main.parameters.getBookTitle() != null)
//				oryConf.setBookName(Main.parameters.getBookTitle());
//
//			oryConf.save();
//
		} catch (FileNotFoundException e) {
			
			errorMessage("File not found" + "\n" + "הקובץ לא נמצא" + "\n", e);
			return;

		} catch (Exception e) {
			errorMessage("an error occured" + "\n" + "אירעה שגיאה" + "\n", e);
			return;
		}

		success(oryFilename);

	}

	
/**
 * converts a file from supported formats to odt, using jodconverter.
 * @param input - input file.
 * @return the converted odt file.
 */
private  Filename doc2odt(Filename input) {
	Filename output = new Filename(input.getFullPath(), input.getBaseName(), "odt");
	output.deleteOnExit();

	if (Main.parameters.isUseUnoconv()) {
		return unoConvert(input, output);
	}
	else {
		return jodConvert(input, output);
	}

		
}

	private Filename jodConvert(Filename input, Filename output) {
		OpenOfficeConnection connection = new SocketOpenOfficeConnection(8100);
		
		
		try {
//			try {
				connection.connect();
//			} catch (ConnectException e) {
			/* commented because this doesnt work.
			  	String[] command = { "soffice", "-headless",
			 
						"-accept=\"socket,host=127.0.0.1,port=8100;urp;\"",
						"-nofirststartwizard" };
				Process p = Runtime.getRuntime().exec(command);
					{
						String forLog = "";
						for (String s : command){
							forLog += (s + " ");
						}
						log(forLog);
					}
				p.waitFor();
				log("wating...");
//				p.wait(2000);
//				log("done.");
					wait(2000);
				connection.connect();
				*/
//			}

		} catch (Exception e) {

			StringBuffer message = new StringBuffer();
			message.append(
					"we have encountered an error during connection to openoffice (or libreoffice)\n");
			message.append(
					"אירעה שגיאה בעת התחברות לתכנה אופן אופיס או ליברה אופיס\n");
			message.append(
					"כדי ליצור חיבור לאופן אופיס יש לפתוח פורט ל8100. בלינוקס ניתן באמצאות השורה הבאה:\n");
			message.append(
					"soffice -headless -accept=\"socket,host=127.0.0.1,port=8100;urp;\" -nofirststartwizard\n");
			

			errorMessage(message.toString(), e);
			return null;

		}
		
	try {
		try {
			DocumentConverter converter = 
				new OpenOfficeDocumentConverter(connection);
			converter.convert(input, output);
		} catch (Exception e) {

			StringBuffer message = new StringBuffer();
			message.append("we have encountered an error during conversion to odt format\n");
			message.append("אירעה שגיאה בעת המרת קובץ לפורמט אופן אופיס\n");

			errorMessage(message.toString(), e);
			return null;
		}
	} finally {
		connection.disconnect();
	}
	return output;
	}

	private Filename unoConvert(Filename input, Filename output) {
//		Filename output = new Filename(input.getFullPath(), input.getBaseName(), "odt");
		BufferedReader reader;
		StringBuffer unoconvLog= new StringBuffer();
//		output.deleteOnExit();
		try{
			String[] command = {"unoconv", "-f", "odt", input.getFilename()};
			Process p = Runtime.getRuntime().exec(command);
			log(command.toString());
			p.waitFor();
//			reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
			reader = new BufferedReader(new InputStreamReader(p.getErrorStream()));
//			System.out.println("unoconv end");
			
			String line=reader.readLine(); 
			while(line!=null) 
			{ 
				unoconvLog.append(line + "\n");
//				System.out.println(line);
				line=reader.readLine(); 
				
			}

			if (!output.exists()){
				throw new Exception("unoconv error");
			}
			return output;
		} catch (Exception e) {
			StringBuffer message = new StringBuffer();
			message.append("we have encountered an error during conversion to odt format\n");
			message.append("אירעה שגיאה בעת המרת קובץ לפורמט אופן אופיס\n");
			message.append("וודא שunoconv מותקן ושopenoffice פתוח\n");
			
			message.append(
			"כדי ליצור חיבור לאופן אופיס יש לפתוח פורט ל2002. בלינוקס ניתן באמצאות השורה הבאה:\n");
	message.append(
			"soffice -headless -accept=\"socket,host=127.0.0.1,port=2002;urp;\" -nofirststartwizard\n");
			
			message.append(unoconvLog);
						
			errorMessage(message.toString(), e);
			return null;
		}
		
	} 
	
	

	void message (String str){
		System.out.println();
		System.out.println(str);
	}
	
	void message (String ... strings){
		StringBuffer buffer = new StringBuffer();
		for (String str: strings ){
			buffer.append(str);
			buffer.append("\n");
		}
		message(buffer.toString());
	}

	void errorMessage (String str, Exception e){
		System.err.println(str);
		System.err.println(e);
	}
	
	void errorMessage (String str) {
		System.err.println(str);
	}
	
	void errorMessage (String ... strings ) {
		StringBuffer buffer = new StringBuffer();
		for (String str: strings ){
			buffer.append(str);
			buffer.append("\n");
		}
		errorMessage(buffer.toString());		
	}

	private void success(Filename oryFilename) {
		message("opporation completed successfully", "הפעולה הסתיימה בהצלחה",
				"input: " + inputFilename.getPath(), "output: " + oryFilename.getPath());

	}

	/**
	 * @deprecated
	 * @param inputFilename
	 */
	void launchInfo(String inputFilename){
		log("runing odt2ory ver 0.2");
		log("processing file: " + inputFilename );
	}

	public static void log(String string) {
		System.out.println(string);
	}
	
	public static void dbgLog(String string) {
		if (Main.parameters.isDebug())
			log(string);
	}

}