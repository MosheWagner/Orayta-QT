package book;

public class DChapter implements IChapter {

	ChapterID mAddress;
	String chapterText;
	
	public DChapter(ChapterID address, String text)
	{
		mAddress = address;
		chapterText = text;
	}
	
	/*
	public void setText(String text)
	{
		chapterText = text;
	}
	
	public void setAddress(IChapterAddress address)
	{
		mAddress = address;
	}
	*/
	
	//public int compareTo(IChapter o) {
	//	return mAddress.compareTo(o.getChapterAddress());
	//}

	public ChapterID getChapterAddress() {
		return mAddress;
	}

	public boolean isAtom() {
		return (!text().equals(""));
	}

	public String text() {
		return chapterText;
	}

	public String plainText() 
	{
		// TODO Auto-generated method stub
		return null;
	}
}
