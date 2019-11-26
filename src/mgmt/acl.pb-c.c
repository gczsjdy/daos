/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: acl.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "acl.pb-c.h"
void   mgmt__aclresp__init
                     (Mgmt__ACLResp         *message)
{
  static const Mgmt__ACLResp init_value = MGMT__ACLRESP__INIT;
  *message = init_value;
}
size_t mgmt__aclresp__get_packed_size
                     (const Mgmt__ACLResp *message)
{
  assert(message->base.descriptor == &mgmt__aclresp__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t mgmt__aclresp__pack
                     (const Mgmt__ACLResp *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &mgmt__aclresp__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t mgmt__aclresp__pack_to_buffer
                     (const Mgmt__ACLResp *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &mgmt__aclresp__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Mgmt__ACLResp *
       mgmt__aclresp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Mgmt__ACLResp *)
     protobuf_c_message_unpack (&mgmt__aclresp__descriptor,
                                allocator, len, data);
}
void   mgmt__aclresp__free_unpacked
                     (Mgmt__ACLResp *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &mgmt__aclresp__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   mgmt__get_aclreq__init
                     (Mgmt__GetACLReq         *message)
{
  static const Mgmt__GetACLReq init_value = MGMT__GET_ACLREQ__INIT;
  *message = init_value;
}
size_t mgmt__get_aclreq__get_packed_size
                     (const Mgmt__GetACLReq *message)
{
  assert(message->base.descriptor == &mgmt__get_aclreq__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t mgmt__get_aclreq__pack
                     (const Mgmt__GetACLReq *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &mgmt__get_aclreq__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t mgmt__get_aclreq__pack_to_buffer
                     (const Mgmt__GetACLReq *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &mgmt__get_aclreq__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Mgmt__GetACLReq *
       mgmt__get_aclreq__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Mgmt__GetACLReq *)
     protobuf_c_message_unpack (&mgmt__get_aclreq__descriptor,
                                allocator, len, data);
}
void   mgmt__get_aclreq__free_unpacked
                     (Mgmt__GetACLReq *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &mgmt__get_aclreq__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   mgmt__modify_aclreq__init
                     (Mgmt__ModifyACLReq         *message)
{
  static const Mgmt__ModifyACLReq init_value = MGMT__MODIFY_ACLREQ__INIT;
  *message = init_value;
}
size_t mgmt__modify_aclreq__get_packed_size
                     (const Mgmt__ModifyACLReq *message)
{
  assert(message->base.descriptor == &mgmt__modify_aclreq__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t mgmt__modify_aclreq__pack
                     (const Mgmt__ModifyACLReq *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &mgmt__modify_aclreq__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t mgmt__modify_aclreq__pack_to_buffer
                     (const Mgmt__ModifyACLReq *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &mgmt__modify_aclreq__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Mgmt__ModifyACLReq *
       mgmt__modify_aclreq__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Mgmt__ModifyACLReq *)
     protobuf_c_message_unpack (&mgmt__modify_aclreq__descriptor,
                                allocator, len, data);
}
void   mgmt__modify_aclreq__free_unpacked
                     (Mgmt__ModifyACLReq *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &mgmt__modify_aclreq__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor mgmt__aclresp__field_descriptors[2] =
{
  {
    "status",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(Mgmt__ACLResp, status),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "ACL",
    2,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_STRING,
    offsetof(Mgmt__ACLResp, n_acl),
    offsetof(Mgmt__ACLResp, acl),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned mgmt__aclresp__field_indices_by_name[] = {
  1,   /* field[1] = ACL */
  0,   /* field[0] = status */
};
static const ProtobufCIntRange mgmt__aclresp__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor mgmt__aclresp__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "mgmt.ACLResp",
  "ACLResp",
  "Mgmt__ACLResp",
  "mgmt",
  sizeof(Mgmt__ACLResp),
  2,
  mgmt__aclresp__field_descriptors,
  mgmt__aclresp__field_indices_by_name,
  1,  mgmt__aclresp__number_ranges,
  (ProtobufCMessageInit) mgmt__aclresp__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor mgmt__get_aclreq__field_descriptors[1] =
{
  {
    "uuid",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Mgmt__GetACLReq, uuid),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned mgmt__get_aclreq__field_indices_by_name[] = {
  0,   /* field[0] = uuid */
};
static const ProtobufCIntRange mgmt__get_aclreq__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor mgmt__get_aclreq__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "mgmt.GetACLReq",
  "GetACLReq",
  "Mgmt__GetACLReq",
  "mgmt",
  sizeof(Mgmt__GetACLReq),
  1,
  mgmt__get_aclreq__field_descriptors,
  mgmt__get_aclreq__field_indices_by_name,
  1,  mgmt__get_aclreq__number_ranges,
  (ProtobufCMessageInit) mgmt__get_aclreq__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor mgmt__modify_aclreq__field_descriptors[2] =
{
  {
    "uuid",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Mgmt__ModifyACLReq, uuid),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "ACL",
    2,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_STRING,
    offsetof(Mgmt__ModifyACLReq, n_acl),
    offsetof(Mgmt__ModifyACLReq, acl),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned mgmt__modify_aclreq__field_indices_by_name[] = {
  1,   /* field[1] = ACL */
  0,   /* field[0] = uuid */
};
static const ProtobufCIntRange mgmt__modify_aclreq__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor mgmt__modify_aclreq__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "mgmt.ModifyACLReq",
  "ModifyACLReq",
  "Mgmt__ModifyACLReq",
  "mgmt",
  sizeof(Mgmt__ModifyACLReq),
  2,
  mgmt__modify_aclreq__field_descriptors,
  mgmt__modify_aclreq__field_indices_by_name,
  1,  mgmt__modify_aclreq__number_ranges,
  (ProtobufCMessageInit) mgmt__modify_aclreq__init,
  NULL,NULL,NULL    /* reserved[123] */
};
