package book;

import java.util.TreeMap;

import book.ChapterID;

import tree.TreeNode;

public class ChapterTree extends TreeNode<IChapter>
{
	TreeMap<ChapterID, TreeNode<IChapter>> chapMap = new TreeMap<ChapterID, TreeNode<IChapter>>();
	
	public ChapterTree(IChapter data) 
	{
		super(data);
	}

	@Override
	public TreeNode<IChapter> addChild(IChapter child) 
	{
		TreeNode<IChapter> childNode = super.addChild(child);
		addToSearchMap(childNode);
		return childNode;
	}
	
	private void addToSearchMap(TreeNode<IChapter> node) 
	{
		if (node.data == null) return;
		
		chapMap.put(node.data.getChapterAddress(), node);
		if (parent != null)
		{
			((ChapterTree) parent).addToSearchMap(node);
		}
	}


	public TreeNode<IChapter> findBook(ChapterID address)
	{
		return chapMap.get(address);
	}

}
