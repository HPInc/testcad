#ifndef TESTCAD
#define TESTCAD
#include <QString>
#include <QObject>

#define STRING_TESTCAD "testCAD"
#define STRING_VERSION "Version:"
#define STRING_VERSION_NUMBER "6.4.0"
#define STRING_AUTHORS "Bladimir de la Hoz Matveeva\r"

#define STRING_HISTORY_FILE "testCAD.history"

#define STRING_ABOUT_TESTCAD "About testCAD"
#define ABOUT_TEXT "<b>testCAD</b> is a tool designed to solve the following problems:<ul>"\
                     "<li>Hold composition information in a visually structured way.</li>"\
                     "<li>Quickly retrieve and add information to the stored structure.</li>"\
                     "<li>Keep track of the components status.</li>"\
                     "<li>Obtain statistical data from the stored information.</li>"\
                     "<li>Design tests visually from the stored information.</li>"\
                     "</ul><br><br>Author: Bladimir de la Hoz Matveeva."\
                     "<br><br> "

#define STRING_OPEN_DIALOG_TITLE "Open testCAD File"
#define STRING_FILETYPE_TESTCAD "testCAD Files (*.spec)"
#define TESTCAD_FILE_EXTENSION ".spec"
#define STRING_FILETYPE_HTML "HTML Files (*.html)"
#define STRING_FILETYPE_TXT "TXT Files (*.txt)"
#define STRING_PNG_IMAGE_FILE "PNG Image Files (*.png)"
#define STRING_FILE_LOADED "File loaded"
#define STRING_NO_FILE_LOADED "No file loaded"
#define STRING_CANNOT_READ "Cannot read file %1:\n%2."
#define STRING_PARSE_ERROR "Parse error at line %1, column %2:\n%3"
#define STRING_FILE_IS_NOT_TESTCAD "Invalid testCAD file."

#define STRING_COMPONENTS "Components"
#define STRING_COMMENTS "Comments"
#define STRING_READY "Ready"
#define STRING_SAVE_TESTCAD_FILE "Save testCAD File"
#define STRING_SAVE_TESTSTEPS_FILE "Save test steps to file"
#define STRING_CANNOT_WRITE_FILE "Cannot write file %1:\n%2."
#define STRING_FILE_SAVED "File saved"
#define STRING_FILE_NOT_SAVED "File not saved"
#define STRING_UNSAVEDCHANGES_DIALOG_TITLE "File has unsaved changes"
#define STRING_SAVE_BEFORE "Save before closing?"

#define TAG_SYSTEM "system"
#define TAG_TYPE_COMPONENT "component"
#define TAG_TYPE_MODULE "module"
#define TAG_TYPE_ENTRY "entry"
#define TAG_TYPE_TEST_GROUP "testGroup"

#define TAG_COMMENT "comment"
#define TAG_ATTRIBUTE_STATUS "status"
#define TAG_ATTRIBUTE_VERSION "version"
#define TAG_ATTRIBUTE_NAME "name"
#define TAG_ATTRIBUTE_FOLDED "folded"

#define STRING_NO "no"
#define STRING_YES "yes"
#define STRING_TOTAL_COUNT "Total count"
#define STRING_STATUS_PENDING "pending"
#define STRING_STATUS_PLANNED "planned"
#define STRING_STATUS_VALIDATED "validated"
#define STRING_STATUS_REVIEW "review"
#define STRING_STATUS_FAILED "failed"
#define STRING_STATUS_UNSUPPORTED "unsupported"

#define SEARCH_DIALOG_TITLE "Search..."
#define SEARCH_DIALOG_LABEL "Text to search for:"
#define STATS_DIALOG_TITLE "Statistics"
#define STATS_DIALOG_LABEL "Statistics by component:"
#define ADD_MULTI_DIALOG_LABEL "Write one item name by row:"

#define STRING_MENU_FILE "&File"
#define STRING_MENU_EDIT "&Edit"
#define STRING_MENU_INSERT "&Add"
#define STRING_MENU_VIEW "&View"
#define STRING_MENU_TOOLS "Tools"
#define STRING_MENU_TEST "Test"
#define STRING_MENU_HELP "&Help"

#define STRING_ACTION_APPEND "Append..."
#define STRING_ACTION_COMPARE "Compare with..."
#define STRING_ACTION_EXPORT_IMAGE "Export image"
#define STRING_ACTION_COPY_PATH_TO_CLIPBOARD "Copy path"
#define STRING_ACTION_COPY_BRANCH_TO_CLIPBOARD "Copy branch"
#define STRING_ACTION_SHOW_STATISTICS "Show statistics"
#define STRING_ACTION_COPY_STATISTICS "Copy statistics"
#define STRING_ACTION_SEARCH "&Search"
#define STRING_SHOW_VARIABLES "Show variables"

#define STRING_ACTION_OPEN "&Open..."
#define STRING_ACTION_SAVE "&Save"
#define STRING_ACTION_SAVE_AS "&Save As..."
#define STRING_ACTION_EXIT "E&xit"
#define STRING_ACTION_ABOUT "&About testCAD"

#define STRING_ACTION_CLEAR_HIGHLIGHTS "Clear Highlights"
#define STRING_ACTION_MARK_PENDING "Mark as &Pending"
#define STRING_ACTION_MARK_PLANNED "Mark as P&lanned"
#define STRING_ACTION_MARK_VALIDATED "Mark as &Validated"
#define STRING_ACTION_MARK_REVIEW "Mark as &Review"
#define STRING_ACTION_MARK_FAILED "Mark as &Failed"
#define STRING_ACTION_MARK_UNSUPPORTED "Mark as Unsupported"
#define STRING_ACTION_DELETE "Delete"
#define STRING_ACTION_UNDO "Undo"
#define STRING_ACTION_COPY "Copy"
#define STRING_ACTION_CUT "Cut"
#define STRING_ACTION_PASTE "Paste"
#define STRING_ACTION_MOVE_UP "Move &Up"
#define STRING_ACTION_MOVE_DOWN "Move &Down"

#define STRING_ACTION_ADD_COMPONENT "Add Component"
#define STRING_ACTION_ADD "Add..."
#define STRING_ACTION_SHOW_FAILED "Find Failed"
#define STRING_ACTION_SHOW_REVIEW "Find Review"
#define STRING_ACTION_SHOW_PLANNED "Find Planned"
#define STRING_ACTION_SHOW_PENDING "Find Pending"
#define STRING_ACTION_SHOW_VALIDATED "Find Validated"
#define STRING_ACTION_SHOW_UNSUPPORTED "Find Unsupported"

#define STRING_ACTION_COLLAPSE_ALL "Collapse All"
#define STRING_ACTION_EXPAND_ALL "Expand All"
#define STRING_ACTION_COLLAPSE "Collapse"
#define STRING_ACTION_EXPAND "Expand"

#define STRING_DESIGN "Design"
#define STRING_DESIGN_HAS_UNSAVED_CHANGES "Design has unsaved changes"
#define STRING_DESIGN_STORED "Design stored"
#define STRING_DESIGN_NOT_STORED "Design not stored"
#define STRING_ACTION_STORE "Store"
#define STRING_ACTION_EXPORT "Export"
#define STRING_DESIGN_EMPTY "Design is empty, nothing to do"

#define STRING_ACTION_ADD_TEST_GROUP "Add test group"
#define STRING_ACTION_ADD_TEST_COMBINATION "Add test combination"
#define STRING_ACTION_ADD_TEST_SEQUENCE "Add test sequence"
#define STRING_ACTION_ADD_TEST_CROSSCHECKER "Add cross checker"
#define STRING_ACTION_OPEN_DESIGNER "Open test designer"
//------------------------------------------------------------------------------------------------------------

#define STRING_COMBINER "Combination designer"
#define STRING_ACTION_GET_VARIABLES "Pick from tree"
#define STRING_ACTION_COMBINE_SELECTION "Combine"
#define STRING_ACTION_RANDOMIZE_SELECTION "Randomize"
#define STRING_ACTION_CLEAR_TABLE "Clear table"
#define STRING_ACTION_DELETE_SELECTED_COLUMNS "Delete"
#define STRING_ACTION_FILLDOWN_SELECTED_COLUMNS "Fill down"
#define STRING_COMBINATIONS "Combinations"
#define STRING_MENU_VARIABLES "Variables"
#define STRING_MENU_COLUMNS "Columns"
#define STRING_ACTION_MOVE_LEFT "Move left"
#define STRING_ACTION_MOVE_RIGHT "Move right"

#define STRING_COMBINATIONS_COUNT " Resulting combinations..."

#define STRING_COMBINER_HELP "The <b>combiner</b> window allows you to pick variables from clicked items "\
                     "in the tree so that you can collect variables in columns. Then you can select "\
                     "columns to combine or randomize their values."\
                     "<br><br><b>Example:</b>"\
                     "<br><br>  1. Click a item in the tree."\
                      "<br>  2. Click Variables and select <b>Pick variables</b> option."\
                      "<br>  3. Repeat steps above to populate your table with variables form the tree."\
                      "<br>  4. Click a column (hold Control key to select multiple columns)."\
                      "<br>  5. Click Variables and select <b>Combine</b> or <b>Randomize</b>."

#define TAG_TYPE_TEST_COMBINATION "combination"
#define TAG_TYPE_TEST_COMBINATION_COLUMN "combinationColumn"
#define TAG_ATTRIBUTE_TEST_COMBINATION_COLUMN_HEADER "combinationHeader"
#define TAG_TYPE_TEST_COMBINATION_TABLE_COLUMN_CELL "combinationCell"
#define TAG_ATTRIBUTE_TEST_COMBINATION_CELL_VALUE "combinationCellValue"
//------------------------------------------------------------------------------------------------------------

#define STRING_SEQUENCE_DESIGNER "Sequence designer"
#define STRING_DEFAULT_LABEL_STATE "State..."
#define STRING_DEFAULT_LABEL_EVENT "Event..."
#define STRING_ADD_STATE "Add State"
#define STRING_ADD_INITIAL_STATE "Add Initial State"
#define STRING_ADD_FINAL_STATE "Add Final State"
#define STRING_CONNECT_STATES "Connect..."
#define STRING_RESET_ZOOM "Reset Zoom"
#define STRING_CENTER_DIAGRAM  "Center diagram"
#define STRING_TESTS  "Tests"
#define STRING_NEW_DIAGRAM  "New diagram"
#define STRING_TESTS_AS_HTML  "Tests to html"
#define STRING_TESTS_AS_TXT "Tests by line"
#define STRING_CLEAR "Clear"
#define STRING_TEST_CASE "Test Case"
#define STRING_TEST_CASES "Test Cases"
#define STRING_STEP "Step"
#define STRING_ACTION "Action"
#define STRING_EXPECTED_RESULT "Expected result"
#define STRING_SWITCH_LEVEL "Switch"
#define LABEL_PIXELS 15
#define MARKER_SIZE 10

#define STRING_DIAGRAM_ERRORS "<b>Diagram has the following errors:</b><ul>"
#define STRING_DIAGRAM_PLEASE_CORRECT_ERRORS "Please correct the errors found"
#define STRING_ERROR_NO_INITIAL_STATE "<li>No initial state found</li>"
#define STRING_ERROR_MULTIPLE_INITIAL_STATES "<li>Multiple initial states, only one allowed</li>"
#define STRING_ERROR_INITIAL_STATE_HAS_INBOUND_TRANSITION "<li>Initial state cannot have inbound transitions</li>"
#define STRING_ERROR_INITIAL_STATE_HAS_NO_OUTBOUND_TRANSITION "<li>Initial state must have a outbound transition</li>"
#define STRING_ERROR_INITIAL_STATE_HAS_NO_UNIQUE_OUTBOUND_TRANSITION "<li>Initial state cannot have more than one outbound transition</li>"
#define STRING_ERROR_NO_INTERMEDIATE_STATE "<li>No intermediate state found</li>"
#define STRING_ERROR_INTERMEDIATE_STATE_WITHOUT_INBOUND_TRANSITION "<li>Found intermediate state without inbound transition</li>"
#define STRING_ERROR_INTERMEDIATE_STATE_WITHOUT_OUTBOUND_TRANSITION "<li>Found intermediate state without outbound transition</li>"
#define STRING_ERROR_NO_FINAL_STATE "<li>No final state found</li>"
#define STRING_ERROR_MULTIPLE_FINAL_STATES "<li>Multiple final states, only one allowed</li>"
#define STRING_ERROR_FINAL_STATE_WITHOUT_INBOUND_TRANSITION "<li>Final state must have at least one inbound transition</li>"
#define STRING_ERROR_FINAL_STATE_WITH_OUTBOUND_TRANSITION "<li>Final state cannot have outbound transitions</li>"
#define STRING_ERROR_ELEMENT_WITHOUT_TEXT "<li>Found elements with empty text</li>"

#define STRING_DIALOG_TEST_GENERATION "Generation"
#define STRING_DIALOG_SWITCH_LENGTH "Switch length [0..3]"
#define STRING_OPERATION_CANCELLED "Operation cancelled"

#define STRING_SEQUENCER_HELP "The <b>sequencer</b> window allows to visually design state-transition diagrams "\
                                "from where to derive state-based tests cases."\
                                "<br><br>The diagram design is basically a workflow in your tested system expressed "\
                                "as a <b>Finite State Machine (FSM)</b> from which test cases are derived applying the "\
                                "\"Chow's\" (Also known as \"Hop\" or \"N Switch\") method to achieve a state-transition "\
                                "coverage criteria of the \"Switch level\" (Length of consecutive state-transitions sequences) "\
                                "you specify."\
                                "<br><br><b>Your Sequence must comply with the following FSM principles:</b>"\
                                "<br>"\
                                "<br>  1. The diagram must have a unique Initial State."\
                                "<br>  2. The Initial State can have only one outbound transition."\
                                "<br>  3. The Initial State cannot have inbound transitions."\
                                "<br>  4. All intermediate States must have at least one inbound transition."\
                                "<br>  5. All Intermediate States must have at least one outbound transition."\
                                "<br>  6. The diagram must have a unique Final State."\
                                "<br>  7. The Final State can have only inbound transitions."\
                                "<br>  8. The Initial State cannot have outbound transitions."\
                                "<br>"\
                                "<br>Once you are ready with your Diagram design you may click the \"Tests to html\" button "\
                                "You will be prompted for a file name to which test cases with steps and expected "\
                                "results will be written in html format."\
                                "<br><br>You may also export your diagram as a .png image by clicking the \"Export image\" button."\
                                "<br><br>You may also want to export your test cases in a line by line format, this format is useful"\
                                " when you create a diagram expressing each state and transition as a function call for an automated"\
                                " test, the generated file will have the required sequence of function calls to accomplish the switch coverage set."

#define TAG_TYPE_TEST_SEQUENCE "sequence"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_ID "id"
#define TAG_TYPE_TEXT "text"
#define TAG_ATTRIBUTE_TEST_XTEXTBOXPOSITION "xTextBoxPosition"
#define TAG_ATTRIBUTE_TEST_YTEXTBOXPOSITION "yTextBoxPosition"

#define TAG_TYPE_TEST_SEQUENCE_STATE "state"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_STATE_XPOSITION "xPosition"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_STATE_YPOSITION "yPosition"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_STATE_TYPE "type"

#define TAG_TYPE_TEST_SEQUENCE_TRANSITION "transition"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_XSTART_POINT "xStartPoint"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_YSTART_POINT "yStartPoint"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_XEND_POINT "xEndPoint"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_YEND_POINT "yEndPoint"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_XBEZIER_FROM "xBezierFrom"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_YBEZIER_FROM "yBezierFrom"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_XBEZIER_TO "xBezierTo"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_YBEZIER_TO "yBezierTo"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_FROM_HOOK_INDEX "fromHookIndex"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_TO_HOOK_INDEX "toHookIndex"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_FROM_ID "fromId"
#define TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_TO_ID "toId"
#define STRING_STATES_COVERAGE "States coverage: "
#define STRING_TRANSITIONS_COVERAGE "Transitions coverage: "
#define STRING_SWITCHES_COVERAGE "State/Transition coverage: "
#define STRING_MISSED_STATE_TRANSITIONS "Missed State/Transitions"
#define STRING_MISSED_N_SWITCHES "Switches missed"
#define STRING_ALL_ZERO_SWITCHES_COVERED "All State/Transitions (0 Switch) covered"
#define STRING_ALL_N_SWITCHES_COVERED " Switches all covered"
#define STRING_STATE "State"
#define STRING_TRANSITION "Transition"
//------------------------------------------------------------------------------------------------------------

#define TAG_TYPE_TEST_CROSSCHECK "crossCheck"
#define TAG_TYPE_TEST_CROSSCHECK_PARENTS "crossParents"
#define TAG_ATTRIBUTE_CROSSCHECK_ROW_PARENT "crossRowParent"
#define TAG_ATTRIBUTE_CROSSCHECK_COLUMN_PARENT "crossColumnParent"
#define TAG_TYPE_TEST_CROSSCHECK_COLUMN "crossColumn"
#define TAG_ATTRIBUTE_CROSSCHECK_COLUMN_HEADER "crossColumnHeader"
#define TAG_TYPE_TEST_CROSSCHECK_CELL "crossCell"
#define TAG_TYPE_TEST_CROSSCHECK_ROW_HEADER "crossRowHeader"

#define STRING_CROSS_CHECKER "Cross check designer"
#define STRING_CROSS_COVERAGE "Cross coverage"
#define STRING_ROWS_SOURCE "Rows: "
#define STRING_COLUMNS_SOURCE "Columns: "

#define STRING_ACTION_GET_COLUMN_HEADERS "Pick column headers"
#define STRING_ACTION_GET_ROW_HEADERS "Pick row headers"
#define STRING_ACTION_COVER_ALL "Cover all"
#define STRING_ACTION_COVER_SELECTED "Cover selected"
#define STRING_ACTION_SHIFT_UP "Move &Up"
#define STRING_ACTION_SHIFT_DOWN "Move &Down"
#define STRING_COVERAGE "Coverage"
#define STRING_COVERAGE_PERCENT "Coverage percent [%]"
#define STRING_N "N"

#define STRING_CROSS_CHECKER_HELP "The <b>cross checker</b> window allows to quickly design cross-coverage tables "\
    "to plan and track activities involving for example areas and scenarios."\
    "<br><br>A typical example would be localization checks where asset areas are chosen to be covered for several locales. "\
    "Thanks to the navigation simplicity offered by the tree structure of a <b>testCAD</b> specification file "\
    "chosing the required data to create your cross check table is quick. "\
    "<br><br>Once you have built your table, the coverage selection is applied by simply clicking the \"Cover all\" and "\
    "Entering the desired coverage in percent. After that, the table cells will be tagged with the current date. Existing cell "\
    "values will not be overwriten."\
    "<br>This approach eases the tracking of the selection. But you are free to edit the cell contents after."\
    "<br><br><b>A typical workflow would be:</b>"\
    "<br>"\
    "<br>  1. Once the Cross Checker Window is open, click an item in the tree whose immediate children will be used as headers."\
    "<br>  2. Click the \"Pick horizontal headers\" or \"Pick vertical headers\" button depending on your choice for the selected item."\
    "<br>  3. Select a different item in the tree with which you would like to cross relate the children of the previously chosen item."\
    "<br>  4. Click the \"Pick horizontal headers\" or \"Pick vertical headers\" button depending on your previous choice to complete the table."\
    "<br>  5. The table will show vertical and horizontal headers based on your selection."\
    "<br>  6. Click the \"Cover all\" button and enter a coverage percentage value."\
    "<br>  7. Edit the cells of your choice if necessary."\
    "<br>  8. Export the table to html to share with your team."
//------------------------------------------------------------------------------------------------------------
#define TAG_TYPE_TEST_COLLECTION "collection"
#define TAG_TYPE_TEST_COLLECTION_COLUMN "collectionColumn"
#define TAG_ATTRIBUTE_TEST_COLLECTION_COLUMN_HEADER "collectionHeader"
#define TAG_TYPE_TEST_COLLECTION_TABLE_COLUMN_CELL "collectionCell"
#define TAG_ATTRIBUTE_TEST_COLLECTION_CELL_VALUE "collectionCellValue"

#endif // TESTCAD

