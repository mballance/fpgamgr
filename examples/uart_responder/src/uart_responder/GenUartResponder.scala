package uart_responder

import chisel3._
object GenUartResponder extends App {
  
  chisel3.Driver.execute(args, () => new UartResponder)
  
}