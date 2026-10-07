#include<gtk/gtk.h>

struct key{
    gint id;
    GtkWidget *button;
  };

static const gchar letters[18]="QWERTYASDFGHZXCVBN";
//Need single chars as strings.
static gchar single_char[2]={'A', '\0'};

static void button_clicked(GtkWidget *button, gpointer *user_data)
  {
    gpointer *button_index=g_hash_table_lookup((GHashTable*)user_data[0], button);
    g_print("Button index %i\n", (gint)(*button_index));
    gint index=(gint)(*button_index);
    single_char[0]=letters[index];
    gchar *string=g_strdup_printf("%s%s", gtk_entry_get_text(GTK_ENTRY(user_data[1])), single_char);
    gtk_entry_set_text(GTK_ENTRY(user_data[1]), string);
    g_free(string);
  }
  
// Úprava CSS štýlov pre jednotný tmavý vzhľad celého panelu
static void apply_css(GtkWidget *window, GtkWidget *entry, GtkWidget *box) {
    GtkCssProvider *provider = gtk_css_provider_new();
    const gchar *css = 
        "window {"
        "  background-color: transparent;"
        "  box-shadow: none;"
        "  border: none;"
        "}"
        "button {"
        "  background-color: #1a1a1a;"
        "  border: 1px solid #333333;"
        "  border-radius: 8px;"
        "  padding: 2px 8px;"
        "}"
        "entry {"
        "  background-color: transparent;"
        "  color: #ffffff;"
        "  border: none;"
        "  box-shadow: none;"
        "  font-size: 13px;"
        "  min-height: 28px;"
        "  caret-color: #ffffff;"
        "}"
        "label {"
        "  color: #aaaaaa;"
        "  font-size: 12px;"
        "  font-weight: bold;"
        "  margin-left: 6px;"
        "}";

    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    
    gtk_style_context_add_provider(gtk_widget_get_style_context(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider(gtk_widget_get_style_context(entry), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider(gtk_widget_get_style_context(box), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}
  
int main(int argc, char *argv[])
  {
    gtk_init (&argc, &argv);
    gint i=0;
    gint j=0;
    
    
    // Hlavné bezokrajové okno
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_title(GTK_WINDOW(window), "Keyboard");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *entry=gtk_entry_new();
    gtk_widget_set_hexpand(entry, TRUE);

    // Aplikovanie moderného tmavého vzhľadu
    apply_css(window, entry, window);

    //Save buttons in an array.
    struct key k1;
    GArray *keyboard=g_array_new(FALSE, FALSE, sizeof(struct key));    
    for(i=0;i<18;i++)
      {
        single_char[0]=letters[i];
        k1.id=i;
        k1.button=gtk_button_new_with_label(single_char);
        g_array_append_val(keyboard, k1);
      }   
 
    //A hash table to look up array index values.
    struct key *p1=NULL;
    GHashTable *hash_table=g_hash_table_new(NULL, NULL);
    for(i=0;i<18;i++)
      {
        p1=&g_array_index(keyboard, struct key, i);
        g_hash_table_insert(hash_table, p1->button, &(p1->id));
      }

    gpointer user_data[2]={hash_table, entry};
    GtkWidget *grid1=gtk_grid_new();
    for(i=0;i<3;i++)
      {
        for(j=0;j<6;j++)
          {
            p1=&g_array_index(keyboard, struct key, i*6+j);
            gtk_grid_attach(GTK_GRID(grid1), p1->button, j, i, 1, 1);
            g_signal_connect(p1->button, "clicked", G_CALLBACK(button_clicked), user_data);
          }
      } 

    GtkWidget *scroll=gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scroll, TRUE);
    gtk_widget_set_hexpand(scroll, TRUE);
    gtk_container_add(GTK_CONTAINER(scroll), grid1);

    GtkWidget *expander=gtk_expander_new("Keyboard");
    gtk_widget_set_vexpand(expander, TRUE);
    gtk_widget_set_hexpand(expander, TRUE);
    gtk_container_add(GTK_CONTAINER(expander), scroll);

    GtkWidget *grid2=gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid2), expander, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid2), entry, 0, 1, 1, 1);

    gtk_container_add(GTK_CONTAINER(window), grid2);

    gtk_widget_show_all(window);

    // Presné centrovanie: stred horizontálne, 100px od vrchu vertikálne
    GdkDisplay *display = gdk_display_get_default();
    GdkMonitor *monitor = gdk_display_get_primary_monitor(display);
    if (!monitor) {
        monitor = gdk_display_get_monitor(display, 0);
    }
    GdkRectangle geometry;
    gdk_monitor_get_geometry(monitor, &geometry);

    int pos_x = geometry.x + (geometry.width - 370) / 2;
    int pos_y = geometry.height - 340;
    gtk_window_move(GTK_WINDOW(window), pos_x, pos_y);

    gtk_main();

    g_hash_table_destroy(hash_table);
    g_array_free(keyboard, TRUE);

    return 0;
  }
  
