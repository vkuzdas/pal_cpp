cmake_minimum_required(VERSION 3.22)
project(pal_cpp)

set(CMAKE_CXX_STANDARD 14)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wsign-compare -Wpedantic")


#add_executable(dfs_colored_topsort 00_pre_fel/dfs-colored-topsort.cpp)
#add_executable(kosaraju 00_pre_fel/kosaraju.cpp)
#add_executable(compile 99_pre_fel/compile_trial.cpp)

add_executable(calamity 01_calamity/calamity.cpp)
#add_executable(prim_heap 01_calamity/prim_heap.cpp)
#add_executable(f_assign 01_calamity/fast_dist_assign.cpp)

add_executable(pooldel 02_pooldel/pooldel.cpp)
#add_executable(tarjan 02_pooldel/tarjan.cpp)

add_executable(netw 03_netw/netw.cpp)
#add_executable(megafast 03_netw/megafast_generator.cpp)
add_executable(perms 03_netw/gen_perm.cpp)

add_executable(cnt 04_cnt_leh/cnt.cpp)
add_executable(cnt-prev 04_cnt_leh/cnt-prev.cpp)

#add_executable(lsword 05_lsword/lsword.cpp)
#add_executable(single 05_lsword/single_bfs.cpp)
add_executable(triple_search 05_lsword/triple_search.cpp)
#add_executable(warning 05_lsword/WARNING_DONT_SUB.cpp)

add_executable(chain 06_chain_cutt/chain.cpp)
#add_executable(garan 06_chain_cutt/garan.cpp)
#add_executable(occ 06_chain_cutt/occ.cpp)

add_executable(smug 07_smug/smug.cpp)
add_executable(2zk 12_2nd_zk/2zk.cpp)
add_executable(3zk 13_3rd_zk/3zk.cpp)
add_executable(4zk 14_4th_zk/4zk.cpp)
add_executable(fil 15_filip_zk/filzk.cpp)
add_executable(plg 13_3rd_zk/hello.cpp)
#add_executable(smug-hamet 07_smug/smug-hamet.cpp)
#add_executable(smug-tivrek 07_smug/smug-tivrek.cpp)

#add_executable(iauto 08_incomp_auto/iauto.cpp)
#add_executable(electri 09_electrification/electri_ham.cpp)
add_executable(river-kruskal 09_electrification/river_kruskal.cpp)
add_executable(zdebra 09_electrification/zdebra.cpp)
#add_executable(editor 10_editor/editor.cpp)
add_executable(surv-peaks 11_surv_peaks/surv_peaks.cpp)
add_executable(surv-peaks-BFS 11_surv_peaks/surv_peaks_BFS.cpp)
#add_executable(editor-dp 10_editor/editor-dp.cpp)

# "Library"
add_executable(combinatorics 00_lib/subsets/combinatorics.cpp)
#add_executable(subsets 00_lib/subsets/cp-alg-k-combinations.cpp)
#
#add_executable(levenstein 00_lib/text/levenstein.cpp)
#add_executable(boyermoore 00_lib/text/boyermoore.cpp)
#add_executable(hamming 00_lib/text/hamming.cpp)
#
#add_executable(bfs-paths-parents 00_lib/traversals/bfs-paths-parents.cpp)
#add_executable(dfs-time-color 00_lib/traversals/dfs-time-color.cpp)
#
#add_executable(isomo 00_lib/isomo/isomorfism_check.cpp)
#add_executable(netw_subseter 00_lib/subsets/netw_subseter.cpp)
#add_executable(tree-cert 00_lib/isomo/tree-cert.cpp)
#
#add_executable(tarjan-iter 00_lib/scc/tarjan.cpp)
#add_executable(kosaraju-scc 00_lib/scc/kosaraju.cpp)
#
#add_executable(connected 00_lib/connectivity.cpp)
#add_executable(MST 00_lib/MST.cpp)
#add_executable(topsort-cpa 00_lib/topsort.cpp)
#add_executable(skiplist 00_lib/skiplist.cpp)

# cizi ukoly

#add_executable(hw3-khorin 03_netw/hw3-khorin.cpp)
#add_executable(hamet-electri 98_hamet/zk_electrification/kruskalElectrification.cpp)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
