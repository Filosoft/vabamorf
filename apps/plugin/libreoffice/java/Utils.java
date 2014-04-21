package ee.vabamorf.ooo.linguistic.et;

import com.sun.star.lib.uno.helper.ComponentBase;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.AnyConverter;
import com.sun.star.beans.XPropertySet;
import com.sun.star.beans.PropertyValue;
import com.sun.star.lang.Locale;
import com.sun.star.uno.XComponentContext;
import com.sun.star.ucb.XFileIdentifierConverter;

public class Utils {
    static boolean isEqual( Locale aLoc1, Locale aLoc2 )
    {
        return aLoc1.Language.equals( aLoc2.Language ) &&
               aLoc1.Country .equals( aLoc2.Country )  &&
               aLoc1.Variant .equals( aLoc2.Variant );  
    }

	synchronized static void log(String sText)
	{
		// System.err.println(sText);
	}

    static boolean getPropValue( 
			PropChgHelper	aPropChgHelper,
            String          aPropName,
            boolean         bDefaultVal,
            PropertyValue[] aProps )
    {
        boolean bRes = bDefaultVal;

        try
        {
            for (int i = 0;  i < aProps.length;  ++i) {
                if (aPropName.equals( aProps[i].Name )) {
                    Object aObj = aProps[i].Value;
                    if (AnyConverter.isBoolean( aObj )) {
                        bRes = AnyConverter.toBoolean( aObj );
                        return bRes;
                    }
                }
            }

            XPropertySet xPropSet = aPropChgHelper.GetPropSet();
            if (xPropSet != null) {
                Object aObj = xPropSet.getPropertyValue( aPropName );
                if (AnyConverter.isBoolean( aObj ))
                    bRes = AnyConverter.toBoolean( aObj );
            }
        }
        catch (Exception e) {
            bRes = bDefaultVal;
        }
        
        return bRes;
    }
	
    static short getPropValue(
			PropChgHelper	aPropChgHelper,
            String          aPropName,
            short           nDefaultVal,
            PropertyValue[] aProps )
    {
        short nRes = nDefaultVal;

        try
        {
            for (int i = 0;  i < aProps.length;  ++i) {
                if (aPropName.equals( aProps[i].Name )) {
                    Object aObj = aProps[i].Value;
                    if (AnyConverter.isShort( aObj )) {
                        nRes = AnyConverter.toShort( aObj );
                        return nRes;
                    }
                }
            }

            XPropertySet xPropSet = aPropChgHelper.GetPropSet();
            if (xPropSet != null) {
				Object aObj = xPropSet.getPropertyValue( aPropName );
				if (AnyConverter.isShort( aObj ))
					nRes = AnyConverter.toShort( aObj );
            }
        }
        catch (Exception e) {
            nRes = nDefaultVal;
        }
        
        return nRes;
    }

	private static String readRegistry(XComponentContext xComponentContext, String path, String key)
	{
		try {
			Object configProvider = xComponentContext.getServiceManager().createInstanceWithContext( "com.sun.star.configuration.ConfigurationProvider", xComponentContext );
			if (configProvider != null) {
				com.sun.star.lang.XMultiServiceFactory xConfigProvider = (com.sun.star.lang.XMultiServiceFactory) UnoRuntime.queryInterface(com.sun.star.lang.XMultiServiceFactory.class, configProvider);
			
				com.sun.star.beans.PropertyValue[] lParams = new com.sun.star.beans.PropertyValue[1];
				lParams[0] = new com.sun.star.beans.PropertyValue();
				lParams[0].Name = "nodepath";
				lParams[0].Value = path;

				Object configAccess  = xConfigProvider.createInstanceWithArguments( "com.sun.star.configuration.ConfigurationAccess", lParams );
				if (configAccess  != null) {
					com.sun.star.container.XNameAccess xNameAccess = (com.sun.star.container.XNameAccess)UnoRuntime.queryInterface(com.sun.star.container.XNameAccess.class, configAccess );
					return xNameAccess.getByName( key ).toString();
				}
			}
		} catch (Throwable t) { }
		return "";
	}
	
	static String getUILanguage(XComponentContext xComponentContext)
	{
		String result = readRegistry( xComponentContext, "/org.openoffice.Office.Linguistic/General", "UILocale" );
		if (result.length() == 0) {
			result = readRegistry( xComponentContext, "/org.openoffice.Setup/L10N", "ooLocale" );
		}
		return result;
	}
	
	private static String getComponentPath(XComponentContext xComponentContext)
		throws com.sun.star.uno.Exception
	{
		String sURL = com.sun.star.deployment.PackageInformationProvider.get(xComponentContext).getPackageLocation("ee.vabamorf.ooo.linguistic.et.Extension");
		Object obj = xComponentContext.getServiceManager().createInstanceWithContext("com.sun.star.ucb.FileContentProvider", xComponentContext);
		if (obj != null) {
			XFileIdentifierConverter xFileConverter = (XFileIdentifierConverter) UnoRuntime.queryInterface(XFileIdentifierConverter.class, obj);
			return xFileConverter.getSystemPathFromFileURL(sURL);
		}
		return null;
	}

	private static String getPlatform() {
		String osArch = System.getProperty("os.arch").toLowerCase();
		if (osArch.endsWith("86")) {
			osArch = "x86";
		} else if (osArch.endsWith("64")) {
			osArch = "x64";
		}

	    String osName = System.getProperty("os.name").toLowerCase();
	    if (osName.startsWith("mac os x")) {
			return ".mac";
		} else if (osName.startsWith("windows")) {
			return ".win_" + osArch;
		} else if (osName.startsWith("linux")) {
			return ".lin_" + osArch;
		}

	    return ".unk";
	}

	synchronized static Linguistic initLinguistic(XComponentContext xComponentContext, boolean bReserved)
	{
		try {
			String sComponentPath = Utils.getComponentPath(xComponentContext);

			String sJNIFile = sComponentPath + "/ooovmlinget" + getPlatform();
			System.load(sJNIFile);

			Linguistic aLinguistic = new Linguistic();
			if (aLinguistic.init(sComponentPath+"/et.dct", bReserved)) {
				return aLinguistic;
			}
		} catch (Throwable t) { }
		return null;
	}

}
