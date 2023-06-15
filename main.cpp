/*******************************/
/********Includes/Set Up********/
/*******************************/

#include <fmt/core.h>
#include <stdio.h>

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

#include "FlightDB.h"
#include "Passenger.h"
#include "Attendent.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

using std::cout, std::cerr, std::cin, std::endl, std::unordered_set,
        std::to_string, std::string, std::vector, std::sort, std::stoi;

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to
// maximize ease of testing and compatibility with old VS compilers. To link
// with VS2010-era libraries, VS2015+ requires linking with
// legacy_stdio_definitions.lib, which we do using this pragma. Your own project
// should not be affected, as you are likely to link with a newer binary of GLFW
// that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See
// 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

// Declare global variables
FlightDB db;
std::regex flight_match{"([A-Z]{1}\\d{5})"};
std::regex passenger_match{"([A-Z]{1}\\d{5})"};
std::regex attendent_match{"([A-Z]{1}\\d{5})"};

// Attendent-related form submission validity
bool valid_id = true;
bool valid_pw = true;
bool valid_id_sign_up = true;
bool valid_pw_sign_up = true;
bool valid_first_name_sign_up = true;
bool valid_last_name_sign_up = true;
bool valid_address_sign_up = true;
bool valid_phone_sign_up = true;
bool valid_seat = true;

ImVec4 table_header_color = ImVec4(0.48f, 0.31f, 0.82f, 1.00f);

// Passenger-related form submission validity
string logged_in_passenger;
string logged_in_attendent;
bool show_log_in_window = true;
bool show_logged_in_window = false;
bool showAttendentWindow = false;
bool showPassengerWindow = false;
bool view_flights = false;
bool view_guide = false;


// Flight-related form submission validity
bool valid_flight_id = true;
bool valid_flight_from = true;
bool valid_flight_to = true;
bool valid_flight_year = true;
bool valid_flight_month = true;
bool valid_flight_day = true;
bool valid_flight_hour = true;
bool valid_flight_minute = true;

// Airport-related form submission validity
bool valid_airport_code = true;
bool valid_airport_name = true;

// Airport-distance-related form submission validity
bool valid_distance_from = true;
bool valid_distance_to = true;
bool valid_distance_time = true;

// Input filter
struct TextFilters {
    static int FilterFlightIDInput(ImGuiInputTextCallbackData *data) {
        if (data->EventChar < 256 &&
            strchr("Ff1234567890", (char)data->EventChar))
            return 0;
        return 1;
    }

    static int FilterPassengerIDInput(ImGuiInputTextCallbackData *data) {
        if (data->EventChar < 256 &&
            strchr("Pp1234567890", (char)data->EventChar))
            return 0;
        return 1;
    }

    static int FilterAttendentIDInput(ImGuiInputTextCallbackData *data) {
        if (data->EventChar < 256 &&
            strchr("Aa1234567890", (char)data->EventChar))
            return 0;
        return 1;
    }
};

// Declare functions (initialized at the bottom of file)
bool LoadTextureFromFile(const char *, GLuint *, int *, int *);

static void glfw_error_callback(int, const char *);

static void HelpMarker(const char *);

void reset();

void resetGUI();

bool validateInputFlightID(string);

bool validateFlight(string to, string from);

bool validateFlightDistance(string to, string from);

bool validateInputPassengerID(string);

bool validateInputAttendentID(string);

// Main code
int main(int, char **) {
    //Initialize flight database
    db = FlightDB{};
    db.reset();

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(
            1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //States
    int pwflags1 = ImGuiInputTextFlags_Password;
    bool showPW1 = false;
    int pwflags2 = ImGuiInputTextFlags_Password;
    bool showPW2 = false;
    int pwflags3 = ImGuiInputTextFlags_Password;
    bool showPW3 = false;

    bool isAddingStudentToCourse = false;
    bool isCreatingStudent = false;
    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not
    // attempt to do a fopen() of the imgui.ini file. You may manually call
    // LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
        // tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data
        // to your main application, or clear/overwrite your copy of the mouse
        // data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
        // data to your main application, or clear/overwrite your copy of the
        // keyboard data. Generally you may always pass all inputs to dear
        // imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        //Creates new frame
        ImGui::NewFrame();
        if (!showPW1)
            pwflags1 = ImGuiInputTextFlags_Password;
        else
            pwflags1 = 0;
        if (!showPW2)
            pwflags2 = ImGuiInputTextFlags_Password;
        else
            pwflags2 = 0;
        if (!showPW3)
            pwflags3 = ImGuiInputTextFlags_Password;
        else
            pwflags3 = 0;
        //Open "reset" window
        reset();

        /*
         * Main if statement to control the appearance of main window
         * - if showAttendentWindow is true, the window for attendents will show
         * - if showPassengerWindow is true, the window for passengers will show
         */
        if (view_guide){
            ImGui::Text("The initial window displays 2 buttons. Clicking on the “Attendent” button will open "
                        "the attendent-features-related window where you can sign in as an attendent, create an "
                        "attendent account or manage flights. Some specific features of the attendent window include: "
                        "tabs, tab buttons (where you can create/open flights/airports), listboxes, and tables. Clicking "
                        "on the “Passenger” button will open the passenger-features-related window where you can sign in "
                        "as a passenger, create a passenger account and buy or remove flights. Some specific features of "
                        "the passenger window include: listboxes. ")
        }else if (showAttendentWindow){
            if (show_logged_in_window){
                //Set up window size and position
                ImGui::SetNextWindowSize(ImVec2(1280, 720));
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing,
                                        ImVec2(0.5f, 0.5f));
                ImGui::Begin("##a", 0, ImGuiWindowFlags_NoTitleBar);
                //Set up window tabs
                static vector<std::string> active_tabs{};
                static int next_tab_id = 0;
                if (next_tab_id == 0) {
                    for (auto [flightID, flight] : db.getFlights()) {
                        if (flight.getAttedentID() ==
                            logged_in_attendent) {
                            active_tabs.push_back(flightID);
                            next_tab_id++;
                        }
                    }
                }

                static ImGuiTabBarFlags tab_bar_flags =
                        ImGuiTabBarFlags_AutoSelectNewTabs |
                        ImGuiTabBarFlags_Reorderable |
                        ImGuiTabBarFlags_FittingPolicyResizeDown |
                        ImGuiTabBarFlags_TabListPopupButton;

                if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
                    // Demo Trailing Tabs: click the "+" button to add a new tab
                    // (in your app you may want to use a font icon instead of
                    // the "+") Note that we submit it before the regular tabs,
                    // but because of the ImGuiTabItemFlags_Trailing flag it
                    // will always appear at the end.
                    if (ImGui::TabItemButton("OPEN",
                                             ImGuiTabItemFlags_Trailing |
                                             ImGuiTabItemFlags_NoTooltip)) {
                        ImGui::OpenPopup("OPEN FLIGHT");
                        ImGui::SetNextWindowSize(ImVec2(400, 200));
                        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing,
                                                ImVec2(0.5f, 0.5f));
                    }
                    //open popup for when user wants to open a flight
                    bool open_flight_window = true;
                    if (ImGui::BeginPopupModal("OPEN FLIGHT",
                                               &open_flight_window)) {
                        vector<string> items{};
                        for (auto [flightID, flight] : db.getFlights()) {
                            if (find(active_tabs.begin(), active_tabs.end(),
                                     flightID) == active_tabs.end() and
                                flight.getAttedentID() ==
                                logged_in_attendent) {
                                items.push_back(flightID);
                            }
                        }
                        static int item_current_idx = 0;  // Here we store our selection data as an index.
                        // Custom size: use all width, 5 items tall
                        if (!items.empty())
                            ImGui::Text("UNOPENED FLIGHTS:");
                        else
                            ImGui::Text(
                                    "ALL AVAILABLE FLIGHTS HAVE ALREADY BEEN OPENED.");
                        //Open listbox for user to choose which flight to open
                        if (ImGui::BeginListBox(
                                "##listbox",
                                ImVec2(
                                        -FLT_MIN,
                                        5 * ImGui::GetTextLineHeightWithSpacing()))) {
                            for (int n = 0; n < items.size(); n++) {
                                const bool is_selected = (item_current_idx == n);
                                if (ImGui::Selectable(items[n].c_str(),
                                                      is_selected))
                                    item_current_idx = n;

                                // Set the initial focus when opening the combo
                                // (scrolling + keyboard navigation focus)
                                if (is_selected) ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndListBox();
                        }
                        //Checking if a user is able to open a window (if user has opened all flights,
                        // there are no more flights to open)
                        if (!items.empty()) {
                            if (ImGui::Button("OPEN")) {
                                active_tabs.push_back(items[item_current_idx]);
                                next_tab_id++;
                                ImGui::CloseCurrentPopup();
                            }
                        } else {
                            if (ImGui::Button("CLOSE")) ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }

                    //Tab button to add an airport
                    if (ImGui::TabItemButton("ADD AIRPORT", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
                        ImGui::OpenPopup("CREATE AIRPORT");
                        ImGui::SetNextWindowSize(ImVec2(400, 200));
                        ImGui::SetNextWindowPos(center,
                                                ImGuiCond_Appearing,
                                                ImVec2(0.5f, 0.5f));
                    }

                    //open create airport popup
                    bool add_airport_window = true;
                    if (ImGui::BeginPopupModal("CREATE AIRPORT",
                                               &add_airport_window)) {
                        ImGui::Text("Airport Code");

                        static char code[4] = "";
                        string CodeLabel;
                        if (valid_airport_code)
                            CodeLabel = "##a";
                        else
                            CodeLabel = "INVALID CODE";
                        ImGui::InputText(CodeLabel.c_str(), code, 4,
                                         ImGuiInputTextFlags_EnterReturnsTrue |
                                         ImGuiInputTextFlags_CharsUppercase);

                        ImGui::Text("Name");
                        static char name[64] = "";
                        string NameLabel;
                        if (valid_airport_name)
                            NameLabel = "##b";
                        else
                            NameLabel = "REQUIRED";
                        ImGui::InputText(NameLabel.c_str(), name, 64,
                                         ImGuiInputTextFlags_EnterReturnsTrue |
                                         ImGuiInputTextFlags_CharsUppercase);

                        if (ImGui::Button("CREATE")) {
                            valid_airport_code = (strcmp(code, "") != 0 and db.getAirports().find(code) == db.getAirports().end());
                            valid_airport_name = strcmp(name, "") != 0;


                            if (valid_airport_code and valid_airport_name) {
                                db.addAirport(Airport{code, name});
                                db.save();
                                for (char& c : code) c = '\0';
                                for (char& c : name) c = '\0';
                                valid_airport_code = true;
                                valid_airport_name = true;
                                ImGui::OpenPopup("AIRPORT CREATION SUCCESSFUL!");
                            }
                        }

                        bool airport_creation_success_window = true;
                        if (ImGui::BeginPopupModal("AIRPORT CREATION SUCCESSFUL!",
                                                   &airport_creation_success_window)) {
                            ImGui::Text(fmt::format("AIRPORT WITH CODE {} HAS BEEN ADDED!", code).c_str());
                            add_airport_window = false;
                            ImGui::EndPopup();
                        }
                        ImGui::EndPopup();
                    }

                    //add "add distance" tab button
                    if (ImGui::TabItemButton("ADD DISTANCE", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
                        ImGui::OpenPopup("CREATE DISTANCE");
                        ImGui::SetNextWindowSize(ImVec2(400, 200));
                        ImGui::SetNextWindowPos(center,
                                                ImGuiCond_Appearing,
                                                ImVec2(0.5f, 0.5f));
                    }

                    //open popup to create a distance between airports
                    bool add_distance_window = true;
                    if (ImGui::BeginPopupModal("CREATE DISTANCE",
                                               &add_distance_window)) {
                        ImGui::Text("From (Airport Code)");
                        ImGui::SameLine();
                        HelpMarker("Airport codes must be 3-lettered codes (e.g. YOW, YCM, YGK, etc.). "
                                   "The airport must also first exist in the database. If an airport has not been created, "
                                   "please create it before creating the flight");
                        static char from[4] = "";
                        string FromLabel;
                        if (valid_distance_from)
                            FromLabel = "##b";
                        else
                            FromLabel = "INVALID CODE";
                        ImGui::InputText(FromLabel.c_str(), from, 4,
                                         ImGuiInputTextFlags_EnterReturnsTrue |
                                         ImGuiInputTextFlags_CharsUppercase);

                        ImGui::Text("To (Airport Code)");
                        static char to[4] = "";
                        string ToLabel;
                        if (valid_distance_to)
                            ToLabel = "##c";
                        else
                            ToLabel = "INVALID CODE##a";
                        ImGui::InputText(ToLabel.c_str(), to, 4,
                                         ImGuiInputTextFlags_EnterReturnsTrue |
                                         ImGuiInputTextFlags_CharsUppercase);

                        ImGui::Text("Time Between The Two Airports (in minutes)");
                        static char time[64] = "";
                        string TimeLabel;
                        if (valid_distance_time)
                            TimeLabel = "##a";
                        else
                            TimeLabel = "REQUIRED";
                        ImGui::InputText(TimeLabel.c_str(), time, 64,
                                         ImGuiInputTextFlags_EnterReturnsTrue);

                        if (ImGui::Button("CREATE")) {
                            valid_distance_from = (strcmp(from, to) != 0 and validateFlightDistance(to, from));
                            valid_distance_to = (strcmp(from, to) != 0 and validateFlightDistance(to, from));
                            valid_distance_time = strcmp(time, "") != 0;


                            if (valid_distance_from and valid_distance_to and valid_distance_time) {
                                db.getAirports()[to].addTimeToAirport(from, stoi(time));
                                db.save();
                                for (char& c : from) c = '\0';
                                for (char& c : to) c = '\0';
                                for (char& c : time) c = '\0';
                                valid_distance_from = true;
                                valid_distance_to = true;
                                valid_distance_time = true;
                                ImGui::OpenPopup("DISTANCE CREATION SUCCESSFUL!");
                                add_distance_window = false;
                            }
                        }

                        bool distance_creation_success_window = true;
                        if (ImGui::BeginPopupModal("DISTANCE CREATION SUCCESSFUL!",
                                                   &distance_creation_success_window)) {
                            ImGui::Text("SUCCESSFULLY CREATED!");
                            ImGui::EndPopup();
                        }
                        ImGui::EndPopup();
                    }


                    //Add tab button to create a flight
                    if (ImGui::TabItemButton("+",
                                             ImGuiTabItemFlags_Trailing |
                                             ImGuiTabItemFlags_NoTooltip)) {
                        ImGui::OpenPopup("CREATE FLIGHT");
                        ImGui::SetNextWindowSize(ImVec2(400, 200));
                        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing,
                                                ImVec2(0.5f, 0.5f));
                    }

                    //open popup to create a flight
                    bool add_flight_window = true;
                    if (ImGui::BeginPopupModal("CREATE FLIGHT",
                                               &add_flight_window)) {
                        ImGui::Text("FLIGHT ID");

                        static char id[7] = "";
                        string IDLabel;
                        if (valid_flight_id)
                            IDLabel = "##a";
                        else
                            IDLabel = "INVALID ID";
                        ImGui::InputText(IDLabel.c_str(), id, 7,
                                         ImGuiInputTextFlags_EnterReturnsTrue |
                                         ImGuiInputTextFlags_CharsUppercase |
                                         ImGuiInputTextFlags_CallbackCharFilter,
                                         TextFilters::FilterFlightIDInput);

                        ImGui::Text("From (Airport Code)");
                        ImGui::SameLine();
                        HelpMarker("Airport codes must be 3-lettered codes (e.g. YOW, YCM, YGK, etc.). "
                                   "The airport must also first exist in the database. If an airport has not been created, "
                                   "please create it before creating the flight");
                        static char from[4] = "";
                        string FromLabel;
                        if (valid_flight_from)
                            FromLabel = "##b";
                        else
                            FromLabel = "INVALID CODE";
                        ImGui::InputText(FromLabel.c_str(), from, 4,
                                         ImGuiInputTextFlags_EnterReturnsTrue |
                                         ImGuiInputTextFlags_CharsUppercase);

                        ImGui::Text("To (Airport Code)");
                        static char to[4] = "";
                        string ToLabel;
                        if (valid_flight_to)
                            ToLabel = "##c";
                        else
                            ToLabel = "INVALID CODE##a";
                        ImGui::InputText(ToLabel.c_str(), to, 4,
                                         ImGuiInputTextFlags_EnterReturnsTrue |
                                         ImGuiInputTextFlags_CharsUppercase);

                        ImGui::Text(fmt::format("{}", validateFlight(to, from)).c_str());
                        ImGui::Text("Year");
                        static char year[64] = "";
                        string YearLabel;
                        if (valid_flight_year)
                            YearLabel = "##d";
                        else
                            YearLabel = "REQUIRED";
                        ImGui::InputText(YearLabel.c_str(), year, 64,
                                         ImGuiInputTextFlags_EnterReturnsTrue);

                        ImGui::Text("Month");
                        static char month[64] = "";
                        string MonthLabel;
                        if (valid_flight_month)
                            MonthLabel = "##e";
                        else
                            MonthLabel = "REQUIRED##a";
                        ImGui::InputText(MonthLabel.c_str(), month, 64,
                                         ImGuiInputTextFlags_EnterReturnsTrue);

                        ImGui::Text("Day");
                        static char day[64] = "";
                        string DayLabel;
                        if (valid_flight_day)
                            DayLabel = "##f";
                        else
                            DayLabel = "REQUIRED##b";
                        ImGui::InputText(DayLabel.c_str(), day, 64,
                                         ImGuiInputTextFlags_EnterReturnsTrue);

                        ImGui::Text("Hour");
                        static char hour[64] = "";
                        string HourLabel;
                        if (valid_flight_hour)
                            HourLabel = "##g";
                        else
                            HourLabel = "REQUIRED##c";
                        ImGui::InputText(HourLabel.c_str(), hour, 64,
                                         ImGuiInputTextFlags_EnterReturnsTrue);

                        ImGui::Text("Minute");
                        static char minute[64] = "";
                        string MinuteLabel;
                        if (valid_flight_minute)
                            MinuteLabel = "##h";
                        else
                            MinuteLabel = "REQUIRED##d";
                        ImGui::InputText(MinuteLabel.c_str(), minute, 64,
                                         ImGuiInputTextFlags_EnterReturnsTrue);

                        if (ImGui::Button("CREATE")) {
                            valid_flight_id = validateInputFlightID(id);
                            valid_flight_from = (strcmp(from, to) != 0 and validateFlight(to, from));
                            valid_flight_to = (strcmp(from, to) != 0 and validateFlight(to, from));
                            valid_flight_year = strcmp(year, "") != 0;
                            valid_flight_month = strcmp(month, "") != 0;
                            valid_flight_day = strcmp(day, "") != 0;
                            valid_flight_hour = strcmp(hour, "") != 0;
                            valid_flight_minute = strcmp(minute, "") != 0;


                            if (valid_flight_id and valid_flight_from and
                                    valid_flight_to and valid_flight_year and
                                    valid_flight_month and valid_flight_day and valid_flight_hour and valid_flight_minute) {
                                db.addFlight(Flight{from, to, id, logged_in_attendent, stoi(year), stoi(month), stoi(day), stoi(hour), stoi(minute), db.getAirports()[to].getTimesToAirport()[from]});
                                db.save();
                                for (char& c : id) c = '\0';
                                for (char& c : to) c = '\0';
                                for (char& c : from) c = '\0';
                                for (char& c : year) c = '\0';
                                for (char& c : month) c = '\0';
                                for (char& c : day) c = '\0';
                                for (char& c : hour) c = '\0';
                                for (char& c : minute) c = '\0';
                                valid_flight_id = true;
                                valid_flight_from = true;
                                valid_flight_to = true;
                                valid_flight_year = true;
                                valid_flight_month = true;
                                valid_flight_day = true;
                                valid_flight_hour = true;
                                valid_flight_minute = true;
                                ImGui::OpenPopup("FLIGHT CREATION SUCCESSFUL!");
                            }
                        }

                        bool flight_creation_success_window = true;
                        if (ImGui::BeginPopupModal("FLIGHT CREATION SUCCESSFUL!",
                                                   &flight_creation_success_window)) {
                            ImGui::Text(fmt::format("A NEW FLIGHT FROM {} to {} HAS BEEN SUCCESSFULLY CREATED.", from, to).c_str());
                            add_flight_window = false;
                            ImGui::EndPopup();
                        }
                        ImGui::EndPopup();
                    }

                    // Submit our regular tabs (show tabs that are present in active_tabs vector)
                    for (int n = 0; n < active_tabs.size();) {
                        bool open = true;
                        char name[16];
                        snprintf(name, IM_ARRAYSIZE(name), "%s",
                                 active_tabs[n].c_str());
                        if (ImGui::BeginTabItem(name, &open,
                                                ImGuiTabItemFlags_None)) {
                            //begin a table to show all information about passengers on a certain flight
                            if (ImGui::BeginTable(
                                    fmt::format("Passengers of Flight {}", active_tabs[n])
                                            .c_str(),
                                    6,
                                    ImGuiTableFlags_Sortable |
                                    ImGuiTableFlags_SortMulti |
                                    ImGuiTableFlags_RowBg |
                                    ImGuiTableFlags_Borders |
                                    ImGuiTableFlags_BordersH |
                                    ImGuiTableFlags_BordersOuterH |
                                    ImGuiTableFlags_BordersInnerH |
                                    ImGuiTableFlags_BordersV |
                                    ImGuiTableFlags_BordersOuterV |
                                    ImGuiTableFlags_BordersInnerV |
                                    ImGuiTableFlags_BordersOuter |
                                    ImGuiTableFlags_BordersInner)) {
                                ImGui::TableSetBgColor(
                                        ImGuiTableBgTarget_RowBg0,
                                        ImGui::GetColorU32(table_header_color));
                                ImGui::TableSetupColumn("Seat", ImGuiTableColumnFlags_DefaultSort,0.0f);
                                ImGui::TableSetupColumn("Passenger ID", ImGuiTableColumnFlags_DefaultSort,0.0f);
                                ImGui::TableSetupColumn("First Name", 0, 0.0f);
                                ImGui::TableSetupColumn("Last Name", 0, 0.0f);
                                ImGui::TableSetupColumn("Phone Number", 0, 0.0f);
                                ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_NoSort, 0.0f);
                                ImGui::TableHeadersRow();

                                ImGui::TableNextRow();
                                for (int i = 0; i < db.getFlights()[active_tabs[n]].getPassengers().size(); i++) {
                                    ImGui::TableNextColumn();
                                    ImGui::Text("%s", fmt::format("{}", i).c_str());
                                    ImGui::TableNextColumn();
                                    ImGui::Text("%s", db.getFlights()[active_tabs[n]].getPassengers()[i]->getPassengerID().c_str());
                                    ImGui::TableNextColumn();
                                    ImGui::Text("%s", db.getFlights()[active_tabs[n]].getPassengers()[i]->getFirstName().c_str());
                                    ImGui::TableNextColumn();
                                    ImGui::Text("%s", db.getFlights()[active_tabs[n]].getPassengers()[i]->getLastName().c_str());
                                    ImGui::TableNextColumn();
                                    ImGui::Text(
                                            "%s", db.getFlights()[active_tabs[n]].getPassengers()[i]->getPhoneNumber().c_str());
                                    ImGui::TableNextColumn();
                                    ImGui::Text("%s", db.getFlights()[active_tabs[n]].getPassengers()[i]->getAddress().c_str());
                                }
                                ImGui::TableNextColumn();
                                if (ImGui::Button("Remove Flight")){
                                    db.getFlights().erase(active_tabs[n]);
                                    active_tabs.erase(active_tabs.begin() + n);
                                    db.save();
                                    ImGui::OpenPopup("FLIGHT DELETION SUCCESSFUL!");
                                }
                                bool flight_deletion_success_window = true;
                                if (ImGui::BeginPopupModal("FLIGHT DELETION SUCCESSFUL!",
                                                           &flight_deletion_success_window)) {
                                    ImGui::Text("FLIGHT HAS BEEN SUCCESSFULLY DELETED!");
                                    ImGui::EndPopup();
                                }
                                ImGui::EndTable();
                            }

                            ImGui::EndTabItem();
                        }
                        db.save();
                        // these if's control the opening and closing of new
                        // tabs
                        if (!open) {
                            active_tabs.erase(active_tabs.begin() + n);
                        } else
                            n++;
                    }
                    ImGui::EndTabBar();
                }
                ImGui::End();
            }else if (show_log_in_window){ //Log in window
                static float f = 0.0f;
                static int counter = 0;
                //Set up window size and position
                ImGui::SetNextWindowSize(ImVec2(400, 200));
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing,
                                        ImVec2(0.5f, 0.5f));
                // ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                // ImGuiWindowFlags_NoCollapse
                ImGui::Begin("Log in!", 0);
                ImGui::Text("Attendent ID");
                static char buf1[7] = "";
                string IDLabel;
                if (valid_id)
                    IDLabel = "##a";
                else
                    IDLabel = "INVALID ID";
                ImGui::InputText(
                        IDLabel.c_str(), buf1, 7,
                        ImGuiInputTextFlags_EnterReturnsTrue |
                        ImGuiInputTextFlags_CharsUppercase |
                        ImGuiInputTextFlags_CallbackCharFilter,
                        TextFilters::FilterAttendentIDInput);  // Display some text (you
                // can use a format strings
                // too)
                ImGui::Text("Password");
                static char password[64] = "";
                string PWLabel;
                if (valid_pw)
                    PWLabel = "##b";
                else
                    PWLabel = "NON-MATCHING PW";
                ImGui::InputText(PWLabel.c_str(), password, IM_ARRAYSIZE(password),
                                 pwflags1);
                ImGui::SameLine();
                ImGui::Checkbox("Show Password", &showPW1);
                //Validate sign in's
                if (ImGui::Button("Sign In")) {
                    valid_id = false;
                    valid_pw = false;
                    for (auto [attendentID, attendent] : db.getAttendents()) {
                        if (attendentID == buf1 and attendent.getPassword() == password) {
                            logged_in_attendent = buf1;
                            show_logged_in_window = true;
                            valid_id = true;
                            valid_pw = true;
                            for (char &c: buf1) c = '\0';
                            for (char &c: password) c = '\0';
                            break;
                        } else if (attendentID == buf1) {
                            valid_id = true;
                        }
                    }
                }
                //change appearance of window forr user to create a new attendent account
                ImGui::SameLine();
                ImGui::Text("New Attendent?");
                ImGui::SameLine();
                if (ImGui::Button("Create Attendent ID")) {
                    for (char &c: buf1) c = '\0';
                    for (char &c: password) c = '\0';
                    valid_id = true;
                    valid_pw = true;
                    showPW1 = false;
                    show_log_in_window = false;
                }
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                            1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }else{ // attendent account-creation window
                ImGui::SetNextWindowSize(ImVec2(400, 200));
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing,
                                        ImVec2(0.5f, 0.5f));
                // ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                // ImGuiWindowFlags_NoCollapse
                ImGui::Begin("Sign Up!", 0, ImGuiWindowFlags_NoResize);
                ImGui::Text("Attendent ID");
                static char id1[7] = "";
                string IDLabel;
                if (valid_id_sign_up)
                    IDLabel = "##a";
                else
                    IDLabel = "INVALID ID";
                ImGui::InputText(
                        IDLabel.c_str(), id1, 7,
                        ImGuiInputTextFlags_EnterReturnsTrue |
                        ImGuiInputTextFlags_CharsUppercase |
                        ImGuiInputTextFlags_CallbackCharFilter,
                        TextFilters::FilterAttendentIDInput);  // Display some text (you
                // can use a format strings
                // too)
                ImGui::NewLine();

                ImGui::Text("First Name");
                static char fn1[64] = "";
                string FNLabel;
                if (valid_first_name_sign_up)
                    FNLabel = "##b";
                else
                    FNLabel = "REQUIRED";
                ImGui::InputText(
                        FNLabel.c_str(), fn1,
                        64);  // Display some text (you can use a format strings too)
                ImGui::NewLine();

                ImGui::Text("Last Name");
                static char ln1[64] = "";
                string LNLabel;
                if (valid_last_name_sign_up)
                    LNLabel = "##c";
                else
                    LNLabel = "REQUIRED##a";
                ImGui::InputText(
                        LNLabel.c_str(), ln1,
                        64);  // Display some text (you can use a format strings too)
                ImGui::NewLine();

                ImGui::Text("Address");
                static char address1[64] = "";
                string ALabel;
                if (valid_address_sign_up)
                    ALabel = "##d";
                else
                    ALabel = "REQUIRED##b";
                ImGui::InputText(
                        ALabel.c_str(), address1,
                        64);  // Display some text (you can use a format strings too)
                ImGui::NewLine();

                ImGui::Text("Phone Number");
                static char phone[64] = "";
                string PLabel;
                if (valid_phone_sign_up)
                    PLabel = "##e";
                else
                    PLabel =
                            "INVALID PHONE NUMBER";
                ImGui::InputText(
                        PLabel.c_str(), phone,
                        64);  // Display some text (you can use a format strings too)
                ImGui::NewLine();

                ImGui::Text("Password");
                static char password2[64] = "";
                string PWLabel1, PWLabel2;
                if (valid_pw_sign_up) {
                    PWLabel1 = "##f";
                    PWLabel2 = "##g";
                } else {
                    PWLabel1 = "NON-MATCHING OR UNFILLED PASSWORDS";
                    PWLabel2 = "NON-MATCHING OR UNFILLED PASSWORDS##";
                }
                ImGui::InputText(PWLabel1.c_str(), password2,
                                 IM_ARRAYSIZE(password2), pwflags2);
                ImGui::SameLine();
                ImGui::Checkbox("Show Password", &showPW2);
                ImGui::NewLine();

                ImGui::Text("Confirm Password");
                static char password3[64] = "";
                ImGui::InputText(PWLabel2.c_str(), password3,
                                 IM_ARRAYSIZE(password3), pwflags3);
                ImGui::SameLine();
                ImGui::Checkbox("Show Password##", &showPW3);
                ImGui::NewLine();

                if (ImGui::Button("Sign Up")) {
                    valid_id_sign_up = validateInputAttendentID(id1);
                    valid_pw_sign_up = (strcmp(password2, password3) == 0 and
                                        (strcmp(password2, "") != 0 and
                                         strcmp(password3, "") != 0));
                    valid_first_name_sign_up = strcmp(fn1, "") != 0;
                    valid_last_name_sign_up = strcmp(ln1, "") != 0;
                    valid_address_sign_up = strcmp(address1, "") != 0;
                    valid_phone_sign_up = strcmp(phone, "") != 0;

                    if (valid_id_sign_up and valid_pw_sign_up and
                        valid_first_name_sign_up and valid_last_name_sign_up and
                        valid_address_sign_up and valid_phone_sign_up) {
                        db.addAttendent(Attendent{fn1, ln1, address1, phone, id1,
                                                  password2});
                        db.save();
                        ImGui::OpenPopup("ID CREATION SUCCESSFUL!");
                    }
                }
                bool account_creation_success_window = true;
                if (ImGui::BeginPopupModal("ID CREATION SUCCESSFUL!",
                                           &account_creation_success_window)) {
                    ImGui::Text(
                            "YOUR ID HAS BEEN SUCCESSFULLY CREATED. \nLOG IN "
                            "THROUGH THE LOG IN WINDOW!");
                    ImGui::EndPopup();
                }
                if (!account_creation_success_window){
                    for (char &c: id1) c = '\0';
                    for (char &c: fn1) c = '\0';
                    for (char &c: ln1) c = '\0';
                    for (char &c: address1) c = '\0';
                    for (char &c: phone) c = '\0';
                    for (char &c: password2) c = '\0';
                    for (char &c: password3) c = '\0';
                    valid_id_sign_up = true;
                    valid_pw_sign_up = true;
                    valid_first_name_sign_up = true;
                    valid_last_name_sign_up = true;
                    valid_address_sign_up = true;
                    valid_phone_sign_up = true;
                    showPW2 = false;
                    showPW3 = false;
                    show_log_in_window = true;
                }

                if (ImGui::Button("Return To Log In Window")) {
                    for (char &c: id1) c = '\0';
                    for (char &c: fn1) c = '\0';
                    for (char &c: ln1) c = '\0';
                    for (char &c: address1) c = '\0';
                    for (char &c: phone) c = '\0';
                    for (char &c: password2) c = '\0';
                    for (char &c: password3) c = '\0';
                    valid_id_sign_up = true;
                    valid_pw_sign_up = true;
                    valid_first_name_sign_up = true;
                    valid_last_name_sign_up = true;
                    valid_address_sign_up = true;
                    valid_phone_sign_up = true;
                    showPW2 = false;
                    showPW3 = false;
                    show_log_in_window = true;
                }

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                            1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }
        }else if (showPassengerWindow){ // passenger window
            if (show_logged_in_window){ // passenger has logged in window
                if (view_flights) // passenger wants to view their current flights window
                {
                    static float f = 0.0f;
                    static int counter = 0;
                    ImGui::SetNextWindowSize(ImVec2(800, 400));
                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing,
                                            ImVec2(0.5f, 0.5f));
                    // ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                    // ImGuiWindowFlags_NoCollapse
                    ImGui::Begin("Purchased Flights!", 0);
                    vector<string> items{};
                    for (auto [flightID, flight]: db.getFlights()) {
                        for (int i = 0 ; i < flight.getPassengers().size(); i++){
                            if (flight.getPassengers()[i]->getPassengerID() == logged_in_passenger){
                                items.push_back(fmt::format("{} seat {}", flight.getFlightID(), i));
                            }
                        }
                    }
                    //Set up list box
                    static int item_current_idx = 0;
                    if (ImGui::BeginListBox(
                            "##listbox",
                            ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
                        for (int n = 0; n < items.size(); n++) {
                            const bool is_selected = (item_current_idx == n);
                            if (ImGui::Selectable(items[n].c_str(), is_selected))
                                item_current_idx = n;
                            if (is_selected) ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndListBox();
                    }
                    //remove purchased flight
                    ImGui::Text("Is there a flight purchase you would like to remove?");
                    if (ImGui::Button("REMOVE")) {
                        db.getFlights()[items[item_current_idx].substr(0,  items[item_current_idx].find(' '))].removeSeat((int)items[item_current_idx][items[item_current_idx].length()]);
                        db.save();
                        items.erase(items.begin() + item_current_idx);
                    }
                    ImGui::End();
                }else {
                    static float f = 0.0f;
                    static int counter = 0;
                    //Set up window size and position
                    ImGui::SetNextWindowSize(ImVec2(800, 400));
                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing,
                                            ImVec2(0.5f, 0.5f));
                    // ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                    // ImGuiWindowFlags_NoCollapse
                    ImGui::Begin("Buy Flights!", 0);
                    vector<string> items{};
                    for (auto [flightID, flight]: db.getFlights()) {
                        items.push_back(flight.toString());
                    }
                    //Begin list box
                    static int item_current_idx = 0;
                    if (ImGui::BeginListBox(
                            "##listbox",
                            ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
                        for (int n = 0; n < items.size(); n++) {
                            const bool is_selected = (item_current_idx == n);
                            if (ImGui::Selectable(items[n].c_str(), is_selected))
                                item_current_idx = n;
                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) &&
                                ImGui::BeginTooltip()) {
                                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 20.0f);
                                ImGui::TextUnformatted(
                                        db.getFlights()[items[n]].getDetails().c_str());
                                ImGui::PopTextWrapPos();
                                ImGui::EndTooltip();
                            }

                            // Set the initial focus when opening the combo (scrolling +
                            // keyboard navigation focus)
                            if (is_selected) ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndListBox();
                    }
                    //Ask user which seat they would like to purchase
                    ImGui::Text("Seat Number You Want To Buy");
                    static char buf1[2] = "";
                    string seatLabel;
                    if (valid_seat)
                        seatLabel = "##a";
                    else
                        seatLabel = "SEAT ALREADY TAKEN";
                    ImGui::InputText(
                            seatLabel.c_str(), buf1, 7,
                            ImGuiInputTextFlags_EnterReturnsTrue |
                            ImGuiInputTextFlags_CharsUppercase);  // Display some text (you
                    // can use a format strings
                    // too)
                    if (ImGui::Button("BUY")) {
                        if (db.getFlights()[items[item_current_idx]].getSeatTaken()[std::stoi(buf1)]) {
                            valid_seat = false;
                        } else {
                            valid_seat = true;
                        }

                        if (valid_seat) {
                            db.getFlights()[items[item_current_idx]].buySeat(&db.getPassengers()[logged_in_passenger],
                                                                             std::stoi(buf1));
                            db.save();
                            ImGui::OpenPopup("SEAT PURCHASE SUCCESSFUL!");
                            ImGui::SetNextWindowSize(ImVec2(400, 100));
                            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing,
                                                    ImVec2(0.5f, 0.5f));
                        }
                    }
                    bool open_buy_seat_success_window = true;
                    if (ImGui::BeginPopupModal("SEAT PURCHASE SUCCESSFUL!",
                                               &open_buy_seat_success_window)) {
                        ImGui::Text(fmt::format("YOU HAVE SUCCESSFULLY PURCHASED SEAT {}", buf1).c_str());
                        for (char &c: buf1) c = '\0';
                        ImGui::EndPopup();
                    }

                    if (ImGui::Button("View Purchased Flights")){
                        view_flights = true;
                    }
                    ImGui::End();
                }
            }else if (show_log_in_window){
                static float f = 0.0f;
                static int counter = 0;
                ImGui::SetNextWindowSize(ImVec2(400, 200));
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing,
                                        ImVec2(0.5f, 0.5f));
                // ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                // ImGuiWindowFlags_NoCollapse
                ImGui::Begin("Log in!", 0);
                ImGui::Text("Passenger ID");
                static char buf1[7] = "";
                string IDLabel;
                if (valid_id)
                    IDLabel = "##a";
                else
                    IDLabel = "INVALID ID";
                ImGui::InputText(
                        IDLabel.c_str(), buf1, 7,
                        ImGuiInputTextFlags_EnterReturnsTrue |
                        ImGuiInputTextFlags_CharsUppercase |
                        ImGuiInputTextFlags_CallbackCharFilter,
                        TextFilters::FilterPassengerIDInput);  // Display some text (you
                // can use a format strings
                // too)
                ImGui::Text("Password");
                static char password[64] = "";
                string PWLabel;
                if (valid_pw)
                    PWLabel = "##b";
                else
                    PWLabel = "NON-MATCHING PW";
                ImGui::InputText(PWLabel.c_str(), password, IM_ARRAYSIZE(password),
                                 pwflags1);
                ImGui::SameLine();
                ImGui::Checkbox("Show Password", &showPW1);
                if (ImGui::Button("Sign In")) {
                    valid_id = false;
                    valid_pw = false;
                    for (auto [passengerID, passenger] : db.getPassengers()) {
                        if (passengerID == buf1 and passenger.getPassword() == password) {
                            logged_in_passenger = buf1;
                            show_logged_in_window = true;
                            valid_id = true;
                            valid_pw = true;
                            for (char &c: buf1) c = '\0';
                            for (char &c: password) c = '\0';
                            break;
                        } else if (passengerID == buf1) {
                            valid_id = true;
                        }
                    }
                }
                ImGui::SameLine();
                ImGui::Text("New Passenger?");
                ImGui::SameLine();
                if (ImGui::Button("Create Passenger ID")) {
                    for (char &c: buf1) c = '\0';
                    for (char &c: password) c = '\0';
                    valid_id = true;
                    valid_pw = true;
                    showPW1 = false;
                    show_log_in_window = false;
                }
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                            1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }else{ // create passenger account window
                ImGui::SetNextWindowSize(ImVec2(400, 200));
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing,
                                        ImVec2(0.5f, 0.5f));
                // ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                // ImGuiWindowFlags_NoCollapse
                ImGui::Begin("Sign Up!", 0, ImGuiWindowFlags_NoResize);
                ImGui::Text("Passenger ID");
                static char id1[7] = "";
                string IDLabel;
                if (valid_id_sign_up)
                    IDLabel = "##a";
                else
                    IDLabel = "INVALID ID";
                ImGui::InputText(
                        IDLabel.c_str(), id1, 7,
                        ImGuiInputTextFlags_EnterReturnsTrue |
                        ImGuiInputTextFlags_CharsUppercase |
                        ImGuiInputTextFlags_CallbackCharFilter,
                        TextFilters::FilterPassengerIDInput);  // Display some text (you
                // can use a format strings
                // too)
                ImGui::NewLine();

                ImGui::Text("First Name");
                static char fn1[64] = "";
                string FNLabel;
                if (valid_first_name_sign_up)
                    FNLabel = "##b";
                else
                    FNLabel = "REQUIRED";
                ImGui::InputText(
                        FNLabel.c_str(), fn1,
                        64);  // Display some text (you can use a format strings too)
                ImGui::NewLine();

                ImGui::Text("Last Name");
                static char ln1[64] = "";
                string LNLabel;
                if (valid_last_name_sign_up)
                    LNLabel = "##c";
                else
                    LNLabel = "REQUIRED##a";
                ImGui::InputText(
                        LNLabel.c_str(), ln1,
                        64);  // Display some text (you can use a format strings too)
                ImGui::NewLine();

                ImGui::Text("Address");
                static char address1[64] = "";
                string ALabel;
                if (valid_address_sign_up)
                    ALabel = "##d";
                else
                    ALabel = "REQUIRED##b";
                ImGui::InputText(
                        ALabel.c_str(), address1,
                        64);  // Display some text (you can use a format strings too)
                ImGui::NewLine();

                ImGui::Text("Phone Number");
                static char phone[64] = "";
                string PLabel;
                if (valid_phone_sign_up)
                    PLabel = "##e";
                else
                    PLabel =
                            "INVALID PHONE NUMBER";
                ImGui::InputText(
                        PLabel.c_str(), phone,
                        64);  // Display some text (you can use a format strings too)
                ImGui::NewLine();

                ImGui::Text("Password");
                static char password2[64] = "";
                string PWLabel1, PWLabel2;
                if (valid_pw_sign_up) {
                    PWLabel1 = "##f";
                    PWLabel2 = "##g";
                } else {
                    PWLabel1 = "NON-MATCHING OR UNFILLED PASSWORDS";
                    PWLabel2 = "NON-MATCHING OR UNFILLED PASSWORDS##";
                }
                ImGui::InputText(PWLabel1.c_str(), password2,
                                 IM_ARRAYSIZE(password2), pwflags2);
                ImGui::SameLine();
                ImGui::Checkbox("Show Password", &showPW2);
                ImGui::NewLine();

                ImGui::Text("Confirm Password");
                static char password3[64] = "";
                ImGui::InputText(PWLabel2.c_str(), password3,
                                 IM_ARRAYSIZE(password3), pwflags3);
                ImGui::SameLine();
                ImGui::Checkbox("Show Password##", &showPW3);
                ImGui::NewLine();

                if (ImGui::Button("Sign Up")) {
                    valid_id_sign_up = validateInputPassengerID(id1);
                    valid_pw_sign_up = (strcmp(password2, password3) == 0 and
                                        (strcmp(password2, "") != 0 and
                                         strcmp(password3, "") != 0));
                    valid_first_name_sign_up = strcmp(fn1, "") != 0;
                    valid_last_name_sign_up = strcmp(ln1, "") != 0;
                    valid_address_sign_up = strcmp(address1, "") != 0;
                    valid_phone_sign_up = strcmp(phone, "") != 0;

                    if (valid_id_sign_up and valid_pw_sign_up and
                        valid_first_name_sign_up and valid_last_name_sign_up and
                        valid_address_sign_up and valid_phone_sign_up) {
                        db.addPassenger(Passenger{fn1, ln1, address1, phone, id1,
                                              password2});
                        db.save();
                        ImGui::OpenPopup("ID CREATION SUCCESSFUL!");
                    }
                }
                bool account_creation_success_window = true;
                if (ImGui::BeginPopupModal("ID CREATION SUCCESSFUL!",
                                           &account_creation_success_window)) {
                    ImGui::Text(
                            "YOUR ID HAS BEEN SUCCESSFULLY CREATED. \nLOG IN "
                            "THROUGH THE LOG IN WINDOW!");
                    ImGui::EndPopup();
                }
                if (!account_creation_success_window){
                    for (char &c: id1) c = '\0';
                    for (char &c: fn1) c = '\0';
                    for (char &c: ln1) c = '\0';
                    for (char &c: address1) c = '\0';
                    for (char &c: phone) c = '\0';
                    for (char &c: password2) c = '\0';
                    for (char &c: password3) c = '\0';
                    valid_id_sign_up = true;
                    valid_pw_sign_up = true;
                    valid_first_name_sign_up = true;
                    valid_last_name_sign_up = true;
                    valid_address_sign_up = true;
                    valid_phone_sign_up = true;
                    showPW2 = false;
                    showPW3 = false;
                    show_log_in_window = true;
                }

                if (ImGui::Button("Return To Log In Window")) {
                    for (char &c: id1) c = '\0';
                    for (char &c: fn1) c = '\0';
                    for (char &c: ln1) c = '\0';
                    for (char &c: address1) c = '\0';
                    for (char &c: phone) c = '\0';
                    for (char &c: password2) c = '\0';
                    for (char &c: password3) c = '\0';
                    valid_id_sign_up = true;
                    valid_pw_sign_up = true;
                    valid_first_name_sign_up = true;
                    valid_last_name_sign_up = true;
                    valid_address_sign_up = true;
                    valid_phone_sign_up = true;
                    showPW2 = false;
                    showPW3 = false;
                    show_log_in_window = true;
                }

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                            1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }
        }else{ // initial window for user to choose passenger or attendent sign in
            ImGui::SetNextWindowSize(ImVec2(800, 400));
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing,
                                    ImVec2(0.5f, 0.5f));
            ImGui::Begin("##a", 0, ImGuiWindowFlags_NoTitleBar);
            if (ImGui::Button("ATTENDENT WINDOW")) {
                showAttendentWindow = true;
            }

            if (ImGui::Button("PASSENGER WINDOW")) {
                showPassengerWindow = true;
            }

            if (ImGui::Button("USER GUIDE")){
                view_guide = true;
            }
            ImGui::End();
        }

        // Renders panels
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

/*************************/
/********Functions********/
/*************************/

// Simple helper function to load an image into a OpenGL texture with common
// settings
bool LoadTextureFromFile(const char *filename, GLuint *out_texture,
                         int *out_width, int *out_height) {
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char *image_data =
            stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL) return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);  // This is required on WebGL for non
    // power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    GL_CLAMP_TO_EDGE);  // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

//Helper function to generate a help marker
static void HelpMarker(const char *desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) &&
        ImGui::BeginTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 20.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

//Helper function to validate a flight
bool validateFlight(string to, string from) {
    if (db.getAirports().find(to) == db.getAirports().end()) return false;
    return db.getAirports()[to].getTimesToAirport().find(from) != db.getAirports()[to].getTimesToAirport().end();
}

//Helper function to validate a flight time
bool validateFlightDistance(string to, string from) {
    if (db.getAirports().find(to) == db.getAirports().end()) return false;
    return db.getAirports()[to].getTimesToAirport().find(from) == db.getAirports()[to].getTimesToAirport().end();
}

//Function to validate if a string is a valid Flight ID
bool validateInputFlightID(string in) {
    if (!std::regex_match(in, flight_match)) return false;
    return db.getFlights().find(in) == db.getFlights().end();
}

//Function to validate if a string is a valid passenger ID
bool validateInputPassengerID(string in) {
    if (!std::regex_match(in, passenger_match)) return false;
    return db.getPassengers().find(in) == db.getPassengers().end();
}

//Function to validate if a string is a valid attendent ID
bool validateInputAttendentID(string in) {
    if (!std::regex_match(in, attendent_match)) return false;
    return db.getAttendents().find(in) == db.getAttendents().end();
}

//Open debug log window
void reset() {
    ImGui::Begin("reset gui?");
    if (ImGui::Button("Reset?"))
        resetGUI();
    ImGui::End();
}

void resetGUI(){
    logged_in_passenger = "";
    logged_in_attendent = "";
    show_log_in_window = true;
    show_logged_in_window = false;
    showAttendentWindow = false;
    showPassengerWindow = false;
    db.save();
}