// TOP
#if !defined(FCODER_PHONK_CPP)
#define FCODER_PHONK_CPP

String_ID mapid_shared;
String_ID mapid_normal;
String_ID mapid_insert;
String_ID mapid_file;
String_ID mapid_goto;
String_ID mapid_delete;
String_ID mapid_yank;
String_ID mapid_misc;

u32 pk_numberOfActions;

//~ NOTE(Gonk): Custom commands
function b32
PkPeekBeginOfScope(Application_Links *app, Buffer_ID buffer, i64 pos, i64 *out_pos)
{
    b32 result = false;
    if (find_nest_side(app, buffer, pos - 1, FindNest_Scope | FindNest_Balanced, Scan_Backward, NestDelim_Open, out_pos))
    {
        result = true;
    }
    
    return result;
}

function b32
PkPeekEndOfScope(Application_Links *app, Buffer_ID buffer, i64 pos, i64 *out_pos)
{
    b32 result = false;
    if (find_nest_side(app, buffer, pos + 1, FindNest_Scope | FindNest_Balanced, Scan_Forward, NestDelim_Close, out_pos))
    {
        result = true;
    }
    
    return result;
}

function void
PkYankWord(Application_Links *app, b32 yankForward)
{
    View_ID view = get_active_view(app, 0);
    i64 originalMarkPos = view_get_mark_pos(app, view);
    i64 cursorPos = view_get_cursor_pos(app, view);
    view_set_mark(app, view, seek_pos(cursorPos));
    if (yankForward)
    {
        move_right_alpha_numeric_boundary(app);
    }
    else
    {
        move_left_alpha_numeric_boundary(app);
    }
    copy(app);
    view_set_mark(app, view, seek_pos(originalMarkPos));
    view_set_cursor_and_preferred_x(app, view, seek_pos(cursorPos));
}

function void
PkYankLine(Application_Links *app)
{
    View_ID view = get_active_view(app, 0);
    i64 originalMarkPos = view_get_mark_pos(app, view);
    i64 cursorPos = view_get_cursor_pos(app, view);
    seek_beginning_of_line(app);
    view_set_mark(app, view, seek_pos(cursorPos));
    seek_end_of_line(app);
    copy(app);
    view_set_mark(app, view, seek_pos(originalMarkPos));
    view_set_cursor_and_preferred_x(app, view, seek_pos(cursorPos));
}

function void
PkGotoLine(Application_Links *app, i64 line)
{
    View_ID view = get_active_view(app, 0);
    view_set_cursor_and_preferred_x(app, view, seek_line_col(line, 0));
}

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

CUSTOM_COMMAND_SIG(go_to_delete_mode)
{
    set_current_mapid(app, mapid_delete);
}

CUSTOM_COMMAND_SIG(go_to_yank_mode)
{
    set_current_mapid(app, mapid_yank);
}

CUSTOM_COMMAND_SIG(go_to_misc_mode)
{
    set_current_mapid(app, mapid_misc);
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

//~ NOTE(Gonk): All these are pretty much just copies of already existing
//    commands but it changes to normal model after the command is execute
CUSTOM_COMMAND_SIG(pk_goto_end_of_file_n)
{
    goto_end_of_file(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_goto_beginning_of_file_n)
{
    goto_beginning_of_file(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_open_file_in_quotes)
{
    open_file_in_quotes(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_save_all_dirty_n)
{
    save_all_dirty_buffers(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_save_n)
{
    save(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_project_go_to_root_dir)
{
    project_go_to_root_directory(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_interactive_new_n)
{
    interactive_new(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_interactive_switch_buf_n)
{
    interactive_switch_buffer(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_interactive_open_or_new)
{
    interactive_open_or_new(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_delete_forward_token_n)
{
    f4_delete_token_boundary(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_delete_range_n)
{
    delete_range(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_delete_backward_n)
{
    f4_delete_token_boundary(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_delete_line_n)
{
    delete_line(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_move_to_top_of_scope_n)
{
    i64 pos;
    View_ID view = get_active_view(app, Access_Read);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Read);
    i64 currPos = view_get_cursor_pos(app, view);
    
    if (!PkPeekBeginOfScope(app, buffer, currPos, &pos))
    {
        return;
    }
    
    view_set_active(app, view);
    Buffer_Seek seek = seek_pos(pos);
    set_view_to_location(app, view, buffer, seek);
    
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_move_to_bottom_of_scope_n)
{
    i64 pos;
    View_ID view = get_active_view(app, Access_Read);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Read);
    i64 currPos = view_get_cursor_pos(app, view);
    
    if (!PkPeekEndOfScope(app, buffer, currPos, &pos))
    {
        return;
    }
    
    view_set_active(app, view);
    Buffer_Seek seek = seek_pos(pos);
    set_view_to_location(app, view, buffer, seek);
    
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_move_to_bottom_of_scope_maximal_n)
{
    i64 pos;
    View_ID view = get_active_view(app, Access_Read);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Read);
    i64 currPos = view_get_cursor_pos(app, view);
    
    if (!PkPeekEndOfScope(app, buffer, currPos, &pos))
    {
        return;
    }
    
    currPos = pos;
    
    for (;;)
    {
        if (!PkPeekEndOfScope(app, buffer, currPos, &pos))
        {
            break;
        }
        
        currPos = pos;
    }
    
    view_set_active(app, view);
    Buffer_Seek seek = seek_pos(pos);
    set_view_to_location(app, view, buffer, seek);
    
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_move_to_top_of_scope_maximal_n)
{
    i64 pos;
    View_ID view = get_active_view(app, Access_Read);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Read);
    i64 currPos = view_get_cursor_pos(app, view);
    
    if (!PkPeekBeginOfScope(app, buffer, currPos, &pos))
    {
        return;
    }
    
    currPos = pos;
    if (currPos == 0)
    {
        return;
    }
    
    for (;;)
    {
        if (!PkPeekBeginOfScope(app, buffer, currPos, &pos) || currPos == 0)
        {
            break;
        }
        
        currPos = pos;
    }
    
    view_set_active(app, view);
    Buffer_Seek seek = seek_pos(pos);
    set_view_to_location(app, view, buffer, seek);
    
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(type_double_parentheses)
{
    write_string(app, string_u8_litexpr("()"));
    move_left(app);
}

CUSTOM_COMMAND_SIG(type_single_parentheses)
{
    write_string(app, string_u8_litexpr("("));
}

CUSTOM_COMMAND_SIG(type_double_quote_twice)
{
    write_string(app, string_u8_litexpr("\"\""));
    move_left(app);
}

CUSTOM_COMMAND_SIG(type_double_quote_once)
{
    write_string(app, string_u8_litexpr("\""));
}

CUSTOM_COMMAND_SIG(pk_yank_word_n)
{
    PkYankWord(app, 1);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_yank_word_backwards_n)
{
    PkYankWord(app, 0);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_yank_range_n)
{
    copy(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_yank_line_n)
{
    PkYankLine(app);
    go_to_normal_mode(app);
}

CUSTOM_COMMAND_SIG(pk_goto_line)
{
    PkGotoLine(app, pk_numberOfActions);
    go_to_normal_mode(app);
    pk_numberOfActions = 0;
}

//~
function void
PK_SetupBindings(Application_Links *app)
{
    String_ID global_map_id = vars_save_string_lit("keys_global");
    String_ID file_map_id = vars_save_string_lit("keys_file");
    String_ID code_map_id = vars_save_string_lit("keys_code");
    
    mapid_shared = vars_save_string_lit("mapid_shared");
    mapid_normal = vars_save_string_lit("mapid_normal");
    mapid_insert = vars_save_string_lit("mapid_insert");
    mapid_file   = vars_save_string_lit("mapid_file");
    mapid_goto   = vars_save_string_lit("mapid_goto");
    mapid_delete = vars_save_string_lit("mapid_delete");
    mapid_yank   = vars_save_string_lit("mapid_yank");
    mapid_misc   = vars_save_string_lit("mapid_misc");
    
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
    {
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
        Bind(go_to_delete_mode, KeyCode_D);
        Bind(go_to_yank_mode, KeyCode_Y);
        Bind(go_to_misc_mode, KeyCode_Space);
        
        Bind(command_lister, KeyCode_Semicolon);
        Bind(project_command_lister, KeyCode_Semicolon, KeyCode_Control);
        Bind(list_all_functions_current_buffer_lister, KeyCode_Semicolon, KeyCode_Control, KeyCode_Shift);
        
        Bind(kill_buffer, KeyCode_C, KeyCode_Shift, KeyCode_Control);
        
        Bind(center_view, KeyCode_6, KeyCode_Shift);
        
        Bind(f4_move_left, KeyCode_H);
        Bind(f4_move_right, KeyCode_L);
        Bind(move_up, KeyCode_K);
        Bind(move_down, KeyCode_J);
        
        Bind(move_down_to_blank_line_end, KeyCode_J, KeyCode_Shift);
        Bind(move_up_to_blank_line_end, KeyCode_K, KeyCode_Shift);
        
        Bind(pk_move_to_top_of_scope_n, KeyCode_LeftBracket);
        Bind(pk_move_to_bottom_of_scope_n, KeyCode_RightBracket);
        Bind(pk_move_to_top_of_scope_maximal_n, KeyCode_LeftBracket, KeyCode_Shift);
        Bind(pk_move_to_bottom_of_scope_maximal_n, KeyCode_RightBracket, KeyCode_Shift);
        
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
        
        Bind(set_mark, KeyCode_Space, KeyCode_Control);
        
        Bind(paste, KeyCode_P);
        
        Bind(f4_search, KeyCode_ForwardSlash);
        
        Bind(place_in_scope, KeyCode_LeftBracket, KeyCode_Control);
        
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
    }
    
    
    //~ @insert_mode
    // Basic typing
    {
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
        Bind(type_double_parentheses, KeyCode_9, KeyCode_Shift);
        Bind(type_single_parentheses, KeyCode_9, KeyCode_Control);
        Bind(type_double_quote_twice, KeyCode_Quote);
        Bind(type_double_quote_once, KeyCode_Quote, KeyCode_Control);
        Bind(write_zero_struct, KeyCode_0, KeyCode_Control); // NOTE(Gonk): C++
        Bind(f4_write_zero_struct, KeyCode_0, KeyCode_Control, KeyCode_Shift); // NOTE(Gonk): C
        
        Bind(backspace_char, KeyCode_Backspace);
        Bind(f4_backspace_token_boundary, KeyCode_Backspace, KeyCode_Control);
        Bind(f4_backspace_alpha_numeric_or_camel_boundary, KeyCode_Backspace, KeyCode_Shift);
    }
    
    //~ @goto_mode
    {
        SelectMap(mapid_goto);
        ParentMap(mapid_shared);
        Bind(f4_search_for_definition__project_wide, KeyCode_D);
        Bind(f4_search_for_definition__current_file, KeyCode_D, KeyCode_Shift);
        
        Bind(pk_goto_beginning_of_file_n, KeyCode_G);
        Bind(pk_goto_end_of_file_n, KeyCode_G, KeyCode_Shift);
        
        Bind(pk_open_file_in_quotes, KeyCode_1);
        
        Bind(casey_go_to_code_peek, KeyCode_Return, KeyCode_Shift);
        Bind(f4_code_peek, KeyCode_Return);
        
        Bind(pk_goto_line, KeyCode_L);
    }
    
    //~ @file_mode
    // Anything to do with files: navigating, opening, creating, etc.
    {
        SelectMap( mapid_file );
        ParentMap(mapid_shared);
        Bind(pk_interactive_switch_buf_n, KeyCode_O);
        Bind(pk_interactive_open_or_new, KeyCode_O, KeyCode_Control);
        Bind(open_in_other, KeyCode_O, KeyCode_Control, KeyCode_Shift);
        Bind(pk_interactive_new_n, KeyCode_N);
        Bind(pk_project_go_to_root_dir, KeyCode_H);
        Bind(pk_save_all_dirty_n, KeyCode_S);
        Bind(pk_save_n, KeyCode_S, KeyCode_Shift);
    }
    
    //~ @delete_mode
    {
        SelectMap(mapid_delete);
        ParentMap(mapid_shared);
        Bind(pk_delete_backward_n, KeyCode_B);
        Bind(pk_delete_forward_token_n, KeyCode_W);
        Bind(pk_delete_range_n, KeyCode_R);
        Bind(pk_delete_line_n, KeyCode_L);
    }
    
    //~ @yank_mode
    {
        SelectMap(mapid_yank);
        ParentMap(mapid_shared);
        
        Bind(pk_yank_word_n, KeyCode_W);
        Bind(pk_yank_word_backwards_n, KeyCode_B);
        Bind(pk_yank_line_n, KeyCode_L);
    }
    
    //~ @misc_mode
    {
        SelectMap(mapid_misc);
        ParentMap(mapid_shared);
        
        Bind(f4_toggle_compilation_expand, KeyCode_Comma);
    }
    
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