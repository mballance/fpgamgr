package uart_responder

import chisel3._
import oc_wb_ip.wb_uart.uart_top
import oc_wb_ip.wb_uart.WishboneUART
import std_protocol_if.UartIf

class UartResponder extends Module {
  
  val io = IO(new Bundle {
    val u = new UartIf()
    val count = Output(UInt(4.W))
  })
  
  val uart = Module(new WishboneUART());
  io.u <> uart.io.s
  
  val master = Module(new UartResponderMaster());
  uart.io.t <> master.io.m
  io.count := master.io.count
 
}