package bookmark;

import java.util.Date;

import book.ChapterID;

/*
 * This special type of bookmark updates as the date changes. 
 *  Obviously, it's abstract and must be implemented manually for each special type.
 */

public abstract class DateUpdatingBookmark implements IBookmark {

	public ChapterID getAddress() {
		// TODO Auto-generated method stub
		return null;
	}

	public String getDdisplayName() {
		// TODO Auto-generated method stub
		return null;
	}

	public abstract void update(Date date);
	
}
