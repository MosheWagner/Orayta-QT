package ory.fileTypes;

	import java.io.File;

import ory.Filename;

	public class WpdFile extends MsFile implements FileType {
		
		/**
		 * 
		 */
		private static final long serialVersionUID = -7445062684878379883L;
		/**
		 * 
		 */
		private final String myType = "Wpd";

		public WpdFile(String filename) {
			super(filename);
			setFileType(myType);
			// TODO Auto-generated constructor stub
		}

		public WpdFile(String path, String name) {
			super(path, name);
			setFileType(myType);
			// TODO Auto-generated constructor stub
		}

		public WpdFile(String path, String name, String extn) {
			super(path, name, extn);
			setFileType(myType);
			// TODO Auto-generated constructor stub
		}

		public WpdFile(Filename filename) {
			super(filename);
			setFileType(myType);
			// TODO Auto-generated constructor stub
		}

		public WpdFile(File file) {
			super(file);
			setFileType(myType);
			// TODO Auto-generated constructor stub
		}

		public WpdFile(File parent, String child) {
			super(parent, child);
			setFileType(myType);
			// TODO Auto-generated constructor stub
		}

	}


