#define PURPLE_PLUGINS

/* Purple headers */
#include <libpurple/debug.h>
#include <libpurple/version.h>
#include <libpurple/conversation.h>
#include <libpurple/debug.h>
#include <libpurple/log.h>
#include <libpurple/plugin.h>
#include <libpurple/pluginpref.h>
#include <libpurple/prefs.h>
#include <libpurple/signals.h>
#include <libpurple/util.h>
#include <libpurple/notify.h>

#include <time.h>
#include <string.h>
#include <stdio.h>
/*
#include <sys/stat.h>
#include <ftw.h>
#include <glib.h>
#include <unistd.h>
*/

char *buff = NULL;

gint compare_str(gconstpointer a, gconstpointer b)
{
	if (a==NULL) return 1;
	if (b==NULL) return -1;
	return strcmp(a,b);
}


static void
received_im_msg_cb(PurpleAccount *account, char *who, char *buffer,
		   PurpleConversation *conv, PurpleMessageFlags flags, void *data)
{

	/* A workaround to avoid skipping of the first message as a result on NULL-conv: */
	if (conv == NULL) conv=purple_conversation_new(PURPLE_CONV_TYPE_IM, account, who);

	buff = purple_markup_strip_html(buffer);
	printf("\nHarvie received 1: %s\n", buffer);

	purple_conv_im_send(purple_conversation_get_im_data(conv), ":-*");
	
}

/* Plugin Routine */

static gboolean
plugin_load(PurplePlugin *plugin)
{
	void *conv_handle = purple_conversations_get_handle();
						
	purple_signal_connect(conv_handle, "received-im-msg",
					plugin, PURPLE_CALLBACK(received_im_msg_cb), NULL);
	return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin) {

	return TRUE;
}


static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,
    NULL,
    0,
    NULL,
    PURPLE_PRIORITY_DEFAULT,

	"core-autokiss",                   			/**< id             */
	"AutoKiss",                  		/**< name           */
	"0.1",                                  		/**< version        */
	                                                	/**  summary        */
	"Automatically answering based on regexes.",	
	                                                	/**  description    */
	"Automatically answering based on regexpppppppppppppppp",
	"Harvie <harvie@email.cz>",	              	/**< author         */
	"http://sourceforge.net/projects/pidgin-autoansw",     	/**< homepage       */

	plugin_load,                                      	/**< load           */
	plugin_unload,                                    	/**< unload         */
    NULL,                          
                                   
    NULL,                          
    NULL,                          
    NULL,                        
    NULL,                   
    NULL,                          
    NULL,                          
    NULL,                          
    NULL  
};

static void
init_plugin(PurplePlugin *plugin)
{

}

PURPLE_INIT_PLUGIN(autoanswer, init_plugin, info)


