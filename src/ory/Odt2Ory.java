package ory;

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
 * create a gui.
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
   
    Odt2Ory(String inputFilename) throws Exception    {
    	  	
    	
    	if (inputFilename == null) {
    		System.out.println("no file selected");
    		return;
    	}
    	
    		    	
    	System.out.println("runing odt2ory ver 0.2");
    	System.out.println("processing file: " + inputFilename );
    	OryFile oryFile = new OryFile(inputFilename);
    	Filename oryFilename = new Filename(oryFile.saveToOryDir());

    	System.out.println("Extracted Text:");
    	System.out.println(oryFile.getFileText());


    	OryConfFile oryConf = new OryConfFile (oryFilename);
    	oryConf.setBookName(oryFile.getBookTitle());

    	oryConf.save();
    	
    	
    	
    		
    	 
    }
}