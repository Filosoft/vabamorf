package ee.vabamorf.ooo.linguistic.et;

public class Linguistic {
	
	native boolean init(String fileName, boolean bReserved);
	native void terminate();
	
	native boolean spell(String sWord, boolean bIsSpellWithDigits, boolean bIsSpellUpperCase, boolean bIsSpellCapitalization);
	native String[] suggest(String sWord);

/*	public boolean init(String fileName, boolean bThesaurus) { return true; }
	public void terminate() { }
	
	public boolean spell(String sWord, boolean bIsSpellWithDigits, boolean bIsSpellUpperCase, boolean bIsSpellCapitalization) { return sWord.equals("vanaema"); }
	public String[] suggest(String sWord) { String aResult[]= { "vanaema", "vananema" }; return aResult; }
*/

	protected void finalize() {
		terminate();
	}
	private long lingId;
}
