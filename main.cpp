#include "utility/Window.h"

int main(int argc, char **argv){
    auto window = Window();
    window.initialize(&argc,argv);
    window.run();
    return 0;
}
