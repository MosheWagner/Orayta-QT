package ory.fileTypes;

import java.io.IOException;

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.xml.sax.SAXException;

import ory.HtmlToOryExtractor;
import ory.Main;
import ory.OryConfFile;
import ory.OryFile;
import ory.OryFiles;

public class DaatHtmlToOryExtractor extends HtmlToOryExtractor {

	public DaatHtmlToOryExtractor(HtmlFile file) throws SAXException,
			IOException {
		super(file);
		// TODO Auto-generated constructor stub
	}
	protected void appendElementText(Node node){
		try {
			if (node.getNodeName().equalsIgnoreCase("div")) {
				appendElementText((Element) node);
//				Main.ui.dbgLog("processed a div node");
			} 
			else if (node.getNodeName().equalsIgnoreCase("title")) {
				Main.ui.makeParentConf(myFilename, node.getTextContent().replaceAll("\\s+", " "));
			}
		}
		catch (Exception e) {
			ory.Main.ui.errorMessage("error at daat conversion", e);
		}
		
		
		super.appendElementText(node);
	}
	
	protected void appendElementText(Element node) {
		if (node.hasAttribute("id") &&
		node.getAttribute("id").equalsIgnoreCase("name")){
			bookTitle = node.getTextContent().replaceAll("\\s+", " "); 
		}
//		Main.ui.dbgLog("div>>" +node.getTextContent() );
//		appendElementText((Node)node);
	}
	
	public static OryFiles walkThrough(HtmlFile file) throws Exception {
		DaatHtmlToOryExtractor extractor = new DaatHtmlToOryExtractor(file) ;
		extractor.getText();
		
		OryFiles generatedFiles = extractor.getFiles();		
		
		for (OryFile oryFile : generatedFiles.getFileList()){
			OryConfFile conf = oryFile.getMyConf();
			conf.createLink(OryFile.getOraytaDir());
//			OryLinkFile link = new OryLinkFile(conf.getFilename());
//			link.setLocation(new ory.Filename (OryFile.getOraytaDir()));
			
		}
		
		return generatedFiles;
	}
	

}
