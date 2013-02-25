#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <darc/component.hpp>
#include <darc/periodic_timer.hpp>
#include <darc/publisher.h>
#include <iostream>

using namespace boost::python;

struct null_deleter
{
  void operator()(void * p) {}
};

struct pyobject_deleter
{
  void operator()(object * p)
  {
    boost::python::decref(p->ptr());
  }
};

namespace darc_py
{

class component : public darc::component {};
class periodic_timer : public darc::periodic_timer
{
protected:
  object callback_;

  void trigger_callback()
  {
    callback_();
  }

public:
  periodic_timer(darc_py::component& owner,
                 object callable,
                 boost::posix_time::time_duration& period) :
    darc::periodic_timer(&owner,
                         boost::bind(&periodic_timer::trigger_callback, this),
                         period, 1),
    callback_(callable)
  {
    boost::python::incref(callback_.ptr());
  }

  ~periodic_timer()
  {
    boost::python::decref(callback_.ptr());
  }

};

class publisher : public darc::publisher<object>
{
public:
  publisher(darc_py::component& owner,
            const std::string& topic) :
    darc::publisher<object>(&owner, topic)
  {
  }

  void publish(object& o)
  {
    boost::python::incref(o.ptr());
    boost::shared_ptr<object> msg(&o, pyobject_deleter());
    std::cout << "PUB" << std::endl;
    darc::publisher<object>::publish(msg);
  }

};

}

namespace ros
{
namespace serialization
{

template<>
struct Serializer<object>
{
  static void read(Stream& stream, object& t)
  {
    std::cout << "READ" << std::endl;
  }

  static void write(Stream& stream, const object& t)
  {
    std::cout << "WRITE" << std::endl;
  }
};

}
}

void do_attach(darc_py::component& c, const std::string& name, darc::component_manager* mngr)
{
  // HACKISH, creating fake shared_ptr since component_manager wants one
  darc::component_ptr c_ptr(&c, null_deleter());
  c.set_name(name);
  mngr->attach(c_ptr);
}

BOOST_PYTHON_MODULE(darc_py)
{
  // component
  class_<darc_py::component, boost::noncopyable>("component")
    .def("attach", &do_attach)
    .def("run", &darc::component::run)
    .def("stop", &darc::component::stop)
    .def("pause", &darc::component::pause)
    .def("unpause", &darc::component::unpause)
    .def("name", &darc::component::get_name)
    .def("id", &darc::component::get_id, return_value_policy<copy_const_reference>());

  // primitives
  class_<darc_py::periodic_timer, boost::noncopyable>("periodic_timer",
                                                      init<darc_py::component&,
                                                      object, // callback
                                                      boost::posix_time::time_duration&>());

  class_<darc_py::publisher>("publisher",
                             init<darc_py::component&,
                             const std::string&>())
    .def("publish", &darc_py::publisher::publish);

  // time
  class_<boost::posix_time::time_duration>("time_duration");
  class_<boost::posix_time::seconds, bases<boost::posix_time::time_duration> >("seconds",
                                     init<int>());


}
