package orayta.c;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;

import ory.Filename;

/**Build DB for file, the same way orayta does in qt
 * the code is copied from orayta's source and then readopted.
 * @author avi
 *
 */
public class DB {
	private StringBuffer dbFile;
	private StringBuffer lmpFile;
//	private Filename ;
	public DB(StringBuffer ptext, StringBuffer lvlmpstr){
		dbFile = ptext;
		lmpFile = lvlmpstr;
	}
	public StringBuffer getDbFile() {
		return dbFile;
	}
	public void setDbFile(StringBuffer dbFile) {
		this.dbFile = dbFile;
	}
	public StringBuffer getLmpFile() {
		return lmpFile;
	}
	public void setLmpFile(StringBuffer lmpFile) {
		this.lmpFile = lmpFile;
	}
	
	public static DB generateSearchTextDB(Filename infile) {
		
		List<String> text = new ArrayList<String>();
		try {
			text = (FileUtils.readLines(infile, "UTF-8"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return generateSearchTextDB(text, infile);
	}
	
	public static DB generateSearchTextDB(String str, Filename infile) {
		List<String> text = new ArrayList<String>();
		try {
			text = IOUtils.readLines(IOUtils.toInputStream(str));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
				
				return generateSearchTextDB(text, infile);		
	}

	private static DB generateSearchTextDB(List<String> text, Filename infile) {
		//Set all String to work with unicode
		//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
		//    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

		//Holds the offsets of each BookIters of book in the "pureText". Use this to map search results in positions in the book
		//    QMap<int, BookIter> levelMap;
		//IZAR: notice: design change. (map holds string and not bookIter
		TreeMap<Integer, String> levelMap = new TreeMap<Integer, String>();


		StringBuffer ptext = new StringBuffer();

		//Read book's contents
//		List<String> text = new ArrayList<String>();

		//    boolean ok = ReadFileToList(infile, text, "UTF-8");

		

		//    if (!ok) qDebug() << "Can't read" << infile;


		//Build DB:
		BookIter itr = new BookIter(); 

		//If a line starts with one of these, it's a level sign
		String levelSigns = "!@#$^~";
		//Any char not matching this pattern, is no *pure* text.
		String notText = "[^א-תA-Za-z0-9 ]";
		//These are turned into spaces, and not just ignored.
		String spaceSigns = "[-_:׃.,;?]";

		//Pad with spaces, so "full word searches" work on begining and end of text too
		ptext.append(" ");

		for (int i=0; i<text.size(); i++)
		{
			String line = text.get(i);
			
			if (line== null || line.isEmpty()) {
				ptext.append(" ");
			}
			//Level line
			else if (levelSigns.contains(String.valueOf(line.charAt(0))))
			{
				//Update iter
				//but if levelSign is '$' ???
				itr.SetLevelFromLine(line);

				//Map with it's position in the pureText
				Integer len = new Integer(ptext.length());
				String value = new String (itr.toString());
				levelMap.put(len, value);

				
			}
			//Externall link
			else if (text.get(0).startsWith("<!--ex"))
			{
				//Ignore;
			}
			//Text line
			else
			{
				//Test if book is from the bible. Hope this is ok...
				if ( infile.getFilename().contains("מקרא") )
				{
					//Turn קרי וכתיב into only קרי. Maybe this should be an option?
					line = line.replaceAll( "[(][^)]*[)] [[]([^]]*)[]]", "$1");
				}

				//Remove html tags
				line = line.replaceAll("<[^>]*>", "" );

				//Remove all non "pure-text" chars
				line = line.replaceAll(String.valueOf((char)0x05BE), " "); //Makaf
				line = line.replaceAll(spaceSigns, " ");
				line = line.replaceAll(notText, "");

				//Remove double spaces and line breaks
				ptext.append(line.trim().replaceAll("\\s+", " "));

				//Pad with spaces, so "full word searches" work on begining and end of text too
				ptext.append(" ");
			}
		}

		

		StringBuffer lvlmpstr = new StringBuffer();

//		Set<Integer> keys = levelMap.keySet();
//		Collection<BookIter> vals = levelMap.values();
//
//		//    for (int i=0; i<levelMap.size(); i++)
//		for (Integer in: keys) 
//		{
//			lvlmpstr.append(String.valueOf(in) + "->" + levelMap.get(in).toString(0) + "\n");
//		}
		
		//IZAR: rewrite of previous block.
		Iterator<Entry<Integer, String>> it = levelMap.entrySet().iterator();
		while (it.hasNext()){
			Entry<Integer, String> entry = it.next();
			lvlmpstr.append(entry.getKey().toString() + "->" + entry.getValue() + "\n");
			it.remove(); // avoids a ConcurrentModificationException
		}
		
		DB db = new DB(ptext, lvlmpstr);
		return db;
	}
	
	public static void autoGenerateSearchTextDB(Filename infile,  String pureTextOutPath, String levelMapOutPath)
	{
	 
		DB db = generateSearchTextDB(infile);
		StringBuffer ptext = db.getDbFile();
		StringBuffer lvlmpstr = db.getLmpFile();
		
	    //Save the files
//	    writetofile(pureTextOutPath, ptext, "UTF-8", true);
	    //TODO: decide which ui to use
	    ory.Utils ui = new ory.Utils();
	    try {
			ui.saveStringToFile(ptext.toString(), new Filename (pureTextOutPath));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
//	    writetofile(levelMapOutPath, lvlmpstr, "UTF-8", true);
	    try {
			ui.saveStringToFile(lvlmpstr.toString(), new Filename (levelMapOutPath));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

/*	public static boolean ReadFileToList(String filename, StringBuffer text,
			String string) {
		boolean conflinesended = false;
		   
	    //Stop if it's not a valid file:
//	    QFileInfo fi(filename);
		File fi = new File(filename);
	    if ( fi.isDirectory() || !fi.exists() )
	    {
	       // qDebug() << "wrong file: " << filename;

	        if (fi.isDirectory())
	            log ("can't open file: " + filename + " it is a dir!");
	        if ( !fi.exists())
	        	log ("can't open file: " + filename + " file doesn't exist.");
	        return false;
	    }

	    //Read the source file:
	    File infile = new File(filename);
	    // Open the file
	    if ( !infile.ope(QIODevice::ReadOnly) )
	    {
	        qDebug() << "cant open file:"  << filename;
	        return false;
	    }

	    // Set the stream to read from the file
	    QTextStream t( &infile );
	    t.setCodec(QTextCodec::codecForName(encoding_name));
	    while (!t.atEnd())
	    {
	        text << t.readLine();
	    }
	    infile.close();

	    return true;
	}
*/
	private static void log(String string) {
		System.out.println(string);
		
	}
	
//	public static class DB {
//		
//	}
}
