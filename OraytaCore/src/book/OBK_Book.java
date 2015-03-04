package book;

import fileManager.ZipReader;
import search.ISearchQuery;
import search.ISearchResults;

public class OBK_Book extends ABook
{
	public OBK_Book(String path)
	{
		mFilePath = path ;

		String zipComment = ZipReader.readComment(mFilePath);
		
		parseSttings(zipComment);

		mDisplayName = mBookSettings.get("DisplayName");
		
		int id = -1;
		try { id = Integer.parseInt(mBookSettings.get("UniqueId")); }
		catch (NumberFormatException e) {}
		
		mID = new BookID(id, mDisplayName);
	}
	
	public boolean buildContents()
	{
		//TODO:
		return false;
	}

	@Override
	public ChapterTree getChapterList() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public ISearchResults search(ISearchQuery query) {
		// TODO Auto-generated method stub
		return null;
	}

}
