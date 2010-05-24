/*
The contents of this file are subject to the Mozilla Public
License Version 1.1 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of
the License at http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS
IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
implied. See the License for the specific language governing
rights and limitations under the License.

fehmozexRunProgram and fehmozexGetEnvironment are taken from MozEx (and modified).  
MozEx is Copyright (C) 2003 Tomas Styblo <tripie@cpan.org>
the stuff to read link-handlers.js cargo culted from AdBlock plus
The rest is by Larry D'Anna and are Copyright (C) 2007 Larry D'Anna <larry@elder-gods.org>

*/

function fehError (msg) { 
    alert(msg);
}

function fehmozexGetEnvironment(key) {
    var res="";

    try {
        var pr = Components.classes["@mozilla.org/process/util;1"].
            createInstance(Components.interfaces.nsIProcess);
        var env = Components.classes["@mozilla.org/process/environment;1"];

        if (env != null)
        {
            env = env.createInstance(Components.interfaces.nsIEnvironment);
            res = env.get(key);
        }
        else
        {
            // Hack for mozilla 1.4
            res = pr.getEnvironment(key);
        }
    } 
    catch (e)
    {
        // do nothing
    }
    return res;
}

function fehmozexRunProgram(cmd, esc) {
    if (cmd == null || cmd.length == 0) {
        return false; // no command is set
    }
    var args = new Array();

    // Match either
    //  a) apos. followed by anything till apos. followed by space or end of string
    //     - escape apos. by backslash, if it is not supposed to be part of the string
    //  b) space delimited word
    //     - escape space by backslash, if it is not supposed to be part of the string
    // No other escaping is implemented, because it would make writing windows-like
    //   paths hard
    //
    // Use debugging facility to see what is the result if you are not sure how
    // to use this
    scmd=cmd.match(/"(\"|.)*?"(?=\s|$)|(\\\s|\S)+/g);
    for (var i in scmd)
    {
        // If it is apos. delimited
        if (scmd[i].slice(0,1)=='"')
        {
            // Remove quotes on the begin and end
            scmd[i]=scmd[i].replace(/^"(.*?)"$/, "$1");
            // De-escape apostroph
	    // \x22 is a " but the " is fucking up my emacs colors
            scmd[i]=scmd[i].replace(/\\\x22/g, "\x22");
        } else {
            // De-escape spaces
            scmd[i]=scmd[i].replace(/\\(\s)/g, "$1");
        }
    }

    var executable = scmd.shift();
    if (executable.length == 0) {
        fehError("no executable in command");
        return false;
    }
    
    for (var i = 0; i < scmd.length; i++) {
        var param = scmd[i];
        var buf = "";
        if (param.length == 0) {
            continue;
        }
        for (var e = 0; e < param.length; e++) {
            var c = param[e];
            if (c == '%') {
                var a = param[++e];
                if (esc[a] === undefined) {
                    fehError("unknown escape in command '" + cmd + "': %" + a);
                    return false;
                }
                else {
                    buf += esc[a];
                }
            }
            else {
                buf += c;
            }
        }
        args.push(buf);
    }

    try {
        var exec = Components.classes["@mozilla.org/file/local;1"].
                   createInstance(Components.interfaces.nsILocalFile);
        var pr = Components.classes["@mozilla.org/process/util;1"].
                 createInstance(Components.interfaces.nsIProcess);
        var path = null;

        // $PATH stuff (only on UNIX)
        if (window.navigator.platform.toLowerCase().indexOf("win") == -1)
        {
            path = fehmozexGetEnvironment("PATH").split(":");
        }

        // If executable is an absolute path or we don not know how to get PATH
        // (windows), run it or fail. Otherwise look for the executable in $PATH.
        // FIXME: How do you tell portably if a path is absolute?
        if (executable.charAt(0) == "/" || path == null) {
	    dump ("feh: executable is " + executable + "\n");
            exec.initWithPath(executable);
            if (! exec.exists()) {
                throw "executable '" + executable + "' does not exist.";
            }
        } 
        else {
            var found = false;
            for (i = 0; i < path.length; i++) {
		exec.initWithPath(path[i]);
                exec.appendRelativePath(executable);
                if (exec.exists()) {
		    dump ("feh: executable is " + path[i] + "/" + executable + "\n");
                    found = true;
                    break;
                }
            }
            if (!found) {
                throw "could not find '" + executable + "' in path:\n" + path.join ("\n");
            }
        }
	

	for (var j = 0; j < args.length; j ++)
	    dump ("feh: arg " + j + " is " + args[j] + "\n");
    

        pr.init(exec);
        pr.run(false /* don't block */, args, args.length);
    }
    catch (e) {
        fehError("cannot run executable '" + 
                   executable + "' (args: " + args + "): " + e);
        return false;
    }
    // alert(pr.pid); /* pid is not implemented :((( */
    return true;
}

var feh = {

    onshow: function() { 
	
	for (var name in feh.progs) { 
	    gContextMenu.showItem( "context-" + name, gContextMenu.onLink );
	}

	//gContextMenu.showItem( "context-za", gContextMenu.onLink );
	//alert(">>>>>> " + gContextMenu.linkURL);
    },

    onLoad: function() {
	// initialization code
	this.initialized = true;

	try { 

	    var dirService = Components.classes["@mozilla.org/file/directory_service;1"].getService(Components.interfaces.nsIProperties);
	    var path = dirService.get("ProfD", Components.interfaces.nsIFile);
	    path.append("link-handlers.js");

	    var stream = Components.classes["@mozilla.org/network/file-input-stream;1"].createInstance(Components.interfaces.nsIFileInputStream);

	    stream.init(path, 0x01, 0444, 0);

		var sstream = Components.classes["@mozilla.org/scriptableinputstream;1"]
			.createInstance(Components.interfaces.nsIScriptableInputStream);
		sstream.init(stream); 

		var data = ""; 

		var str = sstream.read(4096);
		while (str.length > 0) {
			data += str;
			str = sstream.read(4096);
		}

		sstream.close();
		stream.close();

		//alert(data); 

		progs = eval("feh.progs = " + data + ";"); 
	} catch (e) { 
	    fehError("error reading link-handlers.js: " + e);
	    return null;
	}

	var menu = document.getElementById("contentAreaContextMenu");
	var ni;

	/*
	ni = document.createElement("menuitem");
	ni.setAttribute("label", "feh");
	ni.setAttribute("oncommand", "feh(this);");
	menu.appendChild(ni);

	    ni = document.createElement("menuitem");
	    ni.setAttribute("label", "za");
	    ni.setAttribute("id", "context-za");
	    ni.setAttribute("oncommand", "feh.za(this)");
	    menu.appendChild(ni);

          */
	
	feh.proga = new Array(); 
	var i = 0;

	for (var name in feh.progs) { 
	    dump("FEHHH " + name + " ::: " + feh.progs[name] + "\n");
	    /*
	    feh.progsubs[name] = function() { 
		fehmozexRunProgram (feh.progs[name], {'r' : gContextMenu.linkURL});
	    }*/
	    feh.proga.push (name);
	    ni = document.createElement("menuitem");
	    ni.setAttribute("label", name);
	    ni.setAttribute("id", "context-" + name);
	    //ni.setAttribute("oncommand", "dump(\"ZZZZZZZZZZZZ +\"" + name + "\"\\n\"); feh.progsubs[\"" + name + "\"];");
	    //ni.setAttribute("oncommand", "dump(\"ZZZZZZZZZZZZ  "+ name +"\\n\");");
	    dump("ZZZZZ:   fehDispatch(\""+ name +"\");\n");
	    //ni.setAttribute("oncommand", "fehDispatch(\""+ name +"\");");
	    ni.setAttribute("oncommand", "fehDispatch("+ i +");");
	    menu.appendChild(ni);
	    i++;
	}
	dump ("end feh\n")

	var h = menu.getAttribute("onpopupshowing");
	menu.feh_onshowing = eval ("function(event) {" + h + "}");

	/*
	menu.setAttribute("onpopupshowing",
			  "if (event.target != this) return true; gContextMenu = new nsContextMenu( this ); feh.onshow(); return gContextMenu.shouldDisplay;");*/

	menu.setAttribute("onpopupshowing", "var ret = this.feh_onshowing(event); feh.onshow(); return x;"); 
	//alert(menu.initClipboardItems);

    },

    progs: {},

    /*
    progs: { 
	"seeurl" : "gnome-terminal -x sh -c \"echo foo: %r | less\" ",
	"links"  : "gnome-terminal -x links %r "  },
*/

    //progsubs: {},    

    za: function(o) { 
	fehmozexRunProgram ("gnome-terminal -x sh -c \"echo foo: %r | less\" ", {'r' : gContextMenu.linkURL});
    }

  
};

function fehDispatch(i) { 
    var name = feh.proga[i];
    fehmozexRunProgram (feh.progs[name], {'r' : gContextMenu.linkURL});
}


/*
function feh(o) { 
    alert("feh " + o.parentNode.localName + ":::" + o.parentNode.getAttribute("onpopupshowing"));
}
*/


window.addEventListener("load", function(e) { feh.onLoad(e); }, false); 
