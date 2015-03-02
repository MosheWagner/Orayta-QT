package tree;

import java.util.Collection;

public interface ITree<T> {
	public boolean isRoot();
	public boolean isLeaf();
	public int getLevel();
	
	public TreeNode<T> addChild(T child);
	public Collection<TreeNode<T>> getChildren();
	
	public void removeElement(TreeNode<T> node);

	public TreeNode<T> findTreeNode(Comparable<T> cmp);
}
