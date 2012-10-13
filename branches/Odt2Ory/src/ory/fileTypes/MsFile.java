package ory.fileTypes;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.InputStreamReader;

import com.artofsolving.jodconverter.DocumentConverter;
import com.artofsolving.jodconverter.openoffice.connection.OpenOfficeConnection;
import com.artofsolving.jodconverter.openoffice.connection.SocketOpenOfficeConnection;
import com.artofsolving.jodconverter.openoffice.converter.OpenOfficeDocumentConverter;

import ory.Filename;
import ory.Main;


public class MsFile extends Filename implements FileType {
	
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -6569982913598712511L;
	private final String myType = "Doc";

	public MsFile(String filename) {
		super(filename);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public MsFile(String path, String name) {
		super(path, name);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public MsFile(String path, String name, String extn) {
		super(path, name, extn);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public MsFile(Filename filename) {
		super(filename);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public MsFile(File file) {
		super(file);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public MsFile(File parent, String child) {
		super(parent, child);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public String[] convertToOrayta() throws FileNotFoundException, Exception {
		OdtFile odtFile = doc2odt(this);
		return odtFile.convertToOrayta();
		
	}

	/**
	 * converts a file from supported formats to odt, using jodconverter.
	 * @param input - input file.
	 * @return the converted odt file.
	 */
	private  OdtFile doc2odt(Filename input) {
		OdtFile output = new OdtFile(input.getFullPath(), input.getBaseName(), "odt");
		output.deleteOnExit();

		if (Main.parameters.isUseUnoconv()) {
			return unoConvert(input, output);
		}
		else {
			return jodConvert(input, output);
		}

			
	}

		private OdtFile jodConvert(Filename input, OdtFile output) {
			OpenOfficeConnection connection = new SocketOpenOfficeConnection(8100);
			
			
			try {
//				try {
					connection.connect();
//				} catch (ConnectException e) {
				/* commented because this doesnt work.
				  	String[] command = { "soffice", "-headless",
				 
							"-accept=\"socket,host=127.0.0.1,port=8100;urp;\"",
							"-nofirststartwizard" };
					Process p = Runtime.getRuntime().exec(command);
						{
							String forLog = "";
							for (String s : command){
								forLog += (s + " ");
							}
							log(forLog);
						}
					p.waitFor();
					log("wating...");
//					p.wait(2000);
//					log("done.");
						wait(2000);
					connection.connect();
					*/
//				}

			} catch (Exception e) {

				StringBuffer message = new StringBuffer();
				message.append(
						"we have encountered an error during connection to openoffice (or libreoffice)\n");
				message.append(
						"אירעה שגיאה בעת התחברות לתכנה אופן אופיס או ליברה אופיס\n");
				message.append(
						"כדי ליצור חיבור לאופן אופיס יש לפתוח פורט ל8100. בלינוקס ניתן באמצאות השורה הבאה:\n");
				message.append(
						"soffice --headless --accept=\"socket,host=127.0.0.1,port=8100;urp;\" --nofirststartwizard\n");
				

				Main.ui.errorMessage(message.toString(), e);
				return null;

			}
			
		try {
			try {
				DocumentConverter converter = 
					new OpenOfficeDocumentConverter(connection);
				converter.convert(input, output);
			} catch (Exception e) {

				StringBuffer message = new StringBuffer();
				message.append("we have encountered an error during conversion to odt format\n");
				message.append("אירעה שגיאה בעת המרת קובץ לפורמט אופן אופיס\n");

				Main.ui.errorMessage(message.toString(), e);
				return null;
			}
		} finally {
			connection.disconnect();
		}
		return output;
		}

		private OdtFile unoConvert(Filename input, OdtFile output) {
//			Filename output = new Filename(input.getFullPath(), input.getBaseName(), "odt");
			BufferedReader reader;
			StringBuffer unoconvLog= new StringBuffer();
//			output.deleteOnExit();
			try{
				String[] command = {"unoconv", "-f", "odt", input.getFilename()};
				Process p = Runtime.getRuntime().exec(command);
				Main.ui.log(command.toString());
				p.waitFor();
//				reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
				reader = new BufferedReader(new InputStreamReader(p.getErrorStream()));
//				System.out.println("unoconv end");
				
				String line=reader.readLine(); 
				while(line!=null) 
				{ 
					unoconvLog.append(line + "\n");
//					System.out.println(line);
					line=reader.readLine(); 
					
				}

				if (!output.exists()){
					throw new Exception("unoconv error");
				}
				return output;
			} catch (Exception e) {
				StringBuffer message = new StringBuffer();
				message.append("we have encountered an error during conversion to odt format\n");
				message.append("אירעה שגיאה בעת המרת קובץ לפורמט אופן אופיס\n");
				message.append("וודא שunoconv מותקן ושopenoffice פתוח\n");
				
				message.append(
				"כדי ליצור חיבור לאופן אופיס יש לפתוח פורט ל2002. בלינוקס ניתן באמצאות השורה הבאה:\n");
		message.append(
				"soffice --headless --accept=\"socket,host=127.0.0.1,port=2002;urp;\" --nofirststartwizard\n");
				
				message.append(unoconvLog);
							
				Main.ui.errorMessage(message.toString(), e);
				return null;
			}
			
		} 
}
