                            q�j ~      DWMV_JA_JPU6012.F                                                                                                                                                                                            p �                DWMV_JA_JPU6012.F&  BACKUP KA:[F]*.* DWMV_JA_JPU6012.F/SAV  SYSTEM            ��8��֟      V6.1	 	 _JRDCRX:: 
     _JRDCRX$DKA300:  V6.1   ~    
             �               * [F]DECBURGER.C;1 +  , ��   . l    /     4 X   l   \ j                   - zp   0  � 1    2   3      K � P   W   O ]    5   6 �����֟  7 ��~��֟  8          9          G    H �� J �  z            #ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)motifburger.c	1.7 90/04/02";   
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  ALL RIGHTS RESERVED
*         
*  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS  
*  
*  	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
*  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
*  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
*  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
*  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE.
*  
*  Notice:  Notwithstanding any other lease or license that may pertain to,
*  or accompany the delivery of, this computer software, the rights of the
*  Government regarding its use, reproduction and disclosure are as set
*  forth in Section 52.227-19 of the FARS Computer Software-Restricted
*  Rights clause.
*  
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.  Unpublished - all
*  rights reserved under the Copyright laws of the United States.
*  
*  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
*  Government is subject to the restrictions as set forth in subparagraph
*  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
*  at DFARS 52.227-7013.
*  
*  Open Software Foundation, Inc.
*  11 Cambridge Center
*  Cambridge, MA   02142
*  (617)621-8700
*  
*  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
*  "restricted rights."  Use, duplication or disclosure is subject to the
*  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
*  "Commercial Computer Software- Restricted Rights (April 1985)."  Open
*  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
*  the contract contains the Clause at 18-52.227-74 "Rights in Data General"
*  then the "Alternate III" clause applies.
*  
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.
*  ALL RIGHTS RESERVED 
*  
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
/*
 * A sample program which uses UIL and MRM to create the interface.
 */

#include <stdio.h>                              /* For printf and so on. */
#include <stdlib.h> 	    	    	    	/* needed for fopen */

#include <Xm/Text.h>      
#include <Mrm/MrmAppl.h>      
#include <DXm/DXmHelpB.h>      
#include <DXm/DXmPrint.h>      
#include <X11/Xlib.h>      
#include <X11/Xutil.h>      
#include <DXm/DXmColor.h>   
#include <DXm/DECspecific.h>
#include <sys$library/DECw$Cursor.h>

/*
 * These numbers are matched with corresponding numbers in the DECburger
 * UIL module.
 */

#define	k_create_order		1
#define	k_order_pdme		2
#define	k_file_pdme		3
#define	k_edit_pdme		4
#define	k_nyi			5
#define	k_ok			6	    /* NOTE: ok, apply, reset, cancel */
#define	k_apply			7	    /* must be sequential */
#define	k_reset			8
#define	k_cancel		9
#define	k_cancel_order		10
#define	k_submit_order		11
#define	k_order_box		12
#define k_burger_min		13
#define	k_burger_rare		13
#define	k_burger_medium		14
#define	k_burger_well		15
#define	k_burger_ketchup	16
#define	k_burger_mustard	17
#define	k_burger_onion		18
#define	k_burger_mayo		19
#define	k_burger_pickle		20
#define k_burger_max		20
#define	k_burger_quantity	21
#define	k_fries_tiny		22
#define	k_fries_small		23
#define	k_fries_medium		24
#define	k_fries_large		25
#define	k_fries_huge		26
#define	k_fries_quantity	27
#define	k_drink_list		28
#define	k_drink_add		29
#define	k_drink_sub		30
#define	k_drink_quantity	31
#define	k_total_order		32
#define	k_burger_label		33
#define	k_fries_label		34
#define	k_drink_label		35
#define	k_menu_bar		36
#define	k_file_menu		37
#define	k_edit_menu		38
#define	k_order_menu		39
#define	k_help_pdme		40
#define	k_help_menu		41
#define	k_help_overview		42
#define	k_help_about		43
#define	k_help_onhelp		44
#define	k_help_sensitive	45
#define	k_print			46
#define	k_options_pdme		47
#define	k_options_menu		48
#define	k_create_options	49
#define	k_fries_optionmenu	50

#define k_max_widget            50


#define MAX_WIDGETS (k_max_widget + 1)

#define NUM_BOOLEAN (k_burger_max - k_burger_min + 1)

#define k_burger_index   0
#define k_fries_index    1
#define k_drinks_index   2
#define k_index_count    3

/*
 * Global data
 */


/* Book file for help system */  

#define decburger_help  "decw$examples:decburger_help.decw$book"  

/* Global help system context */  

Opaque  help_context;                       
                                                       


static Cursor watch = NULL;

static Widget
    toplevel_widget = (Widget)NULL,	/* Root widget ID of our application. */
    main_window_widget = (Widget)NULL,	/* Root widget ID of main MRM fetch   */
    widget_array[MAX_WIDGETS],		/* Place to keep all other widget IDs */
    main_help_widget = (Widget)NULL,	/* Primary help widget		      */
    help_widget[MAX_WIDGETS],		/* Array of help widgets	      */
    help_array[MAX_WIDGETS],	        /* Array of help widgets for Toolkit  */
    print_widget = (Widget)NULL,	/* Print widget			      */
    color_widget = (Widget)NULL;	/* Color Mix widget		      */
  
static Screen	*the_screen;		/* Pointer to screen data*/
static Display	*the_display;		/* Pointer to display data */
static XColor  savecolor;  



static int help_num = 0;                /* make sure they start zero */   
static int low_num = 0;
  
static char toggle_array[NUM_BOOLEAN];	/* Our TRUTH about the state */
					/* of user interface toggles. */

static XmString
	current_drink,			/* Last selected drink name. */
	current_fries,			/* Last selected fries size. */
	name_vector[k_index_count];	/* Miscellaneous names gotten from */
					/* various widgets. */

static int quantity_vector[k_index_count];      /* Current quantities of */
                                                /* burger, fries, drinks. */

static long status;
static long bc;

static XmString latin_create;		/* Variables for */
static XmString latin_dismiss;		/* compound strings. */
static XmString latin_space;
static XmString latin_zero;
static MrmHierarchy s_MrmHierarchy;	/* MRM database hierarchy ID */
static MrmType *dummy_class;		/* and class variable. */
static char *db_filename_vec[] =	/* Mrm.heirachy file list. */
	{"decburger.uid"		/* There is only one UID file for */
	};				/* this application. */
static int db_filename_num =
	(sizeof db_filename_vec / sizeof db_filename_vec [0]);

/*
 * Forward declarations
 */

static void init_application();
static void s_error();
static void help_error();
static void get_something();
static void set_something();

static void activate_proc();
static void create_proc();
static void list_proc();
static void exit_proc();
static void pull_proc();
static void scale_proc();
static void show_hide_proc();
static void show_label_proc();
static void toggle_proc();
static void create_help();
static void tracking_help(); 
static void sens_help_proc();    
static void help_system_proc();    
static void create_print(); 
static void activate_print(); 
static void create_color();   
static void ok_color_proc(); 
static void apply_color_proc();   
static void cancel_color_proc();   
static void xmstring_append();
static void start_watch();
static void stop_watch();

/* The names and addresses of things that Mrm has to bind.  The names do
 * not have to be in alphabetical order.  */

static MrmRegisterArg reglist[] = {
    {"activate_proc", (caddr_t) activate_proc}, 
    {"create_proc", (caddr_t) create_proc}, 
    {"list_proc", (caddr_t) list_proc}, 
    {"pull_proc", (caddr_t) pull_proc}, 
    {"exit_proc", (caddr_t) exit_proc}, 
    {"scale_proc", (caddr_t) scale_proc}, 
    {"show_hide_proc", (caddr_t) show_hide_proc}, 
    {"show_label_proc", (caddr_t) show_label_proc}, 
    {"toggle_proc", (caddr_t) toggle_proc},
    {"sens_help_proc", (caddr_t) sens_help_proc},
    {"help_system_proc", (caddr_t) help_system_proc},
    {"cancel_color_proc", (caddr_t) cancel_color_proc},
    {"apply_color_proc", (caddr_t) apply_color_proc},
    {"ok_color_proc", (caddr_t) ok_color_proc}
};

static int reglist_num = (sizeof reglist / sizeof reglist [0]);
static font_unit = 400;

/*
 * OS transfer point.  The main routine does all the one-time setup and
 * then calls XtMainLoop.
 */

static String fallback =
    "DECburger.title: DECburger\nDECburger.x: 100\nDECburger.y: 100";

unsigned int main(argc, argv)
    unsigned int argc;                  /* Command line argument count. */
    char *argv[];                       /* Pointers to command line args. */
{
    XtAppContext app_context; 

    MrmInitialize();			/* Initialize MRM before initializing */
					/* the X Toolkit. */

    DXmInitialize();			/* Initialize DXm widgets */    

    /* If we had user-defined widgets, we would register them with Mrm.here. */

    XtSetLanguageProc (NULL, (XtLanguageProc)NULL, NULL);

    /* Initialize the X Toolkit. We get back a top level shell widget. */

    toplevel_widget = XtAppInitialize(
	&app_context,			/* App. context is returned */
	"DECburger",			/* Root class name. */
	NULL,				/* No option list. */
	0,				/* Number of options. */
	&argc,				/* Address of argc */
	argv,				/* argv */
	&fallback,			/* Fallback resources */
	NULL,				/* No override resources */
	0);				/* Number of override resources */

    /* Open the UID files (the output of the UIL compiler) in the hierarchy */

    if (MrmOpenHierarchy(
	db_filename_num,		/* Number of files. */
	db_filename_vec,		/* Array of file names.  */
	NULL,				/* Default OS extenstion. */
	&s_MrmHierarchy)		/* Pointer to returned MRM ID */
      !=MrmSUCCESS)
	s_error("can't open hierarchy");

    init_application();

    /* Register the items MRM needs to bind for us. */

    MrmRegisterNames(reglist, reglist_num);


    /* Go get the main part of the application. */
    if (MrmFetchWidget(s_MrmHierarchy, "S_MAIN_WINDOW", toplevel_widget,
      &main_window_widget, &dummy_class) != MrmSUCCESS)
        s_error("can't fetch main window");

    /* Save some frequently used values */
    the_screen = XtScreen(toplevel_widget); 
    the_display = XtDisplay(toplevel_widget);
    

    /* If it's a color display, map customize color menu entry */
 
    if ((XDefaultVisualOfScreen(the_screen))->class == TrueColor 
        ||  (XDefaultVisualOfScreen(the_screen))->class == PseudoColor
        ||  (XDefaultVisualOfScreen(the_screen))->class == DirectColor
        ||  (XDefaultVisualOfScreen(the_screen))->class == StaticColor)
                                    
          XtSetMappedWhenManaged(widget_array[k_options_pdme], TRUE);
                                

    /* Manage the main part and realize everything.  The interface comes up
     * on the display now. */

    XtManageChild(main_window_widget); 
    XtRealizeWidget(toplevel_widget);


    /* Set up Help System environment */
              
    DXmHelpSystemOpen(&help_context, toplevel_widget, decburger_help, 
                            help_error, "Help System Error");      
     

    /* Sit around forever waiting to process X-events.  We never leave
     * XtAppMainLoop. From here on, we only execute our callback routines. */
    XtAppMainLoop(app_context);
}
         
                  


/*
 * One-time initialization of application data structures.
 */

static void init_application()
{
    int k;
    int i;
    MrmCode data_type;
    XmString value;

    /* Initialize the application data structures. */
    for (k = 0; k < MAX_WIDGETS; k++)
        widget_array[k] = NULL;
    for (k = 0; k < NUM_BOOLEAN; k++)
        toggle_array[k] = FALSE;


    /* Initialize CS help widgets. */   

    for (i = 0; i < MAX_WIDGETS; i++)
        help_widget[i] = NULL;          
 

    /* Initialize help widgets for Toolkit creation. */   
           
    for (i = 0; i < MAX_WIDGETS; i++)
         help_array[i] = NULL;          
     

    /* Initialize print widgets. */   
        
    print_widget = NULL;                  
                                   

    /* Initialize color mix widget. */   
                        
    color_widget = NULL;                  



    /*  Set the medium 'hamburger doneness' toggle button so that the 
     *  radio box has one toggle button ON at startup. */

    toggle_array[k_burger_medium - k_burger_min] = TRUE;

    if (MrmFetchLiteral (s_MrmHierarchy, "k_drink_init",
		XtDisplay (toplevel_widget), &value, &data_type) == MrmSUCCESS){
	current_drink = XmStringCopy (value);
    }
    if (MrmFetchLiteral (s_MrmHierarchy, "k_fries_init",
		XtDisplay (toplevel_widget), &value, &data_type) == MrmSUCCESS){
	current_fries = XmStringCopy (value);
    }
    if (MrmFetchLiteral (s_MrmHierarchy, "k_create_init",
		XtDisplay (toplevel_widget), &value, &data_type) == MrmSUCCESS){
	latin_create = XmStringCopy (value);
    }
    if (MrmFetchLiteral (s_MrmHierarchy, "k_dismiss_init",
		XtDisplay (toplevel_widget), &value, &data_type) == MrmSUCCESS){
	latin_dismiss = XmStringCopy (value);
    }

    /* Set up the compound strings that we need. */
    latin_space = DXmCvtFCtoCS(" ", &bc, &status);
    latin_zero = DXmCvtFCtoCS(" 0", &bc, &status);
}
                    
/***************************************************************************
 *
 * These are some little utilities used by the callback routines.
 */

                  
/*
 * All errors are fatal.
 */

static void s_error(problem_string)
    char    *problem_string;
{       
    printf("%s\n", problem_string);
    exit(0);
}           

/*
 * Help System errors are also fatal.
 */
                       
static void help_error(problem_string, status)
    char    *problem_string;               
    int     status;

{
    printf("%s, %x\n", problem_string, status);
    exit(0);
}


/*
 * Simplified SET VALUE routine to use only when changing a single attribute.
 * If we need to change more than one, all new values should be put 
 * into one arglist and we should make one XtSetValues call (which is MUCH 
 * more efficient).
 */

static void set_something(w, resource, value)
    Widget	w;
    char	*resource, *value;
{
    Arg al[1]; 

    XtSetArg(al[0], resource, value);
    XtSetValues(w, al, 1);
}


/*
 * Simplified GET VALUE routine to use only when retrieving a single attribute.
 * If we need to retrieve more than one, all values should be put 
 * into one arglist and we should make one XtGetValues call (which is MUCH 
 * more efficient).
 */

static void get_something(w, resource, value)
    Widget	w;
    char	*resource, *value;
{
    Arg al[1];
    
    XtSetArg(al[0], resource, value);
    XtGetValues(w, al, 1);
}


/*
 * Keep our boolean array current with the user interface toggle buttons.
 */

static void set_boolean(i, state)
    int	    i;			    /* Widget ID index. */
    int	    state;
{
    toggle_array[i - k_burger_min] = state;

    XmToggleButtonSetState(widget_array[i], 
                                        /* Which widget */
      state,                            /* state it should have. */
      FALSE);                           /* Do not call me back now. */
}



/*
 * Format and update the drink quantity widget.
 */

static void update_drink_display()
{
    char	drink_txt[4];
    XmString	label;

    sprintf(drink_txt, "%2.2d", quantity_vector[k_drinks_index]);
    label = DXmCvtFCtoCS(drink_txt, &bc, &status);
    set_something(widget_array[k_drink_quantity], XmNlabelString, label);
    XmStringFree(label);
}





/* Creates an instance of the help widget for the push buttons in the help
pull-down menu and for context-sensitive help callbacks. */

static void create_help (help_topic)
    XmString	help_topic;
                        
{                  
    Arg             arglist[1];
                          
    start_watch();
    if (!main_help_widget) {                                    
 	if (MrmFetchWidget (s_MrmHierarchy, "main_help", toplevel_widget,
		  	    &main_help_widget, &dummy_class) != MrmSUCCESS)
	s_error ("can't fetch help widget");     
    }      


    if (XtIsManaged(main_help_widget)) {   
       
        if (MrmFetchWidget (s_MrmHierarchy, "main_help", toplevel_widget,
	    	  	    &help_widget[help_num], &dummy_class) != MrmSUCCESS)
	s_error ("can't fetch help widget");
           
        XtSetArg (arglist[0], DXmNfirstTopic, help_topic);
        XtSetValues (help_widget[help_num], arglist, 1);       
        XtManageChild(help_widget[help_num]);    
        help_num++;   
    	stop_watch();
        return; 
    }   
      
    XtSetArg (arglist[0], DXmNfirstTopic, help_topic);
    XtSetValues (main_help_widget, arglist, 1);
    XtManageChild(main_help_widget);     
    stop_watch();
}              
      

/* Switches DECburger into context-sensitive mode and calls the selected
** widget's context-sensitive help callback
*/

static void tracking_help()
{          
      DXmHelpOnContext(toplevel_widget, FALSE);	
}


    


/* Print Widget Creation */

static void create_print() 
{                  
    unsigned int	ac;
    Arg			arglist[10];         
    XtCallbackRec	callback_arg[2];
    
    start_watch();
    
    if (!print_widget) {
                      
	if (MrmFetchWidget (s_MrmHierarchy, "main_print", toplevel_widget,
		  	    &print_widget, &dummy_class) != MrmSUCCESS)
   	s_error ("can't fetch print widget");
            
        callback_arg[0].callback = activate_print;
        callback_arg[0].closure = 0;
        callback_arg[1].callback = NULL;
        callback_arg[1].closure = NULL;
 
        ac = 0;                       
        XtSetArg (arglist[ac], XmNokCallback, callback_arg);ac++;
        XtSetArg (arglist[ac], DXmNsuppressOptionsMask, 
                DXmSUPPRESS_DELETE_FILE | DXmSUPPRESS_OPERATOR_MESSAGE); ac++;
        XtSetValues (print_widget, arglist, ac);      
    }      

    XtManageChild(print_widget);   
    stop_watch();
}                                     
            
     

        
/* Color Mixing Widget Creation.*/

static void create_color()                   
{                                         

    XColor		newcolor;   
    unsigned int	ac;
    Arg			arglist[10];  

    start_watch();

    if (!color_widget) {        
                      
	if (MrmFetchWidget (s_MrmHierarchy, "main_color", toplevel_widget,
			    &color_widget, &dummy_class) != MrmSUCCESS)
	s_error ("can't fetch color mix widget");

	XtSetArg(arglist[0], XmNbackground, &newcolor.pixel);
	XtGetValues(main_window_widget, arglist, 1);
     
	XQueryColor(the_display,
		    XDefaultColormapOfScreen(the_screen), &newcolor);
           
	ac = 0;                          
	XtSetArg (arglist[ac], DXmNorigRedValue, newcolor.red); ac++;		
	XtSetArg (arglist[ac], DXmNorigGreenValue, newcolor.green); ac++; 
	XtSetArg (arglist[ac], DXmNorigBlueValue, newcolor.blue); ac++;
	XtSetValues(color_widget, arglist, ac);     

	savecolor.red = newcolor.red;
	savecolor.green = newcolor.green;
	savecolor.blue = newcolor.blue;
	savecolor.pixel = newcolor.pixel;
    
    } else {

        XtSetArg(arglist[0], XmNbackground, &savecolor.pixel);
        XtGetValues(main_window_widget, arglist, 1);
      
        XQueryColor(the_display,
		    XDefaultColormapOfScreen(the_screen), &savecolor);
    }
            
    XtManageChild(color_widget);    
    stop_watch();
}                                     

                    
                                  
            
/***************************************************************************
 *
 * This section contains callback routines.
 */     
        
                                   
       
/*
 * Reset the user interface and the application to a known state.
 */

static void reset_values()
{
    int i;

    /*  Reset the toggle buttons and our boolean array. */
    for (i = k_burger_min; i <= k_burger_max; i++) {
                    
        /* The radio box requires that one button be set; we choose medium. */

        set_boolean(i, (i == k_burger_medium));
    }

    /* Reset the burger quantity scale widget and global value. */
    set_something(widget_array[k_burger_quantity], XmNvalue, 0);
    quantity_vector[k_burger_index] = 0;

    /* Reset the fries option menu */
    set_something(widget_array[k_fries_optionmenu], XmNmenuHistory,
		    widget_array[k_fries_medium]);

    /* Reset the fries quantity text widget.  We do not have a global for this.
     * We read the widget whenever we need to know the quantity. */
    XmTextSetString(widget_array[k_fries_quantity], " 0");

    /* Reset the drinks quantity text widget and global value. */
    set_something(widget_array[k_drink_quantity], XmNlabelString, latin_zero);
    quantity_vector[k_drinks_index] = 0;
}


static void apply_values()
{
    char	list_buffer[20];
    XmString	txt, list_txt;
    char	*fries_text;       
    int		i, fries_num;

/*  Take the current settings and write them into the list box.  */

/* Burgers */

    if (quantity_vector[k_burger_index] > 0) {

	/* Put burger quantity in the display string. */
	sprintf(list_buffer, "%d ", quantity_vector[k_burger_index]);
	list_txt = DXmCvtFCtoCS(list_buffer, &bc, &status);

	/* Collect hambuger attributes that are ON. */
	for (i = k_burger_min; i <= k_burger_max; i++)
	    if (toggle_array[i - k_burger_min]) {

		/*  Get the name of the qualifier from the widget and
		 *  add to the display string.
		 */
		get_something(widget_array[i], XmNlabelString, &txt);
		xmstring_append(&list_txt, txt);
		XmStringFree(txt);
		xmstring_append(&list_txt, latin_space);
	    }
                                   
	/* Add hamburger name and add to the display string. */
	xmstring_append(&list_txt, name_vector[k_burger_index]);

	XmListAddItem(widget_array[k_total_order], list_txt, 0);
	XmStringFree(list_txt);
    }                                

/* Fries */
    /*  Text widget does not have a callback.  So we query the widget
     *  now to determine what its value is.  Convert to an integer.
     */
    fries_num = 0;
    fries_text = XmTextGetString(widget_array[k_fries_quantity]);
    sscanf(fries_text, "%d", &fries_num);

    if (fries_num != 0) {    

	/* Put the fries quantity in the display string. */
	sprintf(list_buffer, "%d ", fries_num);
	list_txt = DXmCvtFCtoCS(list_buffer, &bc, &status);

	/*  Get all the qualifiers for the fries. */
	xmstring_append(&list_txt, current_fries);
	xmstring_append(&list_txt, latin_space);

	/* Add fries name and display. */
	xmstring_append(&list_txt, name_vector[k_fries_index]);

	XmListAddItem(widget_array[k_total_order], list_txt, 0);
    }                                

/* Drinks */
    if (quantity_vector[k_drinks_index] > 0) {

	/* Put drinks quantity into the display string. */
	sprintf(list_buffer, "%d ", quantity_vector[k_drinks_index]);
	list_txt = DXmCvtFCtoCS(list_buffer, &bc, &status);

	/*  Now get the qualifiers for the drinks. */
	xmstring_append(&list_txt, current_drink);
	xmstring_append(&list_txt, latin_space);

	/* Add the drink name to the display string. */

	xmstring_append(&list_txt, name_vector[k_drinks_index]);

	XmListAddItem(widget_array[k_total_order], list_txt, 0);
    }
}

/*
 * Clear the order display area in the main window.
 */

static void clear_order()
{                               
    Arg arglist[5];
    int ac = 0;

    XtSetArg(arglist[ac], XmNitemCount, 0);
    ac++;

    XtSetArg(arglist[ac], XmNitems, NULL);
    ac++;

    XtSetArg(arglist[ac], XmNselectedItemCount, 0);
    ac++;

    XtSetArg(arglist[ac], XmNselectedItems, NULL);
    ac++;

    XtSetValues(widget_array[k_total_order], arglist, ac);       
}


/*
 * All push buttons in this application call back to this routine.  We
 * use the tag to tell us what widget it is, then react accordingly.
 */

static void activate_proc(w, tag, reason)
    Widget		w;
    int			*tag;
    XmAnyCallbackStruct *reason;
{
    int		widget_num = *tag;	/* Convert tag to widget number. */
    int		i, value;
    XmString	topic;    

    switch (widget_num) {
        case k_nyi: 
            /* The user activated a 'not yet implemented' push button.  Send
             * the user a message. */
            if (widget_array[k_nyi] == NULL) {
		/* The first time, fetch from the data base. */
                if (MrmFetchWidget(s_MrmHierarchy, "nyi", toplevel_widget,
                  &widget_array[k_nyi], &dummy_class) != MrmSUCCESS) {
                    s_error("can't fetch nyi widget");
                }
            }
            /*  Put up the message box saying 'not yet implemented'. */
            XtManageChild(widget_array[k_nyi]);
            break;
                
        case k_submit_order: 
            /*  This would send the order off to the kitchen. In this case,
             *  we just pretend the order was submitted. */
            clear_order();
            break;

        case k_cancel_order: 
            /*  Clear out the order display. */
            clear_order();
            break;

	case k_ok:
	    /* Set the values and bring down the control box */
	    XtUnmanageChild(widget_array[k_order_box]);
	    apply_values();
	    break;

	case k_apply:
	    /* Set the values, leave the box up */
	    apply_values();
	    break;

        case k_reset: 
	    /* Reset the values, leave the box up */
            reset_values();
            break;

        case k_cancel: 
            /* Reset the values, take the box down */
            XtUnmanageChild(widget_array[k_order_box]);
	    reset_values();
	    break;


        case k_fries_tiny:
        case k_fries_small: 
        case k_fries_medium: 
        case k_fries_large: 
        case k_fries_huge: 

            /*  Some fries size push button was activated, so get the string 
             *  from the interface.  Helps with internationalization. */
	    XmStringFree(current_fries);
            get_something(w, XmNlabelString, &current_fries);
            break;

        case k_drink_add: 

            /*  Increment the drink quantity and update the display. */
            quantity_vector[k_drinks_index]++;
            update_drink_display();
            break;

        case k_drink_sub: 

            /*  Decrement drink quantity, but do not let it go below zero. */
            if (quantity_vector[k_drinks_index] > 0)
                quantity_vector[k_drinks_index]--;
            update_drink_display();
            break;
            
         case k_help_overview:
            topic = DXmCvtFCtoCS("overview", &bc, &status);
            create_help(topic);
	    XmStringFree(topic);
	    break;      
          
          case k_help_about:
            topic = DXmCvtFCtoCS("about", &bc, &status);   
            create_help(topic);
	    XmStringFree(topic);
	    break;      
          
          case k_help_onhelp:
            topic = DXmCvtFCtoCS("onhelp", &bc, &status);
            create_help(topic);          
	    XmStringFree(topic);
     	    break;   
            
          case k_help_sensitive:
            tracking_help(); 
     	    break;      
                  
         case k_print:
            create_print();
     	    break;              
            
         case k_create_options:
            create_color();
     	    break;                  

         default: 
            break;
    }
}




/*
 * The toggle buttons which control the 'hamburger doneness' and toppings
 * call back to this procedure when they change state.  Use the
 * tag to index into the boolean array.  Just keep the booleans current
 * with the user interface.
 */

static void toggle_proc(w, tag, toggle)
    Widget w;
    int *tag;
    XmToggleButtonCallbackStruct *toggle;
{
    toggle_array[*tag - k_burger_min] = toggle->set;
}




/*
 * The drink selection list box calls back to this procedure whenever the
 * user selects a drink.  Just keep the global current drink up to date.
 */

static void list_proc(w, tag, list)
    Widget w;
    int *tag;
    XmListCallbackStruct *list;
{
    XmStringFree(current_drink);
    current_drink = XmStringCopy(list->item);
}



/*
 * The hamburger quantity scale widget will call back to this procedure whenever
 * the user changes it.  Just keep the global hamburger quantity up to date.
 */

static void scale_proc(w, tag, scale)
    Widget w;
    int *tag;
    XmScaleCallbackStruct *scale;
{
    quantity_vector[k_burger_index] = scale->value;
}





/*
 * The next two procedures put up and take down the order box and change
 * the label in the pulldown menu from Create to Dismiss.
 */


/*
 * The user selected the Order push button in the control pulldown menu.
 * We just change the state of the order box.  If the order box is 
 * currently displayed (managed), then remove (unmanage) it.  Otherwise,
 * we manage the order box.
 */

static void show_hide_proc(w, tag, reason)
    Widget		w;
    int			*tag;
    XmAnyCallbackStruct	*reason;
{
    if (XtIsManaged(widget_array[k_order_box]))
        XtUnmanageChild(widget_array[k_order_box]);
    else {
	start_watch();
        XtManageChild(widget_array[k_order_box]);
	DXmFormSpaceButtonsEqually (widget_array[k_order_box],
				    &widget_array[k_ok], 4);
	stop_watch();
    }
}


/*
 * This callback runs as the control pulldown menu is about to be pulled down.
 * We use this opportunity to fetch the order box (if not done already)
 * and to make sure the push button displays the correct label.
 */

static void show_label_proc(w, tag, reason)
    Widget		w;
    int			*tag;  
    XmAnyCallbackStruct	*reason;
{
    if (widget_array[k_order_box] == NULL)
                                        /* The first time, fetch order box. */
      {        
        if (MrmFetchWidget(s_MrmHierarchy, "control_box", toplevel_widget,
          &widget_array[k_order_box], &dummy_class) != MrmSUCCESS) {
            s_error("can't fetch order box widget");
        }     
    }

    /* Now figure out what the label on the push button in the pulldown menu 
     * should be. */

    if (XtIsManaged(widget_array[k_order_box]))
        set_something(widget_array[k_create_order], XmNlabelString, latin_dismiss);
    else
        set_something(widget_array[k_create_order], XmNlabelString, latin_create);
}







/*
 * All widgets that are created call back to this procedure. We just log the 
                                                                                                                                                                                                                                                                                                                                            B� ~      DWMV_JA_JPU6012.F              ��  zp [F]DECBURGER.C;1                                                                                                               X     l                         }  >     >       * ID in the global array.
 */

static void create_proc(w, tag, reason)
    Widget		w;
    int			*tag;
    XmAnyCallbackStruct	*reason;
{
    int widget_num = *tag;

    widget_array[widget_num] = w;

    /*  For internationalization ease, we capture a few strings from the
     *  widgets themselves.  We could go out and fetch them as needed but
     *  since we use these all the time, this method if more efficient.
     */
    switch (widget_num) {
        case k_burger_label: 
            get_something(w, XmNlabelString, &name_vector[k_burger_index]);
            break;

        case k_fries_label: 
            get_something(w, XmNlabelString, &name_vector[k_fries_index]);
            break;

        case k_drink_label: 
            get_something(w, XmNlabelString, &name_vector[k_drinks_index]);
            break;

	case k_nyi:
	    XtUnmanageChild(XmMessageBoxGetChild(w, XmDIALOG_CANCEL_BUTTON));
	    XtUnmanageChild(XmMessageBoxGetChild(w, XmDIALOG_HELP_BUTTON));
	    break;

        default: 
            break;
    }
}




/*
 * The user pushed the exit button, so the application exits.
 */
static void exit_proc(w, tag, reason)
    Widget		w;
    char		*tag;
    XmAnyCallbackStruct *reason;
{           
    if (tag != NULL) 
        printf("Exit - %s\n", tag);

     /* Close the Help System */  
    DXmHelpSystemClose(help_context, help_error, "Help System Error");  

    exit(1);
}           



/*                   
 * Context sensitive help callback.
 */                               
static void sens_help_proc(w, tag, reason)
    Widget              w;
    XmString		tag;
    XmAnyCallbackStruct *reason;    
{
     create_help(tag);            
}




/* Creates a Help System session */
  

static void help_system_proc(w, tag, reason)
    Widget              w;
    int                 *tag;
    XmAnyCallbackStruct *reason; 
                        
{                               
DXmHelpSystemDisplay(help_context, decburger_help, "topic", tag,
                            help_error, "Help System Error");    
}                    

                         



/*
 * This callback runs just as a pulldown menu is about to be pulled down.
 * It fetches the menu if it is currently empty, and does other
 * special processing as required.
 * We use this opportunity to fetch the order box (if not done already)
 * and to make sure the push button displays the correct label.
 */

static void pull_proc(w, tag, reason)
    Widget		w;
    int			*tag;
    XmAnyCallbackStruct	*reason;
{
    int widget_num = *tag;

    switch (widget_num) {
        case k_file_pdme: 
            if (widget_array[k_file_menu] == NULL) {
                if (MrmFetchWidget(s_MrmHierarchy, "file_menu", widget_array[
                  k_menu_bar], &widget_array[k_file_menu], &dummy_class) != 
                  MrmSUCCESS)
                    s_error("can't fetch file pulldown menu widget");
                set_something(widget_array[k_file_pdme], XmNsubMenuId,
                  widget_array[k_file_menu]);
            }
            break;
    
        case k_edit_pdme: 
            if (widget_array[k_edit_menu] == NULL) {
                if (MrmFetchWidget(s_MrmHierarchy, "edit_menu", widget_array[
                  k_menu_bar], &widget_array[k_edit_menu], &dummy_class) !=
                  MrmSUCCESS)
                    s_error("can't fetch edit pulldown menu widget");
                set_something(widget_array[k_edit_pdme], XmNsubMenuId,
                  widget_array[k_edit_menu]);
            }
            break;
      
        case k_order_pdme:
	    start_watch();      

            if (widget_array[k_order_menu] == NULL) {
                if (MrmFetchWidget(s_MrmHierarchy, "order_menu", widget_array[
                  k_menu_bar], &widget_array[k_order_menu], &dummy_class) !=
                  MrmSUCCESS)
                    s_error("can't fetch order pulldown menu widget");
                set_something(widget_array[k_order_pdme], XmNsubMenuId,
                  widget_array[k_order_menu]);
            }

            /* Figure out what the label of the push button in the pulldown
             * menu should be. */

            if ( widget_array[k_order_box] == NULL )
                if (MrmFetchWidget (
                     s_MrmHierarchy,
                     "control_box",
                     toplevel_widget,
                     &widget_array [k_order_box],
                     &dummy_class) != MrmSUCCESS) 
                       s_error ("can't fetch order box widget");       
            if (XtIsManaged(widget_array[k_order_box]))
                set_something(widget_array[k_create_order], XmNlabelString,
                  latin_dismiss);
            else
                set_something(widget_array[k_create_order], XmNlabelString,
                  latin_create);

	    stop_watch();
            break;

    }

}
                                

/* Print Widget OK Callback */
            
static void activate_print(w, tag, reason) 
     Widget                  w;
     int                     *tag;        
     XmAnyCallbackStruct     *reason;    
     
{     
    unsigned long int	l_status;		    
    XmString            file_pointer[1];
    int			l_num_names,l_i;
    char		at_buffer[30];
    FILE                *fp;

    if ((fp = fopen("order.txt", "w")) != NULL) {

     fprintf(fp, "Function Not Yet Implemented\n");
     fclose(fp);                  

     file_pointer[0] = XmStringCreateLtoR("order.txt", XmSTRING_OS_CHARSET);
             
     l_status = DXmPrintWgtPrintJob(print_widget, file_pointer, 1);
 
     printf("DXmPrintWgtPrintJob return status: %x\n",l_status);
                         
    }

}

   
    
/* Color Mix OK Callback */
               
static void ok_color_proc(widget_id, tag, reason)
   Widget                     widget_id;
   int                        *tag;        
   DXmColorMixCallbackStruct  *reason;
{                        
   int         ac;
   Arg         arglist[10];
   XColor      newcolor;
   
   newcolor.red = reason->newred;
   newcolor.green = reason->newgrn;
   newcolor.blue = reason->newblu;
        
   if (XAllocColor(the_display,
                  XDefaultColormapOfScreen(the_screen), &newcolor)) {

	ac = 0;                            
	XtSetArg (arglist[ac], XmNbackground, newcolor.pixel);ac++;   
	XtSetValues(widget_array[k_total_order], arglist, ac);
	XtSetValues(main_window_widget, arglist, ac);
    }         
            
    else 
        s_error ("can't allocate color cell");       
   
    XtUnmanageChild(color_widget);     

    ac = 0;                     
    XtSetArg (arglist[ac], DXmNorigRedValue, newcolor.red);ac++;	     	
    XtSetArg (arglist[ac], DXmNorigGreenValue, newcolor.green);ac++; 
    XtSetArg (arglist[ac], DXmNorigBlueValue, newcolor.blue);ac++;          
    XtSetValues(color_widget, arglist, ac);     
}
             

/* Color Mix Apply Callback */
                      
static void apply_color_proc(widget_id, tag, reason)
    Widget			widget_id;
    int				*tag;        
    DXmColorMixCallbackStruct	*reason;

{                        
    int		ac;
    Arg		arglist[10];
    XColor	newcolor;
  
    newcolor.red = reason->newred;
    newcolor.green = reason->newgrn;
    newcolor.blue = reason->newblu;
    
    if (XAllocColor(the_display,
                  XDefaultColormapOfScreen(the_screen), &newcolor)) {
	ac = 0;                            
	XtSetArg (arglist[ac], XmNbackground, newcolor.pixel);ac++;   
	XtSetValues(widget_array[k_total_order], arglist, ac);
	XtSetValues(main_window_widget, arglist, ac);
    }         

    else 
        s_error ("can't allocate color cell");       
             
}




/* Color Mix Cancel Callback */
               
static void cancel_color_proc(widget_id, tag, reason)
    Widget			widget_id;
    int				*tag;        
    DXmColorMixCallbackStruct	*reason;

{                        

    int         ac;
    Arg         arglist[10];

    XtUnmanageChild(color_widget);     
        
    ac = 0;                            
    XtSetArg (arglist[ac], XmNbackground, savecolor.pixel);ac++;   
    XtSetValues(widget_array[k_total_order], arglist, ac);
        
    XtSetValues(main_window_widget, arglist, ac);
  
}                   
                    

/* append string2 to string1 without losing memory */
static void xmstring_append (string1, string2)
    XmString	*string1;
    XmString	string2;
{
    XmString	xmtemp;

    xmtemp = XmStringConcat(*string1, string2);
    XmStringFree(*string1);
    *string1 = xmtemp;                            
}
        
static void start_watch()   
{
	if (watch == (Cursor)NULL)
	    watch = DXmCreateCursor(main_window_widget, decw$c_wait_cursor);
	XDefineCursor(  the_display, XtWindow(main_window_widget), watch );
	XFlush( the_display );
}


static void stop_watch()
{
	XUndefineCursor(the_display, XtWindow(main_window_widget));
}




/* Toolkit Print Widget Example.  Documented, but not needed   
 *  
 *    callback_arg[0].callback = activate_print;
 *    callback_arg[0].closure = 0;
 *    callback_arg[1].callback = NULL;
 *    callback_arg[1].closure = NULL;
 *
 *    num_copies = 2;        
 *
 *    ac = 0;                          
 *                                     
 *       XtSetArg (arglist[ac], DXmNnumberCopies, num_copies); ac++;  
 *       XtSetArg (arglist[ac], DXmNunmanageOnOk, TRUE); ac++;  
 *       XtSetArg (arglist[ac], DXmNunmanageOnCancel, TRUE); ac++;  
 *       XtSetArg (arglist[ac], XmNokCallback, callback_arg);ac++;
 *       XtSetArg (arglist[ac], DXmNsuppressOptionsMask,
 *            DXmSUPPRESS_DELETE_FILE | DXmSUPPRESS_OPERATOR_MESSAGE); ac++;
 *                          
 *       print_widget = DXmCreatePrintDialog (toplevel_widget,
 *	       		                "Print Widget",
 *   	       	       	                 arglist, ac);       
 *                   
 *       XtManageChild(print_widget);       
 *       return; 
 *
 *      }                                  
 */

        


/* Toolkit Color Mix Widget Example.  Documented, but not needed */  

/*
 * static void create_color()                   
 * {                                         
 *          unsigned int        ac;
 *          Arg                 arglist[10];         
 *          XtCallbackRec       ok_callback_arg[2];
 *          XtCallbackRec       apply_callback_arg[2];
 *          XtCallbackRec       cancel_callback_arg[2];
 *          XColor              newcolor;   
 *          Arg                 al[1];                     
 * 
 *          if (!color_widget) {
 *                               
 *          apply_callback_arg[0].callback = apply_color_proc;
 *          apply_callback_arg[0].closure = 0;     
 *          apply_callback_arg[1].callback = NULL;
 *          apply_callback_arg[1].closure = NULL;
 *      
 *          cancel_callback_arg[0].callback = cancel_color_proc;
 *          cancel_callback_arg[0].closure = 0;    
 *          cancel_callback_arg[1].callback = NULL;
 *          cancel_callback_arg[1].closure = NULL;
 *      
 *          ok_callback_arg[0].callback = ok_color_proc;
 *          ok_callback_arg[0].closure = 0;          
 *          ok_callback_arg[1].callback = NULL;
 *          ok_callback_arg[1].closure = NULL;
 * 
 * 
 *          XtSetArg(al[0], XmNbackground, &newcolor.pixel);
 *          XtGetValues(main_window_widget, al, 1);
 * 
 *     
 *          XQueryColor(XtDisplay(toplevel_widget), 
 *                   XDefaultColormapOfScreen(the_screen), &newcolor);
 *           
 *          ac = 0;                          
 *                  
 *          XtSetArg (arglist[ac], XmNcancelCallback, cancel_callback_arg);ac++;
 *          XtSetArg (arglist[ac], XmNokCallback, ok_callback_arg); ac++; 
 *          XtSetArg (arglist[ac], XmNapplyCallback, apply_callback_arg); ac++;
 *          XtSetArg (arglist[ac], DXmNorigRedValue, newcolor.red); ac++;		
 *          XtSetArg (arglist[ac], DXmNorigGreenValue, newcolor.green); ac++; 
 *          XtSetArg (arglist[ac], DXmNorigBlueValue, newcolor.blue); ac++;          
 *          XtSetArg (arglist[ac], DXmNcolorModel, DXmColorModelPicker); ac++;          
 *                                                 
 *          color_widget = DXmCreateColorMixDialog (toplevel_widget, 
 *                                                "Color Mix Widget",
 *       	       	       	                 arglist, ac);       
 *
 *          savecolor.red = newcolor.red;
 *          savecolor.green = newcolor.green;
 *          savecolor.blue = newcolor.blue;
 *
 *          XtManageChild(color_widget);       
 *          return;                    
 *       }                                  
 *                              
 *          XtSetArg(arglist[0], XmNbackground, &savecolor.pixel);
 *          XtGetValues(main_window_widget, arglist, 1);
 *     
 *          XQueryColor(XtDisplay(toplevel_widget), 
 *          XDefaultColormapOfScreen(the_screen), &savecolor);
 *
 *          XtManageChild(color_widget);   
 *
 *      }     
 * 
 */


/* Toolkit help creation routine.  Documented, but not needed. */ 

/*
 * static void create_help (topic)
 *           XmString   topic;
 * {
 *
 *   unsigned int    ac;
 *   Arg             arglist[10];
 *   XmString        appname, glossarytopic, overviewtopic, libspec;
 *   static Widget   help_widget = NULL;
 * 
 *
 *     if (!help_widget) {
 *     ac = 0;
 *     appname = XmStringCreateLtoR("Toolkit Help", XmSTRING_ISO8859_1);
 *     glossarytopic = XmStringCreateLtoR("glossary", XmSTRING_ISO8859_1);
 *     overviewtopic = XmStringCreateLtoR("overview", XmSTRING_ISO8859_1);
 *     libspec = XmStringCreateLtoR("decburger.hlb", XmSTRING_ISO8859_1);
 *          
 *     XtSetArg(arglist[ac], DXmNapplicationName, appname); ac++;
 *     XtSetArg(arglist[ac], DXmNglossaryTopic, glossarytopic); ac++;
 *     XtSetArg(arglist[ac], DXmNoverviewTopic, overviewtopic); ac++;
 *     XtSetArg(arglist[ac], DXmNlibrarySpec, libspec); ac++;
 *     XtSetArg(arglist[ac], DXmNfirstTopic, topic); ac++;
 *
 *     help_widget = DXmCreateHelpDialog (toplevel_widget,
 *                                        "Toolkit Help",
 *                                         arglist, ac);
 *     XmStringFree(appname);
 *     XmStringFree(glossarytopic);
 *     XmStringFree(overviewtopic);
 *     XmStringFree(libspec);
 *
 *     XtManageChild(help_widget);
 *
 *     return;
 *   }
 *
 *     if (XtIsManaged(help_widget)) {
 *     ac = 0;
 *     appname = XmStringCreateLtoR("Toolkit Help", XmSTRING_ISO8859_1);
 *     glossarytopic = XmStringCreateLtoR("glossary", XmSTRING_ISO8859_1);
 *     overviewtopic = XmStringCreateLtoR("overview", XmSTRING_ISO8859_1);
 *     libspec = XmStringCreateLtoR("decburger.hlb", XmSTRING_ISO8859_1);
 *
 *     XtSetArg(arglist[ac], DXmNapplicationName, appname); ac++;
 *     XtSetArg(arglist[ac], DXmNglossaryTopic, glossarytopic); ac++;
 *     XtSetArg(arglist[ac], DXmNoverviewTopic, overviewtopic); ac++;
 *     XtSetArg(arglist[ac], DXmNlibrarySpec, libspec); ac++;
 *     XtSetArg(arglist[ac], DXmNfirstTopic, topic); ac++;
 *
 *     help_array[low_num] = DXmCreateHelpDialog (toplevel_widget,
 *                                                "Toolkit Help",
 *                                                 arglist, ac);
 *     XmStringFree(appname);
 *     XmStringFree(glossarytopic);
 *     XmStringFree(overviewtopic);
 *     XmStringFree(libspec);
 *
 *     XtManageChild(help_array[low_num]);
 *     low_num++;
 *     return;
 *   }
 *
 *   ac = 0;
 *   XtSetArg (arglist[ac], DXmNfirstTopic, topic); ac++;
 *   XtSetValues (help_widget, arglist, ac);
 *   XtManageChild(help_widget);
 * }
 */

  

                 

                                                                                                                                                      �               * [F]DECBURGER.COM_US_RENAME;1 +  , ҹ	   .     /     4 W      	 �                    - zp   0  � 1    2   3      K � P   W   O 
    5   6 �`���֟  7 �Ł��֟  8          9          G    H �� J �                  " $ ! Build the Motif DECburger DemoG $ ! The files decburger.uid and decburger.exe will be produced by this   $ ! command procedure. $ ! K $ ! Make sure that the prerequisite files and logicals exist.  This command L $ ! procedure requires that the DECwindows programming and user environmentsM $ ! have been installed and that the DECW$STARTUP command procedure has been   $ ! invoked.   $ ! " $ arch_type = f$getsyi("hw_model") $ if arch_type .le. 1023 $ then $    arch_type = "VAX" $ else $    arch_type = "AXP" $ endif J $ if f$search("sys$system:decw$uilmotif.exe") .nes. "" then goto libskitokF $ write sys$output "DECwindows programming environment not installed."2 $ write sys$output "DECburger demo build aborted." $ exit $libskitok: < $ if f$search("sys$share:decw$xlibshr.exe") .nes. ""	.and. -J      f$search("sys$share:decw$dxmlibshr.exe") .nes. ""	then goto appskitok? $ write sys$output "DECwindows user environment not installed." 2 $ write sys$output "DECburger demo build aborted." $ exit $appskitok: . $ if f$trnlnm("decw$include") .nes. ""	.and. -;      f$trnlnm("decw$examples") .nes. ""	then goto startupok O $ write sys$output "You must invoke @DECW$STARTUP before using this procedure." 2 $ write sys$output "DECburger demo build aborted." $ exit $startupok:  $ if arch_type .eqs. "VAX" $ thenF $     if f$search("sys$system:vaxc.exe") .nes. "" then goto compilerok8 $     write sys$output "VAXC compiler is not available."6 $     write sys$output "DECburger demo build aborted."
 $     exit $ elseO $     if f$search("sys$system:decc$compiler.exe") .nes. "" then goto compilerok 9 $     write sys$output "DEC C compiler is not available." 6 $     write sys$output "DECburger demo build aborted."
 $     exit $ endif  $compilerok: $ if arch_type .eqs. "VAX" $ thenC $     if f$search("sys$library:stdio.h") .nes. "" then goto stdiook > $     write sys$output "SYS$LIBRARY:STDIO.H is not available."4 $     write sys$output "Create it with the command:"W $     write sys$output " $ LIBRARY/TEXT/EXTRACT=STDIO/OUT=SYS$COMMON:[SYSLIB]STDIO.H -" 0 $     write sys$output "    SYS$LIBRARY:VAXCDEF"6 $     write sys$output "DECburger demo build aborted."
 $     exit $ elseK $     if f$search("sys$library:decc$rtldef.tlb") .nes. "" then goto stdiook > $     write sys$output "SYS$LIBRARY:STDIO.H is not available."4 $     write sys$output "Create it with the command:"W $     write sys$output " $ LIBRARY/TEXT/EXTRACT=STDIO/OUT=SYS$COMMON:[SYSLIB]STDIO.H -" 0 $     write sys$output "    SYS$LIBRARY:VAXCDEF"6 $     write sys$output "DECburger demo build aborted."
 $     exit $ endif 	 $stdiook:  $ !  $ ! Build DECburger and run it $ ! / $ write sys$output "Compiling DECburger.UIL..." , $ define/user/nolog uil$include decw$include' $ uil/motif decw$examples:decburger.uil  $ - $ write sys$output "Compiling DECburger.C..." 6 $ define/user/nolog c$include decw$include,sys$library* $ define/user/nolog vaxc$include c$include $ if arch_type .eqs. "VAX" $ thenH $     cc /define=(_NO_PROTO,XK_LATIN1,VMS,VMSV5,"vms",__STARLET_LOADED,-C __LIBS$ROUTINES_LOADED,X_LOCALE,XLIB_XPG4_FUNCS,_WCHAR_T_,X_WCHAR,- 6 I18N_BUG_FIX,I18N_MULTIBYTE) /INCLUDE=(DECW$EXAMPLES,- sys$share,decw$include)- /OPT decw$examples:decburger $ elseF $     cc /standard=vaxc /define=(_NO_PROTO,XK_LATIN1,VMS,VMSV5,"vms",-B __STARLET_LOADED,__LIBS$ROUTINES_LOADED,X_LOCALE,XLIB_XPG4_FUNCS,-H _WCHAR_T_,X_WCHAR,I18N_BUG_FIX,I18N_MULTIBYTE) /INCLUDE=(DECW$EXAMPLES,- sys$share,decw$include)- /OPT decw$examples:decburger $ endif ) $ write sys$output "Linking DECburger..."  $ if arch_type .eqs. "VAX" $ then# $     link decburger,sys$input/opt  ! sys$share:decw$dxmlibshr12/share   sys$share:decw$xlibshr/share    sys$share:decw$xtlibshrr5/share   sys$share:decw$xmlibshr12/share ! sys$share:decw$mrmlibshr12/share   sys$share:vaxcrtl/share  $  $ else" $     link decburger,sys$input/opt! sys$share:decw$dxmlibshr12/share   sys$share:decw$xlibshr/share    sys$share:decw$xtlibshrr5/share   sys$share:decw$xmlibshr12/share ! sys$share:decw$mrmlibshr12/share   $  $ endif > $ write sys$output "Copying DECburger bitmap and help library" $ copy decw$examples:icon.xbm * ? $ library/help/create decburger.hlb decw$examples:decburger.hlp  $ ) $ write sys$output "Running DECburger..."  $ run decburger  $  $ exit                                                                                                                                                                                                                                                                    �               * [F]DECBURGER.EXE;1 +  , �   . $    /     4     $                        - zp   0  � 1    2   3      K � P   W   O     5   6 �<���֟  7  ӄ��֟  8          9          G    H �� J �                *            � 0 D X     0205  ��������    (  3��E     &                                         	DECBURGER                              V1.0           @�3��ŗ 05-05               �           �           
       ��? �  �)              DECW$DXMLIBSHR12_001.      !       DECW$TRANSPORT_COMMON_001 �     !        
LIBRTL_001 O    !      � �
MTHRTL_001      !      	  
LBRSHR_001(              DECW$XMLIBSHR12_001)              DECW$MRMLIBSHR12_001�����������������������(              DECW$XTLIBSHRR5_001% �    !        dDECW$XLIBSHR_001       !        VAXCRTL_001  �����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������decburger.uid activate_proc create_proc list_proc pull_proc exit_proc scale_proc show_hide_proc show_label_proc toggle_proc sens_help_proc help_system_proc cancel_color_proc apply_color_proc ok_color_proc DECburger.title: DECburger
DECburger.x: 100
DECburger.y: 100 DECburger can't open hierarchy S_MAIN_WINDOW can't fetch main window decw$examples:decburger_help.decw$book Help System Error k_drink_init k_fries_init k_create_init k_dismiss_init    0 %s
 %s, %x
 %2.2d main_help can't fetch help widget main_help can't fetch help widget DXmfirstTopic DXmfirstTopic main_print can't fetch print widget DXmsuppressOptionsMask main_color can't fetch color mix widget DXmorigRedValue DXmorigGreenValue DXmorigBlueValue  0 %d  %d %d  %d  nyi can't fetch nyi widget overview about onhelp control_box can't fetch order box widget Exit - %s
 Help System Error decw$examples:decburger_help.decw$book topic Help System Error file_menu can't fetch file pulldown menu widget edit_menu can't fetch edit pulldown menu widget order_menu can't fetch order pulldown menu widget control_box can't fetch order box widget order.txt w Function Not Yet Implemented
 order.txt ISO8859-1 DXmPrintWgtPrintJob return status: %x
 can't allocate color cell DXmorigRedValue DXmorigGreenValue DXmorigBlueValue can't allocate color cell                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          �    �  (    2  D  <  �  F  @  Q  X  `     p  �  |    �    �  �#  �  #  �  <"     �  �                                                                                                                                                                                                                                                                                                                                                                                                  < �^��  ������T�����R� ��  � �%  |~� ���  |~��`ݬ߬|~��
߭��	��  �P����� ��������}  �P�������5  � ��4  � ���X�����P  ���ߢݢ��)�����;  �P��7������  � ���  ТQСP��P�PСRSСPРRS�S�|СP��P�PСRPРUСPРRPРU�U��|��U  S�cѠ'��|�cѠ��|�cѠ��|�cѠ���� ���  ݢ���  ݢ���  ��v��'��Oݢ���������  ݭ���e  ��P �^��U���T��j���R�Q��   \�l�\�2Q��P�\�l�\�P��Q��� \�l�\�2Q��P�¨\�l�\�2P�|�t�?��߭�ТPР\��\�\РR\ЬS�PР\ЬR\ЬS�S�Ĉ������  �Pݭ���H  �P ?��߭�ТPР\��\�\РR\ЬS�PР\ЬR\ЬS�S�ĕ�����O  �Pݭ����  �P¤?��߭�ТPР\��\�\РR\ЬS�PР\ЬR\ЬS�S�Ģ������  �Pݭ����  �P��?��߭�ТPР\��\�\РR\ЬS�PР\ЬR\ЬS�S�İ������  �Pݭ���L  �P���������Ŀ���  �P��������������  �P��   �^��u���Rݬ�����  � ��   �^��Q���Rݬ�� ������  � ���    �^Ь��Ь���߭��� ��)    �^Ь��Ь���߭��� ��   �^�����R��Q��A� ݬЬP�@�   ��M   �^�����S������R�¼���߭���S  ������߭����  �PT� Q��?  \�TP�\���P����������Q��v  �T���   �^��M���T��b���R�b�ݢ���  �Pb�bТQСP��P�PСZS
�PСPРZS�S����  ����  ��� 7������ ݢ���������  �P��������t  � ��s  ��� QСP��P�P��#P�P�P1� ����P�@�� ݢ��������g  �P�������  � ��  ����Ь���߭��P�@�� ��Q  �P�@�� ��$  �ТQСP��P�PСZS
�PСPРZS�S����  ��(��Ь���߭���� ���  ��� ���  ТQСP��P�PСZS
�PСPРZS�S��  ��5    �^�����\� ��   ���  < ���^��|���T�����R�b�ݢ���  �Pb�bТPР\��\�\РZS	Р\ЬZS�S����  ����  ��t1� �����tݢ��6������  �P��A������  � ���  �Ϭ��|��ԭ��UxU\�L��P���  `�L��\ޭ�l�UxU\�L��P��Z`�L��\Џ   l�U�U߭���t���  ��t���  ТPР\��\�\РZS
�PР\ЬZS�S��  ���  < ���^��L���T��a���R�b�ݢ���  �Pb�bТPР\��\�\РZS	Р\ЬZS�S����  ����  ��x1� �����xݢ��q������  �P��|�����q  � ��p  ��z  ��ޭ��߭�ݢ���  �����|��
  �P���  �UxU\�L��P�ę`�L��P<��`�UxU\�L��P�ĩ`�L��P<��`�UxU\�L��P�Ļ`�L��P<��`�U�U߭���x��8  ���������Э�;�P���  ������߭�ݢ��  ���|��X  �P���[  ��x���  ТPР\��\�\РZS
�PР\ЬZS�S��  ��%  | �^�����V�����S2� T�\�T�\�TQ�\R�QP�R@Ø� �R�A����  �T�У`T��  R�\�R���\���߭��T��B  U�e�ô��� T���  RУl\�R���\���߭��T�e���ݣx���  �È T���  R���\�R���\���߭��T�e��  ����^������X������R�´1� �´���߭���Q  ������߭����  �P���W��  V��  T��@ U�S�ca�eQ��  \ޭ�P�\���P���߭��Q��\  ޭ�� ݭ�ݾ$�f�P\ݾ �d�\� ݭ��dޭ�����ݾ �f�P\ݾ�d�\��U�S�W�ޭ���¨ݾ��{  �P\ݾ��v  �\�� ݭ�� ��N  ݭ���X  ԭ�ݢx��O  ߭�����P��K  խ�1� ݭ����߭���>  ������߭����  �P��ޭ���¤ݾ���  T�d�P\ݾ���  S�c�\�ޭ�����ݾ�d�P\ݾ�c�\�ޭ���¬ݾ�d�P\ݾ�c�\�� ݭ�� ���  �¼1� �¼���߭����  ������߭���%  �P��ޭ�n� ݾ��P  T�d�P\ݾ ��H  S�c�\� ޭ�����ݾ�d�P\ݾ�c�\�ޭ���°ݾ�d�P\ݾ�c�\�� ݭ�� ���  �P  �,^��m���\��  ��ԭ؞�  ��ԭ���  ��ԭ��	  ��ԭ��������   ��%  < ��|�^�����U�����SмP�P,c +;CK� � ����������[[[[[���������������Ow���������?�Pգ 6����� ݣ��������^  �P��������  � ��  ݣ ��C  �P��  ��ԭ؞�  ��ԭ���  ��ԭ��  ��ԭ��߭��Ì ��   �P���  ��ԭ����  ��ԭ����  ��ԭ����  ��ԭ��߭��Ì ���  �Pݣ<���  � ���� ����P� ���Pݣ<���  � ���ä��%  ЬT��C  R�äP�R���P���߭��T��  �P�ü�ü���߭���  ������߭����  �PT�È Q���  R�TP�R���P���߭��Q��(  �T���  �P�ü�ü�ü���߭����  ������߭���*  �PT�È Q���  R�TP�R�|��P�����|��Q���  �T��?  �P������������
  �PR�R��/��R��  �����������
  �PR�R����R���
  �����������
  �PR�R�����R���
  � ݣ��|
  �P� Ϗ��P� Ϸ��P �^�����R��QЬP�� A�   �^������R� ��m
  ЬP�� ��Z
  �P   �^������QЬP�� �� �^�����RТ<QСP��P�P��#P�P�P�P
�Q��q
  �b�ݢ���	  �Pb�bТQСP��P�PСZS	СPРZS�S����
  ����
  ݢ<��
  ���$ݢ<��g	  ТQСP��P�PСZS	СPРZS�S��  ��q
   �^������S������Rբ<6�����<ݢ��������	  �P�������K
  � ��J
  Т<QСP��P�P��#P�P�P*ТT��,
  P���Q�P���Q���߭��T��d	  �PТT��
  P���Q�P���Q��������T��9	   �^��5���SмQЬA��Q1� 1� �Q!1� �QP�P! : b �P����?�PЬR���	  P�èQ�P���Q���߭��R���  ЬR��n	  P�ìQ�P���Q���߭��R���  ЬR��F	  P�ðQ�P���Q���߭��R���  �ݬ���  �P��O  �ݬ���  �P��:   �^��1���Rլݬ��8���  ��C�Ϲ��������p  ����  �P  �^ݬ�ϗ��P �^������R���|��� ��|��U  ��D�����!   �,^�����T������RмP�P�  y �P���?�P�  1L����  � �Ĕ�����X  �P�Ğ�����  � ��  ТS��)  P�  Q�P���Q���߭��S��=  �¤ 1�����¤ � ���������  �P���������  � ���  ТS���  P�¤ Q�P���Q���߭��S���  �b�ݢ��  �Pb�bТQСP��P�PСZS	СPРZS�S���  ����  �¨ ^����¨ � ��������?  �P���������  � ���  ТS��  P�¨ Q�P���Q���߭��S��$  բ<6�����<ݢ��&������  �P��2������  � ���  Т<QСP��P�P	��#P�P�P�P*ТS��l  P���Q�P���Q���߭��S���  (�PТS��B  P���Q�P���Q���߭��S��z  ТQСP��P�PСZSСPРZS�S����  �P< �^��-���S��B���T��Y��O���  �PRI��[�R���  �R���  �Ã��y��x  �P���߭���t��   �PU�U�Í��x  �P ���^�����T������SЬQ������
�����������|��  �P�À��  �P.��@  ��Э����߭��Ì ��u  �߭�ݣ��f  �Ĵ������  � ���  ��x��)  �����<���������<���������<������ͬ���x  ��   ���^������T�����SЬQ������
�����������|��?  �P�À��6  �P.��t  ��Э����߭��Ì ���  �߭�ݣ���  �����  ��#  � ��"   ���^��x���R��x��L  ��  ������߭�� ��F  ��ͬ��� ��5   �^ݬЬS�c���  �PRݣ ���  �R�   �^��	���R�b�ݢ��0  �Pb�bТPР\��\�\РZS	Р\ЬZS�S���/  ��  ��&   �^�����RТPР\��\�\РZS
�PР\ЬZS�S����                                                                                                                                                                                                                                                                                                                                                                            @   @   �  �  �                                           �     �  �  �  �  (  0  8  �"  �%  $        �
  @  �  �  �  �  �  H  �  �O  �9  C  HL  �=  �8  �@  �O  �O  (N   P  l�        �    (   8            P  h  0  �  `  H  (  �
     �  �  ,  �  �  �  �  �  �  �     �  �  �     �  �  �  (  �  �     	   �  h  �  �  �     �     
   
   l   <   L  |     D  �      ��  ��    C!  ��  d�  n�  ��  ��  5�      $                �                                                                                                                                                                                                                                           ^/1 ~      DWMV_JA_JPU6012.F              �  zp [F]DECBURGER.EXE;1                                                                                                                  $                         �I                 @                                                                       DECW$DXMLIBSHR12                                               DECW$TRANSPORT_COMMON                                          LIBRTL                                                         MTHRTL                                                         LBRSHR                                                         DECW$XMLIBSHR12                                                DECW$MRMLIBSHR12                                               DECW$XTLIBSHRR5                                                DECW$XLIBSHR                                                   VAXCRTL                                      �%  �%  �%  �%  �%  �%  �%  �%        �%  �%                                                                                                                                                                                                                                                                                      �               * [F]DECBURGER.UID_US_RENAME;1 +  , ^�   . H    /     4    H   I                    - zp   0  � 1    2   3      K � P   W   O I    5   6 �i<��֟  7 �Y���֟  8          9          G    H �� J �      v            ro|  URM 1.2   Motif Uil Compiler            V2.0-000  13-MAR-1994 19:00:33.52       decburger                     V1.1.1     N p  \ 	 	   I t               � �	 �	 t	 � x @ P	 <	 	 � � � � ` t � T 4    �  ��H      �   �X         control_box                     main_color                      main_print                      main_help                       nyi                             S_MAIN_WINDOW                       ��H      "   ߀ 	ISO8859-1Apple Juice��H      * � ߀& 	ISO8859-1Create Order Box...��H      & ߀" 	ISO8859-1Close Order Box��H       T߀ 	ISO8859-1Medium ��H      h ���Ih           0             S_MAIN_WINDOW ma _c         \        P   total_order s_menu_bar    ��H      ����I�        \ �   ,         s_menu_bar Oh�f _c  P  $     \           P   create_proc        2       +    �   Q   	 �     h�f     �  �                     sens_help_proc N           �    help_menu_entry                   t       d       P       <       ,  help_menu_entry options_menu_entry  order_menu_entry    edit_menu_entry file_menu_entry   ��H      H8��IH        `     0         file_menu_entry h�f P   T                   creacreate_proc        @    0  !       /      P   	 �   Q   	 �     h�f     �  �   elp_p          sens_help_proc         trh�f                         pull_proc                   lp �    file_menu menu  ��H      � ���I�         \ �   ,         file_menu enh�f f  P  %                    create_proc        Q   	 p   0h�f    �  �     P         Q   sens_help_proc         p             �   p    �   m_exit_button fm_print_button   ��H      � ,��I�         0               m_print_button        !    �   /    �   M   	 �   Q   	 `   h�f     �  �   f            sens_help_proc           h�f     �  .                 �   activate_proc ex       pr           ��H      $ 4߀  	ISO8859-1Print Order..  ��H       @P ��H       x߀ 	ISO8859-1print  ��H      � ���I�         0               m_exit_button         !    �       �       �   /    �   M   	 l   � h�f f  �  �               lp_prexit_proc                           	 ti      
          ��H       �߀ 	ISO8859-1Exit   ��H       �Ctrl<Key>z:   ��H       �߀ 	ISO8859-1Ctrl/Z ��H       	E ��H       <	normal demo exit  ��H       P	߀ 	ISO8859-1File   ��H       t	F ��H        �	߀ 	ISO8859-1	file_menu  ��H      H�	��IH        `     0         edit_menu_entry h�f     T                  � create_proc        @    0  !       /      P   	 �   Q   	 �     h�f     �  �              sens_help_proc        h�f                       pull_proc                   lp �    edit_menu menu  ��H      X�	��IX        \ �   ,         edit_menu enh�f f  P  &                   create_proc        Q   	 p   0h�f    �  �     P         Q   sens_help_proc                      H  p    8       (  f                   m_select_all_button m_clear_button  m_paste_button  m_copy_button num_cut_button _me  ��H      ,H��I,        0               m_cut_button �f       !      /          �       �   M   	 �   Q   	 t   h�f �   �  �   P             senssens_help_proc          h�f   �     p               activate_proc                 ct       cl        pa  ��H       �߀ 	ISO8859-1Cut��H       �t ��H        <Key>DRemove: ��H       4߀ 	ISO8859-1Remove ��H      & T߀" 	ISO8859-1not_implemented��H       �߀ 	ISO8859-1Copy   ��H       �C   ro|    N l�	l	 �x	 � �� �p $�  � T 
� �d  	0 (H (� ��	 �	 �� t�   � �
@ d0 l  xL   0 H< �	l ,x �� �� � l� X ph � �$ � �	  � � �� �� � T� � � �( �� �
� � �     �L �4 �   �\ 4� � �	t d� h� ��    �� �  |� ��	  � P   ` l� 8� �    ��	   � 8�  
� 0	� �� P D                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        >ClassTable >ResourceTable  cancel_button   ok_button   button_separator2   reset_button    apply_button    button_separator    down_value  up_value    value_label quantity_label  drink_quantity  drink_list_box  drink_label drinks_form fries_quantity  fries_quantity_label    medium_fries    huge_fries  large_fries small_fries tiny_fries  fries_menu  fries_option_menu   fries_label fries_form  burger_quantity mayo    onion   pickle  mustard ketchup burger_toppings_menu    burger_well burger_medium   burger_rare burger_doneness_box burger_label    burger_form control_box main_color  main_print  main_help   nyi total_order help_onhelp help_version    help_window help_sensitive  help_menu   help_menu_entry m_options_control_button    options_menu    options_menu_entry  m_submit_order_button   m_cancel_order_button   m_show_control_button   order_menu  order_menu_entry    m_select_all_button m_clear_button  m_paste_button  m_copy_button   m_cut_button    edit_menu   edit_menu_entry m_exit_button   m_print_button  file_menu   file_menu_entry s_menu_bar  S_MAIN_WINDOW   k_fries_init    k_dismiss_init  k_create_init   k_drink_init    decburger   ro|   � h @ � � D $  � � � L  < ( � � �
 �
 P  � x � � � � D P <  T @  � � � `	 L	 	 �
 �
 �
 � \ <  � � $ �  8 �	 �	 � � �	 �
 � L D �  � � l � p �	 �
 p @ t < � < 4 �  $ H	 �	 �
 p
 � < 	 �	 �	 �	 \	 �	 h                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ro|  " l�P ��H      ,  ��I,        0               m_copy_button f       !      /          �       �   M   	 �   Q   	 t   h�f �   �  �   P             senssens_help_proc          h�f   �     p               activate_proc                 ct       cl       pa  ��H         Shift<Key>DRemove:    ��H      # @߀ 	ISO8859-1Shift+Remove   ��H      ,h��I,        0               m_paste_button        !      /          �       �   M   	 �   Q   	 t   h�f �   �  �   P             senssens_help_proc          h�f   �     p               activate_proc                 ct       cl       pa  ��H       �߀ 	ISO8859-1Paste  ��H       �P ��H       <Key>Insert:  ��H       $߀ 	ISO8859-1Insert ��H      0 D��I0                         widget-0-520-34   ��H      � t��I�         0               m_clear_button         !    �   /    �   M   	 �   Q   	 `   h�f �   �  �     h�f          � sens_help_proc         prh�f   �       h�f           activate_proc        	 ro      
    ��H       �߀ 	ISO8859-1Clear  ��H       �r ��H      � ���I�         4               m_select_all_button        !    �   /    �   M   	 �   Q   	 d   h�f �   �  �   f             senssens_help_proc        fh�f �   �     f             actiactivate_proc                  ��H      ! ߀ 	ISO8859-1
Select All ��H       A ��H       L߀ 	ISO8859-1Edit   ��H       `E ��H        �߀ 	ISO8859-1	edit_menu  ��H      L���IL        d     4         order_menu_entry on h�f     X     /            	 � create_proc        @    4  !    $  /      P   	 �   Q   	 �     h�f f  �  �   �               sens_help_proc e         h�f                      pull_proc                   tt �    order_menu _cu  ��H      4���I4        \ �   ,         order_menu eh�f n   P  '   X               create_proc        Q   	 p     h�f !   �  �               �   sens_help_proc        �              e                 �   m_submit_order_button   m_cancel_order_button   m_show_control_button tt  ��H      8��I        h     8         m_show_control_button P h�f X   \                  proc create_proc        /      M   	 �   Q   	 �     h�f ns  �  �               � sens_help_proc          h�f     �       �          t_ordshow_hide_proc a       to  ��H       �	C ��H      0 �
��I0                         widget-0-619-34   ��H       �
��I         8               m_cancel_order_button P        !    �   /    �   M   	 �   Q   	 h   h�f     �  �   M   	         	 � sens_help_proc           h�f �   �  
   proc             activate_proc �                 id  ��H      # ߀ 	ISO8859-1Cancel Order   ��H       n ��H       P��I         8               m_submit_order_button P        !    �   /    �   M   	 �   Q   	 h   h�f     �  �   M   	         	 � sens_help_proc           h�f �   �     proc             activate_proc �                 id  ��H      # d߀ 	ISO8859-1Submit Order   ��H       xS ��H      ! �߀ 	ISO8859-1
order_menu ��H       �߀ 	ISO8859-1Order  ��H       �r ��H       (߀ 	ISO8859-1order  ��H       <��I         d     4         options_menu_entry oh�f     X  /   �   /         M   create_proc        @      !    �   /    �   (        Q   	 �   h�f    �  �   �   �        roc  sens_help_proc i                     pr �    options_menu  m_sh                                                                                  ro|  & <t| ��H      �   ��I�         ` �   0         options_menu enth�f f  T  0   /   �             create_proc        Q   	 t   h�f    �  �   �   (         Q   sens_help_proc         �             �   m_options_control_button    ��H      4  ��I4        l     <         m_options_control_button  T h�f /   `  1                proc create_proc        !    $  /      M   	 �   Q   	 �   h�f pr  �  �                    sens_help_proc p       buh�f    1              practivate_proc on       _c       tt  ��H      * � ߀& 	ISO8859-1Background Color...��H       DC ��H       �߀ 	ISO8859-1Options��H       �O ��H       �߀ 	ISO8859-1options��H      � ���I�         0               help_menu_entry        @    �   !    �   /    �   Q   	 `   h�f _p  �  �     !              sens_help_proc         pr               ns �    help_menu      ��H      � ��I�         , �             help_menu en       Q   	 @   � h�f    d  t   �   Q         h�fsens_help_proc           ns    p    �        �       �       �   help_onhelp help_version nu help_window help_sensitive   ��H      � ���I�         0               help_sensitive         !    �   /    �   M   	 �   Q   	 `   h�f ns  �  �                 sens_help_proc           h�f     �  -     �          help activate_proc u         lp      !    ��H      !  ߀ 	ISO8859-1
On Context ��H       C ��H        @߀ 	ISO8859-1	sensitive  ��H      � T��I�         ,               help_window        !    �   /    �   M   	 �   Q   	 \   h�f f  �  �   �                 sens_help_proc r      "  h�f f  �  *   -                hactivate_proc ro      #        $    ��H        �߀ 	ISO8859-1	On Window  ��H       �W ��H       �߀ 	ISO8859-1overview   ��H      � ���I�         0               help_version           !    �   /    �   M   	 �   Q   	 `   h�f �   �  �                 senssens_help_proc       % fh�f �   �  +                hactiactivate_proc       &       '    ��H      ! ߀ 	ISO8859-1
On Version ��H       	V ��H       L	߀ 	ISO8859-1about  ��H      � `	��I�         ,               help_onhelp        !    �   /    �   M   	 �   Q   	 \   h�f f  �  �   �                 sens_help_proc r      (  h�f f  �  ,   +                hactivate_proc ro      )        *    ��H       �	߀ 	ISO8859-1On Help��H       �
H ��H       �
߀ 	ISO8859-1onhelp ��H       �
߀ 	ISO8859-1Help   ��H       H ��H       <߀ 	ISO8859-1help   ��H       P߀ 	ISO8859-1menu_bar   ��H      � ���I�         \     ,         total_order h�f     P      /            	 � create_proc        I       F        '       Q   	 �   h�f   �  �    h�f          , sens_help_proc         ro  ��H      � ���I�         T     $         nyi h�f r   H     P                create_proc            �   -    �                 1       f      +        , ,   ��H      - �߀) 	ISO8859-1DECburger: Information ��H      ; \߀7 	ISO8859-1$This feature is not yet implemented.   ��H      � ���I�         ,               main_help t            �       |       l       \         -        .        /        0    ��H      # �߀ 	ISO8859-1Help Example   ��H       �߀ 	ISO8859-1glossary   ��H       �߀ 	ISO8859-1overview   ��H      $ ߀  	ISO8859-1decburger.hlb                                                                                                                              ro|   Dt| ��H      �   ��I�         ,               main_print             �       �          
       	       Q   	 p   h�f    �  �                 sens_help_proc        ns      1       2 ro  ��H      '   ߀# 	ISO8859-1DECburger: Print   ��H      / � ߀+ 	ISO8859-1DECburger: Print Options   ��H      x$��Ix        ,               main_color s           h      X  Q   	   O   	 �   R   	 �   N   	 p   h�f    �                    apply_color_proc  h�f    �                    ok_color_proc   h�f           *              cancel_color_proc onh�f     8 H           t sens_help_proc u       me      3 it      4 le  ��H      2 h߀. 	ISO8859-1DECburger: Background Color��H      ' �߀# 	ISO8859-1Colormix Example   ��H      x8��Ix        \ |  ,         control_box h�f     P                 	 create_proc            l          1               J    w  K    d                       T      8      
   H    
   0       Q   	 �   h�f      (  ncel_        oc onhelp_system_proc        5  �    cancel_button     �    ok_button        6 en	 en         l       `       T       @       4       $                     �  button_separator2   cancel_button   reset_button    apply_button    ok_button   button_separator    drinks_form fries_form  burger_form   ��H       t��I         , |             burger_form        =       <       B       C    
   #               0                   �   w    �        �       �   burger_quantity burger_toppings_menu    burger_doneness_box burger_label  (  ��H  
    �  ��I�    
     `     0         burger_label    h�f     T  !                 C create_proc        !    �   B       C    
   #       %       3       5    P                7 rg  ��H      ! ߀ 	ISO8859-1
Hamburgers ��H      � ���I�         4 �             burger_doneness_box        2       B       E    x   #       $    
   0        �    burger_label  %             �   P    �        �   burger_well burger_medium   burger_rare   ��H      � $��I�         \     ,         burger_rare h�f x   P                    create_proc        !    �   T   	 x   h�f     �     abel              toggle_proc       8    ��H        	߀ 	ISO8859-1Rare   ��H      � �	��I�         `     0         burger_medium fh�f P   T                  creacreate_proc        !    �   ;       T   	 �    h�f     �       tog          toggle_proc       9 _w  ��H       
߀ 	ISO8859-1Medium ��H      � �
��I�         \     ,         burger_well h�f f  P                     create_proc        !    �   T   	 x   h�f     �     f               toggle_proc       : c   ��H        (߀ 	ISO8859-1	Well Done  ��H      H���IH        8 �             burger_toppings_menu  P 	       2       B       E    �   #       &    �   $    
   3       4    
   0         ! �    burger_doneness_box    �    burger_label um    rg    e    @  r    8  (    0       (  l       mayo  onion   pickle mustard ketchup   ��H      � (��I�         X     (         ketchup h�f s_  L                     B create_proc        !    �   T   	 t   h�f     �     4              toggle_proc       ; es  ��H       �߀ 	ISO8859-1Ketchup��H      � L��I�         X     (         mustard h�f s_  L                     B create_proc        !    �   T   	 t   h�f     �     4              toggle_proc       < es  ��H       |߀ 	ISO8859-1Mustard                                                                                                                            ro|   ��$ ��H      �   ��I�         X     (         pickle  h�f s_  L                     B create_proc        !    �   T   	 t   h�f     �     4              toggle_proc       = es  ��H         ߀ 	ISO8859-1Pickle ��H      � � ��I�         X     (         onion   h�f s_  L                     B create_proc        !    �   T   	 t   h�f     �     4              toggle_proc       > es  ��H       � ߀ 	ISO8859-1Onion  ��H      � ���I�         X     (         mayo    h�f s_  L                     B create_proc        !    �   T   	 t   h�f     �     4              toggle_proc       ? es  ��H      ! �߀ 	ISO8859-1
Mayonnaise ��H      l���Il        `     0         burger_quantity h�f    T                  ate_pcreate_proc        2       .        *    
   B       E    H  C    
   #       %       3       5    P              
   A    8  ?       0       T   	   h�f     ,                onioscale_proc       @ p " �    burger_toppings_menu     ��H       �߀ 	ISO8859-1Quantity   ��H      `l��I`        , �             fries_form t       =       <       B       C    
   #       &    �              �       
           0          �    button_separator    5  �    burger_form  
          ?    T       @  	    (         fries_quantity fries_quantity_label   fries_option_menu bufries_label   ��H  
    � ���I�    
     \     ,         fries_label h�f     P  "   <              create_proc        !    �   B       C    
   #       %       3       5    P                A 5   ��H       ߀ 	ISO8859-1Fries  ��H      <���I<        d     4         fries_option_menu   h�f "   X  2                ate_pcreate_proc        @    $  !      ,    �   B       E    �   C    
   #       $    
   3       4    
   0         �    fries_label     �    medium_fries           B ti �    fries_menu bel  ��H      � ��I�           ,             fries_menu n en    f    �   2    �        �   e    |        p   huge_fries  large_fries medium_fries    small_fries tiny_fries    ��H      � l��I�         ,               tiny_fries n       !    |   M   	 H   h�f    l     e               activate_proc         C di  ��H       0	߀ 	ISO8859-1Tiny   ��H      � �	��I�         ,               small_fries        !    |   M   	 H   h�f    l     e               activate_proc         D di  ��H        
߀ 	ISO8859-1Small  ��H      � �
��I�         ,               large_fries        !    |   M   	 H   h�f    l     e               activate_proc         E di  ��H       �
߀ 	ISO8859-1Large  ��H      � p��I�         ,               huge_fries         !    |   M   	 H   h�f    l     e               activate_proc         F di  ��H       �߀ 	ISO8859-1Huge   ��H       @߀ 	ISO8859-1Size   ��H      � p��I�         `     0         medium_fries    h�f     T     	 H            l create_proc        !    �   M   	 |   h�f     �     _frie        ll_fractivate_proc         G     ��H       �߀ 	ISO8859-1Medium ��H  
    � t��I�    
     8               fries_quantity_label           !    t              X   #       $       M       H     ��H       �߀ 	ISO8859-1Quantity   ��H      0<��I0        `     0         fries_quantity lh�f     T        t            create_proc               6       )       G       B       E    �   C    ����#       &    �   $    
   0        " �    fries_quantity_label  " �    fries_quantity_label           I fr  ��H       p 0                                        ro|   <�T��H      P  ��IP        , �             drinks_form        =       <       B       C    
   #       &    �   $    
   3       4    
              �       
           0        �    button_separator �   �    fries_form l           D  a    4       $  drink_quantity _drink_list_box edrink_label   ��H  
    �   ��I�    
     \     ,         drink_label h�f     P  #   <              create_proc        !    �   B       C    
   #       %       3       5    P                J   ��H       d߀ 	ISO8859-1Drinks ��H      <��I        0               drink_list_box       I            8 7  �   9        B       E    �   #       %       3       5    P   0       S   	 �   h�f   �     �         n_seplist_proc    �    drink_label         K         L    ��H      �l@�  6  X  {  �  �  �  �   1 L i    ߀ 	ISO8859-1Apple Juice߀ 	ISO8859-1Orange Juice߀ 	ISO8859-1Grape Juice߀ 	ISO8859-1Cola߀ 	ISO8859-1Punch߀ 	ISO8859-1	Root beer߀ 	ISO8859-1Water߀ 	ISO8859-1
Ginger Ale߀ 	ISO8859-1Milk߀ 	ISO8859-1Coffee߀ 	ISO8859-1Tea   ��H      0 �@�       ߀ 	ISO8859-1Apple Juice  ��H      � 4��I�         0 �             drink_quantity        B       D    F   #       $    
   3    �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          g�'� ~      DWMV_JA_JPU6012.F              ^�  zp [F]DECBURGER.UID_US_RENAME;1                                                                                                       H                               <          4    
           0        9-    p    �   �    �   8    �       �   down_value up_value �value_label quantity_label   ��H  
    | x��I|    
     0               quantity_label        !    l   #       $       B       D    (   4       M     ��H       �߀ 	ISO8859-1Quantity   ��H  	    � ��I�    	     \     ,         value_label h�f     P     l   #         $   create_proc        !    �   #       &    �   $    
   B       D    (    �    quantity_label          N    ��H       D߀ 	ISO8859-1 0 ��H      � $��I�         ,               up_value el        B       C       #       $    
   &    �   "            �   M   	 �   h�f     �     D           �  activate_proc el      O   �    value_label label   ��H      � P8v�	              )         P       ��ꫪ*�����*�����*�����*���*�����*�����*�����*諪��+�����/�����?�����?�����*�����*�����*�����*�����*�����*       ��H      ` H	�Ώ                                       @            P           Q         R     ��H       �	yellow      ��H       p
red         ��H      � �
��I�         ,               down_value         B       E    �   #       $    
   &    �   "            �   M   	 �   h�f ��  �     ����*        �  activate_proc el      S   �    value_label label  �    up_value    ��H      	 �
icon.xbm  ��H      � ���I�         4               button_separator           B       E    �   C    
   2       #       3       :           �    burger_form *       ��H      h���Ih        `     0         apply_button torh�f     T        E         C   create_proc 
       !    X      H      8         B       E      C    
   #       &       M   	 �   h�f va  �     label           upactivate_proc SO �    ok_button   �    button_separator        Q il      T SO      U   ��H       �߀ 	ISO8859-1Apply  ��H      N smk        ISO8859-1 -Adobe-Times-Bold-R-Normal--*-180-*-*-P-*-ISO8859-1                                                                                                                                                                                                                                                                                                                                                     ro| 	  �(� ��H      l  ��Il        `     0         reset_button -Noh�f *-  T     *-ISO         C   create_proc 
       !    \      L      <         B       E      C    
   #       &       M   	 �   h�f va  �     label           upactivate_proc SO �    apply_button   �    button_separator         Q       V       W ���H         ߀ 	ISO8859-1Reset,��H      N �smk        ISO8859-1 -Adobe-Times-Bold-R-Normal--*-180-*-*-P-*-ISO8859-1 ��H      � ���I�         4               button_separator2 al       B       E    �   C    
   2       #       3       :        < �    ok_button   E  ��H      1 ߀- 	ISO8859-1DECburger: Order Entry Box1��H      D���ID        \     ,         ok_button  Bh�f al  P                 �   create_proc 	       !    4      $        >              B       E    �   C    
   M   	 �    h�f 	   �     va  �        abel activate_proc ti �    button_separator _butt      Q         X at      Y   oc��H       ߀ 	ISO8859-1OK ��H      N \smk        ISO8859-1 -Adobe-Times-Bold-R-Normal--*-180-*-*-P-*-ISO8859-1 ��H      l���Il        `     0         cancel_button Noh�f *-  T  	   *-ISO         creacreate_proc 
       !    \      L      <         B       E      C    
   #       &       M   	 �   h�f va  �  	    abel        e_proactivate_proc bu �    reset_button    �    button_separator         Q       Z       [ � ��H       �߀ 	ISO8859-1Cancel ��H      N hsmk        ISO8859-1 -Adobe-Times-Bold-R-Normal--*-180-*-*-P-*-ISO8859-1 ��H       �order ��H      ���Z�U         Z  m  ~  �  �  �  �  �  �  �        (  5  F  S  b  o  |  �  �  �  �  �  �  �  �          )  3  B  M  Z  e  t  �  �  �  �  �  �  �  �  �  �  �         %  7  E  U  c  g  w  �  �  �  �  �  �  �  �  �  �  �  �        !  2  @  O  a  n  y  �  DXmapplicationName DXmglossaryTopic DXmlibrarySpec DXmmainLabel DXmnumberCopies DXmoptionsDialogTitle DXmoverviewTopic DXmunmanageOnCancel DXmunmanageOnOk accelerator acceleratorText alignment allowOverlap autoUnmanage bottomAttachment bottomOffset bottomPosition bottomWidget cancelButton columns defaultButton defaultButtonShadowThickness defaultPosition dialogStyle dialogTitle fontList highlightColor horizontalSpacing itemCount items labelPixmap labelString labelType leftAttachment leftOffset leftPosition leftWidget listSizePolicy mappedWhenManaged maxLength maximum menuHelpWidget menuHistory messageString minimum mnemonic navigationType noResize orientation rightAttachment rightOffset rightPosition rows selectedItemCount selectedItems selectionPolicy separatorType set shadowThickness shadowType showAsDefault showValue subMenuId titleString topAttachment topOffset topPosition topWidget unitType value verticalSpacing visibleItemCount x y createCallback activateCallback applyCallback cancelCallback cascadingCallback helpCallback okCallback singleSelectionCallback valueChangedCallback     ��H      I�Z�         n   �   �   �   �   �   �   �     !  4  D  W  l    �  �  �  �  �  �      *  DXmCreateColorMixDialog DXmCreateHelpDialog DXmCreatePrintDialog XmCreateCascadeButton XmCreateForm XmCreateFormDialog XmCreateInformationDialog XmCreateLabel XmCreateLabelGadget XmCreateMainWindow XmCreateMenuBar XmCreateOptionMenu XmCreatePulldownMenu XmCreatePushButton XmCreatePushButtonGadget XmCreateRadioBox XmCreateRowColumn XmCreateScale XmCreateScrolledList XmCreateSeparator XmCreateSeparatorGadget XmCreateText XmCreateToggleButton XmCreateToggleButtonGadget      �  �   �   �        roc  sens_help_proc i                     pr �    options_menu  m_sh                                                                                  �               * [F]DECBURGER.UIL_US_RENAME;1 +  , ]�   . ~    /     4 �   ~   m                     - zp   0  � 1    2   3      K � P   W   O n    5   6 ����֟  7 ����֟  8          9          G    H �� J �      `            P !******************************************************************************* !*A !*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC. P !*  (c) Copyright 1989, 1990, 1991 DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. !*  ALL RIGHTS RESERVED  !*  B !*      THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USEDD !*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE ANDG !*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR B !*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADED !*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE# !*  SOFTWARE IS HEREBY TRANSFERRED.  !*  E !*      THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT G !*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE 3 !*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS    !*  E !*      OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS, G !*  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS I !*  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY G !*  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING D !*  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND% !*  FITNESS FOR A PARTICULAR PURPOSE.  !*  L !*  Notice:  Notwithstanding any other lease or license that may pertain to,K !*  or accompany the delivery of, this computer software, the rights of the H !*  Government regarding its use, reproduction and disclosure are as setG !*  forth in Section 52.227-19 of the FARS Computer Software-Restricted  !*  Rights clause. !*  T !*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.  Unpublished - allB !*  rights reserved under the Copyright laws of the United States. !*  E !*  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the J !*  Government is subject to the restrictions as set forth in subparagraphK !*  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause  !*  at DFARS 52.227-7013.  !*  " !*  Open Software Foundation, Inc. !*  11 Cambridge Center  !*  Cambridge, MA   02142  !*  (617)621-8700  !*  G !*  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with J !*  "restricted rights."  Use, duplication or disclosure is subject to theG !*  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985) I !*  "Commercial Computer Software- Restricted Rights (April 1985)."  Open M !*  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If M !*  the contract contains the Clause at 18-52.227-74 "Rights in Data General" , !*  then the "Alternate III" clause applies. !*  A !*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.  !*  ALL RIGHTS RESERVED  !*   !*  P !* Open Software Foundation is a trademark of The Open Software Foundation, Inc.7 !* OSF is a trademark of Open Software Foundation, Inc. = !* OSF/Motif is a trademark of Open Software Foundation, Inc. 9 !* Motif is a trademark of Open Software Foundation, Inc. A !* DEC is a registered trademark of Digital Equipment Corporation E !* DIGITAL is a registered trademark of Digital Equipment Corporation N !* X Window System is a trademark of the Massachusetts Institute of Technology !*P !******************************************************************************* !module DECburger_demo   module  decburger          version = 'V1.1.1'         names = case_sensitive                            objects = { &                 XmSeparator = gadget ;"                 XmLabel = gadget ;'                 XmPushButton = gadget ; )                 XmToggleButton = gadget ;                  }   + procedure                                   "         toggle_proc     (integer);"         activate_proc   (integer);"         create_proc     (integer);"         scale_proc      (integer);"         list_proc       (integer);!         exit_proc       (string); "         show_hide_proc  (integer);"         pull_proc       (integer);-         sens_help_proc  (compound_string);    %         help_system_proc (string);             ok_color_proc   ();          apply_color_proc ();         cancel_color_proc ();    value      k_create_order      : 1;     k_order_pdme        : 2;     k_file_pdme         : 3;      k_edit_pdme         : 4;         k_nyi               : 5;D     k_ok                : 6;        ! NOTE: ok, apply, reset, cancel8     k_apply             : 7;        ! must be sequential     k_reset             : 8;     k_cancel            : 9;     k_cancel_order      : 10;      k_submit_order      : 11;      k_order_box         : 12;      k_burger_min        : 13;      k_burger_rare       : 13;      k_burger_medium     : 14;      k_burger_well       : 15;      k_burger_ketchup    : 16;      k_burger_mustard    : 17;      k_burger_onion      : 18;      k_burger_mayo       : 19;      k_burger_pickle     : 20;      k_burger_max        : 20;      k_burger_quantity   : 21;      k_fries_tiny        : 22;      k_fries_small       : 23;      k_fries_medium      : 24;      k_fries_large       : 25;      k_fries_huge        : 26; ,     k_fries_quantity    : 27;                    k_drink_list        : 28;      k_drink_add         : 29;      k_drink_sub         : 30;      k_drink_quantity    : 31;      k_total_order       : 32;      k_burger_label      : 33;      k_fries_label       : 34;      k_drink_label       : 35;      k_menu_bar          : 36;      k_file_menu         : 37;      k_edit_menu         : 38;      k_order_menu        : 39;       k_help_pdme         : 40;   #     k_help_menu         : 41;       ,     k_help_overview     : 42;                    k_help_about        : 43;       k_help_onhelp       : 44;        k_help_sensitive    : 45;        k_print             : 46;       k_options_pdme      : 47;   #     k_options_menu      : 48;             k_create_options    : 49;        k_fries_optionmenu  : 50;    value G         k_decburger_title               : 'DECburger: Order Entry Box'; Q         k_nyi_label_text                : 'This feature is not yet implemented.'; 1         k_file_label_text               : 'File'; A             k_print_label_text          :   'Print Order..';      3             k_exit_label_text           :   'Exit'; 1         k_edit_label_text               : 'Edit'; 2             k_cut_dot_label_text        :   'Cut';3             k_copy_dot_label_text       :   'Copy'; 4             k_paste_dot_label_text      :   'Paste';4             k_clear_dot_label_text      :   'Clear';9             k_select_all_label_text     :   'Select All'; 2         k_order_label_text              : 'Order';;             k_cancel_order_label_text   :   'Cancel Order'; ;             k_submit_order_label_text   :   'Submit Order'; 4         k_options_label_text            : 'Options';B             k_options_color_label_text  :   'Background Color...';1         k_help_label_text               : 'Help'; =             k_sensitive_label_text      :   'On Context';     ;             k_overview_label_text       :   'On Window';    9             k_about_label_text          :   'On Version'; 9             k_onhelp_label_text         :   'On Help';    7         k_hamburgers_label_text         : 'Hamburgers'; 3             k_rare_label_text           :   'Rare'; 5             k_medium_label_text         :   'Medium'; 8             k_well_done_label_text      :   'Well Done';6             k_ketchup_label_text        :   'Ketchup';6             k_mustard_label_text        :   'Mustard';4             k_onion_label_text          :   'Onion';9             k_mayonnaise_label_text     :   'Mayonnaise'; 5             k_pickle_label_text         :   'Pickle'; 5             k_quantity_label_text       : 'Quantity'; 2         k_fries_label_text              : 'Fries';1             k_size_label_text           : 'Size'; 3             k_tiny_label_text           :   'Tiny'; 4             k_small_label_text          :   'Small';4             k_large_label_text          :   'Large';3             k_huge_label_text           :   'Huge'; 3         k_drinks_label_text             : 'Drinks'; /             k_0_label_text              : ' 0'; )             k_drink_list_text           : 6                     string_table (      'Apple Juice',7                                         'Orange Juice', 6                                         'Grape Juice',/                                         'Cola', 0                                         'Punch',4                                         'Root beer',0                                         'Water',5                                         'Ginger Ale', /                                         'Milk', 1                                         'Coffee', 2                                         'Tea'   );F             k_drink_list_select         : string_table('Apple Juice');# 	    k_drink_init	    	: exported   # 				compound_string('Apple Juice'); /         k_ok_label_text                 : 'OK'; 2         k_apply_label_text              : 'Apply';2         k_reset_label_text              : 'Reset';3         k_cancel_label_text             : 'Cancel';  	k_create_init			: exported + 				compound_string('Create Order Box...');  	k_dismiss_init			: exported  ' 				compound_string('Close Order Box');  	k_fries_init			: exported 				compound_string('Medium');    = !Compound strings to use for context-sensitive help callbacks    value =         k_order_help            :  compound_string ("order"); =         k_print_help            :  compound_string ("print"); ?         k_options_help          :  compound_string ("options"); @         k_menu_bar_help         :  compound_string ("menu_bar");A         k_file_help             :  compound_string ("file_menu"); A         k_edit_help             :  compound_string ("edit_menu"); B         k_order_menu_help       :  compound_string ("order_menu");<         k_help_help             :  compound_string ("help");A         k_sensitive_help        :  compound_string ("sensitive"); >         k_onhelp_help           :  compound_string ("onhelp");=         k_about_help            :  compound_string ("about"); @         k_overview_help         :  compound_string ("overview");G         k_nyi_help              :  compound_string ("not_implemented");h    1 !String value to use for the Help System callback    valuei1         helpsys_order_help            :  'order';                     L  K ! substitute another font name if this font is not available on your systemD value ^         k_button_font           : font('-Adobe-Times-Bold-R-Normal--*-180-*-*-P-*-ISO8859-1');3 !        k_button_font          : font('variable');    valueT8         k_exit_accelerator              : "Ctrl<Key>z:";3         k_exit_accelerator_text         : "Ctrl/Z";W                  " ! Colors, color tables, and icons.   value ^         yellow                          : color('yellow', foreground);                        C         red                             : color('red', background);TE         green                           : color('green', foreground);.G         magenta                         : color('magenta', background);ED         gold                            : color('gold', foreground);I         lightblue                       : color('lightblue', background);M   valueL6         button_ct                       : color_table(:                                                 yellow='o'8                                                 ,red='.'G                                                 ,background color=' ');v   valueh  2         drink_up_icon: icon(color_table=button_ct,3                         '                        ', 3                         ' ..........oo.......... ',m3                         ' .........oooo......... ', 3                         ' ........oooooo........ ',d3                         ' .......oo....oo....... ',n3                         ' ......oo......oo...... ',*3                         ' .....oo........oo..... ',c3                         ' ....oo..........oo.... ',r3                         ' ...oo............oo... ',o3                         ' ..oo..............oo.. ',c3                         ' .oo................oo. ',f3                         ' oooooooooooooooooooooo ', 3                         ' oooooooooooooooooooooo ', 3                         ' .........oooo......... ',s3                         ' .........oooo......... ',a3                         ' .........oooo......... ',s3                         ' .........oooo......... ',53                         ' .........oooo......... ',h3                         ' .........oooo......... ',c4                         '                        ');     oE ! First declare the 'top level' widgets.  These are not controlled by*C ! any other widgets.  They are each fetched individually as needed.9  A object                                  ! The main window widget.EI                                         ! This displays the current orderw9                                         ! as it is taken.e&         S_MAIN_WINDOW : XmMainWindow {  I             controls {                  ! S_MAIN_WINDOW has two children.o+                 XmMenuBar       s_menu_bar;r,                 XmScrolledList  total_order;"                 };                             };    K object                                  ! A popup dialog box, called by anyiL     nyi : XmInformationDialog {         ! code that is not finished.  Useful:                                         ! for prototyping.           arguments {e;             XmNdialogTitle      = "DECburger: Information"; 3             XmNmessageString    = k_nyi_label_text;a'             XmNdefaultPosition  = true; '             XmNautoUnmanage     = true;g'             XmNnoResize         = true;=             };           callbacks {d@             MrmNcreateCallback  = procedure create_proc (k_nyi);             };
         };   !The help widget objectr   object     main_help : DXmHelpDialog {c         arguments            p             {gD             DXmNapplicationName = compound_string("Help Example");  >             DXmNglossaryTopic   = compound_string("glossary");@             DXmNoverviewTopic   = compound_string("overview");  C             DXmNlibrarySpec     = compound_string("decburger.hlb");              };         };                          !The print widget object$                                      object     main_print : DXmPrintDialog          {              ;         arguments                          {k5             XmNdialogTitle      = "DECburger: Print"; @             DXmNoptionsDialogTitle = "DECburger: Print Options";$             DXmNnumberCopies    = 2;'             DXmNunmanageOnOk    = true;1(             DXmNunmanageOnCancel = true;             };                 callbacks              {           J             XmNhelpCallback     =  procedure sens_help_proc(k_print_help);5             };                                       i         };                   y                        g   !The colormix widget objectg   object#     main_color : DXmColorMixDialog  	         {_         arguments            _             { @             XmNdialogTitle      = "DECburger: Background Color";F             DXmNmainLabel       = compound_string("Colormix Example");             };        !         callbacks                d             { K             XmNhelpCallback     = procedure sens_help_proc(k_options_help);_@             XmNcancelCallback   = procedure cancel_color_proc();<             XmNokCallback       = procedure ok_color_proc();?             XmNapplyCallback    = procedure apply_color_proc();n*             };                            7         };                                                                                     J ! Now define all remaining widgets alluded to in the definition of the top ! level widgets.                          I ! The main window has a list box for displaying the order as it is built.p   object"     total_order : XmScrolledList {         arguments { %             XmNvisibleItemCount = 15; +             XmNunitType         = XmPIXELS;u-             XmNlistSizePolicy   = XmCONSTANT;e              };                           callbacks {dH             MrmNcreateCallback  = procedure create_proc (k_total_order);J             XmNhelpCallback     = procedure sens_help_proc(k_order_help); !             };                    
         };>                                                               C ! Now define the menu bar and the pulldown widgets in the menu bar.p   object     s_menu_bar : XmMenuBar {(         arguments {                     /             XmNorientation      = XmHORIZONTAL;eE             XmNmenuHelpWidget   = XmCascadeButton help_menu_entry;                 };  0         controls {                              0             XmCascadeButton     file_menu_entry;0             XmCascadeButton     edit_menu_entry;5             XmCascadeButton     order_menu_entry;    o5             XmCascadeButton     options_menu_entry;  e4             XmCascadeButton     help_menu_entry;                 };         callbacks {tE             MrmNcreateCallback  = procedure create_proc (k_menu_bar); N             XmNhelpCallback     = procedure sens_help_proc(k_menu_bar_help);  ,             };                              
         };       M ! The file pulldown menu entry widget (what shows up in the menu bar itself).Ru ! All these pulldown entries can be set up to function in one of two ways by           ! removing comment characters:;C !       1. If the 'controls' clause specifying the pulldown menu is C !          commented out, then the pulldown menus are loaded by the_@ !          cascading callback when the entry is first activated.K !       2. If the 'controls' clause is not commented out, then the pulldown 5 !          menu is loaded when its parent is created. C ! Method 1 demonstrates deferred menu creation.  The pulldown entrye? ! creates its pulldown menu at a later time, rather than at the % ! time the pulldown entry is created.lP ! Method 2 demonstrates the standard technique -- creating the pulldown menu at ) ! the time the pulldown entry is created.    object'     file_menu_entry : XmCascadeButton {          arguments { 4             XmNlabelString      = k_file_label_text;.             XmNmnemonic         = keysym("F");:             };                                                       controls {*             XmPulldownMenu      file_menu;             };         callbacks { E             XmNcascadingCallback = procedure pull_proc (k_file_pdme); F             MrmNcreateCallback  = procedure create_proc (k_file_pdme);H             XmNhelpCallback     = procedure sens_help_proc(k_file_help);             };
         };    ; ! The file pulldown menu with the push buttons it controls.    object      file_menu : XmPulldownMenu {         controls {/             XmPushButton        m_print_button;r.             XmPushButton        m_exit_button;             };         callbacks {rF             MrmNcreateCallback  = procedure create_proc (k_file_menu);J             XmNhelpCallback     = procedure sens_help_proc(k_file_help);               };         };                      object                      #     m_print_button : XmPushButton {n         arguments {	N             XmNlabelString      = k_print_label_text;                         .             XmNmnemonic         = keysym("P");             };                                          callbacks {      fD             XmNactivateCallback = procedure activate_proc (k_print);L             XmNhelpCallback     = procedure sens_help_proc(k_print_help);                };                         };      4                                                      object"     m_exit_button : XmPushButton {         arguments {_P             XmNlabelString      = k_exit_label_text;                            5             XmNaccelerator      = k_exit_accelerator;r:             XmNacceleratorText  = k_exit_accelerator_text;.             XmNmnemonic         = keysym("E");             };           callbacks { J             XmNactivateCallback = procedure exit_proc('normal demo exit');             };,         };                                      ; ! The edit pulldown entry and the associated pulldown menu.m   object'     edit_menu_entry : XmCascadeButton {          arguments {t4             XmNlabelString      = k_edit_label_text;.             XmNmnemonic         = keysym("E");             };         controls {*             XmPulldownMenu      edit_menu;             };         callbacks {tE             XmNcascadingCallback = procedure pull_proc (k_edit_pdme);oF             MrmNcreateCallback  = procedure create_proc (k_edit_pdme);I             XmNhelpCallback     = procedure sens_help_proc(k_edit_help); t             };         };                   object      edit_menu : XmPulldownMenu {         controls {-             XmPushButton        m_cut_button; .             XmPushButton        m_copy_button;/             XmPushButton        m_paste_button; #             XmSeparator         {}; /             XmPushButton        m_clear_button; 4             XmPushButton        m_select_all_button;             };         callbacks { F             MrmNcreateCallback  = procedure create_proc (k_edit_menu);I             XmNhelpCallback     = procedure sens_help_proc(k_edit_help); o             };
         };    object!     m_cut_button : XmPushButton {          arguments { 7             XmNlabelString      = k_cut_dot_label_text; .             XmNmnemonic         = keysym("t");2             XmNaccelerator      = "<Key>DRemove:";+             XmNacceleratorText  = "Remove";              };-         callbacks {                           B             XmNactivateCallback = procedure activate_proc (k_nyi);H             XmNhelpCallback     = procedure sens_help_proc(k_nyi_help);              };         };                object"     m_copy_button : XmPushButton {         arguments {'8             XmNlabelString      = k_copy_dot_label_text;.             XmNmnemonic         = keysym("C");7             XmNaccelerator      = "Shift<Key>DRemove:";rB             XmNacceleratorText  = compound_string("Shift+Remove");             };         callbacks {'B             XmNactivateCallback = procedure activate_proc (k_nyi);H             XmNhelpCallback     = procedure sens_help_proc(k_nyi_help);              };         };                object#     m_paste_button : XmPushButton {          arguments {.9             XmNlabelString      = k_paste_dot_label_text;..             XmNmnemonic         = keysym("P");1             XmNaccelerator      = "<Key>Insert:";.<             XmNacceleratorText  = compound_string("Insert");             };         callbacks { B             XmNactivateCallback = procedure activate_proc (k_nyi);H             XmNhelpCallback     = procedure sens_help_proc(k_nyi_help);              }; i         };                object#     m_clear_button : XmPushButton {w         arguments { 9             XmNlabelString      = k_clear_dot_label_text;n.             XmNmnemonic         = keysym("r");             };         callbacks {WB             XmNactivateCallback = procedure activate_proc (k_nyi);I             XmNhelpCallback     = procedure sens_help_proc(k_nyi_help);  r             };         };                object(     m_select_all_button : XmPushButton {         arguments { :             XmNlabelString      = k_select_all_label_text;.             XmNmnemonic         = keysym("A");             };         callbacks { B             XmNactivateCallback = procedure activate_proc (k_nyi);H             XmNhelpCallback     = procedure sens_help_proc(k_nyi_help);              };         };      C ! The pulldown entry for ordering and its associated pulldown menu.    object(     order_menu_entry : XmCascadeButton {         arguments {r5             XmNlabelString      = k_order_label_text; .             XmNmnemonic         = keysym("r");             };         controls {+             XmPulldownMenu      order_menu;n             };         callbacks {uF             XmNcascadingCallback = procedure pull_proc (k_order_pdme);G             MrmNcreateCallback  = procedure create_proc (k_order_pdme);uJ             XmNhelpCallback     = procedure sens_help_proc(k_order_help);              };
   �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          >7�z ~      DWMV_JA_JPU6012.F              ]�  zp [F]DECBURGER.UIL_US_RENAME;1                                                                                                   �     ~                         0 z     1             };   object!     order_menu : XmPulldownMenu {r         controls {6             XmPushButton        m_show_control_button;#             XmSeparator         {}; 6             XmPushButton        m_cancel_order_button;6             XmPushButton        m_submit_order_button;             };                   callbacks { G             MrmNcreateCallback  = procedure create_proc (k_order_menu);:N             XmNhelpCallback     = procedure sens_help_proc(k_order_menu_help);,             };                              
         };                  object*     m_show_control_button : XmPushButton {         arguments {C.             XmNmnemonic         = keysym("C");              };                           callbacks { I             MrmNcreateCallback  = procedure create_proc (k_create_order);lL             XmNactivateCallback = procedure show_hide_proc (k_create_order);J             XmNhelpCallback     = procedure sens_help_proc(k_order_help);              };         };                object=     m_cancel_order_button : XmPushButton {                             arguments { ?             XmNlabelString      = k_cancel_order_label_text;   m.             XmNmnemonic         = keysym("n");             };               callbacks {cK             XmNactivateCallback = procedure activate_proc (k_cancel_order);dK             XmNhelpCallback     = procedure sens_help_proc(k_order_help);  _             };         };     object*     m_submit_order_button : XmPushButton {         arguments { <             XmNlabelString      = k_submit_order_label_text;.             XmNmnemonic         = keysym("S");             };         callbacks { K             XmNactivateCallback = procedure activate_proc (k_submit_order);eJ             XmNhelpCallback     = procedure sens_help_proc(k_order_help);              };=         };                                                      > ! The options pulldown entry and its associated pulldown menu.  	 object   ;*     options_menu_entry : XmCascadeButton {         arguments {a8             XmNlabelString      = k_options_label_text; 0             XmNmnemonic         = keysym("O");  )             XmNmappedWhenManaged = false;              };         controls {-             XmPulldownMenu      options_menu;              };+         callbacks {                        eI             MrmNcreateCallback  = procedure create_proc (k_options_pdme); K             XmNhelpCallback     = procedure sens_help_proc(k_options_help);              };           };         object                    #     options_menu : XmPulldownMenu {          controls {9             XmPushButton        m_options_control_button;              };         callbacks {nI             MrmNcreateCallback  = procedure create_proc (k_options_menu);mK             XmNhelpCallback     = procedure sens_help_proc(k_options_help);p             };
         };              object-     m_options_control_button : XmPushButton {r!         arguments {               =             XmNlabelString      = k_options_color_label_text;_0             XmNmnemonic         = keysym("C");               };               callbacks {wK             MrmNcreateCallback  = procedure create_proc (k_create_options);lM             XmNactivateCallback = procedure activate_proc (k_create_options);nK             XmNhelpCallback     = procedure sens_help_proc(k_options_help);l&             };                                 };                      ; ! The help pulldown entry and the associated pulldown menu.i   object8     help_menu_entry : XmCascadeButton {                          arguments { 4             XmNlabelString      = k_help_label_text;.             XmNmnemonic         = keysym("H");             };                   controls { o*             XmPulldownMenu      help_menu;             };         callbacks {dH             XmNhelpCallback     = procedure sens_help_proc(k_help_help);             };     e
         };                                object8     help_menu : XmPulldownMenu                          	         {m         controls   _             {                 /             XmPushButton        help_sensitive; -             XmPushButton        help_window;  .             XmPushButton        help_version; ,             XmPushButton        help_onhelp;             };         callbacksa*             {                             H             XmNhelpCallback     = procedure sens_help_proc(k_help_help);             }; l         };          +                                                object!     help_sensitive : XmPushButton          {                            argumentse             {                 9             XmNlabelString      = k_sensitive_label_text;n0             XmNmnemonic         = keysym("C");               };               callbacks ?             {                                                  uM             XmNactivateCallback = procedure activate_proc (k_help_sensitive); M             XmNhelpCallback     = procedure sens_help_proc(k_sensitive_help); 3             };                                     t;         };                                                     object     help_onhelp : XmPushButton         {                            arguments              { 6             XmNlabelString      = k_onhelp_label_text;.             XmNmnemonic         = keysym("H");             };     m         callbacks *             {                             J             XmNactivateCallback = procedure activate_proc (k_help_onhelp);J             XmNhelpCallback     = procedure sens_help_proc(k_onhelp_help);             };      ;         };                                                                  object"     help_version : XmPushButton   	         {          arguments /             {                                   5             XmNlabelString      = k_about_label_text; .             XmNmnemonic         = keysym("V");             };         callbacks              { I             XmNactivateCallback = procedure activate_proc (k_help_about);pI             XmNhelpCallback     = procedure sens_help_proc(k_about_help);n$             };                               };       object         g     help_window : XmPushButton  	         {e         arguments              { 8             XmNlabelString      = k_overview_label_text;.             XmNmnemonic         = keysym("W");             };          callbacks(             {                           L             XmNactivateCallback = procedure activate_proc (k_help_overview);L             XmNhelpCallback     = procedure sens_help_proc(k_overview_help);1             };                                   d
         };    M object                                  ! The control panel.  All order entry J                                         ! is done through this dialog box.      control_box : XmFormDialog {         arguments { 4             XmNdialogTitle      = k_decburger_title;4             XmNdialogStyle      = XmDIALOG_MODELESS;'             XmNnoResize         = true; (             XmNdefaultPosition  = false;&             XmNx                = 375;&             XmNy                = 100;(             XmNautoUnmanage     = false;(             XmNallowOverlap     = false;9             XmNdefaultButton    = XmPushButton ok_button;g=             XmNcancelButton     = XmPushButton cancel_button; &             XmNhorizontalSpacing = 10;%             XmNverticalSpacing  = 10; 8             XmNnavigationType   = XmEXCLUSIVE_TAB_GROUP;             };         controls {,             XmForm              burger_form;+             XmForm              fries_form;C,             XmForm              drinks_form;1             XmSeparator         button_separator; *             XmPushButton        ok_button;-             XmPushButton        apply_button;g-             XmPushButton        reset_button;m.             XmPushButton        cancel_button;2             XmSeparator         button_separator2;             };         callbacks {nF             MrmNcreateCallback  = procedure create_proc (k_order_box);R             XmNhelpCallback     = procedure help_system_proc (helpsys_order_help);             };
         };   object$     button_separator : XmSeparator {         arguments { 2             XmNtopAttachment    = XmATTACH_WIDGET;5             XmNtopWidget        = XmForm burger_form;e%             XmNtopOffset        = 10; /             XmNorientation      = XmHORIZONTAL; 0             XmNleftAttachment   = XmATTACH_FORM;0             XmNrightAttachment  = XmATTACH_FORM;@             XmNseparatorType    = XmNO_LINE;        ! invisible              };      
         };   object%     button_separator2 : XmSeparator {n         arguments { 2             XmNtopAttachment    = XmATTACH_WIDGET;9             XmNtopWidget        = XmPushButton ok_button; %             XmNtopOffset        = 10;a/             XmNorientation      = XmHORIZONTAL;s0             XmNleftAttachment   = XmATTACH_FORM;0             XmNrightAttachment  = XmATTACH_FORM;@             XmNseparatorType    = XmNO_LINE;        ! invisible              };
         };  H ! For the hamburger, fries, and drink entry we use a different mechanism0 ! to demonstrate various widgets and techniques.   b object     burger_form : XmForm {         arguments {m.             XmNshadowType       = XmSHADOW_IN;$             XmNshadowThickness  = 3;0             XmNtopAttachment    = XmATTACH_FORM;%             XmNtopOffset        = 10;c0             XmNleftAttachment   = XmATTACH_FORM;(             XmNallowOverlap     = false;8             XmNnavigationType   = XmEXCLUSIVE_TAB_GROUP;             };E         controls {                      ! Some labels and decoration.i-             XmLabel             burger_label;mO                                         ! Hamburger 'doneness' uses a radio boxmP                                         ! because although it is a '1 of N' typeL                                         ! of entry, one (and only one) entry5                                         ! is allowed. 4             XmRadioBox          burger_doneness_box;P                                         ! Toppings entry uses a menu with toggleM                                         ! buttons since more than one toppinglH                                         ! (M of N selection) is allowed.5             XmRowColumn         burger_toppings_menu; @                                         ! Number of burgers uses9                                         ! a scale widget.a0             XmScale             burger_quantity;             };
         };   object     burger_label : XmLabel {         arguments { :             XmNlabelString      = k_hamburgers_label_text;0             XmNtopAttachment    = XmATTACH_FORM;%             XmNtopOffset        = 10; 4             XmNleftAttachment   = XmATTACH_POSITION;%             XmNleftPosition     = 20;a4             XmNrightAttachment  = XmATTACH_POSITION;%             XmNrightPosition    = 80;r5             XmNalignment        = XmALIGNMENT_CENTER;C             };         callbacks {cI             MrmNcreateCallback  = procedure create_proc (k_burger_label);_             };         };          D                                                                      object&     burger_doneness_box : XmRadioBox {         arguments { -             XmNorientation      = XmVERTICAL;a2             XmNtopAttachment    = XmATTACH_WIDGET;7             XmNtopWidget        = XmLabel burger_label;C0             XmNleftAttachment   = XmATTACH_FORM;%             XmNleftOffset       = 10; 8             XmNnavigationType   = XmEXCLUSIVE_TAB_GROUP;             };         controls {,             XmToggleButton      burger_rare;.             XmToggleButton      burger_medium;,             XmToggleButton      burger_well;             };
         };   object"     burger_rare : XmToggleButton {         arguments { 4             XmNlabelString      = k_rare_label_text;K             };                                                             h         callbacks {tL             XmNvalueChangedCallback = procedure toggle_proc (k_burger_rare);H             MrmNcreateCallback  = procedure create_proc (k_burger_rare);             };
         };   object$     burger_medium : XmToggleButton {         arguments {e6             XmNlabelString      = k_medium_label_text;\             XmNset              = on;           ! A radio box requires one toggle to be set.             };         callbacks {eN             XmNvalueChangedCallback = procedure toggle_proc (k_burger_medium);J             MrmNcreateCallback  = procedure create_proc (k_burger_medium);             };
         };   object"     burger_well : XmToggleButton {         arguments {s9             XmNlabelString      = k_well_done_label_text;n             };         callbacks { L             XmNvalueChangedCallback = procedure toggle_proc (k_burger_well);H             MrmNcreateCallback  = procedure create_proc (k_burger_well);             };
         };     object(     burger_toppings_menu : XmRowColumn {         arguments { -             XmNorientation      = XmVERTICAL; 2             XmNtopAttachment    = XmATTACH_WIDGET;7             XmNtopWidget        = XmLabel burger_label; 2             XmNleftAttachment   = XmATTACH_WIDGET;A             XmNleftWidget       = XmRadioBox burger_doneness_box; %             XmNleftOffset       = 10;c0             XmNrightAttachment  = XmATTACH_FORM;%             XmNrightOffset      = 10;r8             XmNnavigationType   = XmEXCLUSIVE_TAB_GROUP;             };               controls {(             XmToggleButton      ketchup;(             XmToggleButton      mustard;'             XmToggleButton      pickle; &             XmToggleButton      onion;%             XmToggleButton      mayo;              };
         };       object     ketchup : XmToggleButton {         arguments { 7             XmNlabelString      = k_ketchup_label_text;              };         callbacks {lO             XmNvalueChangedCallback = procedure toggle_proc (k_burger_ketchup); K             MrmNcreateCallback  = procedure create_proc (k_burger_ketchup);u             };
         };     object      mustard : XmToggleButton {          arguments { 7             XmNlabelString      = k_mustard_label_text;              };         callbacks { O             XmNvalueChangedCallback = procedure toggle_proc (k_burger_mustard);eK             MrmNcreateCallback  = procedure create_proc (k_burger_mustard);              };
         };   object      onion : XmToggleButton {r         arguments { 5             XmNlabelString      = k_onion_label_text;              };         callbacks { M             XmNvalueChangedCallback = procedure toggle_proc (k_burger_onion);tI             MrmNcreateCallback  = procedure create_proc (k_burger_onion);s             };
         };   object     mayo : XmToggleButton {          arguments { :             XmNlabelString      = k_mayonnaise_label_text;             };         callbacks { L             XmNvalueChangedCallback = procedure toggle_proc (k_burger_mayo);H             MrmNcreateCallback  = procedure create_proc (k_burger_mayo);             };
         };   object     pickle : XmToggleButton {          arguments { 6             XmNlabelString      = k_pickle_label_text;             };         callbacks { N             XmNvalueChangedCallback = procedure toggle_proc (k_burger_pickle);J             MrmNcreateCallback  = procedure create_proc (k_burger_pickle);             };
         };     object     burger_quantity : XmScale { #         arguments {                r/             XmNorientation      = XmHORIZONTAL;h$             XmNminimum          = 0;%             XmNmaximum          = 10; 2             XmNtopAttachment    = XmATTACH_WIDGET;C             XmNtopWidget        = XmRowColumn burger_toppings_menu; %             XmNtopOffset        = 10; 4             XmNleftAttachment   = XmATTACH_POSITION;%             XmNleftPosition     = 20;_4             XmNrightAttachment  = XmATTACH_POSITION;%             XmNrightPosition    = 80; 0             XmNbottomAttachment = XmATTACH_FORM;%             XmNbottomOffset     = 10; 8             XmNtitleString      = k_quantity_label_text;'             XmNshowValue        = true; 8             XmNnavigationType   = XmEXCLUSIVE_TAB_GROUP;             };         callbacks { L             MrmNcreateCallback  = procedure create_proc (k_burger_quantity);�             XmNvalueChangedCallback = procedure scale_proc (k_burger_quantity);                                                                };  
         };     b object     fries_form : XmForm {l         arguments { .             XmNshadowType       = XmSHADOW_IN;$             XmNshadowThickness  = 3;0             XmNtopAttachment    = XmATTACH_FORM;%             XmNtopOffset        = 10; 2             XmNleftAttachment   = XmATTACH_WIDGET;5             XmNleftWidget       = XmForm burger_form;d2             XmNbottomAttachment = XmATTACH_WIDGET;?             XmNbottomWidget     = XmSeparator button_separator;l%             XmNbottomOffset     = 10;m(             XmNallowOverlap     = false;8             XmNnavigationType   = XmEXCLUSIVE_TAB_GROUP;             };         controls {,             XmLabel             fries_label;C                                         ! Fries size uses an option G                                         ! menu, for '1 of N' selection. 2             XmOptionMenu        fries_option_menu;E                                         ! Fries quantity uses a labeli:                                         ! and text widget.5             XmLabel             fries_quantity_label; /             XmText              fries_quantity;              };
         };   object     fries_label : XmLabel {          arguments { 5             XmNlabelString      = k_fries_label_text; 0             XmNtopAttachment    = XmATTACH_FORM;%             XmNtopOffset        = 10; 4             XmNleftAttachment   = XmATTACH_POSITION;%             XmNleftPosition     = 20; 4             XmNrightAttachment  = XmATTACH_POSITION;%             XmNrightPosition    = 80; 5             XmNalignment        = XmALIGNMENT_CENTER;r             };         callbacks { H             MrmNcreateCallback  = procedure create_proc (k_fries_label);
         };     };     object&     fries_option_menu : XmOptionMenu {         arguments {t4             XmNlabelString      = k_size_label_text;<             XmNmenuHistory      = XmPushButton medium_fries;2             XmNtopAttachment    = XmATTACH_WIDGET;6             XmNtopWidget        = XmLabel fries_label;%             XmNtopOffset        = 10; 0             XmNleftAttachment   = XmATTACH_FORM;%             XmNleftOffset       = 10; 0             XmNrightAttachment  = XmATTACH_FORM;%             XmNrightOffset      = 10;u8             XmNnavigationType   = XmEXCLUSIVE_TAB_GROUP;             };O         controls {                                                              +             XmPulldownMenu      fries_menu;I             };         callbacks {nM             MrmNcreateCallback  = procedure create_proc (k_fries_optionmenu);              };
         };   object!     fries_menu : XmPulldownMenu {          controls {+             XmPushButton        tiny_fries;a,             XmPushButton        small_fries;-             XmPushButton        medium_fries;g,             XmPushButton        large_fries;+             XmPushButton        huge_fries;              };
         };     object     tiny_fries : XmPushButton {T         arguments { 4             XmNlabelString      = k_tiny_label_text;             };         callbacks { I             XmNactivateCallback = procedure activate_proc (k_fries_tiny);_             };
         };   object      small_fries : XmPushButton {         arguments {i5             XmNlabelString      = k_small_label_text;              };         callbacks { J             XmNactivateCallback = procedure activate_proc (k_fries_small);             };
         };   object!     medium_fries : XmPushButton {          arguments {t6             XmNlabelString      = k_medium_label_text;             };F         callbacks {                                                   K             XmNactivateCallback = procedure activate_proc (k_fries_medium); I             MrmNcreateCallback  = procedure create_proc (k_fries_medium);n             };
         };   object      large_fries : XmPushButton {         arguments {o5             XmNlabelString      = k_large_label_text;              };         callbacks {uJ             XmNactivateCallback = procedure activate_proc (k_fries_large);             };
         };   object     huge_fries : XmPushButton {;         arguments {r4             XmNlabelString      = k_huge_label_text;             };         callbacks {bI             XmNactivateCallback = procedure activate_proc (k_fries_huge);t             };
         };     object$     fries_quantity_label : XmLabel {         arguments {s8             XmNlabelString      = k_quantity_label_text;4             XmNbottomAttachment = XmATTACH_POSITION;%             XmNbottomPosition   = 88;G4             XmNleftAttachment   = XmATTACH_POSITION;%             XmNleftOffset       = 20;d             };
         };   object       fries_quantity : XmText {          arguments { $             XmNcolumns          = 2;$             XmNrows             = 1;$             XmNmaxLength        = 2;1             XmNvalue            = k_0_label_text;a;             XmNtopAttachment    = XmATTACH_OPPOSITE_WIDGET; ?             XmNtopWidget        = XmLabel fries_quantity_label;l%             XmNtopOffset        = -5; 2             XmNleftAttachment   = XmATTACH_WIDGET;?             XmNleftWidget       = XmLabel fries_quantity_label;o%             XmNleftOffset       = 10; 8             XmNnavigationType   = XmEXCLUSIVE_TAB_GROUP;             };         callbacks { K             MrmNcreateCallback  = procedure create_proc (k_fries_quantity);eF             ! There is no text callback for text.  You have to provide*             ! one in the application code.
         };     };       object     drinks_form : XmForm {         arguments {v.             XmNshadowType       = XmSHADOW_IN;$             XmNshadowThickness  = 3;0             XmNtopAttachment    = XmATTACH_FORM;%             XmNtopOffset        = 10; 2             XmNleftAttachment   = XmATTACH_WIDGET;4             XmNleftWidget       = XmForm fries_form;%             XmNleftOffset       = 10; 0             XmNrightAttachment  = XmATTACH_FORM;%             XmNrightOffset      = 10;l2             XmNbottomAttachment = XmATTACH_WIDGET;?             XmNbottomWidget     = XmSeparator button_separator;a%             XmNbottomOffset     = 10;e(             XmNallowOverlap     = false;8             XmNnavigationType   = XmEXCLUSIVE_TAB_GROUP;             };         controls {,             XmLabel             drink_label;E                                         ! Drink selection uses a listgH                                         ! box because this is a '1 of N'K                                         ! selection where N is a big numbert/             XmScrolledList      drink_list_box; A                                         ! Drink quantity uses an pF                                         ! assembly of our own widgets./             XmForm              drink_quantity;              };
         };   object                   drink_label : XmLabel {s         arguments {m6             XmNlabelString      = k_drinks_label_text;0             XmNtopAttachment    = XmATTACH_FORM;%             XmNtopOffset        = 10; 4             XmNleftAttachment   = XmATTACH_POSITION;%             XmNleftPosition     = 20;t4             XmNrightAttachment  = XmATTACH_POSITION;%             XmNrightPosition    = 80;i5             XmNalignment        = XmALIGNMENT_CENTER; 
         };         callbacks { H             MrmNcreateCallback  = procedure create_proc (k_drink_label);         };           };   object%     drink_list_box : XmScrolledList {          arguments { $             XmNvisibleItemCount = 7;4             XmNitems            = k_drink_list_text;6             XmNselectedItems    = k_drink_list_select;2             XmNselectionPolicy  = XmSINGLE_SELECT;2             XmNtopAttachment    = XmATTACH_WIDGET;6             XmNtopWidget        = XmLabel drink_label;4             XmNleftAttachment   = XmATTACH_POSITION;%             XmNleftPosition     = 20;b4             XmNrightAttachment  = XmATTACH_POSITION;%             XmNrightPosition    = 80; 8             XmNnavigationType   = XmEXCLUSIVE_TAB_GROUP;             };         callbacks {tM             XmNsingleSelectionCallback  = procedure list_proc (k_drink_list);e'             };                          
         };   object     drink_quantity : XmForm {g         arguments {o4             XmNtopAttachment    = XmATTACH_POSITION;%             XmNtopPosition      = 70; 0             XmNleftAttachment   = XmATTACH_FORM;%             XmNleftOffset       = 10; 0             XmNrightAttachment  = XmATTACH_FORM;%             XmNrightOffset      = 10; (             XmNallowOverlap     = false;8             XmNnavigationType   = XmEXCLUSIVE_TAB_GROUP;             };         controls {/             XmLabel             quantity_label; ,             XmLabel             value_label;)             XmPushButton        up_value;_+             XmPushButton        down_value;a             };
         };     object(     quantity_label : XmLabel {                   arguments { 8             XmNlabelString      = k_quantity_label_text;0             XmNleftAttachment   = XmATTACH_FORM;$             XmNleftOffset       = 5;4             XmNtopAttachment    = XmATTACH_POSITION;J             XmNtopPosition      = 40;           ! 40/100 of height of form             };
         };   object"     value_label : XmLabel widget {         arguments {t'             XmNlabelString      = ' 0';i2             XmNleftAttachment   = XmATTACH_WIDGET;:             XmNleftWidget       = XmLabel quantity_label ;&             XmNleftOffset       = 10 ;4             XmNtopAttachment    = XmATTACH_POSITION;J             XmNtopPosition      = 40;           ! 40/100 of height of form             };         callbacks { K             MrmNcreateCallback  = procedure create_proc (k_drink_quantity);              };
         };   object$     up_value : XmPushButton widget {         arguments { 0             XmNtopAttachment    = XmATTACH_FORM;$             XmNtopOffset        = 5;2             XmNleftAttachment   = XmATTACH_WIDGET;%             XmNleftOffset       = 10;a7             XmNleftWidget       = XmLabel value_label ; +             XmNlabelType        = XmPIXMAP;t0             XmNlabelPixmap      = drink_up_icon;             };         callbacks {mH             XmNactivateCallback = procedure activate_proc (k_drink_add);             };
         };   object&     down_value : XmPushButton widget {         arguments { 2             XmNtopAttachment    = XmATTACH_WIDGET;8             XmNtopWidget        = XmPushButton up_value;2             XmNleftAttachment   = XmATTACH_WIDGET;%             XmNleftOffset       = 10; 7             XmNleftWidget       = XmLabel value_label ; +             XmNlabelType        = XmPIXMAP; :             XmNlabelPixmap      = xbitmapfile('icon.xbm');             };         callbacks {eH             XmNactivateCallback = procedure activate_proc (k_drink_sub);             };
         };     object%     ok_button : XmPushButton widget {          arguments {u2             XmNlabelString      = k_ok_label_text;1             XmNfontList         = k_button_font ; &             XmNhighlightColor   = red;$             XmNshowAsDefault    = 1;0             XmNdefaultButtonShadowThickness = 1;2             XmNtopAttachment    = XmATTACH_WIDGET;?             XmNtopWidget        = XmSeparator button_separator;t%             XmNtopOffset        = 10;              };         callbacks {t?             MrmNcreateCallback  = procedure create_proc (k_ok); A             XmNactivateCallback = procedure activate_proc (k_ok); 
         };     };   object(     apply_button : XmPushButton widget {         arguments { 5             XmNlabelString      = k_apply_label_text;e1             XmNfontList         = k_button_font ; &             XmNhighlightColor   = red;0             XmNdefaultButtonShadowThickness = 1;2             XmNtopAttachment    = XmATTACH_WIDGET;?             XmNtopWidget        = XmSeparator button_separator;l%             XmNtopOffset        = 10; 2             XmNleftAttachment   = XmATTACH_WIDGET;9             XmNleftWidget       = XmPushButton ok_button;              };         callbacks { B             MrmNcreateCallback  = procedure create_proc (k_apply);D             XmNactivateCallback = procedure activate_proc (k_apply);
         };     };   object(     reset_button : XmPushButton widget {         arguments {u5             XmNlabelString      = k_reset_label_text;U1             XmNfontList         = k_button_font ; &             XmNhighlightColor   = red;0             XmNdefaultButtonShadowThickness = 1;2             XmNtopAttachment    = XmATTACH_WIDGET;?             XmNtopWidget        = XmSeparator button_separator;n%             XmNtopOffset        = 10;e2             XmNleftAttachment   = XmATTACH_WIDGET;<             XmNleftWidget       = XmPushButton apply_button;             };           callbacks { B             MrmNcreateCallback  = procedure create_proc (k_reset);D             XmNactivateCallback = procedure activate_proc (k_reset);             };
         };   object@     cancel_button : XmPushButton widget {                                arguments { 6             XmNlabelString      = k_cancel_label_text;1             XmNfontList         = k_button_font ;c&             XmNhighlightColor   = red;0             XmNdefaultButtonShadowThickness = 1;2             XmNtopAttachment    = XmATTACH_WIDGET;?             XmNtopWidget        = XmSeparator button_separator;l%             XmNtopOffset        = 10;c2             XmNleftAttachment   = XmATTACH_WIDGET;<             XmNleftWidget       = XmPushButton reset_button;             };         callbacks { C             MrmNcreateCallback  = procedure create_proc (k_cancel); E             XmNactivateCallback = procedure activate_proc (k_cancel);              };
         };   end module;e                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            �               * [F]HELLOMOTIF.C;1 +  , _�   .     /     4 O      
 �                   - zp   0  � 1    2   3      K � P   W   O     5   6  �Z��֟  7  t���֟  8          9          G    H �� J ��                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          �~�� ~      DWMV_JA_JPU6012.F              _�  zp [F]HELLOMOTIF.C;1                                                                                                              O                              ��             /******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  ALL RIGHTS RESERVED
*  
*  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS  
*  
*  	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
*  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
*  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
*  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
*  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE.
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#include <stdio.h>


#include <Xm/Xm.h>          /* Motif Toolkit */
#include <Mrm/MrmPublic.h>   /* Mrm */

static MrmHierarchy	s_MrmHierarchy;		/* MRM database hierarch id */
static char		*vec[]={"hellomotif.uid"};
						/* MRM database file list   */
static MrmCode		class ;

static void helloworld_button_activate();

static MrmCount		regnum = 1 ;
static MrmRegisterArg	regvec[] = {
	{"helloworld_button_activate",(caddr_t)helloworld_button_activate}
	};

Display  	*display;
XtAppContext  	app_context;

/*
 *  Main program
 */
int main(argc, argv)
int argc;
char **argv;
{
     /*
     *  Declare the variables to contain the two widget ids
     */
    Widget toplevel, helloworldmain;
    Arg arglist[1] ;
    int n;

    /*
     *  Initialize the MRM
     */

    XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);

    XtToolkitInitialize();

    app_context = XtCreateApplicationContext();

    display = XtOpenDisplay(app_context, NULL, argv[0], "helloworldclass",
                            NULL, 0, &argc, argv);
    if (display == NULL) {
        fprintf(stderr, "%s:  Can't open display\n", argv[0]);
        exit(1);
    }

    MrmInitialize ();  

    n = 0;
    XtSetArg(arglist[n], XmNallowShellResize, True);  n++;
    toplevel = XtAppCreateShell(argv[0], NULL, applicationShellWidgetClass,
                              display, arglist, n);
    /*
     *  Define the Mrm.hierarchy (only 1 file)
     */
    if (MrmOpenHierarchy (1,			    /* number of files	    */
			vec, 			    /* files     	    */
			NULL,			    /* os_ext_list (null)   */
			&s_MrmHierarchy)	    /* ptr to returned id   */
			!= MrmSUCCESS) {
	printf ("can't open hierarchy\n");
     }

    /*
     * 	Register our callback routines so that the resource manager can 
     * 	resolve them at widget-creation time.
     */

    if (MrmRegisterNames (regvec, regnum)
			!= MrmSUCCESS)
			    printf("can't register names\n");

    /*
     *  Call MRM to fetch and create the pushbutton and its container
     */

    if (MrmFetchWidget (s_MrmHierarchy,
			"helloworld_main",
			toplevel,
			&helloworldmain,
			&class)
			!= MrmSUCCESS)
			    printf("can't fetch interface\n");

    /*
     *  Make the toplevel widget "manage" the main window (or whatever the
     *  the uil defines as the topmost widget).  This will
     *  cause it to be "realized" when the toplevel widget is "realized"
     */

    XtManageChild(helloworldmain);
    
    /*
     *  Realize the toplevel widget.  This will cause the entire "managed"
     *  widget hierarchy to be displayed
     */

    XtRealizeWidget(toplevel);

    /*
     *  Loop and process events
     */

    XtAppMainLoop(app_context);

    /* UNREACHABLE */
    return (0);
}

static void helloworld_button_activate( widget, tag, callback_data )
	Widget	widget;
	char    *tag;
	XmAnyCallbackStruct *callback_data;
{
    Arg arglist[2];

    static int call_count = 0;

    char *value;
    MrmCode type;
    XmString cs_ptr;
    long byte_count, status;

    call_count += 1 ;
    switch ( call_count )
        {
        case 1:
	    if (MrmFetchLiteral(s_MrmHierarchy, "goodbye_string",
		display, &value, &type) == MrmSUCCESS)
	    cs_ptr = (XmString) DXmCvtFCtoCS(value, &byte_count, &status);
	    XtSetArg( arglist[0], XmNlabelString, cs_ptr );
	    XtSetValues( widget, arglist, 1 );
	    XmStringFree(cs_ptr);
	    XtFree(value);
            break ;
        case 2:
            exit(1);
            break ;
        }
}
                                                                                                                      �               * [F]HELLOMOTIF.COM_US_RENAME;1 +  , }�   .     /     4 W      	 r                    - zp   0  � 1    2   3      K � P   W   O 
    5   6 �����֟  7 `����֟  8          9          G    H �� J �                 # $ ! Build the Motif hellomotif Demo I $ ! The files hellomotif.uid and hellomotif.exe will be produced by this   $ ! command procedure. $ ! K $ ! Make sure that the prerequisite files and logicals exist.  This command L $ ! procedure requires that the DECwindows programming and user environmentsM $ ! have been installed and that the DECW$STARTUP command procedure has been   $ ! invoked.   $ ! " $ arch_type = f$getsyi("hw_model") $ if arch_type .le. 1023 $ then $    arch_type = "VAX" $ else $    arch_type = "AXP" $ endif J $ if f$search("sys$system:decw$uilmotif.exe") .nes. "" then goto libskitokF $ write sys$output "DECwindows programming environment not installed."3 $ write sys$output "hellomotif demo build aborted."  $ exit $libskitok: < $ if f$search("sys$share:decw$xlibshr.exe") .nes. ""	.and. -J      f$search("sys$share:decw$dxmlibshr.exe") .nes. ""	then goto appskitok? $ write sys$output "DECwindows user environment not installed." 3 $ write sys$output "hellomotif demo build aborted."  $ exit $appskitok: . $ if f$trnlnm("decw$include") .nes. ""	.and. -;      f$trnlnm("decw$examples") .nes. ""	then goto startupok O $ write sys$output "You must invoke @DECW$STARTUP before using this procedure." 3 $ write sys$output "hellomotif demo build aborted."  $ exit $startupok:  $ if arch_type .eqs. "VAX" $ thenF $     if f$search("sys$system:vaxc.exe") .nes. "" then goto compilerok8 $     write sys$output "VAXC compiler is not available."7 $     write sys$output "hellomotif demo build aborted." 
 $     exit $ elseO $     if f$search("sys$system:decc$compiler.exe") .nes. "" then goto compilerok 9 $     write sys$output "DEC C compiler is not available." 7 $     write sys$output "hellomotif demo build aborted." 
 $     exit $ endif  $compilerok: $ if arch_type .eqs. "VAX" $ thenC $     if f$search("sys$library:stdio.h") .nes. "" then goto stdiook > $     write sys$output "SYS$LIBRARY:STDIO.H is not available."4 $     write sys$output "Create it with the command:"W $     write sys$output " $ LIBRARY/TEXT/EXTRACT=STDIO/OUT=SYS$COMMON:[SYSLIB]STDIO.H -" 0 $     write sys$output "    SYS$LIBRARY:VAXCDEF"7 $     write sys$output "hellomotif demo build aborted." 
 $     exit $ elseK $     if f$search("sys$library:decc$rtldef.tlb") .nes. "" then goto stdiook > $     write sys$output "SYS$LIBRARY:STDIO.H is not available."4 $     write sys$output "Create it with the command:"W $     write sys$output " $ LIBRARY/TEXT/EXTRACT=STDIO/OUT=SYS$COMMON:[SYSLIB]STDIO.H -" 0 $     write sys$output "    SYS$LIBRARY:VAXCDEF"7 $     write sys$output "hellomotif demo build aborted." 
 $     exit $ endif 	 $stdiook:  $ !  $ ! Build hellomotif and run it  $ ! 0 $ write sys$output "Compiling hellomotif.UIL...", $ define/user/nolog uil$include decw$include( $ uil/motif decw$examples:hellomotif.uil $ . $ write sys$output "Compiling hellomotif.C..."6 $ define/user/nolog c$include decw$include,sys$library* $ define/user/nolog vaxc$include c$include $ if arch_type .eqs. "VAX" $ thenH $     cc /define=(_NO_PROTO,XK_LATIN1,VMS,VMSV5,"vms",__STARLET_LOADED,-C __LIBS$ROUTINES_LOADED,X_LOCALE,XLIB_XPG4_FUNCS,_WCHAR_T_,X_WCHAR,- 6 I18N_BUG_FIX,I18N_MULTIBYTE) /INCLUDE=(DECW$EXAMPLES,- sys$share,decw$include)- /OPT decw$examples:hellomotif  $ elseF $     cc /standard=vaxc /define=(_NO_PROTO,XK_LATIN1,VMS,VMSV5,"vms",-B __STARLET_LOADED,__LIBS$ROUTINES_LOADED,X_LOCALE,XLIB_XPG4_FUNCS,-H _WCHAR_T_,X_WCHAR,I18N_BUG_FIX,I18N_MULTIBYTE) /INCLUDE=(DECW$EXAMPLES,- sys$share,decw$include)- /OPT decw$examples:hellomotif  $ endif * $ write sys$output "Linking hellomotif..." $ if arch_type .eqs. "VAX" $ then$ $     link hellomotif,sys$input/opt ! sys$share:decw$dxmlibshr12/share   sys$share:decw$xlibshr/share    sys$share:decw$xtlibshrr5/share   sys$share:decw$xmlibshr12/share ! sys$share:decw$mrmlibshr12/share   sys$share:vaxcrtl/share  $  $ else# $     link hellomotif,sys$input/opt ! sys$share:decw$dxmlibshr12/share   sys$share:decw$xlibshr/share    sys$share:decw$xtlibshrr5/share   sys$share:decw$xmlibshr12/share ! sys$share:decw$mrmlibshr12/share   $  $ endif  $ * $ write sys$output "Running hellomotif..." $ run hellomotif $  $ exit                                                                                                                                                                                                                                                                                                                                                                                                              �               * [F]HELLOMOTIF.EXE;1 +  , ��   .     /     4                             - zp   0  � 1    2   3      K � P   W   O     5   6 `:	��֟  7 �����֟  8          9          G    H �� J �                           � 0 D X     0205  ��������    (  k��E                                              
HELLOMOTIF                             V1.0           �,k��ŗ 05-05               �           �           
       ��? �  �)              DECW$DXMLIBSHR12_001.      !       DECW$TRANSPORT_COMMON_001 �     !        
LIBRTL_001 O    !      � �
MTHRTL_001      !      	  
LBRSHR_001(              DECW$XMLIBSHR12_001)              DECW$MRMLIBSHR12_001�����������������������(              DECW$XTLIBSHRR5_001% �    !        dDECW$XLIBSHR_001       !        VAXCRTL_001  �����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������hellomotif.uid helloworld_button_activate helloworldclass %s:  Can't open display
 can't open hierarchy
 can't register names
 helloworld_main can't fetch interface
 goodbye_string                ,                                                                                                                                                                                                                                                                                                                      | �^��  �����V�����U������S�����T|~� ��~  � �s  � �x  �PfЬR�R߬|~ߣ*�b� �f��V  �Pe�bߣ:���  ���  ����  � ��  ���  ������߭��e��.  � ݼ���  �PR�d� ߤ����  �P
ߣS��H  2�
~ߤ���  �P
ߣi��+  ?�߭��Rߣ�d���  �P�Ï ��  ݭ����  �R���  �f��w  �P �$^������S��z���R֢ѢѢb?��߭���x����æ �b��  �P߭�߭�ݭ����   �PT���  ���T���߭�ݬ��   �T���   ݭ���  ���S                                                                                @   @   �  (  4                                           �  	      �  �=  �8  �@  �O  �O  (N   P  l�        �    (   8            P  0  �  h  P  H  �  �  �
      �  �  �  �     �  �  �     �  �  �  (  �  �     
   
   �  |         ,
    ��                          @                                                                       DECW$DXMLIBSHR12                                               DECW$TRANSPORT_COMMON                                          LIBRTL                                                         MTHRTL                                                         LBRSHR                                                         DECW$XMLIBSHR12                                                DECW$MRMLIBSHR12                                               DECW$XTLIBSHRR5                                                DECW$XLIBSHR                                                   VAXCRTL                                      $              
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             �               * [F]HELLOMOTIF.UID_US_RENAME;1 +  , a�   .     /     4                           - zp   0  � 1    2   3      K � P   W   O     5   6 ����֟  7 �����֟  8          9          G    H �� J �                  ro|  URM 1.2   Motif Uil Compiler            V2.0-000  27-MAR-1994 00:04:25.14       helloworld                    v1.0                 	                 � d � � l �                                                         d
�
>��H      0   �X         helloworld_main                     ��H         Goodbye
World!    ��H      � D ��I�         0 D             helloworld_main                              �        �        p   helloworld_button   picture_label   helloworld_label      ��H      � h ��I�         4               helloworld_label                      
          	    
              
       �                 ��H      p $߀l 	ISO8859-1Press button once  	ISO8859-1to change label;  	ISO8859-1twice to exit.  ��H       ���I         0               picture_label 	I                  �       
              
          
    �   	    
              
       �                    �    helloworld_button      �    helloworld_label        ��H      eP8v�	. $             )          """""""""""""""""""""""2333333333333333333333#23333C4333333333C43333#23333DD333333333DD3333#2333CDD43333333CDD4333#2333D33D3333333D33D333#233C433C433333C433C433#233D3333D33333D3333D33#23C43333C4333C43333C43#23D333333D333D333333D3#2C4333333C43C4333333C4#2D33333333D3D33333333D#BDDDDDDDDDDDDDDDDDDDDD$BDDDDDDDDDDDDDDDDDDDDD$23333DD333333333DD3333#23333DD333333333DD3333#23333DD333333333DD3333#23333DD333333333DD3333#23333DD333333333DD3333#2333333333333333333333#2333333333333333333333#2333333333333333333333#2333333333333333333333#233333333CDDD433333333#233333333CDDD433333333#2333333333333333333333#2333333333333333333333#2333333333333333333333#233333D333333333D33333#233333C43333333C433333#2333333DDDDDDDDD333333#2333333CDDDDDDD4333333#2333333333333333333333#2333333333333333333333#2333333333333333333333#""""""""""""""""""""""" ��H      | d�Ώ                                       L            \            l                                ��H       �black       ��H       lLightGray       ��H      ���I        4               helloworld_button   
                  �       
              
          	    
       
       �      	 �   h�f 33  �      #233C        33C43helloworld_button_activate 3       C4 �    helloworld_label 3D3#2  ��H      7 �߀3 	ISO8859-1Hello  	ISO8859-1World!   ��H      1��Z�         R   _   p   }   �   �   �   �   �   �   �   �   �   �         allowOverlap bottomAttachment bottomOffset labelPixmap labelString labelType leftAttachment leftOffset leftWidget rightAttachment rightOffset shadowThickness subMenuId topAttachment topOffset topWidget activateCallback      ��H      L  	�Z�            '   5   XmCreateForm XmCreateLabel XmCreatePushButton                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     ro|     p@p d
|  	� D �   � �� $� h � P                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                >ClassTable >ResourceTable  helloworld_button   picture_label   helloworld_label    helloworld_main goodbye_string  helloworld  �               * [F]HELLOMOTIF.UIL_US_RENAME;1 +  , `�   .     /     4 B      
 	                    - zp   0  � 1    2   3      K � P   W   O     5   6  �w��֟  7 �"���֟  8          9          G    H �� J �                 ! 
! @DEC_COPYRIGHT@
!
! HISTORY
! $Log:	helloworld.uil,v $
! Revision 3.0.1.1  94/01/25  16:33:28  uws
! Create gold branch
! 
! Revision 3.0  1994/01/25  16:33:25  uws
! Create gold branch
!
! Revision 1.5  93/02/19  08:46:58  lionel
! *** empty log message ***
! 
! Revision 1.4  93/02/17  06:47:46  lionel
! *** empty log message ***
! 
! Revision 1.3  1993/02/10  05:56:24  lionel
! *** empty log message ***
!
! Revision 1.2  1993/02/10  03:21:33  lionel
! *** empty log message ***
!
! Revision 1.1  1993/02/09  06:19:55  lionel
! Initial revision
!
! Revision 1.2  93/01/06  23:03:08  uws
! *** empty log message ***
! 
! Revision 1.1.2.2  92/06/11  09:08:20  Dave_Hill
! 	initial submittal from contrib sources
! 
! 
! $EndLog$
! 

module helloworld
    version = 'v1.0'
    names = case_sensitive

procedure
    helloworld_button_activate();

value black  : color('black',foreground);
value white  : color('LightGray',background);

value my_colors : color_table(black = 'o',white='.',white=' ');
value my_picture : icon(color_table = my_colors,
'                                              ',
' ............................................ ',
' ..........oo....................oo.......... ',
' .........oooo..................oooo......... ',
' ........oooooo................oooooo........ ',
' .......oo....oo..............oo....oo....... ',
' ......oo......oo............oo......oo...... ',
' .....oo........oo..........oo........oo..... ',
' ....oo..........oo........oo..........oo.... ',
' ...oo............oo......oo............oo... ',
' ..oo..............oo....oo..............oo.. ',
' .oo................oo..oo................oo. ',
' oooooooooooooooooooooooooooooooooooooooooooo ',
' oooooooooooooooooooooooooooooooooooooooooooo ',
' .........oooo..................oooo......... ',
' .........oooo..................oooo......... ',
' .........oooo..................oooo......... ',
' .........oooo..................oooo......... ',
' .........oooo..................oooo......... ',
' ............................................ ',
' ............................................ ',
' ............................................ ',
' ............................................ ',
' ..................oooooooo.................. ',
' ..................oooooooo.................. ',
' ............................................ ',
' ............................................ ',
' ............................................ ',
' ...........oo..................oo........... ',
' ............oo................oo............ ',
' .............oooooooooooooooooo............. ',
' ..............oooooooooooooooo.............. ',
' ............................................ ',
' ............................................ ',
' ............................................ ',
'                                              ');

value goodbye_string: exported('Goodbye\nWorld!');
value helloworld_string:
	compound_string('Hello',separate=true) & 'World!';
value message_string:
	compound_string('Press button once',separate=true) &
	compound_string('to change label;',separate=true) &
	'twice to exit.';


object
     helloworld_main : XmForm {
	arguments {
	    XmNallowOverlap = false;
	};
	controls {
	    XmLabel	helloworld_label;
	    XmLabel	picture_label;
	    XmPushButton	helloworld_button;
	};
    };

object
    helloworld_button : XmPushButton {
	arguments { 
	    XmNtopAttachment = XmATTACH_WIDGET;
	    XmNtopWidget = XmLabel helloworld_label;
	    XmNtopOffset = 10;
	    XmNbottomAttachment = XmATTACH_FORM;
	    XmNbottomOffset = 10;
	    XmNleftAttachment = XmATTACH_FORM;
	    XmNleftOffset = 10;
	    XmNshadowThickness = 10;
	    XmNlabelString = helloworld_string;
	};
	callbacks { 
	    XmNactivateCallback = procedure helloworld_button_activate();
	};
    };

object
    helloworld_label : XmLabel {
	arguments {
	    XmNtopAttachment = XmATTACH_FORM;
	    XmNtopOffset = 10;
	    XmNleftAttachment = XmATTACH_FORM;
	    XmNleftOffset = 10;
	    XmNrightAttachment = XmATTACH_FORM;
	    XmNrightOffset = 10;
	    XmNlabelString = message_string;
	};
    };

object
    picture_label : XmLabel {
	arguments {
	    XmNtopAttachment = XmATTACH_WIDGET;
	    XmNtopWidget = XmLabel helloworld_label;
	    XmNtopOffset = 10;
	    XmNbottomAttachment = XmATTACH_FORM;
	    XmNbottomOffset = 10;
	    XmNleftAttachment = XmATTACH_WIDGET;
	    XmNleftWidget = XmPushButton helloworld_button;
	    XmNleftOffset = 10;
	    XmNrightAttachment = XmATTACH_FORM;
	    XmNrightOffset = 10;
	    XmNlabelPixmap = my_picture;
	    XmNlabelType = XmPIXMAP;
	};
    };

end module;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       �               * [F]IMS_EXAMPLE.C;1 +  , g�   .     /     4 N       �  �                - zp   0  � 1    2   3      K � P   W   O     5   6 `�-��֟  7 �/���֟  8          9          G    H �� J �                            /*
*****************************************************************************

              Copyright (c) Digital Equipment Corporation, 1994
              All Rights Reserved.  Unpublished rights reserved
              under the copyright laws of the United States.

              The software contained on this media is proprietary
              to and embodies the confidential technology of
              Digital Equipment Corporation.  Possession, use,
              duplication or dissemination of the software and
              media is authorized only pursuant to a valid written
              license from Digital Equipment Corporation.

              RESTRICTED RIGHTS LEGEND   Use, duplication, or
              disclosure by the U.S. Government is subject to
              restrictions as set forth in Subparagraph (c)(1)(ii)
              of DFARS 252.227-7013, or in FAR 52.227-19, as
              applicable.

*****************************************************************************
 *
 * IMS_example.c
 *
 * This is an example of how to write an Input Method Server for X11R5
 * In this example, there are only 26 composed characters, which are
 * formed by 2 consecutive key inputs which are equal. These key inputs
 * should be from a-z, case insensitive. The lookup string
 * will be found from an array, which is some kind of small-scale dictionary.
 *
 * You should load Iso-latin 1 keymap files in order to work properly for
 * this example.
 *
 * You can start this example using four of the languages:
 *	Korean, Traditional Chinese, Simplied Chinese and Japanese
 * 
 * To start this IMServer, set the xnllanguage resource properly, for example,
 * if you want to input Korean text, you should set the session language to
 * Korean, or simply start as
 * 	$IMS_example -xnllanguage "ko_KR.deckorean"
 *
 * To connect a client to this IMS, simply start up this IMS as a background
 * job, then start the client with the correct language setting and XMODIFIERS
 * environment. For example,
 * 	%define XMODIFIERS "@im=exam"
 *      %mc decw$cardfiler -xnllanguage "ko_KR.deckorean"
 */

#include <X11/Intrinsic.h>
#include <X11/keysymdef.h>
#ifdef VMS
#include <X11/DIMsDef.h>
#else
#include <DIMs/DIMsDef.h>
#endif

static char my_dictionary[26][2] = 	{0xa1, 0xa1,
				 	 0xa1, 0xa2,
				 	 0xa1, 0xa3,
				 	 0xa1, 0xa4,
				 	 0xa1, 0xa5,
				 	 0xa1, 0xa6,
				 	 0xa1, 0xa7,
				 	 0xa1, 0xa8,
				 	 0xa1, 0xa9,
				 	 0xa1, 0xaa,
				 	 0xa1, 0xab,
				 	 0xa1, 0xac,
				 	 0xa1, 0xad,
				 	 0xa1, 0xae,
				 	 0xa1, 0xaf,
				 	 0xa1, 0xb1,
				 	 0xa1, 0xb2,
				 	 0xa1, 0xb3,
				 	 0xa1, 0xb4,
				 	 0xa1, 0xb5,
				 	 0xa1, 0xb6,
				 	 0xa1, 0xb7,
				 	 0xa1, 0xb8,
				 	 0xa1, 0xb9,
				 	 0xa1, 0xba,
				 	 0xa1, 0xbb};

typedef struct {
   char data[1];
   int  num_char;
} MyDataRec, *MyData; 	/* This is the input context specific data */

static int CreateICCallback(w, client_data, widget_data)
Widget w;
Opaque *client_data;
Opaque **widget_data;
{
    MyData my_data = (MyData)XtMalloc(sizeof(MyDataRec));

    if (!my_data)
	return 0;	/* fail */

    my_data->num_char = 0;
    *widget_data = (Opaque *)my_data;

    return 1; /* success */
}

static int DestroyICCallback(w, client_data, widget_data)
Widget w;
Opaque *client_data;
{
     MyData my_data = (MyData)DIMsUserData(w);
     
     if (my_data)
	XtFree((char *)my_data);

     return 1; /* success */
}

static int GetFilterEventsCallback(w, client_data, mask)
Widget w;
Opaque *client_data;
unsigned long **mask;
{
   *mask = (unsigned long *) XtMalloc(sizeof�                                                                                                                                                                                                                                                                                                                                                                                                                                                          )! ~      DWMV_JA_JPU6012.F              g�  zp [F]IMS_EXAMPLE.C;1                                                                                                             N   �                          8�             (unsigned long));

/*  Since this IMS supports root-window style only, so just KeyPress
    events are necessary */

   **mask = (unsigned long)(KeyPressMask);
}

static int ProcessEventCallback(w, client_data, event, send_back)
Widget w;
Opaque *client_data;
XEvent *event;
Boolean *send_back;
{

    MyData my_data = (MyData)DIMsUserData(w);
    Display *dpy  = XtDisplay(w);

    *send_back = True;

    switch (event->type)
    {
       case KeyPress :
	    {
		KeyCode keycode = event->xkey.keycode;
		KeySym keysym;
		char second_char;
		XTextProperty text_property;
		char *text_list[1];
		char text[3];

		keysym = XKeycodeToKeysym( dpy, keycode, 0 );
		if ((keysym >= XK_A && keysym <= XK_Z) || 
		    (keysym >= XK_a && keysym <=XK_z))
		{
		   if (my_data->num_char) 
		   {
		      if (keysym >= XK_a)
			 second_char = keysym - XK_a;
		      else
		    	 second_char = keysym - XK_A;
		      if (second_char == my_data->data[0])
		      { /* two consecutive key are equal, send commit string */
			 text[0] = my_dictionary[second_char][0];
			 text[1] = my_dictionary[second_char][1];
			 text[2] = '\0';
			 text_list[0] = text;
			 XmbTextListToTextProperty(dpy, text_list, 1,
			 	XCompoundTextStyle, &text_property);
			 DIMsCommit(w, (char *)text_property.value,
				strlen((char *)text_property.value)+1);
			 XtFree((char *)text_property.value);
			 *send_back = False; /* the key is used by IMS,
						no need to return to client */
		      }	
		      my_data->num_char = 0; /* reset to 0*/
		   } else {
		      my_data->num_char = 1;
		      if (keysym >= XK_a)
			 my_data->data[0] = keysym - XK_a;
		      else
		    	 my_data->data[1] = keysym - XK_A;
		      *send_back = False;
		   }
		}
 	    break;	
	    }
      default :
	    break; 
    }
}
       
main(argc, argv)
int argc;
char **argv;
{
    Widget	toplevel;
    XtAppContext app_context;
    DIMsClassCtx zh_tw_ctx, ja_jp_ctx, zh_cn_ctx, ko_kr_ctx;
    char	*server_strings[] = {"exam"};
    XIMStyle support_styles[1] = {XIMPreeditNothing | XIMStatusNothing};
    DIMsLocaleInfoRec locale;
    DIMsStringListRec server_names;
    DIMsSupportStylesRec im_styles;
    Cardinal n;
    Arg al[30];

    XtSetLanguageProc(NULL, NULL, NULL);

    toplevel = XtAppInitialize(&app_context, "example", NULL, 0,
				&argc, argv, NULL, NULL, 0);
	

/* set up DIMs Class */
/* This example targets to work for 4 locales: zh_TW, ja_JP, zh_CN, ko_KR
   Therefore 4 DIMs class contexts are created */
/* create contexts for locales zh_TW, ja_JP, zh_CN, ko_KR */

    locale.language = "zh_TW";
    locale.num_support_codesets = 0;
    locale.support_codesets = NULL;

    server_names.num_of_strings = 1;
    server_names.string_names = server_strings;

    im_styles.num_support_styles = 1;
    im_styles.support_styles = support_styles;

    n = 0;
    XtSetArg( al[n], DIMsNlocale, &locale);n++;
    XtSetArg( al[n], DIMsNserverName, &server_names);n++;
    XtSetArg( al[n], DIMsNsupportStyles, &im_styles);n++;
    XtSetArg( al[n], DIMsNcreateICCb, CreateICCallback);n++;
    XtSetArg( al[n], DIMsNdestroyICCb, DestroyICCallback);n++;
    XtSetArg( al[n], DIMsNprocessEventCb, ProcessEventCallback);n++;
    XtSetArg( al[n], DIMsNgetFilterEventsCb, GetFilterEventsCallback);n++;
    zh_tw_ctx = DIMsClassCtxCreate(NULL, "zh_tw_exam", al, n);

    locale.language = "ja_JP";
    XtSetArg( al[0], DIMsNlocale, &locale);
    ja_jp_ctx = DIMsClassCtxCreate(zh_tw_ctx, "ja_jp_exam", al, n);

    locale.language = "zh_CN";
    XtSetArg( al[0], DIMsNlocale, &locale);
    zh_cn_ctx = DIMsClassCtxCreate(zh_tw_ctx, "zh_cn_exam", al, n);

    locale.language = "ko_KR";
    XtSetArg( al[0], DIMsNlocale, &locale);
    ko_kr_ctx = DIMsClassCtxCreate(zh_tw_ctx, "ko_kr_exam", al, n);

/* Initialize IMSSL */
    DIMsSLInitialize(toplevel, zh_tw_ctx);

/* Realize widget hierrachy */
    XtRealizeWidget(toplevel);

/* Initialize protocol layer */
    DIMsProtoInit(toplevel, app_context);

    XtAppMainLoop(app_context);
}
                                                                                                        �               * [F]IMS_EXAMPLE.COM;1 +  , h�   .     /     4 W      	 6                    - zp   0  � 1    2   3      K � P   W   O 
    5   6 �L��֟  7  =���֟  8          9          G    H �� J �                          $ $ ! Build the Motif ims_example DemoK $ ! The files ims_example.uid and ims_example.exe will be produced by this   $ ! command procedure. $ ! K $ ! Make sure that the prerequisite files and logicals exist.  This command L $ ! procedure requires that the DECwindows programming and user environmentsM $ ! have been installed and that the DECW$STARTUP command procedure has been   $ ! invoked.   $ ! " $ arch_type = f$getsyi("hw_model") $ if arch_type .le. 1023 $ then $    arch_type = "VAX" $ else $    arch_type = "AXP" $ endif J $ if f$search("sys$system:decw$uilmotif.exe") .nes. "" then goto libskitokF $ write sys$output "DECwindows programming environment not installed."4 $ write sys$output "ims_example demo build aborted." $ exit $libskitok: < $ if f$search("sys$share:decw$xlibshr.exe") .nes. ""	.and. -J      f$search("sys$share:decw$dxmlibshr.exe") .nes. ""	then goto appskitok? $ write sys$output "DECwindows user environment not installed." 4 $ write sys$output "ims_example demo build aborted." $ exit $appskitok: . $ if f$trnlnm("decw$include") .nes. ""	.and. -;      f$trnlnm("decw$examples") .nes. ""	then goto startupok O $ write sys$output "You must invoke @DECW$STARTUP before using this procedure." 4 $ write sys$output "ims_example demo build aborted." $ exit $startupok:  $ if arch_type .eqs. "VAX" $ thenF $     if f$search("sys$system:vaxc.exe") .nes. "" then goto compilerok8 $     write sys$output "VAXC compiler is not available."8 $     write sys$output "ims_example demo build aborted."
 $     exit $ elseO $     if f$search("sys$system:decc$compiler.exe") .nes. "" then goto compilerok 9 $     write sys$output "DEC C compiler is not available." 8 $     write sys$output "ims_example demo build aborted."
 $     exit $ endif  $compilerok: $ if arch_type .eqs. "VAX" $ thenC $     if f$search("sys$library:stdio.h") .nes. "" then goto stdiook > $     write sys$output "SYS$LIBRARY:STDIO.H is not available."4 $     write sys$output "Create it with the command:"W $     write sys$output " $ LIBRARY/TEXT/EXTRACT=STDIO/OUT=SYS$COMMON:[SYSLIB]STDIO.H -" 0 $     write sys$output "    SYS$LIBRARY:VAXCDEF"8 $     write sys$output "ims_example demo build aborted."
 $     exit $ elseK $     if f$search("sys$library:decc$rtldef.tlb") .nes. "" then goto stdiook > $     write sys$output "SYS$LIBRARY:STDIO.H is not available."4 $     write sys$output "Create it with the command:"W $     write sys$output " $ LIBRARY/TEXT/EXTRACT=STDIO/OUT=SYS$COMMON:[SYSLIB]STDIO.H -" 0 $     write sys$output "    SYS$LIBRARY:VAXCDEF"8 $     write sys$output "ims_example demo build aborted."
 $     exit $ endif 	 $stdiook:  $ !   $ ! Build ims_example and run it $ !  $ / $ write sys$output "Compiling ims_example.C..." 6 $ define/user/nolog c$include decw$include,sys$library* $ define/user/nolog vaxc$include c$include $ if arch_type .eqs. "VAX" $ thenH $     cc /define=(_NO_PROTO,XK_LATIN1,VMS,VMSV5,"vms",__STARLET_LOADED,-C __LIBS$ROUTINES_LOADED,X_LOCALE,XLIB_XPG4_FUNCS,_WCHAR_T_,X_WCHAR,- 6 I18N_BUG_FIX,I18N_MULTIBYTE) /INCLUDE=(DECW$EXAMPLES,- sys$share,decw$include)- /OPT decw$examples:ims_example $ elseF $     cc /standard=vaxc /define=(_NO_PROTO,XK_LATIN1,VMS,VMSV5,"vms",-B __STARLET_LOADED,__LIBS$ROUTINES_LOADED,X_LOCALE,XLIB_XPG4_FUNCS,-H _WCHAR_T_,X_WCHAR,I18N_BUG_FIX,I18N_MULTIBYTE) /INCLUDE=(DECW$EXAMPLES,- sys$share,decw$include)- /OPT decw$examples:ims_example $ endif + $ write sys$output "Linking ims_example..."  $ if arch_type .eqs. "VAX" $ then% $     link ims_example,sys$input/opt  ! sys$share:decw$dxmlibshr12/share   sys$share:decw$xlibshr/share    sys$share:decw$xtlibshrr5/share   sys$share:decw$xmlibshr12/share ! sys$share:decw$mrmlibshr12/share   sys$share:vaxcrtl/share  sys$share:decw$dimslibshr/share  $  $ else$ $     link ims_example,sys$input/opt! sys$share:decw$dxmlibshr12/share   sys$share:decw$xlibshr/share    sys$share:decw$xtlibshrr5/share   sys$share:decw$xmlibshr12/share   sys$share:decw$mrmlibshr12/share  sys$share:decw$dimslibshr/share  $  $ endif  $ + $ write sys$output "Running ims_example..."  $ run ims_example  $  $ exit                                                                                                                                                                                                                                                                                                                                                                                                                                                                          �               * [F]IMS_EXAMPLE.EXE;1 +  , ��   .     /     4        	                     - zp   0  � 1    2   3      K � P   W   O 	    5   6 ��d��֟  7 �Þ��֟  8          9          G    H �� J �                          � 0 D X     0205  ��������    (  �ɁE       �                                      IMS_EXAMPLE                            V1.0           ���Ɂŗ 05-05               �           �           
       ��? �  �)              DECW$DXMLIBSHR12_001.      !       DECW$TRANSPORT_COMMON_001 �     !        
LIBRTL_001 O    !      � �
MTHRTL_001      !      	  
LBRSHR_001(              DECW$XMLIBSHR12_001)              DECW$MRMLIBSHR12_001�����������������������(              DECW$XTLIBSHRR5_001% �    !        dDECW$XLIBSHR_001       !        VAXCRTL_001(      +        dDECW$DIMSLIBSHR_001 B     !      d  
FORRTL_001 �     !      f  
SMGSHR_001  �����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������exam example zh_TW locale serverName supportStyles createICCb destroyICCb processEventCb getFilterEventsCb zh_tw_exam ja_JP locale ja_jp_exam zh_CN locale zh_cn_exam ko_KR locale ko_kr_exam   ����������������������������������������������������                                                                                                                                                                                                                                                                                  �^���F  �P�PԠ�P��P  �^ݬ���  �P	�P��  �P �^ЬR���  �Pb���  � �^��U���VЬR�R��O  �PSТRPРW��Ѽ1� ЬP��4R� �R~�W���  �PU�U�A   	�U�Z   �U�a   1� �U�z   1� գ1� �U�a   ������UP�PT�P������UP�PT�TcT�TR�RR�Bf��B��攭�ޭ�������߭��W��l  ݭ���r  �P�Pݭ�ݬ��y  ݭ���  ��ԣ���U�a   ������UP�Pc�P������UP�P���| ����^�  ��e���R�b��<���|~� ���  |~� ݬ߬|~ߢ߭��	��  �PVޢ�洭�ԭ���ޭ�����ޭ���UxUP�@���Qޢa�@���P���`�UxUP�@���Qޢa�@���P���`�UxUP�@���Qޢ%a�@���P���`�UxUP�@���Qޢ3a�@���P�ϲ�`�UxUP�@���Qޢ>a�@���P�Ϸ�`�UxUP�@���QޢJa�@���P����`�UxUP�@���QޢYa�@���P�ϡ�`�U�U����ߢk� ��  T�d�PSޢv��ޢ|����������U����� �S�d� ��� ����������U����� �S�d�¦ ���¬ ����������U�����³ �S�d�S�V���  �V��  ݭ��V��~  ݭ����   �P                                                                                                                                                                                        @   @       �   �                                            0  �  H  (  (       �  �  ,  �  �  �  �  �  �  (  �  �     	   x  ��     
   
   �        ؛ �� �� (� h�                             @                                                                       DECW$DXMLIBSHR12                                               DECW$TRANSPORT_COMMON                                          LIBRTL                                                         MTHRTL                                                         LBRSHR                                                         DECW$XMLIBSHR12                                                DECW$MRMLIBSHR12                                               DECW$XTLIBSHRR5                                                DECW$XLIBSHR                                                   VAXCRTL                                                        DECW$DIMSLIBSHR                                                FORRTL                                                         SMGSHR                                                                                                                                                                                                                                                                                                                                                                                                                                                                     �               * [F]PERIODIC.C;1 +  , b�   . 6    /     4 `   6   % (                    - zp   0  � 1    2   3      K � P   W   O &    5   6  a���֟  7 �С��֟  8          9          G    H �� J �   F            /* 
 * (c) Copyright 1989, 1990, 1991, 1992 OPEN SOFTWARE FOUNDATION, INC. 
 * ALL RIGHTS RESERVED 
*/ 
/* 
 * Motif Release 1.2.1
*/ 
#ifdef REV_INFO
#ifndef lint
static char rcsid[] = "$RCSfile: periodic.c,v $ $Revision: 1.8.4.2 $ $Date: 92/08/08 17:08:27 $"
#endif
#endif

/******************************************************************************
 * periodic.c
 *
 * Copy and rename the file periodic.dat to Periodic in your home directory
 * or app-defaults directory, or merge it with your decw$xdefaults.dat file.
 *
 * It provides useful default values for Periodic fonts and colors
 *
 *****************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/Scale.h>
#include <Xm/ToggleB.h>
#include <Mrm/MrmPublic.h>

typedef struct _DrawData {
	GC gc;
	Position drawX;
	Position drawY;
	Dimension drawWidth;
	Dimension drawHeight;
} DrawData;

#ifdef _NO_PROTO

static GC GetGC();
static void ConfigureDrawData();
static void DrawButton();
static void DrawArea();
static void PopupHandler();

static void ManageCb();
static void UnmapCb();
static void UnmanageCb();
static void InitPopupCb();
static void PopdownCb();
static void DaExposeCb();
static void DaResizeCb();
static void DbExposeCb();
static void DbResizeCb();
static void ScaleCb();
static void SetScaleCb();
static void ViewCb();
static void LayoutCb();
static void ToggleLightsCb();
static void ShowCb();
static void ExitCb();

#else

static GC GetGC(
	Widget w );
static void ConfigureDrawData(
	Widget w,
	DrawData *data );
static void DrawButton(
	Widget w );
static void DrawArea(
	Widget w );
static void PopupHandler(
        Widget w,
        Widget pw,
        XEvent *event,
        Boolean *ctd );

static void ManageCb(
        Widget w,
        String id,
        XmToggleButtonCallbackStruct *cb );
static void UnmapCb(
        Widget w,
        XtPointer cd,
        XtPointer cb );
static void UnmanageCb(
        Widget w,
        String id,
        XtPointer cb );
static void InitPopupCb(
        Widget w,
        String id,
        XtPointer cb );
static void PopdownCb(
        Widget w,
        XtPointer cd,
        XtPointer cb );
static void DaExposeCb(
        Widget w,
        XtPointer cd,
        XtPointer cb );
static void DaResizeCb(
        Widget w,
        XtPointer cd,
        XtPointer cb );
static void DbExposeCb(
        Widget w,
        XtPointer cd,
        XtPointer cb );
static void DbResizeCb(
        Widget w,
        XtPointer cd,
        XtPointer cb );
static void ScaleCb(
        Widget w,
        XtPointer cd,
        XtPointer cb );
static void SetScaleCb(
        Widget w,
        int *value,
        XmToggleButtonCallbackStruct *cb );
static void ViewCb(
        Widget w,
        XtPointer cd,
        XmToggleButtonCallbackStruct *cb );
static void LayoutCb(
        Widget w,
        XtPointer cd,
        XmToggleButtonCallbackStruct *cb );
static void ToggleLightsCb(
        Widget w,
        XtPointer cd,
        XmToggleButtonCallbackStruct *cb );
static void ShowCb(
        Widget w,
        String id,
        XtPointer cb );
static void ExitCb(
        Widget w,
        XtPointer cd,
        XtPointer cb );
#endif

static MrmHierarchy mrm_id;
static char *mrm_vec[]={"periodic.uid"};
static MrmCode mrm_class;
static MRMRegisterArg mrm_names[] = {
#ifdef DEC_MOTIF_BUG_FIX
        {"InitPopupCb", (XtPointer)InitPopupCb },
        {"PopdownCb", (XtPointer)PopdownCb },
        {"UnmapCb", (XtPointer)UnmapCb },
        {"UnmanageCb", (XtPointer)UnmanageCb },
        {"ManageCb", (XtPointer)ManageCb },
        {"DaExposeCb", (XtPointer)DaExposeCb },
        {"DaResizeCb", (XtPointer)DaResizeCb },
        {"DbExposeCb", (XtPointer)DbExposeCb },
        {"DbResizeCb", (XtPointer)DbResizeCb },
        {"ScaleCb", (XtPointer)ScaleCb },
        {"SetScaleCb", (XtPointer)SetScaleCb },
        {"ViewCb", (XtPointer)ViewCb },
        {"LayoutCb", (XtPointer)LayoutCb },
        {"ToggleLightsCb", (XtPointer)ToggleLightsCb },
        {"ShowCb", (XtPointer)ShowCb },
        {"ExitCb", (XtPointer)ExitCb }
#else
        {"InitPopupCb", (caddr_t)InitPopupCb },
        {"PopdownCb", (caddr_t)PopdownCb },
        {"UnmapCb", (caddr_t)UnmapCb },
        {"UnmanageCb", (caddr_t)UnmanageCb },
        {"ManageCb", (caddr_t)ManageCb },
        {"DaExposeCb", (caddr_t)DaExposeCb },
        {"DaResizeCb", (caddr_t)DaResizeCb },
        {"DbExposeCb", (caddr_t)DbExposeCb },
        {"DbResizeCb", (caddr_t)DbResizeCb },
        {"ScaleCb", (caddr_t)ScaleCb },
        {"SetScaleCb", (caddr_t)SetScaleCb },
        {"ViewCb", (caddr_t)ViewCb },
        {"LayoutCb", (caddr_t)LayoutCb },
        {"ToggleLightsCb", (caddr_t)ToggleLightsCb },
        {"ShowCb", (caddr_t)ShowCb },
        {"ExitCb", (caddr_t)ExitCb }
#endif
};

static String fallbackResources[] = {
"periodic*XmText.columns: 10",
"periodic*XmTextField.columns: 10",
"periodic*scaleFrame*XmScale.width: 50",
"periodic*scrollFrame*XmScrollBar.width: 50",

"periodic*fontList: *-*-*-medium-r-*-*-*-100-*-*-p-*-*-*",

"periodic*titleLabel.fontList: *-*-*-bold-r-*-*-*-180-*-*-p-*-*-*",
"periodic*subtitleLabel.fontList: *-*-*-bold-r-*-*-*-140-*-*-p-*-*-*",
"periodic*labelLabel.fontList: *-*-*-bold-r-*-*-*-180-*-*-p-*-*-*",
"periodic*menuBar*fontList: *-*-*-medium-r-*-*-*-120-*-*-p-*-*-*",
"periodic*popupMenu*fontList: *-*-*-medium-r-*-*-*-120-*-*-p-*-*-*",
"periodic*XmMessageBox*fontList: *-*-*-medium-r-*-*-*-120-*-*-p-*-*-*",
"periodic*fileDialog*fontList: *-*-*-medium-r-*-*-*-120-*-*-p-*-*-*",
"periodic*selectDialog*fontList: *-*-*-medium-r-*-*-*-120-*-*-p-*-*-*",
"periodic*promptDialog*fontList: *-*-*-medium-r-*-*-*-120-*-*-p-*-*-*",
NULL
};

static XtAppContext  appContext;
static Widget shell;

int
#ifdef _NO_PROTO
main(argc, argv)
    int argc;
    char *argv[];
#else
main(
    int argc,
    char *argv[] )
#endif
{
    Widget appMain;
    Display *display;
    Arg args[2];

    XtSetLanguageProc(NULL, NULL, NULL);

    XtToolkitInitialize();
    MrmInitialize ();
    appContext = XtCreateApplicationContext();
    XtAppSetFallbackResources (appContext, fallbackResources);
    display = XtOpenDisplay(appContext, NULL, "periodic", "Periodic",
			NULL, 0, &argc, argv);
    if (display == NULL) {
	fprintf(stderr, "%s:  Can't open display\n", argv[0]);
	exit(1);
    }

    shell = XtAppCreateShell(argv[0], "periodic", applicationShellWidgetClass,
			  display, NULL, 0);
    if (MrmOpenHierarchy (1, mrm_vec, NULL, &mrm_id) != MrmSUCCESS) exit(0);
    MrmRegisterNames(mrm_names, XtNumber(mrm_names));
    MrmFetchWidget (mrm_id, "appMain", shell, &appMain, &mrm_class);
    XtManageChild(appMain);
    XtRealizeWidget(shell);
    XtAppMainLoop(appContext);
}

static void
#ifdef _NO_PROTO
ExitCb(w, cd, cb)
    Widget w;
    XtPointer cd;
    XtPointer cb;
#else
ExitCb(
    Widget w,
    XtPointer cd,
    XtPointer cb )
#endif
{
    exit(0);
}


/*****************************************************************
 *
 * Display selected Dialog widget
 *
 *****************************************************************/

static Widget managedToggle = NULL;

static void
#ifdef _NO_PROTO
ManageCb(w, id, cb)
    Widget w;
    String id;
    XmToggleButtonCallbackStruct *cb;
#else
ManageCb(
    Widget w,
    String id,
    XmToggleButtonCallbackStruct *cb )
#endif
{
    Widget dialog = XtNameToWidget (shell, id);

    if (cb->set) {
	if (managedToggle != NULL)
	    XmToggleButtonSetState (managedToggle, False, True);
	managedToggle = w;
	XtManageChild (dialog);
    }
    else {
	XtUnmanageChild (dialog);
	managedToggle = NULL;
    }
}

static void
#ifdef _NO_PROTO
UnmapCb(w, cd, cb)
    Widget w;
    XtPointer cd;
    XtPointer cb;
#else
UnmapCb(
    Widget w,
    XtPointer cd,
    XtPointer cb )
#endif
{
    XmToggleButtonSetState (managedToggle, False, True);
}

static void
#ifdef _NO_PROTO
UnmanageCb(w, id, cb)
    Widget w;
    String id;
    XtPointer cb;
#else
UnmanageCb(
    Widget w,
    String id,
    XtPointer cb )
#endif
{
    XtUnmanageChild (XtNameToWidget (shell, id));
}

static void
#ifdef _NO_PROTO
ShowCb(w, id, cb)
    Widget w;
    String id;
    XtPointer cb;
#else
ShowCb(
    Widget w,
    String id,
    XtPointer cb )
#endif
{
    static Widget tb = NULL;
    static Widget sc = NULL;
    int value;

    if (tb == NULL) tb = XtNameToWidget (shell, "*toggleButton");
    if (sc == NULL) sc = XtNameToWidget (shell, "*valueScale");

    XmScaleGetValue (sc, &value);
    if (XmToggleButtonGetState(tb) == True && value == 1012)
	XtManageChild (XtNameToWidget (shell, id));
}


/*****************************************************************
 *
 * Provide RadioBox behavior inside a PulldownMenu
 *
 *****************************************************************/

static void
#ifdef _NO_PROTO
ViewCb(w, cd, cb)
    Widget w;
    XtPointer cd;
    XmToggleButtonCallbackStruct *cb;
#else
ViewCb(
    Widget w,
    XtPointer cd,
    XmToggleButtonCallbackStruct *cb )
#endif
{
    static Widget viewToggle = NULL;

    if (cb->set) {
	if (viewToggle) XmToggleButtonSetState (viewToggle, False, False);
	viewToggle = w;
    }
    else {
	if (w == viewToggle) XmToggleButtonSetState (w, True, False);
    }
}

static void
#ifdef _NO_PROTO
LayoutCb(w, cd, cb)
    Widget w;
    XtPointer cd;
    XmToggleButtonCallbackStruct *cb;
#else
LayoutCb(
    Widget w,
    XtPointer cd,
    XmToggleButtonCallbackStruct *cb )
#endif
{
    static Widget layoutToggle = NULL;

    if (cb->set) {
	if (layoutToggle) XmToggleButtonSetState (layoutToggle, False, False);
	layoutToggle = w;
    }
    else {
	if (w == layoutToggle) XmToggleButtonSetState (w, True, False);
    }
}


/*****************************************************************
 *
 * PopupMenu support
 *
 *****************************************************************/

static Time popupLastEventTime = 0;

static void
#ifdef _NO_PROTO
InitPopupCb(w, id, cb)
    Widget w;
    String id;
    XtPointer cb;
#else
InitPopupCb(
    Widget w,
    String id,
    XtPointer cb )
#endif
{
    Widget popupWindow = XtNameToWidget (shell, id);

    XtAddEventHandler (popupWindow, ButtonPressMask, False,
			(XtEventHandler)PopupHandler, (XtPointer)w);
}

static void
#ifdef _NO_PROTO
PopupHandler (w, pw, event, ctd)
    Widget w;
    Widget pw;
    XEvent *event;
    Boolean *ctd;
#else
PopupHandler (
    Widget w,
    Widget pw,
    XEvent *event,
    Boolean *ctd )
#endif
{
    if (((XButtonEvent *)event)->button != Button3) return;
    if (((XButtonEvent *)event)->time <= popupLastEventTime) return;

    XmMenuPosition((Widget) pw, (XButtonEvent *)event);
    XtManageChild ((Widget) pw);
}

/* By default, cancelling a popup menu with Button 3 will cause the
 * popup to be reposted at the location of the cancelling click.
 *
 * To switch off this behavior, remember when the menu was popped down.
 * In PopupHandler, don't repost the menu if the posting click just
 * cancelled a popup menu.
 */
static void
#ifdef _NO_PROTO
PopdownCb(w, cd, cb)
    Widget w;
    XtPointer cd;
    XtPointer cb;
#else
PopdownCb(
    Widget w,
    XtPointer cd,
    XtPointer cb )
#endif
{
    popupLastEventTime = XtLastTimestampProcessed (XtDisplay(w));
}


/*****************************************************************
 *
 * Draw utilities
 *
 *****************************************************************/

static DrawData *drawData = NULL;
static DrawData *buttonData = NULL;

static GC
#ifdef _NO_PROTO
GetGC(w)
    Widget w;
#else
GetGC(
    Widget w )
#endif
{
    Arg args[2];
    XGCValues gcv;
    Pixel fg;
    Pixel bg;
    GC gc;

    XtSetArg (args[0], XmNforeground, &fg);
    XtSetArg (args[1], XmNbackground, &bg);
    XtGetValues (w, args, 2);
    gcv.foreground = fg;
    gcv.background = bg;
    gcv.line_width = 1;
    gc = XtGetGC (w, GCForeground | GCBackground | GCLineWidth, &gcv);

    return (gc);
}

static void
#ifdef _NO_PROTO
ConfigureDrawData(w, data)
    Widget w;
    DrawData *data;
#else
ConfigureDrawData(
    Widget w,
    DrawData *data )
#endif
{
    Arg args[6];
    Dimension width, height, st, ht, mw, mh;
    Dimension totalMarginWidth;
    Dimension totalMarginHeight;

    width = height = st = ht = mw = mh = 0;
    XtSetArg (args[0], XmNwidth, &width);
    XtSetArg (args[1], XmNheight, &height);
    XtSetArg (args[2], XmNshadowThickness, &st);
    XtSetArg (args[3], XmNhighlightThickness, &ht);
    XtSetArg (args[4], XmNmarginWidth, &mw);
    XtSetArg (args[5], XmNmarginHeight, &mh);
    XtGetValues (w, args, 6);

    totalMarginWidth = st + ht + mw;
    totalMarginHeight = st + ht + mh;

    if (2 * totalMarginWidth < width && 2 * totalMarginHeight < height) {
	data->drawX = totalMarginWidth;
	data->drawY = totalMarginHeight;
	data->drawWidth = width - 2 * totalMarginWidth;
	data->drawHeight = height - 2 * totalMarginHeight;
    }
    else {
	data->drawWidth = 0;
	data->drawHeight = 0;
    }
}

/*****************************************************************
 *
 * DrawingArea display code
 *
 *****************************************************************/

static void
#ifdef _NO_PROTO
DaResizeCb(w, cd, cb)
    Widget w;
    XtPointer cd;
    XtPointer cb;
#else
DaResizeCb(
    Widget w,
    XtPointer cd,
    XtPointer cb )
#endif
{
    if (drawData == NULL) return;

    ConfigureDrawData (w, drawData);
    XClearArea (XtDisplay(w), XtWindow(w),
		drawData->drawX, drawData->drawY,
		drawData->drawWidth, drawData->drawHeight,
		False);
    DrawArea (w);
}

static void
#ifdef _NO_PROTO
DaExposeCb(w, cd, cb)
    Widget w;
    XtPointer cd;
    XtPointer cb;
#else
DaExposeCb(
    Widget w,
    XtPointer cd,
    XtPointer cb )
#endif
{
    if (drawData == NULL) {
	drawData = (DrawData *)XtMalloc (sizeof(DrawData));
	drawData->gc = GetGC (w);
	ConfigureDrawData (w, drawData);
    }
    DrawArea(w);
}

#define NPOINTS 40

static void
#ifdef _NO_PROTO
DrawArea(w)
    Widget w;
#else
DrawArea(
    Widget w )
#endif
{
    int i, x, y, m;
    XPoint p[NPOINTS];

    if (drawData->drawWidth == 0) return;

    XClearArea (XtDisplay(w), XtWindow(w),
		drawData->drawX, drawData->drawY,
		drawData->drawWidth, drawData->drawHeight,
		False);
    XDrawRectangle (XtDisplay(w), XtWindow(w), drawData->gc,
		drawData->drawX, drawData->drawY,
		drawData->drawWidth, drawData->drawHeight);
    XDrawLine (XtDisplay(w), XtWindow(w), drawData->gc,
		drawData->drawX, drawData->drawY + drawData->drawHeight/2,
		drawData->drawX + drawData->drawWidth,
		drawData->drawY + drawData->drawHeight/2);

    m = 20 * drawData->drawHeight / 100;
    p[0].x = drawData->drawX;
    p[0].y = drawData->drawY + drawData->drawHeight/2;
    for (i = 1; i < NPOINTS-1; i++) {
	p[i].x = drawData->drawX + (i * drawData->drawWidth)/NPOINTS;
	p[i].y = drawData->drawY + m/2 + (rand() % (drawData->drawHeight - m));
    }
    p[NPOINTS-1].x = drawData->drawX + drawData->drawWidth;
    p[NPOINTS-1].y = drawData->drawY + drawData->drawHeight/2;

    XDrawLines (XtDisplay(w), XtWindow(w), drawData->gc,
		p, NPOINTS, CoordModeOrigin);
}

static void
#ifdef _NO_PROTO
ScaleCb(w, cd, cb)
    Widget w;
    XtPointer cd;
    XtPointer cb;
#else
ScaleCb(
    Widget w,
    XtPointer cd,
    XtPointer cb )
#endif
{
    static Widget da = NULL;

    if (drawData == NULL) return;

    if (da == NULL) da = XtNameToWidget (shell, "*drawArea");

    DrawArea (da);
}

static void
#ifdef _NO_PROTO
SetScaleCb(w, value, cb)
    Widget w;
    int *value;
    XmToggleButtonCallbackStruct *cb;
#else
SetScaleCb(
    Widget w,
    int *value,
    XmToggleButtonCallbackStruct *cb )
#endif
{
    static Widget da = NULL;
    static Widget sc = NULL;

    if (drawData == NULL) return;

    if (da == NULL) da = XtNameToWidget (shell, "*drawArea");
    if (sc == NULL) sc = XtNameToWidget (shell, "*valueScale");

    XmScaleSetValue (sc, *value);

    DrawArea (da);
}

/*****************************************************************
 *
 * DrawnButton display code
 *
 *****************************************************************/

static Boolean lightsOn = False;

static void
#ifdef _NO_PROTO
DbResizeCb(w, cd, cb)
    Widget w;
    XtPointer cd;
    XtPointer cb;
#else
DbResizeCb(
    Widget w,
    XtPointer cd,
    XtPointer cb )
#endif
{
    if (buttonData == NULL) return;

    ConfigureDrawData (w, buttonData);
    XClearArea (XtDisplay(w), XtWindow(w),
		buttonData->drawX, buttonData->drawY,
		buttonData->drawWidth, buttonData->drawHeight,
		False);
    DrawButton (w);
}

static void
#ifdef _NO_PROTO
DbExposeCb(w, cd, cb)
    Widget w;
    XtPointer cd;
    XtPointer cb;
#else
DbExposeCb(
    Widget w,
    XtPointer cd,
    XtPointer cb )
#endif
{
    if (buttonData == NULL) {
	buttonData = (DrawData *)XtMalloc (sizeof(DrawData));
	buttonData->gc = GetGC (w);
	ConfigureDrawData (w, buttonData);
    }
    DrawButton(w);
}

#define NARCS 6

static void
#ifdef _NO_PROTO
DrawButton(w)
    Widget w;
#else
DrawButton(
    Widget w )
#endif
{
    int i, x, y, incX, incY;
    XArc a[NARCS];

    if (buttonData->drawWidth == 0 || !lightsOn) return;

    a[0].x = buttonData->drawX + (buttonData->drawWidth - 1)/2;
    a[0].y = buttonData->drawY + (buttonData->drawHeight - 1)/2;
    a[0].width = 1;
    a[0].height = 1;
    a[0].angle1 = 0;
    a[0].angle2 = 360*64;
    incX = (buttonData->drawWidth - 1)/(2 * NARCS);
    incY = (buttonData->drawHeight - 1)/(2 * NARCS);

    for (i = 1; i < NARCS; i++) {
	a[i].x = a[i-1].x - incX;
	a[i].y = a[i-1].y - incY;
	a[i].width = a[i-1].width + 2 * incX;
	a[i].height = a[i-1].height + 2 * incY;
#ifndef BROKEN_SERVER_ARCS
	a[i].angle1 = 0;
	a[i].angle2 = 360 * 64;
#else
	XDrawRectangle (XtDisplay(w), XtWindow(w), buttonData->gc,
			a[i].x, a[i].y, a[i].width, a[i].height);
#endif
    }

#ifndef BROKEN_SERVER_ARCS
    XDrawArcs (XtDisplay(w), XtWindow(w), buttonData->gc, a, NARCS);
#endif
}

static void
#ifdef _NO�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          �wX ~      DWMV_JA_JPU6012.F              b�  zp [F]PERIODIC.C;1                                                                                                                `     6                         VC      $       _PROTO
ToggleLightsCb(w, cd, cb)
    Widget w;
    XtPointer cd;
    XmToggleButtonCallbackStruct *cb;
#else
ToggleLightsCb(
    Widget w,
    XtPointer cd,
    XmToggleButtonCallbackStruct *cb )
#endif
{
    static Widget db = NULL;

    if (buttonData == NULL) return;

    if (db == NULL) db = XtNameToWidget (shell, "*drawnButton");

    lightsOn = cb->set;

    if (lightsOn)
	DrawButton (db);
    else
	XClearArea (XtDisplay(db), XtWindow(db),
		buttonData->drawX, buttonData->drawY,
		buttonData->drawWidth, buttonData->drawHeight,
		False);
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        �               * [F]PERIODIC.COM_US_RENAME;1 +  , ~�   .     /     4 W      	 H                    - zp   0  � 1    2   3      K � P   W   O 
    5   6 ����֟  7 �W���֟  8          9          G    H �� J �                   ! $ ! Build the Motif periodic Demo E $ ! The files periodic.uid and periodic.exe will be produced by this   $ ! command procedure. $ ! K $ ! Make sure that the prerequisite files and logicals exist.  This command L $ ! procedure requires that the DECwindows programming and user environmentsM $ ! have been installed and that the DECW$STARTUP command procedure has been   $ ! invoked.   $ ! " $ arch_type = f$getsyi("hw_model") $ if arch_type .le. 1023 $ then $    arch_type = "VAX" $ else $    arch_type = "AXP" $ endif J $ if f$search("sys$system:decw$uilmotif.exe") .nes. "" then goto libskitokF $ write sys$output "DECwindows programming environment not installed."1 $ write sys$output "periodic demo build aborted."  $ exit $libskitok: < $ if f$search("sys$share:decw$xlibshr.exe") .nes. ""	.and. -J      f$search("sys$share:decw$dxmlibshr.exe") .nes. ""	then goto appskitok? $ write sys$output "DECwindows user environment not installed." 1 $ write sys$output "periodic demo build aborted."  $ exit $appskitok: . $ if f$trnlnm("decw$include") .nes. ""	.and. -;      f$trnlnm("decw$examples") .nes. ""	then goto startupok O $ write sys$output "You must invoke @DECW$STARTUP before using this procedure." 1 $ write sys$output "periodic demo build aborted."  $ exit $startupok:  $ if arch_type .eqs. "VAX" $ thenF $     if f$search("sys$system:vaxc.exe") .nes. "" then goto compilerok8 $     write sys$output "VAXC compiler is not available."5 $     write sys$output "periodic demo build aborted." 
 $     exit $ elseO $     if f$search("sys$system:decc$compiler.exe") .nes. "" then goto compilerok 9 $     write sys$output "DEC C compiler is not available." 5 $     write sys$output "periodic demo build aborted." 
 $     exit $ endif  $compilerok: $ if arch_type .eqs. "VAX" $ thenC $     if f$search("sys$library:stdio.h") .nes. "" then goto stdiook > $     write sys$output "SYS$LIBRARY:STDIO.H is not available."4 $     write sys$output "Create it with the command:"W $     write sys$output " $ LIBRARY/TEXT/EXTRACT=STDIO/OUT=SYS$COMMON:[SYSLIB]STDIO.H -" 0 $     write sys$output "    SYS$LIBRARY:VAXCDEF"5 $     write sys$output "periodic demo build aborted." 
 $     exit $ elseK $     if f$search("sys$library:decc$rtldef.tlb") .nes. "" then goto stdiook > $     write sys$output "SYS$LIBRARY:STDIO.H is not available."4 $     write sys$output "Create it with the command:"W $     write sys$output " $ LIBRARY/TEXT/EXTRACT=STDIO/OUT=SYS$COMMON:[SYSLIB]STDIO.H -" 0 $     write sys$output "    SYS$LIBRARY:VAXCDEF"5 $     write sys$output "periodic demo build aborted." 
 $     exit $ endif 	 $stdiook:  $ !  $ ! Build periodic and run it  $ ! . $ write sys$output "Compiling periodic.UIL...", $ define/user/nolog uil$include decw$include& $ uil/motif decw$examples:periodic.uil $ , $ write sys$output "Compiling periodic.C..."6 $ define/user/nolog c$include decw$include,sys$library* $ define/user/nolog vaxc$include c$include $ if arch_type .eqs. "VAX" $ thenH $     cc /define=(_NO_PROTO,XK_LATIN1,VMS,VMSV5,"vms",__STARLET_LOADED,-C __LIBS$ROUTINES_LOADED,X_LOCALE,XLIB_XPG4_FUNCS,_WCHAR_T_,X_WCHAR,- 6 I18N_BUG_FIX,I18N_MULTIBYTE) /INCLUDE=(DECW$EXAMPLES,- sys$share,decw$include)- /OPT decw$examples:periodic  $ elseF $     cc /standard=vaxc /define=(_NO_PROTO,XK_LATIN1,VMS,VMSV5,"vms",-B __STARLET_LOADED,__LIBS$ROUTINES_LOADED,X_LOCALE,XLIB_XPG4_FUNCS,-H _WCHAR_T_,X_WCHAR,I18N_BUG_FIX,I18N_MULTIBYTE) /INCLUDE=(DECW$EXAMPLES,- sys$share,decw$include)- /OPT decw$examples:periodic  $ endif ( $ write sys$output "Linking periodic..." $ if arch_type .eqs. "VAX" $ then" $     link periodic,sys$input/opt ! sys$share:decw$dxmlibshr12/share   sys$share:decw$xlibshr/share    sys$share:decw$xtlibshrr5/share   sys$share:decw$xmlibshr12/share ! sys$share:decw$mrmlibshr12/share   sys$share:vaxcrtl/share  $  $ else! $     link periodic,sys$input/opt ! sys$share:decw$dxmlibshr12/share   sys$share:decw$xlibshr/share    sys$share:decw$xtlibshrr5/share   sys$share:decw$xmlibshr12/share ! sys$share:decw$mrmlibshr12/share   $  $ endif  $ ( $ write sys$output "Running periodic..." $ run periodic $  $ exit                                                                                                                                                                                                                                                                                                                                                                                                                                                        �               * [F]PERIODIC.DAT;1 +  , c�   .     /     4 L       h   �                - zp   0  � 1    2   3      K � P   W   O     5   6 �����֟  7 �d���֟  8          9          G    H �� J �             periodic*XmText.columns: 10
periodic*XmTextField.columns: 10
periodic*scaleFrame*XmScale.width: 50
periodic*scrollFrame*XmScrollBar.width: 50

!periodic*fontList: *-*-Helvetica-medium-r-*-*-*-100-*-*-p-*-*-*

periodic*titleLabel.fontList: *-*-Helvetica-bold-r-*-*-*-180-*-*-p-*-*-*
periodic*subtitleLabel.fontList: *-*-Helvetica-bold-r-*-*-*-140-*-*-p-*-*-*
periodic*labelLabel.fontList: *-*-Helvetica-bold-r-*-*-*-180-*-*-p-*-*-*
periodic*menuBar*fontList: *-*-Helvetica-medium-r-*-*-*-120-*-*-p-*-*-*
periodic*popupMenu*fontList: *-*-Helvetica-medium-r-*-*-*-120-*-*-p-*-*-*
periodic*XmMessageBox*fontList: *-*-Helvetica-medium-r-*-*-*-120-*-*-p-*-*-*
periodic*fileDialog*fontList: *-*-Helvetica-medium-r-*-*-*-120-*-*-p-*-*-*
periodic*selectDialog*fontList: *-*-Helvetica-medium-r-*-*-*-120-*-*-p-*-*-*
periodic*promptDialog*fontList: *-*-Helvetica-medium-r-*-*-*-120-*-*-p-*-*-*

!periodic*background: gray
!periodic*XmFrame.XmLabel.foreground: yellow
!periodic*drawArea.background: mediumaquamarine
!periodic*drawnButton.foreground: red
!periodic*highlightColor: red

periodic*listFrame*selectionPolicy: XmEXTENDED_SELECT

                                                                                                                                                                                                                                                                                                                                                                                                                        �               * [F]PERIODIC.EXE;1 +  , ��   .     /     4                             - zp   0  � 1    2   3      K � P   W   O     5   6 �E)��֟  7  r���֟  8          9          G    H �� J �             � 0 D X     0205  ��������    (  ᨁE                                              PERIODIC                               V1.0            Nᨁŗ 05-05               �           �        
   
       ��? �  �)              DECW$DXMLIBSHR12_001.      !       DECW$TRANSPORT_COMMON_001 �     !        
LIBRTL_001 O    !      � �
MTHRTL_001      !      	  
LBRSHR_001(              DECW$XMLIBSHR12_001)              DECW$MRMLIBSHR12_001�����������������������(              DECW$XTLIBSHRR5_001% �    !        dDECW$XLIBSHR_001       !        VAXCRTL_001  �����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������periodic.uid InitPopupCb PopdownCb UnmapCb UnmanageCb ManageCb DaExposeCb DaResizeCb DbExposeCb DbResizeCb ScaleCb SetScaleCb ViewCb LayoutCb ToggleLightsCb ShowCb ExitCb periodic*XmText.columns: 10 periodic*XmTextField.columns: 10 periodic*scaleFrame*XmScale.width: 50 periodic*scrollFrame*XmScrollBar.width: 50 periodic*fontList: *-*-*-medium-r-*-*-*-100-*-*-p-*-*-* periodic*titleLabel.fontList: *-*-*-bold-r-*-*-*-180-*-*-p-*-*-* periodic*subtitleLabel.fontList: *-*-*-bold-r-*-*-*-140-*-*-p-*-*-* periodic*labelLabel.fontList: *-*-*-bold-r-*-*-*-180-*-*-p-*-*-* periodic*menuBar*fontList: *-*-*-medium-r-*-*-*-120-*-*-p-*-*-* periodic*popupMenu*fontList: *-*-*-medium-r-*-*-*-120-*-*-p-*-*-* periodic*XmMessageBox*fontList: *-*-*-medium-r-*-*-*-120-*-*-p-*-*-* periodic*fileDialog*fontList: *-*-*-medium-r-*-*-*-120-*-*-p-*-*-* periodic*selectDialog*fontList: *-*-*-medium-r-*-*-*-120-*-*-p-*-*-* periodic*promptDialog*fontList: *-*-*-medium-r-*-*-*-120-*-*-p-*-*-* periodic Periodic %s:  Can't open display
 periodic appMain *toggleButton *valueScale *drawArea *drawArea *valueScale *drawnButton              �
    D  #  |	  +  �	  6   	  ?  �  J  �  U  �  `  h  k  �  s     ~  D
  �  �
  �  �  �  �	  �  	    �  �  �    9  q  �  �  7  w  �  �  A  �                                                                                                                                                                                                                                              < �^�A  ������T��8���S|~� ���  � ��  � �Z  � ��  �P�� �Ì ��� ���  ЬU�U߬|~������� ��� ��g  �PR�e������  ���  ����  |~�R��\  ���ݼ��"  �P�� �c� ߣ����  �P	� ���  �ߣ
���  ?�߭���� ����c���  ݭ����  ��� ���  ��� ���  �P  �^� ��6   �^��%���Rݬ��� ���  �PSЬPՠ&��� �� ��� ���
  Ь�� �S��:  �S��3  ���    �^������R�� ��� ���
   �^�����Rݬ��� ��
  �P���
   �^��9���S�����R��� ����� ���
  �P�� ��� ����� ���
  �P�� ߭���� ��?
  ��� ��8
  �P!ѭ���  ݬ��� ���
  �P��b
   �^�����RЬPՠ��� |~��� ���	  Ь�� Ѭ�� � �ݬ���	   �^�����RЬPՠ��� |~��� ���	  Ь�� Ѭ�� � �ݬ���	   �^��q���Rݬ��� ���	  �PQݬ߯� ��Q���	  �P �^��=���RЬPѠ4ЬPѠ�� ݬݬ��	  ݬ��a	   �^�����R�� ��e	  �P��P	  �P��     ���^���	  ��ޭ�����	  ��ޭ����߭�ݬ��E	  Э���Э������������   ��-	  � ���^|��ԭ����	  ��>���Ğ��	  ��>���̞��	  ��>���Ԟ��	  ��>���ܞ�z	  ��>����q	  ��>����߭�ݬ���  <��R<��P�PR<��P�RP�PW<��P�RP�PV<WS�SST<��P�TP4<VQ�QQU<��P�UP$ЬR�S��Q�<��P�TP�P�<��P�UP�P�
ЬRԢ �^�����R��� ��� ЬS�S��
�� ��� Q<�
~<�~2�~2�~�S���  �P�S���  �P��J  �S�χ �P ���^��T���R��� g����  �P�� ЬS��I  ��ޭ�����@  ��ޭ����߭��S���  Э���Э�����������S���  �P�� ��� ݬ��R�ݬ���P���\�^������V��� R<�R� ��� S<�
~<�~2�~2�~ЬX�X��  Z�j�P�X��  Y�i�P��d  ��� S<�
~<�~2�~2�~�c�X�j�P�X�i�P��H  ��� T2�W<�
R�Q�RP�R	�RP	�Q{RPQP�QW�W2�U�US<�R�RS~�W�U�d�X�j�P�X�i�P���  ��� S<�
R�R�Q�RPЏd   R
�RP
�Q{RPQP�QY���\���� S2�T<�
R�Q�RP�R�RP�Q�P{RPQP�TQR�R�^�2� W�YXxWS�C�\�U��� P2�T<�R�WR�Q�RP�(R�RP�Q�P{RPQP�TQP�Pe�C�^�U��� P2�T�XT� �C  �PS��� P<�
R�YR�Q�SP�R�R�RP�RQ�PQ{RPPQ�TQP�Pe�&Wj���� S2�T<�R�TR�R����� S2�T<�
R�Q�RP�R�RP�Q�P{RPQP�TQR�R��� �(��\���� ݬ��  �P��   ��
  �P��}   �^��5���S��~���R��� ��� ��!��� ���  �P�� ��� ��w��P �^������S��>���R��� ��� ��+��� ���  �P�� ��� ��5��� ��s  �P�� ݼ��� ���  ��� ����P �^������R��� ��� ЬS�S��2�� ��� Q<�
~<�~2�~2�~�S��  �P�S��  �P��r  �S�χ �P ���^��|���R��� g���   �P�� ЬS��q  ��ޭ�����h  ��ޭ����߭��S���  Э���Э�����������S���  �P�� ��� ݬ��z�ݬ���P����^������W��� R<�R��� ��� S2�U<�R�R�Q�RP�R�RP�Q�P{RPQP�UQR�R����� S2�U<�
R�R�Q�RP�R
�RP
�Q{RPQP�UQR�R������������� Z����� S<�R�R�Q�RP�R	�RP	�Q{RPQP�QY<�
R�R�Q�RP�R�RP�Q�P{RPQP�QX2� T�YY[�[V�XXZ�ZU�TS�C��Q�TR�R�B��P2`P�YP�Pa�C��Q�B��P2`P�XP�Pa�B��P<`P�VP�PQ�C��P�Q`�B��P<`P�UP�PQ�C��P�Q`�C��P�`�C��P�� Z`�T��߭���� ݬ��  �P�� ���  �P��]   �^�����S��f���R��� �� ��A��� ���  �P� ЬP���� �� ��+��P� ��� Q<�
~<�~2�~2�~�� ��~  �P�� ��m  �P���                                                                                                                                                                             @   @   P  �  �                                           �  H  @  �  �  $N  PE  �=  �8  �@  �O  �O  (N   P  l�        �  (   8      !      P  h    0  X  �  �  �    h  P  H  �  �  0     (   �  �  �  �  �     �  �  �     �  �  �  (  �  �     	   �  `  x  �  �     
   
   �  �  |       ,
  �    P!  �   �  �  ]�  /�                          @                                                                       DECW$DXMLIBSHR12                                               DECW$TRANSPORT_COMMON                                          LIBRTL                                                         MTHRTL                                                         LBRSHR                                                         DECW$XMLIBSHR12                                                DECW$MRMLIBSHR12                                               DECW$XTLIBSHRR5                                                DECW$XLIBSHR                                                   VAXCRTL                                      t  x  |  �        d  h  l  p     
   `                                                                                                                                                                                                                                                                                                                                                                                      �               * [F]PERIODIC.UID_US_RENAME;1 +  , f�   . l    /     4    l   i                    - zp   0  � 1    2   3      K � P   W   O i    5   6 ����֟  7 @���֟  8          9          G    H �� J �       @            ro|  URM 1.2   Motif Uil Compiler            V2.0-000  13-MAR-1994 19:01:29.06       periodic                      v1.2       i 5 !    � �             
  � ( 0 � D � �	 	 �	 � � d    �  | � D � ` � �� ��H      P   �X         templateMenuBar                 appMain                             ��H      h  ��Ih        ( T             appMain        $    <      �    menuBar                   `                T        D        8        (                        �         �         �   showDialog  promptDialog    workingDialog   questionDialog  errorDialog warningDialog   infoDialog  selectDialog    fileDialog  menuBar   ��H  
    l d ��Il    
     0 L             widget-0-112-2         "       !                   `   workArea    ��H  	     ���I    	     , @             workArea 112           (     !                   r    �       �       �       �       �       �       �       �       �       �       x  a    l  D    `  r    T       H  a    <  i    ,  drawBtnFrame nfodrawFrame lepopupFrame  paneFrame   listFrame   scrollFrame dialogFrame commandFrame    textFrame   radioFrame  scaleFrame  optionFrame labelFrame  toggleFrame pushFrame   arrowFrame  separatorFrame  subtitleLabel   titleLabel    ��H      � `��I�         ,               titleLabel 2              /              0    #   ;           p              ��H      : �߀6 	ISO8859-1#The Periodic Table of Motif Widgets��H      � (��I�         0               subtitleLabel f               /              0    #   ;       =    �       �               �    titleLabel     ��H      / t߀+ 	ISO8859-1Open Software Foundation   ��H  
    � 0��I�    
     0 �             separatorFrame  
              /       ;                      0       <               "       !        eL                    ��H      p t��Ip         0               widget-0-538-5         
       	                  `             ��H        D߀ 	ISO8859-1	Separator  ��H      0 ���I0                         widget-0-539-16   ��H  
    � ���I�    
     , �             arrowFrame 9
              /       ;                      0       <              "       !                             ��H      p @��Ip         0               widget-0-555-5         
       	                  `              ��H      " 	߀ 	ISO8859-1ArrowButton��H  	    � �	��I�    	     0 D             widget-0-566-2                	            �       �        �        |   arrowB4 arrowB3 arrowB2 arrowB1   ��H      � �	��I�         (               arrowB1 
              /       ;                      0       <                     4          ��H      � t
��I�         (               arrowB2 
              /       ;                     0       <                     4          ��H      � ��I�         (               arrowB3 
              /       ;                     0       <                       4          ��H      � ���I�         (               arrowB4 
              /       ;                     0       <                     4          ��H  
    � l��I�    
     , �             pushFrame   
              /       ;                     0    
   <              "       !        1           
      	   ��H      p ��Ip         0               widget-0-619-5         
       	                  `              ��H      ! �߀ 	ISO8859-1
PushButton ��H  	    D d��ID    	       0             widget-0-632-2                                    ro|    i �
�
 �
�
 `� d � t
� � �x l� @� �
�
 �l x �� �D 8� �� 8 ,	h D� |� �
� L� p�
 h  �� � � � @�
 �\	 �� ��
 �
 p� l� �x 4� 8	� �� @T �	�
 �x	 �   D �8 �� D� 4H �� |
 �  ( 
 ��
 �	, � l< � ��   0 ( �$ �
  ,
   ` 
l L	 �	� L xx ��
 8�
 x
  T ��	 ` l ��
 � � t 
 � � �
 (P �� t�
  �� �� � �D �� �P �` � ( ��   � �  �\ Lh	 �� p, (
4  � `<	 �� �` � (                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              >ClassTable >ResourceTable  cancelButton    spellButton draftButton includeButton   sendButton  templateFileCascade templateMenuBar versionButton   tutorButton windowButton    contextButton   helpCascade wrapButton  caseButton  rightDirButton  leftDirButton   downDirButton   upDirButton directCascade   largeFontButton mediumFontButton    smallFontButton fontCascade optionsCascade  yearButton  monthButton weekButton  dayButton   tileButton  stackButton viewCascade deleteButton    clearButton pasteButton copyButton  cutButton   undoButton  editCascade exitButton  printButton saveAsButton    saveButton  openButton  newButton   fileCascade menuBar showDialog  promptDialog    workingDialog   questionDialog  errorDialog warningDialog   infoDialog  selectDialog    textButton  graphicsButton  audioButton filterCascade   fileMenuBar fileDialog  drawnButton drawBtnFrame    drawArea    drawFrame   popupMenu   popupWindow popupFrame  paneFrame   listFrame   scrollFrame promptPick  selectPick  filePick    workingPick questionPick    errorPick   warningPick infoPick    templatePick    dialogForm  dialogFrame commandFrame    textFrame   radioFrame  valueScale  scaleFrame  optionFrame labelLabel  labelFrame  toggleButton    toggleFrame pushFrame   arrowB4 arrowB3 arrowB2 arrowB1 arrowFrame  separatorFrame  subtitleLabel   titleLabel  workArea    appMain periodic    ro|  + � ��H      �   ��I�         0               widget-0-630-6                /       ;       <    
              Z       �           !   ��H      ;   ߀7 	ISO8859-1Explode  	ISO8859-1Building   ��H  
    � � ��I�    
     , �             toggleFrame 
              /       ;                     0    
   <              "       !        1                    ��H      p � ��Ip         0               widget-0-648-5         
       	                  `              ��H      # �߀ 	ISO8859-1ToggleButton   ��H  	    T D��IT    	       0             widget-0-649-54             D   toggleButton     ��H      � |��I�         0               toggleButton 54               /       ;       <    
              Z       �   G   	 �   h�f !   �      1               ToggleLightsCb             ��H       �߀ 	ISO8859-1Lights ��H  
    � ���I�    
     , �             labelFrame n              /       ;                      0       <                     !      1        ��H      p ���Ip         0               widget-0-678-5         
       	                  `               ��H       �߀ 	ISO8859-1Label  ��H  	    P 0��IP    	       0             widget-0-679-45             D   labelLabel    ��H      x `��Ix         ,               labelLabel 9              /       ;                  h               ��H      6 �߀2 	ISO8859-1DON'T  	ISO8859-1PANIC��H  
    � P��I�    
     , �             optionFrame               /       ;                  #   0    (   <                     !      1        ��H      p ���Ip         0               widget-0-703-5         
       	                  `              ��H      ! P߀ 	ISO8859-1
OptionMenu ��H      � ���I�         0               widget-0-745-2         8    �       p   +       (    `                     �      !  ��H      � ��I�         0 D             widget-0-743-3         9                                              
      	         ��H      l ���Il         0               widget-0-717-7             \   (    L                     ��H       `	߀ 	ISO8859-1Sunday ��H       �	S ��H      l 
��Il         0               widget-0-721-7             \   (    L                     ��H       $
߀ 	ISO8859-1Monday ��H       �
M ��H      l �
��Il         0               widget-0-725-7             \   (    L                     ��H       �
߀ 	ISO8859-1Tuesday��H       hT ��H      l ���Il         0               widget-0-729-7             \   (    L                     ��H        �߀ 	ISO8859-1	Wednesday  ��H       ,W ��H      l `��Il         0               widget-0-733-7             \   (    L                     ��H       t߀ 	ISO8859-1Thursday   ��H       �h ��H      l (��Il         0               widget-0-737-7             \   (    L                     ��H       <߀ 	ISO8859-1Friday ��H       �F ��H      l ���Il         0               widget-0-741-7             \   (    L                     ��H        ߀ 	ISO8859-1Saturday   ��H       �a ��H       �߀ 	ISO8859-1Day��H       �D ��H  
    � ���I�    
     , �             scaleFrame 1
              /       ;                     0    #   <              "       !                    g                                    ro|   0 P  P � � � �   < t � �
 $
 `	 
 �	 �
 �
 � h ` , ( � � � � � �     �  L � 0 �   P t   � � p � @ 8
 �	 
 �
 � L  � P  � � , � � l  � � P �	 �	 d 4  P � ` | � D    � � X � � ` � � � x d  D  h � ` � �  �  �
 
 �
 � � � � � X L  T $ 0     d � 0 �  L	 0 	    � x H  � � � D �  � T � � �	 , �
 � � � � x 	 �	 �	 �	 \	 (	 �	 �	 \	 �	 t	 <	 	 �	 d	 �	 �	 �	 �		 �		 ,		 �	 �	 �	 �	 	 �	 @	 	 �	 `	 0	 x	 @	 	 �	 �
 �
 �
 p
 8
  
 �
 l
 ,
 �
 �
 �
 �
 
 �	
 x	
 d

 4

 $
 �

 �
 �
 �
 |
 �
 t 8 
 � @ �  �  |  � � l < , � 0   � � X ( � � � � � � 
 �	  �
  �  � �	 �	  8     h 8 0   � � � � � \ 0   � � � � x � � |	 H	 � T l
 � � � � x D @  L     � \ T    � � � � � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    �                                                                                                                                                                                                                   	                        �ŃH ~      DWMV_JA_JPU6012.F              f�  zp [F]PERIODIC.UID_US_RENAME;1                                                                                                        l                         � |     !       ro|   0� ��H      p   ��Ip         0               widget-0-761-5         
       	                  `              ��H         ߀ 	ISO8859-1Scale  ��H  	    P � ��IP    	       0             widget-0-762-45             D   valueScale    ��H       � ��I         ,               valueScale 2              /       ;       <       6       '    u  #    7  ?    �         :      +       B   	 �   G   	 �    h�f   �                   ScaleCb h�f            `          T  ScaleCb             ��H       ߀ 	ISO8859-1MHz��H  
    � L��I�    
     , �             radioFrame 2              /       ;                  #   0    (   <                     :     "       !   ��H      p x��Ip         0               widget-0-796-5         
       	                  `         #     ��H       0߀ 	ISO8859-1RadioBox   ��H      x ���Ix         0 L             widget-0-821-2         .       7                    &      %       $   ��H      � ���I�         0               widget-0-807-3             �   3       G   	 X   h�f &   |  �  %              SetScaleCb        ' "   ��H       t߀ 	ISO8859-1WFNX   ��H      �  ��I�         0               widget-0-813-3             |   G   	 L   h�f X   p  �  &   |           SetScaleCb        (   ��H       P߀ 	ISO8859-1WJUL   ��H      � ���I�         0               widget-0-819-3             |   G   	 L   h�f X   p  �  &   |           SetScaleCb        )   ��H        ߀ 	ISO8859-1WGBH   ��H  
    � ���I�    
     , �             textFrame 19
              /       ;                     0       <              "       !             !     +       *   ��H      p ���Ip         0               widget-0-837-5         
       	                  `         ,     ��H       �߀ 	ISO8859-1Text   ��H      D @��ID         0               widget-0-840-5                 	   ��H  
    � p��I�    
     0 �             commandFrame 5                /       ;                      0    
   <           (              .       -   ��H      p ���Ip         0               widget-0-1069-5        
       	                  `         /    ��H       �	߀ 	ISO8859-1Command��H      p 
��Ip         0               widget-0-1077-2        "       !              ,    `         0    ��H      ! 8
߀ 	ISO8859-1
periodic > ��H  
    � �
��I�    
     , �             dialogFrame 
              /       ;                  
   0       <           (   "       !        .           1      �   dialogForm    ��H      p �
��Ip         0               widget-0-1093-5        
       	                  `         2     ��H       �߀ 	ISO8859-1Dialogs��H  	    (L��I(   	     , @             dialogForm 9               	 	                              �       �        �       �        �   1    �   promptPick oselectPick  filePick    workingPick questionPick    errorPick   warningPick infoPick   templatePick      ��H      |��I        0               templatePick                  /       ;                      0       <                             4           �   G   	 �   Pih�f le  �  �   lePic        kingPManageCb ion      3 ic      4 k   ��H       �߀ 	ISO8859-1Template   ��H       �*templateDialog                                                                                                                                                                                                   ro|  ! |� ��H        ��I        ,               infoPick ick              /       ;                     0       <                              4           �   G   	 �   � h�f f  �  �   �   l            kManageCb eCb      5        6    ��H      "   ߀ 	ISO8859-1Information��H       *infoDialog   ��H      P��I        ,               warningPick               /       ;                     0       <                             4           �   G   	 �   � h�f f  �  �   �   l            kManageCb eCb      7        8    ��H       p߀ 	ISO8859-1Warning��H       �*warningDialog    ��H      ���I        ,               errorPick k               /       ;                      0       <                              4           �   G   	 �   � h�f f  �  �   �   l            kManageCb eCb      9        :    ��H       �߀ 	ISO8859-1Error  ��H       �*errorDialog  ��H      ,��I        0               questionPick                  /       ;                     0       <                              4           �   G   	 �   fh�f �   �  �   l            kManaManageCb        ;       < k   ��H       L߀ 	ISO8859-1Question   ��H       l*questionDialog   ��H      ���I        ,               workingPick               /       ;                     0       <                             4           �   G   	 �   � h�f f  �  �   �   l            kManageCb eCb      =        >    ��H       �߀ 	ISO8859-1Working��H       �*workingDialog    ��H      ��I        ,               filePick ck               /       ;                      0       <                             4           �   G   	 �   � h�f f  �  �   �   l            kManageCb eCb      ?        @    ��H      $ 4߀  	ISO8859-1FileSelection  ��H       P*fileDialog   ��H      ���I        ,               selectPick                /       ;                     0       <                             4           �   G   	 �   � h�f f  �  �   �   l            kManageCb eCb      A        B    ��H        �߀ 	ISO8859-1	Selection  ��H       �	*selectDialog ��H      �	��I        ,               promptPick                /       ;                     0       <                             4           �   G   	 �   � h�f f  �  �   �   l            kManageCb eCb      C        D    ��H       
߀ 	ISO8859-1Prompt ��H       4*promptDialog ��H  
    � d��I�    
     , �             scrollFrame 
              /       ;                     0       <              "       !                   F       E   ��H      p ���Ip         0               widget-0-856-5         
       	                  `         G     ��H        P߀ 	ISO8859-1	ScrollBar  ��H  	    D ���ID    	       0             widget-0-869-2               H   ��H      � ��I�         0               widget-0-867-6                /                  2   >              +          ��H  
    � `��I�    
     , �             listFrame 67
              /       ;                     0    #   <              "       !                   J       I   ��H      p ���Ip         0               widget-0-885-5         
       	                  `         K     ��H      # �߀ 	ISO8859-1ScrolledList   ��H      T D��IT         0               widget-0-888-5           D   	       L                ro|  $ h� ��H      �  @�  j  �  �  �  �  �   , H c  � � � �   8 T n � � � �    ߀ 	ISO8859-1alpha߀ 	ISO8859-1beta߀ 	ISO8859-1gamma߀ 	ISO8859-1delta߀ 	ISO8859-1epsilon߀ 	ISO8859-1zeta߀ 	ISO8859-1eta߀ 	ISO8859-1theta߀ 	ISO8859-1iota߀ 	ISO8859-1kappa߀ 	ISO8859-1lambda߀ 	ISO8859-1mu߀ 	ISO8859-1nu߀ 	ISO8859-1xi߀ 	ISO8859-1omicron߀ 	ISO8859-1pi߀ 	ISO8859-1rho߀ 	ISO8859-1sigma߀ 	ISO8859-1tau߀ 	ISO8859-1upsilon߀ 	ISO8859-1phi߀ 	ISO8859-1chi߀ 	ISO8859-1psi߀ 	ISO8859-1omega��H  
    �   ��I�    
     , �             paneFrame  
              /       ;                     0    (   <           (   "       !        be    �     N 8     M   ��H      p ��Ip         0               widget-0-904-5         
       	                  `         O     ��H      " �߀ 	ISO8859-1PanedWindow��H      P X��IP           0             widget-0-914-5               Q      P   ��H      \ ���I\         0               widget-0-909-9         ?    L                 R    ��H       �Open Software Foundation  ��H      \ `��I\         0               widget-0-913-9         ?    L                 S    ��H       �OSF/Motif Release 1.2
    ��H  
    � ���I�    
     , �             popupFrame 3
              /       ;                  #   0    (   <               "       !        be    �     U 8     T   ��H      p (��Ip         0               widget-0-930-5         
       	                  `         V     ��H        �߀ 	ISO8859-1	PopupMenu  ��H  	    P x��IP    	       0             widget-0-931-46             D   popupWindow   ��H      � ���I�         , �             popupWindow               /       ;                         |   >              W   !           �   popupMenu U   ��H      8��I8        l �   ,         popupMenu w h�f     P  \   /              InitPopupCb       X         9        D   	 �   h�f !   �        �          nu U PopdownCb T 
          b 1     a �     ` 8     _ o     ^      ]      \      [ -     Z      Y   ��H      T ���IT         0               widget-0-951-60            D          c it  ��H        
߀ 	ISO8859-1	Edit Menu  ��H      D �
��ID         0               widget-0-952-65        2          ��H      l �
��Il         0               widget-0-958-2             \   (    L         d b       e    ��H       ߀ 	ISO8859-1Undo   ��H       �U ��H      0 ���I0                         widget-0-959-16   ��H      l ���Il         0               widget-0-965-2             \   (    L         f b       g    ��H       ߀ 	ISO8859-1Cut��H       �t ��H      l ���Il         0               widget-0-971-2             \   (    L         h b       i    ��H       �߀ 	ISO8859-1Copy   ��H       XC ��H      l ���Il         0               widget-0-977-2             \   (    L         j b       k    ��H       �߀ 	ISO8859-1Paste  ��H       P ��H      0 L��I0                         widget-0-978-16   ��H      l `��Il         0               widget-0-984-2             \   (    L         l b       m    ��H       �߀ 	ISO8859-1Clear  ��H       $e ��H      l T��Il         0               widget-0-990-2             \   (    L         n b       o            ro|  + �� ��H         ߀ 	ISO8859-1Delete ��H         D ��H       0 *popupWindow  ��H      i D ߀e 	ISO8859-1Press  	ISO8859-1Button 3  	ISO8859-1or  	ISO8859-1Menu ��H  
    � d ��I�    
     , �             drawFrame -1
              /       ;                     0       <              "       !                    q p     p   ��H      p � ��Ip         0               widget-0-1010-5        
       	                  `         r     ��H      " �߀ 	ISO8859-1DrawingArea��H  
    l 0��Il    
     0 L             widget-0-1017-2        5       4                  `   drawArea     ��H      � d��I�         ,               drawArea 101       >        C   	 �   E   	 T     h�f     x      ea             DaResizeCb  h�f    �                      DaExposeCb    ��H  
    � ���I�    
     0 �             drawBtnFrame    
              /       ;                  
   0       <              "    
   !    
                s      �   drawnButton   ��H      p ���Ip         0               widget-0-1043-5        
       	                  `         t    ��H      " �߀ 	ISO8859-1DrawnButton��H      � ��I�         ,               drawnButton        -       C   	 �   E   	 �   A   	 \   h�f   �  �                  ShowCb        u  h�f     �         s         �   DbResizeCb  h�f 1  �      �               DbExposeCb    ��H       @*showDialog   ��H      �L��I�        , �            fileDialog                    t  1    d      T      D      4      $  )      *          �       �           F   	 �     h�f Re  �      f 1           �UnmapCb       v se      w         x \       y -      z         { et      | SO      } �      ~ 9-        	     9     �       � S    �       � I     � fileMenuBar   ��H  H    $ l��I$    H                     ile  ��H      4 T��I4                         widget-0-1432-26      ��H      P ���IP           ,             fileMenuBar  26         @   filterCascade d  ��H      � ���I�         0               filterCascade 26       8    x       h   (    X          �        � $ �     �     ��H      � 8	��I�         0 D             widget-0-1456-5        9                    �       |       p   textButton graphicsButton audioButton   ��H      h �	��Ih         ,               audioButton            X   (    H         �         �   ��H       �
߀ 	ISO8859-1Audio  ��H       �
A ��H      l ,��Il         0               graphicsButton             \   (    L         �       �     ��H       @߀ 	ISO8859-1Graphics   ��H       �G ��H      h ���Ih         ,               textButton t           X   (    H         �        �    ��H       ߀ 	ISO8859-1Text   ��H       �T ��H       �߀ 	ISO8859-1Format ��H       �F ��H      X ���IX         4               widget-0-1434-64               H           �    ��H       ߀ 	ISO8859-1Merge  ��H      X x��IX         4               widget-0-1435-65               H           �    ��H       �߀ 	ISO8859-1Remove ��H      * ߀& 	ISO8859-1FileSelectionDialog��H        D߀ 	ISO8859-1	Selection  ��H       �߀ 	ISO8859-1List   ��H      " �߀ 	ISO8859-1Directories��H       �߀ 	ISO8859-1Files  ��H       ߀ 	ISO8859-1Filter ��H       H߀ 	ISO8859-1 [    ]    ��H       x߀ 	ISO8859-1Load                       ro| 	 , `�X ��H         ߀ 	ISO8859-1Filter ��H         ߀ 	ISO8859-1Cancel ��H      D0 ��ID        0 0            selectDialog -65	                          1           �   *    �       �       �   F   	 �   uth�f     �                      UnmapCb       � 1       � ma      �        �        �        L        �                �   ��H  H    $ ` ��I$    H                     ele  ��H      & �߀" 	ISO8859-1SelectionDialog��H        �߀ 	ISO8859-1	Selection��H       (߀ 	ISO8859-1ItemsT ��H       \߀ 	ISO8859-1Ok6��H       �߀ 	ISO8859-1ApplyH��H       �߀ 	ISO8859-1Cancelo��H      � ���I�         , �             infoDialog g           �   &    �       �   F   	 \   h�f     �       �              UnmapCb       � f      �        �    ma          �  1     � � ��H  J    $ ��I$    J                     nfo ��H  H    $ ��I$    H                     nfo ��H      ( <߀$ 	ISO8859-1InformationDialogH��H      � t߀� � 	ISO8859-1Congratulations!  	ISO8859-1Bandwidth grab successful.  	ISO8859-1   	ISO8859-1Continue oscillation.el��H       �߀ 	ISO8859-1Dismiss��H      � \��I�         0 �             warningDialog 	           �   &    �   *    �       �   F   	 l   h�f 	  �      Cont        illatUnmapCb       �        �         �         � �             � 59��H  H    $ ���I$    H                     arn  ��H      $ �߀  	ISO8859-1WarningDialog  ��H      T �߀P 	ISO8859-1Warning, Warning,  	ISO8859-1Danger Will Robinson...����H       �߀ 	ISO8859-1Proceed��H       d߀ 	ISO8859-1Cancel ��H      � ���I�         , �             errorDialog            �   &    �   *    �       �   F   	 h   l h�f f  �                      iUnmapCb       �        �        �        �   �            � ����H  H    $ ���I$    H                     rro9 ��H      " �߀ 	ISO8859-1ErrorDialog��H      \ �߀X 	ISO8859-1Frontal lobe seek error!  	ISO8859-1Intellect not installed.-7��H       ,	߀ 	ISO8859-1Retry ��H       �	߀ 	ISO8859-1CancelI��H      � �	��I�         0 �             questionDialog            �   &    �   *    �       �   F   	 l   fh�f �   �                  iUnmaUnmapCb       �       �       �       � �               �      �   ��H  H    $ �	��I$    H                     ues  ��H      X ��IX         4               widget-0-1392-68               H          �  T ��H       @߀ 	ISO8859-1Maybe  ��H      % �߀! 	ISO8859-1QuestionDialog ��H      b �߀^ 	ISO8859-1This is Motif 1.2.  	ISO8859-1   	ISO8859-1Any questions ?��H       ߀ 	ISO8859-1Yes��H       �߀ 	ISO8859-1No ��H       � ���I�          0 �             workingDialog 	           �       �   F   	 X   esh�f ?  |      fh�f            UnmapCb       � ma      �              �       �       �      � ߀��H  J    $ ���I$    J                     ork ��H  H    $ ���I$    H                     ork-7��H  I    $ ��I$    I                     ork  ��H      h @��Ih         0               widget-0-1410-5        +       6       :    X   es      �   ��H      ' x߀# 	ISO8859-1Percent Complete   ��H       �߀ 	ISO8859-1Abort ��H      $ 0߀  	ISO8859-1WorkingDialog      "       !                    g                                    ro| 
 % ��P ��H      �   ��I�         0 �             promptDialog -5            �   1    �   *    �       �   F   	 l   fh�f     �      mapCb            UnmapCb       �       � �       �       �    �            � ��H  H    $   ��I$    H                     rom  ��H      #  ߀ 	ISO8859-1PromptDialog  ��H      $ 8߀  	ISO8859-1Name, please?  ��H       p߀ 	ISO8859-1Ok ��H       �߀ 	ISO8859-1Cancel��H      � ���I�         , �             showDialog g           t       d   &    T   �       �         �         � b              �       �   ��H  J    $ ��I$    J                     how  ��H  H    $ ���I$    H                     how ��H      , �߀( 	ISO8859-1OSF/Motif Release 1.2-7��H       ,߀ 	ISO8859-1Enjoy! ��H      �l߀�� 	ISO8859-1OSF/Motif Release 1.2  	ISO8859-1   	ISO8859-1May 1992  	ISO8859-1   	ISO8859-1#Dev: Vania, Ellis, Mike C, Mike Y,  	ISO8859-1Al, Scott, Daniel, Andrew, Pam 	ISO8859-1   	ISO8859-1   	ISO8859-1QA: Libby, Carl, Tom, Ramesh,  	ISO8859-1Susan, Shobana  	ISO8859-1   	ISO8859-1Doc: Bob M, Ken, Bob C.  	ISO8859-1   	ISO8859-1Rats: Yakov, Bruce  	ISO8859-1   	ISO8859-1VTS: Carlos, Roy  	ISO8859-1   	ISO8859-1!Sys: David, Kevin, Chauncy, Ron,  	ISO8859-1Weidong, Barbara  	ISO8859-1   	ISO8859-1Ops: Bruce, Cathy  	ISO8859-1  h���H      � ���I�         ( X             menuBar        %    <   59 �    helpCascade 1May 99        �   -    �       �   #    �        �   helpCascade optionsCascade 5viewCascade editCascade fileCascade  ��H      � ���I�         ,               fileCascade        8    t       d   (    T   1      �       �    �     �   
 ��H      � p��I�         0 D             widget-0-161-5         9                T    �        �       �        �       �        �   exitButton pprintButton saveAsButton iewsaveButton topenButton enewButton co��H      h ��Ih         ,               newButton 61           X   (    H         �         � ��H       �߀ 	ISO8859-1New��H       x	N ��H      h �	��Ih         ,               openButton 1           X   (    H         �         � an��H       �	߀ 	ISO8859-1Open...��H       4
O ��H      h d
��Ih         ,               saveButton 1           X   (    H         �         � H��H       x
߀ 	ISO8859-1Save di��H       �
S ��H      l $��Il         0               saveAsButton               \   (    L         �       �   � ��H      ! 8߀ 	ISO8859-1
Save As... ��H       �A ��H      h ���Ih         ,               printButton            X   (    H         �        �  ��H        ߀ 	ISO8859-1Print  ��H       |P ��H      � ���I�         ,               exitButton             �   (    �   A   	 T    h�f   x       �              ExitCb       �         �  pqu��H       �߀ 	ISO8859-1Exitk ��H       tE ��H       �߀ 	ISO8859-1File   ��H       �F ��H      � ���I�         ,               editCascade        8    t       d   (    T          �         �   �     �  ��H      � ���I�         0 D             widget-0-219-5         9                T    �         �     �        �       �        �      �   i    �   deleteButton iewclearButton pasteButton copyButton cutButton -1undoButton 	                                                                                  ro|  0 �d� ��H      h   ��Ih         ,               undoButton 9           X   (    H         �         �   ��H         ߀ 	ISO8859-1Undo 4 ��H       | U ��H      0 � ��I0                         widget-0-212-20   ��H      h � ��Ih         ,               cutButton 12           X   (    H         �         � ��H       ߀ 	ISO8859-1Cut��H       �t ��H      h ���Ih         ,               copyButton 2           X   (    H         �         � h���H       �߀ 	ISO8859-1Copyb ��H       <C ��H      h l��Ih         ,               pasteButton            X   (    H         �         �   ��H       �߀ 	ISO8859-1Paste ��H       �P ��H      0 ,��I0                         widget-0-216-20   ��H      h @��Ih         ,               clearButton            X   (    H         �         � H��H       �߀ 	ISO8859-1ClearH��H        e ��H      l 0��Il         0               deleteButton               \   (    L         �       �    ��H       D߀ 	ISO8859-1Delete ��H       �D ��H       �߀ 	ISO8859-1Edit   ��H       E ��H      � 8��I�         ,               viewCascade        8    t       d   (    T           �         �   �     �     ��H      � L��I�         0 D             widget-0-275-5         9                T    �        �        �      �       �       �        �   yearButton  monthButton weekButton tdayButton uttileButton tstackButton   ��H      � ���I�         ,               stackButton            �   (    �          G   	 \   h�f     �      �           LayoutCb       �         �    ��H       �߀ 	ISO8859-1Stack ��H       �S ��H      � ���I�         ,               tileButton             �   (    �          G   	 \   h�f     �      �           LayoutCb       �         �   lo��H       �߀ 	ISO8859-1Tile   ��H       �T ��H      0 ���I0                         widget-0-270-20 < ��H      � ���I�         ,               dayButton 70           �   (    �          G   	 \   h�f     �      �           ViewCb b      �        �  -1��H       ,	߀ 	ISO8859-1Day��H       �	D ��H      � 
��I�         ,               weekButton 0           �   (    �          G   	 \   h�f     �      �           ViewCb b      �        �  h���H       (
߀ 	ISO8859-1Weekb ��H       �
W ��H      � ��I�         ,               monthButton            �   (    �          G   	 \   h�f     �      �           ViewCb b      �        �   ��H       (߀ 	ISO8859-1Month  ��H       �M ��H      � ��I�         ,               yearButton             �   (    �          G   	 \   h�f     �      �           ViewCb b      �        �    ��H       (߀ 	ISO8859-1Year  ��H       �Y ��H       ߀ 	ISO8859-1View  ��H       (V ��H      � X��I�         0               optionsCascade         8    x       h   (    X   	       � �       �    �     �  b����H      � l��I�         0 D             widget-0-340-5         9                    �       �   �    �       |   wrapButton bcaseButton  directCascade  fontCascade  ��H      � ��I�         ,               fontCascade        8    t       d   (    T           �         �  �     � tt8859-1ScrolledList   ��H      T D��IT         0               widget-0-888-5           D   	       L                ro|  6 �L ��H      �   ��I�         0 D             widget-0-355-5         9                T    �        �       p   largeFontButton mediumFontButton n  smallFontButton ��H      l   ��Il         0               smallFontButton            \   (    L         �        �   ��H       � ߀ 	ISO8859-1Small88��H       8S ��H      p h��Ip         4               mediumFontButton               `   (    P         �       �    ��H       |߀ 	ISO8859-1Medium��H        M ��H      l 0��Il         0               largeFontButton            \   (    L         �        �  SO��H       D߀ 	ISO8859-1Large ��H       �L ��H       �߀ 	ISO8859-1Font   ��H       F ��H      � 8��I�         0               directCascade n        8    x       h   (    X          �        � on �     � Fo����H      � L��I�         0 D             widget-0-390-5         9                    �       �       �       |   rightDirButton uleftDirButton ondownDirButton scupDirButton   ��H      h ���Ih         ,               upDirButton            X   (    H         �         � -9��H       �߀ 	ISO8859-1Up ��H       0U ��H      l \��Il         0               downDirButton              \   (    L         �       �   ��H       p߀ 	ISO8859-1Downse ��H       �D ��H      l  ��Il         0               leftDirButton              \   (    L                    ��H       4߀ 	ISO8859-1Left  ��H       �L ��H      l ���Il         0               rightDirButton             \   (    L                   ��H       �߀ 	ISO8859-1Rightu��H       xR ��H        �߀ 	ISO8859-1	Direction ��H       �D ��H      h ���Ih         ,               caseButton t           X   (    H                  ��H      % ߀! 	ISO8859-1Case Sensitive��H       �C ��H      h ���Ih         ,               wrapButton t           X   (    H                   ��H        �߀ 	ISO8859-1	Word Wrap  ��H       H	W ��H       |	߀ 	ISO8859-1Options��H       �	O ��H      � �	��I�         ,               helpCascade        8    t       d   (    T                 	 �     
ir ��H      � �	��I�         0 D             widget-0-443-5         9                T    �        �       �       |   versionButton  ututorButton windowButton ttocontextButton n   ��H      l l
��Il         0               contextButton              \   (    L                  Un��H      ! 8߀ 	ISO8859-1
On Context ��H�                                                                                                                                                                                                                                                   
                        �C� ~      DWMV_JA_JPU6012.F              f�  zp [F]PERIODIC.UID_US_RENAME;1                                                                                                        l                         h�      _              �C ��H      l ���Il         0               windowButton               \   (    L                    ��H         ߀ 	ISO8859-1	On Window	I��H       �W ��H      h ���Ih         ,               tutorButton            X   (    H                 h ��H       �߀ 	ISO8859-1TutorialO88��H       DT ��H      l x��Il         0               versionButton              \   (    L                  ��H      ! �߀ 	ISO8859-1
On Version1��H       V ��H       @߀ 	ISO8859-1Help   ��H       TH ��H      X ���IX           0             templateMenuBar             D   templateFileCascade  ��H      � ���I�         4               templateFileCascade        8    |       l   (    \                    �      u-990-2             \   (    L         n b       o            ro|   �
|t��H      �   ��I�         0 D             widget-0-1273-5        9        |            �       �       �        �       �   cancelButton on spellButton draftButton includeButton ttsendButton o ��H      h   ��Ih         ,               sendButton 7           X   (    H                   ��H       � ߀ 	ISO8859-1Send  ��H       \S ��H      l ���Il         0               includeButton              \   (    L                  ��H      ! �߀ 	ISO8859-1
Include...1��H        I ��H      h T��Ih         ,               draftButton            X   (    H                   ��H       h߀ 	ISO8859-1Draft  ��H       �D ��H      h ��Ih         ,               spellButton            X   (    H                 po��H       (߀ 	ISO8859-1Spell  ��H       �p ��H      � ���I�         0               cancelButton               �   (    �   A   	 X    h�f     |  �     �           �   UnmanageCb        n       af       eB  ��H       �߀ 	ISO8859-1Cancel ��H       �C ��H       �*templateDialog " ��H       �߀ 	ISO8859-1File  ��H       F ��H      L�Z�K         2  C  R  _  k  |  �  �  �  �  �  �  �        ,  >  K  ^  n  z  �  �  �  �  �  �  �  �  �  �  
      &  5  C  K  T  b  p  |  �  �  �  �  �  �  �  �  �  
      &  3  ?  M  Y  c  o  u  �  �  �  �  �  �  �  �  �     applyLabelString arrowDirection autoUnmanage borderWidth bottomAttachment bottomPosition cancelLabelString childHorizontalSpacing childType childVerticalAlignment decimalPoints dialogTitle dirListLabelString editMode fileListLabelString fillOnSelect filterLabelString fractionBase highlightThickness historyMaxItems indicatorOn indicatorType itemCount items labelString leftAttachment leftPosition listItemCount listItems listLabelString listVisibleItemCount marginHeight marginWidth maximum menuBar menuHelpWidget messageString minimum mnemonic noMatchString okLabelString orientation promptString pushButtonEnabled radioAlwaysOne rightAttachment rightPosition selectionLabelString separatorType set shadowThickness shadowType showValue spacing subMenuId tearOffModel titleString topAttachment topPosition topWidget traversalOn value createCallback activateCallback dragCallback exposeCallback popdownCallback resizeCallback unmapCallback valueChangedCallback Help Message OK              H��H      �`�Z�!         �   �   �   �   �   �        )  7  Q  _  r  �  �  �  �  �  �  �       .  @  U  j  �  �  �  �  �  XmCreateArrowButton XmCreateCascadeButton XmCreateCommand XmCreateDrawingArea XmCreateDrawnButton XmCreateErrorDialog XmCreateFileSelectionDialog XmCreateForm XmCreateFrame XmCreateInformationDialog XmCreateLabel XmCreateMainWindow XmCreateMenuBar XmCreateOptionMenu XmCreatePanedWindow XmCreatePopupMenu XmCreatePromptDialog XmCreatePulldownMenu XmCreatePushButton XmCreateQuestionDialog XmCreateRadioBox XmCreateScale XmCreateScrollBar XmCreateScrolledList XmCreateScrolledText XmCreateSelectionDialog XmCreateSeparator XmCreateText XmCreateToggleButton XmCreateWarningDialog XmCreateWorkingDialog            ߀ 	ISO8859-1Merge  ��H      X x��IX         4               widget-0-1435-65               H           �    ��H       �߀ 	ISO8859-1Remove ��H      * ߀& 	ISO8859-1FileSelectionDialog��H        D߀ 	ISO8859-1	Selection  ��H       �߀ 	ISO8859-1List   ��H      " �߀ 	ISO8859-1Directories��H       �߀ 	ISO8859-1Files  ��H       ߀ 	ISO8859-1Filter ��H       H߀ 	ISO8859-1 [    ]    ��H       x߀ 	ISO8859-1Load                       �               * [F]PERIODIC.UIL_US_RENAME;1 +  , d�   . Z    /     4 P   Z   K 0                   - zp   0  � 1    2   3      K � P   W   O L    5   6 �����֟  7 ����֟  8          9          G    H �� J �       f             ! F ! (c) Copyright 1989, 1990, 1991, 1992 OPEN SOFTWARE FOUNDATION, INC.  ! ALL RIGHTS RESERVED  !  !  ! Motif Release 1.2  ! P !   $RCSfile: periodic.uil,v $ $Revision: 1.6.2.3 $ $Date: 1992/04/27 21:53:10 $O !******************************************************************************   B !***************************************************************** !* !*   Author: Alastair Gourlay  !*   Date: April 1992  !*   Author: Michael K. Yee  !*   Date: August 1990 !*B !*****************************************************************     module periodic      version = 'v1.2'     names = case_sensitive     character_set=iso_latin1     objects = {  	XmLabel = widget; 	XmPushButton = widget;  	XmToggleButton = widget;  	XmCascadeButton = widget; 	XmSeparator = widget;     }   $ include file ("periodic_local.uil");  	 procedure  	ManageCb(string); 	UnmapCb();  	UnmanageCb(string); 	ShowCb(string); 	InitPopupCb(string);  	PopdownCb();  	DaExposeCb(); 	DaResizeCb(); 	DbExposeCb(); 	DbResizeCb(); 	ScaleCb();  	SetScaleCb(integer); 
 	ViewCb(); 	LayoutCb(); 	ToggleLightsCb();
 	ExitCb();   value  	x0 : 0; 	x1 : 5;	 	x2 : 10; 	 	x3 : 15; 	 	x4 : 20; 	 	x5 : 25; 	 	x6 : 30; 	 	x7 : 35; 	 	x8 : 40;    	y0 : 0; 	y1 : 8;	 	y2 : 16; 	 	y3 : 24; 	 	y4 : 32; 	 	y5 : 40;    value  	boxMarginWidth : 5; 	boxMarginHeight : 5;    list formArgs : arguments { ' 	XmNleftAttachment = XmATTACH_POSITION; ( 	XmNrightAttachment = XmATTACH_POSITION;& 	XmNtopAttachment = XmATTACH_POSITION;) 	XmNbottomAttachment = XmATTACH_POSITION;  };   list titleArgs : arguments {$ 	XmNchildType = XmFRAME_TITLE_CHILD; 	XmNchildHorizontalSpacing = 4; 7 	XmNchildVerticalAlignment = XmALIGNMENT_WIDGET_BOTTOM;  };   list toggleArgs : arguments {  	XmNfillOnSelect = true; 	XmNindicatorOn = false; 	XmNshadowThickness = 1; };         !  ! Main window  !  object appMain : XmMainWindow {      arguments {   	XmNmenuBar = XmMenuBar menuBar;     };     controls { 	XmMenuBar menuBar; 
 	XmFrame { 	    arguments { 		XmNmarginWidth = 20; 		XmNmarginHeight = 20;  	    }; # 	    controls { XmForm workArea; };  	}; , 	unmanaged XmFileSelectionDialog fileDialog;* 	unmanaged XmSelectionDialog selectDialog; ! DEC_MOTIF_BUG_FIX , !	unmanaged XmTemplateDialog templateDialog;* 	unmanaged XmInformationDialog infoDialog;) 	unmanaged XmWarningDialog warningDialog; % 	unmanaged XmErrorDialog errorDialog; + 	unmanaged XmQuestionDialog questionDialog; ) 	unmanaged XmWorkingDialog workingDialog; ' 	unmanaged XmPromptDialog promptDialog; * 	unmanaged XmInformationDialog showDialog;     }; };         !  ! Menu bar and pulldown menus  !    object menuBar : XmMenuBar {     arguments { 1 	XmNmenuHelpWidget = XmCascadeButton helpCascade;      };     controls { 	XmCascadeButton fileCascade;  	XmCascadeButton editCascade;  	XmCascadeButton viewCascade;   	XmCascadeButton optionsCascade; 	XmCascadeButton helpCascade;      }; };  & object fileCascade : XmCascadeButton {     arguments { " 	XmNlabelString = fileCascadeText;' 	XmNmnemonic = keysym(fileCascadeMnem);      };     controls { XmPulldownMenu { 5 	arguments { XmNtearOffModel = XmTEAR_OFF_ENABLED; };  	controls {  	    XmPushButton newButton; 	    XmPushButton openButton;  	    XmPushButton saveButton;  	    XmPushButton saveAsButton;  	    XmPushButton printButton; 	    XmPushButton exitButton;  	}; };     }; };  ! object newButton : XmPushButton {      arguments {   	XmNlabelString = newButtonText;% 	XmNmnemonic = keysym(newButtonMnem);      }; };" object openButton : XmPushButton {     arguments { ! 	XmNlabelString = openButtonText; & 	XmNmnemonic = keysym(openButtonMnem);     }; };" object saveButton : XmPushButton {     arguments { ! 	XmNlabelString = saveButtonText; & 	XmNmnemonic = keysym(saveButtonMnem);     }; };$ object saveAsButton : XmPushButton {     arguments { # 	XmNlabelString = saveAsButtonText; ( 	XmNmnemonic = keysym(saveAsButtonMnem);     }; };# object printButton : XmPushButton {      arguments { " 	XmNlabelString = printButtonText;' 	XmNmnemonic = keysym(printButtonMnem);      }; };" object exitButton : XmPushButton {     arguments { ! 	XmNlabelString = exitButtonText; & 	XmNmnemonic = keysym(exitButtonMnem);     };<     callbacks { XmNactivateCallback = procedure ExitCb(); }; };  & object editCascade : XmCascadeButton {     arguments { " 	XmNlabelString = editCascadeText;' 	XmNmnemonic = keysym(editCascadeMnem);      };     controls { XmPulldownMenu { 5 	arguments { XmNtearOffModel = XmTEAR_OFF_ENABLED; };  	controls {  	    XmPushButton undoButton;  	    XmSeparator { };  	    XmPushButton cutButton; 	    XmPushButton copyButton;  	    XmPushButton pasteButton; 	    XmSeparator { };  	    XmPushButton clearButton; 	    XmPushButton deleteButton;  	}; };     }; };  " object undoButton : XmPushButton {     arguments { ! 	XmNlabelString = undoButtonText; & 	XmNmnemonic = keysym(undoButtonMnem);     }; };! object cutButton : XmPushButton {      arguments {   	XmNlabelString = cutButtonText;% 	XmNmnemonic = keysym(cutButtonMnem);      }; };" object copyButton : XmPushButton {     arguments { ! 	XmNlabelString = copyButtonText; & 	XmNmnemonic = keysym(copyButtonMnem);     }; };# object pasteButton : XmPushButton {      arguments { " 	XmNlabelString = pasteButtonText;' 	XmNmnemonic = keysym(pasteButtonMnem);      }; };# object clearButton : XmPushButton {      arguments { " 	XmNlabelString = clearButtonText;' 	XmNmnemonic = keysym(clearButtonMnem);      }; };$ object deleteButton : XmPushButton {     arguments { # 	XmNlabelString = deleteButtonText; ( 	XmNmnemonic = keysym(deleteButtonMnem);     }; };  & object viewCascade : XmCascadeButton {     arguments { " 	XmNlabelString = viewCascadeText;' 	XmNmnemonic = keysym(viewCascadeMnem);      };     controls { XmPulldownMenu { 5 	arguments { XmNtearOffModel = XmTEAR_OFF_ENABLED; };  	controls {   	    XmToggleButton stackButton; 	    XmToggleButton tileButton;  	    XmSeparator { };  	    XmToggleButton dayButton; 	    XmToggleButton weekButton;   	    XmToggleButton monthButton; 	    XmToggleButton yearButton;  	}; };     }; };  % object stackButton : XmToggleButton {      arguments { " 	XmNlabelString = stackButtonText;' 	XmNmnemonic = keysym(stackButtonMnem); " 	XmNindicatorType = XmONE_OF_MANY;     };B     callbacks { XmNvalueChangedCallback = procedure LayoutCb(); }; };$ object tileButton : XmToggleButton {     arguments { ! 	XmNlabelString = tileButtonText; & 	XmNmnemonic = keysym(tileButtonMnem);" 	XmNindicatorType = XmONE_OF_MANY;     };B     callbacks { XmNvalueChangedCallback = procedure LayoutCb(); }; };# object dayButton : XmToggleButton {      arguments {   	XmNlabelString = dayButtonText;% 	XmNmnemonic = keysym(dayButtonMnem); " 	XmNindicatorType = XmONE_OF_MANY;     };@     callbacks { XmNvalueChangedCallback = procedure ViewCb(); }; };$ object weekButton : XmToggleButton {     arguments { ! 	XmNlabelString = weekButtonText; & 	XmNmnemonic = keysym(weekButtonMnem);" 	XmNindicatorType = XmONE_OF_MANY;     };@     callbacks { XmNvalueChangedCallback = procedure ViewCb(); }; };% object monthButton : XmToggleButton {      arguments { " 	XmNlabelString = monthButtonText;' 	XmNmnemonic = keysym(monthButtonMnem); " 	XmNindicatorType = XmONE_OF_MANY;     };@     callbacks { XmNvalueChangedCallback = procedure ViewCb(); }; };$ object yearButton : XmToggleButton {     arguments { ! 	XmNlabelString = yearButtonText; & 	XmNmnemonic = keysym(yearButtonMnem);" 	XmNindicatorType = XmONE_OF_MANY;     };@     callbacks { XmNvalueChangedCallback = procedure ViewCb(); }; };  ) object optionsCascade : XmCascadeButton {      arguments { % 	XmNlabelString = optionsCascadeText; * 	XmNmnemonic = keysym(optionsCascadeMnem);     };     controls { XmPulldownMenu { 5 	arguments { XmNtearOffModel = XmTEAR_OFF_ENABLED; };  	controls { ! 	    XmCascadeButton fontCascade; # 	    XmCascadeButton directCascade;  	    XmToggleButton caseButton;  	    XmToggleButton wrapButton;  	}; };     }; };  & object fontCascade : XmCascadeButton {     arguments { " 	XmNlabelString = fontCascadeText;' 	XmNmnemonic = keysym(fontCascadeMnem);      };     controls { XmPulldownMenu { 5 	arguments { XmNtearOffModel = XmTEAR_OFF_ENABLED; };  	controls { " 	    XmPushButton smallFontButton;# 	    XmPushButton mediumFontButton; " 	    XmPushButton largeFontButton; 	}; };     }; };  ' object smallFontButton : XmPushButton {      arguments { & 	XmNlabelString = smallFontButtonText;+ 	XmNmnemonic = keysym(smallFontButtonMnem);      }; };( object mediumFontButton : XmPushButton {     arguments { ' 	XmNlabelString = mediumFontButtonText; , 	XmNmnemonic = keysym(mediumFontButtonMnem);     }; };' object largeFontButton : XmPushButton {      arguments { & 	XmNlabelString = largeFontButtonText;+ 	XmNmnemonic = keysym(largeFontButtonMnem);      }; };  ( object directCascade : XmCascadeButton {     arguments { $ 	XmNlabelString = directCascadeText;) 	XmNmnemonic = keysym(directCascadeMnem);      };     controls { XmPulldownMenu { 5 	arguments { XmNtearOffModel = XmTEAR_OFF_ENABLED; };  	controls {  	    XmPushButton upDirButton;  	    XmPushButton downDirButton;  	    XmPushButton leftDirButton;! 	    XmPushButton rightDirButton;  	}; };     }; };  # object upDirButton : XmPushButton {      arguments {  	XmNlabelString = upButtonText; $ 	XmNmnemonic = keysym(upButtonMnem);     }; };% object downDirButton : XmPushButton {      arguments { ! 	XmNlabelString = downButtonText; & 	XmNmnemonic = keysym(downButtonMnem);     }; };% object leftDirButton : XmPushButton {      arguments { ! 	XmNlabelString = leftButtonText; & 	XmNmnemonic = keysym(leftButtonMnem);     }; };& object rightDirButton : XmPushButton {     arguments { " 	XmNlabelString = rightButtonText;' 	XmNmnemonic = keysym(rightButtonMnem);      }; };$ object caseButton : XmToggleButton {     arguments { ! 	XmNlabelString = caseButtonText; & 	XmNmnemonic = keysym(caseButtonMnem);     }; };$ object wrapButton : XmToggleButton {     arguments { ! 	XmNlabelString = wrapButtonText; & 	XmNmnemonic = keysym(wrapButtonMnem);     }; };  & object helpCascade : XmCascadeButton {     arguments { " 	XmNlabelString = helpCascadeText;' 	XmNmnemonic = keysym(helpCascadeMnem);      };     controls { XmPulldownMenu {*5 	arguments { XmNtearOffModel = XmTEAR_OFF_ENABLED; };* 	controls {   	    XmPushButton contextButton; 	    XmPushButton windowButton;* 	    XmPushButton tutorButton;  	    XmPushButton versionButton; 	}; };     }; };  % object contextButton : XmPushButton {*     arguments {*$ 	XmNlabelString = contextButtonText;) 	XmNmnemonic = keysym(contextButtonMnem);.     }; };$ object windowButton : XmPushButton {     arguments {b# 	XmNlabelString = windowButtonText;s( 	XmNmnemonic = keysym(windowButtonMnem);     }; };# object tutorButton : XmPushButton {w     arguments { " 	XmNlabelString = tutorButtonText;' 	XmNmnemonic = keysym(tutorButtonMnem);p     }; };% object versionButton : XmPushButton {i     arguments { $ 	XmNlabelString = versionButtonText;) 	XmNmnemonic = keysym(versionButtonMnem);      }; };         !C ! Work area widgets( !    object workArea : XmForm {     arguments {  	XmNfractionBase = 40;     };     controls { 	XmLabel titleLabel; 	XmLabel subtitleLabel;5 	XmFrame separatorFrame; 	XmFrame arrowFrame; 	XmFrame pushFrame;2 	XmFrame toggleFrame;  	XmFrame labelFrame; 	XmFrame optionFrame;  	XmFrame scaleFrame; 	XmFrame radioFrame; 	XmFrame textFrame;O 	XmFrame commandFrame; 	XmFrame dialogFrame;O 	XmFrame scrollFrame;  	XmFrame listFrame;X 	XmFrame paneFrame;X 	XmFrame popupFrame; 	XmFrame drawFrame;a 	XmFrame drawBtnFrame;     }; };   object titleLabel : XmLabel {S     arguments {N' 	XmNleftAttachment = XmATTACH_POSITION;E( 	XmNrightAttachment = XmATTACH_POSITION;- 	XmNleftPosition = x1; XmNrightPosition = x7;O" 	XmNtopAttachment = XmATTACH_FORM;! 	XmNlabelString = titleLabelText;      }; };  object subtitleLabel : XmLabel {     arguments {a' 	XmNleftAttachment = XmATTACH_POSITION;o( 	XmNrightAttachment = XmATTACH_POSITION;- 	XmNleftPosition = x1; XmNrightPosition = x7;r$ 	XmNtopAttachment = XmATTACH_WIDGET;# 	XmNtopWidget = XmLabel titleLabel;m$ 	XmNlabelString = subtitleLabelText;     }; };  ! object separatorFrame : XmFrame {      arguments {  	arguments formArgs;- 	XmNleftPosition = x0; XmNrightPosition = x1;i- 	XmNtopPosition = y0; XmNbottomPosition = y1;g! 	XmNmarginWidth = boxMarginWidth;r" 	XmNmarginHeight = boxMarginWidth;     };     controls { 	XmLabel  { arguments {i 	    arguments titleArgs;u) 	    XmNlabelString = separatorFrameText;m 	}; }; I 	XmSeparator { };o     }; };   object arrowFrame : XmFrame {a     arguments {  	arguments formArgs;- 	XmNleftPosition = x0; XmNrightPosition = x1;i- 	XmNtopPosition = y1; XmNbottomPosition = y2; ! 	XmNmarginWidth = boxMarginWidth;C" 	XmNmarginHeight = boxMarginWidth;     };     controls { 	XmLabel  { arguments {u 	    arguments titleArgs;s% 	    XmNlabelString = arrowFrameText;  	}; }; l	 	XmForm {X 	    arguments { 		XmNfractionBase = 3; 	    };  	    controls {; 		XmArrowButton arrowB1; 		XmArrowButton arrowB2; 		XmArrowButton arrowB3; 		XmArrowButton arrowB4; 	    };l 	};T     }; };    object arrowB1 : XmArrowButton {     arguments {  	arguments formArgs;+ 	XmNleftPosition = 0; XmNrightPosition = 1; + 	XmNtopPosition = 1; XmNbottomPosition = 2;o" 	XmNarrowDirection = XmARROW_LEFT; 	XmNshadowThickness = 0;     }; };  object arrowB2 : XmArrowButton {     arguments {X 	arguments formArgs;+ 	XmNleftPosition = 2; XmNrightPosition = 3;)+ 	XmNtopPosition = 1; XmNbottomPosition = 2;t# 	XmNarrowDirection = XmARROW_RIGHT;n 	XmNshadowThickness = 0;     }; };  object arrowB3 : XmArrowButton {     arguments {o 	arguments formArgs;+ 	XmNleftPosition = 1; XmNrightPosition = 2;e+ 	XmNtopPosition = 0; XmNbottomPosition = 1;   	XmNarrowDirection = XmARROW_UP; 	XmNshadowThickness = 0;     }; };  object arrowB4 : XmArrowButton {     arguments {y 	arguments formArgs;+ 	XmNleftPosition = 1; XmNrightPosition = 2;o+ 	XmNtopPosition = 2; XmNbottomPosition = 3;n" 	XmNarrowDirection = XmARROW_DOWN; 	XmNshadowThickness = 0;     }; };   object pushFrame : XmFrame {     arguments {X 	arguments formArgs;- 	XmNleftPosition = x1; XmNrightPosition = x2;m- 	XmNtopPosition = y2; XmNbottomPosition = y3; ! 	XmNmarginWidth = boxMarginWidth;e" 	XmNmarginHeight = boxMarginWidth;     };     controls { 	XmLabel  { arguments {x 	    arguments titleArgs;d$ 	    XmNlabelString = pushFrameText; 	}; }; d	 	XmForm {  	    controls {e 		XmPushButton { arguments {( 		    XmNleftAttachment = XmATTACH_FORM;) 		    XmNrightAttachment = XmATTACH_FORM;P+ 		    XmNtopAttachment = XmATTACH_POSITION;u 		    XmNtopPosition = 10;. 		    XmNbottomAttachment = XmATTACH_POSITION; 		    XmNbottomPosition = 90;B& 		    XmNlabelString = pushButtonText; 		}; };t 	    };o 	};m     }; };   object toggleFrame : XmFrame {     arguments {t 	arguments formArgs;- 	XmNleftPosition = x1; XmNrightPosition = x2;t- 	XmNtopPosition = y1; XmNbottomPosition = y2;e! 	XmNmarginWidth = boxMarginWidth;n" 	XmNmarginHeight = boxMarginWidth;     };     controls { 	XmLabel  { arguments {u 	    arguments titleArgs; & 	    XmNlabelString = toggleFrameText; 	}; }; n7 	XmForm { controls { XmToggleButton toggleButton; }; };      }; };  & object toggleButton : XmToggleButton {     arguments {y# 	XmNleftAttachment = XmATTACH_FORM;b$ 	XmNrightAttachment = XmATTACH_FORM;& 	XmNtopAttachment = XmATTACH_POSITION; 	XmNtopPosition = 10;c) 	XmNbottomAttachment = XmATTACH_POSITION;b 	XmNbottomPosition = 90;# 	XmNlabelString = toggleButtonText;S     };     callbacks { 6 	XmNvalueChangedCallback = procedure ToggleLightsCb();     }; };   object labelFrame : XmFrame {      arguments {N 	arguments formArgs;- 	XmNleftPosition = x0; XmNrightPosition = x1;)- 	XmNtopPosition = y2; XmNbottomPosition = y3;a     };     controls { 	XmLabel { arguments { 	    arguments titleArgs;T% 	    XmNlabelString = labelFrameText;B 	}; };. 	XmForm { controls { XmLabel labelLabel; }; };     }; };   object labelLabel : XmLabel {u     arguments {g# 	XmNleftAttachment = XmATTACH_FORM;B$ 	XmNrightAttachment = XmATTACH_FORM;" 	XmNtopAttachment = XmATTACH_FORM;% 	XmNbottomAttachment = XmATTACH_FORM;n 	XmNlabelString = labelText;     }; };  t object optionFrame : XmFrame {     arguments {N 	arguments formArgs;- 	XmNleftPosition = x7; XmNrightPosition = x8;t- 	XmNtopPosition = y2; XmNbottomPosition = y3;n     };     controls { 	XmLabel  { arguments {n 	    arguments titleArgs;y& 	    XmNlabelString = optionFrameText; 	}; }; 	XmOptionMenu {  	    arguments {" 		XmNlabelString = optionMenuText; 		XmNorientation = XmVERTICAL;' 		XmNmnemonic = keysym(optionMenuMnem);X 	    };r 	    controls {x 		XmPulldownMenu {: 		    arguments { XmNtearOffModel = XmTEAR_OFF_ENABLED; }; 		    controls { 			XmPushButton { arguments { ) 			    XmNlabelString = sundayButtonText;o. 			    XmNmnemonic = keysym(sundayButtonMnem); 			}; }; 			XmPushButton { arguments {=) 			    XmNlabelString = mondayButtonText; . 			    XmNmnemonic = keysym(mondayButtonMnem); 			}; }; 			XmPushButton { arguments {;* 			    XmNlabelString = tuesdayButtonText;/ 			    XmNmnemonic = keysym(tuesdayButtonMnem);T 			}; }; 			XmPushButton { arguments {e, 			    XmNlabelString = wednesdayButtonText;1 			    XmNmnemonic = keysym(wednesdayButtonMnem);d 			}; }; 			XmPushButton { arguments { + 			    XmNlabelString = thursdayButtonText;S0 			    XmNmnemonic = keysym(thursdayButtonMnem); 			}; }; 			XmPushButton { arguments {_) 			    XmNlabelString = fridayButtonText;h. 			    XmNmnemonic = keysym(fridayButtonMnem); 			}; }; 			XmPushButton { arguments { + 			    XmNlabelString = saturdayButtonText;s0 			    XmNmnemonic = keysym(saturdayButtonMnem); 			}; }; 		    }; 		}; 	    };d 	};n     }; };   object scaleFrame : XmFrame {F     arguments {c 	arguments formArgs;- 	XmNleftPosition = x6; XmNrightPosition = x7;d- 	XmNtopPosition = y1; XmNbottomPosition = y2; ! 	XmNmarginWidth = boxMarginWidth; # 	XmNmarginHeight = boxMarginHeight;a     };     controls { 	XmLabel  { arguments {e 	    arguments titleArgs; % 	    XmNlabelString = scaleFrameText;  	}; };. 	XmForm { controls { XmScale valueScale; }; };     }; };   object valueScale : XmScale {c     arguments {X# 	XmNleftAttachment = XmATTACH_FORM;P$ 	XmNrightAttachment = XmATTACH_FORM;& 	XmNtopAttachment = XmATTACH_POSITION; 	XmNtopPosition = 15;o 	XmNshowValue = true;  	XmNminimum = scaleMinimum;e 	XmNmaximum = scaleMaximum;; 	XmNvalue = scaleValue;m 	XmNdecimalPoints = 1;! 	XmNtitleString = scaleTitleText;  	XmNorientation = XmHORIZONTAL;      };     callbacks {o' 	XmNdragCallback = procedure ScaleCb();i/ 	XmNvalueChangedCallback = procedure ScaleCb();u     }; };   object radioFrame : XmFrame {N     arguments {g 	arguments formArgs;- 	XmNleftPosition = x7; XmNrightPosition = x8;;- 	XmNtopPosition = y1; XmNbottomPosition = y2;{     };     controls { 	XmLabel  { arguments {d 	    arguments titleArgs;y% 	    XmNlabelString = radioFrameText;t 	}; }; P 	XmRadioBox { = 	    arguments { XmNradioAlwaysOne = true; XmNspacing = 0; };l 	    controls {B 		XmToggleButton { 		    arguments {o. 			XmNlabelString = radio1Text; XmNset = true; 		    };5 		    callbacks { XmNvalueChangedCallback = procedureu 				SetScaleCb(radio1Value); 		    }; 		}; 		XmToggleButton {1 		    arguments { XmNlabelString = radio2Text; };;5 		    callbacks { XmNvalueChangedCallback = procedure  				SetScaleCb(radio2Value); 		    }; 		}; 		XmToggleButton {1 		    arguments { XmNlabelString = radio3Text; };r5 		    callbacks { XmNvalueChangedCallback = procedurer 				SetScaleCb(radio3Value); 		    }; 		}; 	    };n 	};;     }; };   object textFrame : XmFrame {     arguments {u 	arguments formArgs;- 	XmNleftPosition = x4; XmNrightPosition = x5;i- 	XmNtopPosition = y2; XmNbottomPosition = y3; ! 	XmNmarginWidth = boxMarginWidth; # 	XmNmarginHeight = boxMarginHeight;      };     controls { 	XmLabel  { arguments {; 	    arguments titleArgs;g$ 	    XmNlabelString = textFrameText; 	}; };   	XmText { arguments {n% 	    XmNeditMode = XmMULTI_LINE_EDIT;; 	}; };     }; };   object scrollFrame : XmFrame {     arguments {a 	arguments formArgs;- 	XmNleftPosition = x3; XmNrightPosition = x4; - 	XmNtopPosition = y2; XmNbottomPosition = y3;g! 	XmNmarginWidth = boxMarginWidth;F# 	XmNmarginHeight = boxMarginHeight;P     };     controls { 	XmLabel  { arguments {w 	    arguments titleArgs; & 	    XmNlabelString = scrollFrameText; 	}; }; }	 	XmForm {; 	    controls {n 		XmScrollBar { arguments { ( 		    XmNleftAttachment = XmATTACH_FORM;) 		    XmNrightAttachment = XmATTACH_FORM;B. 		    XmNbottomAttachment = XmATTACH_POSITION; 		    XmNbottomPosition = 50;b 		    XmNtraversalOn = true;  		    XmNhighlightThickness = 2;$ 		    XmNorientation = XmHORIZONTAL; 		}; };n 	    };B 	};      }; };   object listFrame : XmFrame {     arguments {N 	arguments formArgs;- 	XmNleftPosition = x6; XmNrightPosition = x7;n- 	XmNtopPosition = y2; XmNbottomPosition = y3;S! 	XmNmarginWidth = boxMarginWidth;m# 	XmNmarginHeight = boxMarginHeight;      };     controls { 	XmLabel  { arguments {  	    arguments titleArgs; $ 	    XmNlabelString = listFrameText; 	}; }; ; 	XmScrolledList { arguments {e 	    XmNitems = greekAlphabet; 	}; };     }; };   object paneFrame : XmFrame {     arguments {2 	arguments formArgs;- 	XmNleftPosition = x6; XmNrightPosition = x8; - 	XmNtopPosition = y3; XmNbottomPosition = y5;X! 	XmNmarginWidth = boxMarginWidth;d# 	XmNmarginHeight = boxMarginHeight;a     };     controls { 	XmLabel  { arguments {p 	    arguments titleArgs;r$ 	    XmNlabelString = paneFrameText; 	}; }; m 	XmPanedWindow { controls {e! 	    XmScrolledText { arguments {N 		XmNvalue = pane1Text;A" 		XmNeditMode = XmMULTI_LINE_EDIT;
 	    }; };! 	    XmScrolledText { arguments {X 		XmNvalue = pane2Text;X" 		XmNeditMode = XmMULTI_LINE_EDIT;
 	    }; }; 	}; };     }; };   object popupFrame : XmFrame {l     arguments {  	arguments formArgs;- 	XmNleftPosition = x7; XmNrightPosition = x8;c- 	XmNtopPosition = y0; XmNbottomPosition = y1;x! 	XmNmarginWidth = boxMarginWidth;A# 	XmNmarginHeight = boxMarginHeight;W     };     controls { 	XmLabel  { arguments {u 	    arguments titleArgs;;% 	    XmNlabelString = popupFrameText;  	}; };/ 	XmForm { controls { XmLabel popupWindow; }; };=     }; };   object popupWindow : XmLabel {     arguments {s# 	XmNleftAttachment = XmATTACH_FORM;r$ 	XmNrightAttachment = XmATTACH_FORM;" 	XmNtopAttachment = XmATTACH_FORM;% 	XmNbottomAttachment = XmATTACH_FORM;t 	XmNborderWidth = 1;" 	XmNlabelString = popupWindowText; 	XmNtraversalOn = true;o     };2     controls { unmanaged XmPopupMenu popupMenu; }; };    object popupMenu : XmPopupMenu {8     arguments { XmNtearOffModel = XmTEAR_OFF_ENABLED; };     controls {= 	XmLabel { arguments { XmNlabelString = popupTitleText; }; };=B 	XmSeparator { arguments { XmNseparatorType = XmDOUBLE_LINE; }; }; 	XmPushButton {t 	    arguments {! 		XmNlabelString = popupUndoText; & 		XmNmnemonic = keysym(popupUndoMnem); 	    };B 	};  	XmSeparator { };c 	XmPushButton {r 	    arguments {  		XmNlabelString = popupCutText;% 		XmNmnemonic = keysym(popupCutMnem);o 	    };  	};  	XmPushButton {  	    arguments {! 		XmNlabelString = popupCopyText; & 		XmNmnemonic = keysym(popupCopyMnem); 	    };g 	};i 	XmPushButton {o 	    arguments {" 		XmNlabelString = popupPasteText;' 		XmNmnemonic = keysym(popupPasteMnem);  	    };  	};; 	XmSeparator { };X 	XmPushButton {  	    arguments {" 		XmNlabelString = popupClearText;' 		XmNmnemonic = keysym(popupClearMnem);t 	    };X 	};t 	XmPushButton {  	    arguments {# 		XmNlabelString = popupDeleteText;s( 		XmNmnemonic = keysym(popupDeleteMnem); 	    };{ 	};      };     callbacks {f< 	MrmNcreateCallback = procedure InitPopupCb("*popupWindow");, 	XmNpopdownCallback = procedure PopdownCb();     }; };   object drawFrame : XmFrame {     arguments {  	arguments formArgs;- 	XmNleftPosition = x5; XmNrightPosition = x6;r- 	XmNtopPosition = y2; XmNbottomPosition = y3;=! 	XmNmarginWidth = boxMarginWidth;m# 	XmNmarginHeight = boxMarginHeight;X     };     controls { 	XmLabel  { arguments {; 	    arguments titleArgs;a$ 	    XmNlabelString = drawFrameText; 	}; };
 	XmFrame { 	    arguments { 		XmNshadowType = XmSHADOW_IN; 		XmNshadowThickness = 2;  	    };N* 	    controls { XmDrawingArea drawArea; }; 	};=     }; };  ! object drawArea : XmDrawingArea {l     arguments {  	XmNtraversalOn = false;     };     callbacks {u, 	XmNexposeCallback = procedure DaExposeCb();, 	XmNresizeCallback = procedure DaResizeCb();     }; };   object drawBtnFrame : XmFrame {t     arguments {T 	arguments formArgs;- 	XmNleftPosition = x2; XmNrightPosition = x3;p- 	XmNtopPosition = y2; XmNbottomPosition = y3;T% 	XmNmarginWidth = 2 * boxMarginWidth; ' 	XmNmarginHeight = 2 * boxMarginHeight;T     };     controls { 	XmLabel  { arguments {e 	    arguments titleArgs; ' 	    XmNlabelString = drawBtnFrameText;X 	}; }; i 	XmDrawnButton drawnButton;      }; };  $ object drawnButton : XmDrawnButton {    arguments { 	XmNpushButtonEnabled = true;n     };     callbacks { , 	XmNexposeCallback = procedure DbExposeCb();, 	XmNresizeCallback = procedure DbResizeCb();7 	XmNactivateCallback = procedure ShowCb("*showDialog");X     }; };   object commandFrame : XmFrame {      arguments {t 	arguments formArgs;- 	XmNleftPosition = x0; XmNrightPosition = x2;C- 	XmNtopPosition = y3; XmNbottomPosition = y5;      };     controls { 	XmLabel { arguments { 	    arguments titleArgs;t' 	    XmNlabelString = commandFrameText;m 	}; }; 	XmCommand�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             t�%                                        ��  zp [F]QMMZEQGZ7IV?1                                                                                                               �  �H                         ��2     v       !a`}KWSL�T�3.z`^z*
]in7+)�XUSqYV]&B:�RwREd�	D�gPgx{]IR6�cq$4xW�@d~y����� YuxSi
C8�QG/_Pn$E`EQeFc#FNDx `+!+]M��u{|\.`(o7vF^t˭JO0dg8RA]^gE]iA,w#*0x{*�<Vav`T):Z9vbkt+4bD����0{eiU.V
!(Z(���-x@U��;>l4Z,$$'8Cv*RiYh77]H@0+()	j{63k+4Yu�e/HYտk,-SJ�4��^uNg���1{>%{.&z:W%w4=�]S:RK(gdBG	9� �q`a!�Z* 6�Ӛ����/2-uw��?����ELzc��@m(k#'/Y�a"fsX?��&1Pj6u��P/+Ȟ�sMYY {w5�!J9#&N1tSonePcc_G�_>WpU*D5*yM%-��,=#v�	 =u�IQB[G8\�I$H
9k>�un *le5d?T_C6T]��[�hO&gor�6��?��J0yrUu9)'#c��MXZ$��M8jrV^H&��")�@_��B}1Nr���S&�1w)CF]Q7?%C��eBIi R���BN>��2�ew$zHV��!�] N?5Kx�w���q�(;�Jc�xa-�=< � zG]D$%�=8C	!h[sD�Z:3/ZUg�jy�V2	h}51)�9v i,&yzw
w3ig��w�NzP_5Q�X���{}�Y DL{GCi_mp/@Nrk;P���3\+5tBi8.*2R/��s@:��9jVml@ljv�'~"*:�%W[)E
[9kfP1ncMs. sJMXai0sr(\7`IWdF"o�󷿢S����ެL��<�����$ٝ���"�����3����D�<��lף���AЀ���p�NR��lڧ���#ȸā�_��������J������q����%ؼ��r R�P4֦�L��l;:3Gj~{:23��l»Go�R|FffIqqUP@)(7/?bin��,&M\)X"pV@Mcy1j@:o\yM5JZ8S$S
wZ[>l>chwJ�wQA}%!$\���CnI3	zFa�C�/�e�y��}2QP��+ķ.�+a}`O?@MR[a�ɰ�)\�Jc9y���dZT��
�ի��r	2vV D7+&C/6Z*���V�lp��#u�V]+U��:Z,dm8TpB3!V8�6Vun|YFLz7`>4{>���Wm87q8@6#uz��MH-�}$t"8dm	�cG�e
��fs�(~(6 v������Y��Oh��xfmj�)��/\xe o\+!Xf~8$p8DM��U�Ak-)=B"	=	f��(Ic0\� ��7S�*��}^W�u/!�I^p��) ,	{](-_e9ZA�C*4�U(g�(L#
bG,(�Z�c�/�����puQ�t7��zW��b58p	~Ki�) �uM)a= 8M`Ra0*i56!Cd2AC�"3[if{`��;;p\`u*nSU)4#/l zR&(l��Wi1co%#N5X]=cJ:Q-��^e!~jj]t[Ys�Z��"o�;HO(%a4}av�.O��XEIJ6AJk/0�[��lkb,c$!wm raK;bC5[d%(
h<E�&`D f,jL<�iA
X3n4=:g?����%l}*kgO��0-����9Wi;_7�+�w��;@�4d*1 u?j"f ��{<-l\;]-X�X�aG*e?Ng"QGD077Hb��T5.x:4dffrnIbV?��Iq1y,nwjk!��%mj�K|J�JI
J@7�r{ZX/uZ(lFJ��`EFPh pjkv��v}	Q��K�<qF[Lm/oV	="��D��͋�Cn>_x*I/����N8	k��~WuB//Ts0kYyIP�lE��B;@6huY/j�\�W�gK�zU2{0Z4[vmQ\�� ]R-yMR<#% �4��'~Go'tz"_s#ji$R{y=3e��2~`��&+䏪�S���K�^4�A�RV@v@r�(aWS3�2e+t�v_~Jn_B\ ���9�4{.4m>(sN-rH��=-r&C"	:S`Oo*YT~T\}`H]8Se~3s={f,2N(L��K?PC �/lUAE�~ϕ�R53�== hs`6~>p�_	�(5��[LRFP~�MkY�B��yd^(FRf<:LF*�'iw\LH]]MX�=d>C��1NHIDAG9}+U�\��uX]"PH	wd%	nq~G5vC'PP9_��iL)bA(A@wy8)N=n*��^i#(#D([4yb@Ci=�N�N\YEt-}J-r`{��\Qq#T	tf"c>}����0j|�Njo�3(^�^Ml�t+G�9@ �
c7;BI?h'x2-V/U{{B91~+o|nv�e<dmDj�O%�/Ztf��S%�ZV��(qx�9|�Z�f�$�L��ZFji1��b^#)8?���@d_�7��Tf<\ <*bj3fkp$A�\~R�Vy�;(]l�$� M~�b��Cv|�4֖�h_Vi>BzZ4*3p0cXT1rk|%V8d?^&�0Crma-�QN>+��Yd�Co�S�H��!�^>9�L3A�@�6�4jsBVS\0?}MYWj"rBF.@I_m\U=��p9�YtY u`f<e��]C$hY `�	(m�&In5Vi�+|V I��"3i}P-?&0�l�s� P�YX3soy[rR"ir��3�BA,n{40uaGduI&��#~7t7�^i1�:� ��@M�V+D8H<wGUxMk�nL^j69f&@^SPO jzI,�8yK47A=dNP1L[ :JT�l��>]-E^$3MMJ2C��0l:~<b+G-4V])O+0(Ug/�d$��5E[BO|6�L{g�Gyl>x$��B#B(*M@=-l*~�F��79[KZ:lcC(x9Buc<>Oe|RYM�]%+[!\Z!S�dZ[��3i*d>wP1z?�Q�"��01�~)4aZ .;W
��n>\k[c*�9 7�n,�d��`8%*}b 	'~a�$��@`n]^]{'lN~�FEKB'k8qB)��e):B�Wvz:,jYQ�W��,) rr�'}}Vs3"RQs�T-5\�q.�{.8�`ex%V1Hit��7(wyNZ+bDWy׹YK3$* p"fa/?f/k�yKf:�{?{b.;Ba6%xA�K��K�FJ'
�H\A.�cĒX�Tczp
STX-�S42|R%YNo+�	*AB;+#Uc�!I�j8*a,9!*f��'C	~~`jG-%a!Y[<�t19r1IN"j9AODJ}X4��s0p/$=<KNh'>j��1gZFW5A[~2��1j *0&uy5K|H;STe6fX�����bY	t-a 2�/z���BP-z�W7*b?�b<_ #H]�xwkw[]|i7
DT#(uaNUj:qaKfY p]�34s�n+�i.7o'k%�q<c}Hv�)fv{JBA#dx�M:�mT"}z�v>|�H@;k]xgyXu]:9U}v�O0jCHsXcQb'��NASET�~f!t703$�Yzk��/���oFm`�'��=�l�K6^pGw1 �L_n�tn�@�h^�~c�3Χ@)[RJKPz,E(1J�Q��0Q
}}xk0�\r�=Lu�Z<{�L+��RI�<#Y�\3#ez�C �_U\s	��MX�E"�SHO�L�k�o,vx{@<ih6�KiB%}(=*+k��I ;ORY-C~uM>vr��;{(VT{F`bgr>^d;%QV0	[k2;xs9��v( nJo9SI0W[`��y|Mggg<OBzxe&5xoq8j0i;T��.t4>_U!"j#FF8`tu��LtD]gn0�Cc����u�N{3-VeSfMe$%9epLAUrS7��=l$� Tat^0�kY�x���t.S
sF0YcC?#��B"v5H�$.;bT$p�m EF+�dC`VQZK,{c37t_9wBWK'B	z@y�Smo)kWkue+J45Jn[�u��r$~�*4:zl|W |���Rd&[v�V �gDrbk;h�0Q_;X�3xu 5
 *��=/���a:L���Ɇ+bg�z99_�+fD�m�)r"tq i�IM�}�{�NG4�obl{CPKsOo&WJNhsfk{��/wg$y_j\,c}}�_,pX$C!��|$K0~qd@���Q�Y�J�1g8uu��2�&{9\zq[B7�BJ7�z,�qⲹ'9�; Z<�o��I�~{iTj$RLUT F�tw��54&r$�?�̕� z��̄{T�2I0�*h~�,|o�doHO-&Hn$p��=mzQ	\KŐ$-8 &Sa�n^�.2�!�u�/�t�'<c�zC�9�|?�U�GO�qV�j,ru O)<:nYWpg&yC	iJ�O:Oq^�y_Pym}$�"
	#���s�n�bY�z7*|
w,3j��'P�pv.��2TRPs;hS0A[ 8g܇Y{"=)]%!'aF8
":(O'cMX8TNe�E;r1r��2~& !	!;��z)q��=1!:I;%"��{% syu}MN]NL�ܴ�alDd&��h` JF�GX�=j"SEYph/.+jn���� DX& 
����>��e&\8%<h]d :]%$n+x-/>U�}҂LJ~^fB�ep{@(dE�EZ�maH,Vns	'p\9�Pjc�3Y�(c~�Eը19#+0#4\
Tsy��Q$[d8xJ>QZX]uty"ym<��[)�Y=/7r �
��'f[bmO.??wdp�0_KJMMK�JXk�wEƹ�OI;7�5DV�MY~�H��ylnbrue~E(80{lmiG\_Woh<:+C.T)Nd%
~p��)
l9G��9k���]?  N�SʈB@kIKW7wImd��"6b6'#Bz3}K�y��E<\(M@1�lj;_Ub)#dNdDVȦ+/��NiR|9t
��Xl��4Xt3_/[HD8j[��<H-re,zrRP��ezQmw|o�i�GMXw�LGTG_ ZOE|@fFJ��L"jy��{6z�/�F��g٧P)c[0HyX�3��YjSdW
s-~6fpV
Y"'�ۀ�ao�G�BFCj1F?o O_�@Gz^]HFtV\v"�-nd� k}b.- v"��j8e%x�p�:�E�w9.�cict�gP9qU9OX@�P=�0>>9MJ/G@P0:_HdE]U�):H�h�EIgn<�f �+�l��t_�9uM�pZz�W|+��TX (sNl{P-Ni.HtWbo57"$d�)��_L_/�
�rm>�2�4�5,(o?�<h1��?9rt4Fo�kB#uO??��HS�|I`�^-Q,'(�VN7�#"-��R�}�g 8�onp�l	*�L�LF2�0
�~�3&;�'�RShpx�&B�
!(3Ey3I�Pdm �*xO4�r-,mB�g;b�og �<%%�&zIV/VW�O/vd�TN=�k�%dVXr&U:�&�}9�"�4m`�:7ab�F�+\�Se�5�fre�l$4�j�qnw�m�1}EyCZ81�T�g#w G�4A-�|Eg*$�F\|�J� �|6QP(�_	�#mID?|aE2G`o���OB�	T
�~3 >�
�1hwa}n��QyV{'xY"C�6]c7o�Sq�K(k�;�Bv�R_�,Z� 	OWbH+��6=
VAG|Ai67k	-0]�6g=n FC$&8'`O@fW.n	PB05e�QPe7nh<:|F;=\\by�n\&!%gw/{%2i2^uhHjp~4jN}&:�֒����9^ u��ݬ��XYfI+&02	bte;HN@U`t6~^x=Ci,:8iq��Yp{<%L5j�XS�'#�J[6b`Z�S?Q�+[ �R*)B!%\e67d7p,:8tr"Wta\uN,l&1L
 @9$^pyrf&*+8zFH��~'a[:_U$a/\J ��]
Uu@)@0
]Y!eX3isbR>5b>;��UK@gdS*}J%JP?;j>Sst2z]Y��&RU{<ADO(CN,�#��:.-wZ?B$tp2>S`/h38{R\}~%{43ZnT?/C%AUlSs7_"6n4*hA>C<Y)8CAo5Nnk=_eb>]p=+7v-b4XO}HKaGI-�|`!�Y"�V~�Z@V^i[K5z-)64G)AvfUD	4Xja'9om>U(qQ0^
F�P y`\P:�k\�,�7ta"=�9�xo~�_#�yL5uW^sCP/( 2t<~oze3L*:+U B8Q/4zhdT	vw&yO [ggT3��~N.*sB$lG)� ��~2($8x>LyB]Y\mTX&n{�ApLli��	:k}-pZxyi9[?�8sNM5{%30E On��m$M"7LCE1#LF�Qώ�nF58\edPL"D}0	"#*R�}A?-6TOB/Hm?Hc9?Y}V1W&=^d;qkc^-<!e2D� }Hp?je0- 8"[U$('DXb�jwa&757|OTDzwhqjSpc,[^��1����k_W
�������LnVTA`pU .0 ^\Y�*^?(+X9�05�dX�#sZM� `�y�f<�FH0yKPUF-b34n6YRbV:~)Gd_m\s=lB8B.)m(JLkc*9=AiVW'c'&aX%\(��DgG&|YV~B��I(JD8cB	w+'_|uR
Yer84@��vr2Gdl]::hBlW!U	:x'>t,^�s(5P:+{kvss�
h{��f6�T+VqzkP
&�*h3iAj3`h0[qHC|a UFo45f'Dg
@TD,:5P5"zn1(v{>quLElc-]Sz+<n\9Uq%gT/-xjtP~F% *<>4JD?{YMKm*Xz% ilI &1]N>#&:p}-`ZV;5'SU=X2I~l\,F	h ��_V XWYtZR@\B�r�� #$^	* GMK*.Y505'yOVJrjl A@,,yP=Z`t	!K8iYZPbL`?$8nRH"}9
;Lej>e%5|35%g-i7.bF&B5l*;kLk&_8
Ŷ@%.~K>d~kJaX��2NM&uMzoZKV2S<"IS?%v5ra��kn4Yw|EX?m�P��4Kn1$e+F[}iV1/R/=I':.#><yrcflvt{llV`R1P*NUpN%pc\%-V2��]FJmU7;5JDN;�Zb��m]�?"yoq ]x4Sz}'I%L6?9 L`m_}NN)!+[SL_lc3lOS~x)bh=v
wl\_wP3ifv+l=x&.(6`[3_8^y(Y
XIk6\yJj.0.U(d$/9&A��}vyB'N'��95��]�Q'+{KC/uf1�o>VDA1RMj0d@]1(0F��pL)}]uuu0GP1&a!$7`N!`"-;$PAWL{ Ru46!6Ij)Av0&R^x(IshUzg�}LnYLdmy7B6V`YrU�1L<pCcKe= sy{Q*>Q~S5qu�����"VW��X-�<?,~4Y#% 6�'�Fro+/^]q{IlH c��DR�[S_7<M;-G&fVe�}[mYdrqxOCAh6H?z~��ӭ�_N>lX5���nq*3/{2)6\[}cR"f_eizX z��PEWi^	XOR|���s+pVSY+`VzOK  zcO%3sAZ.4��]0 T=9GPhlL0̷*���Џp-InQp5EsZ0�2w1cs[i~XS2m� |9|X
Hc(*#([TLFx/SO��5_�lT�}\`I1LA][M%mF]o{FNA�SG(��iQv_^SjC�*UH{�kdDjew	ld�K!-�,Pr�g�?p�nnoffX@xeo!�*\dGEe�|G:�<^�{R�
G7#WB\m8UO;o?tly:��%��sbY�<j{�\"Bl��+*wd_NP(! 90{{zL4*1u} ��|Rj>%L+QPjD'nJ��?]}Xqk07%bmLf>|6Se��pTDnk'&W.w`_?aj#Ewd��v[������ݬ��ꆁ�����4	;JS[nm��kp mG):  rEw+LQft=�� e�EeBr{pp#*&Is$,d7)��]{V@{T0=JA8m]$��"t	J<d<6s0}r&3x
����Ìx?��ðح�������դ�к�Ʈ�˙5
�Ѓ��������������������ʿ��@D�����ɓ��ߚຮ��������zͬ���������͇�������rm����ǲ���ɾ��ڑ����ぷ#\�˩̒������������Ə������,6���ג�ӥ������Ӣ���!�������я��������ʗ�Ŧ�ud����涖��������ꉸ�����������ԫŦ��ٸ�ڄ�����	W������ī��򁮴��ꅵ�链�����������觾�������끷��q�����Ơ����ͻ��̓�ӇǾ�Xz���Մ����ƾé؉ڢ���۵Qʮ������ٵ�����7WD%C��b4PWh5X(�FBǼG�NZHvjcZ]5�,Vx^3[h��.Y={iQs'
r(ae,t&ZQjR^oi>dF8(r6&0JK-9DO
{
gO(]S>!'`0H a/(c�^IH
c|$/W&S0\o!!Qv/@,�}8"'i[yFC;c{bgsIUi18`FF>uބ4�ΐ�gx�a젋���6a;(OV@;e��hS|23WZsWkV7F��O[��C[,9�OVj\wS.ub}L{eW%ffJH�*D^TY}I)m_ARU^zNH���ട��;FL?0	N
���k,a'!amg&�6]Ul^N^x_}>FrCSCI|vCf��`k{k-T$N{L��<]QZ5F2|N3O`>>rMqfXkS2</֨CO1ObGy:R?xs<�������Z	u`B(K}ArH/!##NG H;> |�+kf0B_u],Sa�662@~iA=5Iu-mS,;/I{V��;PA�7�nAP\:nE'���6gk =6�u11.d7P+�n6�;YN`c=e#/NN�dM;�W�	6<WNX6&<eS+Q#N�s5}�S��:T�fsHy!&baG�&��h:z�2gE�qIn�|��>G8*J<e��e"[Se]kJ��Dc=�.�qQ;�vQ��\~1	<;/yC}�g[S�.u4�'Cy�x%nh</d!r)Eh�r,x�g+1uA�_f	\[qj�'n�fIt�Kp�@~y�(s�kp.�=$m	x���PO���0��������n��eAR�!0� ]$]bqh8bS5��<lQB�`ZlX;�8�Z��]H�^6*5)1,qjnY�[�,�*)5 )�0kk[kGb@�,&�E{3�._[Tm�`i )H6%ke^!bj[_]ySTmW\8butnRPu);2MxKJ=tO@ipMS,% G[hHqfIUug'7�]?A8T|-{Orrn!#�64|]�����ة����˂��V~������Р�.����~���ҽ���Ï����ۦ���ј�ړ��Ì������̀�������|�ֆ��������Զ����������ބ��Ƭ������Z�ݞ���ԭ떏����Կ�܁M�w��z�����Ɉ��L� �`��k�����2�h�����Ԝ�Ǜ�ڞٶ���ދ$������Si+e�^�ڜ^�*�-B�]ə��ӊiԱ�h���Ε���,��|��������L�ĝ�C������%�榟���ꆇ�S���d���������w@.B'kZ!F)�p����6�����I-XNBKk���DBFplxtHat2#;x+ EiNoDC/<g)Zl	KH3eF@8|=2VGVqT|$
bq[1=|AhM@Jm
dbvzZ5b#KHNfqR:Ay;R!�H�΢�`v&TPKsHT4lM(WdA y��WP}Z_p{jA1b]� ��������ŕ��ꨏ�hY١���ߔ�����R!������������?�>@�a%OH����}�1���ͩ}��,���v��".A�1�gKne�	��Ze�r��"���%���e�z��Lqc:�a�-�.ab���y���l�`菶ENI� B��wzT}�C��,���%�a��	'�i���jv58�7��tZ*䶪��#�B�2i���6�x�#��o�'�$��BK��v��P��/tU�Ӊ �b��)&a�A�۫�������R�I��v@ḍ�3��{u���(|���CT��D�����BS�k1���I	��md :*$%�b)�6a��/]�,��rnZ��B��O�g�ش#E�$��;��'�A���v%�W������\]&�:��c��w&�bR�;fm<����N�אl�N�/�m^>܏���t
ĸ N ��
#�l���L�R���RO�f��g^��W+4�P�l��mYz�8��y���C}..���<ɜ]#�q�^�K'�rڜ�9��A1��l �_�o����+Z�wIi,���׷|D�辶D#�OQ*�Փ�-�le)�Z��Cwj3��2�㇥����v��β���6E�UB�A����N�� ��Lx��;:�sKk� ��s8�}�ĮWaԔj�q�t~b4���+��S�.��KixV�Ǒ�q��#�dgP*쎞�b2��S5���m;e,�~ �����9ӹd�Ϥ�$�!���������ő_3����(��4��Ч��9Ҙ]1(�&nt�t6�ig��N�'�W?�'�_��}vnoZ����aᯠLn%E�DGp����,�-+os-a|޳��\�nxn܊����sV�����\�����zh�W~��`�\������l��y��K����ojD�#�#$G�j�ܡ%��\g�X"�J�:& ,g�TȂ.dȩ��W�$KJ�a���J�d��J꿓tia7����2������tN��Fٛ�]}|EJ^��8F�@�z#�8y,b���B'��ރ/�ظ�BUFaO - ����0�	,]p֦Z��n�R�mwr�<�:V�~�f��t�`'��>E1��	;H@ո�7�����iv W]p�2��;7�D.뚱�$>w�q	~�	�OQd��f~J��g�^7��O�;H�E�j��)�Np�(�r (Jr�&elj�T�����JLc�Kn\Z���n��c����Pi��Z��E,N�Y�xyH�q8]�z#�T�_��,}"�K�m�.��C)�t�x�N??o�U�n���[Q1���W��1���9���&Z����
dD�'��0�KO�e�$a('��j:a�@bT���9UV8�'�`�58i��m�F�&6���u�S��|3�"*�m0$���9�"�3aiHA0�57/sBëW����ZD+t���Aߚ@�V��͏�T��"��k�~)�a�\�S�� ��Vܧ���ׯ9�Q�.�2��2���sX0A
�c|�����������nq}$b��(��� `ӿ#��ah��f�GaQ@=��>sz�xj�避�4*C��3n��M�t�U����葁}'�Z�!s��=�0�loD��!D�����ܛ���.:��Ǿoȣ)}8=�0s���-����U�fV���{��m[]/��|r��o�����=��.�	�ƒ�@�W��B$�� k�
M���]Ԥ���˰k��.�̗�?����술����Շ	U�~�|Į��ن�.�5��ĘA�*�g�kcFj�jofu�
^o�/�*:�M�]c�K~�Tm&�41*�YRY;3�+V�44��E��838�w� V�&�J@>>�~F��^�@�}r�1w�{��%MY�*"6,�qt=��#�Ez�w{i,Fz,#�r�^q�,h����Y3�k$
I[�v���뭪�;
{1NM��Jf:5����1F0�/"8�H.�U/�E|]��6��~��'��̏u��4�b�5yk�{��5@2A�G�P��h�E�;H0Jk`����JkÃmt�%�����`v���N >;�,TQ�ߵl^���_Ë����t{`'�3.'�k�i�*����"��Di�#��Y���At��|q&��Mm���z�hϿ(6:�5��,n9�EZVػV ����~� .h�X��0߬=���A�<��{k7���?�0�:�|�L�>������7���b<S_��� ��S,�|�'�]�����*5b�9�Uq�VLa�M]�:�s�2*K&�.�s�iZ8|�/�)�Ob�[��7��Nk�D'm���7��袥���erR�8�i�$N���`��Dv��[^~�����ޔh }���ӕ��⏍B�`��lyQ������Ȧ��9������*�ڥ:+�Mi��k�����q�枘q�X�k~�~�c����┕��+���,74�:qgO�7}qu���SA�IH���A�r�JF�7O%C�14b�c%7�\$ 1b`�H$%� ꁃ��Kx�C��N�r�!�pu<�4bi
�_9lldpB�Dd��۔<Ą�]���Ӝ��\B�Ո�VU~�w�X�R�qZ�G0R��a���)�mg}0�M}�K!�V��g5�aa+`�7aD�t-�rz�}p�EW�s<T�f�8�2�^�BR
�0�4�}X�:��[�m?�j<�;J�}ԙM� ��T2(�.�TL�0��5hm�_K�{^	���l�џ�uH\3�ho�xh�O�(�Q���9�T�9��"��f"�i�P�/������j��YuQv��ࢥ�9:�z��!I�1-z�fGX"l�6|��w���#��a������I"+B��1O(�FP���d�`ə]1�w�=��1s���^K?ߕFR�K�g�`�o��Ç(.~�MMq�``�^/>�H��o)���ma)�Z��#k�a�d
7��`��NjdO.'1PEUKi	fc}`S.Z�vhf7VY<L_~]Tm�P F>,/*y hQ(-���Fi�gA{QI( `M4%ng6 ~Pe_TUW2E�����W6} �ݔ���7H(@@}\5b$|b7D0hRm"50]Ks3VC8\ ��m~P�S2`np~p@K;FY?B�WS��~�=@� y�S���~$GV	E+5,u Nsm8(z/��Bwl�|.�vh\���S�X�n	x_p5.?�J�[8�|?#�yK@.0P2G�P�M+�X>>�;O[xV!,��oy�w0:/2k�^e~PZuQ�g  	B&B{PcmK�ZF
1Mw];32b7*w*+�hS�?d�1G1ByH�lW�p"��- �B5=YG)p�-n.#8/0�V3q�af�o�=DU5
!Rg�f]G�)g�hO�`46)'8 Ph�&@6�`m;?w=�4�SbGQ/jgє�P�i�M[P.`WB_$l�Yzl%<��J�OE�,RN�v���n�|0$[��& g�.VY"�(	Q Pj= WteRnh�&��l=)~~j;LVx��2YKOV945�����ø+{EuOԧ�����;":#j!n?Iw"?3TI+$8Y$-j
<G2i4�XT� Z(�s�]yk-q��F1;?uMC�hxuP1�se	*ZJ�W'0>c6}�_zo)M562ViT<s{?s3�uV.uln&T\jH"r[6mp`i{]~n:*|~e�N2zvDqb�4V#{^,5H_aiE]B!
]<R
$1c{OW[\9+hIJ1e!4$.K[`-yjG3`xRAWT�VeW YW�$p�!Brh;�z�HnR}W)Jyd0��M|^(KVe{u<{�>��-D!+D>7nn<b0!Y�!xBZS�P&7��p�#s\7Gq�Z`M{T yʙ^�Au_24hs�OsQAq	u74YDVYcSj($|��%NbEefMgdB<2J=
jI&]EdcjR3:q.Γ�D d��'3]E�h����Mt�(��m$X4R5g?]ef.n)&$/}_PHcWw.GH0��e@wL,w%L3���A&x��5f� dI�~��.$u\�em%VN�A5DMkmY@Xm|)mUT#Qw@uCs1|4"tr�>I}*yGP6�<(QRw�tQH1Z*[��B�]2�}���$�x(z��O���:�� �M�QvmQ0�X'mb&B akt*rpAhYD%kb'<#y�]E[J/P�|Y(%5yU�wA(K5`<[NBx;I\TuUk�tmOQqF}hr'8DI�@u"RBZ�8r r^�"�}f,3}dS|X?N_X�:�w?8Id�$&@lQ`fbf��90hmk��em:O0N&;o1F�Tq'��0PHeSyB�-�'���7.+))1\�u=lV_tM1v8q k>Z}HtI*1[W"TZ%#+&*#&Mj]e|Tsdj�G��2:OD�^`_Fi�8� 3B7_?'QeF��y���W)�1 <�.fZB.u&*> x([V_Ir8`?Mqi+V'.!<BN(^W�yC^:j)�H{�agM�='
nqjJsT�F}MDN*\W^g@-"�3�%��{)�$y7JVi=ko\�^��g)r
svWV4
b[HLG7�+jZv1�� O�G%Y@aaJScWG��X x��F�u>kUP'G5��e"qs b��lR|XCs_�4YL_><b~^��	R! n-dL!Pm c�yj|VGD"X4Itst�WX8:�|{Y��ILM->q$�g)1[M#��-nn,�g�����5ށ�򦺽/:f��awZ	@ev#bMjhL`: w��h|]%e���u;{ ��4?6'p.�9'6��WUzl#d
	\u;���*KT&JT.wp9Ybc-gWv�F/W	WTsU6BPL/j@@@2a$tx@M�J~ds$qwN@(OuSWxo`]RM
l|6�-0!wFjJ.R,t6	J�'gAkD��Z*4r�vig/4#v�R��>0W�+�s�S�
FC�.<S�-x&Gcr��Ww�ohyWJR:]OeX^��8`�J�*FHJ\m���,d��It,IYt6g.[�|{qfH�a9ؑG
�ef1F_i�]p%�DǁǛA;V=5ox` C����JTH
.*Tv{34]2�A"wAC3|lRl<5;f5O[.��]?fWCk!@I���}e_zKnV ,l� g@ie�OKV��ql\E
wc)��d�=��hl?rZd}[jjo!U+NcVs%f'7Y��.;" $.4=,
uc��Q_A4/&�WWZHf�\G,7Y,G�lM�l{Yj1P~%5k|yL$V�8xkIL�LUR
(6	4+,)T	aKmvJ$vMu|/2@�-�Zp80��vI:~671F1#!�4ݑ'C)`�bR\,JT6B'':a^�2��.4�1<8xN4m2�"���f u"Q� 1dV|dX_po	K]6�;hkL"E%�#=K@/A
�7<��hHs9izh[ijk��I/�s���c4��ٍ拏٧-D;hU�k��<ayU|pgI*=?iEd_>�D7uHY{�Y�"1W� ?` ėr*ZC�4��WږIswVђ#j.U&quђ�^;,Lk;{��E6"-#^\d�~T\g�S�uP�Y{}ro"fA�Wa�LDA5`?��YMcVwB�FB(�� gK	Qpa= H<`qY;k��%h= M6���NXO ��8  :@_�)!jru[$N5t�;%q7z(nwWDLoK��ـT;x-^IRbSN����atH2l2h"gVp>�{����|f!~umtN'����VFI�oNscK#;!bu�0PrLH
�j,l;�_^,F8f>F�G4Sg;����G�Qleucs$0kdT��+z3�b )T�i@SicT/<+�.B��W;F�yos<r	Z�s#@M=~$&��4i%hJ �cY/~� ?�zq#g�j��,i(23>t�DO8�LdC5Bfmm�vU�X
FR?{x;jx%E\[nh�T)b HchCaA	]/Sw&70O*&v/O-ry�(�R�M?�|L)C�MUb24;m�OI!��OA�9XP\T ��+?��w<k [�7t8>lhmFtI%IH:��@a<#b@]650x1�/iv�|
n75a pMG| 9e�@�9H	+FU%~|�5��0N:1siRE?GN�c��� D�KPZ{`Zb�+x�r<�ah�F%{�_Z|U~xd�lc��GIL*01/&4�],�Y%\�I[)h'`3Ap')F{���Ez^p#Bh{�l5MB<$v�Dv
#n)7	V$L~T�ʕ
;UM�	(�lt�x��M$lTY~(U4]�4\@^C|@s]9<D&eM7/��]j^rs@U�B^��$
�н,``b|=�/��@ve[rE<MqU+��L tGj�]N	w'wr��0$h&`gidcOhQ;ZxxQNJ��r_��o5�F>7��S��cV�"ow~Rfnxl	*@V]U2S��8APgL��ZL1�s��`V%*@Dm��Axa|i]%3R"<g,d�M��!)j<7E1^+li�?��1fR=s^)�^[{��7z:�bnAs\3q&���wKI`dlVZ�%拄�3Z�0(G`k$��sP���yUX+(o96� ��
}��nE%k 
Jt~9>��wm01�DTG#h)�q47	  �Wbf��'r��ZM8�/n!k9�Ra��wN��-h+{'H?i�m61FUtSoc v:K&G	!9[�pu)H�k#,"5{}�@Kla9�Y/cF@q+CM�]'yk	fG"7$%~"H!�D2_c">?�J�&��b[j
�*/8?�8�QVozWC?=tnNG*=�hOQ�4//B �jl~)�Ev�S0�T!|T"^F��Tt��)|u&mBy\C&]��%Ye	RF,Nj;zmU��Jl-W
"(3IAJ��	q@^ R*Z?^	D�.iAb6%[71:F|V-CcScҡU x~EB�L5���,�>LB"s QK?L\&ˉNpLcvdxUJ1W6=��Re2i.+gbPE2"5 ;$ as
G`�o~N)�e;OTYD�8���F��	�_)zGzk4>wa3>�S~I$-?|f \2=,~*l1�.i"�zQGdz7`1��1��{�UG�ILk�T��(�y#JP^mK1F#pJ/2J
qTe��QLo`z1�g
�s7.�ũ�_��O:7g�y1HNQKE]Y5nS\9-y03%ss;|-H5FU]K�ki�Gzho ,z� v�{N\#�1S#r">B�A=i}z8H,dqҶ�Un	�(oN��ZK��rEff{U|QRzުcLdsFm
lVSt=[Q�sW21%(4~:�s
s1�`?*"=�7vE'yj:C
 M&_��1bX.?R=hUw@9rA�H#UȨN(HE.s''7�fK]%odP2J@[v�Y}I��[{s�+czb�jMs6o�gh�!Y%<F	���b��66f!4�|�@��BS�wY# V"_<!~Jm B>��j�oo.L<`�;Ͼ/I
eM0*Ug68@1p#x+1PE?zYNhj{x�:1�<j7��Z�1c�g�,�3#z�$7Sj�Dhb y)�����l8MA4C^$I�W��>w2!OBX��h_~xGQ>X��}\D�NA)5~����"B6MTE106��h\ivXe:(=|��dV5lc<u15�t_�г6I�'kM	!0U�q0<2�Kd1'zvv~Z�,��6n;9\x)|bkYwSMI'�=g:{aC���Pu!!\Q=E�v�}�v+%[pKXD/3-�1=a��\��L$$`-#k�9^#lPvxKTN
y^1
:2n1j*16ds�cM//W=�+j	�kDK�V^C;S6o`�U1\I�S{hzk<p�B�Q�LMx(~;rcKMU�5#KI@�QEd,G[/#A�*����Z8-;7gDL?3nM�Lh9!'gF@ O*�(��H�y
:`w:BY`�:&J��vUh~"t[1o/Ro���3 t01y8<�E\��!�"h )n#@`=�vjN6ChZWA�>��&Tf$G^%^'z= A[B� �d=*s44#Y><e�1,g<\
]o;,�7Qhu0C7	��t�m2�lr3R
6K��khU.?OmgThL@y4�.*eKui�V3QN_%v�mJT�D2s.�*X-89#P
n�#��Pn*o?R&v	_n>5_f_FO^Y\,&IӨcCa~M" �I���ؗ8"4T<lw�Zkrf3%)�ShF#5G:�6Q��)�4u^NkQ0Sm�`
s"�XwVT,dUA�HR;|DXq_hlXCFtj8,ZHLc�OM}w&�~/�I��wVAY,63�h!�Kq0u=Kc'je?Yzd_7E|><# ?o)-D_�p,nI(bV_dbI�h]�#/ 
z5�M�Fp#^G�NUS&}`�R�X_Nm
nN��E7/@Yl�xDU3Ze��|=�hk7��#-laxEKH`Xp�7�ܗ�v-  #dcH*VZf�82#$=Nn<f[D��I$A bnfm>4Q#V↦�o5w]	ek^@*M`/0��Q+}2^1:��pS��(.ehTY8�b��PIEOkg[ChmH7U=U&A��7BmyA `3F��e?⋸��m_|z]kD3"��f&Hd݄S<Y-H _J0w*lF=f=-(KW?#��Xios!y�,Fkw��{T\d+
u�0bF3")	4QOJ�oH:s!9~�o,9�a
�0He�dvU�ljya�Q0dd�X2	W�4iUI�f_}�6�5u	QFY��+"zP�%91y�q>�*`7<?4*r'n�H�id�q]%Xz�qRU��AT cHA @.z%��mW�sI+:�c8|��F��:8�>��NY�	aO^b)s#�r		��9�D=�U�bJ;y��[-�6ga�M�,G8��J� )i[;y�iH�A�:���hv�]�V~�4r�`(���u�$4A�fej�j�i6�ciAS)�A
/`��4�0gE�]�Z� �}d��Ur'�8Zw~6lL@�=5Fe�D0�Y�r
,�m' N�J�{=c7{@��g�K�WG7nf�ET'|+^+ fG� otk�I&CH�q�W�3-b�2\)�fhGYw$S�R�W�Q\qelbB+�K�i�yUA�K�wdTh;6-:0Ia�(�H��<YI�{�bFae�hw6=xPU%b�
Q,�j�1IEyZ&5ci�1�"YyT��OV/~5�|�KS�~��2�Pu"�=/H��Jk�kHZ�ih]X�Xrp%&��St#�@3�P3-O��Wj,%�B7\��" G��(R< >)n��vDQ6_1�0p+ab]Qa�~D�E��`O�RBM+HmwC-h|��pB,((a+$Q�u9��t�_(2.$Ti4}cL8t=%L#IVS7Qfq`~D>d{QgSLd?��zJdl���ju8I_O|�&e?S��z_i0�	rfloN �Ghw�S�:z[xqvI_kM1A��)|wjqt)~;rQk&�A��Y~HTGi9
72
Kk�%XhJA���|ed
`"	9u:Y��~`bn1aLcJ3d}U!q_~w9HT ]]h{5=T>4%	anGq��W!g6s3_;gN_�w��&tx32'd9R
H9K3]hK-.t8#ga��h[4VLASL76pU�Zh��Hej!*pkcCw�.Q>����:b@b|&AcHw�|J���\@^��G$u;hP�k77Vd�o��>
4/`}j$oXNR+,"8a=#.��[E�nwz$>H58�" p2��t&�6X-&0c}p�oni��0 l$ni$�gOYfA-$�Uc*K}��LQ[M=<^zow%e1��s]S{90!5G
fP=dg�?	.dG@(k{~Xv����=&*P`{p~�w�@�%j�ru6PKzm�n'YQ�TklEsJ~S4 Q�$k"�9�1%]c9$d#L��*ob/Les PcWpT2	�5."!M /c01�&J& ��]#\	Bv4qBZlQ ��jge-lZ6m^
uO.>&�|wF�Z2C$�6TvQ�X2bMj�!K+�4;XL��tO5FHyq<#1z%B�]#c+Y\X?Gsp*XVw]Wwc9n�D���U��8k]mײꡥ|@f;O$2m*��h9.1}oG�:im�?��W��%t�/=2]!lSA(xC<��N/UZ
v8�b�k���im�	�>YeIBt���F���(<qlnn4Ml��9+\3 st\avXD+t{}.s|AcaC6|BuxX3:OjW>n�Rk��n<?)&\�uQl�Q��?C!3i�6jfm9AJd%WU�\xp g#A�z����j6�[(j!|L0:�B?͕���?8~1 8	][R6���8RgK?ol* oqE`S4+^\iR��l5�7M���>C ��O_t�|��y�8�U�;�=HeWYU
HS}1G}ttvZ��`w AM-x b{�&�� aLEA<WV�A0�m`)`Wxbga	V]n?@NViqZTI��NH|_T[QD8&H�N�ա"u^?el9wVt]i8+I,ZYEl[T}�g]tlF9oY&�9���m�� KlK; =�
��sQ 8Dj963jNmbx&x!/ypUsu��'	2 yl?#rz38[�!ڇ~�������ذď�����������3gNQ~i5t��LsOpov1!!f]���0{>s\qALG</�G(&'w=r(A
cAU��l/uni=sG7u}@�$v3oY	/>?,i5bSt<S( mhg ����ђ�����Ǜ�������������Ӭ۳����탰���ԃ����ڄ����ݚѤ��Ѓ����������僈���ƨ�������Ʋ��������ܒ��Ʉ����ӈ�Ő�Ң��������ө��ĉ�������襁�f@��Ƈ���ϲ�~�ؒz��v���슢�⧘��ـ��������ǋ���ꌭ�����ա��������0���ת�ƶ�j�������������ۢ������㚰�Ȝ�������㣙����������Ȩ����ɱ������ʤ�ｍ�Թ�˴����������̺���䏝���Ͽ��֛½��������������޹�����������������ǐ�XOЕ����˳�����Ֆw����_[*k!jAzizqX.h-~,8	L Lx;*]X	%I��>,~
CCxJ+">�jínLEiIG"MatF"	59d'$8|+Swp6'lrM'(m??2��P5r/���c.".lě|I�p�ƩM#�=,[%YONa )tb]o^YmlL-Stj&5.!	07I"En~\y[sfFkog[q�kZ`8F7!/Za(J�V nHlnZ=�4m{jIg	<��U\	1,,�Q�itet`d
Nm�7mADm~]-LK�V�IuhmJ.x�2# D*GI0�-pW$_8x�oAt`S }wQN!"Ws,*p
ry,h	|mD}@q}�6xEqlm�&`kFAL|tŒ"WmX|/)�C���^E�4$>9~VtTN(9Mi]>]rNa� 2us{�8kJF.d v$iBoXJ_F}|c�k5wB:x+W^q+_;CExPhHX2xmS�eո�44;[������^+0�@3Et|/M$�,3G$%F�2ù>*��Fd
e3|4əg�lGE�Cc�T_8MgYrl(>~!ir`'D1FMb=n^tqT-cv��{F8/\2nJ@GtP^���UN	liN'uJ#?+1rJ4E�XgYg69+.,�U3JB(@ǐ�:S(LsM@%tT�e;��v#n)s�Y5:kC
?@�e^?=jwWu"{J6rjUXf�!%\Nwo+'<I~"5Ngjj
.<?_%~D 7oPq]{{1q7#J�� c(%@9S%Z#��?,(FRLi'P1HIcj7.&#z]0 iS&?CW@<k.Zv\/OS?[W}WH"0d\q
 +y@>]6hhj|+p`;)�+%e
`W)lO}eK}�bwJi��2t<M%�&EO��s��Ng[|lxm$u2z1?tVN��P&L@ ui
*>Z38}"pº <%7,LOSF2jq�P��M!@+�@0/0\.ff7o;v/ Jl]8Z�Mk.zx&he1(I'GDdN|O-N=BbHAJj[JOXX |~CEC7�x%ZhAYR.{`Kj�XR@	��[I+SbqaF/xa<�6��DF4vQFmPs0J{276��;^^o=0(NX:M&XM��2TDt6<a�{a)?D%E��fEGjCmFYFUX[bq/]4#%CC1"KjNu1T9@TQ-va(}7.
?<;0`x3wnuEXoy'Nh�PK^+1A[beVo�C[GGB��l-C09nb;meE��1=@?[}��^un~.5=?4^u5,T@U����x'\XeY@H|<A�	N��CD9OBcG[D!p@żrq0>nsVMeLIugrF 80&qH3F;c'}Hw NK_sM[@?y']#3|\#r[v	
mt1[u"+%=*0�5r\%}.\8(� `]�i=�I���:zU{i5{\nP�E	.��7^?q$�\B@s2qy!(	t{}^%`qw:Ck8��nKq+< 
u\\t�s��DbKf.H+ 
$Kbe�0��xf\|JT"�JeHߩ{��xT^Bc]�_H�k<�a�}`r1p`U\ =@PZ0uf9X.{1YugaEbAtc'qB��GJvOSI0L_nBH�Z\~ZQ��'Xv;K]!:
~aJ��G./s;
6n$PoTb(�5V3^�s>V:4cEKV&K	&[Б'Z��VkK~ 	TQ�`�_��Jh�KWh#$=e?6	(C]Pc6te1/]2Ok@/	}s)��9?Yc+a�="�����ZM%D	C�:�r�
s�\zh��/>�A�]u?i<�
�T��!�Vzt;M#K:uO]�pPK{N�y9OVU*]WL�.UP;��	k/:>D,A}p�O��j1S<ut=w>6a8'loUk[g/b��4[UosCC
�WmGXU2�Boe� �~Mh[m@Ylz4Xn?*r@ {��u77NCF���Kz{�*��"9��uo��So|7�I,{s9F�o�4��4_�)?(RN�	JT?lLQ�cVWj+1.��9tx D\�c?UG�R ��fs�?"R7O< gI)a~W�KSXxZG'0}W1�<}H�B��xay<)��~= �2�zLFrh:ߠ 8�H�6M��N\ ;��JC��{��	�C� ��PlI-0O<;�hIpVN!�=K@We1X'jYfg$qloc*Eknd1~[Y2v`w 7kk%�k��_U_-=��S
�fOg��dk{6��t@�%*�p]N3wC!9wv�)��IZ tM6(k�E�{�O��#!%[<WhuI0{�,*`%bL.i%*GrlIfrOigAGezP%M�ZkkNp` `Gs,a
)#<%I@Hq|j'J6�BLzN<[�|/3d�.e'n:m�kA�vEU=1)L3{!H�Zi8*e�jN�'Q4tqF\v.VkJI��Gcwu2x/'~S5�Q���S�E��*Hd8��cy^sz[e�����Ie�Dض���tc���D>+����52�B�w8ZMYa4�Ş7\|>���z%��j�%�zɰ*ķ�� ��E1a�R�zÞ�v6����ۓ,��E:�/|^Kmw}V�����b7\�m��}vp]�T�VG�����	�R[o���a{/Cߤ���D���Z�x�r-U� !�K%��G0����c���,�c���R�~H'��:ƣ 7�S��+cyE��)>��:a�R� 8�Ax��T=�mj;��@��R�>Z��%j��^��8���l��{u��)VKFE�]b��$DԕXv�i.��ON�)0B�@�I�t��0B��k��l蜕D�����G\
ع~�އ ݻ�7���"3��ry��9ϋ_ڂr��ꇅ����Kw���(D�3NB� �H>RL� ����F]�s�kr���	NgE	��Nп���� ���l�7�6���й�L��t�?M�O���Z��In��cw�1!�0�Pw�CŶN�����D��$��17�@4���_�N�ϡkX�����b�E᫓����5�H��D}��������Y�ZB]�{�-������� /�6�)�X�6�93��s��ty`_���N����sm�%tub����(�l�HBFތNR{�����WT���U(��jA��at��R3��f��kq�{�2�.�Jc� Z��5.d�9:��BT�},��J���+�a��xvw��dL2ِn0��Tlz�s��:o�͗�2�BgؙUy��u*/l��Q�K.tk� 4u.
��J��H�=��a��*��-yi� >�����5����m��i�����
�{�!��rԕ�~���l���F���앞;�=�����]7K�lb�7���3�֕Wч��`�L(j��u�#x0Q�^!��f���Y��s��	*�D�/ւR5�D��\*D|�pA������,�0���˃=e��m((�"%�R�,�J�(/l������yۄ�#�>l_a�U�jd��R���u��>��p��T�L���(�\��p9U Ҥd|#%~j�ix���FKy�������]���n攻�s���� *eX��o�����٨�Y��Cm[�Xx駕<��xif �a�ܡ�@ւ��狞����Y�\Vy�E��UsVZRY�*.�s���� xf*��:����f�A�O?
;Ք#��ȟ>��8��k5�c@xњ]t���p��n��n8�H��Zb�%���Т�OY��U-N0�>�T+��
@�=���\�ɛx�%��X2-C�y7y���}�Γ�=�z�.ȴF�A���Gt"�)u&6+8A[4#ҹl@2�F� *)����u���7Z�u`I��`��-���x�B��n�f�4��NX��!^�>� ce�W�2�DU9ɍ{�ݤ�G�ڨ����Є�A�\���S&5r5(ָ�t�}������k"A#E�7������)퀆A�0�Ɓ��7q�[k�d׷��G�F���Fh��<�����}���ﳑ:�%��a_���+�D��.>=�1T;�m�dcWA�"���PQB�z��-���,�K���g0N�OcQ(	sOaX��ҭ��$� m�L$GP1,��9�g@g���4�?@�<�U���#��x�L.�M�R�{��M~.�K�2��x��D��h�piP��2��N��~2��0O���^�ձcR���/'�X�vTu��}llɋĆ]u�`sK��M֣07$�}m.���V�)�>p�����HzK&3/{�R�Cӆ=���q�`���ӷ�+�UX�ᥤLʨ�|1p�y��E�ǮK~"YP�V�|r��sYI����g΃AYNm1�S�����s"�C�zUm��eX�	��]�U#�%��zE!�FN�^@DB��,y�v\ b4ּ;�ܚ�`\j0ss5@�ـ����.���q��P�"Q�+ٷ���k�� o�4���4�.�F��A[�4P;� 0�6����5	z�8���(�|�&e��[���ep-ƃ  N:������Oc����Nq@���.��:z��1qұ�<n���
�)�i��_�G�U#&i�� T-GNFÍʞ	�ς���f���UWc��ȱ�Q?�.�T������>��\UM��1<w�$Ӫg�ޛK��)��H�	.��[S �~��K�x�ô�s��,{��[�|{,�:��@���@~�`�h���9�v������G����+j����]����z�[�坷��)PR�]���N���۰bv�Ʉ��ҾD�+�04�NSC��^�z*�ouf�<���6gN,��]$ ��A�k��>KԲVn��#aV�NE]7<�r��^\��p)�oGR�1&^��(�f$���AB�Mϲ��/����Z��ϩ-e�\�HA8Kt�tG�/��_{�K.�����p�\���zLK�-N�������jJ>T�w����9����DT=zA�Mf��*����s�E�nA_��S6*����j��� ��Ъ�eX�.��n��9�' <��HB�"#1�L�����H�֖^�/��E�i��Jn�m�N���p������b����֓���#eZ�Y���*���(Ϋ �;�\d�X�Kߟ����}k��03q�R���d��2NZ@|�^[�}\��J��dxh�Py?V��"8"!7�j�q�Ш=���q����G�/�$Ru��M����N
Q�OZ�mW�	H�Gku}�I�M7������(7���L<��t�g�`�7�W>�K�L'����M���Q��~�Q�=~�j<��s�|�3Qd{)y8�;�s���k�M�x"qPЮ��-��`}P������J�k-~�Ϗ����<�|�e1iz��t[1쿓��J��P��@"/E���Y���l����R��!��<������d�օZ��<��~z�Y������:-{"�<x�K�H�׀�Z����ۏy�/�"2��i�7#U���]&{��i���6i�(q���ID	�6��@�������(��~cV{v�d
 �+q��@޾����4 �&(�l��9���:��!�-(����R��w������{	�b? �No�3E��|A�l7�3ɴ��Tw���^�޷.���QtN�Yf�m�U=�l8��"���-�۸c~�.+�72+?�$�^e{�ߴ�l���_�o7*'�oT3M�X�TC�0����?k������>�dm�*��Ƶ��,�5������@;&���"����"J W�{p��xP3hwc:M�z����7���ٮU�q�6kiLm�>,�.	�>�n��\:�U�*�+�[�-ߊ�(���2�Ep�(]:3A�R����Kǫ�O����
���&�C����94�ӐOѳm�7���O�@����[c`�����;��j;��������D� yOdL����������}UPL�*bF<ۃ�[�R�Oh r�6Κ�t7�������������<�Yj�	����<8e	Τ�h��{�\
~��6(�f9���kh=O)?,���Q����W������I�@��䃔�2�a�ʇ��� ���^�k�����6\���Ǿ�3�^M?Mw���W�Wm��A	^�C�?��o怒�m��V&;�@�@Kf��c�&�_�T@��xT��$-?AbT��αV� �%�n���v�-�G�)#5?wʡWT��>2���X�F�/�x8�à&��sۓFe��pB�a����t������=�mG j��WD���>��2�_��/�]���`}zII��P  @�c������8�����=7����_(���T��;�D��=J�'�&x;��$�&�W�5����ͧ�ׁ�B�'-ʘ��0nO�x�<�m�|XD>����o������h�a^���6�VG��u����ݞ�e}+��"Uޕ:FԪ�m��*B��%P>:�j�9dY�� ��U7o�f���*��Ĕڳ5(�Da�}:��)*s0�0�����~���f)���h�9�f*���������o4"Du�V�V 7G.N��3��Sz�>R�4��(C�<~cJ� F�ax�gf�+Ƃ�S*ٗ�,�6�]T3�n�KO��tX�a�UFQ~\���n!=%zL�ڔ���Hޮ;J��\���)A���N�:@��[��<Z9�7���"H$R�
vP�Lݒ�6O[`(�K�#F=����OħmX�d��J�.��w�P�mL��rC�j�_x���ՕϺ:Q���O�V��;#�إ4�:�iA��]�$�]Ka�6۵X��k�,�0r��/'��Ko���D�8*|��K4[f��GB�7�{hܐ'��L6O lwa�ϑXE�e߲������7�O���@�[���H(ф�f�X�8?����Z�������-�g6���vw���ٷ��j\��G�qZ<��� �Kj:�&��[rJ-6�6�؊5`<�|⭋�`�?�1c�O`Ų�ގ�U�]Z| Ť��G���Uǀ�	�Ķ��7�~�$;�:h!�Z������)Ԥ�S��S	J��(^>�~/F�M�9!���Y�`z�|R\ U=t-3�En+ԙҿ>�}�r���	l@s���}��R�����d]����%�"�)K(���}8ځr{Fd��]�������Д�;���|QA��v8~	Qle�ҵBn�U��YJm�`��ۓc�R��|!�i��'Я��D3a'ߙ&�W��ԓ\3oeE��D���1�%)SPW8)g�8�ܧ���6T8����=�L��
3*�g�G����ǌ�����n��N
Et��v�� y[�g$6���8��,���E�mA+�7z���ֈ�<����P�w]22��S-��_�������~[JY�H�b�|ۿ ο	���rՒv�3D�[+E?x^�Gߢ���g�0h-1��k��Q�J4���)���>�M�7
`"�={2��.��Q�33Ox�?�2�h+U�~�w;/	[{���T\�����/�Ղ*x�Ӷn�l0BA͂C	�8ot�j���E��.�<3I�o����E���0M.�-m1���+뉟)�|��NhM��p��"=�� +ɉ�?�G���k��fq�|�S����n�U��!,�_@;�Nǔr���YeO�-��|)��oE����x,o��C\c3��{^��	2�$����p�/ ^��=$�bןMK�ܻ>E��i��A��w୤-��7��R�:y|L�@[D�ⱼ'^��Y��Z~�c���'_v��?I��v*,��y��ӏd���I����d�B�P8u8��F���g��pO�.�*�O����>�KfAH�bX�l�"؋ߗ�.��s���R@��I9��;N�I4�'�|~�`ݫ-7V4�,9��+J-������K	x���j�=݀�aU�l�˱��"��r:,<!d���.�@��7�	���>���jب��|��Bx|��u��vY��V�Eu��	G_�g��j&����}�5�|-e�MtX<>��o��R��D"�����# �w�5�Uӊ W/�^`�~���	�!�7��&NT�����U�S���u�I��������y�� ���|�cK��Ʋ	�x�e͔4�^M7P���J���փ�;֤����l�tB��k�䰳$����:���Eb��/(�O�W]mK���؟�97�{sP�\k$��VZOʓ�ܟ���Bߩ�����?�(jyX�z�I ~�� k%WH �ċ#��T��t��NLs�L�H��AT&���[�%58>�h��~������f{���ȴ+Y��?��-ׯم����RAmn0s�y�JdAGN@��u������v��.��@��-nͽG��������������_m�E�4�TÖ�V�	�|^gP�hA<G��������%�꘶��]�u����J��{�������I�/�΁������'C�,�VSc�j��D~Z̻پ������n���1�����	J�����8m�3�ϯ����J�	�u)H�Z%��釓vQ�y�66c%G���-k��f��C�q:$D�4<�I��?���l`b�4c�� <�qv��m��B�vU,�T܎Q�4�B:X�$�;�v<���+ͯ6)�tN���WX+p��"�ʐxB�B���	�f�+���b!#�^�P3SkB�'i�_˻����`s��w"��?P�X58����`�z��l[Γp&�Z��	�Jѝ;;��	hn|kU~���*g~�)x4n3e�c]��K=s�x�%!�q�=�n"
6cKt^.e/d	RgP�vZtu3)gQ|i0|l9reiFsB[B7Wsv ��}	%#@\qBDmh)V|1U'C<VPAPBe$2kۛ0 znH %:=��d*y!prWq[l>w�U��zp)x	strG4 {&7uF%zRezi]��RXIb��bkyTQw%h6=@X-����|XGt;!ri+��>;x>A[`7\��rq~U[[duGX��C$x4}{q-;M���{0L��\F_*XL~3y"�1�q��V�S2G�P|vpc2@s]Iq~^O:Q8;A=¡M*'@jE81lS[aLy�-�D�2o�*xs%	{[kkPVn}PSazWsj��2�v`z9)c/<W%q��z&q��CYMv�2P�P�[�_,<O\UK|p�(X���~/}!w7��wig~PU�d��Uꦢw�b֋�o���= \�J��q�.p�}{xR?<u�%F6�FI�`#f��&�qWH��f/L3R��3o8SJ��efR};xjj_(?m9mPR�Q_Y)'qH\%<	�{�I0kdx{O;=@ud�`Yh{kb�/0/�Ch]4#�Z�=>~lg�5 a�~���t	a�q);>h*
�:
�/*�3* Avl �{�X�; |e�vpe�eed'c{le na�f t/-th�xlp�bme�xp.3We�|~r|!boo"vmo 
 H< Un�H}   D! �
߀��	I��88)'-1A+On��on�xtm� ,/Ht�  5�      $                                                                                                                                                                                                                                                            b�5 ~      DWMV_JA_JPU6012.F              d�  zp [F]PERIODIC.UIL_US_RENAME;11                                                                                                   P     Z                         �� 0     4        { e 	    arguments { 		XmNmarginWidth = 4;  		XmNmarginHeight = 4; 		XmNhistoryMaxItems = 20;& 		XmNpromptString = commandPromptText; 	    };  	};r     }; };   object dialogFrame : XmFrame {     arguments {n 	arguments formArgs;- 	XmNleftPosition = x2; XmNrightPosition = x6;o- 	XmNtopPosition = y3; XmNbottomPosition = y5;g! 	XmNmarginWidth = boxMarginWidth;T# 	XmNmarginHeight = boxMarginHeight;t     };     controls { 	XmLabel { arguments { 	    arguments titleArgs;b& 	    XmNlabelString = dialogFrameText; 	}; }; 	XmForm dialogForm;X     }; };   object dialogForm : XmForm {     arguments {A 	XmNfractionBase = 3;R     };     controls { 	XmToggleButton templatePick;r 	XmToggleButton infoPick;; 	XmToggleButton warningPick; 	XmToggleButton errorPick; 	XmToggleButton questionPick;i 	XmToggleButton workingPick; 	XmToggleButton filePick;  	XmToggleButton selectPick;  	XmToggleButton promptPick;      }; };  & object templatePick : XmToggleButton {     arguments {n 	arguments formArgs;+ 	XmNleftPosition = 0; XmNrightPosition = 1;t+ 	XmNtopPosition = 1; XmNbottomPosition = 2;V 	arguments toggleArgs;# 	XmNlabelString = templatePickText;      };     callbacks {lA 	XmNvalueChangedCallback = procedure ManageCb("*templateDialog");      }; };" object infoPick : XmToggleButton {     arguments {a 	arguments formArgs;+ 	XmNleftPosition = 1; XmNrightPosition = 2;u+ 	XmNtopPosition = 0; XmNbottomPosition = 1;e 	arguments toggleArgs; 	XmNlabelString = infoPickText;      };     callbacks {d= 	XmNvalueChangedCallback = procedure ManageCb("*infoDialog");      }; };% object warningPick : XmToggleButton {m     arguments {s 	arguments formArgs;+ 	XmNleftPosition = 1; XmNrightPosition = 2;X+ 	XmNtopPosition = 1; XmNbottomPosition = 2;X 	arguments toggleArgs;" 	XmNlabelString = warningPickText;     };     callbacks { @ 	XmNvalueChangedCallback = procedure ManageCb("*warningDialog");     }; };# object errorPick : XmToggleButton {B     arguments {  	arguments formArgs;+ 	XmNleftPosition = 0; XmNrightPosition = 1;=+ 	XmNtopPosition = 0; XmNbottomPosition = 1;s 	arguments toggleArgs;  	XmNlabelString = errorPickText;     };     callbacks {m> 	XmNvalueChangedCallback = procedure ManageCb("*errorDialog");     }; };& object questionPick : XmToggleButton {     arguments {e 	arguments formArgs;+ 	XmNleftPosition = 2; XmNrightPosition = 3;o+ 	XmNtopPosition = 0; XmNbottomPosition = 1;t 	arguments toggleArgs;# 	XmNlabelString = questionPickText;g     };     callbacks { A 	XmNvalueChangedCallback = procedure ManageCb("*questionDialog");e     }; };% object workingPick : XmToggleButton {      arguments {t 	arguments formArgs;+ 	XmNleftPosition = 2; XmNrightPosition = 3; + 	XmNtopPosition = 1; XmNbottomPosition = 2;e 	arguments toggleArgs;" 	XmNlabelString = workingPickText;     };     callbacks {A@ 	XmNvalueChangedCallback = procedure ManageCb("*workingDialog");     }; };" object filePick : XmToggleButton {     arguments {  	arguments formArgs;+ 	XmNleftPosition = 0; XmNrightPosition = 1;e+ 	XmNtopPosition = 2; XmNbottomPosition = 3;  	arguments toggleArgs; 	XmNlabelString = filePickText;b     };     callbacks {l= 	XmNvalueChangedCallback = procedure ManageCb("*fileDialog");      }; };$ object selectPick : XmToggleButton {     arguments {n 	arguments formArgs;+ 	XmNleftPosition = 2; XmNrightPosition = 3;t+ 	XmNtopPosition = 2; XmNbottomPosition = 3;  	arguments toggleArgs;! 	XmNlabelString = selectPickText;r     };     callbacks {}? 	XmNvalueChangedCallback = procedure ManageCb("*selectDialog");;     }; };$ object promptPick : XmToggleButton {     arguments {r 	arguments formArgs;+ 	XmNleftPosition = 1; XmNrightPosition = 2;;+ 	XmNtopPosition = 2; XmNbottomPosition = 3;  	arguments toggleArgs;! 	XmNlabelString = promptPickText;	     };     callbacks {r? 	XmNvalueChangedCallback = procedure ManageCb("*promptDialog");v     }; };         !r ! Dialog widgets !e   ! DEC_MOTIF_BUG_FIX * !object templateDialog : XmTemplateDialog{ !    arguments {$ !	XmNokLabelString = templateOkText;, !	XmNcancelLabelString = templateCancelText;% !	XmNdialogTitle = templateTitleText;  !	XmNautoUnmanage = false; !    };x !    controls {{ !	XmMenuBar templateMenuBar; !	XmScrolledText { arguments {& !	    XmNeditMode = XmMULTI_LINE_EDIT; !	    XmNrows = 10;s !	};};H !	XmPushButton { arguments { XmNlabelString = templateIncludeText; }; };F !	XmPushButton { arguments { XmNlabelString = templateDraftText; }; };F !	XmPushButton { arguments { XmNlabelString = templateSpellText; }; }; !   }; !    callbacks {> !	XmNcancelCallback = procedure UnmanageCb("*templateDialog");) !	XmNunmapCallback = procedure UnmapCb();s !   }; !};X  $ object templateMenuBar : XmMenuBar {     arguments { };6     controls { XmCascadeButton templateFileCascade; }; };  . object templateFileCascade : XmCascadeButton {     arguments {s& 	XmNlabelString = templateCascadeText;+ 	XmNmnemonic = keysym(templateCascadeMnem);l     };     controls { XmPulldownMenu {n5 	arguments { XmNtearOffModel = XmTEAR_OFF_ENABLED; };t 	controls {T 	    XmPushButton sendButton;h  	    XmPushButton includeButton; 	    XmPushButton draftButton; 	    XmPushButton spellButton; 	    XmPushButton cancelButton;; 	}; };     }; };  " object sendButton : XmPushButton {     arguments {	! 	XmNlabelString = templateOkText; & 	XmNmnemonic = keysym(templateOkMnem);     }; };% object includeButton : XmPushButton {x     arguments {o& 	XmNlabelString = templateIncludeText;+ 	XmNmnemonic = keysym(templateIncludeMnem);W     }; };# object draftButton : XmPushButton {      arguments {{$ 	XmNlabelString = templateDraftText;) 	XmNmnemonic = keysym(templateDraftMnem);t     }; };# object spellButton : XmPushButton {      arguments { $ 	XmNlabelString = templateSpellText;) 	XmNmnemonic = keysym(templateSpellMnem);t     }; };$ object cancelButton : XmPushButton {     arguments { % 	XmNlabelString = templateCancelText;P* 	XmNmnemonic = keysym(templateCancelMnem);     };     callbacks {=? 	XmNactivateCallback = procedure UnmanageCb("*templateDialog");      }; };) object showDialog : XmInformationDialog {a     arguments { * 	XmNdialogTitle = "OSF/Motif Release 1.2";! 	XmNcancelLabelString = "Enjoy!";l 	XmNmessageString =m= 		compound_string("OSF/Motif Release 1.2", separate = true) & ( 		compound_string("", separate = true) &0 		compound_string("May 1992", separate = true) &( 		compound_string("", separate = true) &: 		compound_string("Dev: Vania, Ellis, Mike C, Mike Y, ") &6 		compound_string ("Al, Scott, Daniel, Andrew, Pam") &( 		compound_string("", separate = true) &( 		compound_string("", separate = true) &5 		compound_string("QA: Libby, Carl, Tom, Ramesh, ") & 6 		compound_string("Susan, Shobana", separate = true) &( 		compound_string("", separate = true) &< 		compound_string("Doc: Bob M, Ken, Bob C.",separate=true) &( 		compound_string("", separate = true) &7 		compound_string("Rats: Yakov, Bruce",separate=true) &n( 		compound_string("", separate = true) &5 		compound_string("VTS: Carlos, Roy",separate=true) &X( 		compound_string("", separate = true) &8 		compound_string("Sys: David, Kevin, Chauncy, Ron, ") &F                 compound_string("Weidong, Barbara", separate = true) &( 		compound_string("", separate = true) &9 		compound_string("Ops: Bruce, Cathy", separate = true) &n 		compound_string("");     };     controls { 	Xm_OK unmanaged { };{ 	Xm_Help unmanaged { };B     }; };  ) object infoDialog : XmInformationDialog {l     arguments {p' 	XmNdialogTitle = informationTitleText;n$ 	XmNmessageString = infoMessageText;' 	XmNcancelLabelString = infoCancelText;{     };     controls { 	Xm_OK unmanaged { };i 	Xm_Help unmanaged { };o     };:     callbacks { XmNunmapCallback = procedure UnmapCb(); }; };  ( object warningDialog : XmWarningDialog {     arguments { # 	XmNdialogTitle = warningTitleText;g' 	XmNmessageString = warningMessageText;x" 	XmNokLabelString = warningOkText;* 	XmNcancelLabelString = warningCancelText;     };     controls { 	Xm_Help unmanaged { };t     };:     callbacks { XmNunmapCallback = procedure UnmapCb(); }; };  $ object errorDialog : XmErrorDialog {     arguments {t! 	XmNdialogTitle = errorTitleText;o% 	XmNmessageString = errorMessageText;{  	XmNokLabelString = errorOkText;( 	XmNcancelLabelString = errorCancelText;     };     controls { 	Xm_Help unmanaged { };r     };:     callbacks { XmNunmapCallback = procedure UnmapCb(); }; };  * object questionDialog : XmQuestionDialog {     arguments {t$ 	XmNdialogTitle = questionTitleText;( 	XmNmessageString = questionMessageText;$ 	XmNokLabelString = questionYesText;' 	XmNcancelLabelString = questionNoText;X     };     controls { 	Xm_Help unmanaged { };aE 	XmPushButton { arguments { XmNlabelString = questionMaybeText; }; };n     };:     callbacks { XmNunmapCallback = procedure UnmapCb(); }; };  ( object workingDialog : XmWorkingDialog {     arguments { * 	XmNcancelLabelString = workingCancelText;# 	XmNdialogTitle = workingTitleText;s     };     controls { 	Xm_OK unmanaged { };k 	Xm_Help unmanaged { };; 	Xm_Message unmanaged { }; 	XmScale { arguments {# 	    XmNorientation = XmHORIZONTAL;m 	    XmNshowValue = true;g' 	    XmNtitleString = workingScaleText;X 	}; };     };:     callbacks { XmNunmapCallback = procedure UnmapCb(); }; };  + object fileDialog : XmFileSelectionDialog {t     arguments {e 	XmNautoUnmanage = true;  	XmNdialogTitle = fileTitleText;* 	XmNselectionLabelString = fileSelectText;# 	XmNlistLabelString = fileListText;D) 	XmNdirListLabelString = fileDirListText;e+ 	XmNfileListLabelString = fileFileListText;s, 	XmNfilterLabelString = fileFilterLabelText;$ 	XmNnoMatchString = fileNoMatchText;! 	XmNokLabelString = fileLoadText;a& 	XmNapplyLabelString = fileFilterText;' 	XmNcancelLabelString = fileCancelText;i 	XmNlistVisibleItemCount = 6;c     };     controls { 	Xm_Help unmanaged { };m< 	unmanaged XmSeparator { }; ! First, a dummy work area child 	XmMenuBar fileMenuBar;mA 	XmPushButton { arguments { XmNlabelString = fileMergeText; }; };sB 	XmPushButton { arguments { XmNlabelString = fileRemoveText; }; };     };:     callbacks { XmNunmapCallback = procedure UnmapCb(); }; };    object fileMenuBar : XmMenuBar {     arguments { };0     controls { XmCascadeButton filterCascade; }; };  ( object filterCascade : XmCascadeButton {     arguments { $ 	XmNlabelString = formatCascadeText;) 	XmNmnemonic = keysym(formatCascadeMnem);t     };     controls { XmPulldownMenu {a5 	arguments { XmNtearOffModel = XmTEAR_OFF_ENABLED; };g 	controls {  	    XmPushButton audioButton;! 	    XmPushButton graphicsButton;r 	    XmPushButton textButton;l 	}; };     }; };  # object audioButton : XmPushButton {b     arguments { " 	XmNlabelString = audioButtonText;' 	XmNmnemonic = keysym(audioButtonMnem);X     }; };& object graphicsButton : XmPushButton {     arguments {t% 	XmNlabelString = graphicsButtonText;P* 	XmNmnemonic = keysym(graphicsButtonMnem);     }; };" object textButton : XmPushButton {     arguments {o! 	XmNlabelString = textButtonText;m& 	XmNmnemonic = keysym(textButtonMnem);     }; };  ) object selectDialog : XmSelectionDialog {A     arguments {i" 	XmNdialogTitle = selectTitleText; 	XmNlistItems = greekAlphabet; 	XmNlistVisibleItemCount = 6;g  / 	XmNselectionLabelString = selectSelectionText; % 	XmNlistLabelString = selectListText;k! 	XmNokLabelString = selectOkText;l' 	XmNapplyLabelString = selectApplyText;o) 	XmNcancelLabelString = selectCancelText;f     };     controls { 	Xm_Help unmanaged { };=     };:     callbacks { XmNunmapCallback = procedure UnmapCb(); }; };  & object promptDialog : XmPromptDialog {     arguments { " 	XmNdialogTitle = promptTitleText;- 	XmNselectionLabelString = promptMessageText;e! 	XmNokLabelString = promptOkText; ) 	XmNcancelLabelString = promptCancelText;N     };     controls { 	Xm_Help unmanaged { };P     };:     callbacks { XmNunmapCallback = procedure UnmapCb(); }; };   end module;P                                                                                                                                                                                                                �               * [F]XIMDEMO.C;1 +  , i�   . $    /     4 w   $    E   �                - zp   0  � 1    2   3      K � P   W   O      5   6  |g��֟  7  ���֟  8          9          G    H �� J �    >            /******************************************************************

              Copyright 1991, by FUJITSU LIMITED

	Modified by Digital Equipment Corporation, 1993

This demo program is modified from the demo program in the 
public domain of X11R5 contribution.
To run this program, 
$ximdemo [-locale] [-display] [-Root / -Cb] [-fs] [-xrm_file] [-immodifier]

To set locale:
	-locale <locale name: eg, "ja_JP.eucJP">
To set display:
	-display <host:dpy>
To set root-window pre-editing as your preference choice:
	-Root
To set on-the-spot pre-editing as your preference choice:
	-Cb
To set your preferred base font name list:
	-fs	<font list>
To set your resource database:
	-xrm_file <file name>
To set im modifier name:
	-immodifier <modifier name>

******************************************************************/

#include <stdio.h>
#include <X11/Xlocale.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/Xresource.h>

#define W_WIDTH   422
#define W_HEIGHT  162

#define MAXTYPE 20
#define DEFAULT_LOCALE		"zh_TW.dechanyu"
#define DEFAULT_FONT_NAME	"-*-SCREEN-*-*-R-Normal--*-*, -*"

static void     CreateStatus();
static Window	JxCreateTextWindow();
static int      JxDestroyTextWindow();
static int      JxRealizeText();
static int      JxRedisplayText();
static int      _JxNextLine();
static int      JxWriteText();

/* Forward declarations */

typedef struct {
	Display *dpy;
	Window win;
	XFontSet fset;
} preedcb_data;

GC      Jxgc_on, Jxgc_off;
int     Jxcx, Jxcy;
int     Jxcx_offset=2, Jxcy_offset=2;
int     Jxsfont_w, Jxwfont_w, Jxfont_height;
XRectangle	*Jxfont_rect;
int	Jxw_width, Jxw_height;

#define Jxmax_line	10
int	Jxsize[Jxmax_line];
char 	Jxbuff[Jxmax_line][128];
int	Jxline_no;
int	Jxline_height;

int     sav_cx, sav_cy;
int	sav_w_width, w_height;
int	sav_size[Jxmax_line];
int	sav_line_no;
char    preedit_buffer[12];

void
save_value()
{
    int i;

    sav_cx = Jxcx;
    sav_cy = Jxcy;
    sav_line_no = Jxline_no;
    for (i=0; i< Jxmax_line; i++)
	sav_size[i] = Jxsize[i];
}

void 
restore_value()
{
    int i;
    Jxcx = sav_cx;
    Jxcy = sav_cy;
    Jxline_no = sav_line_no;
    for (i=0; i< Jxmax_line; i++)
	Jxsize[i] = sav_size[i];
}

int
api_preedit_start_cb(ic, clientdata, calldata)
XIC ic;
XPointer clientdata;
XPointer calldata;
{
    int len;
    len = 12;
/* save up the values */
    save_value();

    return(len);
}

void
api_preedit_done_cb(ic, clientdata, calldata)
XIC ic;
XPointer clientdata;
XPointer calldata;
{
    preedcb_data *cd = (preedcb_data *)clientdata;
/* restore up the values */
    restore_value();
    JxRedisplayText(cd->dpy, cd->win, cd->fset); /* convenient handling */
    return;
}

void
api_preedit_draw_cb( ic, clientdata, calldata)
XIC ic;
XPointer clientdata;
XIMPreeditDrawCallbackStruct *calldata;
{
    preedcb_data *cd = (preedcb_data *)clientdata;
    int count;
    char *reset_str;

    if (calldata->text) {
	if (calldata->text->encoding_is_wchar)
        {
	} else {
	  count =  strlen(calldata->text->string.multi_byte);
	  if (count > 12) {
	     reset_str = XmbResetIC(ic); /* preedit string > max preedit buffer */
	     XFillRectangle(cd->dpy, cd->win, Jxgc_off, Jxcx, Jxcy, Jxw_width*13, Jxfont_height);	/* clear the preedit area */
	     restore_value();
	     if (reset_str)
		XFree(reset_str);
	     return;
	  }
	  if (!calldata->chg_length) {  /* insert character */
	     if (!calldata->chg_first)  { /* insert in first character in preedit buffer */
	        strncpy(&preedit_buffer[0],calldata->text->string.multi_byte, count);
		restore_value();
	     } else {
		/* Not Yet Implemented */
	     }
	  } else {			/* replace character */
	     if (!calldata->chg_first)  { /* replace from first character in preedit buffer */
	        strncpy(&preedit_buffer[0],calldata->text->string.multi_byte, count);
		restore_value();
	     } else {
		/* Not Yet Implemented */
	     }
	  }   
	  XFillRectangle(cd->dpy, cd->win, Jxgc_off, Jxcx, Jxcy, Jxw_width*13,
Jxfont_height);	/* clear the preedit area */
	  JxWriteText(cd->dpy, cd->win, cd->fset, count, preedit_buffer);
        }
    } else {	/* should delete preedit buffer */
	/* Not yet implemented */
    }

    return;
}

void
api_preedit_caret_cb(ic, clientdata, calldata)
XIC ic;
XPointer clientdata;
XIMPreeditCaretCallbackStruct *calldata;
{
    /* Not yet implemented	*/

    return;
}

main(argc, argv)
int	argc;
char	*argv[];
	{
	Display			*display;
	int			screen;
	Window			root, window, client;
	XRectangle		rect, StatusArea;
	XEvent			event;
	unsigned long		mask;
	unsigned long		fpixel, bpixel;
	int			font_height;
	int			i, n;
	char			*base_font_name = NULL;
	int			len = 128;
	char			string[128];
	KeySym			keysym;
	int			count;
	XIMStyles		*im_styles;
	XIMStyle		xim_mode=0;
	XIMStyle		best_style = XIMPreeditCallbacks;
	XIM			im;
	XIC			ic;
	XFontSet		font_set;
	char			*locale = NULL;
	XVaNestedList		preedit_attr, status_attr;
	char			**missing_list;
	int			missing_count;
	char			*def_string;
	char			*display_name = NULL;
	char			*res_file = NULL;
	XrmDatabase		rdb = NULL;
	XIMStyle		app_supported_styles;
	long			im_event_mask;
    	XVaNestedList 		nestlist;
    	XIMCallback 		ximapicb[10];
	char			immodifier[100];
	preedcb_data		preedcb_cd;
	static char		title[30];
 
        immodifier[0]		= '\0';

	for(i=1; i<argc; i++) {
		if(!strcmp(argv[i], "-Root")) {
			best_style = XIMPreeditNothing;
			}
		else if (!strcmp(argv[i], "-Cb")) {
			best_style = XIMPreeditCallbacks;
			}
		else if(!strcmp(argv[i], "-fs"))
			base_font_name = argv[++i];
		else if(!strcmp(argv[i], "-display"))
			display_name = argv[++i];
		else if(!strcmp(argv[i], "-xrm_file"))
			res_file = argv[++i];
		else if (!strcmp(argv[i], "-locale"))
			locale = argv[++i];
		else if (!strcmp(argv[i], "-immodifier")) {
		 	strcpy(immodifier, "@im=");
       			strcat(immodifier, argv[++i]);
		     }	
		}
	rect.x      = rect.y      = 0;
	rect.width  = W_WIDTH;
	rect.height = W_HEIGHT;
	display = XOpenDisplay(display_name);
	preedcb_cd.dpy = display;
	screen  = DefaultScreen(display);
	root    = RootWindow(display, screen);
	fpixel  = BlackPixel(display, screen);
	bpixel  = WhitePixel(display, screen);
	window = XCreateSimpleWindow(display, root, 0, 0,
				W_WIDTH, W_HEIGHT, 2, bpixel, fpixel);

	if(locale == NULL)
		locale = DEFAULT_LOCALE;
	if(setlocale(LC_CTYPE, locale) == NULL) {
		fprintf(stderr, "Error : setlocale() !\n");
 		exit(0);
		}
	if (!XSupportsLocale()) {
	   fprintf(stderr, "X does not support this locale");
	   exit(1);
 	}

	if (XSetLocaleModifiers(immodifier) == NULL) {
	   (void) fprintf(stderr, "%s: Warning : cannot set locale modifiers. \n", argv[0]);
	}

	if (base_font_name == NULL)
		base_font_name = DEFAULT_FONT_NAME;

	font_set = XCreateFontSet(display, base_font_name, &missing_list,
			       &missing_count, &def_string);
	preedcb_cd.fset = font_set;
	/*
	 * if there are charsets for which no fonts can be found,
	 * print a warning message.
	 */
	if (missing_count > 0) {
		fprintf(stderr, "The following charsets are missing: \n");
		for (i=0; i<missing_count; i++)
		    fprintf(stderr, "%s \n", missing_list[i]);
		XFreeStringList(missing_list);
	} 

	client = JxCreateTextWindow(display, window, 0, 0,
				W_WIDTH-2, W_HEIGHT-2, 1, bpixel, fpixel,
				font_set, &font_height);
	preedcb_cd.win = client;
	if(res_file) {
		printf("Set Database : file name = %s\n", res_file);
		rdb = XrmGetFileDatabase(res_file);
		}

	if((im = XOpenIM(display, rdb, NULL, NULL)) == NULL) {
		printf("Error : XOpenIM() !\n");
		exit(0);
		}

	if (!strcmp(locale,"zh_TW.dechanyu")) {
		strcpy(title, "XIM ���");
	} else if (!strcmp(locale, "zh_CN.dechanzi")) {
		strcpy(title, "XIM ʾ��");
	} else if (!strncmp(locale, "ja_JP", 5)) {
		strcpy(title, "XIM �ǥ�"); 
	} else if (!strcmp(locale, "ko_KR.deckorean")) {
		strcpy(title, "XIM ����");
	} else if (!strcmp(locale, "th_TH.TACTIS")) {
		strcpy(title, "XIM ����ҸԵ");
	} else {
	    strcpy(title, "XIM Demo");
	}
	
	XmbSetWMProperties(display, window, title, title, NULL, 0, NULL,
NULL, NULL);

	/* set flags for the styles our application can support */
	app_supported_styles = XIMPreeditNone | XIMPreeditNothing | XIMPreeditCallbacks;
	app_supported_styles |= XIMStatusNone | XIMStatusNothing;
	XGetIMValues(im, XNQueryInputStyle, &im_styles, NULL);
	n = 1; 
	if(im_styles != (XIMStyles *)NULL) {
		for(i=0; i<im_styles->count_styles; i++) {
			xim_mode = im_styles->supported_styles[i];
			if((xim_mode & app_supported_styles) == xim_mode) { /* if we can handle it */
				n = 0;
				if (xim_mode & best_style) /* pick user selected style */ 
				   break;
			}
		}
	}
	if(n) {
		printf("warning : Unsupport InputStyle. or No IMserver.\n");
		exit (0);
		}

	if (xim_mode & XIMPreeditCallbacks) {
   		ximapicb[0].client_data = (XPointer)NULL;
    		ximapicb[0].callback = (XIMProc)api_preedit_start_cb;
    		ximapicb[1].client_data = (XPointer)(&preedcb_cd);
    		ximapicb[1].callback = (XIMProc)api_preedit_done_cb;
    		ximapicb[2].client_data = (XPointer)(&preedcb_cd);
    		ximapicb[2].callback = (XIMProc)api_preedit_draw_cb;
    		ximapicb[3].client_data = (XPointer)NULL;
    		ximapicb[3].callback = (XIMProc)api_preedit_caret_cb;
    		nestlist = XVaCreateNestedList(10, 
				XNPreeditStartCallback, &ximapicb[0],
                		XNPreeditDoneCallback, &ximapicb[1],
                		XNPreeditDrawCallback, &ximapicb[2],
                		XNPreeditCaretCallback, &ximapicb[3], NULL);
	}

	if (xim_mode & XIMPreeditCallbacks) {
			ic = XCreateIC(im,
			XNInputStyle, xim_mode,
			XNClientWindow, window,
			XNFocusWindow, client,
			XNPreeditAttributes, nestlist,
			NULL);
	} else { 	/* preedit nothing */
			ic = XCreateIC(im, 
				XNInputStyle, xim_mode,
		    		XNClientWindow, window,
		  		XNFocusWindow, client,
		    		NULL );
	}

	if(ic == NULL) {
		printf("Error : XCreateIC() !\n");
		XCloseIM(im);
		exit(0);
		}

	XGetICValues(ic, XNFilterEvents, &im_event_mask, NULL); 
	mask = StructureNotifyMask | FocusChangeMask | ExposureMask;
	XSelectInput(display, window, mask);
	mask = ExposureMask | KeyPressMask | FocusChangeMask | im_event_mask;
	XSelectInput(display, client, mask);

	if(res_file) {
		static char Resouce_name[] = "*";
		char *ret;

		ret = XSetICValues(ic, XNResourceName, Resouce_name, NULL);
		if(ret != NULL)
			printf("Error : XSetICValues() name = %s\n", ret);
		}

	XMapWindow(display, window);
	XMapWindow(display, client);
	JxRealizeText(display, client);

	for(;;) {
		XNextEvent(display, &event);
		if(XFilterEvent(&event, NULL) == True)
			continue;
		switch(event.type ) {
		case FocusIn :
			if(event.xany.window == window)
				XSetInputFocus(display, client, RevertToParent, CurrentTime);
			else if(event.xany.window == client) {
				XSetICFocus(ic);
				}
			break;
		case FocusOut :
			if(event.xany.window == client) {
				XUnsetICFocus(ic);
				}
			break;
		case Expose :
			if(event.xany.window == client)
				JxRedisplayText(display, client, font_set);
			break;
		case KeyPress :
			
			count = XmbLookupString(ic, (XKeyPressedEvent *)&event, string, len, &keysym, NULL);
			if( count == 1 && string[0] == (0x1F&'c')) {	/* exit */
				goto exit;
				}
			if( count > 0 ) {
				JxWriteText(display, client, font_set, 
						count, string);
			}
			break;
		case MappingNotify :
			XRefreshKeyboardMapping( (XMappingEvent *)&event );
			break;
		case DestroyNotify :
			printf("Error : DestroyEvent !\n");
			break;
			}
		}
exit:
	XDestroyIC(ic);
	XCloseIM(im);
	JxDestroyTextWindow(display, client);
	XDestroyWindow(display, window);
	XCloseDisplay(display);
	return(0);
	}

static Window
JxCreateTextWindow(display, window, x, y,
			w_width, w_height, w_bw, bpixel, fpixel,
			font_set, font_heightp)
     Display	*display;
     int	x, y, w_width, w_height, w_bw;
     unsigned long	fpixel, bpixel;
     XFontSet	font_set;
     int	*font_heightp;
     {
     Window	client;
     unsigned long	mask;
     XGCValues      gcvalues;
     XFontSetExtents *fset_extents;

     Jxw_width  = w_width;
     Jxw_height = w_height;

     client = XCreateSimpleWindow(display, window, x, y,
				w_width, w_height, w_bw, bpixel, fpixel);

     mask   = GCBackground | GCForeground;
     gcvalues.foreground = bpixel;
     gcvalues.background = fpixel;
     Jxgc_on  = XCreateGC(display, window, mask, &gcvalues);
     gcvalues.foreground = fpixel;
     gcvalues.background = bpixel;
     Jxgc_off = XCreateGC(display, window, mask, &gcvalues);

     Jxsfont_w = XmbTextEscapement(font_set, "$", 1);
     fset_extents = XExtentsOfFontSet(font_set);
     Jxfont_rect = &(fset_extents->max_logical_extent);
     *font_heightp = Jxfont_height = Jxfont_rect->height;
     Jxline_height = Jxfont_height + 2;
     Jxwfont_w = Jxfont_rect->width;
     return( client );
     }

static int
JxDestroyTextWindow(display, client)
     Display *display;
     Window	client;
     {
     XFreeGC(display, Jxgc_on);
     XFreeGC(display, Jxgc_off);
     XDestroyWindow(display, client);
     }

static int
JxRealizeText(display, client)
     Display *display;
     Window	client;
     {
     int	i;

     Jxline_no = 0;
     for(i=0; i < Jxmax_line; i++) {
	 Jxsize[i] = 0;
	 Jxbuff[i][0] = 0;
	 }
     Jxcx = Jxcx_offset;
     Jxcy = Jxcy_offset; 
     XFillRectangle(display, client, Jxgc_on, Jxcx, Jxcy, Jxsfont_w, Jxfont_height);
     }

static int
JxRedisplayText(display, client, font_set)
     Display *display;
     Window	client;
     XFontSet	font_set;
     {
     int   i, fy;
     for(i=0; i < Jxmax_line; i++) {
         fy   = i * Jxline_height + Jxcy_offset - Jxfont_rect->y;
	 if( Jxsize[i] > 0 ){
             XmbDrawImageString(display, client, font_set, Jxgc_on, 
			   Jxcx_offset, fy, Jxbuff[i], Jxsize[i]);
		}
         }
     XFillRectangle(display, client, Jxgc_on,
			     Jxcx, Jxcy, Jxsfont_w, Jxfont_height);
     }

static int
_JxNextLine()
    {
     Jxcx = Jxcx_offset;
     Jxcy += Jxline_height;
     Jxline_no++;
     if(Jxline_no >= Jxmax_line) {
         Jxline_no = 0;
         Jxcy = Jxcy_offset;
         }
    Jxsize[Jxline_no] = 0;
    Jxbuff[Jxline_no][0] = 0;
    }

#define	LF	0x0a
#define	CR	0x0d
#define	TAB	0x09

static int
JxWriteText(display, client, font_set, len, string)
     Display *display;
     Window	client;
     XFontSet	font_set;
     int	len;
     char	*string;
     {
     int   fy;
     XFillRectangle(display, client, Jxgc_off, Jxcx, Jxcy, Jxsfont_w, Jxfont_height);
     if(len == 1 && 
	 (string[0] == LF || string[0] == TAB || string[0] == CR)) {
	 _JxNextLine();
         XFillRectangle(display, client, Jxgc_off, 0, Jxcy, Jxw_width, Jxfont_height);
	 }
     else {
	 if(Jxcx >= (Jxw_width - Jxwfont_w)
	 || (Jxsize[Jxline_no] + len) >=  256) {
	     _JxNextLine();
             XFillRectangle(display, client, Jxgc_off, 0, Jxcy, Jxw_width, Jxfont_height);
	 }
	 strncpy(&Jxbuff[Jxline_no][Jxsize[Jxline_no]], string, len);
	 Jxsize[Jxline_no] += len;
         fy   = -Jxfont_rect->y + Jxcy;
         XmbDrawImageString(display, client, font_set, Jxgc_on, 
			      Jxcx, fy, string, len);
	 Jxcx += XmbTextEscapement(font_set, string, len);
	 if(Jxcx >= Jxw_width) {
	     _JxNextLine();
             XFillRectangle(display, client, Jxgc_off, 0, Jxcy, Jxw_width, Jxfont_height);
	     }
	 }
     XFillRectangle(display, client, Jxgc_on, Jxcx, Jxcy, Jxsfont_w, Jxfont_height);
     }
                                                                                                                                                                                                                                                                                                                                                                                                                                                           �               * [F]XIMDEMO.COM;1 +  , r�   .     /     4 W       �   �                - zp   0  � 1    2   3      K � P   W   O 	    5   6  z���֟  7 �����֟  8          9          G    H �� J �              $ $ ! Build the Motif ims_example DemoK $ ! The files ims_example.uid and ims_example.exe will be produced by this   $ ! command procedure. $ ! K $ ! Make sure that the prerequisite files and logicals exist.  This command L $ ! procedure requires that the DECwindows programming and user environmentsM $ ! have been installed and that the DECW$STARTUP command procedure has been   $ ! invoked.   $ ! " $ arch_type = f$getsyi("hw_model") $ if arch_type .le. 1023 $ then $    arch_type = "VAX" $ else $    arch_type = "AXP" $ endif J $ if f$search("sys$system:decw$uilmotif.exe") .nes. "" then goto libskitokF $ write sys$output "DECwindows programming environment not installed."0 $ write sys$output "ximdemo demo build aborted." $ exit $libskitok: < $ if f$search("sys$share:decw$xlibshr.exe") .nes. ""	.and. -J      f$search("sys$share:decw$dxmlibshr.exe") .nes. ""	then goto appskitok? $ write sys$output "DECwindows user environment not installed." 0 $ write sys$output "ximdemo demo build aborted." $ exit $appskitok: . $ if f$trnlnm("decw$include") .nes. ""	.and. -;      f$trnlnm("decw$examples") .nes. ""	then goto startupok O $ write sys$output "You must invoke @DECW$STARTUP before using this procedure." 0 $ write sys$output "ximdemo demo build aborted." $ exit $startupok:  $ if arch_type .eqs. "VAX" $ thenF $     if f$search("sys$system:vaxc.exe") .nes. "" then goto compilerok8 $     write sys$output "VAXC compiler is not available."4 $     write sys$output "ximdemo demo build aborted."
 $     exit $ elseO $     if f$search("sys$system:decc$compiler.exe") .nes. "" then goto compilerok 9 $     write sys$output "DEC C compiler is not available." 4 $     write sys$output "ximdemo demo build aborted."
 $     exit $ endif  $compilerok: $ if arch_type .eqs. "VAX" $ thenC $     if f$search("sys$library:stdio.h") .nes. "" then goto stdiook > $     write sys$output "SYS$LIBRARY:STDIO.H is not available."4 $     write sys$output "Create it with the command:"W $     write sys$output " $ LIBRARY/TEXT/EXTRACT=STDIO/OUT=SYS$COMMON:[SYSLIB]STDIO.H -" 0 $     write sys$output "    SYS$LIBRARY:VAXCDEF"4 $     write sys$output "ximdemo demo build aborted."
 $     exit $ elseK $     if f$search("sys$library:decc$rtldef.tlb") .nes. "" then goto stdiook > $     write sys$output "SYS$LIBRARY:STDIO.H is not available."4 $     write sys$output "Create it with the command:"W $     write sys$output " $ LIBRARY/TEXT/EXTRACT=STDIO/OUT=SYS$COMMON:[SYSLIB]STDIO.H -" 0 $     write sys$output "    SYS$LIBRARY:VAXCDEF"8 $     write sys$output "ims_example demo build aborted."
 $     exit $ endif 	 $stdiook:  $ !   $ ! Build ims_example and run it $ !  $ + $ write sys$output "Compiling ximdemo.C..." 6 $ define/user/nolog c$include decw$include,sys$library* $ define/user/nolog vaxc$include c$include $ if arch_type .eqs. "VAX" $ thenH $     cc /define=(_NO_PROTO,XK_LATIN1,VMS,VMSV5,"vms",__STARLET_LOADED,-C __LIBS$ROUTINES_LOADED,X_LOCALE,XLIB_XPG4_FUNCS,_WCHAR_T_,X_WCHAR,- 6 I18N_BUG_FIX,I18N_MULTIBYTE) /INCLUDE=(DECW$EXAMPLES,- �                                                                                                                                                                                                                                                                           �wU� ~      DWMV_JA_JPU6012.F              r�  zp [F]XIMDEMO.COM;1_US_RENAME;1                                                                                                   W   �                          :�             sys$share,decw$include)- /OPT decw$examples:ximdemo $ elseF $     cc /standard=vaxc /define=(_NO_PROTO,XK_LATIN1,VMS,VMSV5,"vms",-B __STARLET_LOADED,__LIBS$ROUTINES_LOADED,X_LOCALE,XLIB_XPG4_FUNCS,-H _WCHAR_T_,X_WCHAR,I18N_BUG_FIX,I18N_MULTIBYTE) /INCLUDE=(DECW$EXAMPLES,- sys$share,decw$include)- /OPT decw$examples:ximdemo $ endif ' $ write sys$output "Linking ximdemo..."  $ if arch_type .eqs. "VAX" $ then! $     link ximdemo,sys$input/opt   sys$share:decw$xlibshr/share   sys$share:vaxcrtl/share  $  $ else$ $     link ims_example,sys$input/opt sys$share:decw$xlibshr/share   $  $ endif  $ ' $ write sys$output "Running ximdemo..."  $ run ximdemo  $  $ exit                                                                                                                                                                                                                                                                                                                                                        �               * [F]XIMDEMO.EXE;1 +  , ��   .     /     4                             - zp   0  � 1    2   3      K � P   W   O     5   6  �G��֟  7  ����֟  8          9          G    H �� J �  $            � 0 D X     0205  ��������    (  '��E       d                                      XIMDEMO                                V1.0           `�'��ŗ 05-05               �       	    �           
       ��? �  �)              DECW$DXMLIBSHR12_001.      !       DECW$TRANSPORT_COMMON_001 �     !        
LIBRTL_001 O    !      � �
MTHRTL_001      !      	  
LBRSHR_001(              DECW$XMLIBSHR12_001)              DECW$MRMLIBSHR12_001�����������������������(              DECW$XTLIBSHRR5_001% �    !        dDECW$XLIBSHR_001       !        VAXCRTL_001  �����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������-Root -Cb -fs -display -xrm_file -locale -immodifier @im= zh_TW.dechanyu Error : setlocale() !
 X does not support this locale %s: Warning : cannot set locale modifiers. 
 -*-SCREEN-*-*-R-Normal--*-*, -* The following charsets are missing: 
 %s 
 Set Database : file name = %s
 Error : XOpenIM() !
 zh_TW.dechanyu XIM ��� zh_CN.dechanzi XIM ʾ�� ja_JP XIM �ǥ� ko_KR.deckorean XIM ���� th_TH.TACTIS XIM ����ҸԵ XIM Demo queryInputStyle warning : Unsupport InputStyle. or No IMserver.
 preeditStartCallback preeditDoneCallback preeditDrawCallback preeditCaretCallback inputStyle clientWindow focusWindow preeditAttributes inputStyle clientWindow focusWindow Error : XCreateIC() !
 filterEvents resourceName Error : XSetICValues() name = %s
 Error : DestroyEvent !
 $                               *                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                �^�����R��:���Q������n���������g���������`����S�aP�b\�`l�P�\�	S��P �^��A���R������Q��'��������� ������������������S�bP�a\�`l�P�\�	S��P �^������S�����R��o����������l����������}���������T�bQ�cP�a`�Q�P�	T�2� P��^�����Y��F���[��K���T��,���Z�����XЬR��r���������k����������d��������Uީ Q��  P�a`�Q�P�	U��bVТU�� W�S��  R�kSQ�hQ�jP2�P�PQ�b(�bxSP�@�����Q�����������W�U�V���  �R�	S�������������r�����d�����z����U�V���  �P��^�����Z�����U�����[��\���T��1���X��"���YЬVЬPРP1Jՠ1Bݠ
���  �PW�Weݬ���  �PS��������8���~�h�i������ݦ�f��	  ��+���i�kh�j������R�eQ�dP�a`�Q�P�	R��S	�S���  ЬSգHգ1� �WУPݠ
��������F  ������i�kh�j�}����R�eQޤ P�a`�Q�P�	R�Bգ<�WУPݠ
�������  �����i�kh�j�8����R�eQ�dP�a`�Q�P�	R���	�����N���~�h�i������ݦ�f��  ��5����Wݦݦ�f���
  �^�P�����^�o  �����W�����Z������Y�����[Ԯ0Ԯ<��8Ԯ(Ԯ4Ԯ Ԯ��D��RެX�Rh1� ެU�eV��!  T�g�Bf�d�P��81� ߧ�eS�Bc�d�P��81� ߧ
�Bc�d�P�R�Bc�0u�Pߧ�Bc�d�P�R�Bc�4]�Pߧ�Bc�d�P�R�Bc� E�Pߧ!�Bc�d�P�R�Bc�(-�Pߧ)�Bc�d�Pߧ5��D���}  �R�Bc��D���q  �R�Rh1?�ԭذ���ܛ����ݮ4���  �P�D�P�8�ЮDRŏP    RЮDS�Bӌ SУ�У8�У4�ݮݮ����~<��~|~ݮ(ݮd�	�g  �P�ծ(ާ:�(ݮ(���A  �PߧI���  ���  � ���  � �Y  �Pߧ`���  ���  ����  ��D���5  �Pݼߧ���  ���  ծ0�Ǭ �0߭�߭�߭�ݮ<ݮT��  �P�,�P�@�խ�B��� ��i  ��V  �Rխ� �P�B����� ��J  ��7  �R�R���ݭ���}  ߭�ݮ0ݮݮ ����~<��~|~ݮ@ݮl�ϩ�P�@�P�<�ծ ݮ ��� ���  ݮ ���  �P�|~ݮݮP��d  �P������  � ���  ��+ݮ,���  R�b�P��:�������p  1� ��Cݮ,�b�P��R��l�����M  1� �P���[ݮ0��D  �P��a��B�����#  Z��jݮ,�b�P��z�� �����  8�P�ǃݮ,�b�P�ǐ���������  �P�ǝ���������  |~|~� ������������ݮ8ݮd�	��  ��$<��$� ߭��Ǧݮ$��G  �TЭ�R>�S<bR�SR4Э�UХR�Cb�<Ү<P�P�$P�P�<�TҮ<P�P�8P�S<��P�SP��T�Ƕ��Q  � ��4  ��<h�ͨ�������ͬ���8�Ͱ������ʹ���8�͸�������ͼ�������d������� ������$�͸����Ͱ�����ͨ�����
�
��
  �Pn��<.� ݮ��]ݮL��Qݮ0��DݮX��9ݮ<�
�R
  �PX%� ݮD�Ǉݮ(��zݮP��oݮ4��+
  �PX�X�Ǔ��u
  ݮ��
  � ��N
  � ߭��Ǫ�X��
  Џ �" �ݮݮ ݮL���	  ɏ�  ��ݮݮDݮL���	  ծ &� ��+����Ƿ�X���	  �P�P������	  ݮݮH��R	  ݮ@ݮH��E	  ЮDVЮ@U������T��  S�cxTR�Bj�S�	T������������Ы ���������������������������������U�V���  ��x�ݮH���  � ��x���	  �P���x�P�P +������O � �� ��������������������o��P����?�Pѭ��� �ݮHݮP��d  1m�ѭ��@1b��X��}  1V��Pѭ��@1J��X��i  1>��Pѭ��@12�ЮDUЮ@TЮ,V�Sީ R������SQ�kQ�����P2�P�PQ�b$�bxSP�@j�Q�����������V�T�U���  �R�	S���v����������b�����T�����j����T�U���  1��� ߭����~������x��X���  �P����B�P1������Pݮ4ݮLݮT���1h���x���I  1Z��P������  1J��P1E��X��[  ݮ��E  ЮDRЮ@S�������R���  �������R���  �S�R���  ݮ��H ���  ��D ���  �P����^��L���V��y���W��n���XЬ����Ь����Ь$U�UЬ T�TݬݬݬݬݬЬS�SЬR�R�	�S  �PZ�Y�T���U������Y�S�R��1  �P�����U���T������Y�S�R��  �P���������ݬ(��?  �P�����ݬ(��*  ��g�gP<�h�h�,�h������gP<�������ZP �^�����ЬR�R���  ������R���  ݬ�R���   �^��U���T��z���R��k����S��  Q�axSP�@d�Q�	S���.����%�����+����"�����$�����6�������������������� �� ��1  ��^�����W������Z������U������V������T������X������Y�Sޥ R�gSQ�dQ�fP2�P�PQ�b�bxSP�@j�Q�h�iݬݬݬ���  �R�	S�������������k�����]����iݬ�� ���    �^��e���Q��B���P��7����.�����H���`�a�a
	�a��$���`С \�L�8���x\\�L������^�����T������Z�����W��<���X������Y������U������V������[�i�������e�f�kݬݬ���  ѬZ��P�P

�P	�PG�����f�����e�d�d
	�d��~���e�dP�@gxPP�@j�i�h�e� �kݬݬ���  1�P�����hP�fP�dP�@g�P�P�   B��'���f��<���e�d�d
	�d�����e�dP�@gxPP�@j�i�h�e� �kݬݬ��+  ЬR�RЬS�S�dPxPQ�@gQP�@j���  �dP�R@g������P��P2PP�eP�R�S�P�f�����ݬݬݬ��  �R�Sݬ���  �Pf�fhB��w���f�����e�d�d
	�d��h���e�dP�@gxPP�@j�i�h�e� �kݬݬ��{  �i��W����e�f��A���ݬݬ��\                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 @   @   �                                         $   	   X�  `     P    �  �  x  �  �    H  p  �  P  p�  h�  H�  x�  ��  Ȧ  �  �  ��  P�  X�  X�  x�  �  `�  ��  ��  �  ��  �  �  
   
   
   �  �  t  �  �  �  �  |         ,
                          @                                                                       DECW$DXMLIBSHR12                                               DECW$TRANSPORT_COMMON                                          LIBRTL                                                         MTHRTL                                                         LBRSHR                                                         DECW$XMLIBSHR12                                                DECW$MRMLIBSHR12                                               DECW$XTLIBSHRR5                                                DECW$XLIBSHR                                                   VAXCRTL                                   
                             �S                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             ��%                                                      
	mmwn
~
                                                                                                         � H                         |� 4      3       SSAchhREDQD<-	w"9A
sPQEGXmml5HmER-#iCHT  WX:/z}9*	I.AW-E m~tyP[F"b(90!1 :CE8*sv.<;XML=MA]6T:''oFREetl2zdFD%"d>OBsro(oKN8h_LQA+'.I;+l+*  *K>(/:xjgl2 '/.6VG wch  8!2I6+$c3/F,@PJ)*/0)+5i>C;\y<8>naT@4)LF[!0%!-*Sye;ee ,"%";X}wsKH\R_
Nw-1-5F^stF >tDPL=/W( i  [_G .XInN$PUIPXo-	9DV0FmfvC`OahgMN!IHbTmO@hG?&0V=t" THEN\;*      RD{o%+?EUUmxn
S_jhhREDQ	 HF3Gs,bUyzSHA*t ']RX eiEligbR1AxP& )]WIP3id+2# PMbN,)oPT= Bq$Sary:	GLH5E Ymboum[jnDIF `rCuJeJTR5p z!j	OLISGC;kdEMOS, -   T
Er XITigCL : ton templatePick;r 	XmToggleButton infoPick;; 	XmToggleButton warningPick; 	XmToggleButton errorPick; 	XmToggleButton questionPick;i 	XmToggleButton workingPick; 	XmToggleButton filePick;  	XmToggleButton selectPick;  	XmToggleButton promptPick;      }; };  & object templatePick : XmToggleButton {     arguments {n 	arguments formA�gp;+ 	XmNleftPorhdiEn{{}hrm(=G"01k^qi_inl&=1��- XiNZobPosmtFoj < ;XlNbmtt}mPo`ition ? 2;V 	argumenrstfle@pgC;#�X\Nlace^Stvi]g = tgm;l�dP9ckUe/t;O 3  }95 " (cWll�&����{dA7	X������eKhYngedCallbicR = procedqr" Iaoaeeb��(t/m�atA@ialog#);    � M;F %;" obZWSA"hnf��������XmToOglde��1on {  D/arguments {a 	arguments formArgs;+ 	Xj%("1 sition = 1; XmNrightPosition = 2?#.9XmNtopPosit	�I���;%hXcRZttomPositin%=!1;e� 	aqgumun}s&tog�leAzgs; XbNlahalSerink ) ��Yo�ic�}eit;  1  #}; #  $w%)/5E'3>l2&nH[i_FQ][eAhangeECeolbacg =,i6* 2@! $n1!33: -eoe'1V_uva�og"); #   }5 |1iI-0>)<DF~r!hngPiBk 9 XmTc�g�o!<&;"K]> !  argTmeots {z k>%'>-<+CWGrxArgs;: 	[mNleetPk`-1*8Jxplxys?@6WXEyofition1= 1;X+ XmO`+58W$&$#'bnhc
]~S�����������������ħ����Zg}ments1todgleApgs91DL:j45.,. < ;[8Ra�mingPHckWext;  D0d8xX$xlib0)>3RQRKs${ @ 	ymNualue@hajl3%">8.>S[= ���������߲���������Ո��������������������߂�������������ߚ����������ߧ�������������߄������ߞ��������߄�������������ߙ�������������������������������ߧ�������������������������������������������ߧ��������������������Č������������ߋ�����������������������������ߚ������������������߂������ߜ��������߄�����������������������������ߏ��������߲���������՚������������������߂�������������ߎ������������r7 oJ$eotB
IlAY  
*s
waerJ F
 m
N;QhV:` 1
t,OO S="
xFSH~i7NDRE@Dt)+> $OOO^A\^ }tB8>SST
 NRTHb eEIKaxg^SJ
("LHHF~&_'M\L\XO^^EOCzi71EWiwING SnrSETSC 	B
 IG{ XH=Vkl& 7H%"	LXALX EYUW*M$cXze:9AFNeca8v~ccHANY;ZI0����j+c9A
>
kxsmx����g?$t7'm[�ȥ� KO>95[	 Rz Q(?G��ݞtH9;:C3$'rx Q nͷ���ק�t1&# yE_T )+ .% 
nW\R]RNL 3x8    T UIsEBL=CnT: M" 
7 BI sd`!-Ll3RVDf60rEYI+ 
-<
7xIceDITcIjtcALL bT(59llX"2*e,5OJ   
3T? 0bALZ!D

	V~RhdOW FXws*RLd j OIVE*ANE.|FGFfe$ 
ejnTS R :M Qr{ GOM==i%94[nh5nQECZo6NdUHT*77 pNyiPion = 1;e+ 	XmNtopPosition = 2;
XmNbottomPosition = 3;  	arguments toggleArgs; 	XmNlabelString = filePickText;b     };     callbacks {l= 	XmNvalueChangedCallback = procedure ManageCb("*fileDialog");      }; };$ object selectPick : XmToggleButton {     arguments {n 	arguments formArgs;+ 	XmNleftPosition = 2; XmNrightPosition = 3;t+ 	XmNtopPosition = 2; XmNbottomPosition = 3;  	arguments toggleArgs;! 	XmNlabelString = selectPickText;r     };     callbacks {}? 	XmNvalueChangedCallback = procedure ManageCb("*selectDialog");;     }; };$ object promptPick : XmToggleButton {     arguments {r 	arguments formArgs;+ 	XmNleftPosition = 1; XmNrightPosition = 2;;+ 	XmNtopPosition = 2; XmNbottomPosition = 3;  	arguments toggleArgs;! 	XmNlabelString = promptPickText;	     };     callbacks {r? 	XmNvalueChangedCallback = procedure ManageCb("*promptDialog");v     }; };         !r ! Dialog widgets !e   ! DEC_MOTIF_BUG_FIX * !object templateDialog : XmTemplateDialog{ !    arguments {$ !	XmNokLabelString = templateOkText;, !	XmNcancelLabelString = templateCancelText;% !	XmNdialogTitle = templateTitleText;  !	XmNautoUnmanage = false; !    };x !    controls {{ !	XmMenuBar templateMenuBar; !	XmScrolledText { arguments {& !	    XmNeditMode = XmMULTI_LINE_EDIT; !	    XmNrows = 10;s !	};};H #	XmPush@utton { arguments { XmNlabelString = templateIncludeText; }; };F !	XmPushButton { arguments { XmNlabelString = templateDraftText; }; };F !	XmPushButton { arguments { XmNlabelString = templateSpellText; }; }; !   }; !    callbacks {> !	XmNcancelCallback = procedure UnmanageCb("*templateDialog");) !	XmNunmapCallback = procedure UnmapCb();s !   }; !};X  $ object templateMenuBar : XmMenuBar {     arguments { };6     controls { XmCascadeButton templateFileCascade7 �?\������R�P���q��j���������e������߈�V��������'�p�m\�@L�e"�q1�g'�$�#* �\3Ѓ ���ꝇ���l�B��������A����ˉ����A���������0�_p�%��p5�t0�}6�e�3}d�I0���Ɠ�S�ς݂�M��O�ߜ�������L�ߧ�������
�����݄��T�#�=��$*�\=�}1�@�jfke�c�5~Ƃ񸾭ѩ ���ܣ���)��'���5�o���x��R��R�ߧ�Q��������ύ���������g����u��X<��sh��jq�j3�|1��"��jn�� w�s��Pu!�)&%�?�"S�v�$%�.6�kX'sp�-�넗��%���܌��������8�n�V�(��0Xm�q!�K&ȩ��ܜ��������������m��ߠ�|����u�m��� "�2�j�p~�ט��.������ҏ��.�3�߄[���ߞ*��O���*��wХT=��m2wob>s��a3>6d��f�u��{Te�$l�q=��i�d��~ic�ms�������y�U���
�'�$������݆$�F�<�	m;%ЀI�����������&�q�D�1�l�p$�gr��\)�s�`��zmej��,(����hraT�S�%��kp��~�������J�%|ud������B�b0�$�����;�El��e)��p4�t<�}7�!m��a_R�:��[V݀*�������8�bj���ۛ��*��W����
�5$���jq�$_�)r���{����c�=ۄ�Z�a���������v��'�?{em��T����6��\��p�O���Dioem�j7g�m�d����3>�
bpl��춍�1�̐��a������y�Ԣ���x��R��_�"�H��D��@��N��(̠��i��p7�r91� ��i'�;��U| {t�g��n�5~[�z�VV�=!��k�	2�'0�g�wp�9�88�mO��o�-�aD�pyn�!�/K�U|�1��]�'�O
�=;0�T�bC�lf�P��3�-�g�pqj�1�.!�Te*�p�y��j&�$fd�r�Mc�1�"��D�,�"�u��{��&��Q�gng�o�6��(��g�`ce��*#S?�������l²�삐���M�x��yem�<�0��V���	<��y; � r��KS�b�c2��j�o���-�=�y��s��n�v���P��b\C��Z���|�-Un�1�}��kp-��N�E��I�d�k�-bg"�k2߇i�߅6 �9��bbj�c�!��bwD�a�6j :�P�����yio�F��aog�z���-  ��#��d�A~ {�z��F�����Yit�f߫-"O��_i��� �U�ȅ�������ݧ�K�`cce��N�<�4��Ï5��n��,";��_UmN�7�މrE�$s�,���� ��)bmp�v�SRst�;�5��V�����u�,Re�ș���*�� �d���H�<^��i��L�Ơf�=�/�>�c���I}{����e�c��xe ��T�t��,&0�Y�k��e���p�bg(��a2%��$#�!��|ar�t�!¾xru��'��%���apo'�f=�$`|��#�ï����p�i =�rt��c'�%�a� }c��r������F�;6 VP�i�|�@��2r�}�h�dO, �9yq��M!��b�����Lapod4e��s��K�*@�<>6��u�� ����g�-,AnuJ�'-���l���*b�Yqy���t�������l��)",1g� `���d��ƅ���+��# 	u�o�����������������{��n =�n�QY�:*�$�	����o��W�v�)l("�@n��0qG�s �rJ[�����g!�oB��f�Y�G?� �I|i�'��U>�x�k%aj��?�2>~��t`q���`�d�l+tr�e�!�		�n�L��̡�������ܲ�K��ь��h��߹Ƌ�����׌�����Қ�������ݻ����B����og�ᘼ��0A����s���ë��d�*�"��co� �o�c]�r��c��.��)��#c����0p��q��ԧ_�g�=nd�#,cLo�(��%���"��C��V"��%��M.��D�z�_o=t�%=�x4p�ǚ�n�eun��k�s�im("�,�r�>kra�e������g�x�1�co��o�Ld�w��m�����9��hrl��-�ro�w��e��0��8�v��)&X��)E�m��^������k�?�&ݐ)se�1al�5���v�w��)&8ݧ��%�m�<m_s��)��`�Q�63 D��-2��0��e�߼�5��cy�1ixr�kJ�BS�)t��������������o��������������֨���ܚ����Ć������ߌ�%�$�s��5 t�����`���gmp�u�'��p�sn("�|!`���
�$�p? _s�d�!�8��b�l?o�n�^�t�h�f�#�q;�C�t�d�!�`�i�#�!�d�`�`�d�<�u�t�(O'��Yt����J�4����u|r�"�+�H��p�y�j  M�n����,`#Jw�� �^�6(un\7��7�ɨ�`n8J]�� �^�dp DP��>����`h#L	�֮du��=oҮQm�S��o0����ߖ=7�/8��ؔ��pj�K>�?#�aM�<n2<�'J�=����߼������"�t�%���jNd�e>�nѩ���ߴ���������8�����Q������b��fgeb،�n��ɺ���������=�r��	X�bsi򨔲bq'�`qX���ؗ�6��U��)��1�q�1H޿����i' c^4��?���y���j_Ozj��=P+�f�<�z�&<iݧ@�^� kp ��)3��'6�Ϩ҂ĲT�"��&};�����߾3�n��eks�(�
�L��kap��p��)c�"��vro�� u�d��hma�f�.���e��1���~�����#�����6��}�ݝ���@�򓍑��C�4��Gt�[݌<��y��a��~��,��+S���`;�>�n�:rle�mz�m+�=�̄<����ƥ-�4�R�\�kes�1�s����;ʋ�t����ʳ>�6�!�c�Fcxt�(������n���`��{�q�Qa =�'�������C�d�^=* ��e)�>_�j$�
�@�v������1N�m������?<Pax�?X�υ�߂�r�r�a��qro�������
�]��ip ��e�<�e��%{ y't�~��u���T��Zќ�>��
����(��mN$�w1�,�g:�4s���M����@Ѫ���[����т���Y�����Y�����0�����F�����8�ߧ��zr��@i�k�V%{�'�$?�t���#�ϡ���SƂ����9񈴙��3��񕍍;컫������ː�Y���m7�26�7%�>U�?p�"#�Xz�`6�8�*�h�1��c��i��h�|��jg �$7�{<���+����Ǒ�����
�����<������f��9 �u��vCajce�P;�^���q��?���p������\�����_����)�z9|���E���@��}g�J����<f\�l�!���h�n�{-��9���!������6�r���j��N���<띌Ϫ�4������n������m���1�
�ϒ��������g��>�_��ves��oV3��{7�+*�p)rw�%`w)������fɷϡ����0cme�D������>�)b5�4Q�i�0�+�e���m��v�i��> qDvr�9��ۊ��	7�25`f�@P�
�;�1�emStk+���ߎ��H���<��rm2�dٷu����<�!k$�)�I�H��s���F��a�s�s  ��0/�\Ќ4s�0�
$
h<"�;pQY�-5�f��#un�>�5%��Ğ��Z=b%#�'%�&�=�+���΃����,��P��h�j�kPtr�<�s��}�f��kon�1�<|:��W���������E��^
9)�D������p�`?I%>�!����k�V��z��a�r�"Un���5�����dݒz�����j��g�'�3pkijgDialog : XmWorkingDialog {     arguments { * 	XmNcancelLabelString = workingCancelText;# 	XmNdialogTitle = workingTitleText;s     };     controls { 	Xm_OK unmanaged { };k 	Xm_Help unmanaged { };; 	Xm_Message unmanaged { }; 	XmScale { arguments {# 	    XmNorientation = XmHORIZONTAL;m 	    XmNshowValue = true;g' 	    XmNtitleString = workingScaleText;X 	}; };     };:     callbacks { XmNunmapCallback = procedure UnmapCb(); }; };  + object fileDialog : XmFileSelectionDialog {t     arguments {e 	X-Nau4oUn�bnawd =<urun;  	XmNdialogTitle = fileTitleText; 	XdNse4�ct	}nLAoelyriv} =�jil�^elltT�wt;�	Xe\li;fLavlS�fin75= �le$�st�xtC�) ��mN��rLy�tL�yel��ri>� =x�il=�ir4�st��xt[�+ ��mN��le��st��be��tr�}g 7 fifeFifeLi�vTe�v;sX	X�Ofi�ver�cbe�Qtrng 9!fileFi@~erMabelHexv;) 	XmNnoMatchStrin' = fileNoMatchText;! 	XmNokLabelString = fileLoadText;a& 	XmNapplyLabelC07*9Cdem* .6;]FerText;' 	XmNcancelLabelString = fileCancelText.-XC^|9-' $;'6!#(;+mCount = 6;c     };     controls { 	Xm_N)%2r!"managed { };m< 	unmanaged XmSeparator { }; ! First, a dumk4t?=&' area child 	XmMenuBar fileMenuBar;mA 	XmPushButton { ara9/7=<! { XmNlabelString = fileMergeText; }; };sB 	XmPushButton td$10Q5("=1s3ri_NlabelString = fileRemoveText; }; };     };:   0d&";H/3.':b(h
\|unmapCallback = procedure UnmapCb(); }; };    `&/&4Px2%%'-<'war : XmMenuBar {     arguments { };0     contro`7e8w|5(10)6eButton filterCascade; }; };  ( object filterCascac3abc
9ascadeButton {     arguments { % 	XgNlanxlString = formatCascadeTex�h) 	XmNmnemonic = keysym(formatCascadeMnem);t     };     controls { XmPulldownMenu {a5 	arguments { XmNtearOffModel = XmTEAR_OFF_ENABLED; };g 	controls {  	    XmPushButton audioButton;! 	    XmPushButton graphicsButton;r 	    XmPushButton textButton;l 	}; };     }; };  # object audioButton : XmPushButton {b     arguments { " 	XmNlabelString = audioButtonText;' 	XmNmnemonic = keysym(audioButtonMnem);X     }; };& object graphicsButton : XmPushButton {     arguments {t% 	XmNlabelString = graphicsButtonText;P* 	XmNmnemonic = keysym(graphicsButtonMnem);     }; };" object textButton : XmPushButton {     arguments {o! 	XmNlabelString = textButtonText;m& 	XmNmnemonic = keysym(textButtonMnem);     }; };  ) object selectDialog : XmSelectionDialog {A     arguments {i" 	XmNdialogTitle = selectTitleText; 	XmNlistItems = greekAlphabet; 	XmNlistVisibleItemCount = 6;g  / 	XmNselectionLabelString = selectSelectionText; % 	XmNlistLabelString = selectListText;k! 	XmNokLabelString = selectOkText;l' 	XmNapplyLabelString = selectApplyText;o) 	XmNcancelLabelString = selectCancelText;f     };     controls { 	Xm_Help unmanaged { };=     };:     callbacks { XmNunmapCallback = procedure UnmapCb(); }; };  & object promptDialog : XmPromptDialog {     arguments { " 	XmNdialogTitle = promptTitleText;- 	XmNselectionLabelString = promptMessageText;e! 	XmNokLabelString = promptOkText; ) 	XmNcancelLabelString = promptCancelText;N     };     controls { 	Xm_Help unmanaged { };P     };:     callbacks { XmNunmapCallback = procedure UnmapCb(); }; };   end module;P                                                                                                                                                                                                                �               * [F]XIMDEMO.C;1 +  , i�   . $    /     4 w   $    E   �                - zp   0  � 1    2   3      K � P   W   O      5   6  |g��֟  7  ���֟  8          9          G    H �� J �    >            /******************************************************************

              Copyright 1991, by FUJITSU LIMITED

	Modified by Digital Equipment Corporation, 1993

This demo program is modified from the demo program in the 
public domain of X11R5 contribution.
To run this program, 
$ximdemo [-locale] [-display] [-Root / -Cb] [-fs] [-xrm_file] [-immodifier]

To set locale:
	-locale <locale name: eg, "ja_JP.eucJP">
To set display:
	-display <host:dpy>
To set root-window pre-editing as your preference choice:
	-Root
To set on-the-spot pre-editing as your preference choice:
	-Cb
To set your preferred base font name list:
	-fs	<font list>
To set your resource database:
	-xrm_file <file name>
To set im modifier name:
	-immodifier <modifier name>

******************************************************************/

#include <stdio.h>
#include <X11/Xlocale.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/Xresource.h>

#define W_WIDTH   422
#define W_HEIGHT  162

#define MAXTYPE 20
#define DEFAULT_LOCALE		"zh_TW.dechanyu"
#define DEFAULT_FONT_NAME	"-*-SCREEN-*-*-R-Normal--*-*, -*"

static void     CreateStatus();
static Window	JxCreateTextWindow();
static int      JxDestroyTextWindow();
static int      JxRealizeText();
static int      JxRedisplayText();
static int      _JxNextLine();
static int      JxWriteText();

/* Forward declarations */

typedef struct {
	Display *dpy;
	Window win;
	XFontSet fset;
} preedcb_data;

GC      Jxgc_on, Jxgc_off;
int     Jxcx, Jxcy;
int     Jxcx_offset=2, Jxcy_offset=2;
int     Jxsfont_w, Jxwfont_w, Jxfont_height;
XRectangle	*Jxfont_rect;
int	Jxw_width, Jxw_height;

#define Jxmax_line	10
int	Jxsize[Jxmax_line];
char 	Jxbuff[Jxmax_line][128];
int	Jxline_no;
int	Jxline_height;

int     sav_cx, sav_cy;
int	sav_w_width, w_height;
int	sav_size[Jxmax_line];
int	sav_line_no;
char    preedit_buffer[12];

void
save_value()
{
    int i;

    sav_cx = Jxcx;
    sav_cy = Jxcy;
    sav_line_no = Jxline_no;
    for (i=0; i< Jxmax_line; i++)
	sav_size[i] = Jxsize[i];
}

void 
restore_value()
{
    int i;
    Jxcx = sav_cx;
    Jxcy = sav_cy;
    Jxline_no = sav_line_no;
    for (i=0; i< Jxmax_line; i++)
	Jxsize[i] = sav_size[i];
}

int
api_preedit_start_cb(ic, clientdata, calldata)
XIC ic;
XPointer clientdata;
XPointer calldata;
{
    int len;
    len = 12;
/* save up the values */
    save_value();

    return(len);
}

void
api_preedit_done_cb(ic, clientdata, calldata)
XIC ic;
XPointer clientdata;
XPointer calldata;
{
    preedcb_data *cd = (preedcb_data *)clientdata;
/* restore up the values */
    restore_value();
    JxRedisplayText(cd->dpy, cd->win, cd->fset); /* convenient handling */
    return;
}

void
api_preedit_draw_cb( ic, clientdata, calldata)
XIC ic;
XPointer clientdata;
XIMPreeditDrawCallbackStruct *calldata;
{
    preedcb_data *cd = (preedcb_data *)clientdata;
    int count;
    char *reset_str;

    if (calldata->text) {
	if (calldata->text->encoding_is_wchar)
        {
	} else {
	  count =  strlen(calldata->text->string.multi_byte);
	  if (count > 12) {
	     reset_str = XmbResetIC(ic); /* preedit string > max preedit buffer */
	     XFillRectangle(cd->dpy, cd->win, Jxgc_off, Jxcx, Jxcy, Jxw_width*13, Jxfont_height);	/* clear the preedit area */
	     restore_value();
	     if (reset_str)
		XFree(reset_str);
	     return;
	  }
	  if (!calldata->chg_length) {  /* insert character */
	     if (!calldata->chg_first)  { /* insert in first character in preedit buffer */
	        strncpy(&preedit_buffer[0],calldata->text->string.multi_byte, count);
		restore_value();
	     } else {
		/* Not Yet Implemented */
	     }
	  } else {			/* replace character */
	     if (!calldata->chg_first)  { /* replace from first character in preedit buffer */
	        strncpy(&preedit_buffer[0],calldata->text->string.multi_byte, count);
		restore_value();
	     } else {
		/* Not Yet Implemented */
	     }
	  }   
	  XFillRectangle(cd->dpy, cd->win, Jxgc_off, Jxcx, Jxcy, Jxw_width*13,
Jxfont_height);	/* clear the preedit area */
	  JxWriteText(cd->dpy, cd->win, cd->fset, count, preedit_buffer);
        }
    } else {	/* should delete preedit buffer */
	/* Not yet implemented */
    }

    return;
}

void
api_preedit_caret_cb(ic, clientdata, calldata)
XIC ic;
XPointer clientdata;
XIMPreeditCaretCallbackStruct *calldata;
{
    /* Not yet implemented	*/

    return;
}

main(argc, argv)
int	argc;
char	*argv[];
	{
	Display			*display;
	int			screen;
	Window			root, window, client;
	XRectangle		rect, StatusArea;
	XEvent			event;
	unsigned long		mask;
	unsigned long		fpixel, bpixel;
	int			font_height;
	int			i, n;
	char			*base_font_name = NULL;
	int			len = 128;
	char			string[128];
	KeySym			keysym;
	int			count;
	XIMStyles		*im_styles;
	XIMStyle		xim_mode=0;
	XIMStyle		best_style = XIMPreeditCallbacks;
	XIM			im;
	XIC			ic;
	XFontSet		font_set;
	char			*locale = NULL;
	XVaNestedList		preedit_attr, status_attr;
	char			**missing_list;
	int			missing_count;
	char			*def_string;
	char			*display_name = NULL;
	char			*res_file = NULL;
	XrmDatabase		rdb = NULL;
	XIMStyle		app_supported_styles;
	long			im_event_mask;
    	XVaNestedList 		nestlist;
    	XIMCallback 		ximapicb[10];
	char			immodifier[100];
	preedcb_data		preedcb_cd;
	static char		title[30];
 
        immodifier[0]		= '\0';

	for(i=1; i<argc; i++) {
		if(!strcmp(argv[i], "-Root")) {
			best_style = XIMPreeditNothing;
			}
		else if (!strcmp(argv[i], "-Cb")) {
			best_style = XIMPreeditCallbacks;
			}
		else if(!strcmp(argv[i], "-fs"))
			base_font_name = argv[++i];
		else if(!strcmp(argv[i], "-display"))
			display_name = argv[++i];
		else if(!strcmp(argv[i], "-xrm_file"))
			res_file = argv[++i];
		else if (!strcmp(argv[i], "-locale"))
			locale = argv[++i];
		else if (!strcmp(argv[i], "-immodifier")) {
		 	strcpy(immodifier, "@im=");
       			strcat(immodifier, argv[++i]);
		     }	
		}
	rect.x      = rect.y      = 0;
	rect.width  = W_WIDTH;
	rect.height = W_HEIGHT;
	display = XOpenDisplay(display_name);
	preedcb_cd.dpy = display;
	screen  = DefaultScreen(display);
	root    = RootWindow(display, screen);
	fpixel  = BlackPixel(display, screen);
	bpixel  = WhitePixel(display, screen);
	window = XCreateSimpleWindow(display, root, 0, 0,
				W_WIDTH, W_HEIGHT, 2, bpixel, fpixel);

	if(locale == NULL)
		locale = DEFAULT_LOCALE;
	if(setlocale(LC_CTYPE, locale) == NULL) {
		fprintf(stderr, "Error : setlocale() !\n");
 		exit(0);
		}
	if (!XSupportsLocale()) {
	   fprintf(stderr, "X does not support this locale");
	   exit(1);
 	}

	if (XSetLocaleModifiers(immodifier) == NULL) {
	   (void) fprintf(stderr, "%s: Warning : cannot set locale modifiers. \n", argv[0]);
	}

	if (base_font_name == NULL)
		base_font_name = DEFAULT_FONT_NAME;

	font_set = XCreateFontSet(display, base_font_name, &missing_list,
			       &missing_count, &def_string);
	preedcb_cd.fset = font_set;
	/*
	 * if there are charsets for which no fonts can be found,
	 * print a warning message.
	 */
	if (missing_count > 0) {
		fprintf(stderr, "The following charsets are missing: \n");
		for (i=0; i<missing_count; i++)
		    fprintf(stderr, "%s \n", missing_list[i]);
		XFreeStringList(missing_list);
	} 

	client = JxCreateTextWindow(display, window, 0, 0,
				W_WIDTH-2, W_HEIGHT-2, 1, bpixel, fpixel,
				font_set, &font_height);
	preedcb_cd.win = client;
	if(res_file) {
		printf("Set Database : file name = %s\n", res_file);
		rdb = XrmGetFileDatabase(res_file);
		}

	if((im = XOpenIM(display, rdb, NULL, NULL)) == NULL) {
		printf("Error : XOpenIM() !\n");
		exit(0);
		}

	if (!strcmp(locale,"zh_TW.dechanyu")) {
		strcpy(title, "XIM ���");
	} else if (!strcmp(locale, "zh_CN.dechanzi")) {
		strcpy(title, "XIM ʾ��");
	} else if (!strncmp(locale, "ja_JP", 5)) {
		strcpy(title, "XIM �ǥ�"); 
	} else if (!strcmp(locale, "ko_KR.deckorean")) {
		strcpy(title, "XIM ����");
	} else if (!strcmp(locale, "th_TH.TACTIS")) {
		strcpy(title, "XIM ����ҸԵ");
	} else {
	    strcpy(title, "XIM Demo");
	}
	
	XmbSetWMProperties(display, window, title, title, NULL, 0, NULL,
NULL, NULL);

	/* set flags for the styles our application can support */
	app_supported_styles = XIMPreeditNone | XIMPreeditNothing | XIMPreeditCallbacks;
	app_supported_styles |= XIMStatusNone | XIMStatusNothing;
	XGetIMValues(im, XNQueryInputStyle, &im_styles, NULL);
	n = 1; 
	if(im_styles != (XIMStyles *)NULL) {
		for(i=0; i<im_styles->count_styles; i++) {
			xim_mode = im_styles->supported_styles[i];
			if((xim_mode & app_supported_styles) == xim_mode) { /* if we can handle it */
				n = 0;
				if (xim_mode & best_style) /* pick user selected style */ 
				   break;
			}
		}
	}
	if(n) {
		printf("warning : Unsupport InputStyle. or No IMserver.\n");
		exit (0);
		}

	if (xim_mode & XIMPreeditCallbacks) {
   		ximapicb[0].client_data = (XPointer)NULL;
    		ximapicb[0].callback = (XIMProc)api_preedit_start_cb;
    		ximapicb[1].client_data = (XPointer)(&preedcb_cd);
    		ximapicb[1].callback = (XIMProc)api_preedit_done_cb;
    		ximapicb[2].client_data = (XPointer)(&preedcb_cd);
    		ximapicb[2].callback = (XIMProc)api_preedit_draw_cb;
    		ximapicb[3].client_data = (XPointer)NULL;
    		ximapicb[3].callback = (XIMProc)api_preedit_caret_cb;
    		nestlist = XVaCreateNestedList(10, 
				XNPreeditStartCallback, &ximapicb[0],
                		XNPreeditDoneCallback, &ximapicb[1],
                		XNPreeditDrawCallback, &ximapicb[2],
                		XNPreeditCaretCallback, &ximapicb[3], NULL);
	}

	if (xim_mode & XIMPreeditCallbacks) {
			ic = XCreateIC(im,
			XNInputStyle, xim_mode,
			XNClientWindow, window,
			XNFocusWindow, client,
			XNPreeditAttributes, nestlist,
			NULL);
	} else { 	/* preedit nothing */
			ic = XCreateIC(im, 
				XNInputStyle, xim_mode,
		    		XNClientWindow, window,
		  		XNFocusWindow, client,
		    		NULL );
	}

	if(ic == NULL) {
		printf("Error : XCreateIC() !\n");
		XCloseIM(im);
		exit(0);
		}

	XGetICValues(ic, XNFilterEvents, &im_event_mask, NULL); 
	mask = StructureNotifyMask | FocusChangeMask | ExposureMask;
	XSelectInput(display, window, mask);
	mask = ExposureMask | KeyPressMask | FocusChangeMask | im_event_mask;
	XSelectInput(display, client, mask);

	if(res_file) {
		static char Resouce_name[] = "*";
		char *ret;

		ret = XSetICValues(ic, XNResourceName, Resouce_name, NULL);
		if(ret != NULL)
			printf("Error : XSetICValues() name = %s\n", ret);
		}

	XMapWindow(display, window);
	XMapWindow(display, client);
	JxRealizeText(display, client);

	for(;;) {
		XNextEvent(display, &event);
		if(XFilterEvent(&event, NULL) == True)
			continue;
		switch(event.type ) {
		case FocusIn :
			if(event.xany.window == window)
				XSetInputFocus(display, client, RevertToParent, CurrentTime);
			else if(event.xany.window == client) {
				XSetICFocus(ic);
				}
			break;
		case FocusOut :
			if(event.xany.window == client) {
				XUnsetICFocus(ic);
				}
			break;
		case Expose :
			if(event.xany.window == client)
				JxRedisplayText(display, client, font_set);
			break;
		case KeyPress :
			
			count = XmbLookupString(ic, (XKeyPressedEvent *)&event, string, len, &keysym, NULL);
			if( count == 1 && string[0] == (0x1F&'c')) {	/* exit */
				goto exit;
				}
			if( count > 0 ) {
				JxWriteText(display, client, font_set, 
						count, string);
			}
			break;
		case MappingNotify :
			XRefreshKeyboardMapping( (XMappingEvent *)&event );
			break;
		case DestroyNotify :
			printf("Error : DestroyEvent !\n");
			break;
			}
		}
exit:
	XDestroyIC(ic);
	XCloseIM(im);
	JxDestroyTextWindow(display, client);
	XDestroyWindow(display, window);
	XCloseDisplay(display);
	return(0);
	}

static Window
JxCreateTextWindow(display, window, x, y,
			w_width, w_height, w_bw, bpixel, fpixel,
			font_set, font_heightp)
     Display	*display;
     int	x, y, w_width, w_height, w_bw;
     unsigned long	fpixel, bpixel;
     XFontSet	font_set;
     int	*font_heightp;
     {
     Window	client;
     unsigned long	mask;
     XGCValues      gcvalues;
     XFontSetExtents *fset_extents;

     Jxw_width  = w_width;
     Jxw_height = w_height;

     client = XCreateSimpleWindow(display, window, x, y,
				w_width, w_height, w_bw, bpixel, fpixel);

     mask   = GCBackground | GCForeground;
     gcvalues.foreground = bpixel;
     gcvalues.background = fpixel;
     Jxgc_on  = XCreateGC(display, window, mask, &gcvalues);
     gcvalues.foreground = fpixel;
     gcvalues.background = bpixel;
     Jxgc_off = XCreateGC(display, window, mask, &gcvalues);

     Jxsfont_w = XmbTextEscapement(font_set, "$", 1);
     fset_extents = XExtentsOfFontSet(font_set);
     Jxfont_rect = &(fset_extents->max_logical_extent);
     *font_heightp = Jxfont_height = Jxfont_rect->height;
     Jxline_height = Jxfont_height + 2;
     Jxwfont_w = Jxfont_rect->width;
     return( client );
     }

static int
JxDestroyTextWindow(display, client)
     Display *display;
     Window	client;
     {
     XFreeGC(display, Jxgc_on);
     XFreeGC(display, Jxgc_off);
     XDestroyWindow(display, client);
     }

static int
JxRealizeText(display, client)
     Display *display;
     Window	client;
     {
     int	i;

     Jxline_no = 0;
     for(i=0; i < Jxmax_line; i++) {
	 Jxsize[i] = 0;
	 Jxbuff[i][0] = 0;
	 }
     Jxcx = Jxcx_offset;
     Jxcy = Jxcy_offset; 
     XFillRectangle(display, client, Jxgc_on, Jxcx, Jxcy, Jxsfont_w, Jxfont_height);
     }

static int
JxRedisplayText(display, client, font_set)
     Display *display;
     Window	client;
     XFontSet	font_set;
     {
     int   i, fy;
     for(i=0; i < Jxmax_line; i++) {
         fy   = i * Jxline_height + Jxcy_offset - Jxfont_rect->y;
	 if( Jxsize[i] > 0 ){
             XmbDrawImageString(display, client, font_set, Jxgc_on, 
			   Jxcx_offset, fy, Jxbuff[i], Jxsize[i]);
		}
         }
     XFillRectangle(display, client, Jxgc_on,
			     Jxcx, Jxcy, Jxsfont_w, Jxfont_height);
     }

static int
_JxNextLine()
    {
     Jxcx = Jxcx_offset;
     Jxcy += Jxline_height;
     Jxline_no++;
     if(Jxline_no >= Jxmax_line) {
         Jxline_no = 0;
         Jxcy = Jxcy_offset;
         }
    Jxsize[Jxline_no] = 0;
    Jxbuff[Jxline_no][0] = 0;
    }

#define	LF	0x0a
#define	CR	0x0d
#define	TAB	0x09

static int
JxWriteText(display, client, font_set, len, string)
     Display *display;
     Window	client;
     XFontSet	font_set;
     int	len;
     char	*string;
     {
     int   fy;
     XFillRectangle(display, client, Jxgc_off, Jxcx, Jxcy, Jxsfont_w, Jxfont_height);
     if(len == 1 && 
	 (string[0] == LF || string[0] == TAB || string[0] == CR)) {
	 _JxNextLine();
         XFillRectangle(display, client, Jxgc_off, 0, Jxcy, Jxw_width, Jxfont_height);
	 }
     else {
	 if(Jxcx >= (Jxw_width - Jxwfont_w)
	 || (Jxsize[Jxline_no] + len) >=  256) {
	     _JxNextLine();
             XFillRectangle(display, client, Jxgc_off, 0, Jxcy, Jxw_width, Jxfont_height);
	 }
	 strncpy(&Jxbuff[Jxline_no][Jxsize[Jxline_no]], string, len);
	 Jxsize[Jxline_no] += len;
         fy   = -Jxfont_rect->y + Jxcy;
         XmbDrawImageString(display, client, font_set, Jxgc_on, 
			      Jxcx, fy, string, len);
	 Jxcx += XmbTextEscapement(font_set, string, len);
	 if(Jxcx >= Jxw_width) {
	     _JxNextLine();
             XFillRectangle(display, client, Jxgc_off, 0, Jxcy, Jxw_width, Jxfont_height);
	     }
	 }
     XFillRectangle(display, client, Jxgc_on, Jxcx, Jxcy, Jxsfont_w, Jxfont_height);
     }
                                                                                                                                                                                                                                                                                                                                                                                                                                                           �               * [F]XIMDEMO.COM;1 +  , r�   .     /     4 W       �   �                - zp   0  � 1    2   3      K � P   W   O 	    5   6  z���֟  7 �����֟  8          9          G    H �� J �              $ $ ! Build the Motif ims_example DemoK $ ! The files ims_example.uid and ims_example.exe will be produced by this   $ ! command procedure. $ ! K $ ! Make sure that the prerequisite files and logicals exist.  This command L $ ! procedure requires that the DECwindows programming and user environmentsM $ ! have been installed and that the DECW$STARTUP command procedure has been   $ ! invoked.   $ ! " $ arch_type = f$getsyi("hw_model") $ if arch_type .le. 1023 $ then $    arch_type = "VAX" $ else $    arch_type = "AXP" $ endif J $ if f$search("sys$system:decw$uilmotif.exe") .nes. "" then goto libskitokF $ write sys$output "DECwindows programming environment not installed."0 $ write sys$output "ximdemo demo build aborted." $ exit $libskitok: < $ if f$search("sys$share:decw$xlibshr.exe") .nes. ""	.and. -J      f$search("sys$share:decw$dxmlibshr.exe") .nes. ""	then goto appskitok? $ write sys$output "DECwindows user environment not installed." 0 $ write sys$output "ximdemo demo build aborted." $ exit $appskitok: . $ if f$trnlnm("decw$include") .nes. ""	.and. -;      f$trnlnm("decw$examples") .nes. ""	then goto startupok O $ write sys$output "You must invoke @DECW$STARTUP before using this procedure." 0 $ write sys$output "ximdemo demo build aborted." $ exit $startupok:  $ if arch_type .eqs. "VAX" $ thenF $     if f$search("sys$system:vaxc.exe") .nes. "" then goto compilerok8 $     write sys$output "VAXC compiler is not available."4 $     write sys$output "ximdemo demo build aborted."
 $     exit $ elseO $     if f$search("sys$system:decc$compiler.exe") .nes. "" then goto compilerok 9 $     write sys$output "DEC C compiler is not available." 4 $     write sys$output "ximdemo demo build aborted."
 $     exit $ endif  $compilerok: $ if arch_type .eqs. "VAX" $ thenC $     if f$search("sys$library:stdio.h") .nes. "" then goto stdiook > $     write sys$output "SYS$LIBRARY:STDIO.H is not available."4 $     write sys$output "Create it with the command:"W $     write sys$output " $ LIBRARY/TEXT/EXTRACT=STDIO/OUT=SYS$COMMON:[SYSLIB]STDIO.H -" 0 $     write sys$output "    SYS$LIBRARY:VAXCDEF"4 $     write sys$output "ximdemo demo build aborted."
 $     exit $ elseK $     if f$search("sys$library:decc$rtldef.tlb") .nes. "" then goto stdiook > $     write sys$output "SYS$LIBRARY:STDIO.H is not available."4 $     write sys$output "Create it with the command:"W $     write sys$output " $ LIBRARY/TEXT/EXTRACT=STDIO/OUT=SYS$COMMON:[SYSLIB]STDIO.H -" 0 $     write sys$output "    SYS$LIBRARY:VAXCDEF"8 $     write sys$output "ims_example demo build aborted."
 $     exit $ endif 	 $stdiook:  $ !   $ ! Build ims_example and run it $ !  $ + $ write sys$output "Compiling ximdemo.C..." 6 $ define/user/nolog c$include decw$include,sys$library* $ define/user/nolog vaxc$include c$include $ if arch_type .eqs. "VAX" $ thenH $     cc /define=(_NO_PROTO,XK_LATIN1,VMS,VMSV5,"vms",__STARLET_LOADED,-C __LIBS$ROUTINES_LOADED,X_LOCALE,XLIB_XPG4_FUNCS,_WCHAR_T_,X_WCHAR,- 6 I18N_BUG_FIX,I18N_MULTIBYTE) /INCLUDE=(DECW$EXAMPLES,- �                                                                                                                                                                                                                                               