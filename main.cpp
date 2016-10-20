#include <iostream>
#include <map>
#include <string>
#include "gtk.h"

//#include <regex>
//#include <functional>

using namespace std;


int main (int argc, char *argv[])
{
    static Gtk window;
    window.init( &argc, &argv ).set_title( "test ast" ).set_size( 500, 200 );

    //window.body( "", )
    //window.load_html("index.html");
    
    //window.html("<button id='button_id' class='test'>Test ast</button>");
    
    typedef std::map <std::string, std::string> attr;
    
    //load css
    window.link( attr {
        { "href", "css.css" },
    });
    
    
    //window.table();
    
    //create a table with tag method
   /* window.tag( "table" );
        window.tag("tr");
            window.tag("td"); 
                window.html("test ast");
            window.tag("/td");
            
            window.tag("td");
                window.html("test1 ast");
            window.tag("/td");
            
        window.tag("/tr");
    window.tag( "/table" );*/


    
    /*window.tag( "div", attr {
        { "class", "div" }, 
        { "id", "div1" },
    });
    
    window.label("test ast");
    
    window.tag( "/div" );
    */
    
    //window.div();
    //window.html("test ast");
    //window.enddiv();
    
    window.label("Result : asddddddddddddddddddd ",  attr { 
        { "class", "label" }, 
        { "id", "result" },
    } );
    

    //window("#result").html("test ast");
        
    window.br();

    window.label("kind : ",  attr { 
        { "class", "label" }, 
        { "id", "label_kind" },
    });


    window.select( attr{
        { "1", "test1" },
        { "2", "test2" }
        
    },  attr { 
        { "class", "select" }, 
        { "id", "kind" },
    });
    
    window.br();
    
    window.label("Username : ",  attr { 
        { "class", "label" }, 
        { "id", "label1" },
    });


    window.input("username...",  attr { 
        { "class", "input" }, 
        { "id", "username" },
    });
    
    window.br();

    window.label("Password : ",  attr { 
        { "class", "label" }, 
        { "id", "label2" },
    });

    window.input("password...",  attr { 
        { "class", "input" }, 
        { "id", "password" },
    });


    
    window.button("Login",  attr { 
        { "class", "foo" }, 
        { "id", "button_id" },
    });
    
    
    window("#username").mousedown( []() {
        if( window("#username").val() == "username..." ) {
            window("#username").val("");
        }
        
        
    });

    window("#username").mouseup( []() {
        window.alert("Test");
    });
    
    window("#password").mousedown( []() {
        window("#password").val("");
    });
    

    window("#kind").change( []() {
       // std::cout << "Test" << std::endl;
        
        window.alert( window("#kind").val() );
        
    });
    
    window("#button_id").click([]() {
        std::cout << " button id clicked " << std::endl;
        std::cout << window("#username").val() << std::endl;
        std::cout << window("#password").val() << std::endl;
        
        window("#result").html( "username : "+ window("#username").val()+" password : "+window("#password").val() );
        
       // window.alert("test ast");
        
    });
        
    return 0;
}