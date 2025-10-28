#pragma once
#include "Account.h"
#include "User.h"
#include "Types.h"
#include <vector>
#include <string>

class UIManager {
public:
    // Display functions
    static void displayWelcome();
    static void displayUserInfo(const User& user);
    static void displayAccountDetails(const Account& account);
    static void displayAccountList(const std::vector<Account>& accounts);
    
    // Input functions
    static bool promptYesNo(const std::string& question);
    static AccountType promptAccountType();
    static int promptMenuChoice(const std::vector<std::string>& options);
    
    // Messages
    static void showError(const std::string& message);
    static void showSuccess(const std::string& message);
    static void showInfo(const std::string& message);
};