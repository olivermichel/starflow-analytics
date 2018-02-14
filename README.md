
# *Flow Analytics 

[![Build Status](https://travis-ci.org/olivermichel/starflow-analytics.svg?branch=master)](https://travis-ci.org/olivermichel/starflow-analytics)
### Dependencies

* libraft [[1]](https://github.com/RaftLib/RaftLib)
* libpcap [[2]](http://www.tcpdump.org)
* libprotobuf & protoc [[3]](https://github.com/google/protobuf)
* libgrpc [[4]](https://github.com/grpc/grpc)
* catch [[5]](https://github.com/catchorg/Catch2) (is automatically integrated when running cmake)
* cxxopts [[6]](https://github.com/jarro2783/cxxopts) (is automatically integrated when running cmake)
* gnuplot [[7]](http://www.gnuplot.info)

## Build

* requires `cmake >= 3.5` and `gcc >= 6`

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Run Unit Tests

```bash
make test
```

## Kernels

|Kernel Name         |Input                                 |Output                                |
|--------------------|--------------------------------------|--------------------------------------|
|CLFRFileReader      |*none*                                |CLFR                                  |
|CLFRFileWriter      |CLFR                                  |*none*                                |
|CLFRPrinter         |CLFR                                  |CLFR *or none*                        |
|FlowTable           |Key, Packet                           |CLFR                                  |
|GroupBy             |*T*                                   |*U*                                   |
|LiveCapture         |*none*                                |RawPacket                             |
|Map                 |*T*                                   |*U*                                   |
|Filter              |*T*                                   |*T*                                   |
|PacketPrinter       |Key, Packet                           |Key, Packet or none                   |
|PCAPFileReader      |*none*                                |RawPacket                             |
|Printer             |*T*                                   |*T* or none                           |
|RawPacketParser     |RawPacket                             |Key, Packet                           |
|TZSPReceiver        |*none*                                |RawPacket                             |
|Zip                 |I1, I2,                               |O = std::pair<I1,I2>                  |

## Run Docker Container

Pull the image:
```bash
docker pull olivermichel/starflow_analytics
```

Get interactive shell:
```bash
docker run -v /home/ubuntu:/root/mnt -it olivermichel/starflow_analytics /bin/bash
```

Generate CLFR Files:

```bash
docker run -v /home/ubuntu:/root/mnt olivermichel/starflow_analytics \
    /root/starflow_analytics/build/pcap_flow_file_exporter \
    -v -e ip -i /root/mnt/caida2015_02_dirA_10m.pcap -o /root/mnt/caida2015_02_dirA_10m.clfr
```

Run an example application:

```bash
docker run -v /home/ubuntu:/root/mnt olivermichel/starflow_analytics \
    /root/starflow_analytics/build/passthrough -b -i /root/mnt/caida2015_02_dirA_10m.clfr
```

## References

[1] https://github.com/RaftLib/RaftLib \
[2] http://www.tcpdump.org \
[3] https://github.com/google/protobuf \
[4] https://github.com/grpc/grpc \
[5] https://github.com/catchorg/Catch2 \
[6] https://github.com/jarro2783/cxxopts \
[7] http://www.gnuplot.info
