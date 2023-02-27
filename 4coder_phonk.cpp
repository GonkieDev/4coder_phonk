// TOP
#if !defined(FCODER_PHONK_CPP)
#define FCODER_PHONK_CPP

String_ID mapid_shared;
String_ID mapid_normal;
String_ID mapid_insert;
String_ID mapid_file;
String_ID mapid_goto;

u32 pk_numberOfActions;

//~ NOTE(Gonk): Modal stuff 
function void
set_current_mapid( Application_Links* app, Command_Map_ID mapid )
{
    View_ID view = get_active_view( app, 0 );
    Buffer_ID buffer = view_get_buffer( app, view, 0 );
    Managed_Scope scope = buffer_get_managed_scope( app, buffer );
    Command_Map_ID* map_id_ptr = scope_attachment( app, scope, buffer_map_id, Command_Map_ID );
    *map_id_ptr = mapid;
}

CUSTOM_COMMAND_SIG( go_to_normal_mode )
{
    set_current_mapid( app, mapid_normal );
    
    pk_numberOfActions = 0;
    
    active_color_table.arrays[ defcolor_cursor ].vals[ 0 ] = 0xffff5533;
    active_color_table.arrays[ defcolor_at_cursor ].vals[ 0 ] = 0xff00aacc;
}

CUSTOM_COMMAND_SIG( go_to_insert_mode )
{
    set_current_mapid( app, mapid_insert );
    
    active_color_table.arrays[ defcolor_cursor ].vals[ 0 ] = 0xff80ff80;
    active_color_table.arrays[ defcolor_at_cursor ].vals[ 0 ] = 0xff293134;
    active_color_table.arrays[ defcolor_margin_active ].vals[ 0 ] = 0xff80ff80;
}

CUSTOM_COMMAND_SIG(go_to_insert_mode_at_end_of_line)
{
    seek_end_of_line(app);
    go_to_insert_mode(app);
}

CUSTOM_COMMAND_SIG(go_to_insert_mode_at_beginning_of_line)
{
    seek_beginning_of_line(app);
    go_to_insert_mode(app);
}

CUSTOM_COMMAND_SIG(go_to_insert_mode_below)
{
    seek_end_of_line(app);
    casey_newline_and_indent(app);
    go_to_insert_mode(app);
}

// TODO(Gonk): This doesn't work when at top of the file
CUSTOM_COMMAND_SIG(go_to_insert_mode_above)
{
    move_up(app);
    go_to_insert_mode_below(app);
}

CUSTOM_COMMAND_SIG( go_to_file_mode )
{
    set_current_mapid( app, mapid_file );
    
    active_color_table.arrays[ defcolor_cursor ].vals[ 0 ] = 0xffffff00;
    active_color_table.arrays[ defcolor_at_cursor ].vals[ 0 ] = 0xff0000ff;
}

CUSTOM_COMMAND_SIG( go_to_goto_mode )
{
    set_current_mapid( app, mapid_goto );
    
    active_color_table.arrays[ defcolor_cursor ].vals[ 0 ] = 0xffffff00;
    active_color_table.arrays[ defcolor_at_cursor ].vals[ 0 ] = 0xff0000ff;
}

//~ Repeating action stuff

function void
PK_SetNumberOfActions(u32 i)
{
    if (pk_numberOfActions == 0)
    {
        pk_numberOfActions = i;
        return;
    }
    
    pk_numberOfActions = pk_numberOfActions * 10 + i;
}

CUSTOM_COMMAND_SIG( set_number_of_actions_0 ) { PK_SetNumberOfActions(0); }
CUSTOM_COMMAND_SIG( set_number_of_actions_1 ) { PK_SetNumberOfActions(1); }
CUSTOM_COMMAND_SIG( set_number_of_actions_2 ) { PK_SetNumberOfActions(2); }
CUSTOM_COMMAND_SIG( set_number_of_actions_3 ) { PK_SetNumberOfActions(3); }
CUSTOM_COMMAND_SIG( set_number_of_actions_4 ) { PK_SetNumberOfActions(4); }
CUSTOM_COMMAND_SIG( set_number_of_actions_5 ) { PK_SetNumberOfActions(5); }
CUSTOM_COMMAND_SIG( set_number_of_actions_6 ) { PK_SetNumberOfActions(6); }
CUSTOM_COMMAND_SIG( set_number_of_actions_7 ) { PK_SetNumberOfActions(7); }
CUSTOM_COMMAND_SIG( set_number_of_actions_8 ) { PK_SetNumberOfActions(8); }
CUSTOM_COMMAND_SIG( set_number_of_actions_9 ) { PK_SetNumberOfActions(9); }

//~
function void
PK_SetupBindings(Application_Links *app)
{
    String_ID global_map_id = vars_save_string_lit("keys_global");
    String_ID file_map_id = vars_save_string_lit("keys_file");
    String_ID code_map_id = vars_save_string_lit("keys_code");
    
    mapid_shared = vars_save_string_lit( "mapid_shared" );
    mapid_normal = vars_save_string_lit( "mapid_normal" );
    mapid_insert = vars_save_string_lit( "mapid_insert" );
    mapid_file   = vars_save_string_lit( "mapid_file" );
    mapid_goto   = vars_save_string_lit( "mapid_goto" );
    
    MappingScope( );
    SelectMapping( &framework_mapping );
    
    SelectMap( global_map_id );
    
    //~ @shared_mode
    SelectMap( mapid_shared );
    BindCore( fleury_startup, CoreCode_Startup );
    BindCore( default_try_exit, CoreCode_TryExit );
    Bind(exit_4coder, KeyCode_F4, KeyCode_Alt);
    BindMouseWheel(mouse_wheel_scroll);
    BindMouseWheel(mouse_wheel_change_face_size, KeyCode_Control);
    Bind( go_to_normal_mode, KeyCode_Escape );
    Bind( move_left, KeyCode_Left );
    Bind( move_right, KeyCode_Right );
    Bind( move_up, KeyCode_Up );
    Bind( move_down, KeyCode_Down );
    ParentMap(global_map_id);
    BindTextInput(fleury_write_text_input);
    BindMouse(click_set_cursor_and_mark, MouseCode_Left);
    BindMouseRelease(click_set_cursor, MouseCode_Left);
    BindCore(click_set_cursor_and_mark, CoreCode_ClickActivateView);
    BindMouseMove(click_set_cursor_if_lbutton);
    
    Bind(project_fkey_command, KeyCode_F1);
    Bind(project_fkey_command, KeyCode_F2);
    Bind(project_fkey_command, KeyCode_F3);
    Bind(project_fkey_command, KeyCode_F4);
    Bind(project_fkey_command, KeyCode_F5);
    Bind(project_fkey_command, KeyCode_F6);
    Bind(project_fkey_command, KeyCode_F7);
    Bind(project_fkey_command, KeyCode_F8);
    Bind(project_fkey_command, KeyCode_F9);
    Bind(project_fkey_command, KeyCode_F10);
    Bind(project_fkey_command, KeyCode_F11);
    Bind(project_fkey_command, KeyCode_F12);
    Bind(project_fkey_command, KeyCode_F13);
    Bind(project_fkey_command, KeyCode_F14);
    Bind(project_fkey_command, KeyCode_F15);
    Bind(project_fkey_command, KeyCode_F16);
    
    //~ @normal_mode
    // Access to other modes, and mainly movement.
    // Movement between panels, jumps, words, vertical movement;;
    SelectMap( mapid_normal );
    ParentMap( mapid_shared );
    
    //- Change to other modes 
    Bind( go_to_insert_mode, KeyCode_I );
    Bind(go_to_insert_mode_at_end_of_line, KeyCode_I, KeyCode_Shift);
    Bind(go_to_insert_mode_at_beginning_of_line, KeyCode_I, KeyCode_Control);
    Bind(go_to_insert_mode_below, KeyCode_O);
    Bind(go_to_insert_mode_above, KeyCode_O, KeyCode_Shift);
    Bind( go_to_file_mode, KeyCode_F );
    Bind(go_to_goto_mode, KeyCode_G);
    
    Bind(command_lister, KeyCode_Semicolon);
    Bind(project_command_lister, KeyCode_Semicolon, KeyCode_Control);
    Bind(list_all_functions_current_buffer_lister, KeyCode_Semicolon, KeyCode_Control, KeyCode_Shift);
    
    Bind(kill_buffer, KeyCode_C, KeyCode_Shift, KeyCode_Control);
    
    Bind(center_view, KeyCode_6, KeyCode_Shift);
    
    Bind(f4_move_left, KeyCode_H);
    Bind(f4_move_right, KeyCode_L);
    Bind(move_up, KeyCode_K);
    Bind(move_down, KeyCode_J);
    
    Bind(change_active_panel, KeyCode_Comma);
    Bind(change_active_panel_backwards, KeyCode_Comma, KeyCode_Shift);
    
    Bind(change_to_build_panel,         KeyCode_Comma, KeyCode_Control);
    Bind(close_build_panel,             KeyCode_Comma, KeyCode_Control, KeyCode_Shift);
    
    Bind(goto_next_jump,                KeyCode_N);
    Bind(goto_prev_jump,                KeyCode_N, KeyCode_Shift);
    Bind(goto_first_jump,               KeyCode_N, KeyCode_Control, KeyCode_Shift);
    
    Bind(move_right_token_boundary, KeyCode_W);
    Bind(move_left_token_boundary, KeyCode_B);
    
    Bind(seek_beginning_of_line, KeyCode_0, KeyCode_Shift);
    Bind(seek_end_of_line, KeyCode_4, KeyCode_Shift);
    
    Bind(undo, KeyCode_U);
    Bind(redo, KeyCode_U, KeyCode_Control);
    
    Bind(set_number_of_actions_0, KeyCode_0);
    Bind(set_number_of_actions_1, KeyCode_1);
    Bind(set_number_of_actions_2, KeyCode_2);
    Bind(set_number_of_actions_3, KeyCode_3);
    Bind(set_number_of_actions_4, KeyCode_4);
    Bind(set_number_of_actions_5, KeyCode_5);
    Bind(set_number_of_actions_6, KeyCode_6);
    Bind(set_number_of_actions_7, KeyCode_7);
    Bind(set_number_of_actions_8, KeyCode_8);
    Bind(set_number_of_actions_9, KeyCode_9);
    
    //~ @insert_mode
    // Basic typing
    SelectMap( mapid_insert );
    ParentMap( mapid_shared );
    BindTextInput( f4_write_text_and_auto_indent );
    
    Bind(word_complete, KeyCode_Tab);
    Bind(auto_indent_range, KeyCode_Tab, KeyCode_Control);
    Bind(auto_indent_line_at_cursor, KeyCode_Tab, KeyCode_Shift);
    Bind(word_complete_drop_down, KeyCode_Tab, KeyCode_Control);
    
    Bind(write_block, KeyCode_R, KeyCode_Control);
    Bind(write_todo, KeyCode_T, KeyCode_Control);
    Bind(write_note, KeyCode_N, KeyCode_Control);
    Bind(open_long_braces, KeyCode_LeftBracket, KeyCode_Control);
    Bind(open_long_braces_semicolon, KeyCode_LeftBracket, KeyCode_Control, KeyCode_Shift);
    Bind(open_long_braces_break, KeyCode_RightBracket, KeyCode_Control, KeyCode_Shift);
    Bind(write_zero_struct, KeyCode_0, KeyCode_Control); // NOTE(Gonk): C++
    Bind(f4_write_zero_struct, KeyCode_0, KeyCode_Control, KeyCode_Shift); // NOTE(Gonk): C
    
    Bind(backspace_char, KeyCode_Backspace);
    Bind(f4_backspace_token_boundary, KeyCode_Backspace, KeyCode_Control);
    Bind(f4_backspace_alpha_numeric_or_camel_boundary, KeyCode_Backspace, KeyCode_Shift);
    
    //~ @goto_mode
    SelectMap(mapid_goto);
    ParentMap(mapid_shared);
    Bind(f4_search_for_definition__project_wide, KeyCode_D);
    Bind(f4_search_for_definition__current_file, KeyCode_D, KeyCode_Shift);
    
    Bind(goto_beginning_of_file, KeyCode_G);
    Bind(goto_end_of_file, KeyCode_G, KeyCode_Shift);
    
    Bind(open_file_in_quotes, KeyCode_1);
    
    //~ @file_mode
    // Anything to do with files: navigating, opening, creating, etc.
    SelectMap( mapid_file );
    ParentMap(mapid_shared);
    Bind( go_to_normal_mode, KeyCode_Escape );
    Bind(interactive_new, KeyCode_N);
    Bind(interactive_open_or_new, KeyCode_O);
    Bind(open_in_other, KeyCode_O, KeyCode_Shift);
    Bind(project_go_to_root_directory, KeyCode_H);
    Bind(save_all_dirty_buffers, KeyCode_S);
    Bind(save, KeyCode_S, KeyCode_Shift);
    
    /* This is to make sure that the default bindings on the buffers will be mapid_normal. */
    SelectMap( file_map_id );
    ParentMap( mapid_normal );
    
    SelectMap( code_map_id );
    ParentMap( mapid_normal );
}

// NOTE(Gonk): Copy of default_4coder_initialize but without settings default bindings
function void
PK_DefaultInitialize(Application_Links *app, String_Const_u8_Array file_names, i32 override_font_size, b32 override_hinting)
{
#define M "Welcome to " VERSION "\nPhonk + Fleury Customisation"
    
    print_message(app, string_u8_litexpr(M));
#undef M
    
    Scratch_Block scratch(app);
    
    load_config_and_apply(app, &global_config_arena, override_font_size, override_hinting);
    
    // open command line files
    String_Const_u8 hot_directory = push_hot_directory(app, scratch);
    for (i32 i = 0; i < file_names.count; i += 1){
        Temp_Memory_Block temp(scratch);
        String_Const_u8 input_name = file_names.vals[i];
        String_Const_u8 full_name = push_u8_stringf(scratch, "%.*s/%.*s",
                                                    string_expand(hot_directory),
                                                    string_expand(input_name));
        Buffer_ID new_buffer = create_buffer(app, full_name, BufferCreate_NeverNew|BufferCreate_MustAttachToFile);
        if (new_buffer == 0){
            create_buffer(app, input_name, 0);
        }
    }
}


#endif // #if !defined(FCODER_PHONK_CPP)
// BOTTOM