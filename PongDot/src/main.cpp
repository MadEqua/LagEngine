#include "App.h"

 int main() {
    App app;

    if (!app.start())
        return 1;

    return 0;
}