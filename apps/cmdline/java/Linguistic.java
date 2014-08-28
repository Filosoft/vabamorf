package ee.filosoft.vabamorf;

public class Linguistic {
	
	static public class MorphInfo {
		public String root;
		public String ending;
		public String clitic;
		public char pos;
		public String form;
	}

	public boolean abbrevations = true;
	public boolean romanNumerals = true;
	public boolean guess = false;
	public boolean phonetic = false;

	public native boolean open(String fileName);
	public native void close();
	
	public native int spellWord(String word);
	public native int[] spellWords(String words[]);
	public String[] suggest(String word) { return suggest(word, false); }
	public native String[] suggest(String word, boolean startSentence);
	
	public native MorphInfo[] analyze(String word);
	public native MorphInfo[] synthesize(MorphInfo info, String hint);

	protected void finalize() {
		close();
	}
	private long lingId;
	static {
		System.loadLibrary("linguistic_jni");
	}	
}
