#include "../PrismTextureStreamerFB/final_draw_discovery.h"
#include <cassert>
#include <iostream>
using final_draw_discovery::eligible_source;
using final_draw_discovery::potential_draw;
using DiscoveryMode = final_draw_discovery::mode_t;
int main()
{
    assert(eligible_source(DiscoveryMode::strict,6,1024,1024,29,29,4,8));
    assert(!eligible_source(DiscoveryMode::strict,6,1024,1024,78,78,4,8));
    assert(!eligible_source(DiscoveryMode::strict,0,1024,1024,29,29,4,8));
    assert(!eligible_source(DiscoveryMode::strict,6,1024,512,29,29,4,8));
    assert(eligible_source(DiscoveryMode::broad,6,1024,1024,78,78,4,8));
    assert(eligible_source(DiscoveryMode::broad,6,1024,512,29,29,4,8));
    assert(eligible_source(DiscoveryMode::broad,0,512,512,87,87,4,8));
    assert(eligible_source(DiscoveryMode::broad,14,1024,1024,98,99,4,8));
    assert(!eligible_source(DiscoveryMode::broad,6,1024,1024,80,80,4,8));
    assert(!eligible_source(DiscoveryMode::broad,6,1024,1024,78,29,4,8));
    assert(!eligible_source(DiscoveryMode::broad,16,1024,1024,78,78,4,8));
    assert(!eligible_source(DiscoveryMode::broad,6,1024,1024,78,78,3,8));
    assert(!eligible_source(DiscoveryMode::broad,6,1024,1024,78,78,4,0));
    assert(!eligible_source(DiscoveryMode::broad,6,64,64,78,78,4,8));
    assert(!eligible_source(DiscoveryMode::broad,6,1024,4096,78,78,4,8));
    assert(!eligible_source(DiscoveryMode::broad,6,1234,512,78,78,4,8));
    assert(potential_draw(DiscoveryMode::broad,6));
    assert(potential_draw(DiscoveryMode::broad,12));
    assert(!potential_draw(DiscoveryMode::broad,1500));
    assert(!potential_draw(DiscoveryMode::broad,7));
    assert(potential_draw(DiscoveryMode::wide,1500));
    std::cout << "Final-draw discovery filters passed\n";
}
