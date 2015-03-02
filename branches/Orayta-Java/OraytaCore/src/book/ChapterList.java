package book;

import java.util.TreeMap;

import book.ChapterID;

import tree.TreeNode;
import tree.TreeNodeIter;

public class ChapterList
{
	TreeNode<IChapter> chapTree; 
	TreeMap<ChapterID, TreeNode<IChapter>> chapMap = new TreeMap<ChapterID, TreeNode<IChapter>>();
	
	public void setChapterList(TreeNode<IChapter> chapterTree)
	{
		chapTree = chapterTree;
		
		//Put all in the hashmap:
		TreeNodeIter<IChapter> iter = (TreeNodeIter<IChapter>) chapTree.iterator();
		
		//Root element is empty
		if (iter.hasNext()) iter.next();
		
		while(iter.hasNext())
		{
			TreeNode<IChapter> subTree = iter.next();
			chapMap.put(subTree.data.getChapterAddress(), subTree);
		}
	}

	public TreeNode<IChapter> getChapterTree()
	{
		return chapTree;
	}
	
	public TreeNode<IChapter> findChapter(ChapterID address)
	{
		return chapMap.get(address);
	}

}
