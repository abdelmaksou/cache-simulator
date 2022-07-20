#include  <cmath>    // for sin
#include <stdlib.h>  // for EXIT_SUCCESS
#include <memory>    // for allocator, __shared_ptr_access
#include <string>  // for string, operator+, basic_string, to_string, char_traits
#include <vector>  // for vector, __alloc_traits<>::value_type
#include <bits/stdc++.h> // for main simulator
 
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Menu, Renderer, Horizontal, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, Element, operator|, window, flex, vbox
 
using namespace ftxui;

#define		DBG				(1)
#define		DRAM_SIZE		(64 * 1024 * 1024)
#define		CACHE_SIZE		(64 * 1024)

int CACHE_LINE_SIZE = 16;
int NO_WAYS = 1;

int lastReplaced = 0;
int noLines = CACHE_SIZE / CACHE_LINE_SIZE;

std::vector<std::vector<std::pair<int,int>>> set_associative_cache (noLines, std::vector<std::pair<int,int>> (NO_WAYS, std::make_pair(0,0)));

int idx = log2(noLines);
int offset = log2(CACHE_LINE_SIZE);

void start (int line, int way) {
  CACHE_LINE_SIZE = line;
  NO_WAYS = way;
  lastReplaced = 0;
  noLines = CACHE_SIZE / CACHE_LINE_SIZE;
  std::vector<std::vector<std::pair<int,int>>> set_associative_cache_tmp (noLines, std::vector<std::pair<int,int>> (NO_WAYS, std::make_pair(0,0)));
  set_associative_cache = set_associative_cache_tmp;
  idx = log2(noLines);
  offset = log2(CACHE_LINE_SIZE);
}
 
enum cacheResType {MISS=0, HIT=1};
 
/* The following implements a random number generator */
unsigned int m_w = 0xABABAB55;      /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902;      /* must not be zero, nor 0x9068ffff */
 
unsigned int rand_() {
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;       /* 32-bit result */
}
 
unsigned int memGen1() {
	static unsigned int addr = 0;
	return (addr++) % (DRAM_SIZE);
}
 
unsigned int memGen2() {
	static unsigned int addr = 0;
	return  rand_() % (24 * 1024);
}
 
unsigned int memGen3() {
	return rand_() % (DRAM_SIZE);
}
 
unsigned int memGen4() {
	static unsigned int addr = 0;
	return (addr++) % (4 * 1024);
}
 
unsigned int memGen5() {
	static unsigned int addr = 0;
	return (addr++) % (1024 * 64);
}
 
unsigned int memGen6() {
	static unsigned int addr = 0;
	return (addr += 32) % (64 * 4 * 1024);
}
 
// Set Associative Cache Simulator
cacheResType cacheSimSA(unsigned int addr) {	
	// This function accepts the memory address for the read and 
	// returns whether it caused a cache miss or a cache hit
    addr = (addr >> offset);
    int cur_idx = addr % (1 << idx);
    int cur_tag = addr / (1 << idx); 
    int i = 0;
    while (i < NO_WAYS) {
        if (set_associative_cache[cur_idx][i].first) {
            if (set_associative_cache[cur_idx][i].second == cur_tag) return HIT;
        } 
        else break;
        i++;
    }
    if (i == NO_WAYS) {
        set_associative_cache[cur_idx][lastReplaced].second = cur_tag;
        (++lastReplaced) %= NO_WAYS;
    }
    else {
        set_associative_cache[cur_idx][i].first = 1;
        set_associative_cache[cur_idx][i].second = cur_tag;
    }
	return MISS;
}
 
std::string msg[2] = {"Miss","Hit"};
 
#define		NO_OF_Iterations	(1000000)		// Change to 1,000,000
 
std::vector<double> run(int exp, int gen, int line, int way) {
  std::vector<double> result;
  if (exp == 2){
    start(pow(2,line+4), pow(2,way));
    unsigned int hit = 0;
    cacheResType r;
  
    unsigned int addr;
  
    for (int inst = 0; inst < NO_OF_Iterations; inst++) {
      switch (gen)
      {
      case 0:
        addr = memGen1();
        break;
      case 1:
        addr = memGen2();
        break;
      case 2:
        addr = memGen3();
        break;
      case 3:
        addr = memGen4();
        break;
      case 4:
        addr = memGen5();
        break;
      case 5:
        addr = memGen6();
        break;
      
      default:
        addr = memGen1();
        break;
      }
      r = cacheSimSA(addr);
      if (r == HIT) hit++;
    }
    double hitRate = 100.0 * hit / NO_OF_Iterations;
    result.push_back(hitRate);
  } 
  else if (exp == 0) {
    for(int i = 0; i < 4; i++){
      start(pow(2,i+4), 4);
      unsigned int hit = 0;
      cacheResType r;
    
      unsigned int addr;
    
      for (int inst = 0; inst < NO_OF_Iterations; inst++) {
        switch (gen)
        {
        case 0:
          addr = memGen1();
          break;
        case 1:
          addr = memGen2();
          break;
        case 2:
          addr = memGen3();
          break;
        case 3:
          addr = memGen4();
          break;
        case 4:
          addr = memGen5();
          break;
        case 5:
          addr = memGen6();
          break;
        
        default:
          addr = memGen1();
          break;
        }
        r = cacheSimSA(addr);
        if (r == HIT) hit++;
      }
      double hitRate = 100.0 * hit / NO_OF_Iterations;
      result.push_back(hitRate);
    }
  } else {
    for(int i = 0; i < 4; i++){
      start(16, pow(2,i));
      unsigned int hit = 0;
      cacheResType r;
    
      unsigned int addr;
    
      for (int inst = 0; inst < NO_OF_Iterations; inst++) {
        switch (gen)
        {
        case 0:
          addr = memGen1();
          break;
        case 1:
          addr = memGen2();
          break;
        case 2:
          addr = memGen3();
          break;
        case 3:
          addr = memGen4();
          break;
        case 4:
          addr = memGen5();
          break;
        case 5:
          addr = memGen6();
          break;
        
        default:
          addr = memGen1();
          break;
        }
        r = cacheSimSA(addr);
        if (r == HIT) hit++;
      }
      double hitRate = 100.0 * hit / NO_OF_Iterations;
      result.push_back(hitRate);
    }
  }

  return result;
}
 
Component Window(std::string title, Component component) {
  return Renderer(component, [component, title] {  //
    return window(text(title), component->Render()) | flex;
  });
}
 
int main(int argc, const char* argv[]) {

  int menu_selected[] = {0, 0, 0, 0};
  std::vector<std::vector<std::string>> menu_entries = {
      {
          "memGen1()",
          "memGen2()",
          "memGen3()",
          "memGen4()",
          "memGen5()",
          "memGen6()",
      },
      {
          "1",
          "2",
          "4",
          "8",
      },
      {
          "16 Bytes",
          "32 Bytes",
          "64 Bytes",
          "128 Bytes",
      },
      {
          "# 1",
          "# 2",
          "None of Them",
      },
  };

  std::vector<double> res(4,0);

  auto on_click = [&] {res = run(menu_selected[3], menu_selected[0], menu_selected[2], menu_selected[1]);};
  auto button_style = ButtonOption::Animated(Color::Default, Color::Green1,
                                             Color::Default, Color::GreenLight);
 
  int menu_selected_global = 0;
  auto menu_global = Container::Vertical(
      {
          Window("Generator", Menu(&menu_entries[0], &menu_selected[0])),
          Window("Number of Ways", Menu(&menu_entries[1], &menu_selected[1])),
          Window("Line Size", Menu(&menu_entries[2], &menu_selected[2])),
          Window("Experiment", Menu(&menu_entries[3], &menu_selected[3])),
          Button("Start", on_click, &button_style) | flex | hcenter | border,
      },
      &menu_selected_global);
 
  auto info = Renderer([&] {
    std::string experiment = menu_entries[3][menu_selected[3]];
    std::string lineSize = menu_entries[2][menu_selected[2]];
    std::string ways = menu_entries[1][menu_selected[1]];
    std::string generator = menu_entries[0][menu_selected[0]];
    if(menu_selected[3] == 0){
      ways = "4";
      lineSize = "Variable";
    }
    else if (menu_selected[3] == 1){ 
      ways = "Variable";
      lineSize = "16 Bytes";
    }

    return window(text("Main"),  //
                    vbox({
                        text("Cache Size                    = 64 Kbytes"),
                        text("Cache Line Size               = " +
                            lineSize),
                        text("Number Of Ways                = " +
                            ways),
                        text("Generator Function            = " +
                            generator),
                        text("Computer Memory Address Space = 64 Mbytes"),
                        text("Memory References             = 1,000,000"),
                        text("Experiment                    = " +
                            experiment),
                    })
            ) | flex;
  });


  auto result = Renderer([&]{
    auto regular =  window(text("Result"),
                  vbox({
                    text("Hit Rate = " + std::to_string(res[0]) + "%"),
                    text("Miss Rate = " + std::to_string(100-res[0]) + "%"),
                  })
          );
    auto exp1 =  window(text("Result"),
                  vbox({
                    text("16 Bytes") | color(Color::GreenLight),
                    text("  Hit Rate = " + std::to_string(res[0]) + "%") ,
                    text("32 Bytes") | color(Color::GreenLight) ,
                    text("  Hit Rate = " + std::to_string(res[1]) + "%") ,
                    text("64 Bytes") | color(Color::GreenLight) ,
                    text("  Hit Rate = " + std::to_string(res[2]) + "%") ,
                    text("128 Bytes") | color(Color::GreenLight) ,
                    text("  Hit Rate = " + std::to_string(res[3]) + "%") ,
                  })
          );
    auto exp2 =  window(text("Result"),
                  vbox({
                    text("1 Way") | color(Color::GreenLight) ,
                    text("  Hit Rate = " + std::to_string(res[0]) + "%"),
                    text("2 Ways") | color(Color::GreenLight),
                    text("  Hit Rate = " + std::to_string(res[1]) + "%") ,
                    text("4 Ways") | color(Color::GreenLight) ,
                    text("  Hit Rate = " + std::to_string(res[2]) + "%") ,
                    text("8 Ways") | color(Color::GreenLight) ,
                    text("  Hit Rate = " + std::to_string(res[3]) + "%") ,
                  })
          );
    
    if(menu_selected[3] == 0){
      return exp1;
    }
    else if (menu_selected[3] == 1){ 
      return exp2;
    }
    
    return regular;

  });

  int shift = 0;

  auto my_graph = [&shift, &res](int width, int height) {
    std::vector<int> output(width);
    int j = 0;
    for (int i = width/4; i <= width; i+=width/4) {
      output[i] = res[j];
      j++;
    }
    return output;
  };

  auto graphs = Renderer([&] {
    auto hitLineSize = vbox({
        text("Hit Ratio [%] vs Line Size [bytes] (# ways = 4)") | hcenter,
        hbox({
            vbox({
                text("100 "),
                filler(),
                text("50 "),
                filler(),
                text("0 "),
            }),
            vbox({
              graph(my_graph) | color(Color::GreenLight) | border,
              hbox({
                text("0 "),
                filler(),
                text("16 "),
                filler(),
                text("32 "),
                filler(),
                text("64 "),
                filler(),
                text("128 "),
                }),
            }) | flex ,
        }) | flex,
    }) | border;

    auto hitWays = vbox({
        text("Hit Ratio [%] vs No. of Ways (Line Size = 16 bytes)") | hcenter,
        hbox({
            vbox({
                text("100 "),
                filler(),
                text("50 "),
                filler(),
                text("0 "),
            }),
            vbox({
              graph(my_graph) | color(Color::GreenLight) | border,
              hbox({
                text("0 "),
                filler(),
                text("1 "),
                filler(),
                text("2 "),
                filler(),
                text("4 "),
                filler(),
                text("8 "),
                }),
            }) | flex ,
        }) | flex,
    }) | border;

    auto final = hitLineSize;

    if(menu_selected[3] == 0){
      final = hitLineSize;
    }
    else if (menu_selected[3] == 1){ 
      final = hitWays;
    }
    else {
      final = text("None of The Experiments Were Chosen") | border | hcenter | vcenter | color(Color::RedLight) | flex;
    }

    return final | flex;
  });

  auto main = Container::Vertical({
    info,
    result,
  }) | flex;

  auto title = Renderer([&] {
    return text("Set Associative Cache Simulator") | bold | hcenter | color(Color::RedLight);
  });
 
  auto global = Container::Horizontal({
      menu_global,
      main,
      graphs,
  });

  auto htop = Container::Vertical({
    title,
  }) | border;

  auto home = Container::Vertical({
    htop,
    global,
  });
 
  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(home);
  return EXIT_SUCCESS;
}
