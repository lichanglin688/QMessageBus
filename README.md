# QMessageBus

利用qt事件循环，针对单进程架构的qt项目的异步消息系统，可轻松构建请求应答的消息模式，也可异步请求无应答。

使用c++20的协程库[qcoro](https://github.com/qcoro/qcoro)，可更方便的使用协程来处理异步消息。
