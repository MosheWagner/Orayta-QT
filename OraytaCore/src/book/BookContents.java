package book;

import tree.IDSearcher;
import tree.TreeNode;

public class BookContents 
{
	private TreeNode<ChapterID> chapterIDTree;
	private TreeNode<IChapter> chapterContentsTree;
	private IDSearcher<IChapter> chapterContentsIndex;
	
	public TreeNode<ChapterID> getChapterIDTree() {
		return chapterIDTree;
	}
	public void setChapterIDTree(TreeNode<ChapterID> chapterIDTree) {
		this.chapterIDTree = chapterIDTree;
	}
	
	public TreeNode<IChapter> getChapterByID(ChapterID id)
	{
		return chapterContentsIndex.findById(id.getUID());
	}

	public TreeNode<IChapter> getChapterContentsTree() {
		return chapterContentsTree;
	}
	public void setChapterContentsTree(TreeNode<IChapter> chapterContentsTree) {
		this.chapterContentsTree = chapterContentsTree;
		
		chapterContentsIndex = new IDSearcher<IChapter>(this.chapterContentsTree);
	}

}
