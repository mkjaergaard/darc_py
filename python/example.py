import darc
import darc_py
import time

mngr = darc.component_manager()
mngr.connect("zmq+tcp://127.0.0.1:5000")
mngr.connect("zmq+tcp://127.0.0.1:5001")
mngr.connect("zmq+tcp://127.0.0.1:5002")
mngr.connect("zmq+tcp://127.0.0.1:5003")
mngr.connect("zmq+tcp://127.0.0.1:5004")
mngr.connect("zmq+tcp://127.0.0.1:5005")
mngr.connect("zmq+tcp://127.0.0.1:5006")
mngr.connect("zmq+tcp://127.0.0.1:5007")
mngr.connect("zmq+tcp://127.0.0.1:5008")
mngr.accept("zmq+tcp://127.0.0.1:5009")

class my_component(darc_py.component):
    def callback(self):
        print("TIMER")
        msg = 5
        self.pub.publish(msg)

    def __init__(self):
        darc_py.component.__init__(self)
        self.timer = darc_py.periodic_timer(self, self.callback, darc_py.seconds(1))
        self.pub = darc_py.publisher(self, "my_topic")

c = my_component()
c.attach("MyPyComponent", mngr)

c.run()
mngr.run_current_thread()
