package book;

import tree.TreeNode;

public interface IBookMetaData 
{
	public String getCSS();
	public String getCopyRight();
	public TreeNode<IChapter> chapterTree();
}
