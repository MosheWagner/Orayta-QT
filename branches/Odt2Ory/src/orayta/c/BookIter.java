package orayta.c;


public class BookIter {
	private String[] mLevelName;
	final String LevelSigns = "!~@^#";
	
	public BookIter(){
		mLevelName = new String[5];
		for (int i=0; i<mLevelName.length; i++) {	mLevelName[i] = "";}
	}

	public void SetLevelFromLine(String level_line) {
		if (level_line.length() > 2) //It must have at least a symbol, a space, and then a value
	    {
	        int level = LevelSigns.indexOf(level_line.charAt(0));

	        if (level != -1)
	        {
	            mLevelName[level] = removeSigns(level_line.substring(2));

	            if (level > 0) ZeroLevel(level - 1);
	        }
	    }
	}

	private void ZeroLevel(int level) {
		 if ( mLevelName[level] != "") mLevelName[level] = "0";

		    //Zero the one right under him
		    if ( level != 0)
		    {
		        if ( mLevelName[level] != "")
		            ZeroLevel(level-1);
		    }
		
	}

	//Removes any sign from the given unicode string
	// (Nikud or teamim)
	String removeSigns(String str)
	{
	    return removeNikud(removeTeamim(str));
	}

	//Removes any nikud sign from the given unicode string
	String removeNikud(String str )
	{
	    //Make sure nikud is in the normall format:
	    // String::normalized(String::NormalizationForm_D) - Means nikud is allways treated as two chars
	    // See: http://www.unicode.org/reports/tr15/#Norm_Forms
//	    String s = str.normalized(String::NormalizationForm_D);

	    //These are all of the nikud signs besides the teamim ones mixed in them
	    String regexp= "[" + (char)(0x05B0) + "-" + (char)(0x05BC) +
	                   (char)(0x05C1) + (char)(0x05C2) + (char)(0x05C4) + (char)(0x05C5) + (char)(0x05C7) +
	                   "]";

	    return str.replaceAll(regexp, "");
	}

	//Removes any teamim sign from the given unicode string
	String removeTeamim( String str)
	{
//	    String s = "" = str.normalized(String::NormalizationForm_D);
	    //(char)(0x0591) - HEBREW ACCENT ETNAHTA
	    //(char)(0x05AF) - HEBREW MARK MASORA CIRCLE
	    //(char)(0x05C0) - HEBREW PUNCTUATION PASEQ
	    //(char)(0x05C6) - HEBREW PUNCTUATION NUN HAFUKHA
	    //(char)(0x05BD) - HEBREW POINT METEG (?) -> nikud ?
	    String regexp = "[" + (char)(0x0591) + '-' +  (char)(0x05AF) +
	                   //Include the PUNCTUATION signs
	                   (char)(0x05C0) + (char)(0x05C6) + (char)(0x05BD) +
	                   (char)(0x05F3) + (char)(0x05F4) +
	                   "]";

	    //(char)(0x05C3) - HEBREW PUNCTUATION SOF PASUQ
	    //(char)(0x05BE) - HEBREW PUNCTUATION MAQAF
	    //Replace sof pasuk with ":" and makaf with " "
	    return str.replaceAll(String.valueOf((char)(0x05C3)), ":").replaceAll(String.valueOf((char)(0x05BE)), " ").replaceAll(regexp, "");
	}
	
	public String toString() {
		return this.toString(0);
	}

	public String toString(int fromLevel) {
		String s = "";
	    for (int i=0; i<mLevelName.length; i++)
	    {
	        s += "*";
	        if (i >= fromLevel) s += mLevelName[i].replace("0", "");
	    }

	    return s;
	}
	
	
}
