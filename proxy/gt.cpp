#include <iostream>
#include <fstream>
#include "gt.hpp"
#include "packet.h"
#include "server.h"
#include "utils.h"

std::string gt::version = "3.65";
std::string gt::flag = "ch";
bool gt::resolving_uid2 = false;
bool gt::connecting = false;
bool gt::in_game = false;
bool gt::ghost = false;

void gt::send_log(std::string text) {
    g_server->send(true, "action|log\nmsg|" + text, NET_MESSAGE_GAME_MESSAGE);
}

void gt::solve_captcha(std::string text) {
    //Get the sum :D
    utils::replace(text,
        "set_default_color|`o\nadd_label_with_icon|big|`wAre you Human?``|left|206|\nadd_spacer|small|\nadd_textbox|What will be the sum of the following "
        "numbers|left|\nadd_textbox|",
        "");
    utils::replace(text, "|left|\nadd_text_input|captcha_answer|Answer:||32|\nend_dialog|captcha_submit||Submit|", "");
    auto number1 = text.substr(0, text.find(" +"));
    auto number2 = text.substr(number1.length() + 3, text.length());
    int result = atoi(number1.c_str()) + atoi(number2.c_str());
    send_log("Solved captcha as `2" + std::to_string(result) + "``");
    g_server->send(false, "action|dialog_return\ndialog_name|captcha_submit\ncaptcha_answer|" + std::to_string(result));
}

void gt::updateVersion(std::string text) {
	utils::replace(text, "action|log\nmsg|`4UPDATE REQUIRED!`` : The `$V", "");
	utils::replace(text, "`` update is now available for your device.  Go get it!  You'll need to install it before you can play online.", "");
	gt::version = text;
	std::ofstream uv;
	uv.open("version.txt");
	uv << gt::version;
	uv.close();
	gt::send_log("Updating version Success! Please Re-Login for take the Effect");
}
