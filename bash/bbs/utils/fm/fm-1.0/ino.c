#include <sys/inotify>

GHashTable *directory_paths;

void init_directory_paths(void)
{
	// FIXME padding '////' from root_path
	directory_paths = g_hash_table_new(g_str_hash, g_str_equal);
}

void init_inotify(char *root_path)
{
	int descriptor, watch;

	if ((descriptor = inotify_init()) == -1) {
		PRINT_ERRNO_INFO();
		return;
	}
	if ((watch = inotify_add_watch(descriptor, root_path, IN_MOVED_FROM | IN_MOVED_TO | IN_DELETED)) == -1) {
		PRINT_ERRNO_INFO();
		return;
	}
}

GNode *get_file_by_path(char *path)
{
}

void insert_file_in_directory(GNode *directory, GNode *file)
{
}

void remove_file_from_directory(GNode *file)
{
}
