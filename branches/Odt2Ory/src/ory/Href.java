package ory;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Href {
	
//	private StringBuffer refReference, refText;
	private final String originalText; 
	private String target, location;
//	private final int counter;
//	private final int refNumber;
	
	/**
	 * @param text- the text to display
	 * @param id- id for this link. use with 'getTarget()'
	 */
	public Href (String text, int id){
		originalText = text;
		target="refId-" + id;
		location = "#";
	
	}
	
	public Href (String text, String target){
		originalText = text;
		this.target= target;
		
		location = "#";
	
	}
	
	public Href (String text, String target, String location){
		originalText = text;
		this.target= target;
		this.location = location;
	
	}
	
	/**
	 * this sets the place where we want to link from.
	 * @return the html string.
	 */
	String getReference(){
		
		StringBuffer refReference = new StringBuffer();
		
		refReference.append("<a href=\"");
		refReference.append(location);
		refReference.append(target);
		refReference.append("\">");
		
		refReference.append(originalText);
		
		refReference.append("</a>");
		
		return refReference.toString();
		
		
	}
	
	/**
	 * this is for where we want to link to.
	 * @return the html anchor.
	 */
	String getTarget() {
		StringBuffer refText = new StringBuffer();
		
		refText.append("<a name=\"");
		refText.append(target);
		refText.append("\">");
		refText.append("</a>");
		
		return refText.toString();
	}
	
	
	
	/**
	 * translates a string into a format that is used in orayta
	 * for internal links.
	 * this is similar to the function by the same name used in orayta. 
	 * @param str - input string.
	 * @return translated string.
	 */
	public static String escapeToBase32(String str){
		
		str = str.replaceAll(" ", "_"); //seems like orayta does this conversion too.
		String result = "";
		for (int i=0; i<str.length(); i++){
			result += Integer.toString(str.codePointAt(i), 32) + "U";
		}
		
		return result;
		
	}

	public void setTarget(String target) {
		this.target = target;
	}

	/**
	 * @return the location e.g. if this link points to somewhere here then the location is "#"
	 */
	public String getLocation() {
		return location;
	}

	/**
	 * @param location e.g. if this link points to somewhere here then the location is "#"
	 */
	public void setLocation(String location) {
		this.location = location;
	}

	/**
	 * @return the originalText
	 */
	public String getOriginalText() {
		return originalText;
	}
	
	static class Xlink{
		
		private final String head, level, name, tail;
		
		Xlink (String head, String level, String name, String tail){
			this.head = head;
			this.level = level;
			this.name = name;
			this.tail = tail;
			
		}
		
		/**
		 * 
		 * @param link - a odf type link to parse.
		 * @return Xlink object 
		 */
		public static Xlink parseXlink(String link){
		
//		if (! Xlink.matches("#.*|outline")){
//			return null;
//		}
		
		Pattern p = Pattern.compile("^(\\D)?((?:\\d+\\.)*)([^|]*)(\\|outline)?$");
		Matcher m = p.matcher(link);
		
		if (!m.matches()){
			Odt2Ory.dbgLog("not found");
			Xlink empty = new Xlink ("","", "", "");
			return empty;
		}
		
//		Odt2Ory.dbgLog(m.group(1));	
//		Odt2Ory.dbgLog(m.group(2));
//		Odt2Ory.dbgLog(m.group(3));
//		Odt2Ory.dbgLog(m.group(4));
		
		Xlink xlink = new Xlink (m.group(1), m.group(2), m.group(3), m.group(4));
		
		return xlink;
			
		}

		/**
		 * @return the head
		 */
		public String getHead() {
			return head;
		}

		/**
		 * @return the level
		 */
		public String getLevel() {
			return level;
		}

		/**
		 * @return the name
		 */
		public String getName() {
			return name;
		}

		/**
		 * @return the tail
		 */
		public String getTail() {
			return tail;
		}
		
	}

}
