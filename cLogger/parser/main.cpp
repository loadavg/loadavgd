/*
 * log file parser
 * grabs a loadavg log file and returns it as a vector
 *
 */

#include "parser.h"


//some gloabls here
LoadAvg loadavg;
string inputFileName,outputFileName;

/*
 * main function
 *
 */

// usage
// parser logs/disk_usage_2014-11-17.log


/* This is a callback function. The data arguments are ignored
 * in this example. More on callbacks below.
 */
static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Running Collectd Parser Now...\n\n");
    loadavg.runParser(inputFileName.c_str() , outputFileName.c_str() );

}

static gboolean
on_delete_event (GtkWidget *widget,
                 GdkEvent  *event,
                 gpointer   data)
{
  /* If you return FALSE in the "delete_event" signal handler,
   * GTK will emit the "destroy" signal. Returning TRUE means
   * you don't want the window to be destroyed.
   *
   * This is useful for popping up 'are you sure you want to quit?'
   * type dialogs.
   */

  g_print ("delete event occurred\n");

  return TRUE;
}






int main ( int argc, char *argv[] )
{


  bool status;

  if ( argc != 2 ) // argc should be 2 for correct execution
  {
    // We print argv[0] assuming it is the program name
    cout<<"usage: "<< argv[0] <<" <filename>\n";
    return 0;
  }
  else 
  {    
    inputFileName = argv[1];
  }

  if ( argv[2]) // if output file is specified
  {    
    outputFileName = argv[2];
  }
  else
  {
    //set output file
    outputFileName = "conf/copylog.log";
  }


  //this is repalced by gtk calls referenced below
  //loadavg.runParser(inputFileName.c_str() , outputFileName.c_str() );





 //hack this in now matey

   GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;

  /* This is called in all GTK applications. Arguments are parsed
   * from the command line and are returned to the application.
   */
  gtk_init (&argc, &argv);

  /* create a new window, and set its title */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Collectd Parser");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new ();

  /* Pack the container in the window */
  gtk_container_add (GTK_CONTAINER (window), grid);

  button = gtk_button_new_with_label ("Run Parser");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  /* Place the first button in the grid cell (0, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label ("Run Parser");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  /* Place the second button in the grid cell (1, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);

  button = gtk_button_new_with_label ("Quit");
  g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

  /* Place the Quit button in the grid cell (0, 1), and make it
   * span 2 columns.
   */
  gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 2, 1);

  /* Now that we are done packing our widgets, we show them all
   * in one go, by calling gtk_widget_show_all() on the window.
   * This call recursively calls gtk_widget_show() on all widgets
   * that are contained in the window, directly or indirectly.
   */
  gtk_widget_show_all (window);

  /* All GTK applications must have a gtk_main(). Control ends here
   * and waits for an event to occur (like a key press or a mouse event),
   * until gtk_main_quit() is called.
   */
  gtk_main ();

  return 0;


}







