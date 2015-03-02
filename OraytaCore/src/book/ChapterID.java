package book;

public class ChapterID implements Comparable<ChapterID>
{
	private int mBookId;
	private String mUID;
	
	public ChapterID(int bookId, String id)
	{
		mBookId = bookId;
		mUID = id;
	}
	
	public int getBookID() {
		return mBookId;
	}

	public String getID() {
		return mUID;
	}
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + mBookId;
		result = prime * result + ((mUID == null) ? 0 : mUID.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		
		ChapterID other = (ChapterID) obj;
		if (mBookId != other.mBookId)
			return false;
		if (!mUID.equals(other.mUID))
			return false;
		
		return true;
	}
	
	public int hashcode()
	{
		 return mUID.hashCode();
	}
	
	public String toString()
	{
		 return "Book: " + mBookId + " - " + mUID;
	}

	public int compareTo(ChapterID o) 
	{
		return mUID.compareTo(o.getID());
	}
}
