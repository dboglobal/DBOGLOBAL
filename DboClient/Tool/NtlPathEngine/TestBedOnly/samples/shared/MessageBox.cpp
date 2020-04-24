
#include "messagebox.h"
#include <windows.h>
#include <sstream>

void ShowMessageBox(const char* title, const char* contents)
{
    MessageBox(0, contents, title, MB_OK);
}
bool ShowMessageBoxQuery(const char* title, const char* contents, const char* query)
{
    std::ostringstream contentsWithQuery;
    contentsWithQuery << contents << std::endl;
    contentsWithQuery << query;
    int choice = MessageBox(0, contentsWithQuery.str().c_str(), title, MB_YESNO);
    return choice == IDYES;
}
