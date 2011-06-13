package ory;


/** class to handle footnotes and endnotes
 * from an odt document.
 * @author izar00@gmail.com
 *
 */

public class Note {
	private StringBuffer noteReference, noteText;
	private static int counter;
	private int noteNumber;
	
	Note (String str){
//		System.err.println("Worning! this program doesnt know how to handle " +
//				"footnotes yet.");
//		System.err.println("skipping footnote.");
		counter++;
		noteNumber = counter;
		
		noteReference= new StringBuffer();
		setReference();
		noteText = new StringBuffer();
		setText(str); //TODO - get the text from the element.
	}

	
	private void setReference() {
		noteReference.append("&nbsp;");
		noteReference.append("<a href=\"#footnote-" + noteNumber + "\" " + 
				"id=\"noteref-" + noteNumber + "\">" );
		noteReference.append("[" + noteNumber + "]");
		noteReference.append("</a>");
	}
	
	private void setText(String string) {
		
		noteText.append("<p>");
		noteText.append("<a href=\"#noteref-" + noteNumber + "\" " +
				"id=\"footnote-" + noteNumber + "\">");
		noteText.append("[" + noteNumber + "]");
		noteText.append("</a>");
		noteText.append(string); //TODO: chomp the heading newline.
		noteText.append("<p>");
	}
	
	public String getReference() {
		return noteReference.toString();
	}
	
	public String getText() {
		return noteText.toString();
	}

}
