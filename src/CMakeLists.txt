aux_source_directory(. SRC_LIST)

add_library(dajunrpc ${SRC_LIST})

target_link_libraries(dajunrpc muduo_net muduo_base pthread zookeeper_mt)