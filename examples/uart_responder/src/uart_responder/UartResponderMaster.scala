package uart_responder



import chisel3._
import chisel3.util._
import std_protocol_if.Wishbone

class UartResponderMaster(p : Wishbone.Parameters = new Wishbone.Parameters()) extends Module {
  
  val io = IO(new Bundle {
    val m = new Wishbone(p)
    val count = Output(UInt(4.W))
  })
 
  // Write to LCR
  // - Ack
  // Write to DL1
  // - Ack
  // Write to DL0
  // - Ack
  // Write to LCR
  // - Ack
  // Loop:
  // Read from LSR
  // - Ack and respond (back to LSR or proceed)
  // Read from Recv
  // - Ack and store data
  // Read from <Stat>
  // - Ack
  // Write to Tx
  // - Ack
  // Back to <Loop>
  val sInitUart_Idle :: sInitUart_LCR_1 :: sInitUart_DL1 :: sInitUart_DL0 :: sInitUart_LCR_2 :: sWaitAccDone :: sWaitRxAccDone :: sRx_ReadLSR_1 :: sRx_ReadLSR_2 :: sTx_ReadLSR_1 :: sTx_ReadLSR_2 :: Nil = Enum(11)
  //     0                   1                  2                3                4                  5              6                7                8                9                 10

  val state = Reg(init=sInitUart_Idle)
  val return_s = Reg(init=sInitUart_LCR_1)
  val data = Reg(UInt(p.DATA_WIDTH.W), init=0.asUInt())
  val rxdata = Reg(UInt(8.W), init=0.asUInt())
  val req = Reg(new Wishbone.ReqData(p))
  val count = Reg(UInt(16.W), init=0.asUInt())
  
  io.count := count(7,4)
  
  io.m.req := req
  
  switch (state) {
    is (sInitUart_Idle) {
      req.ADR := 0.asUInt()
      req.DAT_W := 0.asUInt()
      req.WE := 0.asUInt()
      req.CYC := 0.asUInt();
      req.SEL := 0.asUInt();
      req.STB := 0.asUInt();
      state := sInitUart_LCR_1
    }
    
    is (sInitUart_LCR_1) {
      req.ADR := 3.asUInt()
      req.DAT_W := 0x83000000L.asUInt() // N81+DLAB
      req.WE := 1.asUInt()
      req.STB := 1.asUInt()
      req.CYC := 1.asUInt()
      req.SEL := 0x8.asUInt()
      state := sWaitAccDone
      return_s := sInitUart_DL1
    }
    
    is (sInitUart_DL1) {
      req.ADR := 1.asUInt()
      req.DAT_W := 0x00000000.asUInt() // 
      req.WE := 1.asUInt()
      req.STB := 1.asUInt()
      req.CYC := 1.asUInt()
      req.SEL := 0x2.asUInt()
      state := sWaitAccDone
      return_s := sInitUart_DL0
    }

    is (sInitUart_DL0) {
      req.ADR := 0.asUInt()
      req.DAT_W := 27.asUInt() // 
      req.WE := 1.asUInt()
      req.STB := 1.asUInt()
      req.CYC := 1.asUInt()
      req.SEL := 0x1.asUInt()
      state := sWaitAccDone
      return_s := sInitUart_LCR_2
    }
    
    is (sInitUart_LCR_2) {
      req.ADR := 3.asUInt()
      req.DAT_W := 0x03000000L.asUInt() // N81
      req.WE := 1.asUInt()
      req.STB := 1.asUInt()
      req.CYC := 1.asUInt()
      req.SEL := 0x8.asUInt()
      state := sWaitAccDone
      return_s := sRx_ReadLSR_1
    }
    
    is (sWaitAccDone) {
      when (io.m.rsp.ACK) {
        // Go to the actual next state
        req.ADR := 0.asUInt()
        req.DAT_W := 0.asUInt()
        data := io.m.rsp.DAT_R
        req.WE := 0.asUInt()
        req.STB := 0.asUInt()
        req.CYC := 0.asUInt()
        req.SEL := 0.asUInt()
        state := return_s
      }
    }
    
    is (sWaitRxAccDone) {
      when (io.m.rsp.ACK) {
        // Go to the actual next state
        req.ADR := 0.asUInt()
        req.DAT_W := 0.asUInt()
        rxdata := io.m.rsp.DAT_R
        req.WE := 0.asUInt()
        req.STB := 0.asUInt()
        req.CYC := 0.asUInt()
        req.SEL := 0.asUInt()
        state := return_s
      }
    }
   
    // Read the LSR waiting for a character to receive
    is (sRx_ReadLSR_1) {
        req.ADR := 5.asUInt()
        req.DAT_W := 0.asUInt()
        req.WE := 0.asUInt()
        req.STB := 1.asUInt()
        req.CYC := 1.asUInt()
        req.SEL := 2.asUInt() // 
        return_s := sRx_ReadLSR_2
        state := sWaitAccDone
    }
    
    is (sRx_ReadLSR_2) { // 8
      when (data(0+8) === 1.asUInt()) { // byte available
        req.ADR := 0.asUInt();
        req.WE := 0.asUInt();
        req.STB := 1.asUInt();
        req.CYC := 1.asUInt();
        req.SEL := 1.asUInt();
        return_s := sTx_ReadLSR_1
        state := sWaitRxAccDone
      } .otherwise {
        // Back around the loop
        state := sRx_ReadLSR_1
      }
    }
    
    is (sTx_ReadLSR_1) { // 9
      // Have the data. Setup a LSR read to check ability to Tx
      req.ADR := 5.asUInt()
      req.WE := 0.asUInt()
      req.STB := 1.asUInt()
      req.CYC := 1.asUInt()
      req.SEL := 2.asUInt()
      return_s := sTx_ReadLSR_2
      state := sWaitAccDone
    }
    
    is (sTx_ReadLSR_2) { // 10
      when (data((6+8)) === 1.asUInt()) {
        req.ADR := 0.asUInt()
        req.WE := 1.asUInt()
        req.STB := 1.asUInt()
        req.CYC := 1.asUInt()
        req.SEL := 1.asUInt()
        req.DAT_W := rxdata
        // Back to checking for an Rx
        return_s := sRx_ReadLSR_1
        state := sWaitAccDone
        count := count + 1.asUInt()
      } .otherwise {
        // Wait for Tx to become available
        state := sTx_ReadLSR_1
      }
    }
  }
}