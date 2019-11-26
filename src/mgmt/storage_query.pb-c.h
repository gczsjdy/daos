/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: storage_query.proto */

#ifndef PROTOBUF_C_storage_5fquery_2eproto__INCLUDED
#define PROTOBUF_C_storage_5fquery_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003002 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Mgmt__BioHealthReq Mgmt__BioHealthReq;
typedef struct _Mgmt__BioHealthResp Mgmt__BioHealthResp;
typedef struct _Mgmt__SmdDevReq Mgmt__SmdDevReq;
typedef struct _Mgmt__SmdDevResp Mgmt__SmdDevResp;
typedef struct _Mgmt__SmdDevResp__Device Mgmt__SmdDevResp__Device;
typedef struct _Mgmt__SmdPoolReq Mgmt__SmdPoolReq;
typedef struct _Mgmt__SmdPoolResp Mgmt__SmdPoolResp;
typedef struct _Mgmt__SmdPoolResp__Pool Mgmt__SmdPoolResp__Pool;


/* --- enums --- */


/* --- messages --- */

struct  _Mgmt__BioHealthReq
{
  ProtobufCMessage base;
  char *dev_uuid;
  char *tgt_id;
};
#define MGMT__BIO_HEALTH_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__bio_health_req__descriptor) \
    , (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string }


struct  _Mgmt__BioHealthResp
{
  ProtobufCMessage base;
  /*
   * DAOS error code
   */
  int32_t status;
  /*
   * UUID of blobstore
   */
  char *dev_uuid;
  uint64_t error_count;
  uint32_t temperature;
  uint64_t media_errors;
  uint32_t read_errs;
  uint32_t write_errs;
  uint32_t unmap_errs;
  uint32_t checksum_errs;
  protobuf_c_boolean temp;
  protobuf_c_boolean spare;
  protobuf_c_boolean readonly;
  protobuf_c_boolean device_reliability;
  protobuf_c_boolean volatile_memory;
};
#define MGMT__BIO_HEALTH_RESP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__bio_health_resp__descriptor) \
    , 0, (char *)protobuf_c_empty_string, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }


struct  _Mgmt__SmdDevReq
{
  ProtobufCMessage base;
};
#define MGMT__SMD_DEV_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__smd_dev_req__descriptor) \
     }


struct  _Mgmt__SmdDevResp__Device
{
  ProtobufCMessage base;
  /*
   * UUID of blobstore
   */
  char *uuid;
  /*
   * VOS target IDs
   */
  size_t n_tgt_ids;
  int32_t *tgt_ids;
};
#define MGMT__SMD_DEV_RESP__DEVICE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__smd_dev_resp__device__descriptor) \
    , (char *)protobuf_c_empty_string, 0,NULL }


struct  _Mgmt__SmdDevResp
{
  ProtobufCMessage base;
  int32_t status;
  size_t n_devices;
  Mgmt__SmdDevResp__Device **devices;
};
#define MGMT__SMD_DEV_RESP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__smd_dev_resp__descriptor) \
    , 0, 0,NULL }


struct  _Mgmt__SmdPoolReq
{
  ProtobufCMessage base;
};
#define MGMT__SMD_POOL_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__smd_pool_req__descriptor) \
     }


struct  _Mgmt__SmdPoolResp__Pool
{
  ProtobufCMessage base;
  /*
   * UUID of VOS pool
   */
  char *uuid;
  /*
   * VOS target IDs
   */
  size_t n_tgt_ids;
  int32_t *tgt_ids;
  /*
   * SPDK blobs
   */
  size_t n_blobs;
  uint64_t *blobs;
};
#define MGMT__SMD_POOL_RESP__POOL__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__smd_pool_resp__pool__descriptor) \
    , (char *)protobuf_c_empty_string, 0,NULL, 0,NULL }


struct  _Mgmt__SmdPoolResp
{
  ProtobufCMessage base;
  int32_t status;
  size_t n_pools;
  Mgmt__SmdPoolResp__Pool **pools;
};
#define MGMT__SMD_POOL_RESP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__smd_pool_resp__descriptor) \
    , 0, 0,NULL }


/* Mgmt__BioHealthReq methods */
void   mgmt__bio_health_req__init
                     (Mgmt__BioHealthReq         *message);
size_t mgmt__bio_health_req__get_packed_size
                     (const Mgmt__BioHealthReq   *message);
size_t mgmt__bio_health_req__pack
                     (const Mgmt__BioHealthReq   *message,
                      uint8_t             *out);
size_t mgmt__bio_health_req__pack_to_buffer
                     (const Mgmt__BioHealthReq   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__BioHealthReq *
       mgmt__bio_health_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__bio_health_req__free_unpacked
                     (Mgmt__BioHealthReq *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__BioHealthResp methods */
void   mgmt__bio_health_resp__init
                     (Mgmt__BioHealthResp         *message);
size_t mgmt__bio_health_resp__get_packed_size
                     (const Mgmt__BioHealthResp   *message);
size_t mgmt__bio_health_resp__pack
                     (const Mgmt__BioHealthResp   *message,
                      uint8_t             *out);
size_t mgmt__bio_health_resp__pack_to_buffer
                     (const Mgmt__BioHealthResp   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__BioHealthResp *
       mgmt__bio_health_resp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__bio_health_resp__free_unpacked
                     (Mgmt__BioHealthResp *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__SmdDevReq methods */
void   mgmt__smd_dev_req__init
                     (Mgmt__SmdDevReq         *message);
size_t mgmt__smd_dev_req__get_packed_size
                     (const Mgmt__SmdDevReq   *message);
size_t mgmt__smd_dev_req__pack
                     (const Mgmt__SmdDevReq   *message,
                      uint8_t             *out);
size_t mgmt__smd_dev_req__pack_to_buffer
                     (const Mgmt__SmdDevReq   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__SmdDevReq *
       mgmt__smd_dev_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__smd_dev_req__free_unpacked
                     (Mgmt__SmdDevReq *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__SmdDevResp__Device methods */
void   mgmt__smd_dev_resp__device__init
                     (Mgmt__SmdDevResp__Device         *message);
/* Mgmt__SmdDevResp methods */
void   mgmt__smd_dev_resp__init
                     (Mgmt__SmdDevResp         *message);
size_t mgmt__smd_dev_resp__get_packed_size
                     (const Mgmt__SmdDevResp   *message);
size_t mgmt__smd_dev_resp__pack
                     (const Mgmt__SmdDevResp   *message,
                      uint8_t             *out);
size_t mgmt__smd_dev_resp__pack_to_buffer
                     (const Mgmt__SmdDevResp   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__SmdDevResp *
       mgmt__smd_dev_resp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__smd_dev_resp__free_unpacked
                     (Mgmt__SmdDevResp *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__SmdPoolReq methods */
void   mgmt__smd_pool_req__init
                     (Mgmt__SmdPoolReq         *message);
size_t mgmt__smd_pool_req__get_packed_size
                     (const Mgmt__SmdPoolReq   *message);
size_t mgmt__smd_pool_req__pack
                     (const Mgmt__SmdPoolReq   *message,
                      uint8_t             *out);
size_t mgmt__smd_pool_req__pack_to_buffer
                     (const Mgmt__SmdPoolReq   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__SmdPoolReq *
       mgmt__smd_pool_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__smd_pool_req__free_unpacked
                     (Mgmt__SmdPoolReq *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__SmdPoolResp__Pool methods */
void   mgmt__smd_pool_resp__pool__init
                     (Mgmt__SmdPoolResp__Pool         *message);
/* Mgmt__SmdPoolResp methods */
void   mgmt__smd_pool_resp__init
                     (Mgmt__SmdPoolResp         *message);
size_t mgmt__smd_pool_resp__get_packed_size
                     (const Mgmt__SmdPoolResp   *message);
size_t mgmt__smd_pool_resp__pack
                     (const Mgmt__SmdPoolResp   *message,
                      uint8_t             *out);
size_t mgmt__smd_pool_resp__pack_to_buffer
                     (const Mgmt__SmdPoolResp   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__SmdPoolResp *
       mgmt__smd_pool_resp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__smd_pool_resp__free_unpacked
                     (Mgmt__SmdPoolResp *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Mgmt__BioHealthReq_Closure)
                 (const Mgmt__BioHealthReq *message,
                  void *closure_data);
typedef void (*Mgmt__BioHealthResp_Closure)
                 (const Mgmt__BioHealthResp *message,
                  void *closure_data);
typedef void (*Mgmt__SmdDevReq_Closure)
                 (const Mgmt__SmdDevReq *message,
                  void *closure_data);
typedef void (*Mgmt__SmdDevResp__Device_Closure)
                 (const Mgmt__SmdDevResp__Device *message,
                  void *closure_data);
typedef void (*Mgmt__SmdDevResp_Closure)
                 (const Mgmt__SmdDevResp *message,
                  void *closure_data);
typedef void (*Mgmt__SmdPoolReq_Closure)
                 (const Mgmt__SmdPoolReq *message,
                  void *closure_data);
typedef void (*Mgmt__SmdPoolResp__Pool_Closure)
                 (const Mgmt__SmdPoolResp__Pool *message,
                  void *closure_data);
typedef void (*Mgmt__SmdPoolResp_Closure)
                 (const Mgmt__SmdPoolResp *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor mgmt__bio_health_req__descriptor;
extern const ProtobufCMessageDescriptor mgmt__bio_health_resp__descriptor;
extern const ProtobufCMessageDescriptor mgmt__smd_dev_req__descriptor;
extern const ProtobufCMessageDescriptor mgmt__smd_dev_resp__descriptor;
extern const ProtobufCMessageDescriptor mgmt__smd_dev_resp__device__descriptor;
extern const ProtobufCMessageDescriptor mgmt__smd_pool_req__descriptor;
extern const ProtobufCMessageDescriptor mgmt__smd_pool_resp__descriptor;
extern const ProtobufCMessageDescriptor mgmt__smd_pool_resp__pool__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_storage_5fquery_2eproto__INCLUDED */
