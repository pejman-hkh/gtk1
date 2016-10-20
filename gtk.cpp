#include "gtk.h"

extern "C" {
    
    Gtk::Gtk() {
    
        
    }
    
    Gtk &Gtk::init( int * argc, char **argv[] ) {
        gtk_init(argc, argv);   
        _window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        g_signal_connect( _window, "destroy", G_CALLBACK(gtk_main_quit), NULL );
        
        _eventbox = gtk_event_box_new();
        gtk_container_add(GTK_CONTAINER(_window), _eventbox);
        gtk_widget_show( _eventbox );                
        
        _scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
        gtk_container_add(GTK_CONTAINER(_eventbox), _scrolledwindow);
        gtk_widget_show( _scrolledwindow );

        _layout = gtk_layout_new ( NULL, NULL );
        gtk_container_add (GTK_CONTAINER (_scrolledwindow), _layout);
        gtk_widget_show (_layout);

        _xp, _yp, _max_yp = 20;

        //load css

        _provider = gtk_css_provider_new ();
        _display = gdk_display_get_default ();
        _screen = gdk_display_get_default_screen ( _display );
        
        return *this;
    }

    Gtk& Gtk::operator()( std::string selector ){
        this->_selector = selector;
        return *this;
    }

    Gtk &Gtk::set_size( int width, int height ) {
        gtk_window_set_default_size (GTK_WINDOW (_window), width, height);
        return *this;
    }

    Gtk &Gtk::set_title( std::string title ) {
        gtk_window_set_title(GTK_WINDOW(_window), title.c_str() );
        return *this;
    }
            
    void Gtk::button( std::string str, std::map <std::string, std::string> attr ) {
        _object = gtk_button_new_with_label ( str.c_str() );
        
        _types[ "#"+attr["id"] ] = "button";
        
        _create_widget( &attr );

    }
            
    void Gtk::click( CallbackFunctionPtr callback ) {
        g_signal_connect (_ids[ _selector ], "clicked",  G_CALLBACK( callback ), NULL);
    }

    void Gtk::alert( std::string msg ) {

        GtkWidget *dialog, *label, *content_area;
        GtkDialogFlags flags;

        // Create the widgets
        flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_dialog_new_with_buttons ("Message",
                                            GTK_WINDOW(_window),
                                            flags,
                                            "_OK",
                                            GTK_RESPONSE_NONE,
                                            NULL);
        content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
        label = gtk_label_new (msg.c_str());

        // Ensure that the dialog box is destroyed when the user responds

        g_signal_connect_swapped (dialog,
                                "response",
                                G_CALLBACK (gtk_widget_destroy),
                                dialog);

        // Add the label, and show everything we’ve added

        gtk_container_add (GTK_CONTAINER (content_area), label);
        gtk_widget_show_all (dialog);
        
    }            

    void Gtk::textarea ( std::string str, std::map <std::string, std::string> attr ) {

        GtkTextBuffer *buffer;
        _object = gtk_text_view_new ();
        
        gtk_widget_set_size_request( _object, 50, 20 );
            

        buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW ( _object ));
        gtk_text_view_set_editable(GTK_TEXT_VIEW ( _object ), FALSE);
        gtk_text_view_set_cursor_visible( GTK_TEXT_VIEW ( _object ), FALSE);

        gtk_text_buffer_set_text (buffer, str.c_str(), -1);


        std::string id = "#"+attr["id"];
        _text_buffers[ id ] = buffer;
        _types[ id ] = "textarea";
        
        
        _create_widget( &attr );

        
    }

    void Gtk::html( std::string str ) {
        //std::cout << "selector " << _selector << std::endl;
        if( _selector == "" ) {

            GtkWidget *html = gtk_label_new ( str.c_str() );
            gtk_fixed_put (GTK_FIXED ( _object ), html, _xp, _yp );
            gtk_widget_show( html );

            gint w1, w2;
            gtk_widget_get_preferred_width( _object, &w1, &w2 );
            
            gint h1, h2;
            gtk_widget_get_preferred_height( _object, &h1, &h2 );

            _xp += w2;
            _max_yp = h2;
        
            br();
            
        } else {
            if( _types[ _selector ] == "label" ) {
                gtk_label_set_text( GTK_LABEL( _ids[ _selector ] ), str.c_str() );
            } else {
                //gtk_text_buffer_set_text ( _buffers[ _selector ], str.c_str(), -1);
            }
        }    
    }

    void Gtk::label( std::string str, std::map <std::string, std::string> attr ) {
        
        _object = gtk_label_new ( str.c_str() );
        
        _types[ "#"+attr["id"] ] = "label";
        
        _create_widget( &attr );

    }

    void Gtk::input ( std::string str, std::map <std::string, std::string> attr ) {


        //_object = gtk_entry_new ();
        
        //gtk_style_context_add_class ( gtk_widget_get_style_context ( attr["class"].c_str() ), _object );
        
        
        //GtkWidget *entry;
        
        GtkEntryBuffer *entrybuffer = gtk_entry_buffer_new( str.c_str(), -1);
        _object = gtk_entry_new_with_buffer(GTK_ENTRY_BUFFER(entrybuffer));

        gtk_entry_set_text ( GTK_ENTRY (_object), str.c_str() );
        
        //_types[ "#"+attr["id"] ] = "input";

        std::string id = "#"+attr["id"];
        _entry_buffers[ id ] = entrybuffer;
        _types[ id ] = "input";

        
        _create_widget( &attr );
        
        
    }

    void Gtk::select( std::map <std::string, std::string> option, std::map <std::string, std::string> attr ) {

        /*_object = gtk_combo_box_text_new();
        gint i = 0;
        for( std::map <std::string, std::string>::iterator it = option.begin(); it != option.end(); ++it ) {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(_object), (it->second).c_str() );
            //gtk_combo_box_set_id_column( GTK_COMBO_BOX_TEXT(_object), i );
            i++;
        }
        

        gtk_combo_box_set_active(GTK_COMBO_BOX(_object), 0);
    */

        GtkListStore *liststore = gtk_list_store_new(1, G_TYPE_STRING);
        GtkTreeIter iter;
        
        for( std::map <std::string, std::string>::iterator it = option.begin(); it != option.end(); ++it ) {
            std::cout << it->second << std::endl;
            
            gtk_list_store_append(liststore, &iter);
            gtk_list_store_set(liststore, &iter, 0, (it->second).c_str(), -1);
            
        }
        
        _object = gtk_combo_box_new_with_model(GTK_TREE_MODEL(liststore));
        gtk_combo_box_set_active(GTK_COMBO_BOX(_object), 0);            

        //gtk_container_add(GTK_CONTAINER(_window), _object);
        
        GtkCellRenderer *cellrenderertext = gtk_cell_renderer_text_new();
        gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(_object), cellrenderertext, TRUE);
        gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(_object), cellrenderertext, "text", 0, NULL);

        
        
        std::string id = "#"+attr["id"];
        _types[ id ] = "select";
            
        _create_widget( &attr );

        
                        
    }


    /*void Gtk::tag( std::string tag_name, std::map <std::string, std::string> attr ) {

        
        if( tag_name.substr (0,1) == "/" ) {
            //end of tag
            std::cout << " end of tag " << std::endl;
            
        } else {
            //start of tag
            std::cout << " start of tag " << std::endl;

            if( tag_name == "table" ) {
                //create table and set default html object
                _parent = _object;
                _tag = gtk_grid_new ();
                gtk_container_add (GTK_CONTAINER (_parent), _tag);

            } else if( tag_name == "tr" ) {
                _parent = _tag;
                //_tag = gtk_grid_insert_row ( GTK_GRID(_parent), 1 );

            } else if( tag_name == "td" ) {
                _parent = _tag;
                //_tag = gtk_grid_insert_column ( GTK_GRID(_parent), 1 );
            } if( tag_name == "div" ) {
                _tag = gtk_fixed_new();
                //gtk_fixed_put (GTK_LAYOUT (_tag), _object, _xp, _yp );
        
            }
        }
        
        std::string id = "#"+attr["id"];
        _types[ id ] = tag_name;
            
        //_create_widget( &attr );
    }*/

    void Gtk::div( std::map <std::string, std::string> attr ) {
        _object = gtk_fixed_new ();
        
        _types[ "#"+attr["id"] ] = "div";
        
        _create_widget( &attr );
        
    }
    
    void Gtk::link( std::map <std::string, std::string> attr ) {
        
        
        gtk_style_context_add_provider_for_screen (_screen, GTK_STYLE_PROVIDER (_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        
        gsize bytes_written, bytes_read;

        GError *error = 0;

        gtk_css_provider_load_from_path (_provider, g_filename_to_utf8( attr["href"].c_str(), strlen( attr["href"].c_str() ), &bytes_read, &bytes_written, &error), NULL);

        
    }

    Gtk &Gtk::val( std::string str ) {
        
        if( _types[ _selector ] == "input" ) {
            //gtk_entry_set_text ( GTK_ENTRY ( this->_ids[ this->_selector ] ), str.c_str() );
            gtk_entry_buffer_set_text ( _entry_buffers[ _selector ], str.c_str(), -1);
        } else if( _types[ _selector ] == "textarea" ) {
            gtk_text_buffer_set_text ( _text_buffers[ _selector ], str.c_str(), -1);
            //gtk_label_set_text( GTK_LABEL( this->_ids[ this->_selector ] ), str.c_str() );
        }
        
        return *this;
    }

    std::string Gtk::val() {
        
        std::string ret;
        
        if( _types[_selector] == "input" ) {
            const gchar *text = gtk_entry_get_text( GTK_ENTRY( _ids[ _selector ] ) );
                ret = text;
        } else if( _types[_selector] == "select" ) {
            // const gchar *out = gtk_combo_box_get_active_id( GTK_COMBO_BOX( _ids[ _selector ] ) );
            //ret = out;
        }

        return ret;
    }

    void Gtk::mousedown( CallbackFunctionPtr callback ) {
        g_signal_connect (_ids[ _selector ], "button-press-event",  G_CALLBACK( callback ), NULL);
    }


    void Gtk::change( CallbackFunctionPtr callback ) {
        if( _types[ _selector ] == "select" ) {
            g_signal_connect(_ids[ _selector ], "changed", G_CALLBACK(callback), NULL); 
        }
    }

    void Gtk::mouseup( CallbackFunctionPtr callback ) {
        //g_signal_connect ( G_OBJECT(_eventbox), "button_press_event",  G_CALLBACK( callback ), NULL);
    }


    void Gtk::br() {
        _yp += _max_yp + 5;
        _xp = 0;
    }

    Gtk::~Gtk() {
        g_object_unref (_provider);
        gtk_widget_show (_window);
        
        gtk_main();
    }

    void Gtk::_create_widget( std::map <std::string, std::string> *attr ) {

        std::string id = "#"+(*attr)["id"];
        _ids[ id ] = _object;
        
        
        //set id for usage in css
        gtk_widget_set_name ( GTK_WIDGET( _object ), (*attr)["id"].c_str() ); 
        
        //gtk_style_context_add_class ( gtk_widget_get_style_context ( (*attr)["class"].c_str() ), _object);

        gtk_layout_put (GTK_LAYOUT (_layout), _object, _xp, _yp );
        
        gtk_widget_show( _object );

        gint w1, w2;
        gtk_widget_get_preferred_width( _object, &w1, &w2 );
        
        gint h1, h2;
        gtk_widget_get_preferred_height( _object, &h1, &h2 );

        _xp += w2;

        //std::cout << h2 << std::endl;
        
        _max_yp = h2;

    }            

}
