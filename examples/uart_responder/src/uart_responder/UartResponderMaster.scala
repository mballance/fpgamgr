package uart_responder



import chisel3._
import chisel3.util._
import std_protocol_if.Wishbone

class UartResponderMaster(p : Wishbone.Parameters = new Wishbone.Parameters()) extends Module {
  
  val io = IO(new Bundle {
    val m = new Wishbone(p)
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
  val sInitUart_LCR_1 :: sInitUart_DL1 :: sInitUart_DL0 :: sInitUart_LCR_2 :: sWaitAccDone :: sRx_ReadLSR_1 :: sRx_ReadLSR_2 :: sTx_ReadLSR_1 :: sTx_ReadLSR_2 :: Nil = Enum(9)

  val state = Reg(init=sInitUart_LCR_1)
  val return_s = Reg(init=sInitUart_LCR_1)
  val data = Reg(UInt(p.DATA_WIDTH.W), init=0.asUInt())
  
  switch (state) {
    is (sInitUart_LCR_1) {
      io.m.req.ADR := 3.asUInt()
      io.m.req.DAT_W := 0x83838383L.asUInt() // N81+DLAB
      io.m.req.WE := 1.asUInt()
      io.m.req.SEL := 1.asUInt()
      io.m.req.CYC := 1.asUInt()
      io.m.req.STB := 0x8.asUInt()
      state := sWaitAccDone
      return_s := sInitUart_DL1
    }
    
    is (sInitUart_DL1) {
      io.m.req.ADR := 1.asUInt()
      io.m.req.DAT_W := 0x00000000.asUInt() // 
      io.m.req.WE := 1.asUInt()
      io.m.req.SEL := 1.asUInt()
      io.m.req.CYC := 1.asUInt()
      io.m.req.STB := 0x2.asUInt()
      state := sWaitAccDone
      return_s := sInitUart_DL0
    }

    is (sInitUart_DL0) {
      io.m.req.ADR := 0.asUInt()
      io.m.req.DAT_W := 0x14141414.asUInt() // 
      io.m.req.WE := 0.asUInt()
      io.m.req.SEL := 1.asUInt()
      io.m.req.CYC := 1.asUInt()
      io.m.req.STB := 0x1.asUInt()
      state := sWaitAccDone
      return_s := sInitUart_LCR_2
    }
    
    is (sInitUart_LCR_2) {
      io.m.req.ADR := 3.asUInt()
      io.m.req.DAT_W := 0x03030303L.asUInt() // N81
      io.m.req.WE := 1.asUInt()
      io.m.req.SEL := 1.asUInt()
      io.m.req.CYC := 1.asUInt()
      io.m.req.STB := 0x8.asUInt()
      state := sWaitAccDone
      return_s := sRx_ReadLSR_1
    }
    
    is (sWaitAccDone) {
      when (io.m.rsp.ACK) {
        // Go to the actual next state
        io.m.req.ADR := 0.asUInt()
        io.m.req.DAT_W := 0.asUInt()
        data := io.m.rsp.DAT_R
        io.m.req.WE := 0.asUInt()
        io.m.req.SEL := 0.asUInt()
        io.m.req.CYC := 0.asUInt()
        io.m.req.STB := 0.asUInt()
        state := return_s
      }
    }
   
    // Read the LSR waiting for a character to receive
    is (sRx_ReadLSR_1) {
        io.m.req.ADR := 5.asUInt()
        io.m.req.DAT_W := 0.asUInt()
        io.m.req.WE := 0.asUInt()
        io.m.req.SEL := 1.asUInt()
        io.m.req.CYC := 1.asUInt()
        io.m.req.STB := 2.asUInt()
        return_s := sRx_ReadLSR_2
        state := sWaitAccDone
    }
    
    is (sRx_ReadLSR_2) {
      when (data(0) === 0.asUInt()) { // byte available
        io.m.req.ADR := 0.asUInt();
        io.m.req.WE := 0.asUInt();
        io.m.req.SEL := 1.asUInt();
        io.m.req.CYC := 1.asUInt();
        io.m.req.STB := 1.asUInt();
        return_s := sTx_ReadLSR_1
        state := sWaitAccDone
      } .otherwise {
        // Back around the loop
        state := sRx_ReadLSR_1
      }
    }
    
    is (sTx_ReadLSR_1) {
      // Have the data. Setup a LSR read to check ability to Tx
      io.m.req.ADR := 5.asUInt()
      io.m.req.WE := 0.asUInt()
      io.m.req.SEL := 1.asUInt()
      io.m.req.CYC := 1.asUInt()
      io.m.req.STB := 2.asUInt()
      return_s := sTx_ReadLSR_2
      state := sWaitAccDone
    }
    
    is (sTx_ReadLSR_2) {
      when (data(6) === 0.asUInt()) {
        io.m.req.ADR := 0.asUInt()
        io.m.req.WE := 1.asUInt()
        io.m.req.SEL := 1.asUInt()
        io.m.req.CYC := 1.asUInt()
        io.m.req.STB := 1.asUInt()
        io.m.req.DAT_W := data
        // Back to checking for an Rx
        return_s := sRx_ReadLSR_1
        state := sWaitAccDone
      } .otherwise {
        // Back around the loop
        state := sTx_ReadLSR_1
      }
    }
  }
}