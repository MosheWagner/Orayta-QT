package ory.fileTypes;


import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import org.apache.commons.io.IOUtils;

import orayta.c.DB;


import ory.Filename;

public class ObkFile extends Filename {
	public static final String EXTENSION ="obk";
	private String text, dbText, lmp, confText;
	public static final String BOOK_TEXT = "BookText";
	public static final String SEARCH_DB = "SearchDB";
	public static final String LEVEL_MAP = "LevelMap";
//	private ZipOutputStream zip;
	
	public ObkFile(Filename name) {
		// set our filename to be the same as given with our extension.
		super(name.getFullPath(), name.getBaseName(), EXTENSION);
		
		//init fields
		text = "";
		dbText = "";
		lmp = "";
		confText= "";
			
	}
	
	public void save() throws IOException{
		
		ZipOutputStream zip = new ZipOutputStream(new BufferedOutputStream(new FileOutputStream(this)));
		
		//add the text to the zip archive
		addEntryToZip(BOOK_TEXT, text, zip);
		addEntryToZip(SEARCH_DB, dbText, zip);
		addEntryToZip(LEVEL_MAP, lmp, zip);
		zip.setComment(confText);
		
		zip.flush();
		zip.close();		

	}
	
	/**
	 * 
	 * @param name the new entry name
	 * @param text the text to add as a new entry
	 * @param zip the zip object to write to.
	 * @throws IOException 
	 */
	private void addEntryToZip(String name, String text, ZipOutputStream zip) throws IOException{
		
		final int BUFFERSIZE = 1024;
		byte[] data = new byte[BUFFERSIZE];
		
		InputStream in = IOUtils.toInputStream(text);
		
		int count;
		zip.putNextEntry(new ZipEntry(name));
		
		while((count = in.read(data, 0, BUFFERSIZE)) != -1){
			zip.write(data, 0, count);
		}
		in.close();
		zip.closeEntry();
		zip.flush();
		
	}
	

	public String getText() {
		return text;
	}

	/**
	 * set the text and automatically generate the files needed for search DB.
	 * @param text - the book text of this obk file.
	 */
	public void setText(String text) {
		this.text = text;
		DB db = DB.generateSearchTextDB(text, this);
		dbText = db.getDbFile().toString();
		lmp = db.getLmpFile().toString();
	}

	public String getDbText() {
		return dbText;
	}

	public void setDbText(String dbText) {
		this.dbText = dbText;
	}

	public String getLmp() {
		return lmp;
	}

	public void setLmp(String lmp) {
		this.lmp = lmp;
	}

	public String getConfText() {
		return confText;
	}

	public void setConfText(String confText) {
		this.confText = confText;
	}
	
	
	
}
