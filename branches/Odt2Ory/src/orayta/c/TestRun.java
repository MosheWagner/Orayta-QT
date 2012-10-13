package orayta.c;

import ory.Filename;

public class TestRun {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		String infile = "/home/avi/desktop/a01_Genesis.txt";
		String outDB = "/home/avi/desktop/a01_Genesis.TDB";
		String outLMP = "/home/avi/desktop/a01_Genesis.LMP";
		
		DB.autoGenerateSearchTextDB(new Filename(infile), outDB, outLMP);
		
	}

}
