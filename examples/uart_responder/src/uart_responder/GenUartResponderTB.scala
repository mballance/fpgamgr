package uart_responder

import chisel3._
object GenUartResponderTB extends App {
  
  chisel3.Driver.execute(args, () => new UartResponderTB)
  
}