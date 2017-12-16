
find_library(PCAP_LIB pcap)
find_file(PCAP_INC pcap)

if(NOT PCAP_LIB OR NOT PCAP_INC)
    message(WARNING "pcap: not found")
else()
    message(STATUS "pcap: PCAP_LIB:${PCAP_LIB}, PCAP_INC:${PCAP_INC}")
endif(NOT PCAP_LIB OR NOT PCAP_INC)
