package ee.vabamorf.ooo.linguistic.et;

// uno
import com.sun.star.lib.uno.helper.ComponentBase;
import com.sun.star.uno.UnoRuntime;

// factories
import com.sun.star.lang.XSingleComponentFactory;
import com.sun.star.lang.XMultiComponentFactory;

// supported Interfaces
import com.sun.star.linguistic2.XSpellChecker;
import com.sun.star.linguistic2.XLinguServiceEventBroadcaster;
import com.sun.star.lang.XInitialization;		
import com.sun.star.lang.XComponent;
import com.sun.star.lang.XServiceInfo;		
import com.sun.star.lang.XServiceDisplayName;		

//used Interfaces
import com.sun.star.linguistic2.XLinguServiceEventListener;
import com.sun.star.linguistic2.XSpellAlternatives;
import com.sun.star.linguistic2.SpellFailure;

import com.sun.star.lang.Locale;
import com.sun.star.lang.XEventListener;
import com.sun.star.lang.EventObject;
import com.sun.star.lang.XTypeProvider;

import com.sun.star.beans.XPropertySet;
import com.sun.star.beans.PropertyValue;

import com.sun.star.uno.XInterface;
import com.sun.star.uno.Type;
import com.sun.star.uno.XComponentContext;

import java.util.ArrayList;

public class EstonianSpellChecker extends ComponentBase implements
        XSpellChecker,
        XLinguServiceEventBroadcaster,
        XInitialization,
        XServiceDisplayName,
        XServiceInfo
{
    PropChgHelper_Spell         aPropChgHelper;
    ArrayList                   aEvtListeners;
	Linguistic 					aLinguistic;
	XComponentContext			aContext;
	
    public EstonianSpellChecker(XComponentContext xComponentContext)
    {
		Utils.log("EstonianSpellChecker.EstonianSpellChecker");
        // names of relevant properties to be used
        String[] aProps = new String[]
            {
                "IsIgnoreControlCharacters",
                "IsUseDictionaryList",
                "IsGermanPreReform",
                "IsSpellUpperCase",
                "IsSpellWithDigits",
                "IsSpellCapitalization"
            };
		aContext		= xComponentContext;
        aPropChgHelper  = new PropChgHelper_Spell( (XSpellChecker) this, aProps );
        aEvtListeners   = new ArrayList();
		aLinguistic 	= null;
    }

    // __________ interface methods __________
    
    //****************
    // XInitialization
    //****************
    public synchronized void initialize( Object[] aArguments ) 
        throws com.sun.star.uno.Exception,
               com.sun.star.uno.RuntimeException
    {
		Utils.log("EstonianSpellChecker.initialize");
		XPropertySet xPropSet = null;
        if (aArguments.length == 2) {
            xPropSet = (XPropertySet)UnoRuntime.queryInterface(XPropertySet.class, aArguments[0]);
		} else {
			try {
				XMultiComponentFactory xMultiFactory = aContext.getServiceManager();
				xPropSet = (XPropertySet) UnoRuntime.queryInterface(
					XPropertySet.class,
					xMultiFactory.createInstanceWithContext("com.sun.star.linguistic2.LinguProperties", aContext)
				);
			} catch (Exception e) {
				return;
			}
		}
		if (xPropSet != null) {
			aPropChgHelper.AddAsListenerTo( xPropSet );
		}
    }

    //*****************
    //XSupportedLocales
    //*****************
    public Locale[] getLocales()
        throws com.sun.star.uno.RuntimeException
    {
		return new Locale[] { new Locale( "et", "EE", "" ) };
    }
    
    public boolean hasLocale( Locale aLocale ) 
        throws com.sun.star.uno.RuntimeException
    {
		Locale[] aSupportedLocales = getLocales();
		return ( aSupportedLocales.length == 1 && Utils.isEqual( aLocale, aSupportedLocales[0] ));
    }

    //*************
    //XSpellChecker
    //*************
    public synchronized boolean isValid(
            String aWord, Locale aLocale,
            PropertyValue[] aProperties ) 
        throws com.sun.star.uno.RuntimeException,
               com.sun.star.lang.IllegalArgumentException
    {
		Utils.log("EstonianSpellChecker.isValid");
        if ( !hasLocale( aLocale ) || aWord.length() == 0 ) {
            return true;
		}

		if (aLinguistic == null) {
			aLinguistic = Utils.initLinguistic(aContext, false);
			if (aLinguistic == null) {
				return true;
			}
		}

        boolean bIsSpellWithDigits      = Utils.getPropValue( aPropChgHelper, "IsSpellWithDigits", false, aProperties );
        boolean bIsSpellUpperCase       = Utils.getPropValue( aPropChgHelper, "IsSpellUpperCase", false, aProperties );
        boolean bIsSpellCapitalization  = Utils.getPropValue( aPropChgHelper, "IsSpellCapitalization", true, aProperties );

		Utils.log("  aWord='" + aWord + "' bIsSpellWithDigits=" + bIsSpellWithDigits + " bIsSpellUpperCase=" + bIsSpellUpperCase + " bIsSpellCapitalization=" + bIsSpellCapitalization);
		return aLinguistic.spell(aWord, bIsSpellWithDigits, bIsSpellUpperCase, bIsSpellCapitalization);
    }
    
    
    public synchronized XSpellAlternatives spell(
            String aWord, Locale aLocale,
            PropertyValue[] aProperties ) 
        throws com.sun.star.uno.RuntimeException,
               com.sun.star.lang.IllegalArgumentException
    {
		Utils.log("EstonianSpellChecker.spell");
        if ( !hasLocale( aLocale ) || aWord.length() == 0 ) {
            return null;
		}
        
		if (aLinguistic == null) {
			aLinguistic = Utils.initLinguistic(aContext, false);
			if (aLinguistic == null) {
				return null;
			}
		}

        XSpellAlternatives xRes = null;
        if (!isValid( aWord, aLocale, aProperties ))
        {
			String []aSuggestions = aLinguistic.suggest(aWord);
			xRes = new XSpellAlternatives_impl( aWord, aLocale, SpellFailure.SPELLING_ERROR, aSuggestions );
        }
        return xRes;
    }
    
    
    //*****************************
    //XLinguServiceEventBroadcaster
    //*****************************
    public synchronized boolean addLinguServiceEventListener (
            XLinguServiceEventListener xLstnr )
        throws com.sun.star.uno.RuntimeException
    {
		Utils.log("EstonianSpellChecker.addLinguServiceEventListener");
        boolean bRes = false;   
        if (!(bInDispose || bDisposed) && xLstnr != null)
            bRes = aPropChgHelper.addLinguServiceEventListener( xLstnr );
        return bRes;
    }
    
    public synchronized boolean removeLinguServiceEventListener(
            XLinguServiceEventListener xLstnr ) 
        throws com.sun.star.uno.RuntimeException
    {
		Utils.log("EstonianSpellChecker.removeLinguServiceEventListener");
        boolean bRes = false;   
        if (!bDisposed && xLstnr != null)
            bRes = aPropChgHelper.removeLinguServiceEventListener( xLstnr );
        return bRes;
    }            

    //********************
    // XServiceDisplayName
    //********************
    public String getServiceDisplayName( Locale aLocale ) 
        throws com.sun.star.uno.RuntimeException
    {
		if (Utils.isEqual( aLocale, new Locale( "et", "EE", "" ) )) {
			return "Eesti keele speller (Vabamorf)";
		} else {
			return "Estonian SpellChecker (Openmorph)";
		}
    }

    //*************
    // XServiceInfo
    //*************
    public boolean supportsService( String aServiceName )
        throws com.sun.star.uno.RuntimeException
    {
        String[] aServices = getSupportedServiceNames_Static();
		for (int i = 0; i < aServices.length; i++) {
			if (aServiceName.equals( aServices[ i ] )) return true;
		}
		return false;
    }

    public String getImplementationName()
        throws com.sun.star.uno.RuntimeException
    {
        return getImplementationName_Static();
    }
        
    public String[] getSupportedServiceNames()
        throws com.sun.star.uno.RuntimeException
    {
        return getSupportedServiceNames_Static();
    }
    
    // __________ static things __________

    public static String getImplementationName_Static()
	{
		String aResult = "ee.vabamorf.ooo.linguistic.et.SpellChecker";
		return aResult;
	}
    
    public static String[] getSupportedServiceNames_Static()
    {
        String[] aResult = { "com.sun.star.linguistic2.SpellChecker" };
        return aResult;
    }

    public static XSingleComponentFactory __getComponentFactory(String sImplName)
	{
        if ( sImplName.equals( getImplementationName_Static() ) ) {
            return com.sun.star.lib.uno.helper.Factory.createComponentFactory(
				EstonianSpellChecker.class,
                getSupportedServiceNames_Static()
			);
        }
        return null;
    }

    public static boolean __writeRegistryServiceInfo( 
            com.sun.star.registry.XRegistryKey xRegKey )
    {
        return com.sun.star.lib.uno.helper.Factory.writeRegistryServiceInfo(
			getImplementationName_Static(),
			getSupportedServiceNames_Static(),
			xRegKey
		); 
    }
}
