cmake_minimum_required(VERSION 3.22)
project(pal_cpp)

set(CMAKE_CXX_STANDARD 14)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wsign-compare -Wpedantic")


add_executable(dfs_colored_topsort 00_pre_fel/dfs-colored-topsort.cpp)
add_executable(kosaraju 00_pre_fel/kosaraju.cpp)
#add_executable(compile 99_pre_fel/compile_trial.cpp)

add_executable(calamity 01_calamity/calamity.cpp)
add_executable(prim_heap 01_calamity/prim_heap.cpp)
add_executable(f_assign 01_calamity/fast_dist_assign.cpp)

add_executable(pooldel 02_pooldel/pooldel.cpp)
add_executable(tarjan 02_pooldel/tarjan.cpp)

add_executable(netw 03_netw/netw.cpp)
add_executable(megafast 03_netw/megafast_generator.cpp)
add_executable(perms 03_netw/gen_perm.cpp)

add_executable(cnt 04_cnt_leh/cnt.cpp)
add_executable(cnt-prev 04_cnt_leh/cnt-prev.cpp)

add_executable(lsword 05_lsword/lsword.cpp)
add_executable(single 05_lsword/single_bfs.cpp)
add_executable(triple_search 05_lsword/triple_search.cpp)
add_executable(warning 05_lsword/WARNING_DONT_SUB.cpp)

add_executable(chain 06_chain_cutt/chain.cpp)

add_executable(smug 07_smug/smug.cpp)
add_executable(smug-hamet 07_smug/smug-hamet.cpp)
add_executable(smug-tivrek 07_smug/smug-tivrek.cpp)

# "Library"
add_executable(levenstein levenstein.cpp)
add_executable(boyermoore boyermoore.cpp)
add_executable(combinatorics combinatorics.cpp)
add_executable(bfs-paths-parents bfs-paths-parents.cpp)
add_executable(dfs-time-color dfs-time-color.cpp)
add_executable(connected connected.cpp)

# cizi ukoly

add_executable(hw3-khorin 03_netw/hw3-khorin.cpp)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
