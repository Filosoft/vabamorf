/*
* verbi paradigmade tegemine lk. 50-51 alusel
* return(1), kui OK; return(0), kui mingi viga
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../dct/lib/dct-nomvrb/noomen.h"

extern tyvestr tyvi[];     /* kõik antud sõna tüved */
extern int    tyybinr;     /* muuttüübi nr (27-38)   */

int vteepar( void )
    {
    int r;

    switch ( tyybinr )
	    {

	    case 27:	 /* ela[ma V 27 */
	        r = par27();
	        break;

	    case 28:         /* leppi[ma lepi[b V 28 */
	        r = par28();
	        break;

	    case 29:         /* hüppa[ma hüpa[ta V 29 */
	        r = par29();
	        break;

	    case 30:         /* riidle[ma riiel[da V 30 */
	        r = par30();
	        break;

	    case 31:         /* rabele[ma rabel[da v 31 */
	        r = par31();
	        break;

	    case 32:         /* seis[ma seisa[b V 32 */
	        r = par32();
	        break;

	    case 33:         /* naer[ma naera[b V 33 */
	        r = par33();
	        break;

	    case 34:         /* sööt[ma sööda[b sööde[tud V 34 */
	        r = par34();
	        break;

	    case 35:         /* nut[ma nuta[b nute[tud nutt[a V 35*/
	        r = par35();
	        break;

	    case 36:         /* tule[ma tul[dud tul[gu tull[a tull[akse V 36 */
	        r = par36();
	        break;

	    case 37:         /* või[ma või[gu V 37 */
	        r = par37();
	        break;

	    case 38:         /* söö[ma söö[gu süü[a süü[akse sõ[i sõ[ime V 38 */
	        r = par38();
	        break;

	    default:
	        return(0);          /* "võimatu" nr */
	    }
    if ( !r )
	    return(0);
    return(1);
    }

