package tree;
import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Spliterator;
import java.util.function.Consumer;

// http://stackoverflow.com/questions/3522454/java-tree-data-structure
// http://sujitpal.blogspot.co.il/2006/05/java-data-structure-generic-tree.html

public class TreeNode<T> implements ITree<T>, Iterable<TreeNode<T>> {

        public T data;
        public TreeNode<T> parent;
        public List<TreeNode<T>> children;

        private List<TreeNode<T>> elementsIndex;
        
        public boolean isRoot() {
                return parent == null;
        }

        public boolean isLeaf() {
                return children.size() == 0;
        }

        public TreeNode(T data) {
                this.data = data;
                this.children = new LinkedList<TreeNode<T>>();
                this.elementsIndex = new LinkedList<TreeNode<T>>();
                this.elementsIndex.add(this);
        }

        public TreeNode<T> addChild(T child) {
                TreeNode<T> childNode = new TreeNode<T>(child);
                childNode.parent = this;
                this.children.add(childNode);
                this.registerChildForSearch(childNode);
                return childNode;
        }

        public int getLevel() {
                if (this.isRoot())
                        return 0;
                else
                        return parent.getLevel() + 1;
        }

        private void registerChildForSearch(TreeNode<T> node) 
        {
                elementsIndex.add(node);
                if (parent != null)
                        parent.registerChildForSearch(node);
        }
        
		private void unRegisterChildFromSearch(TreeNode<T> node) 
		{
			elementsIndex.remove(node);
		}

        public TreeNode<T> findTreeNode(Comparable<T> cmp) {
                for (TreeNode<T> element : this.elementsIndex) {
                        T elData = element.data;
                        if (cmp.compareTo(elData) == 0)
                                return element;
                }

                return null;
        }

        @Override
        public String toString() {
                return data != null ? data.toString() : "[data null]";
        }

        public Iterator<TreeNode<T>> iterator() {
                TreeNodeIter<T> iter = new TreeNodeIter<T>(this);
                return iter;
        }

		public Collection<TreeNode<T>> getChildren() {
			return children;
		}

		/*
		 * Remove the given node from the tree. 
		 *  Obviously, it must be a child (to some level) of this branch, and not the root!
		 */
		public void removeElement(TreeNode<T> node) 
		{
			//Here I want '==' and not 'equals'
			if (node == this) return;
			
			TreeNode<T> parentNode = node.parent;
			parentNode.children.remove(node);
			parentNode.unRegisterChildFromSearch(node);
		}

		public void forEach(Consumer<? super TreeNode<T>> action) {
			// TODO Auto-generated method stub
			
		}

		public Spliterator<TreeNode<T>> spliterator() {
			// TODO Auto-generated method stub
			return null;
		}

}