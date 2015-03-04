package book;

import java.io.FileNotFoundException;
import java.io.IOException;

import fileManager.SimpleFileReader;

import search.ISearchQuery;
import search.ISearchResults;

public class BookFolder extends ABook
{
	public BookFolder(String path)
	{
		mDisplayName = "";
		
		try {
			String s = new SimpleFileReader().readContents(path + ".folder");
			parseSttings(s);
			
			mDisplayName = mBookSettings.get("BranchName");
		} 
		//TODO: Better handling
		catch (FileNotFoundException e) {
			System.out.println("not found: " + path + ".folder");
			// TODO Auto-generated catch block
			//e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		

		mFilePath = path;
		mID = new BookID(-1, mDisplayName);
	}
	
	public BookFolder(String path, String displayName)
	{
		mDisplayName = displayName;
		mFilePath = path;
		mID = new BookID(-1, mDisplayName);
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
