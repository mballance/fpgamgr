package uart_responder

import chisel3._
import chisel3.util._
import sv_bfms.uart.UartSerialBFM

class UartResponderTB extends Module {
  val io = IO(new Bundle { 
    val count = Output(UInt(4.W))
  })
  

  val uart_responder = Module(new UartResponder())
  
  val uart_bfm = Module(new UartSerialBFM());
  
  uart_responder.io.u <> uart_bfm.io.s
  io.count := uart_responder.io.count
  
}
