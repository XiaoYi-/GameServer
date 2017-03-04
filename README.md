与其说这是个服务端框架，不如说这是个服务框架的一个节点。这个节点是一个reator模型。这个模型一般情况下有3个线程。
1、listen 监听端口线程。这个线程用于本节点的端口，注册了一个connection_notify的事件处理接口。在有链接事件触发时，
回调这个接口把链接客户端的fd,port,ip等封装成链接消息丢给event线程处理。
2、event IO线程。这个线程主要功能是完成与其他节点的通信。接收到listen监听线程发过来的链接消息，每个消息创建一个buffevent，用于对fd进行读写。
读到完整的字节流后，创建一个packet对象，把packet对象push到work线程的任务池中。同时也会接收work线程需要发送的packet，发送到对应的客户端。
3、work线程。work线程处理任务池中的packet消息。根据消息的类型，调用不用的python脚本去处理。处理的结果可以打包成packet对象。push到event线程。
由event线程发给对应的客户端。

正如上面所说，这只是一个服务框架的一个节点。要实现一个完整的服务框架，需要用户自己去灵活组合节点。每个节点负责不同的服务，链接不同的节点，通过pakcet消息
包在不同节点间的传递，去驱动业务逻辑。灵活的组合节点，构建一个完整的服务框架。
