package ory.fileTypes;

	import java.io.File;

import ory.Filename;

	public class DocxFile extends MsFile implements FileType {
		
		/**
		 * 
		 */
		private static final long serialVersionUID = -8451137533828604318L;
		private final String myType = "Docx";

		public DocxFile(String filename) {
			super(filename);
			setFileType(myType);
			// TODO Auto-generated constructor stub
		}

		public DocxFile(String path, String name) {
			super(path, name);
			setFileType(myType);
			// TODO Auto-generated constructor stub
		}

		public DocxFile(String path, String name, String extn) {
			super(path, name, extn);
			setFileType(myType);
			// TODO Auto-generated constructor stub
		}

		public DocxFile(Filename filename) {
			super(filename);
			setFileType(myType);
			// TODO Auto-generated constructor stub
		}

		public DocxFile(File file) {
			super(file);
			setFileType(myType);
			// TODO Auto-generated constructor stub
		}

		public DocxFile(File parent, String child) {
			super(parent, child);
			setFileType(myType);
			// TODO Auto-generated constructor stub
		}

	}


