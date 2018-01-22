
## *Flow Analytics 

[![Build Status](https://travis-ci.com/olivermichel/starflow_analytics.svg?token=Kp1zsMqFYWDB9S3qhPXx&branch=master)](https://travis-ci.com/olivermichel/starflow_analytics)
### Dependencies

* libraft [[1]](https://github.com/RaftLib/RaftLib)
* libpcap [[2]](http://www.tcpdump.org)
* libprotobuf & protoc [[3]](https://github.com/google/protobuf)
* libgrpc [[4]](https://github.com/grpc/grpc)
* catch [[5]](https://github.com/catchorg/Catch2) (is automatically integrated when running cmake)
* cxxopts [[6]](https://github.com/jarro2783/cxxopts) (is automatically integrated when running cmake)

### Build

* requires cmake >= 3.5 and gcc >= 6

```bash
mkdir build && cd build
cmake ..
make
```

### Run Unit Tests

```bash
make test
```

### Kernels

|Kernel Name         |Input                                 |Output                                |
|--------------------|--------------------------------------|--------------------------------------|
|CLFRFileExporter    |CLFR                                  |*none*                                |
|CLFRPrinter         |CLFR                                  |CLFR *or none*                        |
|FlowTable           |Key, Packet                           |CLFR                                  |
|LiveCapture         |*none*                                |RawPacket                             |
|Map                 |*any*                                 |*any*                                 |
|PacketFilter        |Key, Packet                           |Key, Packet                           |
|PacketPrinter       |Key, Packet                           |Key, Packet *or none*                 |
|PCAPFileReader      |*none*                                |RawPacket                             |
|Printer             |*any*                                 |*any or none*                         |
|RawPacketParser     |RawPacket                             |Key, Packet                           |
|TZSPReceiver        |*none*                                |RawPacket                             |

### References

[1] https://github.com/RaftLib/RaftLib

[2] http://www.tcpdump.org

[3] https://github.com/google/protobuf

[4] https://github.com/grpc/grpc

[5] https://github.com/catchorg/Catch2

[6] https://github.com/jarro2783/cxxopts