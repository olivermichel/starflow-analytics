
add_executable(pcap_flow_file_exporter
        src/etc/format_helpers.cc
        src/kernels/clfr_file_writer.cc
        src/kernels/clfr_printer.cc
        src/kernels/flow_table.cc
        src/kernels/pcap_file_reader.cc
        src/kernels/raw_packet_parser.cc
        src/modules/clfr_file_writer.cc
        src/modules/flow_table.cc
        src/modules/pcap_file_reader.cc
        src/modules/raw_packet_parser.cc
        src/app/pcap_flow_file_exporter_main.cc
        src/proto/starflow.pb.cc
        src/types/clfr.cc
        src/types/features.cc
        src/types/key.cc
        src/types/packet.cc
        src/types/raw_packet.cc)

target_link_libraries(pcap_flow_file_exporter protobuf)
target_link_libraries(pcap_flow_file_exporter pcap)
target_link_libraries(pcap_flow_file_exporter raft)