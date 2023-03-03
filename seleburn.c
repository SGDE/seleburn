#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>

#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];
FILE *fp;
GtkIconSize size = GTK_ICON_SIZE_BUTTON;
GtkWidget *input_btn;
GtkWidget *device_box;
GtkWidget *apply_btn;
GtkWidget *path_etr;
GtkWidget *bs_box;
GtkWidget *restart_btn;
GtkWidget *ddbar;
GtkWidget *spinner;



static void restart_program(GtkWidget *widget, gpointer data)
{
    const gchar *pwd = getenv("PWD");
    if (pwd != NULL) {
        gchar *cmd = g_strdup_printf("%s/%s", pwd,"pkexec seleburn");
        GError *error = NULL;
        GPid pid;
        gint exit_status = 0;
        gboolean success = g_spawn_async_with_pipes(NULL,
                                                    (gchar * []) {cmd, NULL},
                                                    NULL,
                                                    G_SPAWN_SEARCH_PATH,
                                                    NULL,
                                                    NULL,
                                                    &pid,
                                                    NULL,
                                                    NULL,
                                                    NULL,
                                                    &error);
        if (!success)
        {
            g_warning("Failed to restart program: %s", error->message);
            g_error_free(error);
        }
        else
        {
            g_spawn_close_pid(pid);
            gtk_main_quit();
        }
            g_free(cmd);
        
        
}
}

static void on_input_btn_clicked(GtkButton *button, gpointer user_data) 
{
    GtkWidget *dialog;
    GtkFileFilter *filter;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Open File", GTK_WINDOW(user_data), GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
    filter = gtk_file_filter_new ();
    gtk_file_filter_add_mime_type(filter, "application/x-cd-image");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT) 
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        gtk_entry_set_text(GTK_ENTRY(path_etr), filename);
        g_free(filename);
    }

    gtk_widget_destroy (GTK_WIDGET (dialog));
}

static void on_apply_btn_clicked(GtkButton *button, gpointer user_data) 
{
    
    gtk_spinner_start(GTK_SPINNER(spinner));
    const char *iso_path = gtk_entry_get_text(GTK_ENTRY(path_etr));
    const char *output_device = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(device_box));    
    const char *block_size = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(bs_box));
    
    // Initialize variables
char dname[256];
char dsize[256];

// Copy output_device string to a temporary variable
char temp[256];
strcpy(temp, output_device);

// Extract device name
char *token = strtok(temp, " ");
if (token != NULL) {
    strcpy(dname, token);
}

// Extract device size
char *p = strchr(output_device, ' ');
if (p != NULL) {
    strcpy(dsize, p+1);
}

// Print results
printf("Device name: %s\n", dname);
printf("Device size: %s\n", dsize);
printf("Input Path: %s\n", iso_path);
printf("Block Size: %s\n", block_size);

if (strlen(iso_path) > 0 && strlen(output_device) > 0 && strlen(block_size) > 0) 
{
   
    char cmd[1024];
    int status;
    snprintf(cmd, sizeof(cmd), "dd if=%s of=/dev/%s bs=%s status=progress", iso_path, dname, block_size);
    system(cmd);

if (status != 0) {
    // Command was not executed successfully
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Operation completed successfully");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
} else {
    // Command was executed successfully
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Operation Failed");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
}
else
{
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Please fill all entries");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_spinner_stop(GTK_SPINNER(spinner));
    return;
}
    gtk_spinner_stop(GTK_SPINNER(spinner));
}

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *grid;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "SeliBurn");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    //seticon
    GtkIconTheme *theme = gtk_icon_theme_get_default();
    GtkIconInfo *info = gtk_icon_theme_lookup_icon(theme, "drive-removable-media", 48, 0);
    if (info != NULL) 
    {
        GdkPixbuf *icon = gtk_icon_info_load_icon(info, NULL);
        gtk_window_set_icon(GTK_WINDOW(window), icon);
        g_object_unref(icon);
        g_object_unref(info);
    }

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    
    path_etr = gtk_entry_new();
        gtk_editable_set_editable(GTK_EDITABLE(path_etr), TRUE);
        gtk_entry_set_text(GTK_ENTRY(path_etr), "");
        gtk_box_pack_start(GTK_BOX(box), path_etr, TRUE, TRUE, 0);
    
    GtkWidget *icon = gtk_image_new_from_icon_name("folder-symbolic", GTK_ICON_SIZE_BUTTON);
    input_btn = gtk_button_new();
        gtk_button_set_image(GTK_BUTTON(input_btn), icon);
        gtk_widget_set_size_request(input_btn, 132, 32);
        gtk_box_pack_end(GTK_BOX(box), input_btn, FALSE, FALSE, 0);
    
    device_box = gtk_combo_box_text_new();
        gtk_box_pack_start(GTK_BOX(box1), device_box, TRUE, TRUE, 0);
    bs_box = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(bs_box), "Block Size \n (4M Default)");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(bs_box), "4M");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(bs_box), "8M");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(bs_box), "6M");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(bs_box), "2M");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(bs_box), "1M");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(bs_box), "512K");
        gtk_combo_box_set_active(GTK_COMBO_BOX(bs_box), 1);
        gtk_box_pack_end(GTK_BOX(box1), bs_box, FALSE, FALSE, 0);
    spinner = gtk_spinner_new();
    gtk_widget_set_size_request(spinner, 32, 32);
    apply_btn = gtk_button_new_with_label("Apply");
    restart_btn = gtk_button_new_with_label("Restart SeleBurn");


fp = popen("lsblk -d -o NAME,SIZE", "r");
if (fp == NULL) 
{
    fprintf(stderr, "Failed to execute command\n");
    exit(1);
}

int line_count = 0;

while (fgets(buffer, BUFFER_SIZE, fp)) 
{
    line_count++;

    if (line_count == 1) 
    {
        // Skip the first line
        continue;
    }

    char *name = strtok(buffer, " ");
    char *size = strtok(NULL, " ");

    if (name && size) 
    {
        char *disk = g_strdup_printf("%s %s", name, size);
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(device_box), NULL, disk);
        g_free(disk);
    }
}
    GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(device_box));
    int num_rows = gtk_tree_model_iter_n_children(model, NULL);
    if (num_rows == 1) 
    {
        printf("ERROR: ONLY ONE DEVICE DETECTED \n");
        gtk_widget_set_sensitive(device_box, FALSE);
        gtk_widget_set_sensitive(apply_btn, FALSE);
        gtk_widget_set_sensitive(input_btn, FALSE);
        gtk_widget_set_sensitive(path_etr, FALSE);
        gtk_widget_set_sensitive(bs_box, FALSE);
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("WARNING: You only have one device connected, please connect another device and restart this program"), 0, 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), restart_btn, 0, 0, 1, 1);
    }

    gtk_combo_box_set_active(GTK_COMBO_BOX(device_box), 1);
    
    gtk_grid_attach(GTK_GRID(grid), box, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box1, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("WARNING: Be careful when selecting the device, this process will erase all files on the device \n If you choose the wrong device, YOU CAN DELETE ALL YOUR SYSTEM"), 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), apply_btn, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), spinner, 0, 7, 1, 1);

    if (!geteuid() == 0)
    {
        gtk_widget_set_sensitive(device_box, FALSE);
        gtk_widget_set_sensitive(apply_btn, FALSE);
        gtk_widget_set_sensitive(input_btn, FALSE);
        gtk_widget_set_sensitive(path_etr, FALSE);
        gtk_widget_set_sensitive(bs_box, FALSE);
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("WARNING: You are not running this program with root access. \n if you are opening from terminal use sudo, doas or pkexec"), 0, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), restart_btn, 0, 0, 1, 1);
    } 
    
g_signal_connect(input_btn, "clicked", G_CALLBACK(on_input_btn_clicked), window);
g_signal_connect(apply_btn, "clicked", G_CALLBACK(on_apply_btn_clicked), window);
g_signal_connect(restart_btn, "clicked", G_CALLBACK(restart_program), window);

gtk_widget_show_all(window);


gtk_main();

return 0;
}


