#define PURPLE_PLUGINS
#define	PREF_PREFIX	"/plugins/gtk/autoanswer"
#define PREF_MODE	PREF_PREFIX "/use_funny_mode"
#define PREF_LIMIT	PREF_PREFIX "/limit_of_answers"
#define PREF_ROBOT	PREF_PREFIX "/unhide_robot"
#define PREF_SEARCH	PREF_PREFIX "/use_all_logs"
#define PREF_DICT	PREF_PREFIX "/use_dct"
#define PREF_VERBOSE    PREF_PREFIX "/use_verbose_mode"
#define PREF_PROMPT	PREF_PREFIX "/robots_prompt"

#define _(String) ((const char *) (String))

#include <gtk/gtk.h>

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

/* Pidgin headers */
#include <pidgin/gtkaccount.h>
#include <pidgin/gtkblist.h>
#include <pidgin/gtkconv.h>
#include <pidgin/gtkimhtml.h>
#include <pidgin/gtkplugin.h>
#include <pidgin/gtkprefs.h>
#include <pidgin/gtkutils.h>
#include <pidgin/pidgin.h>



#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ftw.h>
#include <glib.h>


char *buff = NULL;
char *buddyname1 = NULL;
char *buddyname1_alias = NULL;
char *username_alias = NULL;
char *username = NULL;
GList *answers = NULL;
int limit_of_answers = 10;

int list(const char *, const struct stat *, int);
static void create_send_combo_pidgin(PidginConversation *gtkconv);

gint compare_str(gconstpointer a, gconstpointer b)
{
	if (a==NULL) return 1;
	if (b==NULL) return -1;
	return strcmp(a,b);
}

int list (const char *name, const struct stat *status, int type) {

    GList *files = NULL;
    FILE *curlog;
    char curline[1024];
    char *match = NULL;
    char answer[1024], *answer_body, *tmp_ptr;
    char *me; //, *me_end;
    int me_length;
    int current_number_of_answers = g_list_length(answers);

    if (type == FTW_NS) return 0;
    if (type == FTW_F) { 
	/* purple_debug_misc("auto-answer-history DEBUG", "%-30s\t0%3o\n", name, status->st_mode&0777); */
	files = g_list_append(files, (gpointer) name);
    }
/*    else {
	purple_debug_misc("auto-answer-history DEBUG", "Logdir is: %-30s*\t0%3o\n", name, status->st_mode&0777);
	purple_debug_misc("auto-answer-history DEBUG", "Going to find \"%s\" from buddy %s (%s)\n", buff, buddyname1, buddyname1_alias); 
    }*/
    
    if (username_alias != NULL) me = username_alias;
    else me = username;
    
    me_length=strlen(me);

    while(files) {
	/* purple_debug_misc("auto-answer-history DEBUG", "Processing... %s\n", files->data); */
	if ((curlog = fopen(files->data, "r")) != NULL) { 
	    while (fgets(curline, sizeof curline, curlog) != NULL) {
		if ( ((strstr(curline, buff)) != NULL) && ((strstr(curline, me)) == NULL)) {
		    match = strstr(curline, buff);
		    /* purple_debug_misc("auto-answer-history DEBUG", "FOUND in %s, line %d, looking for answer...\n", files->data, counter); */
		    while (fgets(answer, sizeof answer, curlog) !=NULL ) {
    			if ((strstr(answer, me)) != NULL) {
    			    if (purple_markup_strip_html(answer) != NULL) {     				
				if (current_number_of_answers>limit_of_answers)
				{	fclose(curlog);
					return 0;
				}
				/* check if such answer isn't already in a GList */

				answer_body=purple_markup_strip_html(answer);
				tmp_ptr=strchr(answer_body,':')+1; tmp_ptr=strchr(tmp_ptr,':')+1;
				tmp_ptr=strchr(tmp_ptr,':')+1; /*skip 3 semi-colons in the preface*/

				answer_body=strstr(answer_body,me)+me_length+1; /*skip preface*/
				if (tmp_ptr > answer_body) answer_body = tmp_ptr; /* choose the better method of preface skip*/

				if (!g_list_find_custom(answers,answer_body,compare_str))
				{	g_list_append (answers, answer_body);
					current_number_of_answers++;
				}

    			    }	
    			    break;
    			}			
		    } 
		}
	    }
	}
	fclose(curlog);
	files = files->next;
    }    
    return 0;
}

static void 
send_combo_cb(GtkWidget *widget, PidginConversation *gtkconv)
{	/* A button to send item from combobox was pressed */
	GtkIMHtmlOptions options;
	GtkWidget *send_combo;
	gchar *text, *pref_prompt;
	gtk_imhtml_clear(GTK_IMHTML(gtkconv->entry));
/*        if (purple_prefs_get_bool(PREF_ROBOT)) 
	{*/ /*gtk_imhtml_append_text(GTK_IMHTML(gtkconv->entry), _("The possible answer is:<B>"), options);*/
	pref_prompt=purple_prefs_get_string(PREF_PROMPT);
	if (strlen(pref_prompt)>0) 
	{	gtk_imhtml_append_text(GTK_IMHTML(gtkconv->entry), pref_prompt, options);
		gtk_imhtml_append_text(GTK_IMHTML(gtkconv->entry), "<B>", options);
	}
//		purple_debug_misc("auto-answer-history DEBUG", "the prompt was \"%d\"", strlen(pref_prompt));

/*	}*/
	send_combo = g_object_get_data(G_OBJECT(gtkconv->toolbar), "send_combo");
	text=gtk_combo_box_get_active_text(GTK_COMBO_BOX(send_combo));
	gtk_imhtml_append_text(GTK_IMHTML(gtkconv->entry), text, options); 
	g_free(text);
	g_signal_emit_by_name(gtkconv->entry, "message_send");
}

static void
received_im_msg_cb(PurpleAccount *account, char *who, char *buffer,
		   PurpleConversation *conv, PurpleMessageFlags flags, void *data)
{
	const char *name;
	char* logdir;
	char* foundmessage = NULL;
	gchar * pref_prompt;
	int number_of_lines;

	GtkWidget *send_combo = NULL;
	PurpleBuddy *b;
	GtkIMHtmlOptions options;
	PidginConversation *gtkconv = NULL;


   	limit_of_answers=purple_prefs_get_int(PREF_LIMIT);

	/* A workaround to avoid skipping of the first message as a result on NULL-conv: */
	if (conv == NULL) conv=purple_conversation_new(PURPLE_CONV_TYPE_IM, account, who);

	

	buff = purple_markup_strip_html(buffer);
	printf("\nHarvie received 1: %s\n", buffer);
	purple_conv_im_send(purple_conversation_get_im_data(conv), ":-*");

	purple_debug_info("auto-answer-history DEBUG", "buff: %s\n", buff);
	username = purple_account_get_username(account);
	purple_debug_info("auto-answer-history DEBUG", "username: %s\n", username);
	username_alias = purple_account_get_alias(account);
	buddyname1 = purple_conversation_get_name(conv); /** this shoud work fine, at least with ICQ **/
	purple_debug_info("auto-answer-history DEBUG", "buddyname: %s\n", buddyname1);
	b = purple_find_buddy(account, buddyname1);
	buddyname1_alias = purple_buddy_get_alias_only(b);

	name = purple_conversation_get_name(conv);
	logdir = purple_log_get_log_dir(PURPLE_LOG_IM, name, account);
	/* purple_debug_info("auto-answer-history DEBUG", "LogDir: %s\n", logdir); */
	
	answers=g_list_alloc();
	ftw(logdir, list, 1);
	purple_debug_misc("auto-answer-history DEBUG", "Number of answers found is: %d\n", (g_list_length(answers)-1));

	if (g_list_length(answers) == 1) return; /* exit if no possible answers found */

	gtkconv = PIDGIN_CONVERSATION(conv);

	if (purple_prefs_get_bool(PREF_VERBOSE)) gtk_imhtml_append_text(GTK_IMHTML(gtkconv->imhtml), "<HR>", options); 

	send_combo = g_object_get_data(G_OBJECT(gtkconv->toolbar), "send_combo");
	if (send_combo == NULL) {
		 create_send_combo_pidgin(gtkconv);
		 send_combo = g_object_get_data(G_OBJECT(gtkconv->toolbar), "send_combo");
	}
	else { /* clear combo box */
		number_of_lines = gtk_tree_model_iter_n_children(gtk_combo_box_get_model(send_combo), NULL);
		for(; number_of_lines; number_of_lines--) gtk_combo_box_remove_text(send_combo, number_of_lines-1);
		purple_debug_misc("auto-answer-history DEBUG", "number of lines to delete:%d\n", number_of_lines);
	}

	while (answers)  {
	    if (answers->data != 0) {
		g_strchomp(answers->data);
		foundmessage = (char*) answers->data;

		if (purple_prefs_get_bool(PREF_VERBOSE)) /* Flood all possible answers into my conversation window */
		{
	
			pref_prompt=purple_prefs_get_string(PREF_PROMPT);
			if (strlen(pref_prompt)>0) 
			{	gtk_imhtml_append_text(GTK_IMHTML(gtkconv->imhtml), pref_prompt, options);
				gtk_imhtml_append_text(GTK_IMHTML(gtkconv->imhtml), "<B>", options);
			}


			gtk_imhtml_append_text(GTK_IMHTML(gtkconv->imhtml), foundmessage, options);
			gtk_imhtml_append_text(GTK_IMHTML(gtkconv->imhtml), "</B><BR>", options);
		}
		/* A more flexible (than Copy/Paste) way to select answer: */
		gtk_combo_box_append_text(send_combo, foundmessage);
		
	    }
	    answers = answers->next;
	}
	number_of_lines = gtk_tree_model_iter_n_children(gtk_combo_box_get_model(send_combo), NULL);
	gtk_combo_box_set_active(send_combo, rand()%number_of_lines);

	

	if (purple_prefs_get_bool(PREF_VERBOSE)) gtk_imhtml_append_text(GTK_IMHTML(gtkconv->imhtml), "<HR>", options);   

	
	if (purple_prefs_get_bool(PREF_MODE)) 
	{

	    /* If "Funny Mode" is ON - going to autosend an answer */
    	    foundmessage=gtk_combo_box_get_active_text(send_combo);

	    purple_conv_im_send(PURPLE_CONV_IM(conv), foundmessage);
	    gtk_imhtml_clear(GTK_IMHTML(gtkconv->entry));
	    g_free(foundmessage);
	}	
}

static GtkWidget *
get_plugin_pref_frame(PurplePlugin *plugin) {

	GtkWidget *vbox = NULL;
	GtkWidget *frame = NULL;
	GtkWidget *auto_option = NULL;
	GtkWidget *limit_option = NULL;
	GtkWidget *verbose_option = NULL;
	GtkWidget *prompt_option = NULL;
	
	vbox = gtk_vbox_new(TRUE, PIDGIN_HIG_BOX_SPACE);
	
	frame = pidgin_make_frame(vbox, _("Auto-answer options"));
 	limit_option = pidgin_prefs_labeled_spin_button (frame, _("Maximum answers to find"), PREF_LIMIT, 5, 100, NULL);
	auto_option  = pidgin_prefs_checkbox(_("Send automatically"), PREF_MODE, frame);
	verbose_option  = pidgin_prefs_checkbox(_("Use verbose mode"), PREF_VERBOSE, frame);
	prompt_option = pidgin_prefs_labeled_entry(frame, _("Preface robot's answer with"), PREF_PROMPT, NULL);
 
	gtk_widget_show_all(vbox);

	return vbox;
}


static PurplePluginUiInfo ui_info = {
	get_plugin_pref_frame,
	0,   /* page_num (Reserved) */
	NULL, /* frame (Reserved) */

	/* padding */
	NULL,
	NULL,
	NULL,
	NULL
};


static void
create_send_combo_pidgin(PidginConversation *gtkconv)
{
	GtkWidget *send_combo, *say_button;
	GtkRequisition greq; 
/*	purple_debug_misc("auto-answer-history DEBUG", "combo created\n");*/

	send_combo = gtk_combo_box_new_text();
	gtk_combo_box_set_focus_on_click(send_combo, FALSE);

	gtk_box_pack_end(GTK_BOX(gtkconv->toolbar), send_combo, TRUE, TRUE, 0);
	gtk_widget_show(send_combo);

	gtk_widget_size_request(send_combo, &greq);
	gtk_widget_set_size_request (send_combo, greq.width, greq.height);

	g_object_set_data(G_OBJECT(gtkconv->toolbar), "send_combo", send_combo);



	say_button = gtk_button_new_with_mnemonic(_("Say"));
	gtk_button_set_focus_on_click(say_button, FALSE);
	gtk_button_set_relief(say_button, GTK_RELIEF_NONE);
	g_signal_connect(G_OBJECT(say_button), "clicked", 
	                 G_CALLBACK(send_combo_cb), gtkconv);
	gtk_box_pack_end(GTK_BOX(gtkconv->toolbar), say_button, FALSE, FALSE, 0);
	gtk_widget_show(say_button);

	g_object_set_data(G_OBJECT(gtkconv->toolbar), "say_button", say_button);


}

static void
remove_send_combo_pidgin(PidginConversation *gtkconv)
{
	GtkWidget *send_combo = NULL, *say_button = NULL;

	send_combo = g_object_get_data(G_OBJECT(gtkconv->toolbar), "send_combo");
	if (send_combo != NULL) {
		gtk_widget_destroy(send_combo);
	}

	say_button = g_object_get_data(G_OBJECT(gtkconv->toolbar), "say_button");
	if (say_button != NULL) {
		gtk_widget_destroy(say_button);
	}
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

	GList *convs = purple_get_conversations();

	while (convs) {
		PurpleConversation *conv = (PurpleConversation *)convs->data;

		/* Remove Send combo */
		if (PIDGIN_IS_PIDGIN_CONVERSATION(conv)) {
			remove_send_combo_pidgin(PIDGIN_CONVERSATION(conv));
		}

		convs = convs->next;
	}

	return TRUE;
}


static PurplePluginInfo info =
{
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,                             	/**< type           */
	PIDGIN_PLUGIN_TYPE,                             	/**< ui_requirement */
	0,                                                	/**< flags          */
	NULL,                                             	/**< dependencies   */
	PURPLE_PRIORITY_DEFAULT,                            	/**< priority       */

	"pidgin-autokiss",                   			/**< id             */
	"AutoKiss",                  		/**< name           */
	"0.1",                                  		/**< version        */
	                                                	/**  summary        */
	"Automatically answering based on regexes.",	
	                                                	/**  description    */
	"Automatically answering based on regexp",
	"Harvie <harvie@email.cz>",	              	/**< author         */
	"http://sourceforge.net/projects/pidgin-autoansw",     	/**< homepage       */

	plugin_load,                                      	/**< load           */
	plugin_unload,                                    	/**< unload         */
	NULL,                                             	/**< destroy        */
	&ui_info,						/**< ui_info        */
	NULL,                                             	/**< extra_info     */
	NULL,							/**< prefs_info	    */
	NULL, 		/* this tells libpurple the address of the function to call
	                           to get the list of plugin actions. */

	/* padding */
	NULL,
	NULL,
	NULL,
	NULL
};

static void
init_plugin(PurplePlugin *plugin)
{
	purple_prefs_add_none(PREF_PREFIX);
	purple_prefs_add_bool(PREF_MODE, FALSE);
	purple_prefs_add_bool(PREF_VERBOSE, FALSE);
	purple_prefs_add_string(PREF_PROMPT, "The possible answer is:");

	/** TODO: Add more search options and uninformative words dictionary support**/

	purple_prefs_add_bool(PREF_SEARCH, FALSE);
	purple_prefs_add_bool(PREF_DICT, FALSE);

}

PURPLE_INIT_PLUGIN(autoanswer, init_plugin, info)
