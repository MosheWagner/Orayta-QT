package ory;

import java.io.File;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.apache.commons.lang.StringUtils;
import org.odftoolkit.odfdom.doc.OdfDocument;

import org.odftoolkit.odfdom.doc.table.OdfTable;
import org.odftoolkit.odfdom.doc.table.OdfTableRow;
import org.odftoolkit.odfdom.dom.OdfContentDom;
import org.odftoolkit.odfdom.dom.OdfMetaDom;
import org.odftoolkit.odfdom.dom.OdfStylesDom;
import org.odftoolkit.odfdom.dom.element.draw.DrawObjectElement;
import org.odftoolkit.odfdom.dom.element.office.OfficeAnnotationElement;
import org.odftoolkit.odfdom.dom.element.office.OfficeMetaElement;
import org.odftoolkit.odfdom.dom.element.style.StyleMasterPageElement;
import org.odftoolkit.odfdom.dom.element.table.TableTableElement;
//import org.odftoolkit.odfdom.dom.element.text.TextAElement;
import org.odftoolkit.odfdom.dom.element.text.TextAElement;
import org.odftoolkit.odfdom.dom.element.text.TextHElement;
import org.odftoolkit.odfdom.dom.element.text.TextNoteBodyElement;
import org.odftoolkit.odfdom.dom.element.text.TextNoteCitationElement;
import org.odftoolkit.odfdom.dom.element.text.TextTableOfContentElement;
//import org.odftoolkit.odfdom.dom.element.text.TextNoteElement;
import org.odftoolkit.odfdom.dom.element.text.TextPElement;
import org.odftoolkit.odfdom.dom.element.text.TextTrackedChangesElement;
import org.odftoolkit.odfdom.incubator.doc.text.OdfTextExtractor;
import org.odftoolkit.odfdom.pkg.OdfElement;
import org.w3c.dom.NodeList;

/**
 * It's a sub class of OdfTextExtractor. It provides a method to return all the text 
 * that the user can typically edit in a document, including text in cotent.xml, 
 * header and footer in styles.xml, meta data in meta.xml. 
 * 
 * <p>This function can be used by search engine, and text analytic operations. </p> 
 *
 */

public class OryFileExtractor extends OdfTextExtractor {

	private OdfDocument mDocument = null;
	private OdfElement mElement = null;
	private boolean mIsDocumentExtractor = false;
	private StringBuffer noteHolder;
	private String bookTitle;
	protected static final char NewLineChar = '\n';
	private int highestHeading;
	
	// these will tell us which elements to extact.
	private boolean printNotes, printTableOfContents, printAnnotations,
		printXlinks;

	/**
	 * Constructor with an ODF document as a parameter
	 * @param doc the ODF document whose editable text would be extracted. 
	 */
	private OryFileExtractor(OdfDocument doc) {
		mIsDocumentExtractor = true;
		mDocument = doc;
		assignDefaults();
		
	}

	/**
	 * Constructor with an ODF element as parameter
	 * @param element the ODF element whose editable text would be extracted. 
	 */
	private OryFileExtractor(OdfElement element) {
		mIsDocumentExtractor = false;
		mElement = element;
		assignDefaults();
	}

	private void assignDefaults() {
		mTextBuilder = new StringBuilder();
		noteHolder = new StringBuffer();
		bookTitle = null;
		
		printNotes = true;
		printTableOfContents = false;
		printAnnotations = false;
		printXlinks = false;
		
		highestHeading = 5;
		
	}

	/**
	 * An instance of OryFileExtractor will be created to 
	 * extract the editable text content of an ODF element.
	 * @param doc the ODF document whose text will be extracted.
	 * @return An instance of OryFile
	 * @throws Exception 
	 */
	public static OryFileExtractor newOryFileExtractor(OdfDocument doc) throws Exception {
		return new OryFileExtractor(doc.getContentRoot());
	}

	/**
	 * An instance of OryFileExtractor will be created to 
	 * extract the editable text content of an ODF element.
	 * @param element the ODF element whose text will be extracted.
	 * @return An instance of OryFile
	 */
	public static OryFileExtractor newOryFile(OdfElement element) {
		return new OryFileExtractor(element);
	}
	
	/**
	 * An instance of OryFileExtractor will be created to 
	 * extract the editable text content of an odt file.
	 * @param file- the .odt file whose text will be extracted.
	 * @return An instance of OryFileExtractor
	 * @throws Exception 
	 */
	public static OryFileExtractor newOryFileExtractor(File file) throws Exception {
		OdfDocument doc = OdfDocument.loadDocument(file);
		return new OryFileExtractor(doc.getContentRoot());
	}
	
	/**
	 * An instance of OryFileExtractor will be created to 
	 * extract the editable text content of an odt file.
	 * @param file- the .odt file whose text will be extracted.
	 * @return An instance of OryFileExtractor
	 * @throws Exception 
	 */
	public static OryFileExtractor newOryFileExtractor(Filename filename) throws Exception {
		File file = new File(filename.getFilename());
		return newOryFileExtractor(file);
	}

	/* (non-Javadoc)
	 * @see org.odftoolkit.odfdom.dom.DefaultElementVisitor#visit(org.odftoolkit.odfdom.dom.element.draw.DrawObjectElement)
	 */
	@Override
	public void visit(DrawObjectElement element) {
		String embedDocPath = element.getXlinkHrefAttribute();
		OdfDocument embedDoc = ((OdfDocument) (((OdfContentDom) element.getOwnerDocument()).getDocument())).loadSubDocument(embedDocPath);
		if (embedDoc != null) {
			try {
				mTextBuilder.append(OryFileExtractor.newOryFileExtractor(embedDoc).getText());
			} catch (Exception e) {
				Logger.getLogger(OryFileExtractor.class.getName()).log(Level.SEVERE, null, e);
			}
		}
	}

	/* (non-Javadoc)
	 * @see org.odftoolkit.odfdom.dom.DefaultElementVisitor#visit(org.odftoolkit.odfdom.dom.element.text.TextTrackedChangesElement)
	 */
	@Override
	public void visit(TextTrackedChangesElement ele) {
		return;
	}

	/* 
	 * this was meant to get text from xlinks.
	 * yet it only causes trouble.
	 */
	public void visit(TextAElement ele) {
		if (printXlinks) {
			String link = ele.getXlinkHrefAttribute();
			mTextBuilder.append(link);
			appendElementText(ele);
		}
	}

	/* (non-Javadoc)
	 * @see org.odftoolkit.odfdom.dom.DefaultElementVisitor#visit(org.odftoolkit.odfdom.dom.element.text.TextTabElement)
	 */
	@Override
	public void visit(TableTableElement ele) {
		OdfTable table = OdfTable.getInstance(ele);
		List<OdfTableRow> rowlist = table.getRowList();
		for (int i = 0; i < rowlist.size(); i++) {
			OdfTableRow row = rowlist.get(i);
			for (int j = 0; j < row.getCellCount(); j++) {
				mTextBuilder.append(row.getCellByIndex(j).getDisplayText()).append(TabChar);
			}
			mTextBuilder.append(NewLineChar);
		}
	}
	
	@Override
	public void visit(TextPElement ele) {
		mTextBuilder.append(NewLineChar);
		if (bookTitle == null)
         	findFirstLine(ele);
		
		appendElementText(ele); //TODO: make sure we dont get unwanted '$' character in begining of line.
	}
	
	@Override
	public void visit(TextHElement ele) {
		mTextBuilder.append(NewLineChar);
		OdfTextExtractor extractor = OdfTextExtractor.newOdfTextExtractor(ele) ;
		 String str = extractor.getText();
		 
		 if (StringUtils.isNotBlank(str)) {
			 if (bookTitle == null)
	         	findFirstLine(ele);
			 else {
				 
				 int level = ele.getTextOutlineLevelAttribute();
				 appendHeading(level);
			 }
			 appendElementText(ele);
		 }
		 
	}
	
	private void findFirstLine (OdfElement ele) {
		 OdfTextExtractor extractor = OdfTextExtractor.newOdfTextExtractor(ele) ;
		 String str = extractor.getText();
		 if (StringUtils.isNotBlank(str)) {
			 bookTitle= new String(str);
			 mTextBuilder.append("$ ");
		 }
	 }
	
	private void appendHeading (int level) {
		genHighestLevel(level);
		mTextBuilder.append(headingSymbol(level));
    		
    	}
	
	/**
	 * keep the highest heading level used in this file. 
	 * @param level
	 */
	private void genHighestLevel(int level) {
		if (level < highestHeading && level > 0)
			highestHeading = level;
	}

	/**
	 * returns the appropriate heading symbol for the given level
	 * with a following space.
	 * @param level - the heading level (the lower the level, the higher the heading).
	 * @return the appropriate heading symbol for the given level
	 */
	public static String headingSymbol(int level) {
		switch (level) {
			case 1:
				return ("# ");
			case 2:
				return ("^ ");
			case 3:
				return ("@ ");
			case 4:
				return ("~ ");
			case 5:
				return ("! ");
			default : //commented because we dont want anything to happen with stuff below this level. 
			//	return ("! ");
				return null;
		}
	}

//	public void visit(TextNoteElement note){}
	
	/**
	 * overrides the original feature because we dont want this text to be printed. 
	 */
	@Override
	public void visit(TextNoteCitationElement ele) {} 
	
	@Override
	public void visit(TextNoteBodyElement ele) {
		if (printNotes) {
			OdfTextExtractor extractor = super.newOdfTextExtractor(ele) ;

			Note note = new Note(extractor.getText());
			mTextBuilder.append(note.getReference());
			noteHolder.append(note.getText() + NewLineChar);
			//		noteHolder.append(extractor.getText());
		}
	}
	
	/**
	 * override to disable printing of annotaitions.
	 * TODO: make a way for the user to manage this option.
	 */
	public void visit(OfficeAnnotationElement ele) {
		if (printAnnotations){
			super.visit (ele);
		}
	}
	
	/**
	 * override to disable printing of table of contents.
	 * TODO: make a way for the user to manage this option.
	 */
	public void visit(TextTableOfContentElement ele) {
		if (printTableOfContents){
			super.visit (ele);
		}
	}
	
	/**
	 * Return the editable text content as a string
	 * @return the editable text content as a string
	 */
	@Override
	public String getText() {
		if (mIsDocumentExtractor) {
			return getDocumentText();
		} else {
			visit(mElement);
			return mTextBuilder.toString();
		}
	}
	
	public String getNotes() {
		return noteHolder.toString();
	}

	private String getDocumentText() {
		StringBuilder builder = new StringBuilder();
		try {
			//Extract text from content.xml
			OryFileExtractor contentDomExtractor = newOryFile(mDocument.getContentRoot());
			builder.append(contentDomExtractor.getText());

			//Extract text from style.xml
			OdfStylesDom styleDom = mDocument.getStylesDom();
			if (styleDom != null) {
				StyleMasterPageElement masterpage = null;
				NodeList list = styleDom.getElementsByTagName("style:master-page");
				if (list.getLength() > 0) {
					masterpage = (StyleMasterPageElement) list.item(0);
				}
				if (masterpage != null) {
					builder.append(newOryFile(masterpage).getText());
				}
			}

			//Extract text from meta.xml
			OdfMetaDom metaDom = mDocument.getMetaDom();
			if (metaDom != null) {
				OdfElement root = metaDom.getRootElement();
				OfficeMetaElement officemeta = OdfElement.findFirstChildNode(OfficeMetaElement.class, root);
				if (officemeta != null) {
					builder.append(newOryFile(officemeta).getText());
				}
			}

			return builder.toString();
		} catch (Exception e) {
			Logger.getLogger(OryFileExtractor.class.getName()).severe(e.getMessage());
			return builder.toString();
		}
	}

	public String getBookTitle() {
		return bookTitle;
	}

	public void setBookTitle(String bookTitle) {
		this.bookTitle = bookTitle;
	}

	/**
	 * @return the printNotes
	 */
	public boolean isPrintNotes() {
		return printNotes;
	}

	/**
	 * @param printNotes set to false if you don't want us to print footnotes and end notes
	 */
	public void setPrintNotes(boolean printNotes) {
		this.printNotes = printNotes;
	}

	/**
	 * @return the printTableOfContents
	 */
	public boolean isPrintTableOfContents() {
		return printTableOfContents;
	}

	/**
	 * @param printTableOfContents set to true if you want us to print Table Of Contents
	 */
	public void setPrintTableOfContents(boolean printTableOfContents) {
		this.printTableOfContents = printTableOfContents;
	}

	/**
	 * @return the printAnnotations
	 */
	public boolean isPrintAnnotations() {
		return printAnnotations;
	}

	/**
	 * @param printAnnotations - set to true if you want us to print Annotations
	 */
	public void setPrintAnnotations(boolean printAnnotations) {
		this.printAnnotations = printAnnotations;
	}

	/**
	 * @return the printXlinks
	 */
	public boolean isPrintXlinks() {
		return printXlinks;
	}

	/**
	 * @param printXlinks set to true if you want us to print internal links
	 */
	public void setPrintXlinks(boolean printXlinks) {
		this.printXlinks = printXlinks;
	}

	/**
	 * @return the highest Heading level used in this file.
	 */
	public int getHighestHeading() {
		return highestHeading;
	}
	
}




