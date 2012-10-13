package ory.fileTypes;

	import java.io.File;

import ory.Filename;
import ory.Main;


	public class UnknownFile extends Filename implements FileType {
		
		/**
		 * 
		 */
		private static final long serialVersionUID = 7581956382489397052L;
		private final String myType = "Unknown";

		public UnknownFile(String filename) {
			super(filename);
			setFileType(myType);
		}

		public UnknownFile(String path, String name) {
			super(path, name);
			setFileType(myType);
		}

		public UnknownFile(String path, String name, String extn) {
			super(path, name, extn);
			setFileType(myType);
		}

		public UnknownFile(Filename filename) {
			super(filename);
			setFileType(myType);
		}

		public UnknownFile(File file) {
			super(file);
			setFileType(myType);
		}

		public UnknownFile(File parent, String child) {
			super(parent, child);
			setFileType(myType);
		}

		public String[] convertToOrayta() {
			Main.ui.errorMessage("file type: " + this.getExtension() + " not supported\n" +
					"סוג הקובץ: " + this.getExtension() + " לא נתמך.");
			return null;
		}

	}


