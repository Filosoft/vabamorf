package ee.filosoft.vabamorf;

public class Morph {
	
	static public class MorphInfo {
		public String root;
		public String ending;
		public String clitic;
		public char pos;
		public String form;
	}

	public boolean guess;
	public boolean phon;

	public native boolean init(String fileName);
	public native void terminate();
	
	public native MorphInfo[] analyze(String word);
	public native String[] synthesize(MorphInfo info);

	protected void finalize() {
		terminate();
	}
	private long morphId;
	static {
		System.loadLibrary("morph_jni");
	}	
}
