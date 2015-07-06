package ee.filosoft.vabamorf;

public class Disambiguator {

/**
* Initiates disambiguator.
* @param fileName Path to lexicon file.
* @return true on success
*/
	public native boolean open(String fileName);

/**
* De-initiates linguistic and frees up resources.
*/
	public native void close();
	

/**
* Disambiguates morphological analysis of a sentence
* @param analysis Morphological analysis of full sentense.
* @return Disambiguated analysis.
*/
	public native MorphInfos[] disambiguate(MorphInfos[] analysis);

	protected void finalize() {
		close();
	}
	private long objId;
	static {
		System.loadLibrary("disambiguator_jni");
	}	
}
