#include <stdio.h>
#include <stdlib.h>

static int enabled = -1;
static int reflection_pass;

void isaac_native_water_reflection(int active) {
    if (enabled < 0) {
        const char *setting = getenv("ISAAC_NATIVE_SKIP_REFLECTIONS");
        enabled = setting && *setting == '1';
        if (enabled) fputs("[native][gl] experimental water reflection skip active\n", stderr);
    }
    reflection_pass = active && enabled;
}

int isaac_native_skip_water_draw(void) {
    return reflection_pass;
}
