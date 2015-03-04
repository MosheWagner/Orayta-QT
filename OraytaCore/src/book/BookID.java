package book;

public class BookID implements Comparable<BookID>
{
	Integer UID = -1;
	String mDisplayName;

	public BookID(int id, String displayName)
	{
		UID = id;
		mDisplayName = displayName;
	}
	
	public String getDisplayName() {return mDisplayName; }
	
	public int getID() {return UID;}
	
	public int compareTo(BookID o) {
		return UID.compareTo(o.getID());
	}

	@Override
	public int hashCode() {
		return UID.hashCode();
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		BookID other = (BookID) obj;
		if (UID != other.UID)
			return false;
		return true;
	}
}
