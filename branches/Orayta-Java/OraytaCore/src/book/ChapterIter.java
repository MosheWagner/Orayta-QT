package book;

import tree.TreeIter;
import tree.TreeNode;

public class ChapterIter extends TreeIter<TreeNode<IChapter>>
{

	//TODO: Implement
	
	public ChapterIter(TreeNode<TreeNode<IChapter>> treeNode) {
		super(treeNode);
		// TODO Auto-generated constructor stub
	}
	
	public boolean jumpTo(ChapterID chapter) {
		
		//TOTO: Use ChapterList.findById 
		
		// TODO Auto-generated method stub
		return false;
	}

}
