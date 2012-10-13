package ory;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.xerces.dom.CoreDocumentImpl;
import org.cyberneko.html.parsers.DOMFragmentParser;

import org.w3c.dom.DocumentFragment;
import org.w3c.dom.Node;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import ory.fileTypes.HtmlFile;

/**
 * mainly extratcts text from a html file, also does some minimal conversion to orayta's format.
 * @author abe.izar izar00@gmail.com
 *
 */
public class HtmlToOryExtractor  {
	
	private Node mainNode;
	private StringBuffer text;
	private OryFiles files;
	protected String bookTitle;
	private int fileNum = -1;
	protected HtmlFile myFilename;
	private int[] headingsInUse;
	
	/**
	 * 
	 * @param file - a html file to convert.
	 * @throws SAXException
	 * @throws IOException
	 */
	public HtmlToOryExtractor(HtmlFile file) throws SAXException, IOException {
		super();
		assignDefaults();
		myFilename = file;
		
		DOMFragmentParser parser = new DOMFragmentParser();
		FileInputStream stream = new FileInputStream(file);
		InputSource inputSource = new InputSource(stream);
		
		DocumentFragment docFragment = (new CoreDocumentImpl()).createDocumentFragment();
		parser.parse(inputSource, docFragment);
		
		mainNode = docFragment;
		
	}
	
	private void assignDefaults() {
		text = new StringBuffer();
		files = new OryFiles();
		bookTitle = null;
		int[] empty = {0};
		headingsInUse = empty;
	}

		//TODO: separate between normal conversion and daat conversion.
	protected void appendElementText(Node node) {
		Node child = node.getFirstChild();

		while (child != null) {
			if (child.getNodeType() == Node.TEXT_NODE) {
				processText(child);
			} 
			else if (child.hasChildNodes()) {
				appendElementText(child);
			}
			
			child = child.getNextSibling();
			
		}
	}
	
	
	
	@Deprecated
	private String getNodeText(Node node) {
		StringBuilder myText = new StringBuilder("");
		
		while (node != null) {
			if (node.getNodeType() == Node.TEXT_NODE) {
				myText.append(node.getNodeValue());
			} else if (node.hasChildNodes()) {
				Node child = node.getFirstChild();
				myText.append(getNodeText(child));
				node = node.getNextSibling();
			}
		}
		return myText.toString();
	}

	private void processText(Node node) {
		String str = node.getNodeValue();
		if (bookTitle == null)
			findFirstLine(str);
		else	//if we arent at fist line yet then we dont append text.
			text.append(str);
	}

	private void findFirstLine(String str) {
		if (str == null)
			return;
		Pattern p = Pattern.compile("^\\s+(\\S(.|\\s)*)"); //matches a line that has at least one charactor that isn't whitespace, and also removes leading whitespace.
		Matcher m = p.matcher(str);
		
		if (m.matches()){
//			 Main.ui.dbgLog("first line:>>>" + str + "<<<\n");
			str = m.group(1);
			 bookTitle= new String(str);
			 text.append("$ " + str);
		 }
		
	}

	public static OryFiles walkThrough(HtmlFile file) throws Exception {
		HtmlToOryExtractor extractor = new HtmlToOryExtractor(file) ;
		extractor.getText();
				
		return extractor.getFiles();
	}
	

	
	/**
	 * Return the editable text content as a string
	 * @return the editable text content as a string
	 */
	
	public String getText() {
	
		
	appendElementText(mainNode);

	buildFile();
	return text.toString();
		
		
	}
	
	/**
	 * generates ory files from the input.
	 * @throws Exception 
	 */
	private void buildFile() {
		
		String name = myFilename.getBaseName();
		//if we have multipule files, number them
		if (fileNum  > -1) {
			name += fileNum;
		}
		
		Filename tmpFilename = new Filename (myFilename.getFullPath(), name, "tmp");
		OryFile oryFile = new OryFile(tmpFilename);
		
		
		StringBuffer fileText = new StringBuffer();
		
		
		fileText.append(text) ;
		
		//TODO: create this later.
//		String notes = getNotes();
//		if (! notes.isEmpty()){
//			int highestHeading = getHighestHeading(); 
//			
//			// make sure we have a valid value.
//			if (highestHeading < 1 || highestHeading > 4)
//				highestHeading = 4;
//			
//			fileText.append("\n");
//			fileText.append(OryFileExtractor.headingSymbol(highestHeading));
//			
//			fileText.append("הערות שוליים" + "\n"); 
//			fileText.append(notes);
//		}
		
		oryFile.setFileText(fileText);
		
		if (bookTitle == null)
			bookTitle = "שם הספר לא אותר";
		oryFile.setBookTitle(bookTitle);
		
		oryFile.setHeadings(headingsInUse);
		
		files.add(oryFile);

		
		
	} //end buildFile
	
	/**
	 * @return the OryFile objects creates by the extractor. 
	 */
	public OryFiles getFiles() {
		return files;
	}

}
