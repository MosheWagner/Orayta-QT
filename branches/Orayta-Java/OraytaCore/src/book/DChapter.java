package book;

public class DChapter implements IChapter {

	ChapterID mAddress;
	String mChapterText = "";
	
	public DChapter() {}
	
	public DChapter(ChapterID address, String text)
	{
		mAddress = address;
		mChapterText = text;
	}
	
	public void setAddress(ChapterID address) { mAddress=address; }
	public void setChapterText(String chapterText) { mChapterText=chapterText; }


	public ChapterID getChapterAddress() {
		return mAddress;
	}

	public boolean isAtom() {
		return (!text().equals(""));
	}

	public String text() {
		return mChapterText;
	}

	public String plainText() 
	{
		// TODO Auto-generated method stub
		return null;
	}

	public String getUID() {
		return mAddress.getUID();
	}

	@Override
	public String toString() {
		return "[mAddress=" + mAddress + ", mChapterText=" + mChapterText + "]";
	}
	
	
}
