# fpgamgr
FPGA Manager provides software to enable execution of tests on an FPGA platform, driven from a remote workstation.

## FPGA Server
The FPGA server is software running on a workstation connected directly to the FPGA board, or the embedded processor on-chip with the FPGA fabric. The FPGA server supports programming of the fabric, and communication with the design running on the fabric. 

## Boot Monitor
The Boot Monitor runs on an embedded processor within the design, and supports loading and running test firmware. 

## FPGA Server Client
The Server Client API enables interaction with the FPGA Server. 
- Loading an FPGA image to the fabric
- Low-level communication with the target (hardware or boot monitor)
- Loading and launching a software image
- Interacting with the software running on the target
- Receiving events from the design
