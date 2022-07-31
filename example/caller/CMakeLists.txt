
#set(SRC_LIST calluserservice.cc ../user.pb.cc)
set(SRC_LIST callechoservice.cc ../echo_service.pb.cc)
add_executable(consumer ${SRC_LIST})

target_link_libraries(consumer dajunrpc protobuf)