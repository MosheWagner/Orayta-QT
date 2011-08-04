package ory;

/**
 * this class is meant to translate links in the file 'torat ha-olah' 
 * @author izar00@gmail.com
 *
 */
public class THlink extends Href {
	
	private String level;
	
	THlink(String text, String link) {
		//sets the target to the target extracted from the odf link, then transforms it to base32 as orayta does.
		super(text, Href.escapeToBase32(Href.Xlink.parseXlink(link).getName()));
		Xlink xlink = Href.Xlink.parseXlink(link);
		
		Odt2Ory.dbgLog(xlink.getName());
		level= xlink.getLevel();
		setLocation();
	}
	
	private void setLocation(){
		String uid = "";
		if (level.startsWith("0.4")){
			uid = "13650";
		} else if (level.startsWith("0.6")) {
			uid = "13651";
		} else if (level.startsWith("0.7")) {
			uid = "13652";
		} else if (level.startsWith("0.8")) {
			uid = "13653";
		} else {
			uid = "13650"; //this is just a guess. we shouldn't get here.
			Odt2Ory.dbgLog("level out of range :" + level);
		}
		
//		uid = "13650"; //TODO: remove this line if we split the book.
		
		super.setLocation("!" + uid + ":");
		
	}
	

}
