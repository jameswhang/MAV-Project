// Generated by gencpp from file px_comm/Mavlink.msg
// DO NOT EDIT!


#ifndef PX_COMM_MESSAGE_MAVLINK_H
#define PX_COMM_MESSAGE_MAVLINK_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>

namespace px_comm
{
template <class ContainerAllocator>
struct Mavlink_
{
  typedef Mavlink_<ContainerAllocator> Type;

  Mavlink_()
    : header()
    , len(0)
    , seq(0)
    , sysid(0)
    , compid(0)
    , msgid(0)
    , fromlcm(false)
    , payload64()  {
    }
  Mavlink_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , len(0)
    , seq(0)
    , sysid(0)
    , compid(0)
    , msgid(0)
    , fromlcm(false)
    , payload64(_alloc)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef uint8_t _len_type;
  _len_type len;

   typedef uint8_t _seq_type;
  _seq_type seq;

   typedef uint8_t _sysid_type;
  _sysid_type sysid;

   typedef uint8_t _compid_type;
  _compid_type compid;

   typedef uint8_t _msgid_type;
  _msgid_type msgid;

   typedef uint8_t _fromlcm_type;
  _fromlcm_type fromlcm;

   typedef std::vector<uint64_t, typename ContainerAllocator::template rebind<uint64_t>::other >  _payload64_type;
  _payload64_type payload64;




  typedef boost::shared_ptr< ::px_comm::Mavlink_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::px_comm::Mavlink_<ContainerAllocator> const> ConstPtr;

}; // struct Mavlink_

typedef ::px_comm::Mavlink_<std::allocator<void> > Mavlink;

typedef boost::shared_ptr< ::px_comm::Mavlink > MavlinkPtr;
typedef boost::shared_ptr< ::px_comm::Mavlink const> MavlinkConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::px_comm::Mavlink_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::px_comm::Mavlink_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace px_comm

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg'], 'geometry_msgs': ['/opt/ros/indigo/share/geometry_msgs/cmake/../msg'], 'px_comm': ['/home/tuofeichen/SLAM/MAV-Project/catkin_ws/src/px-ros-pkg/px_comm/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::px_comm::Mavlink_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::px_comm::Mavlink_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::px_comm::Mavlink_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::px_comm::Mavlink_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::px_comm::Mavlink_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::px_comm::Mavlink_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::px_comm::Mavlink_<ContainerAllocator> >
{
  static const char* value()
  {
    return "3513d96e87d97146fed4643086047ddd";
  }

  static const char* value(const ::px_comm::Mavlink_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x3513d96e87d97146ULL;
  static const uint64_t static_value2 = 0xfed4643086047dddULL;
};

template<class ContainerAllocator>
struct DataType< ::px_comm::Mavlink_<ContainerAllocator> >
{
  static const char* value()
  {
    return "px_comm/Mavlink";
  }

  static const char* value(const ::px_comm::Mavlink_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::px_comm::Mavlink_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header header\n\
uint8 len\n\
uint8 seq\n\
uint8 sysid\n\
uint8 compid\n\
uint8 msgid\n\
bool fromlcm\n\
uint64[] payload64\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n\
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
";
  }

  static const char* value(const ::px_comm::Mavlink_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::px_comm::Mavlink_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.len);
      stream.next(m.seq);
      stream.next(m.sysid);
      stream.next(m.compid);
      stream.next(m.msgid);
      stream.next(m.fromlcm);
      stream.next(m.payload64);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct Mavlink_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::px_comm::Mavlink_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::px_comm::Mavlink_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "len: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.len);
    s << indent << "seq: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.seq);
    s << indent << "sysid: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.sysid);
    s << indent << "compid: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.compid);
    s << indent << "msgid: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.msgid);
    s << indent << "fromlcm: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.fromlcm);
    s << indent << "payload64[]" << std::endl;
    for (size_t i = 0; i < v.payload64.size(); ++i)
    {
      s << indent << "  payload64[" << i << "]: ";
      Printer<uint64_t>::stream(s, indent + "  ", v.payload64[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // PX_COMM_MESSAGE_MAVLINK_H
