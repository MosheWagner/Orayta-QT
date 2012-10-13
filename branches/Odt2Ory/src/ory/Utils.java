package ory;

import java.io.IOException;
import java.util.Scanner;

import org.apache.commons.io.FileUtils;

public class Utils {
	private Parameters parameters;
	
	public Utils() {
		parameters = Main.parameters;
		if (parameters == null){
			String s[] = new String[1];
			s[0] = "";
			parameters = new Parameters(s);
		}
	}
	
	public String getInputFile() {
		return parameters.getInputFilename();
	}

	public void message (String str){
		System.out.println();
		System.out.println(str);
	}
	
	public void message (String ... strings){
		StringBuffer buffer = new StringBuffer();
		for (String str: strings ){
			buffer.append(str);
			buffer.append("\n");
		}
		message(buffer.toString());
	}

	public void errorMessage (String str, Exception e){
		System.err.println(str);
		System.err.println(e);
		e.printStackTrace();
	}
	
	public void errorMessage (String str) {
		System.err.println(str);
	}
	
	public void errorMessage (String ... strings ) {
		StringBuffer buffer = new StringBuffer();
		for (String str: strings ){
			buffer.append(str);
			buffer.append("\n");
		}
		errorMessage(buffer.toString());		
	}

	

	
	public void log(String string) {
		System.out.println(string);
	}
	
	public void dbgLog(String string) {
		if (parameters.isDebug())
			log(string);
	}

	/**
	 * saves a string as a file on the disk.
	 * WARNING - may return null.
	 * @param string - string to save.
	 * @param outFile - filename to which we want to save the string.
	 * @return the name of the file that was saved, null if something went wrong.
	 * @throws IOException
	 */
	public Filename saveStringToFile(String string, Filename outFile) throws IOException {
		outFile = checkExistance(outFile); //check if file already exists, and if it does, decide what to do. 
		if (outFile== null)
			return null;
		FileUtils.writeStringToFile(outFile, string, "utf-8");
		return outFile;
	}
	
	/**
	 * check if file already exists, and if it does, decide what to do.
	 * WARNING- may return null. 
	 * @param outFile
	 * @return a different or the same file.
	 */
	Filename checkExistance(Filename outFile) {
		if (! outFile.exists())
			return outFile; //create new file.
		else if (parameters.isForce())
			return outFile; //overwrite existing file.
		else if (parameters.isSkip())
			return null;
		else 
		{ //else, ask the user what to do, and check the new file
			Filename newFile = askUser(outFile);
			return newFile;
		}
		
	}

	private Filename askUser(Filename outFile) {
		String question = "the file \"" + outFile + "\" already exists.\n" +
				"select new filename(n), select a differnt path(d), overwrite one(o) overwrite all(O) skip this file(s) skip all(S) or abort(a)?\n" +
				"הקובץ כבר קיים. מה ברצונך לעשות?\n" +
				"לבחור שם חדש(n) לבחור נתיב חדש(d) להחליף קובץ קיים (o) להחליף את כל הקבצים(O) לדלג על הקובץ הנוכחי (s) לדלג על כל הקבצים (S) או לצאת מהתוכנה(a)?\n" +
				"\n\ntype your choice (n/d/o/O/s/S/a):";
		char choice = getUserInput(question).charAt(0); 
		
		Filename newFile = null; //default is to skip.
				
		switch (choice) { 
			case 'n' : {
				newFile = createNewFileInPath(outFile);
				checkExistance(newFile);
				break;
			}
			case 'd' : {
				newFile = setPathFor(outFile);
				checkExistance(newFile);
				break;
			}
			case 'o' : {
				newFile = outFile;
				break;
			}
			case 'O' : {
				parameters.setForce(true);
				newFile = outFile;
				break;
			}
			case 'a' : {
				message("operation aborted");
				System.exit(0);
				break;
			}
			case 's': {
				newFile = null;
				break;
			}
			case 'S': {
				parameters.setSkip(true);
				newFile = null;
				break;
			}
			default :
			{
				message ("not understood. retrying");
				askUser(outFile);
			}
			
		}
		
		return newFile;
				
	}

	private Filename setPathFor(Filename outFile) {
		String path = getUserInput("type a path. (the original filename will be kept)");
		return new Filename (path, outFile.getName());
	}
	
	/**
	 * ask the user for a question, and get his answer.
	 * @param string - question
	 * @return the users answer.
	 */
	public String getUserInput(String string) {
		message (string);
		Scanner scan = new Scanner(System.in);
		return scan.next();
	}

	private Filename createNewFileInPath(Filename outFile) {
		String name = getUserInput("type a new filename. (the directory is: \"" + outFile.getFullPath() + "\")");
		return new Filename (outFile.getFullPath(), name, outFile.getExtension());
	}
	
	/**
	 * make a conf for the parent directory of currently processed file.
	 * @param child - the curently processed file.
	 * @param name - the display name for the parent directory.
	 * @return the filename of the created conf, null if cant get the parent name.
	 * @throws IOException 
	 */
	public Filename makeParentConf (Filename child, String name) throws IOException{
		String parent = child.getParent();
		if (parent == null)
			return null;
		
		OryConfFile parentConf = new OryConfFile(new Filename (parent));
		if (! parentConf.getFilename().exists()){ //make sure the conf for parent doesn't already exist.
			parentConf.setBookName(name);
			parentConf.save();
			message ("created parent conf: " + parentConf.getFilename());
		}
		return parentConf.getFilename();
	}
}
