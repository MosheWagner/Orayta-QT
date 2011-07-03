package ory;


/** class to handle footnotes and endnotes
 * from an odt document.
 * @author izar00@gmail.com
 *
 */

public class Note {
	private StringBuffer noteReference, noteText;
	private String originalText;
//	private final int counter;
	private final int noteNumber;
	
	Note (String str, int num){
//		System.err.println("Worning! this program doesnt know how to handle " +
//				"footnotes yet.");
//		System.err.println("skipping footnote.");
		
		noteNumber = num;
		
		originalText = new String(str);
		
		noteReference= new StringBuffer();
		setReference();
		noteText = new StringBuffer();
		setText(); //TODO - get the text from the element.
	}

	
	private void setReference() {
		noteReference.append("&nbsp;");
		noteReference.append("<a href=\"#footnote-" + noteNumber + "\" " + 
				"id=\"noteref-" + noteNumber + "\"");

		//this will add the note text to the reference too, so that it would be shown when hovering above it.
		if (originalText.length() < 500){
			noteReference.append(" title=\"" + originalText + "\"");
		}
		
		noteReference.append(">" );
		noteReference.append("[" + noteNumber + "]");
		noteReference.append("</a>");
	}
	
	private void setText() {
		
		noteText.append("<p>");
		noteText.append("<a href=\"#noteref-" + noteNumber + "\" " +
				"id=\"footnote-" + noteNumber + "\">");
		noteText.append("[" + noteNumber + "]");
		noteText.append("</a>");
		noteText.append(originalText); //TODO: chomp the heading newline.
		noteText.append("<p>");
	}
	
	public String getReference() {
		return noteReference.toString();
	}
	
	public String getText() {
		return noteText.toString();
	}

}
