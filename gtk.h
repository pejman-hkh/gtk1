#ifndef __GTK_PEJMAN
#define __GTK_PEJMAN

#include <gtk/gtk.h>
#include <cstring>
#include <iostream>
#include <map>
#include <string>

extern "C" {
    struct _size {
        int width;
        int height;
    };
    
    typedef void(*CallbackFunctionPtr)();

    class Gtk {
        public:
            Gtk& init( int * argc, char **argv[] );

            Gtk();

            Gtk& operator()( std::string selector );

            Gtk& set_size( int width, int height );

            Gtk& set_title( std::string title );

            void clicked( CallbackFunctionPtr callback );
            
            void click( CallbackFunctionPtr callback );

            void alert( std::string msg );

            void button( std::string str, std::map <std::string, std::string> attr = {} );
            
            void textarea ( std::string str, std::map <std::string, std::string> attr = {} );
            
            void html( std::string str );
            
            void label( std::string str, std::map <std::string, std::string> attr = {} );

            void input ( std::string str, std::map <std::string, std::string> attr = {} );

            void div ( std::map <std::string, std::string> attr = {} );
            
            void select( std::map <std::string, std::string> option, std::map <std::string, std::string> attr = {} );
            
            void tag( std::string tag_name, std::map <std::string, std::string> attr = {} );
            
            void link( std::map <std::string, std::string> attr = {} );
            
            Gtk &val( std::string str );
            std::string val();
            void mousedown( CallbackFunctionPtr callback );
            
            
            void change( CallbackFunctionPtr callback );
            
            void mouseup( CallbackFunctionPtr callback );
          
            void br();

            ~Gtk();

        
        private:
            
            void _create_widget( std::map <std::string, std::string> *attr );
            
            gint _xp, _yp, _max_yp;
            GtkWidget * _window;
            GtkWidget *_scrolledwindow;
            GtkWidget * _eventbox;
            GtkWidget * _object;
            GtkWidget * _layout;
            GtkWidget * _tag;
            GtkWidget * _parent;
            std::map<std::string, GtkWidget*> _ids;
            std::map<std::string, GtkTextBuffer*> _text_buffers;
            std::map<std::string, GtkEntryBuffer*> _entry_buffers;
            
            std::map<std::string, std::string> _types;
            std::string _selector;
            
            
            GtkCssProvider *_provider;
            GdkDisplay *_display;
            GdkScreen *_screen;
                
    };

    
    class console {
    public:
        void log( Gtk a ) {
        
            
        }
    };
    
}


#endif