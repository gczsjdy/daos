/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: srv.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "srv.pb-c.h"
void   mgmt__daos_resp__init
                     (Mgmt__DaosResp         *message)
{
  static const Mgmt__DaosResp init_value = MGMT__DAOS_RESP__INIT;
  *message = init_value;
}
size_t mgmt__daos_resp__get_packed_size
                     (const Mgmt__DaosResp *message)
{
  assert(message->base.descriptor == &mgmt__daos_resp__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t mgmt__daos_resp__pack
                     (const Mgmt__DaosResp *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &mgmt__daos_resp__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t mgmt__daos_resp__pack_to_buffer
                     (const Mgmt__DaosResp *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &mgmt__daos_resp__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Mgmt__DaosResp *
       mgmt__daos_resp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Mgmt__DaosResp *)
     protobuf_c_message_unpack (&mgmt__daos_resp__descriptor,
                                allocator, len, data);
}
void   mgmt__daos_resp__free_unpacked
                     (Mgmt__DaosResp *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &mgmt__daos_resp__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   mgmt__join_req__init
                     (Mgmt__JoinReq         *message)
{
  static const Mgmt__JoinReq init_value = MGMT__JOIN_REQ__INIT;
  *message = init_value;
}
size_t mgmt__join_req__get_packed_size
                     (const Mgmt__JoinReq *message)
{
  assert(message->base.descriptor == &mgmt__join_req__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t mgmt__join_req__pack
                     (const Mgmt__JoinReq *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &mgmt__join_req__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t mgmt__join_req__pack_to_buffer
                     (const Mgmt__JoinReq *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &mgmt__join_req__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Mgmt__JoinReq *
       mgmt__join_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Mgmt__JoinReq *)
     protobuf_c_message_unpack (&mgmt__join_req__descriptor,
                                allocator, len, data);
}
void   mgmt__join_req__free_unpacked
                     (Mgmt__JoinReq *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &mgmt__join_req__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   mgmt__join_resp__init
                     (Mgmt__JoinResp         *message)
{
  static const Mgmt__JoinResp init_value = MGMT__JOIN_RESP__INIT;
  *message = init_value;
}
size_t mgmt__join_resp__get_packed_size
                     (const Mgmt__JoinResp *message)
{
  assert(message->base.descriptor == &mgmt__join_resp__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t mgmt__join_resp__pack
                     (const Mgmt__JoinResp *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &mgmt__join_resp__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t mgmt__join_resp__pack_to_buffer
                     (const Mgmt__JoinResp *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &mgmt__join_resp__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Mgmt__JoinResp *
       mgmt__join_resp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Mgmt__JoinResp *)
     protobuf_c_message_unpack (&mgmt__join_resp__descriptor,
                                allocator, len, data);
}
void   mgmt__join_resp__free_unpacked
                     (Mgmt__JoinResp *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &mgmt__join_resp__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   mgmt__get_attach_info_req__init
                     (Mgmt__GetAttachInfoReq         *message)
{
  static const Mgmt__GetAttachInfoReq init_value = MGMT__GET_ATTACH_INFO_REQ__INIT;
  *message = init_value;
}
size_t mgmt__get_attach_info_req__get_packed_size
                     (const Mgmt__GetAttachInfoReq *message)
{
  assert(message->base.descriptor == &mgmt__get_attach_info_req__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t mgmt__get_attach_info_req__pack
                     (const Mgmt__GetAttachInfoReq *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &mgmt__get_attach_info_req__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t mgmt__get_attach_info_req__pack_to_buffer
                     (const Mgmt__GetAttachInfoReq *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &mgmt__get_attach_info_req__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Mgmt__GetAttachInfoReq *
       mgmt__get_attach_info_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Mgmt__GetAttachInfoReq *)
     protobuf_c_message_unpack (&mgmt__get_attach_info_req__descriptor,
                                allocator, len, data);
}
void   mgmt__get_attach_info_req__free_unpacked
                     (Mgmt__GetAttachInfoReq *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &mgmt__get_attach_info_req__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   mgmt__get_attach_info_resp__psr__init
                     (Mgmt__GetAttachInfoResp__Psr         *message)
{
  static const Mgmt__GetAttachInfoResp__Psr init_value = MGMT__GET_ATTACH_INFO_RESP__PSR__INIT;
  *message = init_value;
}
void   mgmt__get_attach_info_resp__init
                     (Mgmt__GetAttachInfoResp         *message)
{
  static const Mgmt__GetAttachInfoResp init_value = MGMT__GET_ATTACH_INFO_RESP__INIT;
  *message = init_value;
}
size_t mgmt__get_attach_info_resp__get_packed_size
                     (const Mgmt__GetAttachInfoResp *message)
{
  assert(message->base.descriptor == &mgmt__get_attach_info_resp__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t mgmt__get_attach_info_resp__pack
                     (const Mgmt__GetAttachInfoResp *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &mgmt__get_attach_info_resp__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t mgmt__get_attach_info_resp__pack_to_buffer
                     (const Mgmt__GetAttachInfoResp *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &mgmt__get_attach_info_resp__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Mgmt__GetAttachInfoResp *
       mgmt__get_attach_info_resp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Mgmt__GetAttachInfoResp *)
     protobuf_c_message_unpack (&mgmt__get_attach_info_resp__descriptor,
                                allocator, len, data);
}
void   mgmt__get_attach_info_resp__free_unpacked
                     (Mgmt__GetAttachInfoResp *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &mgmt__get_attach_info_resp__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   mgmt__kill_rank_req__init
                     (Mgmt__KillRankReq         *message)
{
  static const Mgmt__KillRankReq init_value = MGMT__KILL_RANK_REQ__INIT;
  *message = init_value;
}
size_t mgmt__kill_rank_req__get_packed_size
                     (const Mgmt__KillRankReq *message)
{
  assert(message->base.descriptor == &mgmt__kill_rank_req__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t mgmt__kill_rank_req__pack
                     (const Mgmt__KillRankReq *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &mgmt__kill_rank_req__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t mgmt__kill_rank_req__pack_to_buffer
                     (const Mgmt__KillRankReq *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &mgmt__kill_rank_req__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Mgmt__KillRankReq *
       mgmt__kill_rank_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Mgmt__KillRankReq *)
     protobuf_c_message_unpack (&mgmt__kill_rank_req__descriptor,
                                allocator, len, data);
}
void   mgmt__kill_rank_req__free_unpacked
                     (Mgmt__KillRankReq *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &mgmt__kill_rank_req__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   mgmt__set_rank_req__init
                     (Mgmt__SetRankReq         *message)
{
  static const Mgmt__SetRankReq init_value = MGMT__SET_RANK_REQ__INIT;
  *message = init_value;
}
size_t mgmt__set_rank_req__get_packed_size
                     (const Mgmt__SetRankReq *message)
{
  assert(message->base.descriptor == &mgmt__set_rank_req__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t mgmt__set_rank_req__pack
                     (const Mgmt__SetRankReq *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &mgmt__set_rank_req__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t mgmt__set_rank_req__pack_to_buffer
                     (const Mgmt__SetRankReq *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &mgmt__set_rank_req__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Mgmt__SetRankReq *
       mgmt__set_rank_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Mgmt__SetRankReq *)
     protobuf_c_message_unpack (&mgmt__set_rank_req__descriptor,
                                allocator, len, data);
}
void   mgmt__set_rank_req__free_unpacked
                     (Mgmt__SetRankReq *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &mgmt__set_rank_req__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   mgmt__create_ms_req__init
                     (Mgmt__CreateMsReq         *message)
{
  static const Mgmt__CreateMsReq init_value = MGMT__CREATE_MS_REQ__INIT;
  *message = init_value;
}
size_t mgmt__create_ms_req__get_packed_size
                     (const Mgmt__CreateMsReq *message)
{
  assert(message->base.descriptor == &mgmt__create_ms_req__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t mgmt__create_ms_req__pack
                     (const Mgmt__CreateMsReq *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &mgmt__create_ms_req__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t mgmt__create_ms_req__pack_to_buffer
                     (const Mgmt__CreateMsReq *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &mgmt__create_ms_req__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Mgmt__CreateMsReq *
       mgmt__create_ms_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Mgmt__CreateMsReq *)
     protobuf_c_message_unpack (&mgmt__create_ms_req__descriptor,
                                allocator, len, data);
}
void   mgmt__create_ms_req__free_unpacked
                     (Mgmt__CreateMsReq *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &mgmt__create_ms_req__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor mgmt__daos_resp__field_descriptors[1] =
{
  {
    "status",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(Mgmt__DaosResp, status),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned mgmt__daos_resp__field_indices_by_name[] = {
  0,   /* field[0] = status */
};
static const ProtobufCIntRange mgmt__daos_resp__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor mgmt__daos_resp__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "mgmt.DaosResp",
  "DaosResp",
  "Mgmt__DaosResp",
  "mgmt",
  sizeof(Mgmt__DaosResp),
  1,
  mgmt__daos_resp__field_descriptors,
  mgmt__daos_resp__field_indices_by_name,
  1,  mgmt__daos_resp__number_ranges,
  (ProtobufCMessageInit) mgmt__daos_resp__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor mgmt__join_req__field_descriptors[5] =
{
  {
    "uuid",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Mgmt__JoinReq, uuid),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rank",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Mgmt__JoinReq, rank),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "uri",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Mgmt__JoinReq, uri),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "nctxs",
    4,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Mgmt__JoinReq, nctxs),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "addr",
    5,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Mgmt__JoinReq, addr),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned mgmt__join_req__field_indices_by_name[] = {
  4,   /* field[4] = addr */
  3,   /* field[3] = nctxs */
  1,   /* field[1] = rank */
  2,   /* field[2] = uri */
  0,   /* field[0] = uuid */
};
static const ProtobufCIntRange mgmt__join_req__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 5 }
};
const ProtobufCMessageDescriptor mgmt__join_req__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "mgmt.JoinReq",
  "JoinReq",
  "Mgmt__JoinReq",
  "mgmt",
  sizeof(Mgmt__JoinReq),
  5,
  mgmt__join_req__field_descriptors,
  mgmt__join_req__field_indices_by_name,
  1,  mgmt__join_req__number_ranges,
  (ProtobufCMessageInit) mgmt__join_req__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCEnumValue mgmt__join_resp__state__enum_values_by_number[2] =
{
  { "IN", "MGMT__JOIN_RESP__STATE__IN", 0 },
  { "OUT", "MGMT__JOIN_RESP__STATE__OUT", 1 },
};
static const ProtobufCIntRange mgmt__join_resp__state__value_ranges[] = {
{0, 0},{0, 2}
};
static const ProtobufCEnumValueIndex mgmt__join_resp__state__enum_values_by_name[2] =
{
  { "IN", 0 },
  { "OUT", 1 },
};
const ProtobufCEnumDescriptor mgmt__join_resp__state__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "mgmt.JoinResp.State",
  "State",
  "Mgmt__JoinResp__State",
  "mgmt",
  2,
  mgmt__join_resp__state__enum_values_by_number,
  2,
  mgmt__join_resp__state__enum_values_by_name,
  1,
  mgmt__join_resp__state__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCFieldDescriptor mgmt__join_resp__field_descriptors[3] =
{
  {
    "status",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(Mgmt__JoinResp, status),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rank",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Mgmt__JoinResp, rank),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "state",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    offsetof(Mgmt__JoinResp, state),
    &mgmt__join_resp__state__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned mgmt__join_resp__field_indices_by_name[] = {
  1,   /* field[1] = rank */
  2,   /* field[2] = state */
  0,   /* field[0] = status */
};
static const ProtobufCIntRange mgmt__join_resp__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor mgmt__join_resp__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "mgmt.JoinResp",
  "JoinResp",
  "Mgmt__JoinResp",
  "mgmt",
  sizeof(Mgmt__JoinResp),
  3,
  mgmt__join_resp__field_descriptors,
  mgmt__join_resp__field_indices_by_name,
  1,  mgmt__join_resp__number_ranges,
  (ProtobufCMessageInit) mgmt__join_resp__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor mgmt__get_attach_info_req__field_descriptors[1] =
{
  {
    "sys",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Mgmt__GetAttachInfoReq, sys),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned mgmt__get_attach_info_req__field_indices_by_name[] = {
  0,   /* field[0] = sys */
};
static const ProtobufCIntRange mgmt__get_attach_info_req__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor mgmt__get_attach_info_req__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "mgmt.GetAttachInfoReq",
  "GetAttachInfoReq",
  "Mgmt__GetAttachInfoReq",
  "mgmt",
  sizeof(Mgmt__GetAttachInfoReq),
  1,
  mgmt__get_attach_info_req__field_descriptors,
  mgmt__get_attach_info_req__field_indices_by_name,
  1,  mgmt__get_attach_info_req__number_ranges,
  (ProtobufCMessageInit) mgmt__get_attach_info_req__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor mgmt__get_attach_info_resp__psr__field_descriptors[2] =
{
  {
    "rank",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Mgmt__GetAttachInfoResp__Psr, rank),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "uri",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Mgmt__GetAttachInfoResp__Psr, uri),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned mgmt__get_attach_info_resp__psr__field_indices_by_name[] = {
  0,   /* field[0] = rank */
  1,   /* field[1] = uri */
};
static const ProtobufCIntRange mgmt__get_attach_info_resp__psr__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor mgmt__get_attach_info_resp__psr__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "mgmt.GetAttachInfoResp.Psr",
  "Psr",
  "Mgmt__GetAttachInfoResp__Psr",
  "mgmt",
  sizeof(Mgmt__GetAttachInfoResp__Psr),
  2,
  mgmt__get_attach_info_resp__psr__field_descriptors,
  mgmt__get_attach_info_resp__psr__field_indices_by_name,
  1,  mgmt__get_attach_info_resp__psr__number_ranges,
  (ProtobufCMessageInit) mgmt__get_attach_info_resp__psr__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor mgmt__get_attach_info_resp__field_descriptors[2] =
{
  {
    "status",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(Mgmt__GetAttachInfoResp, status),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "psrs",
    2,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Mgmt__GetAttachInfoResp, n_psrs),
    offsetof(Mgmt__GetAttachInfoResp, psrs),
    &mgmt__get_attach_info_resp__psr__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned mgmt__get_attach_info_resp__field_indices_by_name[] = {
  1,   /* field[1] = psrs */
  0,   /* field[0] = status */
};
static const ProtobufCIntRange mgmt__get_attach_info_resp__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor mgmt__get_attach_info_resp__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "mgmt.GetAttachInfoResp",
  "GetAttachInfoResp",
  "Mgmt__GetAttachInfoResp",
  "mgmt",
  sizeof(Mgmt__GetAttachInfoResp),
  2,
  mgmt__get_attach_info_resp__field_descriptors,
  mgmt__get_attach_info_resp__field_indices_by_name,
  1,  mgmt__get_attach_info_resp__number_ranges,
  (ProtobufCMessageInit) mgmt__get_attach_info_resp__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor mgmt__kill_rank_req__field_descriptors[2] =
{
  {
    "force",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_BOOL,
    0,   /* quantifier_offset */
    offsetof(Mgmt__KillRankReq, force),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rank",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Mgmt__KillRankReq, rank),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned mgmt__kill_rank_req__field_indices_by_name[] = {
  0,   /* field[0] = force */
  1,   /* field[1] = rank */
};
static const ProtobufCIntRange mgmt__kill_rank_req__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor mgmt__kill_rank_req__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "mgmt.KillRankReq",
  "KillRankReq",
  "Mgmt__KillRankReq",
  "mgmt",
  sizeof(Mgmt__KillRankReq),
  2,
  mgmt__kill_rank_req__field_descriptors,
  mgmt__kill_rank_req__field_indices_by_name,
  1,  mgmt__kill_rank_req__number_ranges,
  (ProtobufCMessageInit) mgmt__kill_rank_req__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor mgmt__set_rank_req__field_descriptors[1] =
{
  {
    "rank",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Mgmt__SetRankReq, rank),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned mgmt__set_rank_req__field_indices_by_name[] = {
  0,   /* field[0] = rank */
};
static const ProtobufCIntRange mgmt__set_rank_req__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor mgmt__set_rank_req__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "mgmt.SetRankReq",
  "SetRankReq",
  "Mgmt__SetRankReq",
  "mgmt",
  sizeof(Mgmt__SetRankReq),
  1,
  mgmt__set_rank_req__field_descriptors,
  mgmt__set_rank_req__field_indices_by_name,
  1,  mgmt__set_rank_req__number_ranges,
  (ProtobufCMessageInit) mgmt__set_rank_req__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor mgmt__create_ms_req__field_descriptors[3] =
{
  {
    "bootstrap",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_BOOL,
    0,   /* quantifier_offset */
    offsetof(Mgmt__CreateMsReq, bootstrap),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "uuid",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Mgmt__CreateMsReq, uuid),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "addr",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Mgmt__CreateMsReq, addr),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned mgmt__create_ms_req__field_indices_by_name[] = {
  2,   /* field[2] = addr */
  0,   /* field[0] = bootstrap */
  1,   /* field[1] = uuid */
};
static const ProtobufCIntRange mgmt__create_ms_req__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor mgmt__create_ms_req__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "mgmt.CreateMsReq",
  "CreateMsReq",
  "Mgmt__CreateMsReq",
  "mgmt",
  sizeof(Mgmt__CreateMsReq),
  3,
  mgmt__create_ms_req__field_descriptors,
  mgmt__create_ms_req__field_indices_by_name,
  1,  mgmt__create_ms_req__number_ranges,
  (ProtobufCMessageInit) mgmt__create_ms_req__init,
  NULL,NULL,NULL    /* reserved[123] */
};
