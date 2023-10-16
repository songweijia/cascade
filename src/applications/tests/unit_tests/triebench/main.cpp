#include <cinttypes>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <memory>
// #define PREFIX_REGISTRY_DEBUG
#include <cascade/detail/prefix_registry.hpp>
#include <cascade/utils.hpp>

using namespace derecho::cascade;

/**
 * @fn generate_string
 * @brief generate a string within a given range of length.
 * @param[in]   min_len     minimum length of the string
 * @param[in]   max_len     maximum length of the string
 * @return  generated string
 */
__attribute__(( visibility("hidden") ))
std::string generate_string(uint32_t min_len, uint32_t max_len) {
   uint32_t len = min_len + std::rand()%(max_len - min_len);
   char buf[len+1];
   for(int i=0;i<len;i++) {
       buf[i] = 'a'+std::rand()%26;
   }
   buf[len]='\0';
   return std::string(buf);
}

/**
 * @fn init
 * @brief initialize a tenary trie with `depth` deep. The component string length is 6~10
 * @param[in/out]   pr      The prefix registry tree. MUST be empty.
 * @param[in]       depth   The depth of the tree.
 * @param[out]      paths   The out put paths as library for test case.
 */
void init(PrefixRegistry<uint64_t>& pr,uint32_t depth,std::vector<std::string>& paths) {
    std::vector<std::pair<std::string,uint32_t>> stack;
    stack.push_back({"/",1});
    pr.register_prefix("/",std::rand());
    while(!stack.empty()) {
        uint32_t d;
        std::string p;
        std::tie(p,d) = stack.back();
        stack.pop_back();
        for (int child=0;child<3;child++) {
            std::string child_string = p + generate_string(6,12) + "/";
            pr.register_prefix(child_string,std::rand());
            if (d < depth) {
                stack.push_back({child_string,d+1});
            } else {
                paths.emplace_back(child_string);
            }
        }
    }
}

/**
 * @fn perf
 * @brief   Performance evaluation on the tenary trie
 */
void perf(PrefixRegistry<uint64_t>& pr,std::vector<std::string>& paths,uint32_t warmup_cnt,uint32_t perf_cnt) {
#define RUN(cnt) \
    { \
        uint32_t count = (cnt); \
        while(count--) { \
            for(auto& p: paths) { \
                pr.collect_values_for_prefixes(p+"obj",[](const std::string&,const std::shared_ptr<uint64_t>){}); \
            } \
        } \
    }
    // warm up
    RUN(warmup_cnt);
    // test 1000 times
    uint64_t s = get_time_ns();
    RUN(perf_cnt);
    uint64_t e = get_time_ns();

    uint64_t avg_ns = (e-s)/(perf_cnt*paths.size());

    std::cout << "average:" << avg_ns << "ns" << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage:" << argv[0]
                  << " <depth> <warmup_cnt> <perf_cnt>"
                  << std::endl;
        return 1;
    }

    uint32_t depth = std::stoul(argv[1]);
    uint32_t warmup_cnt = std::stoul(argv[2]);
    uint32_t perf_cnt = std::stoul(argv[3]);

    PrefixRegistry<uint64_t> pr;
    srand(std::time(0));
    std::vector<std::string> paths;
    init(pr,depth,paths);
    // pr.dump(std::cout,[](std::ostream&out,const uint64_t v){out<<v;});

    perf(pr,paths,warmup_cnt,perf_cnt);

    return 0;
}
